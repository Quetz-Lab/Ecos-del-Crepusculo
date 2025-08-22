@echo off
echo ========================================
echo Cleaning build files...
echo ========================================

:: Remove build files
if exist "build\build_files" (
    echo Removing build_files directory...
    rmdir /s /q "build\build_files"
)

:: Remove bin directory
if exist "bin" (
    echo Removing bin directory...
    rmdir /s /q "bin"
)

:: Remove solution and project files
if exist "raylib-quickstart.sln" (
    echo Removing solution file...
    del "raylib-quickstart.sln"
)

:: Remove makefiles
if exist "Makefile" (
    echo Removing Makefile...
    del "Makefile"
)

:: Remove Visual Studio files
if exist ".vs" (
    echo Removing .vs directory...
    rmdir /s /q ".vs"
)

:: Remove object files
for /r %%i in (*.obj) do del "%%i"
for /r %%i in (*.o) do del "%%i"

echo ========================================
echo Clean completed!
echo ========================================
pause