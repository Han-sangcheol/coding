"""Python Callback - 일급 함수와 람다"""
from typing import Callable, List

class Button:
    def __init__(self):
        self.on_click = None
    
    def set_on_click(self, callback: Callable):
        self.on_click = callback
    
    def click(self):
        print("[Button] 클릭!")
        if self.on_click:
            self.on_click(1)

class Observable:
    def __init__(self):
        self.observers: List[Callable] = []
    
    def attach(self, callback: Callable):
        self.observers.append(callback)
    
    def notify(self, data):
        print(f"[Observable] 통지: {len(self.observers)}명")
        for obs in self.observers:
            obs(data)

def main():
    print("\n=== Python Callback Pattern ===")
    
    # 람다 콜백
    btn = Button()
    btn.set_on_click(lambda x: print(f"  → 콜백 호출: {x}"))
    btn.click()
    
    # 다중 콜백
    obs = Observable()
    obs.attach(lambda x: print(f"  → Observer 1: {x}"))
    obs.attach(lambda x: print(f"  → Observer 2: {x * 2}"))
    obs.notify(10)

if __name__ == "__main__":
    main()
