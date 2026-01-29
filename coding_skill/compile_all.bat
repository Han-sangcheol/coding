@echo off
REM ============================================================================
REM 파일명: compile_all.bat
REM 기능: 모든 C 예제 파일을 컴파일하는 배치 스크립트
REM ============================================================================

echo ========================================
echo C 디자인 패턴 예제 컴파일 스크립트
echo ========================================
echo.

REM 1. Wrapper 패턴 컴파일
echo [1/3] Wrapper 패턴 컴파일 중...
gcc -o 01_wrapper.exe 01_wrapper_pattern.c -Wall
if %ERRORLEVEL% EQU 0 (
    echo ✓ 01_wrapper.exe 컴파일 성공
) else (
    echo ✗ 01_wrapper.exe 컴파일 실패
)
echo.

REM 2. Adapter 패턴 컴파일
echo [2/3] Adapter 패턴 컴파일 중...
gcc -o 02_adapter.exe 02_adapter_pattern.c -Wall
if %ERRORLEVEL% EQU 0 (
    echo ✓ 02_adapter.exe 컴파일 성공
) else (
    echo ✗ 02_adapter.exe 컴파일 실패
)
echo.

REM 3. 통합 예제 컴파일
echo [3/3] Wrapper + Adapter 통합 예제 컴파일 중...
gcc -o 03_combined.exe 03_wrapper_adapter_combined.c -Wall -lm
if %ERRORLEVEL% EQU 0 (
    echo ✓ 03_combined.exe 컴파일 성공
) else (
    echo ✗ 03_combined.exe 컴파일 실패
)
echo.

echo ========================================
echo 컴파일 완료
echo ========================================
echo.
echo 실행 방법:
echo   01_wrapper.exe
echo   02_adapter.exe
echo   03_combined.exe
echo.
pause
