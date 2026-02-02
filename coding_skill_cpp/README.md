# C++ 디자인 패턴 학습 자료

## 개요

이 폴더는 C 버전 디자인 패턴을 **C++17/20**의 현대적 기능으로 재구현한 자료입니다.

**총 32개 패턴** (00-31번)

## C vs C++ 주요 차이점

| 개념 | C 버전 | C++ 버전 |
|------|--------|----------|
| 함수 포인터 | `void (*fp)(int)` | `std::function<void(int)>` + 람다 |
| 구조체 | `struct + 함수 포인터` | 클래스 + 가상 함수 |
| 메모리 관리 | 수동 `malloc/free` | RAII + 스마트 포인터 |
| 콜백 | 함수 포인터 | `std::function`, 람다 |
| 타입 추상화 | `void*` | 템플릿 |
| 리소스 관리 | 수동 | RAII (생성자/소멸자) |

## 필요 환경

- **컴파일러**: 
  - g++ 9.0 이상
  - clang++ 10.0 이상
  - MSVC 2019 이상
- **C++ 표준**: C++17 (일부 C++20 사용)
- **CMake**: 3.15 이상 (선택사항)

## 빌드 방법

### Windows (MinGW)

```bash
# 자동 빌드
compile_all.bat

# 또는 CMake 직접 사용
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

### Linux/Mac

```bash
# Makefile 사용
make

# 개별 컴파일
g++ -std=c++17 -o 00_function_pointer 00_function_pointer_basics.cpp

# 전체 실행
make run
```

## C++에서 추가된 패턴 특화 기능

### 00. Function Pointer → std::function + 람다
- 타입 안전한 함수 객체
- 람다 캡처로 상태 보유
- 템플릿과 조합

### 01. Wrapper → RAII + 스마트 포인터
- 자동 리소스 관리
- `unique_ptr`, `shared_ptr`
- 커스텀 deleter

### 03. Singleton → Meyers Singleton
- 지역 static으로 thread-safe 보장 (C++11+)
- CRTP를 이용한 Singleton Base

### 04. Callback → std::function + 람다
- 상태를 가진 콜백
- 멤버 함수 바인딩

### 05. State Machine → enum class + std::map
- 타입 안전한 enum
- `std::variant`로 상태 데이터 관리

### 06. Factory → 템플릿 Factory
- 컴파일 타임 타입 체크
- 레지스트리 패턴

### 07. Observer → std::vector<std::function>
- 템플릿으로 제네릭 구현
- weak_ptr로 메모리 누수 방지

### 11. Memory Pool → 커스텀 allocator
- STL 컨테이너와 통합
- `std::pmr` (C++17)

### 13. Reference Counting → shared_ptr
- 자동 참조 카운팅
- `weak_ptr`로 순환 참조 방지

### 17. RAII → C++의 핵심!
- 소멸자로 자동 정리
- 예외 안전성 보장

### 20. Lazy Init → std::optional
- `std::once_flag`로 thread-safe

### 21. Cache → std::unordered_map
- LRU 캐시 구현

### 22. Zero-Copy → 이동 시맨틱
- `std::move`
- RVO (Return Value Optimization)

## 학습 로드맵

### 🔗 C++ 스킬 상관관계 다이어그램

```
            [함수 포인터 & std::function (00)]
                        │
          ┌─────────────┼─────────────┐
          │             │             │
    [람다 & 클로저]  [템플릿]    [RAII]
          │             │             │
          ↓             ↓             ↓
    [std::function]  [제네릭]   [스마트 포인터]
          │             │             │
          └─────────────┼─────────────┘
                        ↓
              [현대적 C++ 패턴]
                        │
          ┌─────────────┼─────────────┐
          │             │             │
    [이동 시맨틱]   [std::variant]  [멀티스레딩]
```

### 📈 C++ 학습 추천 순서 (C++17/20 기준)

#### 레벨 1: C++ 기초 (1-2주)
```
00 → 01 → 03 → 04
함수   RAII    Singleton  Callback
포인터 스마트   (Meyers)   (lambda)
      포인터
```
**학습 내용:**
- std::function과 람다 표현식
- 스마트 포인터 (unique_ptr, shared_ptr, weak_ptr)
- RAII 원칙
- 이동 시맨틱 기초

#### 레벨 2: 중급 C++ (2-3주)
```
06 → 07 → 05 → 22
Factory  Observer  State    Zero-Copy
(템플릿)  (weak_ptr)  Machine  (std::move)
```
**학습 내용:**
- 템플릿 클래스와 함수
- std::variant, std::optional
- 이동 시맨틱 (std::move, std::forward)
- 완벽한 전달 (perfect forwarding)

#### 레벨 3: 고급 C++ (2-3주)
```
11 → 13 → 17 → 20
Memory   RefCount  RAII    Lazy
Pool     (shared_  (커스텀) (optional)
(allocator) ptr)
```
**학습 내용:**
- 커스텀 allocator
- std::pmr (C++17 메모리 리소스)
- 예외 안전성 (Exception Safety)
- constexpr, consteval (C++20)

#### 레벨 4: 시스템 프로그래밍 (3-4주)
```
18 → 19 → 28 → 30
Semaphore  Mutex   Interrupt  Assertion
(std::     (lock_  Handler    (static_
counting_  guard)             assert)
semaphore)
```
**학습 내용:**
- std::mutex, std::lock_guard
- std::condition_variable
- std::atomic
- 메모리 모델과 순서

### 🎯 학습 경로별 추천 (C++)

#### 경로 A: 임베디드 C++ 개발자
```
00 → 01 → 05 → 17 → 26 → 28 → 22
함수  RAII  State   자동   HAL   ISR   최적화
            Machine 리소스
```
**중점 기술:**
- RAII와 스마트 포인터 (필수)
- 이동 시맨틱 (성능)
- constexpr (컴파일 타임 계산)
- 템플릿 메타프로그래밍

#### 경로 B: 게임 엔진 개발자
```
00 → 01 → 06 → 11 → 12 → 16 → 22
함수  RAII  Factory  Pool   Object  Double  Zero-
                                    Buffer  Copy
```
**중점 기술:**
- 커스텀 allocator
- 이동 시맨틱
- 캐시 친화적 설계
- SIMD 최적화

#### 경로 C: 시스템 프로그래머
```
00 → 01 → 17 → 18 → 19 → 28 → 30
함수  RAII  자동   동기화  Mutex  ISR   Assert
            관리
```
**중점 기술:**
- 멀티스레딩 (std::thread)
- 원자적 연산 (std::atomic)
- 메모리 순서 (memory_order)
- 예외 안전성

#### 경로 D: 서버/백엔드 개발자
```
00 → 04 → 07 → 10 → 13 → 18 → 21
함수  Callback  Observer  Queue  RefCount  동기화  Cache
```
**중점 기술:**
- 비동기 I/O
- weak_ptr (순환 참조 방지)
- std::async, std::future
- 동시성 패턴

### 🔄 C 패턴과 C++ 패턴 매핑

| C 패턴 | C++ 구현 | 난이도 변화 | 핵심 차이 |
|--------|---------|-----------|----------|
| **함수 포인터** | `std::function` + 람다 | ⬇️ 쉬워짐 | 타입 안전, 상태 캡처 |
| **Wrapper** | RAII + 스마트 포인터 | ➡️ 비슷 | 자동 정리 |
| **Singleton** | Meyers Singleton | ⬇️ 쉬워짐 | thread-safe 자동 |
| **Callback** | `std::function` + 람다 | ⬇️ 쉬워짐 | 멤버 함수 바인딩 |
| **State Machine** | `enum class` + `std::variant` | ⬆️ 복잡해짐 | 타입 안전 증가 |
| **Factory** | 템플릿 Factory | ⬆️ 복잡해짐 | 컴파일 타임 체크 |
| **Observer** | `std::function` + `weak_ptr` | ➡️ 비슷 | 메모리 안전 |
| **Memory Pool** | 커스텀 allocator | ⬆️ 복잡해짐 | STL 통합 |
| **RefCount** | `shared_ptr` | ⬇️ 쉬워짐 | 자동 관리 |
| **Ring Buffer** | 템플릿 + iterator | ⬆️ 복잡해짐 | STL 스타일 |
| **RAII** | 생성자/소멸자 | ⬇️ 쉬워짐 | C++의 핵심 |
| **Zero-Copy** | 이동 시맨틱 | ⬇️ 쉬워짐 | `std::move` |

### 📊 패턴별 의존 관계 (C++)

| 패턴 | 선행 학습 필요 | 연관 C++ 기능 |
|------|--------------|--------------|
| **00: 함수 포인터** | C++ 기초 | `std::function`, 람다, 클로저 |
| **01: Wrapper** | RAII 개념 | 스마트 포인터, 커스텀 deleter |
| **03: Singleton** | 클래스, static | thread-safe static, CRTP |
| **04: Callback** | std::function (00) | 람다 캡처, `std::bind` |
| **05: State Machine** | `enum class` | `std::variant`, `std::visit` |
| **06: Factory** | 템플릿 기초 | 가변 템플릿, perfect forwarding |
| **07: Observer** | std::function (00) | `weak_ptr`, 템플릿 |
| **11: Memory Pool** | allocator 개념 | `std::pmr`, 커스텀 allocator |
| **13: RefCount** | 스마트 포인터 | `shared_ptr`, `weak_ptr`, `enable_shared_from_this` |
| **17: RAII** | 생성자/소멸자 | Rule of 5, 이동 시맨틱 |
| **20: Lazy Init** | `std::optional` | `std::once_flag`, `call_once` |
| **22: Zero-Copy** | 이동 시맨틱 | `std::move`, `std::forward`, RVO |
| **18: Semaphore** | 동기화 기초 | `std::mutex`, `std::condition_variable` |
| **28: ISR** | 원자적 연산 | `std::atomic`, memory_order |

### 🎓 C++ 고급 주제 로드맵

#### Phase 1: Modern C++ 기초
```
람다 & 클로저 → std::function → 이동 시맨틱
     ↓              ↓               ↓
RAII 원칙 → 스마트 포인터 → Rule of 5
```

#### Phase 2: 템플릿 & 제네릭
```
함수 템플릿 → 클래스 템플릿 → 가변 템플릿
     ↓              ↓               ↓
SFINAE → if constexpr → Concepts (C++20)
```

#### Phase 3: 동시성 & 병렬성
```
std::thread → std::mutex → std::atomic
     ↓              ↓               ↓
std::async → 조건 변수 → 메모리 모델
```

#### Phase 4: 최적화 & 성능
```
이동 시맨틱 → RVO/NRVO → constexpr
     ↓              ↓               ↓
Zero-Copy → 캐시 최적화 → SIMD
```

### 💡 C vs C++ 선택 가이드

#### C를 선택하는 경우:
- ✅ 극도로 제한된 리소스 (8비트 MCU)
- ✅ 예측 가능한 성능 필요
- ✅ 기존 C 코드베이스 유지
- ✅ 단순한 하드웨어 제어

#### C++를 선택하는 경우:
- ✅ 32비트 이상 MCU
- ✅ 복잡한 상태 관리 필요
- ✅ 타입 안전성 중요
- ✅ 자동 리소스 관리 필요
- ✅ 코드 재사용성 중요

#### C와 C++ 혼용:
- ✅ HAL은 C, 응용은 C++
- ✅ ISR은 C, 비즈니스 로직은 C++
- ✅ 드라이버는 C, 미들웨어는 C++

### 1단계: 기본 (00-10)
C++의 핵심 기능 이해
- std::function과 람다
- RAII와 스마트 포인터
- 클래스와 상속

### 2단계: 중급 (11-22)
고급 C++ 기능
- 템플릿
- 이동 시맨틱
- std::optional, std::variant

### 3단계: 고급 (23-31)
시스템 프로그래밍
- 원자적 연산
- 멀티스레딩
- 성능 최적화

## C 버전과 비교

| 항목 | C | C++ |
|------|---|-----|
| 코드 복잡도 | 낮음 | 중간 |
| 타입 안전성 | 낮음 | 높음 |
| 메모리 안전 | 수동 | 자동 (RAII) |
| 표현력 | 낮음 | 높음 |
| 성능 | 최고 | 거의 동일 (Zero-overhead) |
| 학습 곡선 | 완만 | 가파름 |

## 실무 적용

- **임베디드**: C++ 사용 가능 (AVR 제외)
- **모바일**: C++ 필수 (Android NDK, iOS)
- **게임**: C++ 표준
- **시스템**: C/C++ 혼용

## 참고 자료

- C 버전: `../coding_skill/`
- "Effective C++" - Scott Meyers
- "Modern C++ Design" - Andrei Alexandrescu
- cppreference.com

**마지막 업데이트**: 2026-01-30
