"""Python Reference Counting - 내장 기능"""
import sys

class Buffer:
    def __init__(self):
        print("[Buffer] 생성")
    
    def __del__(self):
        print("[Buffer] 소멸")

def main():
    print("=== Python Reference Counting (내장) ===")
    
    buf1 = Buffer()
    print(f"ref_count: {sys.getrefcount(buf1) - 1}")
    
    buf2 = buf1
    print(f"ref_count: {sys.getrefcount(buf1) - 1}")
    
    del buf2
    print(f"ref_count: {sys.getrefcount(buf1) - 1}")

if __name__ == "__main__":
    main()
