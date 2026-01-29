@echo off
REM ============================================================================
REM C++ 디자인 패턴 컴파일 스크립트 (Windows)
REM ============================================================================

echo ========================================
echo C++ 디자인 패턴 컴파일
echo ========================================
echo.

REM 빌드 디렉토리 생성
if not exist build mkdir build

REM CMake 설정 및 빌드
cd build
echo [1/2] CMake 설정 중...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo CMake 설정 실패!
    pause
    exit /b 1
)

echo.
echo [2/2] 빌드 중...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo 빌드 실패!
    pause
    exit /b 1
)

cd ..

echo.
echo ========================================
echo 컴파일 완료!
echo ========================================
echo.
echo 실행 파일 위치: build/bin/
echo.
echo 개별 실행:
echo   build\bin\00_function_pointer.exe
echo   build\bin\01_wrapper.exe
echo   ...
echo.
pause
