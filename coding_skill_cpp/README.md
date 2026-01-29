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
