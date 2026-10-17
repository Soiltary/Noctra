param([string]$Destination = (Join-Path (Split-Path -Parent $PSScriptRoot) 'engine'))
$ErrorActionPreference = 'Stop'
$ProgressPreference = 'Continue'
$releaseUrl = 'https://github.com/chiaki-ng/chiaki-ng/releases/download/v1.10.0/chiaki-ng-win_x64-MSYS2-Release-portable.zip'
$expectedHash = '4f1e824fc522560730e0b7a901e8811de2bab4a45ad5000386f7124bd855e4ac'
$innerArchive = 'chiaki-ng-win_x64-MSYS2-portable-1.10.0-test-build-f5519640adea6ec48087c30d5b9fad62d81166a8.zip'
$stage = $null
try {
    $Destination = [IO.Path]::GetFullPath($Destination)
    $target = Join-Path $Destination 'chiaki-ng-Win'
    if (Test-Path -LiteralPath (Join-Path $target 'chiaki.exe')) {
        Write-Host 'An engine already exists. Keeping its files and settings.'
        exit 0
    }
    if (Test-Path -LiteralPath $target) { throw 'The engine target already exists but is incomplete. Move it aside before retrying; no existing files were replaced.' }
    $stage = Join-Path ([IO.Path]::GetTempPath()) ('noctra-engine-' + [guid]::NewGuid().ToString('N'))
    New-Item -ItemType Directory -Path $stage | Out-Null
    $archive = Join-Path $stage 'official.zip'
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
    Write-Host 'Noctra Link - downloading the official chiaki-ng engine (about 97 MB).'
    Invoke-WebRequest -UseBasicParsing -Uri $releaseUrl -OutFile $archive
    $actualHash = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($actualHash -ne $expectedHash) { throw 'SHA-256 mismatch. The download will not be installed.' }
    Write-Host 'SHA-256 verified. Extracting the original engine...'
    $outer = Join-Path $stage 'outer'
    $inner = Join-Path $stage 'inner'
    Expand-Archive -LiteralPath $archive -DestinationPath $outer
    $nested = Join-Path $outer $innerArchive
    if (-not (Test-Path -LiteralPath $nested)) { throw 'Unexpected upstream archive layout.' }
    Expand-Archive -LiteralPath $nested -DestinationPath $inner
    $engine = Join-Path $inner 'chiaki-ng-Win'
    if (-not (Test-Path -LiteralPath (Join-Path $engine 'chiaki.exe'))) { throw 'The upstream engine executable is missing.' }
    New-Item -ItemType Directory -Path $Destination -Force | Out-Null
    Move-Item -LiteralPath $engine -Destination $target
    Write-Host 'Engine ready. Return to Noctra Link.'
} catch {
    Write-Host ('Setup failed: ' + $_.Exception.Message) -ForegroundColor Red
    exit 1
} finally {
    if ($stage -and (Test-Path -LiteralPath $stage)) { Remove-Item -LiteralPath $stage -Recurse -Force }
}
