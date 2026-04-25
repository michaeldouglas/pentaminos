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

echo Compilando versao unica: GUI com opcoes (resolver/jogar)...
echo.

REM Finaliza executavel se estiver rodando para evitar erro de link
taskkill /IM pentaminos_play.exe /F >nul 2>&1

if not exist build_gui mkdir build_gui
pushd build_gui

set "RAYLIB_VENDOR="
set "raylib="
set "GXX_PATH="
set "MINGW_BIN="

for %%I in (g++.exe) do set "GXX_PATH=%%~$PATH:I"
if defined GXX_PATH (
    for %%I in ("%GXX_PATH%") do set "MINGW_BIN=%%~dpI"
)

REM 1) Compila JOGAVEL (sem prefill)
g++ -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"C:\msys64\ucrt64\include" -L"C:\msys64\ucrt64\lib" -O2 -o pentaminos_play.exe ..\main.cpp ..\src\board\Board.cpp ..\src\piece\Piece.cpp ..\src\state\State.cpp ..\src\graph\GraphSolver.cpp ..\src\avl\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32
if errorlevel 1 (
    echo Nao compilou jogavel com MSYS2, tentando com vcpkg MinGW...
    g++ -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"c:\temp\vcpkg\installed\x64-mingw-dynamic\include" -L"c:\temp\vcpkg\installed\x64-mingw-dynamic\lib" -O2 -o pentaminos_play.exe ..\main.cpp ..\src\board\Board.cpp ..\src\piece\Piece.cpp ..\src\state\State.cpp ..\src\graph\GraphSolver.cpp ..\src\avl\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32
    if errorlevel 1 (
        echo ERRO: falha ao compilar versao JOGAVEL com MSYS2 e vcpkg.
        popd & exit /b 1
    ) else (
        set "RAYLIB_VENDOR=VCPKG"
        set "raylib=c:\temp\vcpkg\installed\x64-mingw-dynamic"
    )
) else (
    set "RAYLIB_VENDOR=MSYS2"
    set "raylib=C:\msys64\ucrt64"
)

REM (Removido) Compilacao separada DEMO. Agora a GUI unica tem opcoes internas.

popd

echo.
echo =============================================
echo    SUCESSO! Compilacao concluida
echo =============================================
echo.
REM Copia a DLL do raylib para a pasta do executavel, se existir
if "%RAYLIB_VENDOR%"=="VCPKG" (
    if exist "%raylib%\bin\*.dll" copy /Y "%raylib%\bin\*.dll" "build_gui\" >nul
) else if "%RAYLIB_VENDOR%"=="MSYS2" (
    if exist "%raylib%\bin\raylib.dll" copy /Y "%raylib%\bin\raylib.dll" "build_gui\raylib.dll" >nul
)

REM Copia as DLLs do runtime MinGW usadas por builds dinamicos.
REM Sem elas, o jogo abre no seu PC, mas falha em maquinas sem MinGW/MSYS2.
if defined MINGW_BIN (
    if exist "%MINGW_BIN%libstdc++-6.dll" copy /Y "%MINGW_BIN%libstdc++-6.dll" "build_gui\" >nul
    if exist "%MINGW_BIN%libwinpthread-1.dll" copy /Y "%MINGW_BIN%libwinpthread-1.dll" "build_gui\" >nul
    if exist "%MINGW_BIN%libgcc_s_seh-1.dll" copy /Y "%MINGW_BIN%libgcc_s_seh-1.dll" "build_gui\" >nul
    if exist "%MINGW_BIN%libgcc_s_sjlj-1.dll" copy /Y "%MINGW_BIN%libgcc_s_sjlj-1.dll" "build_gui\" >nul
    if exist "%MINGW_BIN%libgcc_s_dw2-1.dll" copy /Y "%MINGW_BIN%libgcc_s_dw2-1.dll" "build_gui\" >nul
)

echo Binarios gerados:
echo   - build_gui\pentaminos_play.exe   ^(GUI: jogar e resolver^)
echo   - DLLs necessarias copiadas para build_gui\
echo.
echo Para rodar manualmente:
echo   build_gui\pentaminos_play.exe 6 10
echo.
echo Observacao: sua organizacao pode bloquear a execucao ^(Device Guard/WDAC^).
echo Se ocorrer bloqueio, execute apenas em ambiente permitido ou WSLg.
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
