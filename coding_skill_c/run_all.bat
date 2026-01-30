@echo off
REM ============================================================================
REM 파일명: run_all.bat
REM 기능: 모든 C 디자인 패턴 예제를 순차적으로 실행
REM ============================================================================

echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║                                                            ║
echo ║     C 디자인 패턴 예제 실행 스크립트 (31개 패턴)          ║
echo ║                                                            ║
echo ╚════════════════════════════════════════════════════════════╝
echo.

REM 실행 옵션 선택
echo 실행 모드를 선택하세요:
echo 1. 전체 실행 (1-31번 모두)
echo 2. 기본 패턴만 (1-10번)
echo 3. 특정 패턴 선택
echo.
set /p choice="선택 (1-3): "

if "%choice%"=="1" goto run_all
if "%choice%"=="2" goto run_basic
if "%choice%"=="3" goto run_specific

:run_all
echo.
echo === 전체 패턴 실행 시작 ===
echo.

for %%i in (01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31) do (
    if exist %%i_*.exe (
        echo.
        echo ════════════════════════════════════════
        echo  패턴 %%i 실행 중...
        echo ════════════════════════════════════════
        for %%f in (%%i_*.exe) do (
            echo.
            %%f
            echo.
            echo ────────────────────────────────────────
        )
    )
)

goto end

:run_basic
echo.
echo === 기본 패턴 (1-10번) 실행 ===
echo.

for %%i in (01 02 03 04 05 06 07 08 09 10) do (
    if exist %%i_*.exe (
        echo.
        echo [%%i] 실행 중...
        for %%f in (%%i_*.exe) do %%f
        echo.
    )
)

goto end

:run_specific
echo.
echo 실행할 패턴 번호를 입력하세요 (01-31):
set /p pattern_num="번호: "

if exist %pattern_num%_*.exe (
    echo.
    echo === 패턴 %pattern_num% 실행 ===
    echo.
    for %%f in (%pattern_num%_*.exe) do %%f
) else (
    echo 오류: 패턴 %pattern_num%의 실행 파일을 찾을 수 없습니다.
    echo 먼저 compile_all.bat를 실행하세요.
)

:end
echo.
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║                                                            ║
echo ║     모든 예제 실행 완료!                                   ║
echo ║                                                            ║
echo ╚════════════════════════════════════════════════════════════╝
echo.
pause
