"""Python Mock Object - unittest.mock"""
from unittest.mock import Mock
from abc import ABC, abstractmethod

class ISensor(ABC):
    @abstractmethod
    def read(self) -> float:
        pass

class RealSensor(ISensor):
    def read(self) -> float:
        print("[Real] Hardware read")
        return 25.5

class MockSensor(ISensor):
    def read(self) -> float:
        print("[Mock] Test data")
        return 20.0

def process_temperature(sensor: ISensor):
    temp = sensor.read()
    print(f"[App] Temperature: {temp}°C")

def main():
    print("=== Python Mock Object ===")
    
    real = RealSensor()
    mock = MockSensor()
    
    process_temperature(real)
    process_temperature(mock)
    
    # unittest.mock 사용
    mock_sensor = Mock(spec=ISensor)
    mock_sensor.read.return_value = 22.0
    print("\n[unittest.mock]")
    temp = mock_sensor.read()
    print(f"Mocked temp: {temp}")

if __name__ == "__main__":
    main()
