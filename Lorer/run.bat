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

