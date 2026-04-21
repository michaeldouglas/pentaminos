# Menu interativo de compilacao - Pentaminos
# Executar: .\menu.ps1

Clear-Host
Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "   PENTAMINOS - Menu de Compilacao" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

$opcoes = @(
    "Compilar Versao CONSOLE (Recomendado para comecao)",
    "Compilar Versao GUI (Bonita, precisa Raylib)",
    "Instalar Raylib (para GUI)",
    "Ver Status do Projeto",
    "Sair"
)

for ($i = 0; $i -lt $opcoes.Length; $i++) {
    Write-Host "$($i+1) - $($opcoes[$i])"
}

Write-Host ""
$choice = Read-Host "Escolha uma opcao (1-5)"

switch ($choice) {
    "1" { Compilar-Console }
    "2" { Compilar-GUI }
    "3" { Instalar-Raylib }
    "4" { Ver-Status }
    "5" { exit }
    default { 
        Write-Host "Opcao invalida!" -ForegroundColor Red
    }
}

# ============================================================
function Compilar-Console {
    Write-Host "`n=== Compilando Versao Console ===" -ForegroundColor Green
    Write-Host ""
    
    if (-not (Test-Path "build_console")) {
        New-Item -ItemType Directory "build_console" -Force | Out-Null
    }
    
    Push-Location build_console
    
    Write-Host "Compilando..." -ForegroundColor Yellow
    g++ -std=c++17 -I..\include -O2 -o pentaminos ..\main.cpp `
        ..\src\Board.cpp ..\src\Piece.cpp ..\src\State.cpp `
        ..\src\GraphSolver.cpp ..\src\AVLTree.cpp
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERRO na compilacao!" -ForegroundColor Red
        Pop-Location
        return
    }
    
    Write-Host "Sucesso! Executavel: pentaminos.exe" -ForegroundColor Green
    Write-Host ""
    $run = Read-Host "Deseja executar? (S/N)"
    
    if ($run -eq "S" -or $run -eq "s") {
        .\pentaminos.exe
    }
    
    Pop-Location
}

# ============================================================
function Compilar-GUI {
    Write-Host "`n=== Compilando Versao GUI ===" -ForegroundColor Green
    Write-Host ""
    Write-Host "Verificando se Raylib esta instalado..." -ForegroundColor Yellow
    
    $raylib_path = "c:\temp\vcpkg\installed\x64-mingw-dynamic"
    
    if (-not (Test-Path "$raylib_path\lib\raylib.lib")) {
        Write-Host "Raylib nao encontrado!" -ForegroundColor Red
        Write-Host "Execute opcao 3 para instalar." -ForegroundColor Yellow
        return
    }
    
    Write-Host "Raylib encontrado. Compilando..." -ForegroundColor Yellow
    
    if (-not (Test-Path "build_gui")) {
        New-Item -ItemType Directory "build_gui" -Force | Out-Null
    }
    
    Push-Location build_gui
    
    g++ -std=c++17 -I..\include -I"$raylib_path\include" `
        -L"$raylib_path\lib" -O2 -o pentaminos `
        ..\main.cpp ..\src\Board.cpp ..\src\Piece.cpp `
        ..\src\State.cpp ..\src\GraphSolver.cpp ..\src\AVLTree.cpp `
        -lraylib -lwinmm -luser32 -lgdi32
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERRO na compilacao!" -ForegroundColor Red
        Pop-Location
        return
    }
    
    Write-Host "Sucesso! Executavel: pentaminos.exe" -ForegroundColor Green
    Write-Host ""
    $run = Read-Host "Deseja executar? (S/N)"
    
    if ($run -eq "S" -or $run -eq "s") {
        .\pentaminos.exe
    }
    
    Pop-Location
}

# ============================================================
function Instalar-Raylib {
    Write-Host "`n=== Instalando Raylib ===" -ForegroundColor Green
    Write-Host ""
    Write-Host "Isto instalara vcpkg e Raylib automaticamente." -ForegroundColor Yellow
    Write-Host "Precisa de ~5-10 minutos e ~2GB de espaco." -ForegroundColor Yellow
    Write-Host ""
    
    $cont = Read-Host "Continuar? (S/N)"
    if ($cont -ne "S" -and $cont -ne "s") { return }
    
    Write-Host ""
    Write-Host "Criando diretorio..." -ForegroundColor Yellow
    
    if (-not (Test-Path "c:\temp")) {
        New-Item -ItemType Directory "c:\temp" -Force | Out-Null
    }
    
    Push-Location "c:\temp"
    
    if (-not (Test-Path "vcpkg")) {
        Write-Host "Clonando vcpkg (pode demorar)..." -ForegroundColor Yellow
        git clone https://github.com/Microsoft/vcpkg.git
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "ERRO ao clonar vcpkg!" -ForegroundColor Red
            Write-Host "Certifique-se que tem Git instalado." -ForegroundColor Yellow
            Pop-Location
            return
        }
    }
    
    Push-Location "vcpkg"
    
    Write-Host ""
    Write-Host "Bootstrapando vcpkg..." -ForegroundColor Yellow
    .\bootstrap-vcpkg.bat
    
    Write-Host ""
    Write-Host "Instalando Raylib (pode demorar MUITO)..." -ForegroundColor Yellow
    .\vcpkg.exe install raylib:x64-mingw-dynamic
    
    Write-Host ""
    Write-Host "Concluido! Raylib instalado em: c:\temp\vcpkg\installed\x64-mingw-dynamic" -ForegroundColor Green
    
    Pop-Location
    Pop-Location
}

# ============================================================
function Ver-Status {
    Write-Host "`n=== STATUS DO PROJETO ===" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Arquivos principais:" -ForegroundColor Yellow
    
    if (Test-Path "main.cpp") { Write-Host "  [OK] main.cpp" -ForegroundColor Green }
    if (Test-Path "src\Board.cpp") { Write-Host "  [OK] src\Board.cpp" -ForegroundColor Green }
    if (Test-Path "include\Board.h") { Write-Host "  [OK] include\Board.h" -ForegroundColor Green }
    
    Write-Host ""
    Write-Host "Ferramentas:" -ForegroundColor Yellow
    
    try {
        $null = g++ --version 2>$null
        Write-Host "  [OK] g++ instalado" -ForegroundColor Green
    } catch {
        Write-Host "  [FALTA] g++" -ForegroundColor Red
    }
    
    if (Test-Path "c:\temp\vcpkg\installed\x64-mingw-dynamic\include\raylib.h") {
        Write-Host "  [OK] Raylib instalado" -ForegroundColor Green
    } else {
        Write-Host "  [FALTA] Raylib - Execute opcao 3 para instalar" -ForegroundColor Red
    }
    
    Write-Host ""
    Write-Host "Versoes compiladas:" -ForegroundColor Yellow
    
    if (Test-Path "build_console\pentaminos.exe") {
        Write-Host "  [OK] Console compilado" -ForegroundColor Green
    }
    if (Test-Path "build_gui\pentaminos.exe") {
        Write-Host "  [OK] GUI compilado" -ForegroundColor Green
    }
    
    Write-Host ""
}
