"""
============================================================================
파일명: 07_decorators.py
주제: Python 데코레이터
실행: python 07_decorators.py
============================================================================
"""

def example_01_basic_decorator():
    """예제 1: 기본 데코레이터"""
    print("\n=== 예제 1: 기본 데코레이터 ===\n")
    
    # 데코레이터: 함수를 감싸는 함수
    def my_decorator(func):
        def wrapper():
            print("함수 실행 전")
            func()
            print("함수 실행 후")
        return wrapper
    
    @my_decorator
    def say_hello():
        print("Hello!")
    
    say_hello()
    
    # 위의 @my_decorator는 다음과 같음:
    # say_hello = my_decorator(say_hello)

def example_02_decorator_with_args():
    """예제 2: 인자가 있는 데코레이터"""
    print("\n=== 예제 2: 인자가 있는 데코레이터 ===\n")
    
    def timer_decorator(func):
        import time
        def wrapper(*args, **kwargs):
            start = time.time()
            result = func(*args, **kwargs)
            end = time.time()
            print(f"실행 시간: {end - start:.4f}초")
            return result
        return wrapper
    
    @timer_decorator
    def slow_function(n):
        total = sum(range(n))
        return total
    
    result = slow_function(1000000)
    print(f"결과: {result}")

def example_03_builtin_decorators():
    """예제 3: 내장 데코레이터"""
    print("\n=== 예제 3: 내장 데코레이터 ===\n")
    
    class MyClass:
        def __init__(self, value):
            self._value = value
        
        @property
        def value(self):
            """getter"""
            return self._value
        
        @staticmethod
        def static_method():
            """정적 메서드 (self 없음)"""
            print("정적 메서드")
        
        @classmethod
        def class_method(cls):
            """클래스 메서드 (cls)"""
            print(f"클래스 메서드: {cls.__name__}")
    
    obj = MyClass(100)
    print(f"value: {obj.value}")
    MyClass.static_method()
    MyClass.class_method()

def example_04_c_vs_python():
    """예제 4: C vs Python 비교"""
    print("\n=== 예제 4: C vs Python 비교 ===\n")
    
    print("[C 방식 (Wrapper 패턴)]")
    print("""
    void wrapper_function(void (*func)()) {
        printf("Before\\n");
        func();
        printf("After\\n");
    }
    """)
    
    print("\n[Python 방식 (데코레이터)]")
    print("""
    @my_decorator
    def my_function():
        pass
    """)

def main():
    print("=" * 40)
    print("Python 기초: 07. 데코레이터")
    print("=" * 40)
    
    example_01_basic_decorator()
    example_02_decorator_with_args()
    example_03_builtin_decorators()
    example_04_c_vs_python()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 08_generators.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

데코레이터: @decorator
기본: def decorator(func):
내장: @property, @staticmethod, @classmethod
용도: 로깅, 타이밍, 권한 체크

============================================================================
"""
