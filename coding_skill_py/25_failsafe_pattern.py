"""Python Failsafe - Enum"""
from enum import Enum, auto

class Mode(Enum):
    NORMAL = auto()
    DEGRADED = auto()
    SAFE = auto()

class System:
    def __init__(self):
        self.mode = Mode.NORMAL
        self.errors = 0
    
    def report_error(self):
        self.errors += 1
        if self.errors >= 3 and self.mode == Mode.NORMAL:
            self.mode = Mode.DEGRADED
            print("[Failsafe] → DEGRADED")
    
    def operate(self):
        print(f"[System] Mode: {self.mode.name}")

def main():
    print("=== Python Failsafe ===")
    sys = System()
    for i in range(5):
        sys.report_error()
    sys.operate()

if __name__ == "__main__":
    main()
