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

echo Compilando versoes: JOGAVEL e DEMO (preenchida)...
echo.

REM Finaliza executavel se estiver rodando para evitar erro de link
taskkill /IM pentaminos.exe /F >nul 2>&1

if not exist build_gui mkdir build_gui
pushd build_gui

set "RAYLIB_VENDOR="
set "raylib="

REM 1) Compila JOGAVEL (sem prefill)
g++ -std=c++17 -I..\include -I"C:\msys64\ucrt64\include" -L"C:\msys64\ucrt64\lib" -O2 -o pentaminos_play.exe ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32
if errorlevel 1 (
    echo Nao compilou jogavel com MSYS2, tentando com vcpkg MinGW...
    g++ -std=c++17 -I..\include -I"c:\temp\vcpkg\installed\x64-mingw-dynamic\include" -L"c:\temp\vcpkg\installed\x64-mingw-dynamic\lib" -O2 -o pentaminos_play.exe ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32
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

REM 2) Compila DEMO (prefill por padrao)
g++ -std=c++17 -DPREFILL_DEFAULT=1 -I..\include -I"%raylib%\include" -L"%raylib%\lib" -O2 -o pentaminos_demo.exe ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32
if errorlevel 1 (
    echo ERRO: falha ao compilar versao DEMO com o mesmo vendor. Tentando alternativo...
    if "%RAYLIB_VENDOR%"=="MSYS2" (
        g++ -std=c++17 -DPREFILL_DEFAULT=1 -I..\include -I"c:\temp\vcpkg\installed\x64-mingw-dynamic\include" -L"c:\temp\vcpkg\installed\x64-mingw-dynamic\lib" -O2 -o pentaminos_demo.exe ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 & set "RAYLIB_VENDOR=VCPKG" & set "raylib=c:\temp\vcpkg\installed\x64-mingw-dynamic"
    ) else (
        g++ -std=c++17 -DPREFILL_DEFAULT=1 -I..\include -I"C:\msys64\ucrt64\include" -L"C:\msys64\ucrt64\lib" -O2 -o pentaminos_demo.exe ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 & set "RAYLIB_VENDOR=MSYS2" & set "raylib=C:\msys64\ucrt64"
    )
    if errorlevel 1 (
        echo ERRO: falha ao compilar versao DEMO tambem nos dois vendors.
        popd & exit /b 1
    )
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
echo Binarios gerados:
echo   - build_gui\pentaminos_play.exe   ^(jogavel^)
echo   - build_gui\pentaminos_demo.exe   ^(preenchido automaticamente^)
echo.
echo Para rodar manualmente:
echo   build_gui\pentaminos_play.exe 6 10
echo   build_gui\pentaminos_demo.exe 6 10
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
