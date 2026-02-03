"""
============================================================================
파일명: 10_comprehensions.py
주제: Python 컴프리헨션
실행: python 10_comprehensions.py
============================================================================
"""

def example_01_list_comprehension():
    """예제 1: 리스트 컴프리헨션"""
    print("\n=== 예제 1: 리스트 컴프리헨션 ===\n")
    
    # C 방식
    """
    int squares[10];
    for (int i = 0; i < 10; i++) {
        squares[i] = i * i;
    }
    """
    
    # Python 일반 루프
    squares = []
    for i in range(10):
        squares.append(i ** 2)
    print(f"일반 루프: {squares}")
    
    # Python 컴프리헨션 (한 줄!)
    squares = [i ** 2 for i in range(10)]
    print(f"컴프리헨션: {squares}")
    
    # 조건 포함
    evens = [i for i in range(20) if i % 2 == 0]
    print(f"짝수: {evens}")

def example_02_dict_comprehension():
    """예제 2: 딕셔너리 컴프리헨션"""
    print("\n=== 예제 2: 딕셔너리 컴프리헨션 ===\n")
    
    # 숫자와 제곱
    squares_dict = {i: i**2 for i in range(5)}
    print(f"딕셔너리: {squares_dict}")
    
    # 리스트를 딕셔너리로
    names = ["Alice", "Bob", "Charlie"]
    name_lengths = {name: len(name) for name in names}
    print(f"이름 길이: {name_lengths}")

def example_03_set_comprehension():
    """예제 3: 셋 컴프리헨션"""
    print("\n=== 예제 3: 셋 컴프리헨션 ===\n")
    
    # 중복 제거
    numbers = [1, 2, 2, 3, 3, 3, 4, 5]
    unique = {x for x in numbers}
    print(f"중복 제거: {unique}")

def example_04_nested_comprehension():
    """예제 4: 중첩 컴프리헨션"""
    print("\n=== 예제 4: 중첩 컴프리헨션 ===\n")
    
    # 2D 리스트 평탄화
    matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
    flat = [item for row in matrix for item in row]
    print(f"평탄화: {flat}")

def example_05_c_vs_python():
    """예제 5: C vs Python 비교"""
    print("\n=== 예제 5: C vs Python 비교 ===\n")
    
    print("[C 방식 (10줄)]")
    print("""
    int result[100];
    int count = 0;
    for (int i = 0; i < 100; i++) {
        if (i % 2 == 0) {
            result[count++] = i * i;
        }
    }
    """)
    
    print("\n[Python 컴프리헨션 (1줄)]")
    result = [i**2 for i in range(100) if i % 2 == 0]
    print(f"결과 (일부): {result[:5]}...")

def main():
    print("=" * 40)
    print("Python 기초: 10. 컴프리헨션")
    print("=" * 40)
    
    example_01_list_comprehension()
    example_02_dict_comprehension()
    example_03_set_comprehension()
    example_04_nested_comprehension()
    example_05_c_vs_python()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 11_advanced_oop.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

리스트: [x for x in range(10)]
딕셔너리: {k: v for k, v in items}
셋: {x for x in list}
조건: [x for x in list if condition]
중첩: [item for row in matrix for item in row]

장점:
- 간결
- 빠름
- Pythonic

============================================================================
"""
