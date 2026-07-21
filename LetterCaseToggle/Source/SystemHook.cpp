#include "SystemHook.h"
#include "App.h"
#include <QMetaObject>

KeyboardHook *g_KeyboardHook = nullptr;
HWND g_lastExternalForeground = nullptr;

bool IsOwnProcessWindow(HWND hwnd)
{
	if (!hwnd)
		return false;

	DWORD windowPid = 0;
	GetWindowThreadProcessId(hwnd, &windowPid);
	return windowPid == GetCurrentProcessId();
}

// Returns the best guess at "the window the user wants us to act on": either
// the last real external window we saw focused, or whatever's foreground now.
// Always excludes our own process, so we never target our own console/windows.
HWND GetTargetWindow()
{
	if (g_lastExternalForeground && IsWindow(g_lastExternalForeground) && !IsOwnProcessWindow(g_lastExternalForeground))
		return g_lastExternalForeground;

	const HWND foreground = GetForegroundWindow();
	if (foreground && !IsOwnProcessWindow(foreground))
		return foreground;

	return nullptr;
}

// Resolves the actually-focused child control within target's own thread
// (e.g. the edit control inside a window), falling back to target itself.
HWND GetTargetFocusHwnd(HWND target)
{
	if (!target)
		return nullptr;

	const DWORD threadId = GetWindowThreadProcessId(target, nullptr);

	GUITHREADINFO info{};
	info.cbSize = sizeof(info);

	if (GetGUIThreadInfo(threadId, &info) && info.hwndFocus)
		return info.hwndFocus;

	return target;
}

// Attempts to bring target to the foreground, working around Windows' normal
// restrictions on stealing focus via the AttachThreadInput trick. Not
// guaranteed to succeed - callers must re-check GetForegroundWindow()
// themselves before relying on it (see HandleCaps).
bool FocusTargetWindow(HWND target)
{
	if (!target)
		return false;

	const HWND foreground = GetForegroundWindow();
	if (foreground == target || IsChild(target, foreground))
		return true;

	const DWORD foregroundThread = foreground ? GetWindowThreadProcessId(foreground, nullptr) : 0;
	const DWORD targetThread = GetWindowThreadProcessId(target, nullptr);
	const DWORD currentThread = GetCurrentThreadId();

	if (foregroundThread)
		AttachThreadInput(currentThread, foregroundThread, TRUE);
	AttachThreadInput(currentThread, targetThread, TRUE);

	if (IsIconic(target))
		ShowWindow(target, SW_RESTORE);

	SetForegroundWindow(target);

	AttachThreadInput(currentThread, targetThread, FALSE);
	if (foregroundThread)
		AttachThreadInput(currentThread, foregroundThread, FALSE);

	const HWND active = GetForegroundWindow();
	return active == target || IsChild(target, active);
}

bool IsTargetWindowActive(HWND target)
{
	if (!target)
		return false;

	const HWND foreground = GetForegroundWindow();
	return foreground == target || IsChild(target, foreground);
}

// Fast path for copying a selection: sent directly to the focused control's
// HWND, so it works regardless of which window currently has OS focus. Many
// standard Win32 controls handle this; custom editors (VS, VS Code, browsers)
// often don't, in which case selected stays empty and HandleCaps falls back
// to a real simulated Ctrl+C via SendCtrlCommand.
void CopySelectionFromTarget(HWND target)
{
	const HWND focus = GetTargetFocusHwnd(target);
	if (focus)
		SendMessage(focus, WM_COPY, 0, 0);
}

#pragma region KeyboardHook Class

bool KeyboardHook::Start()
{
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(nullptr), 0);
	qDebug() << "Successfully created hook";
	return IsActive();
}

void KeyboardHook::Stop()
{
	if (hook) {
		UnhookWindowsHookEx(hook);
		hook = nullptr;
		qDebug() << "Successfully unhooked";
	}
}

bool KeyboardHook::IsActive() const
{
	return hook != nullptr;
}

#pragma endregion

static bool g_capsPhysicallyDown = false;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION) {
		auto *kb = (KBDLLHOOKSTRUCT *)lParam; // Event parameters
		const bool isCapsLock = (kb->vkCode == VK_CAPITAL);

		if (kb->flags & LLKHF_INJECTED)
			return CallNextHookEx(nullptr, nCode, wParam, lParam);

		const HWND foreground = GetForegroundWindow();
		if (foreground && !IsOwnProcessWindow(foreground))
			g_lastExternalForeground = foreground;

		if (isCapsLock && (wParam == WM_KEYDOWN || wParam == WM_KEYUP)) {
			if (wParam == WM_KEYDOWN) {
				if (!g_capsPhysicallyDown) {
					g_capsPhysicallyDown = true;
					if (g_App) {
						// Dispatch to Qt event loop thread
						QMetaObject::invokeMethod(
							g_App,
							[=]() { g_App->HandleCaps(); },
							Qt::QueuedConnection
						);
					}
				}
				// else: autorepeat while held down - ignore, already handled on the first down-edge
			}
			else { // WM_KEYUP
				g_capsPhysicallyDown = false;
			}

			// Don't forward the event
			return 1;
		}
	}

	return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void SendCtrlCommand(WORD vk)
{
	qDebug() << "SendCtrlCommand('" << vk << "')";
	INPUT inputs[4] = {};

	// CTRL press
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_LCONTROL;

	// Key press
	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = vk;

	// Key release
	inputs[2].type = INPUT_KEYBOARD;
	inputs[2].ki.wVk = vk;
	inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

	// CTRL release
	inputs[3].type = INPUT_KEYBOARD;
	inputs[3].ki.wVk = VK_LCONTROL;
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	// NOTE: we deliberately do NOT suspend the keyboard hook around SendInput. 
	// KeyboardProc already ignores injected input (LLKHF_INJECTED check above),
	// so suspending here would be unnecessary - it could also open a race window: 
	// if a *real* Caps Lock press landed during the brief unhooked period, it 
	// would go straight to Windows and toggle the real Caps Lock state. Since the 
	// hook (once reinstalled) would swallow every subsequent Caps Lock press, 
	// that stray toggle could never be undone by pressing Caps Lock again - only 
	// closing the app (which removes the hook) would let a real press through to 
	// fix it.
	SendInput(4, inputs, sizeof(INPUT));
}

void SendCapsLockToggle()
{
	INPUT inputs[2] = {};

	// Caps Lock press
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_CAPITAL;

	// Caps Lock release
	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_CAPITAL;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	// This goes out through SendInput, so KeyboardProc sees LLKHF_INJECTED and
	// passes it straight to CallNextHookEx instead of swallowing it - that's
	// what lets it actually reach Windows and toggle the real Caps Lock state,
	// restoring normal typing-case behaviour for whoever currently has focus.
	SendInput(2, inputs, sizeof(INPUT));
}
