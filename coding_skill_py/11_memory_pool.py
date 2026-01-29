"""Python Memory Pool - queue.Queue 기반 (GC가 있어 불필요하지만 개념 학습용)"""
from queue import Queue

class MemoryPool:
    def __init__(self, size: int):
        self.pool = Queue()
        for i in range(size):
            self.pool.put([None])  # 재사용 가능한 객체
    
    def acquire(self):
        if not self.pool.empty():
            return self.pool.get()
        return None
    
    def release(self, obj):
        obj[0] = None
        self.pool.put(obj)

def main():
    print("=== Python Memory Pool (학습용) ===")
    pool = MemoryPool(3)
    
    obj1 = pool.acquire()
    obj1[0] = 100
    print(f"Acquired: {obj1[0]}")
    
    pool.release(obj1)
    print("Released")

if __name__ == "__main__":
    main()
