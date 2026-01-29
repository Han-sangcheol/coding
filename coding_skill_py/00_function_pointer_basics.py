"""
============================================================================
파일명: 00_function_pointer_basics.py
기능: Python 일급 함수와 콜백 완전 정복

C 버전과의 주요 차이점:
  - 함수 포인터 → 일급 함수 (First-class function)
  - typedef → typing (타입 힌팅)
  - 구조체 + 함수 포인터 → 클래스 + 메서드
  - 콜백 → 데코레이터, 람다, 클로저
  - 배열 → 리스트, 딕셔너리

Python 추가 기능:
  - 데코레이터 (@decorator)
  - 클로저 (Closure)
  - functools.partial
  - callable 객체 (__call__)
  - 동적 함수 생성
  - 메타프로그래밍

============================================================================
📚 학습 목차
============================================================================

1단계: Python의 일급 함수
2단계: 람다 표현식
3단계: 클로저 (Closure)
4단계: 데코레이터
5단계: callable 객체
6단계: functools 활용
7단계: 실무 예제 (Observer, Command, Strategy)
8단계: C vs Python 비교

============================================================================
"""

from typing import Callable, List, Dict, Any, TypeVar, Generic, Protocol
from functools import wraps, partial
from abc import ABC, abstractmethod
from dataclasses import dataclass
import time

# ============================================================================
# 1단계: Python의 일급 함수
# ============================================================================
# 
# Python에서 함수는 "일급 객체" (First-class citizen)
# 
# 의미:
#   - 변수에 할당 가능
#   - 함수의 인자로 전달 가능
#   - 함수의 반환값이 될 수 있음
#   - 리스트/딕셔너리에 저장 가능
# 
# C의 함수 포인터보다 훨씬 유연!
# 
# ============================================================================

def hello_world():
    print("Hello, World!")

def goodbye():
    print("Goodbye!")

def step1_first_class_function():
    print("\n")
    print("=" * 40)
    print("  1단계: Python의 일급 함수")
    print("=" * 40)
    print()
    
    # 함수를 변수에 할당
    print("[함수를 변수에 할당]")
    greet = hello_world
    greet()  # Hello, World!
    
    # 함수를 리스트에 저장
    print("\n[함수를 리스트에 저장]")
    functions = [hello_world, goodbye]
    for func in functions:
        func()
    
    # 함수를 딕셔너리에 저장
    print("\n[함수를 딕셔너리에 저장]")
    commands = {
        'hello': hello_world,
        'bye': goodbye
    }
    commands['hello']()
    commands['bye']()
    
    # 함수의 속성 확인
    print("\n[함수 정보]")
    print(f"  함수 이름: {hello_world.__name__}")
    print(f"  함수 타입: {type(hello_world)}")
    print(f"  호출 가능: {callable(hello_world)}")
    
    print("\n💡 Python에서 함수는 진정한 일급 객체입니다!")

# ============================================================================
# 2단계: 람다 표현식
# ============================================================================
# 
# lambda 인자: 표현식
# 
# C++의 람다보다 단순하지만, 한 줄로 간단한 함수 생성 가능
# 
# ============================================================================

def step2_lambda():
    print("\n")
    print("=" * 40)
    print("  2단계: 람다 표현식")
    print("=" * 40)
    print()
    
    # 기본 람다
    print("[기본 람다]")
    add = lambda a, b: a + b
    print(f"3 + 7 = {add(3, 7)}")
    
    # 람다를 리스트에 저장
    print("\n[람다 리스트]")
    operations = [
        lambda a, b: a + b,
        lambda a, b: a - b,
        lambda a, b: a * b,
        lambda a, b: a / b
    ]
    
    for i, op in enumerate(operations):
        result = op(10, 2)
        print(f"  연산 {i+1}: 10, 2 → {result}")
    
    # 람다 정렬
    print("\n[람다로 정렬]")
    students = [
        {'name': 'Alice', 'age': 25},
        {'name': 'Bob', 'age': 20},
        {'name': 'Charlie', 'age': 23}
    ]
    
    sorted_by_age = sorted(students, key=lambda s: s['age'])
    print("  나이순:", [s['name'] for s in sorted_by_age])
    
    print("\n💡 람다는 간단한 함수를 한 줄로 만들 수 있습니다!")

# ============================================================================
# 3단계: 클로저 (Closure)
# ============================================================================
# 
# 함수가 자신이 정의된 환경(스코프)을 기억하는 것
# 
# C에는 없는 강력한 기능!
# C++의 람다 캡처와 유사하지만 더 자연스러움
# 
# ============================================================================

def make_multiplier(n):
    """n을 곱하는 함수를 생성"""
    def multiplier(x):
        return x * n  # n을 기억!
    return multiplier

def make_counter():
    """카운터 함수 생성"""
    count = 0
    
    def counter():
        nonlocal count  # 외부 변수 수정
        count += 1
        return count
    
    return counter

def step3_closure():
    print("\n")
    print("=" * 40)
    print("  3단계: 클로저 (Closure)")
    print("=" * 40)
    print()
    
    # 곱셈 클로저
    print("[곱셈 클로저]")
    times2 = make_multiplier(2)
    times10 = make_multiplier(10)
    
    print(f"5 * 2 = {times2(5)}")
    print(f"5 * 10 = {times10(5)}")
    
    # 카운터 클로저
    print("\n[카운터 클로저]")
    counter1 = make_counter()
    counter2 = make_counter()  # 독립적인 카운터
    
    print(f"Counter 1: {counter1()}")
    print(f"Counter 1: {counter1()}")
    print(f"Counter 2: {counter2()}")
    print(f"Counter 1: {counter1()}")
    
    print("\n💡 클로저는 상태를 가진 함수를 만들 수 있습니다!")

# ============================================================================
# 4단계: 데코레이터
# ============================================================================
# 
# 함수를 꾸며주는(decorate) 함수
# @ 기호로 사용
# 
# C의 Wrapper 패턴을 언어 레벨에서 지원!
# 
# ============================================================================

def timing_decorator(func):
    """함수 실행 시간을 측정하는 데코레이터"""
    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f"  [{func.__name__}] 실행 시간: {(end-start)*1000:.2f}ms")
        return result
    return wrapper

def logging_decorator(func):
    """함수 호출을 로깅하는 데코레이터"""
    @wraps(func)
    def wrapper(*args, **kwargs):
        print(f"  [LOG] {func.__name__} 호출")
        result = func(*args, **kwargs)
        print(f"  [LOG] {func.__name__} 반환: {result}")
        return result
    return wrapper

@timing_decorator
def slow_function():
    """느린 함수 시뮬레이션"""
    total = sum(range(1000000))
    return total

@logging_decorator
def add_numbers(a, b):
    """두 수를 더함"""
    return a + b

# 데코레이터 팩토리
def repeat(times):
    """함수를 n번 반복 실행하는 데코레이터"""
    def decorator(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            results = []
            for i in range(times):
                print(f"    실행 {i+1}/{times}")
                result = func(*args, **kwargs)
                results.append(result)
            return results
        return wrapper
    return decorator

@repeat(3)
def say_hello(name):
    print(f"      Hello, {name}!")
    return True

def step4_decorator():
    print("\n")
    print("=" * 40)
    print("  4단계: 데코레이터")
    print("=" * 40)
    print()
    
    # 타이밍 데코레이터
    print("[타이밍 데코레이터]")
    result = slow_function()
    
    # 로깅 데코레이터
    print("\n[로깅 데코레이터]")
    result = add_numbers(10, 20)
    
    # 반복 데코레이터
    print("\n[반복 데코레이터]")
    results = say_hello("Python")
    
    print("\n💡 데코레이터는 함수를 쉽게 확장할 수 있게 해줍니다!")

# ============================================================================
# 5단계: callable 객체
# ============================================================================
# 
# __call__ 메서드를 구현하면 객체를 함수처럼 호출 가능
# C++의 함수 객체(Functor)와 동일한 개념
# 
# ============================================================================

class Adder:
    """덧셈 함수 객체"""
    def __init__(self, offset):
        self.offset = offset
    
    def __call__(self, x):
        return x + self.offset

class Counter:
    """카운터 함수 객체"""
    def __init__(self):
        self.count = 0
    
    def __call__(self):
        self.count += 1
        print(f"    Count: {self.count}")
        return self.count
    
    def reset(self):
        self.count = 0

def step5_callable():
    print("\n")
    print("=" * 40)
    print("  5단계: callable 객체")
    print("=" * 40)
    print()
    
    # Adder 사용
    print("[Adder 객체]")
    add10 = Adder(10)
    add100 = Adder(100)
    
    print(f"5 + 10 = {add10(5)}")
    print(f"5 + 100 = {add100(5)}")
    
    # Counter 사용
    print("\n[Counter 객체]")
    counter = Counter()
    counter()
    counter()
    counter()
    print(f"    Total: {counter.count}")
    
    # callable 확인
    print("\n[callable 확인]")
    print(f"  add10은 callable? {callable(add10)}")
    print(f"  counter는 callable? {callable(counter)}")
    
    print("\n💡 __call__로 함수처럼 동작하는 객체를 만들 수 있습니다!")

# ============================================================================
# 6단계: functools 활용
# ============================================================================

def divide(a, b):
    return a / b

def print_message(prefix, message):
    print(f"{prefix}: {message}")

def step6_functools():
    print("\n")
    print("=" * 40)
    print("  6단계: functools 활용")
    print("=" * 40)
    print()
    
    # partial - 부분 적용
    print("[functools.partial]")
    divide_by_2 = partial(divide, b=2)
    print(f"10 / 2 = {divide_by_2(10)}")
    print(f"20 / 2 = {divide_by_2(20)}")
    
    # 메시지 프리픽스 고정
    print("\n[메시지 프리픽스 고정]")
    info = partial(print_message, "[INFO]")
    error = partial(print_message, "[ERROR]")
    
    info("시스템 시작")
    error("오류 발생!")
    
    # reduce - 누적 계산
    from functools import reduce
    print("\n[functools.reduce]")
    numbers = [1, 2, 3, 4, 5]
    sum_all = reduce(lambda a, b: a + b, numbers)
    product = reduce(lambda a, b: a * b, numbers)
    
    print(f"합계: {sum_all}")
    print(f"곱: {product}")
    
    print("\n💡 functools는 함수형 프로그래밍을 도와줍니다!")

# ============================================================================
# 7단계: 실무 예제
# ============================================================================

# Observer Pattern
class Observable:
    """옵저버 패턴"""
    def __init__(self):
        self._observers: List[Callable] = []
    
    def attach(self, observer: Callable):
        self._observers.append(observer)
    
    def detach(self, observer: Callable):
        self._observers.remove(observer)
    
    def notify(self, data: Any):
        print(f"[Observable] {len(self._observers)}개 옵저버에게 통지")
        for observer in self._observers:
            observer(data)

# Command Pattern
@dataclass
class Command:
    """명령 패턴"""
    execute: Callable
    undo: Callable
    
    def do(self):
        self.execute()
    
    def undo_action(self):
        self.undo()

class CommandInvoker:
    """명령 실행자"""
    def __init__(self):
        self.history: List[Command] = []
    
    def execute(self, command: Command):
        print("[Invoker] 명령 실행")
        command.do()
        self.history.append(command)
    
    def undo(self):
        if self.history:
            print("[Invoker] 명령 취소")
            command = self.history.pop()
            command.undo_action()

# Strategy Pattern
class Sorter:
    """전략 패턴"""
    def __init__(self):
        self.strategy: Callable = None
    
    def set_strategy(self, strategy: Callable):
        self.strategy = strategy
    
    def sort(self, data: List):
        if self.strategy:
            self.strategy(data)

def step7_real_world():
    print("\n")
    print("=" * 40)
    print("  7단계: 실무 예제")
    print("=" * 40)
    print()
    
    # Observer Pattern
    print("[Observer Pattern]")
    observable = Observable()
    
    observable.attach(lambda data: print(f"  → Observer 1: {data}"))
    observable.attach(lambda data: print(f"  → Observer 2: {data * 2}"))
    
    observable.notify(10)
    
    # Command Pattern
    print("\n[Command Pattern]")
    value = [0]  # 리스트로 감싸서 클로저에서 수정 가능하게
    
    increment = Command(
        execute=lambda: (value.__setitem__(0, value[0] + 1),
                        print(f"  Value++ = {value[0]}"))[-1],
        undo=lambda: (value.__setitem__(0, value[0] - 1),
                     print(f"  Value-- = {value[0]}"))[-1]
    )
    
    invoker = CommandInvoker()
    invoker.execute(increment)
    invoker.execute(Command(
        execute=lambda: (value.__setitem__(0, value[0] + 10),
                        print(f"  Value+10 = {value[0]}"))[-1],
        undo=lambda: (value.__setitem__(0, value[0] - 10),
                     print(f"  Value-10 = {value[0]}"))[-1]
    ))
    
    invoker.undo()
    invoker.undo()
    
    # Strategy Pattern
    print("\n[Strategy Pattern]")
    sorter = Sorter()
    numbers = [5, 2, 8, 1, 9]
    
    print(f"원본: {numbers}")
    
    sorter.set_strategy(lambda data: data.sort())
    sorter.sort(numbers)
    print(f"오름차순: {numbers}")
    
    sorter.set_strategy(lambda data: data.sort(reverse=True))
    sorter.sort(numbers)
    print(f"내림차순: {numbers}")
    
    print("\n💡 Python의 일급 함수로 디자인 패턴이 매우 간결합니다!")

# ============================================================================
# 8단계: C vs Python 비교
# ============================================================================

def step8_comparison():
    print("\n")
    print("=" * 40)
    print("  8단계: C vs Python 비교")
    print("=" * 40)
    print()
    
    print("함수 포인터 비교:")
    print("\n[C 버전]")
    print("  void (*fp)(int);")
    print("  fp = my_function;")
    print("  fp(10);")
    
    print("\n[Python 버전]")
    print("  fp = my_function  # 그냥 할당!")
    print("  fp(10)")
    
    print("\n\n특징 비교:")
    
    print("\nC 함수 포인터:")
    print("  ✓ 빠르고 가벼움")
    print("  ✓ 임베디드에 최적")
    print("  ✗ 문법 복잡")
    print("  ✗ 타입 안전성 낮음")
    
    print("\nPython 일급 함수:")
    print("  ✓ 매우 간단하고 직관적")
    print("  ✓ 강력한 기능 (클로저, 데코레이터)")
    print("  ✓ 동적 타입")
    print("  ✗ 느림 (인터프리터)")
    print("  ✗ 메모리 사용량 높음")
    
    print("\n\n언제 무엇을 사용할까?")
    print("  • 임베디드, 실시간 → C")
    print("  • 웹, 데이터 분석 → Python")
    print("  • 시스템 프로그래밍 → C/C++")
    print("  • 빠른 프로토타입 → Python")
    print("  • AI/ML → Python")

# ============================================================================
# 메인 함수
# ============================================================================

def main():
    print("\n")
    print("╔" + "=" * 60 + "╗")
    print("║" + " " * 60 + "║")
    print("║" + "     Python 일급 함수와 콜백 완전 정복".center(60) + "║")
    print("║" + "     (C 함수 포인터의 Pythonic 대안)".center(60) + "║")
    print("║" + " " * 60 + "║")
    print("╚" + "=" * 60 + "╝")
    
    step1_first_class_function()
    step2_lambda()
    step3_closure()
    step4_decorator()
    step5_callable()
    step6_functools()
    step7_real_world()
    step8_comparison()
    
    print("\n")
    print("=" * 40)
    print("  학습 완료!")
    print("=" * 40)
    print()
    print("Python의 일급 함수는")
    print("C의 함수 포인터보다 훨씬 유연하고 표현력이 높습니다!")
    print("\n다음 학습:")
    print("  01_wrapper_pattern.py")
    print("  04_callback_pattern.py")
    print("  05_state_machine.py")
    print()

if __name__ == "__main__":
    main()

"""
============================================================================
실행 방법
============================================================================

실행:
  python 00_function_pointer_basics.py

타입 체크 (optional):
  mypy 00_function_pointer_basics.py

============================================================================
C vs Python 최종 정리
============================================================================

C 함수 포인터:
  - 빠르고 효율적
  - 임베디드에 최적
  - 문법이 복잡

Python 일급 함수:
  - 매우 간단하고 직관적
  - 클로저, 데코레이터 등 강력한 기능
  - 동적이고 유연함
  - 느리지만 대부분 충분히 빠름

실무 조언:
  - 성능 중요 → C/C++
  - 생산성 중요 → Python
  - 프로토타입 → Python
  - 프로덕션 → 상황에 따라

============================================================================
"""
