#!/bin/bash
# Standoff 2 External Tool Build Script for Linux with MinGW-w64
# This script builds Windows executables using MinGW-w64 cross-compiler

set -e  # Exit on error

echo "=========================================="
echo "Standoff 2 External Tool Build Script"
echo "=========================================="

# Check for MinGW-w64
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "ERROR: MinGW-w64 cross-compiler not found!"
    echo "Please install MinGW-w64:"
    echo ""
    echo "Ubuntu/Debian: sudo apt-get install mingw-w64"
    echo "Arch Linux: sudo pacman -S mingw-w64-gcc"
    echo "Fedora: sudo dnf install mingw64-gcc"
    echo ""
    exit 1
fi

# Set compiler
CXX=x86_64-w64-mingw32-g++
CXXFLAGS="-std=c++17 -Wall -Wextra -O2 -I/usr/x86_64-w64-mingw32/include"

# Create output directory
mkdir -p Lorer

echo "Compiler: $($CXX --version | head -n1)"
echo ""

# Build Core DLL
echo "=========================================="
echo "Building Standoff2Core.dll"
echo "=========================================="

$CXX -c src/core/MemoryManager.cpp -o src/core/MemoryManager.o $CXXFLAGS
$CXX -shared src/core/MemoryManager.o -o Lorer/Standoff2Core.dll -static-libgcc -static-libstdc++ -lpsapi

# Build Loader EXE
echo ""
echo "=========================================="
echo "Building lorer.exe"
echo "=========================================="

$CXX -c src/loader/Loader.cpp -o src/loader/Loader.o $CXXFLAGS -mwindows
$CXX src/loader/Loader.o -o Lorer/lorer.exe -static-libgcc -static-libstdc++ -ladvapi32 -mwindows

# Create README
echo ""
echo "Creating README.txt..."

cat > Lorer/README.txt << 'EOF'
Standoff 2 External Tool v1.0.0
=================================

USAGE:
1. Install BlueStacks 5 on Windows
2. Install and run Standoff 2 in BlueStacks
3. Copy these files to your Windows machine:
   - lorer.exe
   - Standoff2Core.dll
4. Run lorer.exe as Administrator
5. The tool will automatically inject into HD-Player.exe

REQUIREMENTS:
- Windows 10/11 64-bit
- BlueStacks 5
- Standoff 2 v0.37.0 (check offsets for other versions)

BUILD INSTRUCTIONS (Linux):
----------------------------
1. Install MinGW-w64: sudo apt install mingw-w64
2. Run: chmod +x build.sh && ./build.sh
3. Windows binaries will be in ./Lorer/

BUILD INSTRUCTIONS (Windows):
------------------------------
1. Install MinGW-w64: https://www.mingw-w64.org/downloads/
2. Run build.bat as Administrator
3. Binaries will be in Lorer/

TROUBLESHOOTING:
- If injection fails, run as Administrator
- Ensure BlueStacks process is named HD-Player.exe
- Check that offsets are updated for your game version
- Disable antivirus temporarily (may flag as false positive)

OFFSETS UPDATE:
---------------
To find correct offsets for your game version:
1. Use Cheat Engine to attach to HD-Player.exe
2. Find PlayerManager static pointer
3. Inspect player structure for health, position, etc.
4. Update src/core/offsets.h

DISCLAIMER:
-----------
This tool is for educational purposes only.
Use at your own risk. Game modifications may violate Terms of Service.

EOF

# Create run script for Windows
cat > Lorer/run.bat << 'EOF'
@echo off
REM Run Standoff 2 External Tool
REM Make sure BlueStacks is running

echo Checking for BlueStacks process...
tasklist /FI "IMAGENAME eq HD-Player.exe" 2>NUL | find /I /N "HD-Player.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo BlueStacks found. Proceeding with injection...
    timeout /t 2 /nobreak >nul
    lorer.exe
) else (
    echo ERROR: BlueStacks 5 not found!
    echo Please start BlueStacks first, then run this script.
    pause
)

EOF

# Make files executable (when copied to Windows)
chmod +x Lorer/lorer.exe 2>/dev/null || true

echo ""
echo "=========================================="
echo "Build completed successfully!"
echo "=========================================="
echo "Output files in ./Lorer/:"
echo "- lorer.exe"
echo "- Standoff2Core.dll"
echo "- README.txt"
echo "- run.bat"
echo ""
echo "Next steps:"
echo "1. Copy files to Windows machine"
echo "2. Ensure BlueStacks 5 is running"
echo "3. Run lorer.exe as Administrator"
echo ""
