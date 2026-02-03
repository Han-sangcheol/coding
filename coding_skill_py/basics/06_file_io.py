"""
============================================================================
파일명: 06_file_io.py
주제: Python 파일 입출력
실행: python 06_file_io.py
============================================================================
"""

def example_01_basic_file_io():
    """예제 1: 기본 파일 입출력"""
    print("\n=== 예제 1: 기본 파일 입출력 ===\n")
    
    # C 방식
    """
    FILE* fp = fopen("test.txt", "w");
    fprintf(fp, "Hello\\n");
    fclose(fp);  # 수동 close 필수!
    """
    
    # Python 방식 1: 수동 close
    file = open("test.txt", "w")
    file.write("Hello, Python!\n")
    file.close()  # 수동
    print("파일 작성 완료 (수동 close)")
    
    # Python 방식 2: with (권장)
    with open("test.txt", "w") as file:
        file.write("Hello, Python!\n")
        file.write("자동 close!\n")
    # 자동으로 close됨!
    print("파일 작성 완료 (자동 close)")

def example_02_read_file():
    """예제 2: 파일 읽기"""
    print("\n=== 예제 2: 파일 읽기 ===\n")
    
    # 파일 작성
    with open("test.txt", "w") as f:
        f.write("첫 번째 줄\n")
        f.write("두 번째 줄\n")
        f.write("세 번째 줄\n")
    
    # 전체 읽기
    with open("test.txt", "r") as f:
        content = f.read()
        print("전체 내용:")
        print(content)
    
    # 한 줄씩 읽기
    with open("test.txt", "r") as f:
        print("\n한 줄씩:")
        for line in f:
            print(f"  {line.strip()}")

def example_03_context_manager():
    """예제 3: with문 (컨텍스트 매니저)"""
    print("\n=== 예제 3: with문 ===\n")
    
    print("""
    with문의 장점:
    - 자동 리소스 관리
    - 예외 발생해도 close 보장
    - RAII와 유사
    
    with open("file.txt") as f:
        # 파일 사용
        pass
    # 자동 close
    """)

def example_04_c_vs_python():
    """예제 4: C vs Python 비교"""
    print("\n=== 예제 4: C vs Python 비교 ===\n")
    
    print("[C 방식]")
    print("""
    FILE* fp = fopen("file.txt", "r");
    if (fp == NULL) { ... }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }
    fclose(fp);  # 수동!
    """)
    
    print("\n[Python 방식]")
    print("""
    with open("file.txt") as f:
        for line in f:
            print(line)
    # 자동 close!
    """)

def main():
    print("=" * 40)
    print("Python 기초: 06. 파일 I/O")
    print("=" * 40)
    
    example_01_basic_file_io()
    example_02_read_file()
    example_03_context_manager()
    example_04_c_vs_python()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 07_decorators.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

open: open("file.txt", "r")
with: with open(...) as f:
읽기: f.read(), f.readline(), for line in f
쓰기: f.write(text)
자동 close: with문

============================================================================
"""
