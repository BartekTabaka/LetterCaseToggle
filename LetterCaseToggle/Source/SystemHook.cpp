#include "SystemHook.h"
#include "App.h"
#include <QMetaObject>

#pragma region KeyboardHook Class

bool KeyboardHook::Start()
{
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(nullptr), 0);

	// Check if the hook was successfully installed
	return IsActive();
}

void KeyboardHook::Stop()
{
	if (hook) {
		UnhookWindowsHookEx(hook);
		hook = nullptr;
	}
}

bool KeyboardHook::IsActive() const
{
	return hook != nullptr;
}

#pragma endregion

LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION) {
		auto* kb = (KBDLLHOOKSTRUCT*)lParam; // Event parameters
		if (kb->vkCode == VK_CAPITAL && wParam == WM_KEYDOWN) {
			if (g_App) {
				// Dispatch to Qt event loop thread
				QMetaObject::invokeMethod(g_App, "HandleCaps", Qt::QueuedConnection);
			}

			// Don't forward the event
			return 1;
		}
	}

	return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void SendCtrlCommand(WORD vk)
{
	INPUT inputs[4] = {};

	// CTRL press
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_CONTROL;

	// Key press
	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = vk;

	// Key release
	inputs[2].type = INPUT_KEYBOARD;
	inputs[2].ki.wVk = vk;
	inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

	// CTRL release
	inputs[3].type = INPUT_KEYBOARD;
	inputs[3].ki.wVk = VK_CONTROL;
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(4, inputs, sizeof(INPUT));
}
