# Python 디자인 패턴 학습 자료

## 개요

이 폴더는 C 버전 디자인 패턴을 **Python 3.9+**의 강력한 기능으로 재구현한 자료입니다.

**총 32개 패턴** (00-31번)

## C vs Python 주요 차이점

| 개념 | C 버전 | Python 버전 |
|------|--------|-------------|
| 함수 포인터 | `void (*fp)(int)` | 일급 함수, 람다 |
| 구조체 | `struct + 함수 포인터` | 클래스 + 메서드 |
| 메모리 관리 | 수동 `malloc/free` | 가비지 컬렉터 |
| 콜백 | 함수 포인터 | 데코레이터, 람다 |
| 타입 | 정적, 명시적 | 동적, 타입 힌팅 |
| 리소스 관리 | 수동 | `with` 문 |

## 필요 환경

- **Python**: 3.9 이상 (타입 힌팅 지원)
- **선택사항**:
  - mypy (타입 체크)
  - pytest (테스트)

## 실행 방법

### 전체 실행

```bash
python run_all.py
```

### 개별 실행

```bash
python 00_function_pointer_basics.py
python 01_wrapper_pattern.py
python 04_callback_pattern.py
```

### 타입 체크 (선택사항)

```bash
pip install mypy
mypy *.py
```

## Python에서 추가된 특화 기능

### 00. Function Pointer → 일급 함수
- 함수를 변수처럼 사용
- 클로저
- 데코레이터

### 01. Wrapper → 데코레이터
- `@decorator` 문법
- `functools.wraps`
- 컨텍스트 매니저 (`with`)

### 03. Singleton → 메타클래스
- `__new__` 오버라이드
- 클래스 데코레이터
- 모듈 레벨 싱글톤 (가장 Pythonic)

### 04. Callback → 일급 함수
- 람다와 partial
- 클로저로 상태 보유

### 05. State Machine → Enum + 딕셔너리
- `match` 문 (Python 3.10+)
- Enum 활용

### 06. Factory → classmethod
- `@classmethod`
- 딕셔너리 레지스트리

### 07. Observer → weakref
- 약한 참조로 메모리 누수 방지
- property로 자동 통지

### 11. Memory Pool → 불필요
- 가비지 컬렉터가 자동 관리
- 개념 학습용으로만 구현

### 13. Reference Counting → 내장
- Python은 기본적으로 참조 카운팅
- `sys.getrefcount()`로 확인

### 14. Ring Buffer → collections.deque
- 최적화된 양방향 큐
- `maxlen`으로 자동 크기 제한

### 17. RAII → 컨텍스트 매니저
- `with` 문
- `__enter__`, `__exit__`
- `contextlib.contextmanager`

### 20. Lazy Init → cached_property
- `@functools.cached_property`
- `@property` + 캐싱

### 21. Cache → lru_cache
- `@functools.lru_cache`
- 자동 캐시 관리

### 22. Zero-Copy → memoryview
- 복사 없는 버퍼 접근
- NumPy 배열

### 24. Retry → 데코레이터
- 간단한 재시도 로직
- tenacity 라이브러리 (고급)

### 29. Mock Object → unittest.mock
- 강력한 Mock/MagicMock
- patch 데코레이터

### 31. Tracing → 데코레이터
- 함수 호출 자동 추적
- 성능 프로파일링

## 학습 로드맵

### 🔗 Python 스킬 상관관계 다이어그램

```
                [일급 함수 & 람다 (00)]
                        │
          ┌─────────────┼─────────────┐
          │             │             │
    [데코레이터 (01)]  [클로저 (04)]  [클래스 (03)]
          │             │             │
          ↓             ↓             ↓
    [컨텍스트 매니저]  [property]  [메타클래스]
          │             │             │
          └─────────────┼─────────────┘
                        ↓
              [고급 패턴 조합]
                        │
          ┌─────────────┼─────────────┐
          │             │             │
    [asyncio]      [제너레이터]   [typing]
```

### 📈 Python 학습 추천 순서

#### 레벨 1: Python 기초 기능 (1-2주)
```
00 → 01 → 03 → 04
함수   데코   클래스  콜백
포인터 레이터        (람다)
```
**학습 내용:**
- 일급 함수 (함수를 변수처럼 사용)
- 람다 표현식
- 데코레이터 기본 (@decorator)
- 클래스와 메서드

#### 레벨 2: 중급 기능 (2-3주)
```
06 → 07 → 05 → 10
Factory  Observer  State    Event
(classmethod)  (weakref)  Machine  Queue
```
**학습 내용:**
- @classmethod, @staticmethod
- property와 descriptor
- weakref (약한 참조)
- collections.deque

#### 레벨 3: 고급 기능 (2-3주)
```
17 → 20 → 21 → 22
컨텍스트  Lazy    Cache   Zero-Copy
매니저   (cached  (lru)   (memoryview)
(with)   property)
```
**학습 내용:**
- __enter__, __exit__
- functools (wraps, lru_cache, cached_property)
- contextlib.contextmanager
- memoryview

#### 레벨 4: 전문가 (3-4주)
```
29 → 30 → 31 → asyncio
Mock  Assertion  Tracing  비동기
(unittest.mock)          프로그래밍
```
**학습 내용:**
- unittest.mock (Mock, MagicMock, patch)
- typing 모듈 (고급 타입 힌팅)
- asyncio (async/await)
- 성능 프로파일링

### 🎯 학습 경로별 추천

#### 경로 A: 웹 개발자 (Django/Flask/FastAPI)
```
00 → 01 → 04 → 17 → 20 → 21 → asyncio
함수  데코레이터  콜백  with  Lazy  Cache  비동기
```
**중점 패턴:**
- 데코레이터 (라우팅, 권한)
- 컨텍스트 매니저 (DB 연결)
- Cache (성능 최적화)
- asyncio (비동기 처리)

#### 경로 B: 데이터 과학/ML 엔지니어
```
00 → 03 → 06 → 20 → 21 → 22
함수  클래스  Factory  Lazy  Cache  Zero-Copy
```
**중점 패턴:**
- Factory (모델 생성)
- Cache (계산 결과 저장)
- Zero-Copy (대용량 데이터)
- property (데이터 검증)

#### 경로 C: 자동화/스크립팅 개발자
```
00 → 01 → 04 → 24 → 29 → 30
함수  데코레이터  콜백  Retry  Mock  Assertion
```
**중점 패턴:**
- 데코레이터 (로깅, 재시도)
- Retry (네트워크 오류 처리)
- Mock (테스트 자동화)

#### 경로 D: 시스템/백엔드 개발자
```
00 → 03 → 05 → 10 → 17 → 18 → asyncio
함수  클래스  State  Queue  with  동기화  비동기
```
**중점 패턴:**
- State Machine (시스템 상태 관리)
- Event Queue (이벤트 처리)
- 컨텍스트 매니저 (리소스 관리)
- asyncio (동시성)

### 🔄 C 패턴과 Python 패턴 매핑

| C 패턴 | Python 구현 | 난이도 변화 | 핵심 차이 |
|--------|------------|-----------|----------|
| **함수 포인터** | 일급 함수, 람다 | ⬇️ 쉬워짐 | 자연스러운 문법 |
| **Wrapper** | 데코레이터 | ⬇️ 쉬워짐 | @decorator 문법 |
| **Singleton** | 메타클래스/모듈 | ⬆️ 복잡해짐 | 여러 방법 존재 |
| **Callback** | 람다, 일급 함수 | ⬇️ 쉬워짐 | 간결한 표현 |
| **State Machine** | Enum + dict | ⬇️ 쉬워짐 | match 문 활용 |
| **Factory** | classmethod | ⬇️ 쉬워짐 | @classmethod |
| **Observer** | weakref + property | ➡️ 비슷 | 메모리 관리 필요 |
| **Memory Pool** | 불필요 | ⬇️ 필요없음 | GC가 자동 처리 |
| **Ring Buffer** | collections.deque | ⬇️ 쉬워짐 | 내장 자료구조 |
| **RAII** | with 문 | ⬇️ 쉬워짐 | 자동 정리 |
| **Cache** | lru_cache | ⬇️ 쉬워짐 | 내장 데코레이터 |
| **Mock** | unittest.mock | ⬇️ 쉬워짐 | 강력한 라이브러리 |

### 📊 패턴별 의존 관계 (Python)

| 패턴 | 선행 학습 필요 | 연관 Python 기능 |
|------|--------------|-----------------|
| **00: 함수 포인터** | Python 기초 | 람다, 클로저, 데코레이터 |
| **01: Wrapper** | 일급 함수 (00) | functools.wraps, 데코레이터 |
| **03: Singleton** | 클래스, __new__ | 메타클래스, 모듈 |
| **04: Callback** | 일급 함수 (00) | 람다, functools.partial |
| **05: State Machine** | Enum, 딕셔너리 | match 문 (3.10+), typing |
| **06: Factory** | 클래스, classmethod | @classmethod, 레지스트리 패턴 |
| **07: Observer** | 클래스, weakref | property, __setattr__ |
| **10: Event Queue** | collections.deque | queue.Queue, asyncio.Queue |
| **17: RAII** | 컨텍스트 매니저 | __enter__, __exit__, contextlib |
| **20: Lazy Init** | property | @functools.cached_property |
| **21: Cache** | 데코레이터 | @functools.lru_cache |
| **29: Mock** | unittest | unittest.mock, pytest-mock |

### 1단계: Python 기초 (00-10)
- 일급 함수
- 람다와 클로저
- 데코레이터
- 컨텍스트 매니저

### 2단계: 고급 기능 (11-22)
- property와 descriptor
- 제너레이터
- 메타클래스
- typing 모듈

### 3단계: 실무 적용 (23-31)
- 비동기 프로그래밍 (asyncio)
- 멀티스레딩
- 성능 최적화

## C 버전과 비교

| 항목 | C | Python |
|------|---|--------|
| 코드 복잡도 | 중간 | 낮음 |
| 타입 안전성 | 중간 | 낮음 (타입 힌팅으로 개선) |
| 메모리 안전 | 낮음 | 높음 (GC) |
| 표현력 | 낮음 | 매우 높음 |
| 성능 | 최고 | 낮음 (하지만 대부분 충분) |
| 학습 곡선 | 중간 | 완만 |
| 개발 속도 | 느림 | 매우 빠름 |

## 실무 적용

- **웹 개발**: Python (Django, Flask, FastAPI)
- **데이터 분석**: Python (Pandas, NumPy)
- **AI/ML**: Python (TensorFlow, PyTorch)
- **스크립팅**: Python
- **프로토타이핑**: Python
- **임베디드**: C (Python은 부적합)

## Python 특화 추가 학습

이 자료를 마스터한 후:

1. **asyncio** - 비동기 프로그래밍
2. **typing** - 고급 타입 힌팅
3. **dataclasses** - 데이터 클래스
4. **itertools** - 이터레이터 도구
5. **concurrent.futures** - 병렬 처리

## 참고 자료

- C 버전: `../coding_skill/`
- C++ 버전: `../coding_skill_cpp/`
- "Fluent Python" - Luciano Ramalho
- "Python Design Patterns" - Brandon Rhodes
- docs.python.org

**마지막 업데이트**: 2026-01-30
