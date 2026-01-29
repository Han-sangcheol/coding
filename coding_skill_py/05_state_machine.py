"""Python State Machine - Enum + dict"""
from enum import Enum, auto
from typing import Callable, Dict

class State(Enum):
    IDLE = auto()
    RUNNING = auto()
    STOPPED = auto()

class StateMachine:
    def __init__(self):
        self.current = State.IDLE
        self.on_enter: Dict[State, Callable] = {}
    
    def set_on_enter(self, state: State, callback: Callable):
        self.on_enter[state] = callback
    
    def transition(self, next_state: State):
        print(f"[SM] {self.current.name} → {next_state.name}")
        self.current = next_state
        if next_state in self.on_enter:
            self.on_enter[next_state]()

def main():
    print("\n=== Python State Machine ===")
    
    sm = StateMachine()
    sm.set_on_enter(State.RUNNING, lambda: print("  → 실행 시작!"))
    sm.set_on_enter(State.STOPPED, lambda: print("  → 정지됨"))
    
    sm.transition(State.RUNNING)
    sm.transition(State.STOPPED)
    sm.transition(State.IDLE)

if __name__ == "__main__":
    main()
