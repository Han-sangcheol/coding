# C언어 디자인 패턴 학습 자료 (완전판)

## 📚 개요

이 폴더는 임베디드 시스템 및 펌웨어 개발에 필수적인 **32개의 C 언어 디자인 패턴과 프로그래밍 기법**을 포함하고 있습니다.

**총 패턴 수: 32개** (00-31번)
- 기초: 함수 포인터 (00)
- 기본 패턴: 10개 (01-10)
- 메모리/데이터 구조: 6개 (11-16)
- 리소스/성능: 6개 (17-22)
- 안전성/하드웨어: 6개 (23-28)
- 테스트/디버깅: 3개 (29-31)

## 🌐 다른 언어로 배우기

같은 패턴을 다른 언어로도 학습할 수 있습니다!

- **C++ 버전**: [`../coding_skill_cpp/`](../coding_skill_cpp/) - RAII, 스마트 포인터, 템플릿
- **Python 버전**: [`../coding_skill_py/`](../coding_skill_py/) - 데코레이터, 컨텍스트 매니저

| 특징 | C | C++ | Python |
|------|---|-----|--------|
| 성능 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐ |
| 안전성 | ⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| 표현력 | ⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| 학습 난이도 | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐ |
| 임베디드 적합 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐ |

---

## 📂 전체 파일 목록

### 기본 디자인 패턴 (01-10)

| 번호 | 파일명 | 패턴명 | 난이도 | 중요도 |
|------|--------|--------|--------|--------|
| 01 | `01_wrapper_pattern.c` | Wrapper | ⭐⭐ | 🔥🔥🔥 |
| 02 | `02_adapter_pattern.c` | Adapter | ⭐⭐⭐ | 🔥🔥🔥 |
| 03 | `03_singleton_pattern.c` | Singleton | ⭐⭐ | 🔥🔥🔥 |
| 04 | `04_callback_pattern.c` | Callback | ⭐⭐ | 🔥🔥🔥🔥 |
| 05 | `05_state_machine.c` | State Machine | ⭐⭐⭐⭐ | 🔥🔥🔥🔥 |
| 06 | `06_factory_pattern.c` | Factory | ⭐⭐⭐ | 🔥🔥🔥 |
| 07 | `07_observer_pattern.c` | Observer | ⭐⭐⭐ | 🔥🔥🔥 |
| 08 | `08_strategy_pattern.c` | Strategy | ⭐⭐⭐ | 🔥🔥 |
| 09 | `09_command_pattern.c` | Command | ⭐⭐⭐ | 🔥🔥 |
| 10 | `10_event_queue.c` | Event Queue | ⭐⭐⭐ | 🔥🔥🔥 |

### 메모리/데이터 구조 패턴 (11-16)

| 번호 | 파일명 | 패턴명 | 난이도 | 중요도 |
|------|--------|--------|--------|--------|
| 11 | `11_memory_pool.c` | Memory Pool | ⭐⭐⭐ | 🔥🔥🔥🔥 |
| 12 | `12_object_pool.c` | Object Pool | ⭐⭐⭐ | 🔥🔥🔥 |
| 13 | `13_reference_counting.c` | Reference Counting | ⭐⭐⭐⭐ | 🔥🔥 |
| 14 | `14_ring_buffer.c` | Ring Buffer | ⭐⭐⭐ | 🔥🔥🔥🔥 |
| 15 | `15_linked_list.c` | Linked List | ⭐⭐ | 🔥🔥🔥 |
| 16 | `16_double_buffer.c` | Double Buffer | ⭐⭐⭐ | 🔥🔥🔥 |

### 리소스/성능 패턴 (17-22)

| 번호 | 파일명 | 패턴명 | 난이도 | 중요도 |
|------|--------|--------|--------|--------|
| 17 | `17_raii_pattern.c` | RAII | ⭐⭐⭐ | 🔥🔥 |
| 18 | `18_semaphore_pattern.c` | Semaphore | ⭐⭐⭐ | 🔥🔥🔥🔥 |
| 19 | `19_mutex_guard.c` | Mutex Guard | ⭐⭐ | 🔥🔥🔥 |
| 20 | `20_lazy_initialization.c` | Lazy Init | ⭐⭐ | 🔥🔥 |
| 21 | `21_cache_pattern.c` | Cache | ⭐⭐⭐ | 🔥🔥🔥 |
| 22 | `22_zero_copy.c` | Zero-Copy | ⭐⭐⭐⭐ | 🔥🔥🔥 |

### 안전성/하드웨어 패턴 (23-28)

| 번호 | 파일명 | 패턴명 | 난이도 | 중요도 |
|------|--------|--------|--------|--------|
| 23 | `23_watchdog_pattern.c` | Watchdog | ⭐⭐ | 🔥🔥🔥🔥 |
| 24 | `24_retry_pattern.c` | Retry | ⭐⭐ | 🔥🔥🔥 |
| 25 | `25_failsafe_pattern.c` | Failsafe | ⭐⭐⭐ | 🔥🔥🔥🔥 |
| 26 | `26_hal_pattern.c` | HAL | ⭐⭐⭐⭐ | 🔥🔥🔥🔥 |
| 27 | `27_driver_interface.c` | Driver Interface | ⭐⭐⭐ | 🔥🔥🔥 |
| 28 | `28_interrupt_handler.c` | Interrupt Handler | ⭐⭐⭐⭐ | 🔥🔥🔥🔥 |

### 테스트/디버깅 패턴 (29-31)

| 번호 | 파일명 | 패턴명 | 난이도 | 중요도 |
|------|--------|--------|--------|--------|
| 29 | `29_mock_object.c` | Mock Object | ⭐⭐⭐ | 🔥🔥🔥 |
| 30 | `30_assertion_pattern.c` | Assertion | ⭐⭐ | 🔥🔥🔥 |
| 31 | `31_tracing_pattern.c` | Tracing | ⭐⭐ | 🔥🔥 |

---

## 🚀 빠른 시작

### 컴파일

```bash
# 모든 패턴 컴파일
compile_all.bat

# 개별 패턴 컴파일
gcc -o 01_wrapper.exe 01_wrapper_pattern.c -Wall
```

### 실행

```bash
# 대화식 실행 (권장)
run_all.bat

# 개별 실행
01_wrapper.exe
```

---

## 🎯 학습 로드맵

### 🔰 1단계: 기본기 (필수, 1-2주)
**목표: 디자인 패턴의 기초 이해**

1. **Wrapper Pattern** (01) - 기존 함수 감싸기
2. **Adapter Pattern** (02) - 인터페이스 변환
3. **Singleton Pattern** (03) - 전역 인스턴스 관리
4. **Callback Pattern** (04) - 이벤트 기반 프로그래밍
5. **State Machine** (05) - 상태 기반 설계

**학습 방법:**
- 각 파일의 주석을 정독
- 예제 코드 실행 및 수정
- 자신만의 예제 작성

---

### 🔥 2단계: 실전 응용 (중급, 2-3주)
**목표: 실무 프로젝트 적용 가능**

6. **Factory Pattern** (06) - 객체 생성 관리
7. **Observer Pattern** (07) - 일대다 통신
8. **Memory Pool** (11) - 실시간 메모리 관리
9. **Ring Buffer** (14) - UART 버퍼 구현
10. **Event Queue** (10) - 비동기 이벤트 처리

**프로젝트 아이디어:**
- UART 통신 프로그램
- 센서 모니터링 시스템
- 간단한 RTOS 태스크 관리

---

### ⚡ 3단계: 고급 최적화 (고급, 2-4주)
**목표: 성능 최적화 및 안전성 확보**

11. **HAL Pattern** (26) - 하드웨어 추상화
12. **Watchdog Pattern** (23) - 시스템 안전성
13. **Zero-Copy** (22) - 성능 최적화
14. **Double Buffer** (16) - DMA 버퍼링
15. **Interrupt Handler** (28) - ISR 설계

**프로젝트 아이디어:**
- FOTA 시스템 구현
- 모터 제어 시스템
- 실시간 데이터 수집

---

### 🎓 4단계: 마스터 (전문가, 4주+)
**목표: 전체 아키텍처 설계 능력**

16-31번 패턴 모두 습득
- 테스트 자동화 (Mock, Assertion)
- 성능 분석 (Tracing, Cache)
- 안전 시스템 (Failsafe, Retry)

**최종 프로젝트:**
- 완전한 임베디드 펌웨어 구현
- 상용 제품 수준의 코드 작성

---

## 💼 실무 프로젝트별 필수 패턴

### 🔄 FOTA (Firmware Over The Air)
- State Machine (05) ⭐⭐⭐⭐⭐
- Wrapper (01) ⭐⭐⭐⭐
- Ring Buffer (14) ⭐⭐⭐⭐
- Retry (24) ⭐⭐⭐
- Watchdog (23) ⭐⭐⭐

### 📡 센서 모니터링 시스템
- Observer (07) ⭐⭐⭐⭐⭐
- Factory (06) ⭐⭐⭐⭐
- Adapter (02) ⭐⭐⭐⭐
- Event Queue (10) ⭐⭐⭐

### 🎮 모터 제어 시스템
- State Machine (05) ⭐⭐⭐⭐⭐
- Command (09) ⭐⭐⭐⭐
- Failsafe (25) ⭐⭐⭐⭐⭐
- Watchdog (23) ⭐⭐⭐⭐
- Interrupt Handler (28) ⭐⭐⭐⭐

### 📶 통신 프로토콜 스택
- Ring Buffer (14) ⭐⭐⭐⭐⭐
- State Machine (05) ⭐⭐⭐⭐⭐
- Callback (04) ⭐⭐⭐⭐
- Memory Pool (11) ⭐⭐⭐⭐

---

## 📖 면접 대비 핵심 패턴 TOP 10

면접에서 가장 자주 물어보는 패턴 (중요도 순):

1. **State Machine** (05) - 거의 모든 임베디드 면접
2. **Ring Buffer** (14) - UART/DMA 관련 필수
3. **Memory Pool** (11) - 실시간 시스템 필수
4. **Observer** (07) - 이벤트 기반 설계
5. **Callback** (04) - 비동기 처리
6. **Singleton** (03) - 가장 쉬운 패턴
7. **Factory** (06) - 객체 생성 관리
8. **HAL** (26) - 이식성 필수
9. **Wrapper** (01) - 추상화 기본
10. **Adapter** (02) - 통합 필수

**면접 대비 전략:**
- 각 패턴의 "왜 사용하는가?"를 명확히
- 실제 프로젝트 경험과 연결
- 장단점을 균형있게 설명

---

## 🔧 개발 환경

### 필수 도구
- **컴파일러:** GCC (MinGW on Windows)
- **에디터:** VSCode, Cursor, Vim 등
- **버전 관리:** Git

### 권장 학습 환경
- **OS:** Windows / Linux / macOS
- **시간:** 하루 1-2시간, 주 5일
- **학습 기간:** 약 3-4개월 (전체 마스터)

---

## 📚 참고 자료

### 추천 도서
- "Design Patterns for Embedded Systems in C" - Bruce Powel Douglass
- "Patterns in C" - Adam Tornhill
- "Making Embedded Systems" - Elecia White

### 오픈소스 프로젝트
- **FreeRTOS** - RTOS 패턴
- **lwIP** - 네트워크 스택
- **mbedTLS** - 보안 라이브러리
- **Linux Kernel** - 드라이버 패턴

---

## 🎯 학습 체크리스트

### 기본 패턴 (01-10)
- [ ] 01: Wrapper - 기존 함수 감싸기
- [ ] 02: Adapter - 인터페이스 변환
- [ ] 03: Singleton - 전역 인스턴스
- [ ] 04: Callback - 이벤트 처리
- [ ] 05: State Machine - 상태 관리
- [ ] 06: Factory - 객체 생성
- [ ] 07: Observer - 일대다 통신
- [ ] 08: Strategy - 알고리즘 교체
- [ ] 09: Command - 명령 캡슐화
- [ ] 10: Event Queue - 이벤트 큐잉

### 메모리/데이터 구조 (11-16)
- [ ] 11: Memory Pool - 메모리 풀
- [ ] 12: Object Pool - 객체 풀
- [ ] 13: Reference Counting - 참조 카운팅
- [ ] 14: Ring Buffer - 순환 버퍼
- [ ] 15: Linked List - 연결 리스트
- [ ] 16: Double Buffer - 이중 버퍼

### 리소스/성능 (17-22)
- [ ] 17: RAII - 리소스 자동 관리
- [ ] 18: Semaphore - 동기화
- [ ] 19: Mutex Guard - 뮤텍스 가드
- [ ] 20: Lazy Init - 지연 초기화
- [ ] 21: Cache - 캐싱
- [ ] 22: Zero-Copy - 제로 카피

### 안전성/하드웨어 (23-28)
- [ ] 23: Watchdog - 감시 타이머
- [ ] 24: Retry - 재시도
- [ ] 25: Failsafe - 안전 모드
- [ ] 26: HAL - 하드웨어 추상화
- [ ] 27: Driver Interface - 드라이버 인터페이스
- [ ] 28: Interrupt Handler - 인터럽트 처리

### 테스트/디버깅 (29-31)
- [ ] 29: Mock Object - 테스트 객체
- [ ] 30: Assertion - 런타임 검증
- [ ] 31: Tracing - 함수 추적

---

## 💡 학습 팁

### ✅ DO
- 코드를 직접 타이핑하며 학습
- 예제를 수정하며 실험
- 에러를 만들고 해결하며 학습
- 실제 프로젝트에 적용
- 면접 대응 포인트 암기

### ❌ DON'T
- 복사-붙여넣기만
- 이론만 공부
- 한 번에 모두 학습
- 패턴 남용
- 실습 없이 진행

---

## 🏆 학습 성과 측정

### 초급 (1-10번 완료)
- [ ] 10개 패턴 이해
- [ ] 각 패턴 설명 가능
- [ ] 간단한 예제 작성

### 중급 (1-20번 완료)
- [ ] 20개 패턴 마스터
- [ ] 프로젝트에 적용 경험
- [ ] 면접 질문 대응 가능

### 고급 (1-31번 완료)
- [ ] 31개 패턴 완전 마스터
- [ ] 아키텍처 설계 가능
- [ ] 실무 프로젝트 리드 가능

---

## 📧 문의 및 피드백

이 학습 자료에 대한 질문이나 개선 사항이 있다면 언제든지 문의해주세요!

**GitHub Repository:** https://github.com/Han-sangcheol/coding

**Happy Coding! 🎉**

---

**마지막 업데이트:** 2026-01-29
**작성자:** C Embedded Systems Study Group
**버전:** 2.0 (31개 패턴 완전판)
