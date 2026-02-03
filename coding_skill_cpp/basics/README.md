# C++ 기초 학습 가이드 (C 개발자용)

## 개요

이 디렉토리는 **C 언어를 이미 알고 있는 개발자**를 위한 C++ 기초 학습 자료입니다.

C와 C++의 차이점을 중심으로 설명하며, 각 챕터는 독립적으로 학습 가능하도록 구성되어 있습니다.

---

## 왜 C++를 배워야 하는가?

### C의 장점
- 빠른 실행 속도
- 작은 메모리 footprint
- 하드웨어 직접 제어
- 명확한 메모리 관리

### C++의 추가 장점
- **C의 모든 장점 유지** (Zero-overhead 원칙)
- **자동 메모리 관리** (RAII, 스마트 포인터)
- **타입 안전성** (템플릿, 타입 체크 강화)
- **추상화** (클래스, 상속, 다형성)
- **생산성** (STL, 람다, 현대적 문법)
- **대규모 프로젝트 관리** 용이

### C vs C++ 한눈에 비교

| 기능 | C | C++ |
|------|---|-----|
| 메모리 관리 | 수동 (malloc/free) | 자동 (RAII, 스마트 포인터) |
| 코드 재사용 | 함수, 매크로 | 클래스, 템플릿, 상속 |
| 다형성 | 함수 포인터 | 가상 함수, 템플릿 |
| 자료구조 | 수동 구현 | STL (vector, map 등) |
| 문자열 | char*, 수동 관리 | std::string (자동 관리) |
| 에러 처리 | 반환값, errno | 예외 (exception) |
| 타입 안전성 | 약함 (void*) | 강함 (템플릿) |

---

## C 개발자를 위한 학습 로드맵

```
[1단계] 기본 문법 (1-3일)
   ↓
[2단계] 클래스와 객체 (3-5일)
   ↓
[3단계] 템플릿과 STL (5-7일)
   ↓
[4단계] 현대적 C++ 기능 (7-10일)
   ↓
[5단계] 패턴 학습 (상위 디렉토리)
```

---

## 챕터별 학습 내용

### 1단계: 기본 문법 익히기

| 챕터 | 파일명 | 주요 내용 | 학습 시간 |
|------|--------|-----------|-----------|
| 01 | `01_basic_syntax.cpp` | iostream, namespace, auto, string | 1시간 |
| 02 | `02_references_vs_pointers.cpp` | 참조자(&), const 참조 | 1시간 |
| 03 | `03_classes_objects.cpp` | 클래스, 멤버 함수, 접근 제어 | 2시간 |

**학습 목표**: C++의 기본 문법과 C와의 차이점 이해

### 2단계: 객체 지향 프로그래밍

| 챕터 | 파일명 | 주요 내용 | 학습 시간 |
|------|--------|-----------|-----------|
| 04 | `04_constructors_destructors.cpp` | 생성자, 소멸자, RAII | 2시간 |
| 05 | `05_inheritance_polymorphism.cpp` | 상속, virtual, 다형성 | 3시간 |

**학습 목표**: C의 함수 포인터 패턴을 C++ 클래스로 대체

### 3단계: 템플릿과 STL

| 챕터 | 파일명 | 주요 내용 | 학습 시간 |
|------|--------|-----------|-----------|
| 06 | `06_templates_basics.cpp` | 함수/클래스 템플릿, 타입 안전성 | 2시간 |
| 07 | `07_stl_containers.cpp` | vector, map, set | 2시간 |
| 08 | `08_stl_algorithms.cpp` | sort, find, transform | 2시간 |

**학습 목표**: C의 수동 자료구조를 STL로 대체

### 4단계: 현대적 C++ 기능

| 챕터 | 파일명 | 주요 내용 | 학습 시간 |
|------|--------|-----------|-----------|
| 09 | `09_lambda_functions.cpp` | 람다, 캡처, std::function | 2시간 |
| 10 | `10_smart_pointers.cpp` | unique_ptr, shared_ptr | 2시간 |
| 11 | `11_move_semantics.cpp` | 이동 의미론, 성능 최적화 | 3시간 |
| 12 | `12_modern_features.cpp` | constexpr, nullptr, range-for | 2시간 |

**학습 목표**: 메모리 안전성과 성능 최적화

---

## 추천 학습 순서

### 빠른 학습 (핵심만)
```
01 → 03 → 04 → 07 → 10
```
**이유**: 기본 문법 → 클래스 → RAII → STL 컨테이너 → 스마트 포인터

### 일반 학습 (권장)
```
01 → 02 → 03 → 04 → 05 → 06 → 07 → 08 → 09 → 10
```
**이유**: 순차적으로 개념을 쌓아올림

### 심화 학습 (전체)
```
01부터 12까지 모두
```
**이유**: 모던 C++의 모든 기능 마스터

---

## 컴파일 방법

### Windows (MSYS2 UCRT64)

```bash
# 단일 파일 컴파일
g++ -std=c++17 01_basic_syntax.cpp -o 01_basic_syntax.exe

# 실행
./01_basic_syntax.exe
```

### Linux / Mac

```bash
# 단일 파일 컴파일
g++ -std=c++17 01_basic_syntax.cpp -o 01_basic_syntax

# 실행
./01_basic_syntax
```

### 권장 컴파일 옵션

```bash
g++ -std=c++17 -Wall -Wextra -O2 파일명.cpp -o 실행파일명
```

- `-std=c++17`: C++17 표준 사용
- `-Wall -Wextra`: 모든 경고 활성화
- `-O2`: 최적화 레벨 2

---

## 각 챕터 학습 방법

### 1. 파일 열기
```bash
code 01_basic_syntax.cpp  # VS Code
vim 01_basic_syntax.cpp   # Vim
```

### 2. 상단 주석 읽기
- **이 챕터에서 배울 내용**: 학습 목표 확인
- **C와의 주요 차이점**: C와 비교하며 이해

### 3. 예제 실행
```bash
g++ -std=c++17 01_basic_syntax.cpp -o test.exe
./test.exe
```

### 4. 코드 수정해보기
- 예제 코드를 직접 수정하며 실험
- 컴파일 에러를 보며 학습

### 5. 연습 문제 풀기
- 각 챕터 끝의 연습 문제 해결

---

## 실습 문제

각 챕터를 학습한 후 다음을 시도해보세요:

### 01. 기본 문법
- [ ] C의 `printf`를 `cout`으로 변환
- [ ] `auto` 키워드로 타입 추론 연습
- [ ] `std::string`으로 문자열 조작

### 03. 클래스
- [ ] C의 struct + 함수를 클래스로 변환
- [ ] private 멤버로 캡슐화 연습

### 04. 생성자/소멸자
- [ ] C의 `init/destroy` 함수를 생성자/소멸자로 변환
- [ ] RAII 패턴으로 리소스 관리

### 07. STL 컨테이너
- [ ] C의 배열을 `std::vector`로 변환
- [ ] C의 해시맵을 `std::map`으로 변환

### 10. 스마트 포인터
- [ ] `malloc/free`를 `unique_ptr`로 변환
- [ ] 메모리 누수 방지 연습

---

## 다음 단계

이 기초 학습을 마친 후:

1. **상위 디렉토리의 패턴 학습**
   - `00_function_pointer_basics.cpp`부터 시작
   - `01_wrapper_pattern.cpp`, `02_adapter_pattern.cpp` 등

2. **프로젝트 적용**
   - 기존 C 프로젝트를 C++로 리팩토링
   - STL과 스마트 포인터 활용

3. **심화 학습**
   - Effective C++ (Scott Meyers)
   - C++ Concurrency (멀티스레딩)
   - 템플릿 메타프로그래밍

---

## 학습 팁

### C 개발자가 흔히 하는 실수

1. **포인터 남용**
   ```cpp
   // ❌ C 습관
   void process(MyClass* obj) {
       if (obj != NULL) { ... }
   }
   
   // ✅ C++ 방식
   void process(const MyClass& obj) {
       // NULL 체크 불필요
   }
   ```

2. **수동 메모리 관리**
   ```cpp
   // ❌ C 습관
   MyClass* obj = (MyClass*)malloc(sizeof(MyClass));
   free(obj);
   
   // ✅ C++ 방식
   auto obj = std::make_unique<MyClass>();
   // 자동 해제됨
   ```

3. **매크로 과용**
   ```cpp
   // ❌ C 습관
   #define MAX(a, b) ((a) > (b) ? (a) : (b))
   
   // ✅ C++ 방식
   template<typename T>
   T max(T a, T b) { return a > b ? a : b; }
   ```

### 효과적인 학습 전략

1. **C 코드를 C++로 변환 연습**
   - 기존 C 프로젝트의 일부를 C++로 재작성
   - 변환 과정에서 C++의 장점 체험

2. **컴파일 에러를 두려워하지 말 것**
   - C++의 에러 메시지는 상세함
   - 에러 메시지를 읽고 이해하는 연습

3. **온라인 컴파일러 활용**
   - [Compiler Explorer](https://godbolt.org/)
   - [C++ Insights](https://cppinsights.io/)

---

## 참고 자료

### 온라인 리소스
- [cppreference.com](https://en.cppreference.com/) - C++ 레퍼런스
- [learncpp.com](https://www.learncpp.com/) - 온라인 튜토리얼
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) - 모범 사례

### 추천 서적
- **A Tour of C++** (Bjarne Stroustrup) - C++ 창시자의 간결한 가이드
- **Effective C++** (Scott Meyers) - 55가지 모범 사례
- **C++ Primer** (Stanley Lippman) - 종합 입문서

---

**작성일**: 2026-02-03  
**대상**: C 언어를 알고 있는 개발자  
**난이도**: 초급 → 중급  
**총 학습 시간**: 약 30시간
