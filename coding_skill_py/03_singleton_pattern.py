"""
Python Singleton Pattern - 메타클래스와 데코레이터
C 버전: static 변수 + 초기화 플래그
Python 버전: 메타클래스, __new__, 데코레이터
"""

from functools import wraps
from threading import Lock

# 1. 메타클래스를 이용한 Singleton
class SingletonMeta(type):
    _instances = {}
    _lock = Lock()
    
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            with cls._lock:
                if cls not in cls._instances:
                    cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]

class Logger(metaclass=SingletonMeta):
    def __init__(self):
        print("[Logger] 생성")
    
    def log(self, message: str):
        print(f"[LOG] {message}")

# 2. __new__를 이용한 Singleton
class Config:
    _instance = None
    
    def __new__(cls):
        if cls._instance is None:
            print("[Config] 생성")
            cls._instance = super().__new__(cls)
            cls._instance.version = "1.0.0"
        return cls._instance
    
    def get_version(self) -> str:
        return self.version

# 3. 데코레이터를 이용한 Singleton
def singleton(cls):
    instances = {}
    lock = Lock()
    
    @wraps(cls)
    def get_instance(*args, **kwargs):
        if cls not in instances:
            with lock:
                if cls not in instances:
                    instances[cls] = cls(*args, **kwargs)
        return instances[cls]
    
    return get_instance

@singleton
class Database:
    def __init__(self):
        print("[Database] 생성")
    
    def query(self, sql: str):
        print(f"[DB] Query: {sql}")

# 4. 모듈 레벨 Singleton (가장 Pythonic)
# Python에서 모듈은 기본적으로 싱글톤!
class _SystemMonitor:
    def __init__(self):
        print("[SystemMonitor] 생성")
        self.cpu_usage = 0.0
    
    def update(self):
        self.cpu_usage += 1.0
        print(f"[Monitor] CPU: {self.cpu_usage}%")

# 모듈 레벨에서 인스턴스 생성
system_monitor = _SystemMonitor()

def main():
    print("\n=== Python Singleton Pattern ===")
    
    # 메타클래스 Singleton
    print("\n1. 메타클래스 Singleton:")
    logger1 = Logger()
    logger2 = Logger()
    print(f"Same instance? {logger1 is logger2}")
    logger1.log("Message 1")
    
    # __new__ Singleton
    print("\n2. __new__ Singleton:")
    config1 = Config()
    config2 = Config()
    print(f"Same instance? {config1 is config2}")
    print(f"Version: {config1.get_version()}")
    
    # 데코레이터 Singleton
    print("\n3. 데코레이터 Singleton:")
    db1 = Database()
    db2 = Database()
    print(f"Same instance? {db1 is db2}")
    db1.query("SELECT * FROM users")
    
    # 모듈 레벨 Singleton
    print("\n4. 모듈 레벨 Singleton:")
    system_monitor.update()
    system_monitor.update()
    
    print("\n모든 방법으로 싱글톤 구현 가능!")

if __name__ == "__main__":
    main()
