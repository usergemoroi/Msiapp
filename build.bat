@echo off
REM Standoff 2 External Tool Build Script for MinGW-w64
REM Make sure MinGW-w64 is installed and in PATH

echo =======================================
echo Building Standoff 2 External Tool
echo =======================================

REM Get MinGW path from registry or common locations
set MINGW_PATH=
for /f "tokens=3" %%a in ('reg query "HKLM\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\" /v ProgramW6432 2^>nul ^| findstr /i "ProgramW6432"') do set PROGRAMS=%%a

REM Check common MinGW installations
if exist "C:\msys64\mingw64\bin\g++.exe" (
    set MINGW_PATH=C:\msys64\mingw64\bin
) else if exist "C:\mingw64\bin\g++.exe" (
    set MINGW_PATH=C:\mingw64\bin
) else if exist "%PROGRAMFILES%\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" (
    set MINGW_PATH=%PROGRAMFILES%\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin
)

if "%~1"=="--mingw-path" if not "%~2"=="" set MINGW_PATH=%~2

REM Add MinGW to PATH if found
if not "%MINGW_PATH%"=="" (
    set PATH=%MINGW_PATH%;%PATH%
    echo Found MinGW at: %MINGW_PATH%
) else (
    echo WARNING: MinGW path not found, trying system PATH...
)

REM Check if g++ is available
where g++ >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: g++ not found in PATH!
    echo Please install MinGW-w64 from: https://www.mingw-w64.org/downloads/
    echo Or run: build.bat --mingw-path "C:\path\to\mingw64\bin"
    pause
    exit /b 1
)

echo.
echo Compiler: 
g++ --version | findstr /i "g++"

REM Create output directory
if not exist "Lorer" mkdir Lorer

REM Build Core DLL
echo.
echo =======================================
echo Building Standoff2Core.dll
echo =======================================
g++ -c src\core\MemoryManager.cpp -o src\core\MemoryManager.o -std=c++17 -Wall -O2
if %errorlevel% neq 0 (
    echo ERROR: Failed to compile MemoryManager.cpp
    pause
    exit /b 1
)

g++ -shared src\core\MemoryManager.o -o Lorer\Standoff2Core.dll -static-libgcc -static-libstdc++ -lpsapi
if %errorlevel% neq 0 (
    echo ERROR: Failed to link Standoff2Core.dll
    pause
    exit /b 1
)

echo SUCCESS: Standoff2Core.dll built!

REM Build Loader EXE
echo.
echo =======================================
echo Building lorer.exe
echo =======================================
g++ -c src\loader\Loader.cpp -o src\loader\Loader.o -std=c++17 -Wall -O2 -mwindows
if %errorlevel% neq 0 (
    echo ERROR: Failed to compile Loader.cpp
    pause
    exit /b 1
)

g++ src\loader\Loader.o -o Lorer\lorer.exe -static-libgcc -static-libstdc++ -ladvapi32 -mwindows
if %errorlevel% neq 0 (
    echo ERROR: Failed to link lorer.exe
    pause
    exit /b 1
)

echo SUCCESS: lorer.exe built!

REM Create README.txt
echo.
echo Creating README.txt...
(
echo Standoff 2 External Tool v1.0.0
echo =================================
echo.
echo USAGE:
echo 1. Install BlueStacks 5
echo 2. Install and run Standoff 2 in BlueStacks
echo 3. Run lorer.exe as Administrator
echo 4. The tool will automatically inject into HD-Player.exe
echo.
echo REQUIREMENTS:
echo - Windows 10/11 64-bit
echo - BlueStacks 5
echo - MinGW-w64 (for building from source)
echo.
echo BUILD INSTRUCTIONS:
echo 1. Install MinGW-w64 from https://www.mingw-w64.org/downloads/
echo 2. Add MinGW to PATH or specify path in build.bat
echo 3. Run build.bat
echo.
echo TROUBLESHOOTING:
echo - If injection fails, run as Administrator
echo - Ensure BlueStacks process is named HD-Player.exe
echo - Check that offsets are updated for your game version
echo.
echo OFFSETS UPDATE:
echo Edit src\core\offsets.h with correct values for your game version
echo Current version: Standoff 2 v0.37.0
echo.
) > Lorer\README.txt

echo.
echo =======================================
echo Build completed successfully!
echo =======================================
echo Output files:
echo - Lorer\lorer.exe
echo - Lorer\Standoff2Core.dll
echo - Lorer\README.txt
echo.
echo Press any key to continue...
pause >nul