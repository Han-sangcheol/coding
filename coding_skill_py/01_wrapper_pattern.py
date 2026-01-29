"""
============================================================================
파일명: 01_wrapper_pattern.py
기능: Python Wrapper(래퍼) 패턴 - 데코레이터와 컨텍스트 매니저

C 버전과의 차이점:
  - 수동 open/close → with 문 (컨텍스트 매니저)
  - 함수 포인터 → 데코레이터
  - 구조체 → 클래스
  - 수동 메모리 관리 → 가비지 컬렉터

Python 추가 기능:
  - 데코레이터 (@decorator)
  - 컨텍스트 매니저 (with문)
  - __getattr__로 동적 래핑
  - property 데코레이터
  - functools.wraps

============================================================================
"""

from typing import Callable, Any, TypeVar, Generic
from functools import wraps
from contextlib import contextmanager
import time
from datetime import datetime
from abc import ABC, abstractmethod

# ============================================================================
# 1. 컨텍스트 매니저 - 파일 래퍼
# ============================================================================
# 
# Python의 with 문은 자동 리소스 관리
# __enter__와 __exit__로 RAII와 동일한 효과
# 
# ============================================================================

class ManagedFile:
    """파일 자동 관리 래퍼"""
    def __init__(self, filename: str, mode: str = 'r'):
        self.filename = filename
        self.mode = mode
        self.file = None
    
    def __enter__(self):
        print(f"[ManagedFile] Opening: {self.filename}")
        self.file = open(self.filename, self.mode)
        return self.file
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        if self.file:
            self.file.close()
            print(f"[ManagedFile] Closed: {self.filename}")
        return False  # 예외를 다시 발생시킴

def demo_context_manager():
    print("\n=== 컨텍스트 매니저 파일 래퍼 ===")
    
    with ManagedFile("test.txt", "w") as f:
        f.write("Hello from Python!\n")
        f.write("Context manager is awesome!\n")
    # 여기서 자동으로 파일 닫힘
    
    print("\n파일이 자동으로 닫혔습니다!")

# ============================================================================
# 2. 데코레이터 - 함수 래핑
# ============================================================================
# 
# Python의 데코레이터는 함수를 감싸는 가장 간단한 방법
# 
# ============================================================================

def logging_wrapper(func: Callable) -> Callable:
    """로깅 데코레이터"""
    @wraps(func)
    def wrapper(*args, **kwargs):
        print(f"[LOG] Calling {func.__name__}({args}, {kwargs})")
        result = func(*args, **kwargs)
        print(f"[LOG] {func.__name__} returned: {result}")
        return result
    return wrapper

def timing_wrapper(func: Callable) -> Callable:
    """성능 측정 데코레이터"""
    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        result = func(*args, **kwargs)
        end = time.perf_counter()
        duration = (end - start) * 1000000  # microseconds
        print(f"[Timing] {func.__name__} took {duration:.2f} μs")
        return result
    return wrapper

@logging_wrapper
@timing_wrapper
def add(a: int, b: int) -> int:
    """두 수를 더함"""
    return a + b

def demo_decorator():
    print("\n=== 데코레이터 함수 래핑 ===")
    
    result = add(10, 20)
    print(f"Result: {result}")

# ============================================================================
# 3. 클래스 데코레이터
# ============================================================================

def singleton(cls):
    """싱글톤 패턴 데코레이터"""
    instances = {}
    
    @wraps(cls)
    def get_instance(*args, **kwargs):
        if cls not in instances:
            print(f"[Singleton] Creating {cls.__name__} instance")
            instances[cls] = cls(*args, **kwargs)
        else:
            print(f"[Singleton] Returning existing {cls.__name__} instance")
        return instances[cls]
    
    return get_instance

@singleton
class Database:
    """싱글톤 데이터베이스"""
    def __init__(self):
        self.connection = "Connected"
    
    def query(self, sql: str):
        return f"Query: {sql}"

def demo_class_decorator():
    print("\n=== 클래스 데코레이터 (싱글톤) ===")
    
    db1 = Database()
    db2 = Database()
    
    print(f"Same instance? {db1 is db2}")

# ============================================================================
# 4. __getattr__를 이용한 동적 래핑
# ============================================================================

class LoggingProxy:
    """모든 메서드 호출을 로깅하는 프록시"""
    def __init__(self, obj: Any):
        self._obj = obj
    
    def __getattr__(self, name: str):
        attr = getattr(self._obj, name)
        
        if callable(attr):
            def wrapper(*args, **kwargs):
                print(f"[Proxy] Calling {name}")
                result = attr(*args, **kwargs)
                print(f"[Proxy] {name} returned: {result}")
                return result
            return wrapper
        else:
            return attr

class Calculator:
    """간단한 계산기"""
    def add(self, a: int, b: int) -> int:
        return a + b
    
    def multiply(self, a: int, b: int) -> int:
        return a * b

def demo_dynamic_wrapping():
    print("\n=== 동적 래핑 (__getattr__) ===")
    
    calc = Calculator()
    proxied = LoggingProxy(calc)
    
    result1 = proxied.add(5, 3)
    result2 = proxied.multiply(4, 7)

# ============================================================================
# 5. 인터페이스 기반 래퍼
# ============================================================================

class ILogger(ABC):
    """로거 인터페이스"""
    @abstractmethod
    def log(self, message: str):
        pass

class ConsoleLogger(ILogger):
    """콘솔 로거"""
    def log(self, message: str):
        print(f"[Console] {message}")

class FileLogger(ILogger):
    """파일 로거"""
    def __init__(self, filename: str):
        self.filename = filename
    
    def log(self, message: str):
        with open(self.filename, 'a') as f:
            f.write(f"[File] {message}\n")

class TimestampLoggerWrapper(ILogger):
    """타임스탬프를 추가하는 로거 래퍼"""
    def __init__(self, logger: ILogger):
        self.logger = logger
    
    def log(self, message: str):
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        self.logger.log(f"[{timestamp}] {message}")

def demo_interface_wrapper():
    print("\n=== 인터페이스 기반 래퍼 ===")
    
    console = ConsoleLogger()
    wrapped = TimestampLoggerWrapper(console)
    
    wrapped.log("시스템 시작")
    wrapped.log("작업 완료")

# ============================================================================
# 6. property를 이용한 지연 초기화
# ============================================================================

class ExpensiveResource:
    """비용이 큰 리소스"""
    def __init__(self):
        print("[ExpensiveResource] 초기화 중...")
        time.sleep(0.1)  # 초기화 시뮬레이션
    
    def do_work(self):
        print("[ExpensiveResource] 작업 수행")

class ResourceProxy:
    """지연 초기화 프록시"""
    def __init__(self):
        print("[Proxy] 생성 (리소스는 아직 생성 안됨)")
        self._resource = None
    
    @property
    def resource(self):
        """첫 접근 시 자동으로 생성"""
        if self._resource is None:
            print("[Proxy] 첫 접근! 리소스 생성")
            self._resource = ExpensiveResource()
        return self._resource
    
    def do_work(self):
        self.resource.do_work()

def demo_lazy_init():
    print("\n=== 지연 초기화 (property) ===")
    
    proxy = ResourceProxy()
    print("\n첫 번째 호출:")
    proxy.do_work()
    
    print("\n두 번째 호출:")
    proxy.do_work()

# ============================================================================
# 7. contextlib를 이용한 간편한 컨텍스트 매니저
# ============================================================================

@contextmanager
def managed_resource(name: str):
    """컨텍스트 매니저 제너레이터"""
    print(f"[Resource] Acquiring: {name}")
    resource = f"Resource-{name}"
    try:
        yield resource
    finally:
        print(f"[Resource] Releasing: {name}")

def demo_contextlib():
    print("\n=== contextlib 데코레이터 ===")
    
    with managed_resource("Database") as db:
        print(f"  Using {db}")
    
    print("\n자동으로 해제되었습니다!")

# ============================================================================
# 8. 메서드 체이닝을 위한 래퍼
# ============================================================================

class FluentCalculator:
    """메서드 체이닝을 지원하는 계산기"""
    def __init__(self, value: int = 0):
        self._value = value
    
    def add(self, n: int) -> 'FluentCalculator':
        print(f"  {self._value} + {n}")
        self._value += n
        return self
    
    def multiply(self, n: int) -> 'FluentCalculator':
        print(f"  {self._value} * {n}")
        self._value *= n
        return self
    
    def result(self) -> int:
        return self._value

def demo_fluent_interface():
    print("\n=== 메서드 체이닝 (Fluent Interface) ===")
    
    result = (FluentCalculator(10)
              .add(5)
              .multiply(2)
              .add(3)
              .result())
    
    print(f"\nFinal result: {result}")

# ============================================================================
# 9. 데코레이터 팩토리
# ============================================================================

def retry(max_attempts: int):
    """재시도 데코레이터 팩토리"""
    def decorator(func: Callable) -> Callable:
        @wraps(func)
        def wrapper(*args, **kwargs):
            for attempt in range(1, max_attempts + 1):
                try:
                    print(f"[Retry] Attempt {attempt}/{max_attempts}")
                    return func(*args, **kwargs)
                except Exception as e:
                    if attempt == max_attempts:
                        print(f"[Retry] Failed after {max_attempts} attempts")
                        raise
                    print(f"[Retry] Failed: {e}, retrying...")
            return None
        return wrapper
    return decorator

@retry(max_attempts=3)
def unstable_function(success_rate: float = 0.5):
    """불안정한 함수 시뮬레이션"""
    import random
    if random.random() > success_rate:
        raise Exception("Random failure")
    return "Success!"

def demo_retry():
    print("\n=== 재시도 데코레이터 ===")
    
    try:
        result = unstable_function(0.7)
        print(f"Result: {result}")
    except Exception as e:
        print(f"Failed: {e}")

# ============================================================================
# 메인 함수
# ============================================================================

def main():
    print("\n")
    print("╔" + "=" * 60 + "╗")
    print("║" + " " * 60 + "║")
    print("║" + "     Python Wrapper 패턴".center(60) + "║")
    print("║" + "     (데코레이터와 컨텍스트 매니저)".center(60) + "║")
    print("║" + " " * 60 + "║")
    print("╚" + "=" * 60 + "╝")
    
    demo_context_manager()
    demo_decorator()
    demo_class_decorator()
    demo_dynamic_wrapping()
    demo_interface_wrapper()
    demo_lazy_init()
    demo_contextlib()
    demo_fluent_interface()
    demo_retry()
    
    print("\n")
    print("=" * 40)
    print("  Python Wrapper 패턴 완료!")
    print("=" * 40)
    print("\nPython의 데코레이터와 with 문은")
    print("C의 Wrapper 패턴을 언어 레벨에서 지원합니다!")
    print()

if __name__ == "__main__":
    main()

"""
============================================================================
실행 방법
============================================================================

실행:
  python 01_wrapper_pattern.py

============================================================================
C vs Python Wrapper 비교
============================================================================

C Wrapper:
  - 수동 open/close
  - 함수 포인터로 추상화
  - 메모리 누수 위험

Python Wrapper:
  - with 문으로 자동 관리
  - 데코레이터로 간편한 래핑
  - 가비지 컬렉터
  - __getattr__로 동적 래핑

핵심:
  Python에서 Wrapper = 데코레이터 + 컨텍스트 매니저

============================================================================
"""
