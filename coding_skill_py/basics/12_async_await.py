"""
============================================================================
파일명: 12_async_await.py
주제: Python 비동기 프로그래밍 (async/await)
실행: python 12_async_await.py
============================================================================
"""

import asyncio

async def example_01_basic_async():
    """예제 1: 기본 async/await"""
    print("\n=== 예제 1: 기본 async/await ===\n")
    
    async def say_hello():
        print("Hello")
        await asyncio.sleep(1)  # 비동기 대기
        print("World")
    
    await say_hello()

async def example_02_multiple_tasks():
    """예제 2: 여러 작업 동시 실행"""
    print("\n=== 예제 2: 여러 작업 동시 실행 ===\n")
    
    async def task(name, delay):
        print(f"  {name} 시작")
        await asyncio.sleep(delay)
        print(f"  {name} 완료 ({delay}초)")
        return f"{name} 결과"
    
    # 동시 실행
    results = await asyncio.gather(
        task("작업1", 2),
        task("작업2", 1),
        task("작업3", 3)
    )
    
    print(f"결과: {results}")

async def example_03_async_for():
    """예제 3: async for"""
    print("\n=== 예제 3: async for ===\n")
    
    async def async_range(n):
        for i in range(n):
            await asyncio.sleep(0.1)
            yield i
    
    async for i in async_range(5):
        print(f"  값: {i}")

def example_04_c_vs_python():
    """예제 4: C vs Python 비교"""
    print("\n=== 예제 4: C vs Python 비교 ===\n")
    
    print("[C 방식 (콜백)]")
    print("""
    void on_complete(void* data) {
        printf("완료\\n");
    }
    
    start_async_task(on_complete);
    """)
    
    print("\n[Python async/await]")
    print("""
    async def my_task():
        result = await async_operation()
        print(f"완료: {result}")
    
    asyncio.run(my_task())
    """)

async def main_async():
    """비동기 메인 함수"""
    print("=" * 40)
    print("Python 기초: 12. Async/Await")
    print("=" * 40)
    
    await example_01_basic_async()
    await example_02_multiple_tasks()
    await example_03_async_for()
    example_04_c_vs_python()
    
    print("\n" + "=" * 40)
    print("🎉 모든 Python 기초 학습 완료!")
    print("다음: 상위 디렉토리의 패턴 학습")
    print("=" * 40)

def main():
    """동기 메인 함수"""
    asyncio.run(main_async())

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

async def: 비동기 함수
await: 비동기 대기
asyncio.run(): 비동기 함수 실행
asyncio.gather(): 여러 작업 동시 실행

장점:
- I/O 대기 시간 효율적
- 동시성
- 콜백 지옥 방지

용도:
- 웹 서버 (FastAPI)
- 네트워크 요청
- 파일 I/O
- DB 쿼리

C의 콜백 패턴 → Python의 async/await

============================================================================
"""
