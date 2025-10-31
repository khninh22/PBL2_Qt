@echo off
REM Build Image Downloader

echo ========================================
echo   Building Image Downloader
echo ========================================
echo.

echo Compiling download_image.cpp...
g++ -std=c++17 download_image.cpp -o download_image.exe -lwininet -static

if %ERRORLEVEL% == 0 (
    echo.
    echo ========================================
    echo   BUILD SUCCESS!
    echo ========================================
    echo.
    echo Executable: download_image.exe
    echo.
    echo Usage:
    echo   1. Run without arguments for interactive mode:
    echo      download_image.exe
    echo.
    echo   2. Direct download:
    echo      download_image.exe [URL] [output_filename]
    echo.
    echo Example:
    echo   download_image.exe https://source.unsplash.com/1920x1080/?soccer login_bg.jpg
    echo.
) else (
    echo.
    echo ========================================
    echo   BUILD FAILED!
    echo ========================================
    echo.
    echo Make sure you have MinGW installed and in PATH.
    echo.
)

pause
