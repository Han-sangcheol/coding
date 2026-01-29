"""Python Assertion - assert"""

def divide(a: int, b: int) -> int:
    assert b != 0, "Divisor cannot be zero"
    return a // b

def main():
    print("=== Python Assertion ===")
    
    print(f"10 / 2 = {divide(10, 2)}")
    # divide(10, 0)  # AssertionError (주석 처리)

if __name__ == "__main__":
    main()
