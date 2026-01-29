"""Python HAL - ABC"""
from abc import ABC, abstractmethod

class IGPIO(ABC):
    @abstractmethod
    def write(self, pin: int, value: bool):
        pass

class STM32_GPIO(IGPIO):
    def write(self, pin: int, value: bool):
        print(f"[STM32] GPIO {pin} = {value}")

class Nordic_GPIO(IGPIO):
    def write(self, pin: int, value: bool):
        print(f"[Nordic] GPIO {pin} = {value}")

def app_toggle_led(hal: IGPIO):
    hal.write(5, True)

def main():
    print("=== Python HAL ===")
    stm32 = STM32_GPIO()
    nordic = Nordic_GPIO()
    
    app_toggle_led(stm32)
    app_toggle_led(nordic)

if __name__ == "__main__":
    main()
