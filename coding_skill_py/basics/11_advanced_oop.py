"""
============================================================================
파일명: 11_advanced_oop.py
주제: Python 고급 OOP
실행: python 11_advanced_oop.py
============================================================================
"""

def example_01_magic_methods():
    """예제 1: 매직 메서드"""
    print("\n=== 예제 1: 매직 메서드 ===\n")
    
    class Vector:
        def __init__(self, x, y):
            self.x = x
            self.y = y
        
        def __str__(self):
            """print()에서 호출"""
            return f"Vector({self.x}, {self.y})"
        
        def __add__(self, other):
            """+ 연산자 오버로딩"""
            return Vector(self.x + other.x, self.y + other.y)
        
        def __eq__(self, other):
            """== 연산자"""
            return self.x == other.x and self.y == other.y
    
    v1 = Vector(1, 2)
    v2 = Vector(3, 4)
    v3 = v1 + v2  # __add__ 호출
    
    print(f"v1: {v1}")
    print(f"v2: {v2}")
    print(f"v1 + v2: {v3}")

def example_02_property():
    """예제 2: @property"""
    print("\n=== 예제 2: @property ===\n")
    
    class Temperature:
        def __init__(self, celsius):
            self._celsius = celsius
        
        @property
        def celsius(self):
            """Getter"""
            return self._celsius
        
        @celsius.setter
        def celsius(self, value):
            """Setter"""
            if value < -273.15:
                raise ValueError("절대영도 이하!")
            self._celsius = value
        
        @property
        def fahrenheit(self):
            """계산된 속성"""
            return self._celsius * 9/5 + 32
    
    temp = Temperature(25)
    print(f"섭씨: {temp.celsius}°C")
    print(f"화씨: {temp.fahrenheit}°F")
    
    temp.celsius = 30
    print(f"변경 후: {temp.celsius}°C")

def example_03_class_vs_static():
    """예제 3: 클래스 메서드 vs 정적 메서드"""
    print("\n=== 예제 3: 클래스/정적 메서드 ===\n")
    
    class MyClass:
        count = 0  # 클래스 변수
        
        def __init__(self):
            MyClass.count += 1
        
        @classmethod
        def get_count(cls):
            """클래스 메서드 (cls)"""
            return f"{cls.__name__}: {cls.count}개"
        
        @staticmethod
        def utility_function(x, y):
            """정적 메서드 (self 없음)"""
            return x + y
    
    obj1 = MyClass()
    obj2 = MyClass()
    
    print(MyClass.get_count())
    print(f"유틸리티: {MyClass.utility_function(10, 20)}")

def example_04_abstract_class():
    """예제 4: 추상 클래스"""
    print("\n=== 예제 4: 추상 클래스 ===\n")
    
    from abc import ABC, abstractmethod
    
    class Shape(ABC):
        @abstractmethod
        def area(self):
            pass
    
    class Circle(Shape):
        def __init__(self, radius):
            self.radius = radius
        
        def area(self):
            return 3.14 * self.radius ** 2
    
    # shape = Shape()  # 에러! 추상 클래스는 객체 생성 불가
    circle = Circle(5)
    print(f"원 면적: {circle.area()}")

def main():
    print("=" * 40)
    print("Python 기초: 11. 고급 OOP")
    print("=" * 40)
    
    example_01_magic_methods()
    example_02_property()
    example_03_class_vs_static()
    example_04_abstract_class()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 12_async_await.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

매직 메서드:
- __init__, __str__, __add__, __eq__

@property:
- getter/setter
- 계산된 속성

@classmethod: 클래스 메서드 (cls)
@staticmethod: 정적 메서드 (self 없음)

ABC: 추상 베이스 클래스
@abstractmethod: 추상 메서드

============================================================================
"""
