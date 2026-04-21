@echo off
chcp 65001 >nul
cls

echo =============================================
echo    PENTAMINOS - Interface Grafica (GUI)
echo =============================================
echo.
echo 1 - Compilar e Rodar (GUI)
echo 2 - Instalar Raylib
echo 3 - Ver Status
echo 4 - Sair
echo.
set /p choice="Escolha uma opcao (1-4): "

if "%choice%"=="1" (
    call :compilar_e_rodar
) else if "%choice%"=="2" (
    call :instalar_raylib
) else if "%choice%"=="3" (
    call :ver_status
) else if "%choice%"=="4" (
    exit /b 0
) else (
    echo Opcao invalida!
    pause
    goto :eof
)

pause
goto :eof

:compilar_e_rodar
echo.
echo === Compilando Interface Grafica ===
echo.

REM Finaliza executavel se estiver rodando para evitar erro de link
taskkill /IM pentaminos.exe /F >nul 2>&1

REM Detecta Raylib (vcpkg MinGW ou MSYS2 UCRT64)
set "raylib_vcpkg=c:\temp\vcpkg\installed\x64-mingw-dynamic"
set "raylib_msys=C:\msys64\ucrt64"
set "RAYLIB_VENDOR="

if exist "%raylib_vcpkg%\include\raylib.h" (
    set "raylib=%raylib_vcpkg%"
    set "RAYLIB_VENDOR=VCPKG"
) else (
    if exist "%raylib_msys%\include\raylib.h" (
        set "raylib=%raylib_msys%"
        set "RAYLIB_VENDOR=MSYS2"
    ) else (
        echo ERRO: Raylib nao instalado!
        echo Execute opcao 2 para instalar (vcpkg) ou instale via MSYS2: pacman -S mingw-w64-ucrt-x86_64-raylib
        exit /b 1
    )
)

if not exist build_gui mkdir build_gui
cd build_gui

g++ -std=c++17 -I..\include -I"%raylib%\include" -L"%raylib%\lib" -O2 ^
    -o pentaminos ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp ^
    ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp ^
    -lraylib -lopengl32 -lwinmm -luser32 -lgdi32

if errorlevel 1 (
    echo.
    echo ERRO na compilacao!
    cd ..
    exit /b 1
)

cd ..

echo Sucesso! Executando...
REM Copia a DLL do raylib para a pasta do executavel, se existir
if "%RAYLIB_VENDOR%"=="VCPKG" (
    if exist "%raylib%\bin\raylib.dll" copy /Y "%raylib%\bin\raylib.dll" "build_gui\raylib.dll" >nul
) else if "%RAYLIB_VENDOR%"=="MSYS2" (
    if exist "%raylib%\bin\raylib.dll" copy /Y "%raylib%\bin\raylib.dll" "build_gui\raylib.dll" >nul
)
build_gui\pentaminos.exe
goto :eof

:instalar_raylib
echo.
echo === Instalando Raylib ===
echo.
echo Isto instalara vcpkg e Raylib (~10 minutos, ~2GB)
echo.

if not exist c:\temp mkdir c:\temp
cd /d c:\temp

if not exist vcpkg (
    echo Clonando vcpkg...
    git clone https://github.com/Microsoft/vcpkg.git
    if errorlevel 1 (
        echo ERRO: Git nao encontrado
        exit /b 1
    )
)

cd vcpkg
echo Bootstrapando vcpkg...
call bootstrap-vcpkg.bat

echo.
echo Instalando Raylib (pode demorar MUITO)...
.\vcpkg install raylib:x64-mingw-dynamic

if errorlevel 1 (
    echo ERRO na instalacao
    exit /b 1
)

echo Raylib instalado com sucesso!
cd /d "%~dp0"
goto :eof

:ver_status
echo.
echo === STATUS DO PROJETO ===
echo.
if exist main.cpp echo   [OK] main.cpp
if exist src\Board.cpp echo   [OK] src\Board.cpp
if exist include\Board.h echo   [OK] include\Board.h

echo.
g++ --version 2>nul | findstr /R "^" >nul && echo   [OK] g++ instalado || echo   [FALTA] g++

if exist "c:\temp\vcpkg\installed\x64-mingw-dynamic\include\raylib.h" (
    echo   [OK] Raylib instalado
) else (
    echo   [FALTA] Raylib
)

if exist build_gui\pentaminos.exe echo   [OK] Compilado e pronto!

echo.
goto :eof
