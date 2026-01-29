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
