# Python 기초 학습 가이드 (C 개발자용)

## 개요

이 디렉토리는 **C 언어를 이미 알고 있는 개발자**를 위한 Python 기초 학습 자료입니다.

C와 Python의 철학적 차이를 중심으로 설명하며, 각 챕터는 독립적으로 학습 가능하도록 구성되어 있습니다.

---

## 왜 Python을 배워야 하는가?

### C의 철학
> "프로그래머를 믿어라. 프로그래머가 원하는 것을 막지 마라."

- 완전한 제어권
- 최고의 성능
- 하드웨어 직접 접근

### Python의 철학
> "Simple is better than complex. Readability counts."

- 빠른 개발 속도
- 읽기 쉬운 코드
- 풍부한 라이브러리

### C vs Python 한눈에 비교

| 항목 | C | Python |
|------|---|--------|
| **타입 선언** | 필수 (int, char*) | 불필요 (동적 타입) |
| **메모리 관리** | 수동 (malloc/free) | 자동 (GC) |
| **컴파일** | 필요 | 불필요 (인터프리터) |
| **개발 속도** | 느림 | 매우 빠름 |
| **실행 속도** | 매우 빠름 (1x) | 느림 (50-100x) |
| **코드 길이** | 길다 | 매우 짧다 (1/5 ~ 1/10) |
| **용도** | 시스템, 임베디드 | 웹, 데이터, AI |

---

## C 개발자를 위한 학습 로드맵

```
[1단계] 기본 문법 (1-2일)
   ↓
[2단계] 자료구조 (2-3일)
   ↓
[3단계] 함수와 클래스 (3-5일)
   ↓
[4단계] Python다운 코드 (5-7일)
   ↓
[5단계] 실무 프로젝트
```

---

## 챕터별 학습 내용

### 1단계: 기본 문법 익히기

| 챕터 | 파일명 | 주요 내용 | 학습 시간 |
|------|--------|-----------|-----------|
| 01 | `01_basic_syntax.py` | 변수, print, 들여쓰기, 타입 힌트 | 1시간 |
| 02 | `02_lists_tuples_dicts.py` | list, tuple, dict, set | 2시간 |
| 03 | `03_functions.py` | def, 기본 매개변수, *args, 람다 | 1시간 |

**학습 목표**: C의 문법을 잊고 Python 문법에 익숙해지기

### 2단계: 객체 지향 프로그래밍

| 챕터 | 파일명 | 주요 내용 | 학습 시간 |
|------|--------|-----------|-----------|
| 04 | `04_classes_objects.py` | class, __init__, self, @property | 2시간 |
| 05 | `05_modules_packages.py` | import, __init__.py, 패키지 | 1시간 |
| 06 | `06_file_io.py` | open(), with, 자동 리소스 관리 | 1시간 |

**학습 목표**: C의 수동 관리를 Python의 자동 관리로 대체

### 3단계: Pythonic 코드 작성

| 챕터 | 파일명 | 주요 내용 | 학습 시간 |
|------|--------|-----------|-----------|
| 07 | `07_decorators.py` | @decorator, 함수 래퍼 | 2시간 |
| 08 | `08_generators.py` | yield, 지연 평가 | 2시간 |
| 09 | `09_context_managers.py` | with, __enter__/__exit__ | 1시간 |
| 10 | `10_comprehensions.py` | list/dict comprehension | 1시간 |

**학습 목표**: Python다운 간결한 코드 작성

### 4단계: 고급 기능

| 챕터 | 파일명 | 주요 내용 | 학습 시간 |
|------|--------|-----------|-----------|
| 11 | `11_advanced_oop.py` | 메타클래스, 디스크립터 | 3시간 |
| 12 | `12_async_await.py` | async/await, asyncio | 3시간 |

**학습 목표**: 고급 기능으로 복잡한 문제 해결

---

## 추천 학습 순서

### 빠른 학습 (핵심만)
```
01 → 02 → 03 → 04 → 06
```
**이유**: 기본 문법 → 자료구조 → 함수 → 클래스 → 파일 I/O

### 일반 학습 (권장)
```
01 → 02 → 03 → 04 → 05 → 06 → 07 → 10
```
**이유**: Pythonic 코드 작성까지

### 심화 학습 (전체)
```
01부터 12까지 모두
```
**이유**: Python의 모든 기능 마스터

---

## 실행 방법

### Python 설치 확인

```bash
# 버전 확인
python --version   # 또는 python3 --version

# 3.8 이상 권장
```

### 단일 파일 실행

```bash
# 실행
python 01_basic_syntax.py

# 또는 (Linux/Mac)
python3 01_basic_syntax.py
```

### 대화형 모드 (권장)

```bash
# Python REPL 시작
python

>>> # 코드를 한 줄씩 실행
>>> print("Hello, Python!")
Hello, Python!

>>> # Ctrl+D (Linux/Mac) 또는 Ctrl+Z (Windows)로 종료
```

### IPython (더 나은 REPL)

```bash
# 설치
pip install ipython

# 실행
ipython

# 자동 완성, 구문 강조, 히스토리 등 제공
```

---

## 각 챕터 학습 방법

### 1. 파일 열기
```bash
code 01_basic_syntax.py  # VS Code
vim 01_basic_syntax.py   # Vim
```

### 2. 상단 주석 읽기
- **이 챕터에서 배울 내용**: 학습 목표 확인
- **C와의 주요 차이점**: C와 비교하며 이해

### 3. 대화형으로 실험
```bash
python
>>> from 01_basic_syntax import *
>>> # 각 함수를 직접 호출해보기
```

### 4. 전체 실행
```bash
python 01_basic_syntax.py
```

### 5. 코드 수정해보기
- 예제 코드를 직접 수정하며 실험
- 에러를 보며 학습

---

## 실습 문제

각 챕터를 학습한 후 다음을 시도해보세요:

### 01. 기본 문법
- [ ] C의 `printf`를 `print()`로 변환
- [ ] 타입 선언 없이 변수 사용
- [ ] f-string으로 문자열 포맷팅

### 02. 자료구조
- [ ] C의 배열을 `list`로 변환
- [ ] C의 struct를 `dict`로 표현
- [ ] C의 해시맵을 `dict`로 변환

### 04. 클래스
- [ ] C의 struct + 함수를 `class`로 변환
- [ ] `__init__`으로 초기화
- [ ] `@property`로 getter/setter 대체

### 06. 파일 I/O
- [ ] C의 `fopen/fclose`를 `with open()`으로 변환
- [ ] 자동 리소스 관리 확인

### 10. 컴프리헨션
- [ ] C의 `for` 루프를 list comprehension으로 변환
- [ ] 코드 길이 비교

---

## 다음 단계

이 기초 학습을 마친 후:

1. **실무 프로젝트**
   - Flask/Django로 웹 개발
   - pandas로 데이터 분석
   - requests로 API 클라이언트

2. **라이브러리 학습**
   - NumPy (과학 계산)
   - Pandas (데이터 분석)
   - Matplotlib (시각화)

3. **패턴 학습**
   - 상위 디렉토리의 Python 패턴 파일들

---

## 학습 팁

### C 개발자가 흔히 하는 실수

1. **타입 명시 강박**
   ```python
   # ❌ C 습관
   def add(a: int, b: int) -> int:
       return a + b
   
   # ✅ Python 방식 (타입 힌트는 선택)
   def add(a, b):
       return a + b
   ```

2. **세미콜론 사용**
   ```python
   # ❌ C 습관
   x = 10;
   print(x);
   
   # ✅ Python 방식
   x = 10
   print(x)
   ```

3. **중괄호 사용**
   ```python
   # ❌ C 습관
   if x > 0 {
       print("positive")
   }
   
   # ✅ Python 방식 (들여쓰기)
   if x > 0:
       print("positive")
   ```

4. **수동 메모리 관리**
   ```python
   # ❌ C 습관 (불필요)
   data = allocate_memory(100)
   try:
       # ... 사용 ...
   finally:
       free_memory(data)
   
   # ✅ Python 방식 (자동)
   data = [0] * 100
   # ... 사용 ...
   # 자동으로 메모리 해제됨
   ```

5. **포인터 생각**
   ```python
   # ❌ C 습관
   def swap(a_ptr, b_ptr):
       temp = a_ptr[0]
       a_ptr[0] = b_ptr[0]
       b_ptr[0] = temp
   
   # ✅ Python 방식
   def swap(a, b):
       return b, a  # 튜플 반환
   
   x, y = swap(x, y)
   ```

### 효과적인 학습 전략

1. **C 코드를 Python으로 변환 연습**
   ```c
   // C
   for (int i = 0; i < 10; i++) {
       printf("%d\n", i);
   }
   ```
   
   ```python
   # Python
   for i in range(10):
       print(i)
   ```

2. **"C 방식"을 의도적으로 버리기**
   - 세미콜론, 중괄호, 타입 선언을 쓰고 싶어도 참기
   - Python다운 방식 찾기

3. **REPL을 적극 활용**
   ```python
   >>> help(list)  # 도움말 보기
   >>> dir(list)   # 사용 가능한 메서드 보기
   >>> list?       # IPython에서 자세한 정보
   ```

4. **PEP 8 스타일 가이드 따르기**
   - 들여쓰기: 공백 4개
   - 변수명: snake_case
   - 클래스명: PascalCase

---

## C vs Python 코드 비교

### 예제 1: Hello World

```c
// C (5줄)
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```

```python
# Python (1줄)
print("Hello, World!")
```

### 예제 2: 배열 합계

```c
// C (11줄)
#include <stdio.h>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += arr[i];
    }
    printf("Sum: %d\n", sum);
    return 0;
}
```

```python
# Python (2줄)
arr = [1, 2, 3, 4, 5]
print(f"Sum: {sum(arr)}")
```

### 예제 3: 파일 읽기

```c
// C (15줄)
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* fp = fopen("file.txt", "r");
    if (fp == NULL) {
        perror("Error");
        return 1;
    }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    return 0;
}
```

```python
# Python (2줄)
with open("file.txt") as f:
    print(f.read())
```

---

## Python의 철학 (The Zen of Python)

```python
>>> import this
```

```
Beautiful is better than ugly.
Explicit is better than implicit.
Simple is better than complex.
Complex is better than complicated.
Readability counts.
...
```

---

## 참고 자료

### 온라인 리소스
- [Python.org 공식 문서](https://docs.python.org/3/)
- [Real Python](https://realpython.com/) - 튜토리얼
- [Python Tutor](https://pythontutor.com/) - 코드 시각화

### 추천 서적
- **Fluent Python** (Luciano Ramalho) - Python다운 코드
- **Effective Python** (Brett Slatkin) - 90가지 모범 사례
- **Python Crash Course** (Eric Matthes) - 빠른 입문

### 유용한 도구
- **Black** - 자동 코드 포맷터
- **pylint** - 코드 품질 검사
- **mypy** - 타입 체크 (선택적)

---

## C 개발자를 위한 Python 체크리스트

### 문법
- [ ] 세미콜론 없이 코드 작성
- [ ] 중괄호 대신 들여쓰기 사용
- [ ] `camelCase` 대신 `snake_case` 사용

### 사고방식
- [ ] 타입 선언 하지 않기
- [ ] 메모리 관리 걱정하지 않기
- [ ] 포인터 잊어버리기

### Pythonic
- [ ] List comprehension 사용
- [ ] `with` 문으로 리소스 관리
- [ ] 제너레이터로 메모리 효율 높이기

### 실무
- [ ] `pip`로 패키지 관리
- [ ] 가상 환경 사용 (`venv`)
- [ ] 타입 힌트 선택적 사용

---

## 성능이 중요할 때

Python은 느립니다. 하지만:

### 해결 방법
1. **병목 지점만 C로 작성**
   - ctypes, cffi로 C 라이브러리 호출
   - Cython으로 C 확장 작성

2. **NumPy 사용**
   - 내부적으로 C로 구현됨
   - 배열 연산은 C 수준 성능

3. **PyPy 사용**
   - JIT 컴파일러
   - 일반적으로 CPython보다 5-10배 빠름

### 언제 Python, 언제 C?

| 상황 | 선택 |
|------|------|
| 프로토타입 | Python |
| 웹 백엔드 | Python |
| 데이터 분석 | Python |
| 시스템 프로그래밍 | C |
| 임베디드 | C |
| 실시간 처리 | C |
| 성능 핵심 부분 | C |
| 그 외 모든 것 | Python |

---

**작성일**: 2026-02-03  
**대상**: C 언어를 알고 있는 개발자  
**난이도**: 초급 → 중급  
**총 학습 시간**: 약 25시간
