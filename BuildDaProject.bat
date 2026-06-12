@echo off

set "buildDir=build"
if exist "%~dp0%buildDir%" (
    echo Deleted previous build folder
    rmdir /s /q "%~dp0%buildDir%"
)

echo.
echo ========================
echo Building project...
echo ========================
cmake -B build -S . -G "Visual Studio 17 2022" -DCMAKE_PREFIX_PATH="C:\Qt\6.11.0\msvc2022_64" || exit /b

echo.
echo ========================
echo Compiling project...
echo ========================
cmake --build build || exit /b
echo.

pause