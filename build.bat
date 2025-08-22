@echo off
echo ========================================
echo Building Ecos del Crepusculo
echo ========================================

:: Check if premake5 exists
if not exist "build\premake5.exe" (
    echo ERROR: premake5.exe not found in build directory
    echo Please ensure premake5.exe is in the build folder
    pause
    exit /b 1
)

:: Get the directory where this script is located
set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

:: Generate project files
echo Generating project files...
cd build
premake5.exe vs2022
if errorlevel 1 (
    echo ERROR: Failed to generate project files
    cd ..
    pause
    exit /b 1
)
cd ..

:: Build the project
echo Building project...
if exist "raylib-quickstart.sln" (
    :: Try to build with MSBuild first
    where msbuild >nul 2>nul
    if %errorlevel% == 0 (
        echo Using MSBuild...
        msbuild raylib-quickstart.sln /p:Configuration=Debug /p:Platform=x64
        if errorlevel 1 (
            echo ERROR: MSBuild failed
            pause
            exit /b 1
        )
    ) else (
        echo MSBuild not found. Please build manually using Visual Studio
        echo Opening solution file...
        start raylib-quickstart.sln
        pause
        exit /b 0
    )
) else (
    echo ERROR: Solution file not found
    pause
    exit /b 1
)

echo ========================================
echo Build completed successfully!
echo ========================================

:: Check if executable was created
if exist "bin\Debug\*.exe" (
    echo Executable created in bin\Debug\
    echo.
    echo Do you want to run the game? (Y/N)
    set /p choice=
    if /i "%choice%"=="Y" (
        echo Running game...
        cd bin\Debug
        for %%f in (*.exe) do (
            echo Starting %%f...
            start %%f
            goto :end
        )
        :end
        cd ..\..
    )
) else (
    echo WARNING: No executable found in bin\Debug\
)

pause