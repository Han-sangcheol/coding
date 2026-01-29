"""Python Watchdog - time 모듈"""
import time

class Watchdog:
    def __init__(self, timeout: int):
        self.timeout = timeout
        self.kick()
    
    def kick(self):
        self.last_kick = time.time()
    
    def check(self) -> bool:
        elapsed = time.time() - self.last_kick
        return elapsed < self.timeout

def main():
    print("=== Python Watchdog ===")
    wd = Watchdog(3)
    print(f"Status: {'OK' if wd.check() else 'Timeout'}")

if __name__ == "__main__":
    main()
