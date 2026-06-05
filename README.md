# CapsLockToggle

Narzędzie tła dla Windows — zaznacz tekst gdziekolwiek i naciśnij **CapsLock**,
aby przełączyć wielkość liter bez zmiany stanu klawiatury.

## Logika przełączania

| Stan zaznaczonego tekstu          | Wynik          |
|-----------------------------------|----------------|
| Przynajmniej jedna mała litera    | → WSZYSTKO DUŻE |
| Wszystkie litery duże (lub brak)  | → wszystko małe |

## Budowanie (Visual Studio / MSVC)

```
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Wynikowy plik: `build/bin/Release/CapsLockToggle.exe`

## Budowanie (MinGW / Makefile)

```
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

## Wymagania

- Windows 7 lub nowszy
- MSVC 2019+ lub MinGW z GCC 10+
- Brak zewnętrznych zależności

## Działanie

1. Uruchom `CapsLockToggle.exe` — pojawi się ikona w zasobniku systemowym.
2. Zaznacz dowolny tekst w dowolnej aplikacji.
3. Naciśnij **CapsLock** — tekst zostanie zastąpiony wersją z przestawionymi literami.
4. Kliknij ikonę prawym przyciskiem myszy → **Zamknij CapsLockToggle**, aby zakończyć.

## Szczegóły techniczne

- `WH_KEYBOARD_LL` — przechwytuje CapsLock systemowo zanim dotrze do aplikacji
- CapsLock jest **blokowany** — nie zmienia stanu LED ani stanu klawiatury
- Schowek jest zachowywany i przywracany po każdej operacji
- Chroni przed rekurencją przy symulowaniu Ctrl+C / Ctrl+V
- Mutex zapobiega uruchomieniu wielu instancji
- Brak UAC — nie wymaga uprawnień administratora

## Autostart (opcjonalnie)

Dodaj skrót do `CapsLockToggle.exe` w folderze:

```
%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup
```
