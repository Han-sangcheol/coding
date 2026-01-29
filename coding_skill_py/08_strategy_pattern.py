"""Python Strategy - 일급 함수"""
from typing import Callable, List

class Sorter:
    def __init__(self):
        self.strategy: Callable = None
    
    def set_strategy(self, strategy: Callable):
        self.strategy = strategy
    
    def sort(self, data: List[int]):
        if self.strategy:
            self.strategy(data)

def main():
    print("\n=== Python Strategy ===")
    sorter = Sorter()
    nums = [5, 2, 8, 1]
    
    sorter.set_strategy(lambda d: d.sort())
    sorter.sort(nums)
    print(f"오름차순: {nums}")
    
    sorter.set_strategy(lambda d: d.sort(reverse=True))
    sorter.sort(nums)
    print(f"내림차순: {nums}")

if __name__ == "__main__":
    main()
