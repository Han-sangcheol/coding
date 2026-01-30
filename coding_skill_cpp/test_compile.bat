@echo off
echo 단일 파일 컴파일 테스트...
echo.

g++ -std=c++17 -o test.exe 00_function_pointer_basics.cpp -Wall 2> error.txt

if exist error.txt (
    echo === 컴파일 에러 ===
    type error.txt
) else (
    echo 컴파일 성공!
)

if exist test.exe (
    echo.
    echo 실행 파일 생성됨: test.exe
) else (
    echo.
    echo 실행 파일 생성 실패
)
