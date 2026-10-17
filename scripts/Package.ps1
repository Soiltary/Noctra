param(
    [string]$Executable = (Join-Path (Split-Path -Parent $PSScriptRoot) 'build/Release/NoctraLink.exe'),
    [string]$Version = '1.0.0'
)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
if ($Version -notmatch '^\d+\.\d+\.\d+$') { throw 'Version must use major.minor.patch.' }
if (-not (Test-Path -LiteralPath $Executable -PathType Leaf)) { throw 'Build NoctraLink.exe first, or pass -Executable.' }
$dist = Join-Path $root 'dist'
$packageName = "Noctra-Link-v$Version-Windows-x64"
$stage = Join-Path $dist $packageName
if (Test-Path -LiteralPath $stage) { Remove-Item -LiteralPath $stage -Recurse -Force }
New-Item -ItemType Directory -Path $stage -Force | Out-Null
Copy-Item -LiteralPath $Executable -Destination (Join-Path $stage 'NoctraLink.exe')
foreach ($item in @('README.md','LICENSE','THIRD_PARTY_NOTICES.md','CHANGELOG.md','docs','licenses','assets')) {
    Copy-Item -LiteralPath (Join-Path $root $item) -Destination $stage -Recurse
}
New-Item -ItemType Directory -Path (Join-Path $stage 'scripts') | Out-Null
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'Fetch-Engine.ps1') -Destination (Join-Path $stage 'scripts')
$archive = Join-Path $dist "$packageName.zip"
Compress-Archive -LiteralPath $stage -DestinationPath $archive -Force
$hash = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
"$hash  $packageName.zip" | Set-Content -LiteralPath (Join-Path $dist 'SHA256SUMS.txt') -Encoding ascii
Write-Host "Created $archive"
