param(
    [string]$TargetDir,

    [string]$SearchDirs,

    [string]$Objdump,

    [string]$ZipPath
)

$ErrorActionPreference = "Stop"

if (-not $TargetDir) { $TargetDir = $env:DLL_TARGET_DIR }
if (-not $SearchDirs) { $SearchDirs = $env:DLL_SEARCH_DIRS }
if (-not $Objdump) { $Objdump = $env:DLL_OBJDUMP }
if (-not $ZipPath) { $ZipPath = $env:DLL_ZIP }

if (-not $TargetDir -or -not $SearchDirs -or -not $Objdump) {
    Write-Host "AVISO: parametros incompletos para copiar DLLs automaticamente."
    exit 1
}

$TargetDir = (Resolve-Path -LiteralPath $TargetDir).Path
$SearchDirs = $SearchDirs -split ";" |
    Where-Object { $_ -and (Test-Path -LiteralPath $_) } |
    ForEach-Object { (Resolve-Path -LiteralPath $_).Path } |
    Select-Object -Unique

if (-not (Test-Path -LiteralPath $Objdump)) {
    Write-Host "AVISO: objdump nao encontrado. Nao foi possivel detectar DLLs automaticamente."
    exit 0
}

$systemDlls = @(
    "advapi32.dll", "bcrypt.dll", "comctl32.dll", "comdlg32.dll", "crypt32.dll",
    "dwmapi.dll", "gdi32.dll", "imm32.dll", "kernel32.dll", "msvcrt.dll",
    "ole32.dll", "oleaut32.dll", "opengl32.dll", "rpcrt4.dll", "secur32.dll",
    "setupapi.dll", "shell32.dll", "shlwapi.dll", "user32.dll", "uxtheme.dll",
    "version.dll", "winmm.dll", "ws2_32.dll"
) | ForEach-Object { $_.ToLowerInvariant() }

$seen = @{}
$queue = New-Object System.Collections.Queue

Get-ChildItem -LiteralPath $TargetDir -File |
    Where-Object { $_.Extension -in ".exe", ".dll" } |
    ForEach-Object { $queue.Enqueue($_.FullName) }

while ($queue.Count -gt 0) {
    $file = $queue.Dequeue()
    $key = $file.ToLowerInvariant()
    if ($seen.ContainsKey($key)) {
        continue
    }
    $seen[$key] = $true

    $dllNames = & $Objdump -p $file 2>$null |
        Select-String -Pattern "DLL Name:\s*(.+)$" |
        ForEach-Object { $_.Matches[0].Groups[1].Value.Trim() } |
        Select-Object -Unique

    foreach ($dllName in $dllNames) {
        $lowerName = $dllName.ToLowerInvariant()
        if ($systemDlls -contains $lowerName -or $lowerName.StartsWith("api-ms-win-")) {
            continue
        }

        $targetDll = Join-Path $TargetDir $dllName
        if (Test-Path -LiteralPath $targetDll) {
            $queue.Enqueue((Resolve-Path -LiteralPath $targetDll).Path)
            continue
        }

        $sourceDll = $null
        foreach ($dir in $SearchDirs) {
            $candidate = Join-Path $dir $dllName
            if (Test-Path -LiteralPath $candidate) {
                $sourceDll = $candidate
                break
            }
        }

        if ($sourceDll) {
            Copy-Item -LiteralPath $sourceDll -Destination $targetDll -Force
            Write-Host "DLL copiada: $dllName"
            $queue.Enqueue($targetDll)
        } else {
            Write-Host "AVISO: DLL nao encontrada nas pastas conhecidas: $dllName"
        }
    }
}

if ($ZipPath) {
    $packageFiles = Get-ChildItem -LiteralPath $TargetDir -File |
        Where-Object { $_.Extension -in ".exe", ".dll" } |
        ForEach-Object { $_.FullName }

    if ($packageFiles) {
        Compress-Archive -LiteralPath $packageFiles -DestinationPath $ZipPath -Force
        Write-Host "Pacote atualizado: $ZipPath"
    }
}
