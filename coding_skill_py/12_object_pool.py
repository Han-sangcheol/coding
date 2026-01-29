"""Python Object Pool"""
from typing import List, Generic, TypeVar, Callable

T = TypeVar('T')

class ObjectPool(Generic[T]):
    def __init__(self, factory: Callable[[], T], size: int):
        self.available: List[T] = [factory() for _ in range(size)]
        self.in_use: List[T] = []
    
    def acquire(self) -> T:
        if not self.available:
            return None
        obj = self.available.pop()
        self.in_use.append(obj)
        return obj
    
    def release(self, obj: T):
        if obj in self.in_use:
            self.in_use.remove(obj)
            self.available.append(obj)

class Packet:
    def __init__(self):
        self.id = 0
        self.data = ""

def main():
    print("=== Python Object Pool ===")
    pool = ObjectPool(Packet, 3)
    
    p1 = pool.acquire()
    p1.id = 1
    print(f"Acquired packet {p1.id}")
    
    pool.release(p1)
    print("Released")

if __name__ == "__main__":
    main()
