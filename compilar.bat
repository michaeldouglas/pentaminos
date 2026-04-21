@echo off
chcp 65001 >nul
cls

echo =============================================
echo    PENTAMINOS - Compilador GUI
echo =============================================
echo.

REM Verifica se g++ esta disponivel
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ERRO: g++ nao encontrado no PATH
    echo Solucao: Instale MinGW conforme INSTALAR_FERRAMENTAS.md
    exit /b 1
)

echo Compilando versao GUI...
echo.

REM Finaliza executavel se estiver rodando para evitar erro de link
taskkill /IM pentaminos.exe /F >nul 2>&1

if not exist build_gui mkdir build_gui
pushd build_gui

set "RAYLIB_VENDOR="
set "raylib="

REM Tentativa 1: MSYS2 ucrt64
g++ -std=c++17 -I..\include -I"C:\msys64\ucrt64\include" -L"C:\msys64\ucrt64\lib" -O2 -o pentaminos ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32

if errorlevel 1 (
    echo Nao compilou com MSYS2, tentando com vcpkg MinGW...
    REM Tentativa 2: vcpkg MinGW
    g++ -std=c++17 -I..\include -I"c:\temp\vcpkg\installed\x64-mingw-dynamic\include" -L"c:\temp\vcpkg\installed\x64-mingw-dynamic\lib" -O2 -o pentaminos ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32
    if errorlevel 1 (
        echo ERRO na compilacao com MSYS2 e com vcpkg.
        echo Instale Raylib e tente novamente:
        echo   - vcpkg: vcpkg install raylib:x64-mingw-dynamic
        echo   - MSYS2 ucrt64: pacman -S mingw-w64-ucrt-x86_64-raylib
        popd
        exit /b 1
    ) else (
        set "RAYLIB_VENDOR=VCPKG"
        set "raylib=c:\temp\vcpkg\installed\x64-mingw-dynamic"
    )
) else (
    set "RAYLIB_VENDOR=MSYS2"
    set "raylib=C:\msys64\ucrt64"
)

popd

echo.
echo =============================================
echo    SUCESSO! Compilacao concluida
echo =============================================
echo.
REM Copia a DLL do raylib para a pasta do executavel, se existir
if "%RAYLIB_VENDOR%"=="VCPKG" (
    if exist "%raylib%\bin\raylib.dll" copy /Y "%raylib%\bin\raylib.dll" "build_gui\raylib.dll" >nul
) else if "%RAYLIB_VENDOR%"=="MSYS2" (
    if exist "%raylib%\bin\raylib.dll" copy /Y "%raylib%\bin\raylib.dll" "build_gui\raylib.dll" >nul
)
echo Executavel: build_gui\pentaminos.exe
echo Iniciando com dimensoes padrao 6x10...
build_gui\pentaminos.exe 6 10
exit /b %errorlevel%

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
echo Instalando Raylib (pode demorar)...
.\vcpkg install raylib:x64-mingw-dynamic

if errorlevel 1 (
    echo ERRO na instalacao
    exit /b 1
)

echo Raylib instalado com sucesso!
cd /d "%~dp0"
exit /b 0
