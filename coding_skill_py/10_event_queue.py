"""Python Event Queue - queue.Queue와 asyncio"""
from queue import Queue
from typing import Callable
import asyncio

class EventQueue:
    def __init__(self):
        self.events: Queue[Callable] = Queue()
    
    def push(self, event: Callable):
        self.events.put(event)
        print(f"[Queue] 이벤트 추가 (크기: {self.events.qsize()})")
    
    def process(self):
        while not self.events.empty():
            print("[Queue] 이벤트 처리")
            event = self.events.get()
            event()

# Async 버전
class AsyncEventQueue:
    def __init__(self):
        self.queue = asyncio.Queue()
    
    async def push(self, event: Callable):
        await self.queue.put(event)
    
    async def process(self):
        while not self.queue.empty():
            event = await self.queue.get()
            await event()

def main():
    print("\n=== Python Event Queue ===")
    
    queue = EventQueue()
    
    queue.push(lambda: print("  → Event 1"))
    queue.push(lambda: print("  → Event 2"))
    queue.push(lambda: print("  → Event 3"))
    
    queue.process()

if __name__ == "__main__":
    main()
