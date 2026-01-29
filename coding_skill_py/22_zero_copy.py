"""Python Zero-Copy - memoryview"""
import array

def main():
    print("=== Python Zero-Copy (memoryview) ===")
    
    data = array.array('i', range(10))
    print(f"Original: {list(data)}")
    
    # memoryview로 복사 없이 접근
    view = memoryview(data)
    print(f"View: {list(view)}")
    
    # 슬라이스도 복사 없음
    subview = view[2:5]
    print(f"Subview: {list(subview)}")

if __name__ == "__main__":
    main()
