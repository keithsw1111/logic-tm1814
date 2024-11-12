@echo off
REM Set the build directory
set BUILD_DIR=build

REM Create the build directory if it doesn't exist
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

REM Change to the build directory
cd %BUILD_DIR%

REM Run CMake to configure the project
cmake ..

REM Build the project
cmake --build . --config Release

REM Change back to the original directory
cd ..