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

// Low-level keyboard hook that intercepts Caps Lock and dispatches handling
// to the Qt event loop. The original event is suppressed.
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);

// Simulates a Ctrl + key keyboard shortcut using SendInput.
void SendCtrlCommand(WORD vk);
