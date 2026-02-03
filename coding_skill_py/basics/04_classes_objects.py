"""
============================================================================
파일명: 04_classes_objects.py
주제: Python 클래스와 객체
실행: python 04_classes_objects.py
============================================================================
"""

def example_01_basic_class():
    """예제 1: 기본 클래스"""
    print("\n=== 예제 1: 기본 클래스 ===\n")
    
    # C 방식: struct + 함수
    """
    typedef struct {
        char name[50];
        int age;
    } Person;
    
    void Person_Print(Person* p) { ... }
    """
    
    # Python 방식: 클래스
    class Person:
        def __init__(self, name, age):
            """생성자 (C의 init 함수)"""
            self.name = name
            self.age = age
        
        def print_info(self):
            """메서드"""
            print(f"이름: {self.name}, 나이: {self.age}")
    
    person = Person("홍길동", 30)
    person.print_info()

def example_02_properties():
    """예제 2: @property (getter/setter)"""
    print("\n=== 예제 2: @property ===\n")
    
    class BankAccount:
        def __init__(self, owner):
            self._owner = owner
            self._balance = 0  # private (관례상 _)
        
        @property
        def balance(self):
            """Getter"""
            return self._balance
        
        def deposit(self, amount):
            if amount > 0:
                self._balance += amount
                print(f"{amount}원 입금")
        
        def withdraw(self, amount):
            if 0 < amount <= self._balance:
                self._balance -= amount
                print(f"{amount}원 출금")
    
    account = BankAccount("홍길동")
    account.deposit(10000)
    print(f"잔액: {account.balance}")

def example_03_inheritance():
    """예제 3: 상속"""
    print("\n=== 예제 3: 상속 ===\n")
    
    class Animal:
        def __init__(self, name):
            self.name = name
        
        def speak(self):
            pass
    
    class Dog(Animal):
        def speak(self):
            return f"{self.name}: 멍멍!"
    
    class Cat(Animal):
        def speak(self):
            return f"{self.name}: 야옹~"
    
    dog = Dog("멍멍이")
    cat = Cat("야옹이")
    
    print(dog.speak())
    print(cat.speak())

def example_04_c_vs_python():
    """예제 4: C vs Python 비교"""
    print("\n=== 예제 4: C vs Python 비교 ===\n")
    
    print("[C 방식]")
    print("""
    typedef struct { char name[50]; int age; } Person;
    void Person_Init(Person* p, const char* name, int age);
    void Person_Print(Person* p);
    """)
    
    print("\n[Python 방식]")
    class Person:
        def __init__(self, name, age):
            self.name = name
            self.age = age
        
        def print_info(self):
            print(f"{self.name}, {self.age}세")
    
    p = Person("홍길동", 30)
    p.print_info()

def main():
    print("=" * 40)
    print("Python 기초: 04. 클래스와 객체")
    print("=" * 40)
    
    example_01_basic_class()
    example_02_properties()
    example_03_inheritance()
    example_04_c_vs_python()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 05_modules_packages.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

클래스: class MyClass:
생성자: __init__(self)
메서드: def method(self):
속성: self.name
@property: getter/setter
상속: class Child(Parent):

============================================================================
"""
