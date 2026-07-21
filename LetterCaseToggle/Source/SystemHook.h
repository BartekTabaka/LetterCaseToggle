#pragma once
#include <Windows.h>

class KeyboardHook {
public:
	bool Start();
	void Stop();
	bool IsActive() const;
private:
	HHOOK hook = nullptr;
};

extern KeyboardHook *g_KeyboardHook;
extern HWND g_lastExternalForeground;

//Determines if the specified window was created by the current application process.
bool IsOwnProcessWindow(HWND hwnd);

// Returns the best guess for the window the user wants us to act on.
// Never returns windows belonging to this process.
HWND GetTargetWindow();

// Returns the currently focused control within the target window.
// Falls back to the target window if no child control is focused.
HWND GetTargetFocusHwnd(HWND target);

// Attempts to bring the target window to the foreground.
// The operation may fail; callers should verify the actual foreground window.
bool FocusTargetWindow(HWND target);

bool IsTargetWindowActive(HWND target);

// Attempts to copy the current selection directly from the focused control.
// Does not depend on the target window being the OS foreground window.
void CopySelectionFromTarget(HWND target);

// Low-level keyboard hook that intercepts Caps Lock and dispatches handling
// to the Qt event loop. The original event is suppressed.
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);

// Simulates a Ctrl + key keyboard shortcut using SendInput.
void SendCtrlCommand(WORD vk);

// Simulates a real Caps Lock press+release (as injected input, so KeyboardProc
// lets it through instead of swallowing it). Used to fall back to normal Caps
// Lock behaviour when there was no selection to toggle-case.
void SendCapsLockToggle();
