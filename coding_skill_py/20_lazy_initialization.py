"""Python Lazy Init - property"""
from functools import cached_property

class LazyResource:
    @cached_property
    def data(self):
        print("[Lazy] 첫 접근! 초기화")
        return 100

def main():
    print("=== Python Lazy Initialization ===")
    res = LazyResource()
    print(f"1st access: {res.data}")
    print(f"2nd access: {res.data}")

if __name__ == "__main__":
    main()
