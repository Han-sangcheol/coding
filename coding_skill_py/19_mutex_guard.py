"""Python Mutex Guard - with Lock()"""
from threading import Lock

lock = Lock()

def critical_section():
    with lock:  # 자동 acquire
        print("[Critical] 보호된 영역")
    # 자동 release

def main():
    print("=== Python Mutex Guard ===")
    critical_section()
    print("자동 release 완료!")

if __name__ == "__main__":
    main()
