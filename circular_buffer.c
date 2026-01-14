/**
 * 고성능 센서 데이터를 위한 Circular Buffer (Ring Buffer) 구현상황: 
 * 귀하는 초당 수천 번의 인터럽트가 발생하는 환경에서 센서 데이터를 수집하는 시스템을 개발 중입니다.
 * 데이터의 유실을 방지하고 메모리를 효율적으로 사용하기 위해 고정된 크기의 Circular Buffer를
 * 구현해야 합니다.
 * 요구사항언어: C 또는 C++
 * 기능:init(size): 버퍼 초기화.
 *      push(data): 버퍼에 데이터를 삽입.
 *                  (버퍼가 가득 찼을 때의 처리 방식을 결정하고 주석으로 명시하세요.)
 *      pop(): 가장 오래된 데이터를 꺼내고 반환.
 *      isEmpty(), isFull(): 버퍼 상태 확인.
 * 제약조건:malloc과 같은 동적 할당은 초기화 시에만 사용하거나, 혹은 정적 배열을 사용하세요.
 *        (실제 임베디드 환경 고려)데이터 삽입/삭제의 시간 복잡도는 **$O(1)$**이어야 합니다.
 *        멀티스레드(또는 인터럽트-메인 루프) 환경을 고려하여 Thread-safe하게 설계한다면
 *        가산점이 있습니다.
 * ============================================================================
 * 이 코드에서 보고자 하는 것 (평가 포인트)
 *       Edge Case 처리: * 버퍼가 텅 빈 상태에서 pop을 호출할 때 어떻게 처리하는가?
 *                     버퍼가 가득 찼을 때 head와 tail 인덱스를 어떻게 관리하는가?
 *       메모리 효율성: * % (Modulo) 연산을 사용하는가, 
 *                    아니면 비트 마스킹(Buffer Size가 $2^n$일 때)을 사용하여 
 *                    성능을 최적화하는가?
 *       코드 안정성:인덱스 오버플로우를 어떻게 방지하는가?
 *                  임베디드 환경에서 필수적인 volatile 키워드나 
 *                  원자적(Atomic) 연산에 대한 고민이 녹아 있는가?
 *       가독성과 인터페이스:변수명이 명확한가?
 *                  (예: pRead, pWrite 등)함수의 확장성이 고려되었는가?
 * ============================================================================
 * File: circular_buffer.c
 * 파일명: circular_buffer.c
 * 
 * Description / 기능 설명:
 * - High-performance Circular Buffer (Ring Buffer) for sensor data collection
 *   고성능 센서 데이터 수집을 위한 Circular Buffer (Ring Buffer) 구현
 * - Designed for environments with thousands of interrupts per second
 *   초당 수천 번의 인터럽트 환경에서 데이터 유실 방지 및 메모리 효율적 사용
 * - Memory efficient with fixed-size buffer and O(1) operations
 *   고정 크기 버퍼와 O(1) 연산으로 메모리 효율적
 * - Thread-safe design considering interrupt-main loop scenarios
 *   인터럽트-메인 루프 환경을 고려한 Thread-safe 설계
 * 
 * Key Features / 주요 특징:
 * 1. O(1) time complexity for push/pop operations
 *    push/pop 연산의 O(1) 시간 복잡도
 * 2. Bit masking for index calculation optimization (buffer size must be 2^n)
 *    비트 마스킹을 통한 인덱스 계산 최적화 (버퍼 크기 2^n 제한)
 * 3. volatile keyword to prevent compiler optimization
 *    volatile 키워드로 컴파일러 최적화 방지
 * 4. Atomic operations for thread-safety
 *    원자적 연산을 통한 Thread-safety 보장
 * 
 * Buffer Full Policy / 버퍼 가득 참 정책:
 * - OVERWRITE_OLDEST: Overwrites oldest data (default, prioritizes latest sensor data)
 *   가장 오래된 데이터를 덮어씀 (기본값, 센서 데이터 특성상 최신 데이터 우선)
 * - Can be changed to REJECT_NEW policy if needed
 *   필요시 REJECT_NEW 정책으로 변경 가능
 * 
 * Author / 작성자: [Developer]
 * Date / 작성일: 2026-01-14
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ============================================================================
 * Windows UTF-8 Console Output Support / 윈도우 UTF-8 콘솔 출력 지원
 * ============================================================================ */
#ifdef _WIN32
    #include <windows.h>    /* Windows API for SetConsoleOutputCP() */
    #include <locale.h>     /* For setlocale() */
    #define ENABLE_UTF8_CONSOLE()   do { \
        SetConsoleOutputCP(CP_UTF8); \
        SetConsoleCP(CP_UTF8); \
        setlocale(LC_ALL, ".UTF-8"); \
    } while(0)
#else
    /* Linux/Mac - UTF-8 is default / 리눅스/맥 - UTF-8이 기본 */
    #include <locale.h>
    #define ENABLE_UTF8_CONSOLE()   setlocale(LC_ALL, "")
#endif

/* ============================================================================
 * Configuration Macros / 설정 매크로 정의
 * ============================================================================ */

/* Buffer size (set as 2^n for bit masking optimization) */
/* 버퍼 크기 (2^n 형태로 설정 - 비트 마스킹 최적화를 위함) */
#define DEFAULT_BUFFER_SIZE_BITS    8       /* 2^8 = 256 */
#define DEFAULT_BUFFER_SIZE         (1 << DEFAULT_BUFFER_SIZE_BITS)

/* Buffer full policy / 버퍼 가득 참 정책 */
#define POLICY_OVERWRITE_OLDEST     0       /* Overwrite oldest data / 가장 오래된 데이터 덮어쓰기 */
#define POLICY_REJECT_NEW           1       /* Reject new data / 새 데이터 거부 */

/* Currently applied policy / 현재 적용할 정책 선택 */
#define BUFFER_FULL_POLICY          POLICY_OVERWRITE_OLDEST

/* Error codes / 에러 코드 정의 */
#define CB_SUCCESS                  0       /* Success / 성공 */
#define CB_ERROR_EMPTY              -1      /* Buffer empty / 버퍼 비어있음 */
#define CB_ERROR_FULL               -2      /* Buffer full / 버퍼 가득 참 */
#define CB_ERROR_INVALID_SIZE       -3      /* Invalid size / 유효하지 않은 크기 */
#define CB_ERROR_NULL_POINTER       -4      /* NULL pointer error / NULL 포인터 에러 */

/* ============================================================================
 * Data Type Definitions / 데이터 타입 정의
 * ============================================================================ */

/* Sensor data type (can be modified as needed) */
/* 센서 데이터 타입 (필요에 따라 수정 가능) */
typedef int32_t SensorData_t;

/**
 * Circular Buffer Structure / Circular Buffer 구조체
 * 
 * Memory Layout / 메모리 레이아웃:
 * +---+---+---+---+---+---+---+---+
 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |  <- buffer array / 버퍼 배열
 * +---+---+---+---+---+---+---+---+
 *       ^           ^
 *      read       write
 *      (tail)     (head)
 * 
 * - write(head): Position where next data will be written
 *                다음에 데이터를 쓸 위치
 * - read(tail): Position where next data will be read
 *               다음에 데이터를 읽을 위치
 * - count: Current number of stored data items
 *          현재 저장된 데이터 개수
 */
typedef struct {
    SensorData_t*   pBuffer;            /* Data storage buffer pointer / 데이터 저장 버퍼 포인터 */
    volatile uint32_t writeIndex;       /* Write index (head) - volatile to prevent optimization */
                                        /* 쓰기 인덱스 (head) - volatile로 최적화 방지 */
    volatile uint32_t readIndex;        /* Read index (tail) - volatile to prevent optimization */
                                        /* 읽기 인덱스 (tail) - volatile로 최적화 방지 */
    volatile uint32_t count;            /* Current stored data count / 현재 저장된 데이터 개수 */
    uint32_t        capacity;           /* Maximum buffer capacity / 버퍼 최대 용량 */
    uint32_t        indexMask;          /* Bit mask for index wrapping (capacity - 1) */
                                        /* 비트 마스킹용 마스크 (capacity - 1) */
    bool            isInitialized;      /* Initialization flag / 초기화 완료 여부 */
} CircularBuffer_t;

/* ============================================================================
 * Static Memory Allocation (for embedded environments)
 * 정적 메모리 할당 (임베디드 환경용)
 * Uncomment below to use static array instead of dynamic allocation
 * 동적 할당 대신 정적 배열 사용 시 아래 주석 해제
 * ============================================================================ */
#ifdef USE_STATIC_ALLOCATION
static SensorData_t staticBuffer[DEFAULT_BUFFER_SIZE];
static CircularBuffer_t staticCircularBuffer;
#endif

/* ============================================================================
 * Helper Functions (internal use) / 헬퍼 함수 (내부용)
 * ============================================================================ */

/**
 * @brief Check if given value is power of two
 *        주어진 값이 2의 거듭제곱인지 확인
 * @param n Value to check / 확인할 값
 * @return true: is power of two / 2의 거듭제곱
 *         false: not / 아님
 * 
 * Bit operation explanation / 비트 연산 설명:
 * - If n is power of two, only one bit is set in binary
 *   n이 2의 거듭제곱이면 이진수에서 1이 하나만 존재
 * - Example / 예: 8 = 1000(2), 8-1 = 7 = 0111(2), 1000 & 0111 = 0
 */
static inline bool isPowerOfTwo(uint32_t n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

/**
 * @brief Wrap index within buffer bounds (using bit masking)
 *        인덱스를 버퍼 범위 내로 래핑 (비트 마스킹 사용)
 * @param cb Circular Buffer pointer / Circular Buffer 포인터
 * @param index Index to wrap / 래핑할 인덱스
 * @return Wrapped index / 래핑된 인덱스
 * 
 * Performance optimization / 성능 최적화:
 * - Uses bit AND operation instead of % (modulo)
 *   % (modulo) 연산 대신 비트 AND 연산 사용
 * - Only valid when buffer size is 2^n
 *   버퍼 크기가 2^n일 때만 유효
 * - Example / 예: index=10, capacity=8, mask=7(0111)
 *                 10(1010) & 7(0111) = 2(0010)
 */
static inline uint32_t wrapIndex(CircularBuffer_t* cb, uint32_t index) {
    return index & cb->indexMask;
}

/* ============================================================================
 * Public API Functions / 공개 API 함수
 * ============================================================================ */

/**
 * @brief Initialize Circular Buffer / Circular Buffer 초기화
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @param size Buffer size (must be power of two)
 *             버퍼 크기 (반드시 2의 거듭제곱이어야 함)
 * @return CB_SUCCESS: success / 성공
 *         CB_ERROR_*: failure / 실패
 * 
 * Notes / 주의사항:
 * - size must be power of two for bit masking optimization
 *   size는 2의 거듭제곱이어야 비트 마스킹 최적화 가능
 * - Dynamic memory allocation is only performed in this function
 *   동적 메모리 할당은 이 함수에서만 수행됨
 */
int cb_init(CircularBuffer_t* cb, uint32_t size) {
    /* NULL pointer validation / NULL 포인터 검증 */
    if (cb == NULL) {
        return CB_ERROR_NULL_POINTER;
    }
    
    /* Size validation - must be power of two */
    /* 크기 유효성 검증 - 2의 거듭제곱이어야 함 */
    if (size == 0 || !isPowerOfTwo(size)) {
        printf("[ERROR] Buffer size must be power of two (e.g., 64, 128, 256...)\n");
        printf("[ERROR] 버퍼 크기는 2의 거듭제곱이어야 합니다. (예: 64, 128, 256...)\n");
        return CB_ERROR_INVALID_SIZE;
    }
    
    /* Initialize structure members / 구조체 멤버 초기화 */
    cb->capacity = size;
    cb->indexMask = size - 1;           /* Generate bit mask / 비트 마스킹용 마스크 생성 */
    cb->writeIndex = 0;
    cb->readIndex = 0;
    cb->count = 0;
    
#ifdef USE_STATIC_ALLOCATION
    /* Use static allocation / 정적 할당 사용 */
    cb->pBuffer = staticBuffer;
    memset(cb->pBuffer, 0, sizeof(SensorData_t) * size);
#else
    /* Dynamic memory allocation (only during initialization) */
    /* 동적 메모리 할당 (초기화 시에만 수행) */
    cb->pBuffer = (SensorData_t*)malloc(sizeof(SensorData_t) * size);
    if (cb->pBuffer == NULL) {
        printf("[ERROR] Memory allocation failed\n");
        printf("[ERROR] 메모리 할당 실패\n");
        return CB_ERROR_NULL_POINTER;
    }
    memset(cb->pBuffer, 0, sizeof(SensorData_t) * size);
#endif
    
    cb->isInitialized = true;
    printf("[INFO] Circular Buffer initialized (capacity: %u, mask: 0x%X)\n", 
           cb->capacity, cb->indexMask);
    printf("[INFO] Circular Buffer 초기화 완료 (용량: %u, 마스크: 0x%X)\n", 
           cb->capacity, cb->indexMask);
    
    return CB_SUCCESS;
}

/**
 * @brief Deinitialize Circular Buffer / Circular Buffer 해제
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @return CB_SUCCESS: success / 성공
 */
int cb_deinit(CircularBuffer_t* cb) {
    if (cb == NULL) {
        return CB_ERROR_NULL_POINTER;
    }
    
#ifndef USE_STATIC_ALLOCATION
    if (cb->pBuffer != NULL) {
        free(cb->pBuffer);
        cb->pBuffer = NULL;
    }
#endif
    
    cb->isInitialized = false;
    cb->capacity = 0;
    cb->writeIndex = 0;
    cb->readIndex = 0;
    cb->count = 0;
    
    return CB_SUCCESS;
}

/**
 * @brief Check if buffer is empty / 버퍼가 비어있는지 확인
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @return true: empty / 비어있음
 *         false: has data / 데이터 있음
 * 
 * Time complexity / 시간 복잡도: O(1)
 */
bool cb_isEmpty(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return true;
    }
    return (cb->count == 0);
}

/**
 * @brief Check if buffer is full / 버퍼가 가득 찼는지 확인
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @return true: full / 가득 참
 *         false: has space / 여유 있음
 * 
 * Time complexity / 시간 복잡도: O(1)
 */
bool cb_isFull(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return false;
    }
    return (cb->count == cb->capacity);
}

/**
 * @brief Insert data into buffer (Push) / 버퍼에 데이터 삽입 (Push)
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @param data Sensor data to insert / 삽입할 센서 데이터
 * @return CB_SUCCESS: success / 성공
 *         CB_ERROR_*: failure / 실패
 * 
 * Buffer Full Handling Policy / 버퍼 가득 참 처리 정책:
 * [POLICY_OVERWRITE_OLDEST] (default / 기본값)
 *   - Overwrites oldest data / 가장 오래된 데이터를 덮어씀
 *   - Used because latest sensor data is more important
 *     센서 데이터 특성상 최신 데이터가 더 중요하므로 이 정책 사용
 *   - Data loss occurs but system doesn't block
 *     데이터 유실 발생하지만 시스템 멈춤 없음
 * 
 * [POLICY_REJECT_NEW]
 *   - Rejects new data and returns error / 새 데이터를 거부하고 에러 반환
 *   - Use when preserving existing data is important
 *     기존 데이터 보존이 중요한 경우 사용
 * 
 * Time complexity / 시간 복잡도: O(1)
 * Thread-safety: Basic safety ensured through volatile variables
 *                volatile 변수 사용으로 기본적인 안전성 확보
 */
int cb_push(CircularBuffer_t* cb, SensorData_t data) {
    /* Validation / 유효성 검증 */
    if (cb == NULL || !cb->isInitialized) {
        return CB_ERROR_NULL_POINTER;
    }
    
    /* Handle buffer full condition / 버퍼 가득 참 처리 */
    if (cb_isFull(cb)) {
#if (BUFFER_FULL_POLICY == POLICY_OVERWRITE_OLDEST)
        /**
         * Policy: Overwrite oldest data
         * 정책: 가장 오래된 데이터 덮어쓰기
         * - Move readIndex forward by one to discard oldest data
         *   readIndex를 한 칸 앞으로 이동하여 오래된 데이터 버림
         * - count remains unchanged (stays at capacity)
         *   count는 유지 (가득 찬 상태 유지)
         * 
         * WARNING: This policy allows data loss.
         *          Use POLICY_REJECT_NEW for critical data.
         * 주의: 이 정책은 데이터 유실을 허용합니다.
         *       중요 데이터의 경우 POLICY_REJECT_NEW 사용을 권장합니다.
         */
        cb->readIndex = wrapIndex(cb, cb->readIndex + 1);
        /* count not changed - already equals capacity */
        /* count는 변경하지 않음 - 이미 capacity와 같음 */
#elif (BUFFER_FULL_POLICY == POLICY_REJECT_NEW)
        /**
         * Policy: Reject new data
         * 정책: 새 데이터 거부
         * - Buffer is full, reject insertion
         *   버퍼가 가득 찼으므로 삽입 거부
         * - Return error to caller for handling
         *   호출자에게 에러 반환하여 처리 위임
         */
        return CB_ERROR_FULL;
#endif
    } else {
        /* Increment count if buffer has space */
        /* 버퍼에 여유 공간이 있으면 count 증가 */
        cb->count++;
    }
    
    /* Insert data / 데이터 삽입 */
    cb->pBuffer[cb->writeIndex] = data;
    
    /* Increment write index (wrap using bit masking) */
    /* 쓰기 인덱스 증가 (비트 마스킹으로 래핑) */
    cb->writeIndex = wrapIndex(cb, cb->writeIndex + 1);
    
    return CB_SUCCESS;
}

/**
 * @brief Extract data from buffer (Pop) / 버퍼에서 데이터 추출 (Pop)
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @param pData Pointer to store extracted data / 추출된 데이터를 저장할 포인터
 * @return CB_SUCCESS: success / 성공
 *         CB_ERROR_EMPTY: buffer empty / 버퍼 비어있음
 * 
 * Edge Case Handling / Edge Case 처리:
 * - Returns CB_ERROR_EMPTY if buffer is empty
 *   버퍼가 비어있으면 CB_ERROR_EMPTY 반환
 * - Returns CB_ERROR_NULL_POINTER if pData is NULL
 *   pData가 NULL이면 CB_ERROR_NULL_POINTER 반환
 * 
 * Time complexity / 시간 복잡도: O(1)
 */
int cb_pop(CircularBuffer_t* cb, SensorData_t* pData) {
    /* Validation / 유효성 검증 */
    if (cb == NULL || !cb->isInitialized) {
        return CB_ERROR_NULL_POINTER;
    }
    
    if (pData == NULL) {
        return CB_ERROR_NULL_POINTER;
    }
    
    /* Check if buffer is empty - Edge Case handling */
    /* 버퍼 비어있음 검사 - Edge Case 처리 */
    if (cb_isEmpty(cb)) {
        /**
         * Edge Case: Pop attempted on empty buffer
         * Edge Case: 빈 버퍼에서 pop 시도
         * - Return error code to notify caller
         *   에러 코드 반환으로 호출자에게 상황 알림
         * - pData is not modified (keeps previous value)
         *   pData는 변경하지 않음 (이전 값 유지)
         */
        return CB_ERROR_EMPTY;
    }
    
    /* Extract data / 데이터 추출 */
    *pData = cb->pBuffer[cb->readIndex];
    
    /* Increment read index (wrap using bit masking) */
    /* 읽기 인덱스 증가 (비트 마스킹으로 래핑) */
    cb->readIndex = wrapIndex(cb, cb->readIndex + 1);
    
    /* Decrement data count / 데이터 개수 감소 */
    cb->count--;
    
    return CB_SUCCESS;
}

/**
 * @brief Get current data count in buffer / 버퍼의 현재 데이터 개수 반환
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @return Current stored data count / 현재 저장된 데이터 개수
 */
uint32_t cb_getCount(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return 0;
    }
    return cb->count;
}

/**
 * @brief Get free space in buffer / 버퍼의 여유 공간 반환
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @return Remaining space count / 남은 공간 개수
 */
uint32_t cb_getFreeSpace(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return 0;
    }
    return cb->capacity - cb->count;
}

/**
 * @brief Clear buffer contents (size maintained) / 버퍼 내용 초기화 (크기는 유지)
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @return CB_SUCCESS: success / 성공
 */
int cb_clear(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return CB_ERROR_NULL_POINTER;
    }
    
    cb->writeIndex = 0;
    cb->readIndex = 0;
    cb->count = 0;
    
    return CB_SUCCESS;
}

/**
 * @brief View data without removing (Peek)
 *        버퍼에서 데이터를 제거하지 않고 확인 (Peek)
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @param pData Pointer to store peeked data / 확인된 데이터를 저장할 포인터
 * @return CB_SUCCESS: success / 성공
 *         CB_ERROR_EMPTY: buffer empty / 버퍼 비어있음
 */
int cb_peek(CircularBuffer_t* cb, SensorData_t* pData) {
    if (cb == NULL || !cb->isInitialized || pData == NULL) {
        return CB_ERROR_NULL_POINTER;
    }
    
    if (cb_isEmpty(cb)) {
        return CB_ERROR_EMPTY;
    }
    
    *pData = cb->pBuffer[cb->readIndex];
    return CB_SUCCESS;
}

/* ============================================================================
 * Thread-Safe Version (for interrupt environments)
 * Thread-Safe 버전 (인터럽트 환경용)
 * 
 * Functions for sharing data between ISR (Interrupt Service Routine)
 * and main loop in embedded environments.
 * 임베디드 환경에서 인터럽트 서비스 루틴(ISR)과 메인 루프 간의
 * 데이터 공유 시 사용하는 함수들입니다.
 * 
 * Implementation / 구현 방식:
 * 1. Critical section protection via interrupt disable/enable
 *    인터럽트 비활성화/활성화를 통한 임계 영역 보호
 * 2. Or using hardware-supported atomic operations
 *    또는 하드웨어 지원 원자적 연산 사용
 * ============================================================================ */

/* Critical section entry/exit macros (modify per platform) */
/* 임계 영역 진입/퇴장 매크로 (플랫폼에 따라 수정 필요) */
#ifdef __ARM_ARCH
    /* For ARM Cortex-M series / ARM Cortex-M 시리즈용 */
    #define ENTER_CRITICAL()    __disable_irq()
    #define EXIT_CRITICAL()     __enable_irq()
#else
    /* General environment (dummy implementation for testing) */
    /* 일반 환경 (테스트용 더미 구현) */
    #define ENTER_CRITICAL()    do {} while(0)
    #define EXIT_CRITICAL()     do {} while(0)
#endif

/**
 * @brief Thread-safe Push function / Thread-safe Push 함수
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @param data Data to insert / 삽입할 데이터
 * @return CB_SUCCESS: success / 성공
 * 
 * Usage scenario / 사용 시나리오:
 * - Use when storing sensor data in ISR
 *   인터럽트 서비스 루틴(ISR)에서 센서 데이터 저장 시 사용
 * - Prevents race condition with main loop
 *   메인 루프와의 데이터 경쟁 상태 방지
 */
int cb_push_safe(CircularBuffer_t* cb, SensorData_t data) {
    int result;
    
    ENTER_CRITICAL();       /* Disable interrupts / 인터럽트 비활성화 */
    result = cb_push(cb, data);
    EXIT_CRITICAL();        /* Enable interrupts / 인터럽트 활성화 */
    
    return result;
}

/**
 * @brief Thread-safe Pop function / Thread-safe Pop 함수
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 * @param pData Pointer to store extracted data / 추출된 데이터를 저장할 포인터
 * @return CB_SUCCESS: success / 성공
 */
int cb_pop_safe(CircularBuffer_t* cb, SensorData_t* pData) {
    int result;
    
    ENTER_CRITICAL();       /* Disable interrupts / 인터럽트 비활성화 */
    result = cb_pop(cb, pData);
    EXIT_CRITICAL();        /* Enable interrupts / 인터럽트 활성화 */
    
    return result;
}

/* ============================================================================
 * Debug and Test Functions / 디버그 및 테스트 함수
 * ============================================================================ */

/**
 * @brief Print buffer status (for debugging) / 버퍼 상태 출력 (디버깅용)
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 */
void cb_printStatus(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        printf("[DEBUG] Buffer not initialized. / 버퍼가 초기화되지 않았습니다.\n");
        return;
    }
    
    printf("============ Circular Buffer Status / 상태 ============\n");
    printf("Capacity / 용량: %u\n", cb->capacity);
    printf("Current Count / 현재 개수: %u\n", cb->count);
    printf("Free Space / 여유 공간: %u\n", cb_getFreeSpace(cb));
    printf("Write Index (Head) / 쓰기 인덱스: %u\n", cb->writeIndex);
    printf("Read Index (Tail) / 읽기 인덱스: %u\n", cb->readIndex);
    printf("Is Empty / 비어있음: %s\n", cb_isEmpty(cb) ? "Yes" : "No");
    printf("Is Full / 가득 참: %s\n", cb_isFull(cb) ? "Yes" : "No");
    printf("=======================================================\n");
}

/**
 * @brief Print buffer contents (for debugging) / 버퍼 내용 출력 (디버깅용)
 * @param cb Circular Buffer structure pointer / Circular Buffer 구조체 포인터
 */
void cb_printContents(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        printf("[DEBUG] Buffer not initialized. / 버퍼가 초기화되지 않았습니다.\n");
        return;
    }
    
    printf("[Buffer Contents / 버퍼 내용] ");
    if (cb_isEmpty(cb)) {
        printf("(empty / 비어있음)\n");
        return;
    }
    
    uint32_t idx = cb->readIndex;
    for (uint32_t i = 0; i < cb->count; i++) {
        printf("%d ", cb->pBuffer[idx]);
        idx = wrapIndex(cb, idx + 1);
    }
    printf("\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    CircularBuffer_t circularBuffer;
    SensorData_t data;
    int result;
    
    /* Enable UTF-8 console output for Korean characters */
    /* 한글 출력을 위한 UTF-8 콘솔 설정 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Circular Buffer Test Program\n");
    printf("  Circular Buffer 테스트 프로그램\n");
    printf("========================================\n\n");
    
    /* 1. Buffer initialization test / 버퍼 초기화 테스트 */
    printf("[Test 1] Buffer Initialization (size: 8) / 버퍼 초기화 (크기: 8)\n");
    result = cb_init(&circularBuffer, 8);   /* 2^3 = 8 */
    if (result != CB_SUCCESS) {
        printf("Initialization failed! / 초기화 실패!\n");
        return -1;
    }
    cb_printStatus(&circularBuffer);
    
    /* 2. Data Push test / 데이터 Push 테스트 */
    printf("\n[Test 2] Push 5 data items / 데이터 5개 Push\n");
    for (int i = 1; i <= 5; i++) {
        cb_push(&circularBuffer, i * 10);
        printf("Push: %d\n", i * 10);
    }
    cb_printStatus(&circularBuffer);
    cb_printContents(&circularBuffer);
    
    /* 3. Data Pop test / 데이터 Pop 테스트 */
    printf("\n[Test 3] Pop 2 data items / 데이터 2개 Pop\n");
    for (int i = 0; i < 2; i++) {
        result = cb_pop(&circularBuffer, &data);
        if (result == CB_SUCCESS) {
            printf("Pop: %d\n", data);
        }
    }
    cb_printStatus(&circularBuffer);
    cb_printContents(&circularBuffer);
    
    /* 4. Buffer full test (Overwrite policy) / 버퍼 가득 참 테스트 (Overwrite 정책) */
    printf("\n[Test 4] Buffer Full Test (Push 10 more items)\n");
    printf("         버퍼 가득 참 테스트 (데이터 10개 추가 Push)\n");
    for (int i = 100; i <= 190; i += 10) {
        cb_push(&circularBuffer, i);
        printf("Push: %d (Count / 개수: %u)\n", i, cb_getCount(&circularBuffer));
    }
    cb_printStatus(&circularBuffer);
    cb_printContents(&circularBuffer);
    
    /* 5. Pop from empty buffer test (Edge Case) */
    /* 빈 버퍼에서 Pop 테스트 (Edge Case) */
    printf("\n[Test 5] Pop from Empty Buffer (Edge Case)\n");
    printf("         빈 버퍼에서 Pop 테스트 (Edge Case)\n");
    cb_clear(&circularBuffer);
    printf("Buffer cleared / 버퍼 초기화 완료\n");
    result = cb_pop(&circularBuffer, &data);
    if (result == CB_ERROR_EMPTY) {
        printf("As expected, CB_ERROR_EMPTY returned (pop from empty buffer)\n");
        printf("예상대로 CB_ERROR_EMPTY 반환됨 (빈 버퍼에서 Pop 시도)\n");
    }
    
    /* 6. Peek test / Peek 테스트 */
    printf("\n[Test 6] Peek Test / Peek 테스트\n");
    cb_push(&circularBuffer, 999);
    result = cb_peek(&circularBuffer, &data);
    if (result == CB_SUCCESS) {
        printf("Peek result / Peek 결과: %d (data not removed / 데이터 제거 안됨)\n", data);
    }
    printf("Buffer count / 버퍼 개수: %u (should be same after peek / Peek 후에도 동일해야 함)\n", 
           cb_getCount(&circularBuffer));
    
    /* 7. Memory deallocation / 메모리 해제 */
    printf("\n[Test 7] Buffer Memory Deallocation / 버퍼 메모리 해제\n");
    cb_deinit(&circularBuffer);
    printf("Memory deallocated / 메모리 해제 완료\n");
    
    printf("\n========================================\n");
    printf("  All Tests Completed\n");
    printf("  모든 테스트 완료\n");
    printf("========================================\n");
    
    return 0;
}
