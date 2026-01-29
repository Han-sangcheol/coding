/**
 * ============================================================================
 * [코딩테스트 Level 10] 미니 Circular Buffer 통합 구현
 * ============================================================================
 * 
 * 문제 설명:
 * 지금까지 배운 모든 개념을 통합하여 완전한 Circular Buffer를 구현합니다.
 * 이 버퍼는 임베디드 시스템에서 센서 데이터를 효율적으로 저장하고 관리합니다.
 * 
 * 통합 개념:
 * - Level 1: typedef와 고정 크기 자료형
 * - Level 2: 구조체 정의
 * - Level 3: 포인터와 참조에 의한 전달
 * - Level 4: 동적 메모리 할당
 * - Level 5: 비트 연산 (2의 거듭제곱 확인)
 * - Level 6: 비트 마스킹 인덱스 래핑
 * - Level 7: 매크로와 조건부 컴파일
 * - Level 8: 큐 자료구조 (FIFO)
 * - Level 9: volatile과 임계 영역
 * 
 * 요구사항:
 * 1. init(size): 버퍼 초기화 (size는 2의 거듭제곱)
 * 2. push(data): 데이터 삽입 (가득 차면 오래된 데이터 덮어쓰기)
 * 3. pop(): 가장 오래된 데이터 추출
 * 4. isEmpty(), isFull(): 상태 확인
 * 5. Thread-safe 버전 제공
 * 
 * ============================================================================
 * File: 10_circular_buffer_mini.c
 * 파일명: 10_circular_buffer_mini.c
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
 * [Level 7: 조건부 컴파일]
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
 * Configuration and Constants / 설정 및 상수
 * [Level 7: 매크로 상수]
 * ============================================================================ */
#define DEFAULT_BUFFER_SIZE     8       /* 기본 버퍼 크기 (2^3) */

/* 버퍼 가득 참 정책 */
#define POLICY_OVERWRITE_OLD    0       /* 오래된 데이터 덮어쓰기 */
#define POLICY_REJECT_NEW       1       /* 새 데이터 거부 */
#define BUFFER_FULL_POLICY      POLICY_OVERWRITE_OLD

/* 에러 코드 */
#define CB_SUCCESS              0
#define CB_ERROR_EMPTY          -1
#define CB_ERROR_FULL           -2
#define CB_ERROR_INVALID_SIZE   -3
#define CB_ERROR_NULL           -4
#define CB_ERROR_ALLOC          -5

/* ============================================================================
 * Critical Section Macros / 임계 영역 매크로
 * [Level 9: volatile과 임계 영역]
 * ============================================================================ */
#ifdef __ARM_ARCH
    #define ENTER_CRITICAL()    __disable_irq()
    #define EXIT_CRITICAL()     __enable_irq()
#else
    #define ENTER_CRITICAL()    do {} while(0)
    #define EXIT_CRITICAL()     do {} while(0)
#endif

/* ============================================================================
 * Type Definitions / 타입 정의
 * [Level 1: typedef와 고정 크기 자료형]
 * ============================================================================ */
typedef int32_t DataType_t;         /* 버퍼에 저장할 데이터 타입 */

/* ============================================================================
 * Circular Buffer Structure / Circular Buffer 구조체
 * [Level 2: 구조체 정의] + [Level 9: volatile]
 * ============================================================================ */

/**
 * Circular Buffer Structure / Circular Buffer 구조체
 * 
 * Memory Layout / 메모리 레이아웃:
 * +---+---+---+---+---+---+---+---+
 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
 * +---+---+---+---+---+---+---+---+
 *       ^           ^
 *     read        write
 *     (tail)      (head)
 */
typedef struct {
    DataType_t*     pBuffer;        /* 데이터 버퍼 포인터 [Level 3: 포인터] */
    volatile uint32_t writeIndex;   /* 쓰기 인덱스 [Level 9: volatile] */
    volatile uint32_t readIndex;    /* 읽기 인덱스 */
    volatile uint32_t count;        /* 현재 요소 개수 */
    uint32_t        capacity;       /* 버퍼 용량 */
    uint32_t        indexMask;      /* 비트 마스크 [Level 6: 비트 마스킹] */
    bool            isInitialized;  /* 초기화 상태 */
} CircularBuffer_t;

/* ============================================================================
 * Helper Functions / 헬퍼 함수
 * [Level 5: 비트 연산]
 * ============================================================================ */

/**
 * @brief Check if n is power of two / n이 2의 거듭제곱인지 확인
 * [Level 5: n & (n-1) == 0 패턴]
 */
static inline bool isPowerOfTwo(uint32_t n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

/**
 * @brief Wrap index using bit masking / 비트 마스킹으로 인덱스 래핑
 * [Level 6: index & mask]
 */
static inline uint32_t wrapIndex(uint32_t index, uint32_t mask) {
    return index & mask;
}

/* ============================================================================
 * Circular Buffer API / Circular Buffer API 함수
 * ============================================================================ */

/**
 * @brief Initialize circular buffer / Circular Buffer 초기화
 * @param cb Pointer to buffer structure [Level 3: 포인터]
 * @param size Buffer size (must be power of 2) [Level 5: 2^n 제약]
 * @return CB_SUCCESS or error code
 */
int cb_init(CircularBuffer_t* cb, uint32_t size) {
    /* NULL 검사 [Level 3: 포인터 검증] */
    if (cb == NULL) {
        return CB_ERROR_NULL;
    }
    
    /* 크기 유효성 검사 [Level 5: 2의 거듭제곱 확인] */
    if (size == 0 || !isPowerOfTwo(size)) {
        printf("[ERROR] Size must be power of 2 (e.g., 4, 8, 16...)\n");
        return CB_ERROR_INVALID_SIZE;
    }
    
    /* 구조체 초기화 */
    cb->capacity = size;
    cb->indexMask = size - 1;       /* [Level 6: 마스크 생성] */
    cb->writeIndex = 0;
    cb->readIndex = 0;
    cb->count = 0;
    
    /* 동적 메모리 할당 [Level 4: malloc] */
    cb->pBuffer = (DataType_t*)calloc(size, sizeof(DataType_t));
    if (cb->pBuffer == NULL) {
        printf("[ERROR] Memory allocation failed\n");
        return CB_ERROR_ALLOC;
    }
    
    cb->isInitialized = true;
    printf("[INIT] Circular Buffer: size=%u, mask=0x%X\n", size, cb->indexMask);
    
    return CB_SUCCESS;
}

/**
 * @brief Deinitialize circular buffer / Circular Buffer 해제
 * [Level 4: 메모리 해제]
 */
int cb_deinit(CircularBuffer_t* cb) {
    if (cb == NULL) {
        return CB_ERROR_NULL;
    }
    
    if (cb->pBuffer != NULL) {
        free(cb->pBuffer);
        cb->pBuffer = NULL;
    }
    
    cb->isInitialized = false;
    cb->capacity = 0;
    cb->writeIndex = 0;
    cb->readIndex = 0;
    cb->count = 0;
    
    printf("[DEINIT] Buffer memory released\n");
    return CB_SUCCESS;
}

/**
 * @brief Check if buffer is empty / 버퍼가 비어있는지 확인
 * [Level 8: 큐 상태 확인]
 */
bool cb_isEmpty(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return true;
    }
    return (cb->count == 0);
}

/**
 * @brief Check if buffer is full / 버퍼가 가득 찼는지 확인
 */
bool cb_isFull(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return false;
    }
    return (cb->count == cb->capacity);
}

/**
 * @brief Push data to buffer / 버퍼에 데이터 삽입
 * [Level 8: enqueue] + [Level 6: 인덱스 래핑]
 * 
 * Buffer Full Policy / 버퍼 가득 참 정책:
 * POLICY_OVERWRITE_OLD: 가장 오래된 데이터 덮어씀 (기본값)
 * POLICY_REJECT_NEW: 새 데이터 거부
 */
int cb_push(CircularBuffer_t* cb, DataType_t data) {
    if (cb == NULL || !cb->isInitialized) {
        return CB_ERROR_NULL;
    }
    
    /* 버퍼 가득 참 처리 [Level 7: 조건부 컴파일] */
    if (cb_isFull(cb)) {
#if (BUFFER_FULL_POLICY == POLICY_OVERWRITE_OLD)
        /* 오래된 데이터 덮어쓰기: readIndex 증가 */
        cb->readIndex = wrapIndex(cb->readIndex + 1, cb->indexMask);
        /* count는 유지 (이미 capacity) */
#else
        return CB_ERROR_FULL;
#endif
    } else {
        cb->count++;
    }
    
    /* 데이터 삽입 [Level 6: 비트 마스킹 래핑] */
    cb->pBuffer[cb->writeIndex] = data;
    cb->writeIndex = wrapIndex(cb->writeIndex + 1, cb->indexMask);
    
    return CB_SUCCESS;
}

/**
 * @brief Pop data from buffer / 버퍼에서 데이터 추출
 * [Level 8: dequeue] + [Level 3: 포인터로 값 반환]
 */
int cb_pop(CircularBuffer_t* cb, DataType_t* pData) {
    if (cb == NULL || !cb->isInitialized || pData == NULL) {
        return CB_ERROR_NULL;
    }
    
    /* 빈 버퍼 검사 - Edge Case [Level 8] */
    if (cb_isEmpty(cb)) {
        return CB_ERROR_EMPTY;
    }
    
    /* 데이터 추출 */
    *pData = cb->pBuffer[cb->readIndex];
    cb->readIndex = wrapIndex(cb->readIndex + 1, cb->indexMask);
    cb->count--;
    
    return CB_SUCCESS;
}

/**
 * @brief Peek data without removing / 제거하지 않고 데이터 확인
 */
int cb_peek(CircularBuffer_t* cb, DataType_t* pData) {
    if (cb == NULL || !cb->isInitialized || pData == NULL) {
        return CB_ERROR_NULL;
    }
    
    if (cb_isEmpty(cb)) {
        return CB_ERROR_EMPTY;
    }
    
    *pData = cb->pBuffer[cb->readIndex];
    return CB_SUCCESS;
}

/**
 * @brief Get current count / 현재 데이터 개수 반환
 */
uint32_t cb_getCount(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return 0;
    }
    return cb->count;
}

/**
 * @brief Clear buffer / 버퍼 비우기
 */
int cb_clear(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        return CB_ERROR_NULL;
    }
    
    cb->writeIndex = 0;
    cb->readIndex = 0;
    cb->count = 0;
    
    return CB_SUCCESS;
}

/* ============================================================================
 * Thread-Safe API / Thread-Safe API
 * [Level 9: 임계 영역 보호]
 * ============================================================================ */

/**
 * @brief Thread-safe push / Thread-safe push
 */
int cb_push_safe(CircularBuffer_t* cb, DataType_t data) {
    int result;
    ENTER_CRITICAL();
    result = cb_push(cb, data);
    EXIT_CRITICAL();
    return result;
}

/**
 * @brief Thread-safe pop / Thread-safe pop
 */
int cb_pop_safe(CircularBuffer_t* cb, DataType_t* pData) {
    int result;
    ENTER_CRITICAL();
    result = cb_pop(cb, pData);
    EXIT_CRITICAL();
    return result;
}

/* ============================================================================
 * Debug Functions / 디버그 함수
 * ============================================================================ */

/**
 * @brief Print buffer status / 버퍼 상태 출력
 */
void cb_printStatus(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        printf("[ERROR] Buffer not initialized\n");
        return;
    }
    
    printf("┌─────────────────────────────────────┐\n");
    printf("│  Circular Buffer Status / 상태     │\n");
    printf("├─────────────────────────────────────┤\n");
    printf("│  Capacity:    %-8u              │\n", cb->capacity);
    printf("│  Count:       %-8u              │\n", cb->count);
    printf("│  Write Index: %-8u              │\n", cb->writeIndex);
    printf("│  Read Index:  %-8u              │\n", cb->readIndex);
    printf("│  Is Empty:    %-8s              │\n", cb_isEmpty(cb) ? "Yes" : "No");
    printf("│  Is Full:     %-8s              │\n", cb_isFull(cb) ? "Yes" : "No");
    printf("└─────────────────────────────────────┘\n");
}

/**
 * @brief Print buffer contents / 버퍼 내용 출력
 */
void cb_printContents(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) {
        printf("[ERROR] Buffer not initialized\n");
        return;
    }
    
    printf("Buffer Contents: ");
    if (cb_isEmpty(cb)) {
        printf("(empty)\n");
        return;
    }
    
    uint32_t idx = cb->readIndex;
    for (uint32_t i = 0; i < cb->count; i++) {
        printf("%d ", cb->pBuffer[idx]);
        idx = wrapIndex(idx + 1, cb->indexMask);
    }
    printf("\n");
}

/**
 * @brief Visualize buffer / 버퍼 시각화
 */
void cb_visualize(CircularBuffer_t* cb) {
    if (cb == NULL || !cb->isInitialized) return;
    
    printf("\n[Visual] ");
    for (uint32_t i = 0; i < cb->capacity; i++) {
        /* 유효한 데이터 범위인지 확인 */
        bool hasData = false;
        uint32_t idx = cb->readIndex;
        for (uint32_t j = 0; j < cb->count; j++) {
            if (idx == i) {
                hasData = true;
                break;
            }
            idx = wrapIndex(idx + 1, cb->indexMask);
        }
        
        if (hasData) {
            printf("[%3d]", cb->pBuffer[i]);
        } else {
            printf("[   ]");
        }
    }
    printf("\n         ");
    for (uint32_t i = 0; i < cb->capacity; i++) {
        if (i == cb->readIndex && i == cb->writeIndex) {
            printf(" R,W ");
        } else if (i == cb->readIndex) {
            printf("  R  ");
        } else if (i == cb->writeIndex) {
            printf("  W  ");
        } else {
            printf("     ");
        }
    }
    printf("\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 10: Mini Circular Buffer\n");
    printf("  Level 10: 미니 Circular Buffer 통합\n");
    printf("========================================\n\n");
    
    printf("이 구현에 사용된 개념들:\n");
    printf("  [Level 1] typedef, 고정 크기 자료형\n");
    printf("  [Level 2] 구조체 정의\n");
    printf("  [Level 3] 포인터, 참조 전달\n");
    printf("  [Level 4] 동적 메모리 할당\n");
    printf("  [Level 5] 비트 연산 (2의 거듭제곱)\n");
    printf("  [Level 6] 비트 마스킹 인덱스 래핑\n");
    printf("  [Level 7] 매크로, 조건부 컴파일\n");
    printf("  [Level 8] FIFO 큐 자료구조\n");
    printf("  [Level 9] volatile, 임계 영역\n\n");
    
    CircularBuffer_t buffer;
    DataType_t data;
    int result;
    
    /* Test 1: 초기화 */
    printf("[Test 1] Initialize (size=8)\n");
    result = cb_init(&buffer, 8);
    if (result != CB_SUCCESS) {
        printf("Init failed!\n");
        return -1;
    }
    cb_printStatus(&buffer);
    cb_visualize(&buffer);
    
    /* Test 2: Push 데이터 */
    printf("\n[Test 2] Push 5 items\n");
    for (int i = 1; i <= 5; i++) {
        cb_push(&buffer, i * 10);
        printf("  Push: %d\n", i * 10);
    }
    cb_printStatus(&buffer);
    cb_printContents(&buffer);
    cb_visualize(&buffer);
    
    /* Test 3: Pop 데이터 */
    printf("\n[Test 3] Pop 2 items\n");
    for (int i = 0; i < 2; i++) {
        result = cb_pop(&buffer, &data);
        if (result == CB_SUCCESS) {
            printf("  Pop: %d\n", data);
        }
    }
    cb_printStatus(&buffer);
    cb_printContents(&buffer);
    cb_visualize(&buffer);
    
    /* Test 4: 버퍼 가득 참 (Overwrite 정책) */
    printf("\n[Test 4] Buffer Full - Overwrite Policy\n");
    printf("  Adding 8 more items to trigger overwrite...\n");
    for (int i = 100; i <= 170; i += 10) {
        cb_push(&buffer, i);
        printf("  Push: %d (count=%u)\n", i, cb_getCount(&buffer));
    }
    cb_printStatus(&buffer);
    cb_printContents(&buffer);
    cb_visualize(&buffer);
    
    /* Test 5: 빈 버퍼에서 Pop (Edge Case) */
    printf("\n[Test 5] Pop from Empty Buffer (Edge Case)\n");
    cb_clear(&buffer);
    printf("  Buffer cleared\n");
    result = cb_pop(&buffer, &data);
    if (result == CB_ERROR_EMPTY) {
        printf("  Result: CB_ERROR_EMPTY (expected!)\n");
    }
    
    /* Test 6: Peek */
    printf("\n[Test 6] Peek Test\n");
    cb_push(&buffer, 999);
    result = cb_peek(&buffer, &data);
    if (result == CB_SUCCESS) {
        printf("  Peek: %d (not removed)\n", data);
        printf("  Count after peek: %u\n", cb_getCount(&buffer));
    }
    
    /* Test 7: 메모리 해제 */
    printf("\n[Test 7] Deinitialize\n");
    cb_deinit(&buffer);
    
    printf("\n========================================\n");
    printf("  Congratulations!\n");
    printf("  축하합니다!\n");
    printf("========================================\n");
    printf("\n모든 10개 레벨을 완료했습니다!\n");
    printf("이제 고급 Circular Buffer를 구현할 준비가 되었습니다.\n");
    printf("\n다음 단계: circular_buffer.c 분석 및 확장\n");
    
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
 * [Test 1] 버퍼 초기화 성공 (size=8, mask=0x7)
 * [Test 2] 5개 데이터 Push 후 상태 출력
 * [Test 3] 2개 Pop 후 상태 출력
 * [Test 4] Overwrite 정책으로 오래된 데이터 덮어쓰기
 * [Test 5] 빈 버퍼에서 Pop 시 CB_ERROR_EMPTY
 * [Test 6] Peek 후 데이터 유지 확인
 * [Test 7] 메모리 정상 해제
 * 
 * ============================================================================
 */
