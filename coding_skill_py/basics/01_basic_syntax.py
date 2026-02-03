"""
============================================================================
파일명: 01_basic_syntax.py
주제: Python 기본 문법 (C와의 차이점)
실행: python 01_basic_syntax.py

============================================================================
이 챕터에서 배울 내용
============================================================================

- 변수 선언 (타입 명시 불필요)
- print vs printf
- 들여쓰기 기반 블록
- 타입 힌트 (선택적)
- 문자열 다루기

============================================================================
C와의 주요 차이점
============================================================================

C에서는:
- 타입 명시 필수 (int, char* 등)
- 세미콜론 필수
- 중괄호로 블록 구분
- printf/scanf
- 컴파일 필요

Python에서는:
- 타입 명시 불필요 (동적 타입)
- 세미콜론 불필요
- 들여쓰기로 블록 구분
- print/input
- 바로 실행

============================================================================
"""

def example_01_variables():
    """예제 1: 변수 선언"""
    print("\n=== 예제 1: 변수 선언 ===\n")
    
    # C 방식 (비교용, 실제 C 코드)
    """
    int x = 10;
    double y = 3.14;
    char* name = "Python";
    """
    
    # Python 방식 (타입 명시 불필요)
    x = 10              # int로 추론
    y = 3.14            # float로 추론
    name = "Python"     # str로 추론
    
    print(f"x = {x} (타입: {type(x).__name__})")
    print(f"y = {y} (타입: {type(y).__name__})")
    print(f"name = {name} (타입: {type(name).__name__})")
    
    # 타입이 자유롭게 변경됨
    x = "이제 문자열"
    print(f"x = {x} (타입: {type(x).__name__})")
    
    """
    Python의 특징:
    - 동적 타입: 실행 시간에 타입 결정
    - 타입 추론: 컴파일러가 자동으로 타입 파악
    - 유연성: 같은 변수에 다른 타입 대입 가능
    """

def example_02_print():
    """예제 2: 출력 (print vs printf)"""
    print("\n=== 예제 2: 출력 ===\n")
    
    # C 방식
    """
    printf("Hello, World!\n");
    printf("정수: %d, 실수: %.2f\n", 42, 3.14);
    """
    
    # Python 방식 1: 기본 print
    print("Hello, World!")
    
    # Python 방식 2: 여러 값 출력
    print("정수:", 42, "실수:", 3.14)
    
    # Python 방식 3: f-string (Python 3.6+)
    num = 42
    pi = 3.14159
    print(f"정수: {num}, 실수: {pi:.2f}")
    
    # Python 방식 4: format 메서드
    print("정수: {}, 실수: {:.2f}".format(num, pi))
    
    """
    print의 장점:
    - 포맷 문자열 불필요
    - 타입 자동 처리
    - f-string으로 간결
    """

def example_03_indentation():
    """예제 3: 들여쓰기"""
    print("\n=== 예제 3: 들여쓰기 (블록 구분) ===\n")
    
    # C 방식
    """
    if (x > 0) {
        printf("양수\n");
        printf("x = %d\n", x);
    }
    """
    
    # Python 방식 (중괄호 없음, 들여쓰기로 구분)
    x = 10
    if x > 0:
        print("양수")
        print(f"x = {x}")
    
    # 중첩된 블록
    for i in range(3):
        print(f"i = {i}")
        if i % 2 == 0:
            print("  짝수")
        else:
            print("  홀수")
    
    """
    들여쓰기 규칙:
    - 공백 4개 (PEP 8)
    - 탭 사용 가능하지만 공백 권장
    - 들여쓰기 잘못하면 IndentationError
    """

def example_04_type_hints():
    """예제 4: 타입 힌트 (선택적)"""
    print("\n=== 예제 4: 타입 힌트 ===\n")
    
    # 타입 힌트 없음 (일반적)
    def add(a, b):
        return a + b
    
    # 타입 힌트 있음 (선택적, 문서화 목적)
    def multiply(a: int, b: int) -> int:
        return a * b
    
    print(f"10 + 20 = {add(10, 20)}")
    print(f"10 * 20 = {multiply(10, 20)}")
    
    # 타입 힌트는 강제가 아님
    result = multiply(1.5, 2.5)  # float 전달해도 동작
    print(f"1.5 * 2.5 = {result}")
    
    """
    타입 힌트:
    - Python 3.5+
    - 선택적 (강제 아님)
    - 문서화 및 IDE 자동완성에 유용
    - mypy로 타입 체크 가능
    """

def example_05_strings():
    """예제 5: 문자열"""
    print("\n=== 예제 5: 문자열 ===\n")
    
    # C 방식
    """
    char* str = "Hello";
    char buffer[100];
    strcpy(buffer, str);
    strcat(buffer, " World");
    printf("%s\n", buffer);
    """
    
    # Python 방식
    str1 = "Hello"
    str2 = "World"
    result = str1 + " " + str2  # 간단한 연결
    print(result)
    
    # 문자열 메서드
    text = "python programming"
    print(f"대문자: {text.upper()}")
    print(f"제목: {text.title()}")
    print(f"단어 수: {len(text.split())}")
    
    # 문자열 슬라이싱
    print(f"처음 6글자: {text[:6]}")
    print(f"마지막 11글자: {text[-11:]}")
    
    # 멀티라인 문자열
    multiline = """
    여러 줄에
    걸친 문자열
    """
    print(multiline)

def example_06_c_vs_python():
    """예제 6: C vs Python 코드 비교"""
    print("\n=== 예제 6: C vs Python 비교 ===\n")
    
    print("[C 코드]")
    print("""
    #include <stdio.h>
    
    int main() {
        int sum = 0;
        for (int i = 1; i <= 10; i++) {
            sum += i;
        }
        printf("합계: %d\\n", sum);
        return 0;
    }
    """)
    
    print("\n[Python 코드]")
    sum_val = 0
    for i in range(1, 11):
        sum_val += i
    print(f"합계: {sum_val}")
    
    # 더 간단하게
    sum_val = sum(range(1, 11))
    print(f"합계 (간단): {sum_val}")

def example_07_common_mistakes():
    """예제 7: C 개발자가 흔히 하는 실수"""
    print("\n=== 예제 7: 흔한 실수 ===\n")
    
    print("[1] 세미콜론 붙이기")
    # x = 10;  # 에러는 아니지만 불필요
    x = 10   # 올바른 방식
    
    print("[2] 중괄호 사용")
    # if x > 0 { }  # 에러!
    if x > 0:  # 올바른 방식 (콜론 + 들여쓰기)
        print("양수")
    
    print("[3] 타입 선언")
    # int y = 20;  # 에러!
    y = 20  # 올바른 방식
    
    print("[4] 배열 선언")
    # int arr[5];  # 에러!
    arr = [0] * 5  # 올바른 방식 (리스트)
    
    print("Python 문법에 익숙해지세요!")

def main():
    """메인 함수"""
    print("=" * 40)
    print("Python 기초: 01. 기본 문법")
    print("=" * 40)
    
    example_01_variables()
    example_02_print()
    example_03_indentation()
    example_04_type_hints()
    example_05_strings()
    example_06_c_vs_python()
    example_07_common_mistakes()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 02_lists_tuples_dicts.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

변수:
- 타입 명시 불필요
- 동적 타입
- 자유로운 타입 변경

출력:
- print() 함수
- f-string으로 간편
- 포맷 문자열 불필요

블록:
- 중괄호 대신 들여쓰기
- 공백 4개 (PEP 8)
- 콜론(:) 후 들여쓰기

타입 힌트:
- 선택적
- 문서화 목적
- 강제 아님

C에서 Python으로:
- 세미콜론 제거
- 중괄호 → 들여쓰기
- 타입 선언 제거
- printf → print

============================================================================
"""
