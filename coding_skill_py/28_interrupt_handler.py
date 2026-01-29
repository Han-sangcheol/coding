"""Python Interrupt Handler - signal"""
import signal
import sys

def signal_handler(signum, frame):
    print("[Signal] Received signal")
    sys.exit(0)

def main():
    print("=== Python Interrupt Handler ===")
    signal.signal(signal.SIGINT, signal_handler)
    print("Press Ctrl+C to test (주석 처리됨)")
    # signal.pause()  # 실제로는 이걸 사용

if __name__ == "__main__":
    main()
