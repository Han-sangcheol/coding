"""Python Retry - 데코레이터"""
from functools import wraps
from typing import Callable

def retry(max_attempts: int):
    def decorator(func: Callable) -> Callable:
        @wraps(func)
        def wrapper(*args, **kwargs):
            for attempt in range(1, max_attempts + 1):
                try:
                    return func(*args, **kwargs)
                except Exception as e:
                    print(f"[Retry] Attempt {attempt} failed: {e}")
                    if attempt == max_attempts:
                        raise
        return wrapper
    return decorator

@retry(max_attempts=3)
def unstable_operation():
    import random
    if random.random() > 0.5:
        raise Exception("Random failure")
    return "Success!"

def main():
    print("=== Python Retry ===")
    try:
        result = unstable_operation()
        print(f"Result: {result}")
    except:
        print("Failed after retries")

if __name__ == "__main__":
    main()
