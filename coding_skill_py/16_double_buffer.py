"""Python Double Buffer - 리스트 스왑"""
from typing import List, TypeVar, Generic

T = TypeVar('T')

class DoubleBuffer(Generic[T]):
    def __init__(self, size: int):
        self.buffer_a: List[T] = [None] * size
        self.buffer_b: List[T] = [None] * size
        self.front = self.buffer_a
        self.back = self.buffer_b
    
    def write(self, data: T, index: int):
        if 0 <= index < len(self.back):
            self.back[index] = data
    
    def read(self, index: int) -> T:
        if 0 <= index < len(self.front):
            return self.front[index]
        return None
    
    def swap(self):
        self.front, self.back = self.back, self.front
        print("[DoubleBuffer] Swapped!")

def main():
    print("=== Python Double Buffer ===")
    db = DoubleBuffer[int](5)
    
    db.write(10, 0)
    db.write(20, 1)
    db.swap()
    
    print(f"Read: {db.read(0)}, {db.read(1)}")

if __name__ == "__main__":
    main()
