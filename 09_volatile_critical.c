/**
 * ============================================================================
 * [코딩테스트 Level 9] volatile 키워드와 임계 영역
 * ============================================================================
 * 
 * 문제 설명:
 * 임베디드 시스템에서는 인터럽트 서비스 루틴(ISR)과 메인 루프가 변수를 공유합니다.
 * 컴파일러 최적화로 인해 변수 값이 예상대로 동작하지 않을 수 있습니다.
 * volatile 키워드와 임계 영역 보호는 이러한 문제를 해결하는 핵심 기술입니다.
 * 
 * 요구사항:
 * 1. volatile 키워드가 필요한 상황을 이해하세요.
 * 2. 임계 영역(Critical Section)의 개념을 이해하세요.
 * 3. 인터럽트 비활성화/활성화로 임계 영역을 보호하세요.
 * 4. Thread-safe한 함수 설계 방법을 익히세요.
 * 
 * 학습 포인트:
 * - volatile의 역할 (컴파일러 최적화 방지)
 * - 임계 영역과 Race Condition
 * - ENTER_CRITICAL / EXIT_CRITICAL 패턴
 * - ISR과 메인 루프 간 데이터 공유
 * 
 * ============================================================================
 * File: 09_volatile_critical.c
 * 파일명: 09_volatile_critical.c
 * Author / 작성자: [Developer]
 * Date / 작성일: 2026-01-14
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* ============================================================================
 * Windows UTF-8 Console Output Support / 윈도우 UTF-8 콘솔 출력 지원
 * ============================================================================ */
#ifdef _WIN32
    #include <windows.h>
    #include <locale.h>
    #define ENABLE_UTF8_CONSOLE()   do { \
        SetConsoleOutputCP(CP_UTF8); \
        SetConsoleCP(CP_UTF8); \
        setlocale(LC_ALL, ".UTF-8"); \
    } while(0)
#else
    #include <locale.h>
    #define ENABLE_UTF8_CONSOLE()   setlocale(LC_ALL, "")
#endif

/* ============================================================================
 * Platform-specific Critical Section Macros / 플랫폼별 임계 영역 매크로
 * ============================================================================ */

/**
 * 임계 영역 보호 매크로
 * Critical Section Protection Macros
 * 
 * ARM Cortex-M:
 *   __disable_irq() / __enable_irq()
 * 
 * AVR:
 *   cli() / sei()
 * 
 * Windows (테스트용):
 *   EnterCriticalSection() / LeaveCriticalSection()
 * 
 * 일반 환경 (시뮬레이션):
 *   더미 구현 (실제 인터럽트 없음)
 */

#ifdef __ARM_ARCH
    /* ARM Cortex-M 시리즈 */
    #define ENTER_CRITICAL()    __disable_irq()
    #define EXIT_CRITICAL()     __enable_irq()
    #define PLATFORM_HAS_INTERRUPTS     1
#elif defined(__AVR__)
    /* AVR 마이크로컨트롤러 */
    #include <avr/interrupt.h>
    #define ENTER_CRITICAL()    cli()
    #define EXIT_CRITICAL()     sei()
    #define PLATFORM_HAS_INTERRUPTS     1
#else
    /* 일반 환경 - 시뮬레이션용 더미 구현 */
    static volatile int _critical_nesting = 0;
    #define ENTER_CRITICAL()    do { _critical_nesting++; } while(0)
    #define EXIT_CRITICAL()     do { _critical_nesting--; } while(0)
    #define PLATFORM_HAS_INTERRUPTS     0
#endif

/* ============================================================================
 * Volatile Demonstration / volatile 시연
 * ============================================================================ */

/**
 * volatile이 필요한 변수들
 * Variables that need volatile
 * 
 * 1. ISR에서 수정하고 메인에서 읽는 변수
 * 2. 메인에서 수정하고 ISR에서 읽는 변수
 * 3. 하드웨어 레지스터 (Memory-mapped I/O)
 */

/* ISR과 공유되는 변수들 - volatile 필수 */
/* Variables shared with ISR - volatile required */
volatile uint32_t g_sensorData = 0;         /* ISR에서 업데이트 */
volatile bool g_dataReady = false;          /* 데이터 준비 플래그 */
volatile uint32_t g_interruptCount = 0;     /* 인터럽트 횟수 */
volatile uint32_t g_tickCounter = 0;        /* 시스템 틱 카운터 */

/* volatile 없이 선언된 변수 (문제 발생 가능) */
/* Variable without volatile (can cause problems) */
uint32_t g_nonVolatileCounter = 0;

/**
 * @brief Simulated ISR (Interrupt Service Routine)
 *        시뮬레이션된 인터럽트 서비스 루틴
 * 
 * 실제 임베디드 시스템에서는 하드웨어 인터럽트에 의해 호출됩니다.
 * In real embedded systems, this is called by hardware interrupts.
 */
void simulatedISR(void) {
    /* 이 함수는 "인터럽트"처럼 갑자기 호출된다고 가정 */
    g_sensorData = 12345;
    g_dataReady = true;
    g_interruptCount++;
    g_tickCounter++;
}

/**
 * @brief Demonstrate why volatile is needed
 *        volatile이 필요한 이유 시연
 */
void demonstrateVolatileNeed(void) {
    printf("============ Why volatile is Needed / volatile이 필요한 이유 ============\n\n");
    
    printf("문제 상황 (volatile 없을 때):\n");
    printf("┌────────────────────────────────────────────────────────────────┐\n");
    printf("│  uint32_t counter = 0;  // volatile 없음                      │\n");
    printf("│                                                                │\n");
    printf("│  void ISR() {                                                  │\n");
    printf("│      counter++;  // 인터럽트에서 증가                          │\n");
    printf("│  }                                                             │\n");
    printf("│                                                                │\n");
    printf("│  void main() {                                                 │\n");
    printf("│      while (counter < 10) {  // 무한 루프 가능!               │\n");
    printf("│          // 컴파일러가 counter를 레지스터에 캐시               │\n");
    printf("│          // -> 메모리의 실제 값 변경을 감지 못함               │\n");
    printf("│      }                                                         │\n");
    printf("│  }                                                             │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n\n");
    
    printf("컴파일러 최적화 결과 (문제!):\n");
    printf("┌────────────────────────────────────────────────────────────────┐\n");
    printf("│  LDR R0, [counter]    ; counter를 R0에 한 번만 로드            │\n");
    printf("│  loop:                                                         │\n");
    printf("│      CMP R0, #10      ; 항상 같은 값 비교                      │\n");
    printf("│      BLT loop         ; 영원히 루프!                           │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n\n");
    
    printf("해결책 (volatile 사용):\n");
    printf("┌────────────────────────────────────────────────────────────────┐\n");
    printf("│  volatile uint32_t counter = 0;  // volatile 선언!            │\n");
    printf("│                                                                │\n");
    printf("│  컴파일러 최적화 결과:                                         │\n");
    printf("│  loop:                                                         │\n");
    printf("│      LDR R0, [counter]    ; 매번 메모리에서 로드               │\n");
    printf("│      CMP R0, #10                                               │\n");
    printf("│      BLT loop                                                  │\n");
    printf("│  -> ISR에서 변경된 값을 정상적으로 감지!                       │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n");
    
    printf("=========================================================================\n");
}

/**
 * @brief Demonstrate volatile usage scenarios
 *        volatile 사용 시나리오 시연
 */
void demonstrateVolatileScenarios(void) {
    printf("\n============ volatile Usage Scenarios / volatile 사용 시나리오 ============\n\n");
    
    printf("1. ISR과 메인 루프 간 플래그:\n");
    printf("   volatile bool g_dataReady = false;\n");
    printf("   ISR:  g_dataReady = true;\n");
    printf("   Main: while (!g_dataReady) { /* wait */ }\n\n");
    
    printf("2. 공유 카운터:\n");
    printf("   volatile uint32_t g_tickCounter = 0;\n");
    printf("   ISR:  g_tickCounter++;  (1ms 타이머)\n");
    printf("   Main: delay_ms = g_tickCounter + 100;\n\n");
    
    printf("3. 하드웨어 레지스터 (Memory-mapped I/O):\n");
    printf("   #define GPIO_REG  (*(volatile uint32_t*)0x40021000)\n");
    printf("   GPIO_REG = 0x01;  // LED ON\n");
    printf("   while (GPIO_REG & 0x02);  // 상태 대기\n\n");
    
    printf("4. DMA 버퍼:\n");
    printf("   volatile uint8_t dmaBuffer[256];\n");
    printf("   // DMA가 백그라운드에서 데이터 채움\n");
    printf("   // CPU가 데이터 읽을 때 volatile 필요\n");
    
    printf("===========================================================================\n");
}

/* ============================================================================
 * Critical Section Demonstration / 임계 영역 시연
 * ============================================================================ */

/**
 * @brief Demonstrate race condition / Race Condition 시연
 */
void demonstrateRaceCondition(void) {
    printf("\n============ Race Condition / 경쟁 상태 ============\n\n");
    
    printf("문제 상황: count++ 연산은 원자적이지 않음!\n\n");
    
    printf("count++ 연산의 실제 동작 (3단계):\n");
    printf("┌────────────────────────────────────────────────────────────────┐\n");
    printf("│  1. LOAD:  temp = count;     // 메모리에서 읽기                │\n");
    printf("│  2. ADD:   temp = temp + 1;  // 1 증가                         │\n");
    printf("│  3. STORE: count = temp;     // 메모리에 쓰기                  │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n\n");
    
    printf("Race Condition 예시 (count 초기값: 5):\n");
    printf("┌──────────────────────────────────────────────────────────────────────┐\n");
    printf("│  Time │ Main Loop          │ ISR (인터럽트)      │ count 값        │\n");
    printf("├──────────────────────────────────────────────────────────────────────┤\n");
    printf("│  T1   │ temp = count (5)   │                     │ 5               │\n");
    printf("│  T2   │ temp = 5 + 1 = 6   │                     │ 5               │\n");
    printf("│  T3   │ <-- 인터럽트 발생! │ temp = count (5)    │ 5               │\n");
    printf("│  T4   │                    │ temp = 5 + 1 = 6    │ 5               │\n");
    printf("│  T5   │                    │ count = 6           │ 6               │\n");
    printf("│  T6   │ count = 6          │ <-- ISR 종료        │ 6 (7이어야 함!) │\n");
    printf("└──────────────────────────────────────────────────────────────────────┘\n\n");
    
    printf("결과: 두 번 증가해야 하는데 한 번만 증가됨!\n");
    printf("원인: 읽기-수정-쓰기 사이에 인터럽트 발생\n");
    
    printf("=============================================================\n");
}

/**
 * @brief Demonstrate critical section protection
 *        임계 영역 보호 시연
 */
void demonstrateCriticalSection(void) {
    printf("\n============ Critical Section Protection / 임계 영역 보호 ============\n\n");
    
    printf("해결책: 임계 영역 보호\n\n");
    
    printf("방법 1: 인터럽트 비활성화/활성화\n");
    printf("┌────────────────────────────────────────────────────────────────┐\n");
    printf("│  ENTER_CRITICAL();    // 인터럽트 비활성화                     │\n");
    printf("│  count++;             // 원자적으로 실행됨 (인터럽트 없음)     │\n");
    printf("│  EXIT_CRITICAL();     // 인터럽트 활성화                       │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n\n");
    
    printf("방법 2: 원자적 연산 (Atomic Operations)\n");
    printf("┌────────────────────────────────────────────────────────────────┐\n");
    printf("│  __atomic_add_fetch(&count, 1, __ATOMIC_SEQ_CST);             │\n");
    printf("│  // 또는                                                       │\n");
    printf("│  atomic_fetch_add(&count, 1);  // C11 <stdatomic.h>           │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n\n");
    
    printf("구현 예시 (Circular Buffer용):\n");
    printf("┌────────────────────────────────────────────────────────────────┐\n");
    printf("│  int cb_push_safe(CircularBuffer* cb, int data) {             │\n");
    printf("│      int result;                                               │\n");
    printf("│      ENTER_CRITICAL();                                         │\n");
    printf("│      result = cb_push(cb, data);  // 일반 push 호출           │\n");
    printf("│      EXIT_CRITICAL();                                          │\n");
    printf("│      return result;                                            │\n");
    printf("│  }                                                             │\n");
    printf("└────────────────────────────────────────────────────────────────┘\n");
    
    printf("=========================================================================\n");
}

/* ============================================================================
 * Thread-Safe Counter Example / Thread-Safe 카운터 예제
 * ============================================================================ */

/**
 * Thread-safe counter structure / Thread-safe 카운터 구조체
 */
typedef struct {
    volatile uint32_t value;    /* volatile 필수 */
} SafeCounter_t;

/**
 * @brief Initialize safe counter / 안전한 카운터 초기화
 */
void safeCounter_init(SafeCounter_t* counter) {
    if (counter != NULL) {
        counter->value = 0;
    }
}

/**
 * @brief Increment counter safely / 안전하게 카운터 증가
 */
void safeCounter_increment(SafeCounter_t* counter) {
    if (counter == NULL) return;
    
    ENTER_CRITICAL();
    counter->value++;
    EXIT_CRITICAL();
}

/**
 * @brief Decrement counter safely / 안전하게 카운터 감소
 */
void safeCounter_decrement(SafeCounter_t* counter) {
    if (counter == NULL) return;
    
    ENTER_CRITICAL();
    if (counter->value > 0) {
        counter->value--;
    }
    EXIT_CRITICAL();
}

/**
 * @brief Get counter value safely / 안전하게 카운터 값 읽기
 */
uint32_t safeCounter_get(SafeCounter_t* counter) {
    if (counter == NULL) return 0;
    
    uint32_t value;
    ENTER_CRITICAL();
    value = counter->value;
    EXIT_CRITICAL();
    
    return value;
}

/**
 * @brief Demonstrate safe counter / 안전한 카운터 시연
 */
void demonstrateSafeCounter(void) {
    printf("\n============ Safe Counter Demo / 안전한 카운터 시연 ============\n\n");
    
    SafeCounter_t counter;
    safeCounter_init(&counter);
    
    printf("초기값: %u\n", safeCounter_get(&counter));
    
    printf("\n증가 5회:\n");
    for (int i = 0; i < 5; i++) {
        safeCounter_increment(&counter);
        printf("  After increment: %u\n", safeCounter_get(&counter));
    }
    
    printf("\n감소 3회:\n");
    for (int i = 0; i < 3; i++) {
        safeCounter_decrement(&counter);
        printf("  After decrement: %u\n", safeCounter_get(&counter));
    }
    
    printf("\n최종값: %u\n", safeCounter_get(&counter));
    
    printf("=============================================================\n");
}

/* ============================================================================
 * Best Practices Summary / 모범 사례 요약
 * ============================================================================ */

void printBestPractices(void) {
    printf("\n============ Best Practices / 모범 사례 요약 ============\n\n");
    
    printf("1. volatile 사용:\n");
    printf("   ✓ ISR과 공유되는 모든 변수에 volatile 선언\n");
    printf("   ✓ 하드웨어 레지스터 접근 시 volatile 사용\n");
    printf("   ✗ volatile은 원자성을 보장하지 않음!\n\n");
    
    printf("2. 임계 영역:\n");
    printf("   ✓ 읽기-수정-쓰기 연산은 임계 영역으로 보호\n");
    printf("   ✓ 임계 영역은 최대한 짧게 유지\n");
    printf("   ✓ 중첩 임계 영역 주의 (nesting 처리 필요)\n\n");
    
    printf("3. ISR 설계:\n");
    printf("   ✓ ISR은 최대한 짧고 빠르게\n");
    printf("   ✓ 복잡한 처리는 플래그 설정 후 메인에서 수행\n");
    printf("   ✓ ISR에서 printf 등 느린 함수 호출 금지\n\n");
    
    printf("4. 데이터 공유 패턴:\n");
    printf("   Producer (ISR) -> Consumer (Main):\n");
    printf("   ┌─────────────────────────────────────────┐\n");
    printf("   │ ISR:                                    │\n");
    printf("   │   buffer[writeIndex] = data;           │\n");
    printf("   │   writeIndex = (writeIndex + 1) & mask;│\n");
    printf("   │   dataReady = true;                    │\n");
    printf("   │                                        │\n");
    printf("   │ Main:                                  │\n");
    printf("   │   if (dataReady) {                     │\n");
    printf("   │       process(buffer[readIndex]);      │\n");
    printf("   │       readIndex = (readIndex + 1) & mask;│\n");
    printf("   │       dataReady = false;               │\n");
    printf("   │   }                                    │\n");
    printf("   └─────────────────────────────────────────┘\n");
    
    printf("==========================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 9: volatile & Critical Section\n");
    printf("  Level 9: volatile과 임계 영역\n");
    printf("========================================\n\n");
    
#if PLATFORM_HAS_INTERRUPTS
    printf("[INFO] 실제 인터럽트 지원 플랫폼\n\n");
#else
    printf("[INFO] 시뮬레이션 모드 (인터럽트 없음)\n\n");
#endif
    
    /* Test 1: volatile 필요성 */
    printf("[Test 1] Why volatile is Needed\n");
    demonstrateVolatileNeed();
    
    /* Test 2: volatile 사용 시나리오 */
    printf("\n[Test 2] volatile Usage Scenarios\n");
    demonstrateVolatileScenarios();
    
    /* Test 3: Race Condition */
    printf("\n[Test 3] Race Condition\n");
    demonstrateRaceCondition();
    
    /* Test 4: Critical Section Protection */
    printf("\n[Test 4] Critical Section Protection\n");
    demonstrateCriticalSection();
    
    /* Test 5: Safe Counter Demo */
    printf("\n[Test 5] Safe Counter Demo\n");
    demonstrateSafeCounter();
    
    /* Test 6: Best Practices */
    printf("\n[Test 6] Best Practices Summary\n");
    printBestPractices();
    
    printf("\n========================================\n");
    printf("  All Tests Completed\n");
    printf("  모든 테스트 완료\n");
    printf("========================================\n");
    
    return 0;
}

/**
 * ============================================================================
 * Expected Output / 예상 출력:
 * ============================================================================
 * 
 * [Test 1] volatile 없을 때 컴파일러 최적화로 인한 문제 설명
 * [Test 2] ISR, 하드웨어 레지스터 등 volatile 사용 시나리오
 * [Test 3] Race Condition 문제 상황 시각화
 * [Test 4] ENTER_CRITICAL/EXIT_CRITICAL 패턴 설명
 * [Test 5] Thread-safe 카운터 동작 확인
 * [Test 6] 모범 사례 요약
 * 
 * ============================================================================
 */
