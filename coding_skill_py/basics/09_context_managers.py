"""
============================================================================
파일명: 09_context_managers.py
주제: Python 컨텍스트 매니저
실행: python 09_context_managers.py
============================================================================
"""

def example_01_with_statement():
    """예제 1: with문"""
    print("\n=== 예제 1: with문 ===\n")
    
    # C 방식
    """
    FILE* fp = fopen("file.txt", "r");
    // 사용
    fclose(fp);  # 수동!
    """
    
    # Python with문
    with open("test.txt", "w") as f:
        f.write("자동 close!")
    # 자동으로 close됨
    
    print("파일 자동 닫힘!")

def example_02_custom_context_manager():
    """예제 2: 커스텀 컨텍스트 매니저"""
    print("\n=== 예제 2: 커스텀 컨텍스트 매니저 ===\n")
    
    class Timer:
        def __enter__(self):
            """with 블록 시작"""
            import time
            self.start = time.time()
            print("타이머 시작")
            return self
        
        def __exit__(self, exc_type, exc_val, exc_tb):
            """with 블록 끝"""
            import time
            self.end = time.time()
            print(f"타이머 종료: {self.end - self.start:.4f}초")
            return False
    
    with Timer():
        # 시간 측정할 코드
        total = sum(range(1000000))
        print(f"합계: {total}")

def example_03_contextlib():
    """예제 3: contextlib"""
    print("\n=== 예제 3: contextlib ===\n")
    
    from contextlib import contextmanager
    
    @contextmanager
    def my_context():
        print("시작")
        yield  # with 블록 실행
        print("종료")
    
    with my_context():
        print("작업 수행")

def example_04_c_vs_python():
    """예제 4: C vs Python 비교"""
    print("\n=== 예제 4: C vs Python 비교 ===\n")
    
    print("[C 방식 (RAII)]")
    print("""
    // C++의 RAII
    class Resource {
        Resource() { /* 획득 */ }
        ~Resource() { /* 해제 */ }
    };
    """)
    
    print("\n[Python 방식 (with)]")
    print("""
    with open("file.txt") as f:
        # 사용
        pass
    # 자동 해제
    """)

def main():
    print("=" * 40)
    print("Python 기초: 09. 컨텍스트 매니저")
    print("=" * 40)
    
    example_01_with_statement()
    example_02_custom_context_manager()
    example_03_contextlib()
    example_04_c_vs_python()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 10_comprehensions.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

with문: with ... as var:
__enter__: 시작 시 호출
__exit__: 종료 시 호출
용도: 자동 리소스 관리
예: 파일, 락, 연결

============================================================================
"""
