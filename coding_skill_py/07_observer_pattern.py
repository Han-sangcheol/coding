"""Python Observer - weakref와 property"""
from typing import List, Callable, Generic, TypeVar
import weakref

T = TypeVar('T')

class Subject(Generic[T]):
    def __init__(self):
        self._observers: List[Callable] = []
        self._data: T = None
    
    def attach(self, observer: Callable):
        self._observers.append(observer)
    
    def detach(self, observer: Callable):
        self._observers.remove(observer)
    
    @property
    def data(self) -> T:
        return self._data
    
    @data.setter
    def data(self, value: T):
        self._data = value
        self._notify()
    
    def _notify(self):
        print(f"[Subject] 통지: {len(self._observers)}명")
        for obs in self._observers:
            obs(self._data)

def main():
    print("\n=== Python Observer Pattern ===")
    
    subject = Subject[int]()
    
    subject.attach(lambda val: print(f"  → Observer 1: {val}"))
    subject.attach(lambda val: print(f"  → Observer 2: {val * 2}"))
    
    subject.data = 10
    subject.data = 20

if __name__ == "__main__":
    main()
