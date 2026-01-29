"""Python Ring Buffer - collections.deque"""
from collections import deque
from typing import Generic, TypeVar

T = TypeVar('T')

class RingBuffer(Generic[T]):
    def __init__(self, maxsize: int):
        self.buffer = deque(maxlen=maxsize)
    
    def push(self, item: T):
        self.buffer.append(item)
    
    def pop(self) -> T:
        return self.buffer.popleft()
    
    def __len__(self):
        return len(self.buffer)

def main():
    print("=== Python Ring Buffer ===")
    rb = RingBuffer[int](5)
    
    for i in range(1, 4):
        rb.push(i * 10)
    
    while len(rb) > 0:
        print(f"Pop: {rb.pop()}")

if __name__ == "__main__":
    main()
