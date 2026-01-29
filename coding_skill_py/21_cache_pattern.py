"""Python Cache - functools.lru_cache"""
from functools import lru_cache

@lru_cache(maxsize=128)
def expensive_function(n: int) -> int:
    print(f"[Cache] Computing {n}...")
    return n * n

def main():
    print("=== Python Cache (lru_cache) ===")
    
    print(expensive_function(5))
    print(expensive_function(5))  # 캐시됨
    print(expensive_function(10))
    
    print(f"\nCache info: {expensive_function.cache_info()}")

if __name__ == "__main__":
    main()
