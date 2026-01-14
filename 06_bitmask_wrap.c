/**
 * ============================================================================
 * [코딩테스트 Level 6] 비트 마스킹을 이용한 인덱스 래핑
 * ============================================================================
 * 
 * 문제 설명:
 * Circular Buffer에서 인덱스가 버퍼 끝에 도달하면 처음으로 돌아가야 합니다.
 * 일반적으로 % (modulo) 연산을 사용하지만, 나눗셈은 느린 연산입니다.
 * 버퍼 크기가 2의 거듭제곱이면 비트 AND 연산으로 훨씬 빠르게 처리할 수 있습니다.
 * 
 * 요구사항:
 * 1. index % size와 index & mask가 동일한 결과를 내는 조건을 이해하세요.
 * 2. 모듈로 연산과 비트 마스킹의 성능을 비교하세요.
 * 3. 인덱스 래핑 함수를 비트 마스킹으로 구현하세요.
 * 4. 인덱스 오버플로우를 안전하게 처리하세요.
 * 
 * 학습 포인트:
 * - index & mask == index % size (size가 2^n일 때)
 * - 비트 마스킹이 모듈로보다 빠른 이유
 * - Circular Buffer 인덱스 관리
 * - 인덱스 오버플로우 방지
 * 
 * ============================================================================
 * File: 06_bitmask_wrap.c
 * 파일명: 06_bitmask_wrap.c
 * Author / 작성자: [Developer]
 * Date / 작성일: 2026-01-14
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>       /* clock() for performance measurement */

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
 * Constants / 상수 정의
 * ============================================================================ */
#define BUFFER_SIZE_BITS    4           /* 2^4 = 16 */
#define BUFFER_SIZE         (1 << BUFFER_SIZE_BITS)
#define BUFFER_MASK         (BUFFER_SIZE - 1)

#define PERF_ITERATIONS     10000000    /* 성능 테스트 반복 횟수 */

/* ============================================================================
 * Helper Functions / 헬퍼 함수
 * ============================================================================ */

/**
 * @brief Print number in binary format / 숫자를 이진수로 출력
 */
void printBinary(uint32_t value, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%c", (value & (1U << i)) ? '1' : '0');
        if (i > 0 && i % 4 == 0) printf(" ");
    }
}

/* ============================================================================
 * Index Wrapping Methods / 인덱스 래핑 방법
 * ============================================================================ */

/**
 * @brief Wrap index using modulo operator / 모듈로 연산으로 인덱스 래핑
 * @param index Current index / 현재 인덱스
 * @param size Buffer size / 버퍼 크기
 * @return Wrapped index / 래핑된 인덱스
 * 
 * 단점: 나눗셈은 CPU에서 가장 느린 연산 중 하나
 * Disadvantage: Division is one of the slowest CPU operations
 */
static inline uint32_t wrapIndexModulo(uint32_t index, uint32_t size) {
    return index % size;
}

/**
 * @brief Wrap index using bit masking / 비트 마스킹으로 인덱스 래핑
 * @param index Current index / 현재 인덱스
 * @param mask Bit mask (size - 1) / 비트 마스크 (size - 1)
 * @return Wrapped index / 래핑된 인덱스
 * 
 * 조건: size must be power of 2 / size는 2의 거듭제곱이어야 함
 * 장점: 비트 AND는 CPU에서 1 사이클에 실행
 * Advantage: Bit AND executes in 1 CPU cycle
 */
static inline uint32_t wrapIndexMask(uint32_t index, uint32_t mask) {
    return index & mask;
}

/* ============================================================================
 * Demonstration Functions / 시연 함수
 * ============================================================================ */

/**
 * @brief Demonstrate why bit masking works / 비트 마스킹이 작동하는 이유 시연
 * 
 * 핵심 원리:
 * size = 2^n 일 때, size - 1은 하위 n비트가 모두 1인 마스크가 됨
 * 
 * 예: size = 8 (2^3)
 *     mask = 7 = 0111
 *     
 *     index = 10 = 1010
 *     10 % 8 = 2
 *     10 & 7 = 1010 & 0111 = 0010 = 2  ✓ 같은 결과!
 */
void demonstrateWhyItWorks(void) {
    printf("============ Why Bit Masking Works / 비트 마스킹 원리 ============\n\n");
    
    uint32_t size = 8;      /* 2^3 */
    uint32_t mask = size - 1;
    
    printf("버퍼 크기: size = %u (2^3)\n", size);
    printf("마스크:    mask = size - 1 = %u\n\n", mask);
    
    printf("mask의 이진수: ");
    printBinary(mask, 8);
    printf("\n");
    printf("-> 하위 3비트가 모두 1 (111)\n\n");
    
    printf("비교 테이블:\n");
    printf("%-8s %-12s %-8s %-12s %-8s %-8s\n", 
           "Index", "Binary", "% 8", "& 7 Binary", "& 7", "Match");
    printf("--------------------------------------------------------\n");
    
    for (uint32_t i = 0; i <= 20; i++) {
        uint32_t modResult = i % size;
        uint32_t andResult = i & mask;
        
        printf("%-8u ", i);
        printBinary(i, 8);
        printf(" %-8u ", modResult);
        printBinary(andResult, 8);
        printf(" %-8u %-8s\n", andResult, modResult == andResult ? "Yes" : "No");
    }
    
    printf("\n결론: size가 2의 거듭제곱일 때, index %% size == index & (size-1)\n");
    printf("=================================================================\n");
}

/**
 * @brief Demonstrate circular wrapping visualization
 *        순환 래핑 시각화
 */
void demonstrateCircularWrapping(void) {
    printf("\n============ Circular Wrapping Visualization ============\n\n");
    
    uint32_t size = 8;
    uint32_t mask = size - 1;
    
    printf("버퍼 크기: %u, 인덱스 범위: 0 ~ %u\n\n", size, size - 1);
    
    /* 버퍼 시각화 */
    printf("Buffer Layout / 버퍼 레이아웃:\n");
    printf("┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
    printf("│ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │\n");
    printf("└───┴───┴───┴───┴───┴───┴───┴───┘\n\n");
    
    printf("인덱스 증가 시뮬레이션 (writeIndex):\n");
    printf("-----------------------------------\n");
    
    uint32_t writeIndex = 0;
    for (int i = 0; i < 16; i++) {
        printf("Step %2d: writeIndex = %u -> ", i, writeIndex);
        
        /* 버퍼 위치 표시 */
        printf("[");
        for (uint32_t j = 0; j < size; j++) {
            if (j == writeIndex) {
                printf(" W ");  /* Write position */
            } else {
                printf(" . ");
            }
        }
        printf("]\n");
        
        /* 인덱스 증가 및 래핑 */
        writeIndex = wrapIndexMask(writeIndex + 1, mask);
    }
    
    printf("\n-> 인덱스가 7을 넘으면 자동으로 0으로 돌아감 (Circular)\n");
    printf("=========================================================\n");
}

/**
 * @brief Demonstrate read/write index management
 *        읽기/쓰기 인덱스 관리 시연
 */
void demonstrateReadWriteIndex(void) {
    printf("\n============ Read/Write Index Management ============\n\n");
    
    uint32_t size = 8;
    uint32_t mask = size - 1;
    
    uint32_t writeIndex = 0;
    uint32_t readIndex = 0;
    
    int32_t buffer[8] = {0};
    
    printf("Circular Buffer 시뮬레이션:\n");
    printf("--------------------------------------------------\n");
    
    /* Push 5개 */
    printf("\n[PUSH 5 items]\n");
    for (int i = 1; i <= 5; i++) {
        buffer[writeIndex] = i * 10;
        printf("  Push %d at index %u\n", i * 10, writeIndex);
        writeIndex = wrapIndexMask(writeIndex + 1, mask);
    }
    printf("  writeIndex = %u, readIndex = %u\n", writeIndex, readIndex);
    
    /* Pop 3개 */
    printf("\n[POP 3 items]\n");
    for (int i = 0; i < 3; i++) {
        printf("  Pop %d from index %u\n", buffer[readIndex], readIndex);
        readIndex = wrapIndexMask(readIndex + 1, mask);
    }
    printf("  writeIndex = %u, readIndex = %u\n", writeIndex, readIndex);
    
    /* Push 6개 더 (래핑 발생) */
    printf("\n[PUSH 6 more items - wrapping occurs]\n");
    for (int i = 6; i <= 11; i++) {
        buffer[writeIndex] = i * 10;
        printf("  Push %d at index %u", i * 10, writeIndex);
        if (writeIndex < wrapIndexMask(writeIndex + 1, mask)) {
            printf("\n");
        } else {
            printf(" <- WRAPPED!\n");
        }
        writeIndex = wrapIndexMask(writeIndex + 1, mask);
    }
    printf("  writeIndex = %u, readIndex = %u\n", writeIndex, readIndex);
    
    /* 현재 버퍼 상태 */
    printf("\nCurrent buffer state / 현재 버퍼 상태:\n");
    printf("Index: ");
    for (uint32_t i = 0; i < size; i++) printf("%4u ", i);
    printf("\nValue: ");
    for (uint32_t i = 0; i < size; i++) printf("%4d ", buffer[i]);
    printf("\n");
    
    printf("=====================================================\n");
}

/**
 * @brief Performance comparison: Modulo vs Bit Masking
 *        성능 비교: 모듈로 vs 비트 마스킹
 */
void performanceComparison(void) {
    printf("\n============ Performance Comparison / 성능 비교 ============\n\n");
    
    uint32_t size = BUFFER_SIZE;
    uint32_t mask = BUFFER_MASK;
    
    volatile uint32_t result = 0;   /* volatile: 컴파일러 최적화 방지 */
    clock_t start, end;
    double moduloTime, maskTime;
    
    printf("테스트 설정:\n");
    printf("  Buffer size: %u\n", size);
    printf("  Iterations: %d\n", PERF_ITERATIONS);
    printf("  (volatile 사용으로 최적화 방지)\n\n");
    
    /* Modulo 연산 테스트 */
    printf("Testing modulo (index %% size)...\n");
    start = clock();
    for (int i = 0; i < PERF_ITERATIONS; i++) {
        result = wrapIndexModulo(i, size);
    }
    end = clock();
    moduloTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    printf("  Modulo time: %.2f ms\n", moduloTime);
    
    /* Bit masking 테스트 */
    printf("Testing bit mask (index & mask)...\n");
    start = clock();
    for (int i = 0; i < PERF_ITERATIONS; i++) {
        result = wrapIndexMask(i, mask);
    }
    end = clock();
    maskTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    printf("  Bit mask time: %.2f ms\n", maskTime);
    
    /* 결과 비교 */
    printf("\n결과 비교:\n");
    printf("  Modulo:   %.2f ms\n", moduloTime);
    printf("  Bit Mask: %.2f ms\n", maskTime);
    if (maskTime > 0 && moduloTime > 0) {
        printf("  Speed up: %.1fx faster\n", moduloTime / maskTime);
    }
    
    printf("\n결론:\n");
    printf("  - 비트 마스킹이 모듈로보다 빠름\n");
    printf("  - 초당 수천 번 호출되는 인터럽트 환경에서 중요\n");
    printf("  - 단, 버퍼 크기가 2의 거듭제곱이어야 함\n");
    
    /* result 사용 (컴파일러 경고 방지) */
    (void)result;
    
    printf("=============================================================\n");
}

/**
 * @brief Demonstrate index overflow prevention
 *        인덱스 오버플로우 방지 시연
 */
void demonstrateOverflowPrevention(void) {
    printf("\n============ Index Overflow Prevention / 오버플로우 방지 ============\n\n");
    
    uint32_t mask = BUFFER_MASK;
    
    printf("문제 상황: 인덱스가 계속 증가하면 uint32_t 오버플로우 발생 가능\n\n");
    
    /* 매우 큰 인덱스 테스트 */
    uint32_t largeIndex = 0xFFFFFFFF;   /* uint32_t 최대값 */
    
    printf("테스트: 최대 uint32_t 값 근처에서의 래핑\n");
    printf("  largeIndex = %u (0x%08X)\n", largeIndex, largeIndex);
    printf("  mask = %u\n\n", mask);
    
    for (int i = 0; i < 5; i++) {
        uint32_t wrapped = wrapIndexMask(largeIndex, mask);
        printf("  wrapIndexMask(%u) = %u\n", largeIndex, wrapped);
        largeIndex++;   /* 오버플로우 발생 (0으로 돌아감) */
    }
    
    printf("\n결과: 비트 마스킹은 오버플로우와 무관하게 항상 유효한 인덱스 반환\n");
    printf("  -> 0 ~ (size-1) 범위 보장\n");
    
    printf("====================================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 6: Bit Mask Index Wrapping\n");
    printf("  Level 6: 비트 마스킹 인덱스 래핑\n");
    printf("========================================\n\n");
    
    /* Test 1: 비트 마스킹 원리 */
    printf("[Test 1] Why Bit Masking Works / 비트 마스킹 원리\n");
    demonstrateWhyItWorks();
    
    /* Test 2: 순환 래핑 시각화 */
    printf("\n[Test 2] Circular Wrapping Visualization\n");
    demonstrateCircularWrapping();
    
    /* Test 3: Read/Write 인덱스 관리 */
    printf("\n[Test 3] Read/Write Index Management\n");
    demonstrateReadWriteIndex();
    
    /* Test 4: 성능 비교 */
    printf("\n[Test 4] Performance Comparison\n");
    performanceComparison();
    
    /* Test 5: 오버플로우 방지 */
    printf("\n[Test 5] Index Overflow Prevention\n");
    demonstrateOverflowPrevention();
    
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
 * [Test 1] index % 8 == index & 7 임을 확인
 * [Test 2] 인덱스 0~7 순환 확인
 * [Test 4] 비트 마스킹이 모듈로보다 빠름 확인
 * [Test 5] 오버플로우에도 유효한 인덱스 반환 확인
 * 
 * ============================================================================
 */
