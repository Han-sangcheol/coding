"""Python Tracing - 데코레이터"""
from functools import wraps
from typing import Callable

_trace_depth = 0

def trace(func: Callable) -> Callable:
    @wraps(func)
    def wrapper(*args, **kwargs):
        global _trace_depth
        print(" " * _trace_depth + f"→ {func.__name__}()")
        _trace_depth += 1
        try:
            result = func(*args, **kwargs)
            return result
        finally:
            _trace_depth -= 1
            print(" " * _trace_depth + f"← {func.__name__}()")
    return wrapper

@trace
def function_c():
    print("      FunctionC 실행")

@trace
def function_b():
    print("      FunctionB 실행")
    function_c()

@trace
def function_a():
    print("      FunctionA 실행")
    function_b()

def main():
    print("=== Python Tracing ===")
    function_a()

if __name__ == "__main__":
    main()
