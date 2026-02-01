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

