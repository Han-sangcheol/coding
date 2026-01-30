@echo off
setlocal enabledelayedexpansion

echo ========================================
echo C++ 컴파일 (개별 실행)
echo ========================================
echo.

REM 파일 목록
set FILES=00_function_pointer_basics 01_wrapper_pattern 02_adapter_pattern 03_singleton_pattern

for %%F in (%FILES%) do (
    echo 컴파일 중: %%F.cpp
    g++ -std=c++17 -o "%%F.exe" "%%F.cpp" 2>nul
    if exist "%%F.exe" (
        echo   ✓ 성공
    ) else (
        echo   ✗ 실패
    )
)

echo.
echo 완료!
echo.
echo 실행: .\00_function_pointer_basics.exe
pause
