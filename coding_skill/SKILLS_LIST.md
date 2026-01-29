# C 프로그래밍 필수 스킬 및 디자인 패턴 목록

## 📋 개요
임베디드 시스템 및 펌웨어 개발에서 반드시 알아야 할 프로그래밍 패턴과 기술들의 종합 목록입니다.

---

## ✅ 이미 구현된 패턴

| 번호 | 패턴명 | 파일명 | 난이도 | 중요도 |
|------|--------|--------|--------|--------|
| 01 | Wrapper Pattern | 01_wrapper_pattern.c | ⭐⭐ | 🔥🔥🔥 |
| 02 | Adapter Pattern | 02_adapter_pattern.c | ⭐⭐⭐ | 🔥🔥🔥 |

---

## 📝 구현 예정 패턴 및 스킬

### 🎯 기본 디자인 패턴 (Basic Design Patterns)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 03 | **Singleton Pattern** | 전역적으로 단 하나의 인스턴스만 존재 (설정 관리, 로거) | ⭐⭐ | 🔥🔥🔥 | GPIO, UART 드라이버 |
| 04 | **Factory Pattern** | 객체 생성 로직 캡슐화 (센서 객체 생성 통일) | ⭐⭐⭐ | 🔥🔥🔥 | 다중 디바이스 관리 |
| 05 | **Observer Pattern** | 이벤트 발생 시 등록된 옵저버들에게 통지 | ⭐⭐⭐ | 🔥🔥🔥 | 센서 데이터 구독 |
| 06 | **Strategy Pattern** | 알고리즘을 런타임에 교체 가능하도록 캡슐화 | ⭐⭐⭐ | 🔥🔥 | 압축, 암호화 알고리즘 |
| 07 | **Command Pattern** | 요청을 객체로 캡슐화 (실행 취소/재실행 지원) | ⭐⭐⭐ | 🔥🔥 | 모터 제어, UI 명령 |

### 🔄 상태 및 이벤트 관리 (State & Event Management)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 08 | **State Machine** | 상태 전이 기반 시스템 설계 | ⭐⭐⭐⭐ | 🔥🔥🔥🔥 | FOTA, 통신 프로토콜 |
| 09 | **Callback Pattern** | 함수 포인터로 비동기 이벤트 처리 | ⭐⭐ | 🔥🔥🔥🔥 | 인터럽트, RTOS 태스크 |
| 10 | **Event Queue** | 이벤트를 큐에 저장하고 순차 처리 | ⭐⭐⭐ | 🔥🔥🔥 | 이벤트 드리븐 시스템 |

### 💾 메모리 관리 패턴 (Memory Management Patterns)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 11 | **Memory Pool** | 고정 크기 메모리 블록 사전 할당 | ⭐⭐⭐ | 🔥🔥🔥🔥 | 실시간 시스템 |
| 12 | **Object Pool** | 객체 재사용 풀 관리 | ⭐⭐⭐ | 🔥🔥🔥 | 패킷 버퍼 관리 |
| 13 | **Reference Counting** | 객체 참조 카운팅으로 자동 메모리 해제 | ⭐⭐⭐⭐ | 🔥🔥 | 공유 리소스 관리 |

### 📦 데이터 구조 패턴 (Data Structure Patterns)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 14 | **Ring Buffer** | 순환 버퍼 (FIFO + 고정 메모리) | ⭐⭐⭐ | 🔥🔥🔥🔥 | UART 수신 버퍼 |
| 15 | **Linked List** | 동적 리스트 관리 | ⭐⭐ | 🔥🔥🔥 | 태스크 큐, 이벤트 리스트 |
| 16 | **Double Buffer** | 읽기/쓰기 버퍼 분리 (더블 버퍼링) | ⭐⭐⭐ | 🔥🔥🔥 | DMA, 그래픽 렌더링 |

### 🔐 리소스 관리 패턴 (Resource Management)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 17 | **RAII (Resource Acquisition Is Initialization)** | 리소스 자동 관리 (C 스타일) | ⭐⭐⭐ | 🔥🔥 | 파일, 뮤텍스 |
| 18 | **Semaphore Pattern** | 동기화 및 리소스 제어 | ⭐⭐⭐ | 🔥🔥🔥🔥 | RTOS 멀티태스킹 |
| 19 | **Mutex Guard** | 뮤텍스 자동 잠금/해제 | ⭐⭐ | 🔥🔥🔥 | 크리티컬 섹션 |

### ⚡ 성능 최적화 패턴 (Performance Optimization)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 20 | **Lazy Initialization** | 필요할 때만 초기화 | ⭐⭐ | 🔥🔥 | 센서 늦은 초기화 |
| 21 | **Cache Pattern** | 자주 사용하는 데이터 캐싱 | ⭐⭐⭐ | 🔥🔥🔥 | Flash 데이터 캐시 |
| 22 | **Zero-Copy** | 메모리 복사 없이 데이터 전달 | ⭐⭐⭐⭐ | 🔥🔥🔥 | 고속 데이터 처리 |

### 🛡️ 안전성 패턴 (Safety & Reliability)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 23 | **Watchdog Pattern** | 시스템 무한 루프 감지 | ⭐⭐ | 🔥🔥🔥🔥 | 안전 시스템 |
| 24 | **Retry Pattern** | 실패 시 재시도 로직 | ⭐⭐ | 🔥🔥🔥 | 통신, 센서 읽기 |
| 25 | **Failsafe Pattern** | 오류 발생 시 안전 모드 전환 | ⭐⭐⭐ | 🔥🔥🔥🔥 | 모터 제어, 브레이크 |

### 🔌 하드웨어 추상화 패턴 (Hardware Abstraction)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 26 | **HAL Pattern** | 하드웨어 추상화 계층 | ⭐⭐⭐⭐ | 🔥🔥🔥🔥 | MCU 이식성 |
| 27 | **Driver Interface** | 드라이버 인터페이스 표준화 | ⭐⭐⭐ | 🔥🔥🔥 | GPIO, I2C, SPI |
| 28 | **Interrupt Handler** | 인터럽트 최소화 및 분리 | ⭐⭐⭐⭐ | 🔥🔥🔥🔥 | ISR 설계 |

### 🧪 테스트 및 디버깅 패턴 (Testing & Debugging)

| 번호 | 패턴명 | 설명 | 난이도 | 중요도 | 실무 활용도 |
|------|--------|------|--------|--------|------------|
| 29 | **Mock Object** | 테스트용 가짜 객체 | ⭐⭐⭐ | 🔥🔥🔥 | 단위 테스트 |
| 30 | **Assertion Pattern** | 런타임 검증 | ⭐⭐ | 🔥🔥🔥 | 디버깅 |
| 31 | **Tracing Pattern** | 함수 호출 추적 | ⭐⭐ | 🔥🔥 | 성능 분석 |

---

## 🎓 학습 순서 추천

### 초급 단계 (필수 기본기)
1. ✅ Wrapper Pattern
2. ✅ Adapter Pattern
3. **Singleton Pattern** ← 다음 학습
4. **Callback Pattern**
5. **State Machine**

### 중급 단계 (실전 활용)
6. **Factory Pattern**
7. **Observer Pattern**
8. **Ring Buffer**
9. **Memory Pool**
10. **Event Queue**

### 고급 단계 (최적화 & 아키텍처)
11. **Strategy Pattern**
12. **Command Pattern**
13. **HAL Pattern**
14. **Zero-Copy**
15. **Watchdog Pattern**

---

## 💼 실무 프로젝트별 필수 패턴

### FOTA (Firmware Over The Air)
- State Machine ⭐⭐⭐⭐⭐
- Wrapper Pattern ⭐⭐⭐⭐
- Ring Buffer ⭐⭐⭐⭐
- Retry Pattern ⭐⭐⭐

### 센서 모니터링 시스템
- Observer Pattern ⭐⭐⭐⭐⭐
- Factory Pattern ⭐⭐⭐⭐
- Adapter Pattern ⭐⭐⭐⭐
- Event Queue ⭐⭐⭐

### 모터 제어 시스템
- State Machine ⭐⭐⭐⭐⭐
- Command Pattern ⭐⭐⭐⭐
- Failsafe Pattern ⭐⭐⭐⭐⭐
- Watchdog Pattern ⭐⭐⭐⭐

### 통신 프로토콜 스택
- Ring Buffer ⭐⭐⭐⭐⭐
- State Machine ⭐⭐⭐⭐⭐
- Callback Pattern ⭐⭐⭐⭐
- Memory Pool ⭐⭐⭐⭐

### RTOS 멀티태스킹
- Semaphore Pattern ⭐⭐⭐⭐⭐
- Event Queue ⭐⭐⭐⭐
- Memory Pool ⭐⭐⭐⭐
- Observer Pattern ⭐⭐⭐

---

## 📊 패턴별 학습 난이도 및 시간

| 난이도 | 예상 학습 시간 | 패턴 수 |
|--------|---------------|---------|
| ⭐ (쉬움) | 1-2시간 | 0개 |
| ⭐⭐ (보통) | 2-4시간 | 9개 |
| ⭐⭐⭐ (어려움) | 4-8시간 | 17개 |
| ⭐⭐⭐⭐ (매우 어려움) | 8-16시간 | 5개 |

**총 학습 시간 예상:** 약 150-250시간 (실습 포함)

---

## 🎯  핵심 패턴 TOP 10

1. **State Machine** - 가장 많이 물어보는 패턴
2. **Ring Buffer** - 임베디드 필수
3. **Memory Pool** - 실시간 시스템 핵심
4. **Observer Pattern** - 이벤트 기반 설계
5. **Callback Pattern** - 비동기 처리
6. **Singleton Pattern** - 가장 쉬운 패턴
7. **Factory Pattern** - 객체 생성 관리
8. **HAL Pattern** - 이식성 필수
9. **Wrapper Pattern** - 추상화 기본
10. **Adapter Pattern** - 통합 필수

---

## 📚 다음 단계 행동 계획

### 우선순위 1: 기본 패턴 마스터
- [ ] 03_singleton_pattern.c
- [ ] 04_callback_pattern.c
- [ ] 05_state_machine.c

### 우선순위 2: 메모리 관리
- [ ] 06_memory_pool.c
- [ ] 07_ring_buffer_pattern.c
- [ ] 08_object_pool.c

### 우선순위 3: 고급 디자인
- [ ] 09_factory_pattern.c
- [ ] 10_observer_pattern.c
- [ ] 11_strategy_pattern.c

---

## 🔗 각 패턴의 연관 관계

```
Singleton ──┐
            ├──> Factory ──> Observer ──> Event Queue
Wrapper ────┤
            └──> Adapter ──> HAL Pattern
                              │
Ring Buffer ──> Memory Pool ──┤
                              │
Callback ────> State Machine ─┴──> Command Pattern
```

---

## ⚠️ 학습 시 주의사항

1. **패턴은 도구일 뿐** - 무조건 사용하지 말 것
2. **오버 엔지니어링 주의** - 단순한 문제에 복잡한 패턴 금지
3. **실습이 최우선** - 이론만 보지 말고 직접 구현
4. **실무 코드 분석** - 오픈소스 임베디드 프로젝트 참고
5. **성능 측정** - 패턴 적용 전후 비교

---

## 📖 학습 리소스

### 추천 도서
- "Design Patterns for Embedded Systems in C" - Bruce Powel Douglass
- "Patterns in C" - Adam Tornhill
- "Making Embedded Systems" - Elecia White

### 오픈소스 프로젝트 참고
- FreeRTOS (RTOS 패턴)
- lwIP (네트워크 스택)
- mbedTLS (보안 라이브러리)
- Linux Kernel (드라이버 패턴)

---

**마지막 업데이트:** 2026-01-29
**작성자:** C Embedded Systems Study Group
