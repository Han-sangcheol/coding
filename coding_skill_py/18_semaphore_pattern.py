"""Python Semaphore - threading.Semaphore"""
from threading import Semaphore

def main():
    print("=== Python Semaphore ===")
    sem = Semaphore(2)
    
    sem.acquire()
    print(f"Acquired, value: {sem._value}")
    
    sem.release()
    print(f"Released, value: {sem._value}")

if __name__ == "__main__":
    main()
