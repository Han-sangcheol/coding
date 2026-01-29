"""Python Command - dataclass"""
from dataclasses import dataclass
from typing import Callable, List

@dataclass
class Command:
    execute: Callable
    undo: Callable

class Invoker:
    def __init__(self):
        self.history: List[Command] = []
    
    def execute(self, command: Command):
        command.execute()
        self.history.append(command)
    
    def undo(self):
        if self.history:
            command = self.history.pop()
            command.undo()

def main():
    print("\n=== Python Command ===")
    value = [0]
    
    invoker = Invoker()
    invoker.execute(Command(
        execute=lambda: (value.__setitem__(0, value[0] + 1), print(f"Value++ = {value[0]}"))[-1],
        undo=lambda: (value.__setitem__(0, value[0] - 1), print(f"Value-- = {value[0]}"))[-1]
    ))
    
    invoker.execute(Command(
        execute=lambda: (value.__setitem__(0, value[0] + 10), print(f"Value+10 = {value[0]}"))[-1],
        undo=lambda: (value.__setitem__(0, value[0] - 10), print(f"Value-10 = {value[0]}"))[-1]
    ))
    
    invoker.undo()
    invoker.undo()

if __name__ == "__main__":
    main()
