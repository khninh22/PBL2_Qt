# ===== BUILD SCRIPT FOR PBL2_Qt =====
# Tự động tìm Qt CMake và build project

Write-Host "=== PBL2_Qt Auto Build Script ===" -ForegroundColor Cyan

# Tìm Qt installation
$qtPaths = @(
    "C:\Qt\6.9.3\mingw_64\bin\cmake.exe",
    "C:\Qt\6.8.0\mingw_64\bin\cmake.exe",
    "C:\Qt\Tools\CMake_64\bin\cmake.exe",
    "C:\Program Files\CMake\bin\cmake.exe"
)

$cmakePath = $null
foreach ($path in $qtPaths) {
    if (Test-Path $path) {
        $cmakePath = $path
        Write-Host "[OK] Found CMake: $path" -ForegroundColor Green
        break
    }
}

if (-not $cmakePath) {
    Write-Host "[ERROR] CMake not found!" -ForegroundColor Red
    Write-Host "Please install Qt with CMake or install CMake separately." -ForegroundColor Yellow
    pause
    exit 1
}

# Build
Write-Host "`n[*] Building project..." -ForegroundColor Yellow
Set-Location "d:\PBL2_Qt\build_refactor"

& $cmakePath --build . --config Release

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n[OK] BUILD SUCCESS!" -ForegroundColor Green
    Write-Host "Executable: d:\PBL2_Qt\build_refactor\bin\QuanLyThueSanQt.exe" -ForegroundColor Cyan
    
    # Run?
    $run = Read-Host "`nRun application? (y/n)"
    if ($run -eq 'y') {
        Write-Host "`n[*] Starting application..." -ForegroundColor Yellow
        Start-Process "d:\PBL2_Qt\build_refactor\bin\QuanLyThueSanQt.exe"
    }
} else {
    Write-Host "`n[ERROR] BUILD FAILED!" -ForegroundColor Red
    Write-Host "Check errors above." -ForegroundColor Yellow
}

pause

