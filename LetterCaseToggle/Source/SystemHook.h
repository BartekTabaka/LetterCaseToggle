#pragma once
#include <Windows.h>

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
void SendCtrlCommand(WORD vk);
