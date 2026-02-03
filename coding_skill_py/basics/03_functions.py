"""
============================================================================
파일명: 03_functions.py
주제: Python 함수
실행: python 03_functions.py
============================================================================
"""

def example_01_basic():
    """예제 1: 기본 함수"""
    print("\n=== 예제 1: 기본 함수 ===\n")
    
    # C 방식
    # int add(int a, int b) { return a + b; }
    
    # Python 방식 (타입 명시 불필요)
    def add(a, b):
        return a + b
    
    print(f"10 + 20 = {add(10, 20)}")
    
    # 여러 값 반환 (tuple)
    def get_min_max(numbers):
        return min(numbers), max(numbers)
    
    minimum, maximum = get_min_max([3, 1, 4, 1, 5])
    print(f"최소: {minimum}, 최대: {maximum}")

def example_02_default_params():
    """예제 2: 기본 매개변수"""
    print("\n=== 예제 2: 기본 매개변수 ===\n")
    
    def greet(name, message="안녕하세요"):
        print(f"{message}, {name}님!")
    
    greet("홍길동")                    # 기본값 사용
    greet("김철수", "반갑습니다")      # 기본값 재정의

def example_03_args_kwargs():
    """예제 3: 가변 인자"""
    print("\n=== 예제 3: 가변 인자 ===\n")
    
    # *args: 위치 인자
    def sum_all(*numbers):
        return sum(numbers)
    
    print(f"합계: {sum_all(1, 2, 3, 4, 5)}")
    
    # **kwargs: 키워드 인자
    def print_info(**info):
        for key, value in info.items():
            print(f"  {key}: {value}")
    
    print_info(name="홍길동", age=30, city="서울")

def example_04_lambda():
    """예제 4: 람다"""
    print("\n=== 예제 4: 람다 (익명 함수) ===\n")
    
    # 일반 함수
    def square(x):
        return x ** 2
    
    # 람다
    square_lambda = lambda x: x ** 2
    
    print(f"5의 제곱: {square(5)}")
    print(f"람다 5의 제곱: {square_lambda(5)}")
    
    # sorted with lambda
    points = [(1, 5), (3, 2), (2, 8)]
    sorted_points = sorted(points, key=lambda p: p[1])
    print(f"y값 기준 정렬: {sorted_points}")

def example_05_first_class():
    """예제 5: 일급 객체 (함수는 값)"""
    print("\n=== 예제 5: 함수는 일급 객체 ===\n")
    
    def add(a, b):
        return a + b
    
    def multiply(a, b):
        return a * b
    
    # 함수를 변수에 저장
    operation = add
    print(f"10 + 20 = {operation(10, 20)}")
    
    operation = multiply
    print(f"10 * 20 = {operation(10, 20)}")
    
    # 함수를 인자로 전달
    def apply_operation(op, a, b):
        return op(a, b)
    
    result = apply_operation(add, 5, 3)
    print(f"apply_operation: {result}")

def main():
    print("=" * 40)
    print("Python 기초: 03. 함수")
    print("=" * 40)
    
    example_01_basic()
    example_02_default_params()
    example_03_args_kwargs()
    example_04_lambda()
    example_05_first_class()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 04_classes_objects.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

기본 함수: def func():
기본 매개변수: def func(a, b=10):
가변 인자: def func(*args, **kwargs):
람다: lambda x: x ** 2
일급 객체: 함수는 값처럼 사용 가능

============================================================================
"""
