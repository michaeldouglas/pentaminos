@echo off
setlocal
if exist "%SystemRoot%\System32\chcp.com" "%SystemRoot%\System32\chcp.com" 65001 >nul
cls

echo =============================================
echo    PENTAMINOS - Compilador GUI
echo =============================================
echo.

if exist "C:\msys64\ucrt64\bin\g++.exe" set "PATH=C:\msys64\ucrt64\bin;%PATH%"
if exist "c:\temp\vcpkg\installed\x64-mingw-dynamic\bin\g++.exe" set "PATH=c:\temp\vcpkg\installed\x64-mingw-dynamic\bin;%PATH%"

REM Verifica se g++ esta disponivel
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ERRO: g++ nao encontrado no PATH
    echo Solucao: Instale MinGW conforme INSTALAR_FERRAMENTAS.md
    echo.
    pause
    exit /b 1
)

echo Compilando versao unica: GUI com opcoes (resolver/jogar)...
echo.

REM Finaliza executavel se estiver rodando para evitar erro de link
taskkill /IM pentaminos_play.exe /F >nul 2>&1
taskkill /IM pentaminos_demo.exe /F >nul 2>&1

if not exist build_gui mkdir build_gui
set "LOG=%CD%\build_gui\compilacao.log"
set "LOG_VCPKG=%CD%\build_gui\compilacao_vcpkg.log"
set "LOG_DEMO=%CD%\build_gui\compilacao_demo.log"
set "LOG_DEMO_VCPKG=%CD%\build_gui\compilacao_demo_vcpkg.log"
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
echo Comando MSYS2/UCRT64: > "%LOG%"
echo g++ -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"C:\msys64\ucrt64\include" -L"C:\msys64\ucrt64\lib" -O2 -o pentaminos_play.exe ... -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 >> "%LOG%"
echo. >> "%LOG%"
g++ -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"C:\msys64\ucrt64\include" -L"C:\msys64\ucrt64\lib" -O2 -o pentaminos_play.exe ..\main.cpp ..\src\board\Board.cpp ..\src\piece\Piece.cpp ..\src\state\State.cpp ..\src\graph\GraphSolver.cpp ..\src\avl\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 >> "%LOG%" 2>&1
if errorlevel 1 (
    echo Nao compilou jogavel com MSYS2, tentando com vcpkg MinGW...
    echo Comando vcpkg MinGW: > "%LOG_VCPKG%"
    echo g++ -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"c:\temp\vcpkg\installed\x64-mingw-dynamic\include" -L"c:\temp\vcpkg\installed\x64-mingw-dynamic\lib" -O2 -o pentaminos_play.exe ... -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 >> "%LOG_VCPKG%"
    echo. >> "%LOG_VCPKG%"
    g++ -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"c:\temp\vcpkg\installed\x64-mingw-dynamic\include" -L"c:\temp\vcpkg\installed\x64-mingw-dynamic\lib" -O2 -o pentaminos_play.exe ..\main.cpp ..\src\board\Board.cpp ..\src\piece\Piece.cpp ..\src\state\State.cpp ..\src\graph\GraphSolver.cpp ..\src\avl\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 >> "%LOG_VCPKG%" 2>&1
    if errorlevel 1 (
        echo ERRO: falha ao compilar versao JOGAVEL com MSYS2 e vcpkg.
        echo.
        echo ===== LOG MSYS2/UCRT64 =====
        type "%LOG%"
        echo.
        echo ===== LOG VCPKG =====
        type "%LOG_VCPKG%"
        echo.
        echo Logs salvos em:
        echo   %LOG%
        echo   %LOG_VCPKG%
        echo.
        popd
        pause
        exit /b 1
    ) else (
        set "RAYLIB_VENDOR=VCPKG"
        set "raylib=c:\temp\vcpkg\installed\x64-mingw-dynamic"
    )
) else (
    set "RAYLIB_VENDOR=MSYS2"
    set "raylib=C:\msys64\ucrt64"
)

REM 2) Compila DEMO (abre com o tabuleiro preenchido por padrao)
if "%RAYLIB_VENDOR%"=="MSYS2" (
    echo Comando DEMO MSYS2/UCRT64: > "%LOG_DEMO%"
    echo g++ -DPREFILL_DEFAULT -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"C:\msys64\ucrt64\include" -L"C:\msys64\ucrt64\lib" -O2 -o pentaminos_demo.exe ... -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 >> "%LOG_DEMO%"
    echo. >> "%LOG_DEMO%"
    g++ -DPREFILL_DEFAULT -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"C:\msys64\ucrt64\include" -L"C:\msys64\ucrt64\lib" -O2 -o pentaminos_demo.exe ..\main.cpp ..\src\board\Board.cpp ..\src\piece\Piece.cpp ..\src\state\State.cpp ..\src\graph\GraphSolver.cpp ..\src\avl\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 >> "%LOG_DEMO%" 2>&1
) else (
    echo Comando DEMO vcpkg MinGW: > "%LOG_DEMO_VCPKG%"
    echo g++ -DPREFILL_DEFAULT -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"c:\temp\vcpkg\installed\x64-mingw-dynamic\include" -L"c:\temp\vcpkg\installed\x64-mingw-dynamic\lib" -O2 -o pentaminos_demo.exe ... -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 >> "%LOG_DEMO_VCPKG%"
    echo. >> "%LOG_DEMO_VCPKG%"
    g++ -DPREFILL_DEFAULT -std=c++17 -I..\src\avl -I..\src\board -I..\src\graph -I..\src\piece -I..\src\state -I"c:\temp\vcpkg\installed\x64-mingw-dynamic\include" -L"c:\temp\vcpkg\installed\x64-mingw-dynamic\lib" -O2 -o pentaminos_demo.exe ..\main.cpp ..\src\board\Board.cpp ..\src\piece\Piece.cpp ..\src\state\State.cpp ..\src\graph\GraphSolver.cpp ..\src\avl\AVLTree.cpp -lraylib -lopengl32 -lwinmm -luser32 -lgdi32 >> "%LOG_DEMO_VCPKG%" 2>&1
)
if errorlevel 1 (
    echo ERRO: falha ao compilar versao DEMO.
    echo.
    echo ===== LOG DEMO MSYS2/UCRT64 =====
    if exist "%LOG_DEMO%" type "%LOG_DEMO%"
    echo.
    echo ===== LOG DEMO VCPKG =====
    if exist "%LOG_DEMO_VCPKG%" type "%LOG_DEMO_VCPKG%"
    echo.
    echo Logs salvos em:
    echo   %LOG_DEMO%
    echo   %LOG_DEMO_VCPKG%
    echo.
    popd
    pause
    exit /b 1
)

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
    if exist "%raylib%\bin\*raylib*.dll" copy /Y "%raylib%\bin\*raylib*.dll" "build_gui\" >nul
    if exist "%raylib%\bin\glfw3.dll" copy /Y "%raylib%\bin\glfw3.dll" "build_gui\" >nul
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

REM Detecta dependencias das DLLs ja copiadas e copia automaticamente
REM outras bibliotecas necessarias, como glfw3.dll.
if defined MINGW_BIN (
    if exist "%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" (
        set "DLL_TARGET_DIR=%~dp0build_gui"
        set "DLL_SEARCH_DIRS=%raylib%\bin;%MINGW_BIN%;C:\msys64\ucrt64\bin;c:\temp\vcpkg\installed\x64-mingw-dynamic\bin"
        set "DLL_OBJDUMP=%MINGW_BIN%objdump.exe"
        set "DLL_ZIP=%~dp0build_gui\jogo.zip"
        "%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -File "%~dp0copiar_dlls.ps1"
        if errorlevel 1 (
            echo AVISO: ocorreu um erro ao copiar DLLs automaticamente.
            echo Execute manualmente: powershell -NoProfile -ExecutionPolicy Bypass -File copiar_dlls.ps1 -TargetDir build_gui -SearchDirs C:\msys64\ucrt64\bin -Objdump C:\msys64\ucrt64\bin\objdump.exe -ZipPath build_gui\jogo.zip
        )
    ) else (
        echo AVISO: PowerShell nao encontrado; o pacote zip automatico nao foi atualizado.
    )
)

echo Binarios gerados:
echo   - build_gui\pentaminos_play.exe   ^(GUI: jogar e resolver^)
echo   - build_gui\pentaminos_demo.exe   ^(abre preenchido/resolvido^)
echo   - DLLs necessarias copiadas para build_gui\
echo   - build_gui\jogo.zip              ^(pacote para enviar^)
echo.
echo Para rodar manualmente:
echo   build_gui\pentaminos_play.exe 6 10
echo   build_gui\pentaminos_demo.exe 6 10
echo.
echo Observacao: sua organizacao pode bloquear a execucao ^(Device Guard/WDAC^).
echo Se ocorrer bloqueio, execute apenas em ambiente permitido ou WSLg.
echo.
pause
exit /b 0

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
