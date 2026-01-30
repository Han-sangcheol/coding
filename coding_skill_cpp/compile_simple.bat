@echo off
REM ============================================================================
REM C++ 디자인 패턴 간단 컴파일 스크립트 (g++ 직접 사용)
REM ============================================================================

echo ========================================
echo C++ 디자인 패턴 컴파일 (g++)
echo ========================================
echo.

set TOTAL=0
set SUCCESS=0
set FAILED=0

REM 각 .cpp 파일 컴파일
for %%f in (*.cpp) do (
    set /A TOTAL+=1
    echo [!TOTAL!] 컴파일 중: %%f
    g++ -std=c++17 -o "%%~nf.exe" "%%f" -Wall -Wextra 2>error.log
    if !ERRORLEVEL! EQU 0 (
        echo    ✓ 성공: %%~nf.exe
        set /A SUCCESS+=1
    ) else (
        echo    ✗ 실패: %%f
        type error.log
        set /A FAILED+=1
    )
    echo.
)

echo ========================================
echo 컴파일 완료
echo ========================================
echo 총: %TOTAL%개
echo 성공: %SUCCESS%개
echo 실패: %FAILED%개
echo.

if exist error.log del error.log

echo 실행 방법:
echo   .\00_function_pointer.exe
echo   .\01_wrapper.exe
echo   등등...
echo.
pause
