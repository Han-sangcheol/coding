"""
============================================================================
파일명: 08_generators.py
주제: Python 제너레이터
실행: python 08_generators.py
============================================================================
"""

def example_01_basic_generator():
    """예제 1: 기본 제너레이터"""
    print("\n=== 예제 1: 기본 제너레이터 ===\n")
    
    # 일반 함수: 모든 값을 리스트로 반환
    def get_numbers_list(n):
        result = []
        for i in range(n):
            result.append(i ** 2)
        return result  # 모든 값을 메모리에 저장
    
    # 제너레이터: 값을 하나씩 생성
    def get_numbers_generator(n):
        for i in range(n):
            yield i ** 2  # yield = 값을 하나씩 반환
    
    # 리스트 (메모리 많이 사용)
    numbers_list = get_numbers_list(5)
    print(f"리스트: {numbers_list}")
    
    # 제너레이터 (메모리 효율적)
    numbers_gen = get_numbers_generator(5)
    print(f"제너레이터: {numbers_gen}")
    print("제너레이터 값:", list(numbers_gen))

def example_02_large_data():
    """예제 2: 대용량 데이터"""
    print("\n=== 예제 2: 대용량 데이터 처리 ===\n")
    
    # 리스트: 모든 값을 메모리에 저장
    # big_list = [i for i in range(1000000)]  # 메모리 많이 사용
    
    # 제너레이터: 값을 하나씩 생성
    def big_generator(n):
        for i in range(n):
            yield i
    
    gen = big_generator(1000000)
    
    # 필요한 만큼만 사용
    for i, val in enumerate(gen):
        if i >= 5:
            break
        print(f"  {val}")
    
    print("제너레이터는 메모리 효율적!")

def example_03_generator_expression():
    """예제 3: 제너레이터 표현식"""
    print("\n=== 예제 3: 제너레이터 표현식 ===\n")
    
    # 리스트 컴프리헨션
    squares_list = [x**2 for x in range(10)]
    print(f"리스트: {squares_list}")
    
    # 제너레이터 표현식 (괄호)
    squares_gen = (x**2 for x in range(10))
    print(f"제너레이터: {squares_gen}")
    print(f"값: {list(squares_gen)}")

def example_04_c_vs_python():
    """예제 4: C vs Python 비교"""
    print("\n=== 예제 4: C vs Python 비교 ===\n")
    
    print("[C 방식]")
    print("""
    // 모든 값을 배열에 저장
    int* values = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        values[i] = i * i;
    }
    // 메모리 많이 사용
    """)
    
    print("\n[Python 제너레이터]")
    print("""
    # 값을 하나씩 생성
    def gen(n):
        for i in range(n):
            yield i * i
    # 메모리 효율적
    """)

def main():
    print("=" * 40)
    print("Python 기초: 08. 제너레이터")
    print("=" * 40)
    
    example_01_basic_generator()
    example_02_large_data()
    example_03_generator_expression()
    example_04_c_vs_python()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 09_context_managers.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

제너레이터: yield 키워드
표현식: (x for x in range(10))
장점:
- 메모리 효율
- 지연 평가
- 대용량 데이터 처리

============================================================================
"""
