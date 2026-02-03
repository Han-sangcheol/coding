"""
============================================================================
파일명: 02_lists_tuples_dicts.py
주제: Python 자료구조 (list, tuple, dict, set)
실행: python 02_lists_tuples_dicts.py
============================================================================
"""

def example_01_list():
    """예제 1: list (가변 배열)"""
    print("\n=== 예제 1: list ===\n")
    
    # C 배열 (고정 크기)
    # int arr[5] = {1, 2, 3, 4, 5};
    
    # Python 리스트 (동적 크기)
    numbers = [1, 2, 3, 4, 5]
    print(f"리스트: {numbers}")
    
    # 추가/삭제
    numbers.append(6)       # 끝에 추가
    numbers.insert(0, 0)    # 인덱스 0에 추가
    numbers.remove(3)       # 값 3 제거
    print(f"수정 후: {numbers}")
    
    # 인덱싱/슬라이싱
    print(f"첫 번째: {numbers[0]}")
    print(f"마지막: {numbers[-1]}")
    print(f"처음 3개: {numbers[:3]}")
    print(f"마지막 3개: {numbers[-3:]}")
    
    # 다양한 타입 혼합 가능
    mixed = [1, "hello", 3.14, True]
    print(f"혼합 리스트: {mixed}")

def example_02_tuple():
    """예제 2: tuple (불변 리스트)"""
    print("\n=== 예제 2: tuple ===\n")
    
    # tuple: 한 번 만들면 변경 불가
    point = (10, 20)
    print(f"점: {point}")
    print(f"x: {point[0]}, y: {point[1]}")
    
    # point[0] = 100  # 에러! 변경 불가
    
    # 언패킹
    x, y = point
    print(f"언패킹: x={x}, y={y}")
    
    # 함수 반환값으로 유용
    def get_info():
        return "홍길동", 30, "서울"
    
    name, age, city = get_info()
    print(f"이름: {name}, 나이: {age}, 도시: {city}")

def example_03_dict():
    """예제 3: dict (키-값 쌍)"""
    print("\n=== 예제 3: dict ===\n")
    
    # C의 수동 해시맵 대체
    person = {
        "name": "홍길동",
        "age": 30,
        "city": "서울"
    }
    
    print(f"이름: {person['name']}")
    print(f"나이: {person['age']}")
    
    # 추가/수정
    person["job"] = "개발자"
    person["age"] = 31
    print(f"수정 후: {person}")
    
    # 순회
    for key, value in person.items():
        print(f"  {key}: {value}")
    
    # get으로 안전하게 접근
    email = person.get("email", "없음")
    print(f"이메일: {email}")

def example_04_set():
    """예제 4: set (집합)"""
    print("\n=== 예제 4: set ===\n")
    
    # 중복 제거, 순서 없음
    numbers = {1, 2, 3, 3, 4, 4, 5}
    print(f"set: {numbers}")  # {1, 2, 3, 4, 5}
    
    # 집합 연산
    a = {1, 2, 3, 4}
    b = {3, 4, 5, 6}
    
    print(f"합집합: {a | b}")
    print(f"교집합: {a & b}")
    print(f"차집합: {a - b}")

def example_05_c_vs_python():
    """예제 5: C vs Python 비교"""
    print("\n=== 예제 5: C vs Python 비교 ===\n")
    
    print("[C 방식]")
    print("""
    // 배열 (고정 크기)
    int arr[5] = {1, 2, 3, 4, 5};
    
    // 해시맵 (수동 구현 필요)
    struct HashNode { ... };
    """)
    
    print("\n[Python 방식]")
    # 동적 배열
    arr = [1, 2, 3, 4, 5]
    arr.append(6)  # 간단!
    
    # 해시맵 (내장)
    hash_map = {"key1": "value1", "key2": "value2"}
    print(f"Python: 모두 내장 자료구조!")

def main():
    print("=" * 40)
    print("Python 기초: 02. 자료구조")
    print("=" * 40)
    
    example_01_list()
    example_02_tuple()
    example_03_dict()
    example_04_set()
    example_05_c_vs_python()
    
    print("\n" + "=" * 40)
    print("학습 완료!")
    print("다음: 03_functions.py")
    print("=" * 40)

if __name__ == "__main__":
    main()

"""
============================================================================
핵심 요약
============================================================================

list: [1, 2, 3]
- 가변
- 동적 크기
- append, insert, remove

tuple: (1, 2, 3)
- 불변
- 언패킹
- 함수 반환값

dict: {"key": "value"}
- 키-값 쌍
- 해시맵
- get, items

set: {1, 2, 3}
- 집합
- 중복 없음
- 집합 연산

============================================================================
"""
