"""
Python Adapter Pattern - Duck Typing과 ABC
C 버전: 구조체 + 함수 포인터
Python 버전: ABC, Protocol, Duck Typing
"""

from abc import ABC, abstractmethod
from typing import Protocol

# 타겟 인터페이스 (ABC 사용)
class ICelsiusSensor(ABC):
    @abstractmethod
    def read_celsius(self) -> float:
        pass
    
    @abstractmethod
    def get_name(self) -> str:
        pass

# Adaptee 1: 화씨 센서
class FahrenheitSensor:
    def read_fahrenheit(self) -> float:
        return 77.0
    
    def get_sensor_name(self) -> str:
        return "Fahrenheit Sensor"

# Adaptee 2: 켈빈 센서
class KelvinSensor:
    def get_kelvin(self) -> float:
        return 300.0
    
    def id(self) -> str:
        return "Kelvin Sensor"

# Adapter 1: 화씨 → 섭씨
class FahrenheitAdapter(ICelsiusSensor):
    def __init__(self):
        self.sensor = FahrenheitSensor()
    
    def read_celsius(self) -> float:
        f = self.sensor.read_fahrenheit()
        return (f - 32.0) * 5.0 / 9.0
    
    def get_name(self) -> str:
        return self.sensor.get_sensor_name()

# Adapter 2: 켈빈 → 섭씨
class KelvinAdapter(ICelsiusSensor):
    def __init__(self):
        self.sensor = KelvinSensor()
    
    def read_celsius(self) -> float:
        return self.sensor.get_kelvin() - 273.15
    
    def get_name(self) -> str:
        return self.sensor.id()

# 데코레이터를 이용한 Adapter (Python 특화)
def celsius_adapter(fahrenheit_func):
    """화씨 함수를 섭씨로 변환하는 데코레이터"""
    def wrapper(*args, **kwargs):
        f = fahrenheit_func(*args, **kwargs)
        return (f - 32.0) * 5.0 / 9.0
    return wrapper

@celsius_adapter
def get_room_temperature():
    return 72.0  # 화씨

# 클라이언트 코드
def process_temperature(sensor: ICelsiusSensor):
    print(f"{sensor.get_name()}: {sensor.read_celsius():.2f}°C")

def main():
    print("\n=== Python Adapter Pattern ===")
    
    f_adapter = FahrenheitAdapter()
    k_adapter = KelvinAdapter()
    
    process_temperature(f_adapter)
    process_temperature(k_adapter)
    
    print("\n데코레이터 Adapter:")
    print(f"Room: {get_room_temperature():.2f}°C")
    
    print("\nDuck Typing으로 유연한 인터페이스!")

if __name__ == "__main__":
    main()
