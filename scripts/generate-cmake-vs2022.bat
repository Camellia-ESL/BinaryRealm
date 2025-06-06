@echo off
setlocal
set BUILD_DIR=build
set ASSETS_DIR=assets
set TARGET_ASSETS_DIR=%BUILD_DIR%\assets

REM Goes in the main directory
cd ..

REM Create build directory if it doesn't exist

rem CMake configure and build
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
    mkdir %TARGET_ASSETS_DIR%
)

REM Copies assets in the build folder
robocopy "%ASSETS_DIR%" "%TARGET_ASSETS_DIR%" /E /MIR

rem Check for assets copy success 
if %ERRORLEVEL% GEQ 8 (
    echo [ERROR] Failed to copy assets.
    pause
    exit /b 1
)

REM Change to build directory
cd build

REM Run CMake for Visual Studio 2022 (x64)
cmake .. -G "Visual Studio 17 2022" -A x64

echo Build complete and assets copied to %TARGET_ASSETS_DIR%
pause
