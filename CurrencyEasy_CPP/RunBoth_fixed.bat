@echo off
cd /d "%~dp0"

set "BIN=%~dp0bin\x64\Debug"
if not exist "%BIN%\Currency.Service.exe" set "BIN=%~dp0bin\x64\Release"

if not exist "%BIN%\Currency.Service.exe" goto missing
if not exist "%BIN%\Currency.Web.exe" goto missing

start "Currency Service" "%BIN%\Currency.Service.exe"
timeout /t 2 /nobreak >nul
start "Currency Web" "%BIN%\Currency.Web.exe"
timeout /t 2 /nobreak >nul
start "" "http://127.0.0.1:8080/"
exit /b 0

:missing
echo Programs not found. Build Solution in Visual Studio (Ctrl+Shift+B).
pause
exit /b 1
