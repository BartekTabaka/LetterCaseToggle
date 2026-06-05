# CapsLockToggle

A background tool for Windows — select text anywhere and press **CapsLock**
to toggle case without changing the keyboard state.

## Toggle Logic

| State of selected text           | Result          |
|----------------------------------|-----------------|
| At least one lowercase letter    | → ALL CAPS      |
| All uppercase letters (or none)  | → all lowercase |

## Building (Visual Studio / MSVC)

```
mkdir build && cd build
cmake .. -G “Visual Studio 17 2022” -A x64
cmake --build . --config Release
```

Resulting file: `build/bin/Release/CapsLockToggle.exe`

## Building (MinGW / Makefile)

```
mkdir build && cd build
cmake .. -G “MinGW Makefiles”
cmake --build .
```

## Requirements

- Windows 7 or later
- MSVC 2019+ or MinGW with GCC 10+
- No external dependencies

## How it works

1. Run `CapsLockToggle.exe` — an icon will appear in the system tray.
2. Select any text in any application.
3. Press **CapsLock** — the text will be replaced with a version with swapped letters.
4. Right-click the icon → **Close CapsLockToggle** to exit.

## Technical Details

- `WH_KEYBOARD_LL` — intercepts CapsLock at the system level before it reaches the application
- CapsLock is **locked** — does not change the LED status or keyboard state
- The clipboard is preserved and restored after each operation
- Protects against recursion when simulating Ctrl+C / Ctrl+V
- A mutex prevents multiple instances from running
- No UAC — does not require 

## Autostart (optional)

Add a shortcut to `CapsLockToggle.exe` in the following folder:

```
%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup
```
