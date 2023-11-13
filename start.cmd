@echo off
cls

REM Derleme
gcc -o bank main.c

REM Hata kontrolü
if %errorlevel% neq 0 (
    echo "Derleme hatası."
    pause
    exit /b 1
)

bank.exe

if %errorlevel% neq 0 (
    echo "Program hatası."
    pause
    exit /b 1
)

pause
exit /b 0