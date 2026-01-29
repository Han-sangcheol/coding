"""Python RAII - with 문"""

class FileResource:
    def __init__(self, filename: str):
        self.filename = filename
        self.file = None
    
    def __enter__(self):
        self.file = open(self.filename, 'w')
        print("[RAII] 파일 열림")
        return self.file
    
    def __exit__(self, *args):
        if self.file:
            self.file.close()
        print("[RAII] 파일 자동 닫힘")

def main():
    print("=== Python RAII (with) ===")
    with FileResource("test.txt") as f:
        f.write("Hello")
    print("블록 종료!")

if __name__ == "__main__":
    main()
