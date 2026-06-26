#pragma once
#include <Windows.h>

static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
static void SendCtrlCommand(WORD vk);
