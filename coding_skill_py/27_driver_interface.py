"""Python Driver Interface - Protocol"""
from typing import Protocol

class IDriver(Protocol):
    def open(self) -> int: ...
    def close(self) -> int: ...
    def read(self, buf: bytearray, length: int) -> int: ...
    def write(self, buf: bytes, length: int) -> int: ...

class UARTDriver:
    def open(self) -> int:
        print("[UART] Open")
        return 0
    
    def close(self) -> int:
        print("[UART] Close")
        return 0
    
    def read(self, buf: bytearray, length: int) -> int:
        return length
    
    def write(self, buf: bytes, length: int) -> int:
        return length

def main():
    print("=== Python Driver Interface ===")
    uart = UARTDriver()
    uart.open()
    uart.close()

if __name__ == "__main__":
    main()
