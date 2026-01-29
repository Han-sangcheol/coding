/**
 * ============================================================================
 * [코딩테스트 Level 5] 비트 연산 기초 - 2의 거듭제곱 확인
 * ============================================================================
 * 
 * 문제 설명:
 * 비트 연산은 하드웨어에서 직접 지원되어 매우 빠른 연산입니다.
 * Circular Buffer에서 인덱스 래핑을 최적화하려면 버퍼 크기가 2의 거듭제곱이어야 합니다.
 * 이를 위해 주어진 숫자가 2의 거듭제곱인지 확인하는 함수가 필요합니다.
 * 
 * 요구사항:
 * 1. 비트 연산자 (&, |, ^, ~, <<, >>)를 이해하세요.
 * 2. n & (n-1) == 0 패턴으로 2의 거듭제곱을 확인하세요.
 * 3. 비트 시프트로 2의 거듭제곱을 생성하세요.
 * 4. 특정 비트를 설정/해제/토글하는 함수를 작성하세요.
 * 
 * 학습 포인트:
 * - 비트 연산자 6가지
 * - 2의 거듭제곱 판별 알고리즘
 * - 비트 마스크 생성 및 활용
 * - 비트 연산의 성능 이점
 * 
 * ============================================================================
 * File: 05_bitwise_power_of_two.c
 * 파일명: 05_bitwise_power_of_two.c
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
 * Helper Function: Print Binary / 이진수 출력 헬퍼 함수
 * ============================================================================ */

/**
 * @brief Print number in binary format / 숫자를 이진수로 출력
 * @param value Value to print / 출력할 값
 * @param bits Number of bits to display / 표시할 비트 수
 */
void printBinary(uint32_t value, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%c", (value & (1U << i)) ? '1' : '0');
        if (i > 0 && i % 4 == 0) printf(" ");   /* 4비트마다 공백 */
    }
}

/* ============================================================================
 * Bitwise Operators Demonstration / 비트 연산자 시연
 * ============================================================================ */

/**
 * @brief Demonstrate all bitwise operators / 모든 비트 연산자 시연
 * 
 * 비트 연산자 6가지:
 * 1. & (AND): 둘 다 1이면 1
 * 2. | (OR): 하나라도 1이면 1
 * 3. ^ (XOR): 서로 다르면 1
 * 4. ~ (NOT): 비트 반전
 * 5. << (Left Shift): 왼쪽으로 이동
 * 6. >> (Right Shift): 오른쪽으로 이동
 */
void demonstrateBitwiseOperators(void) {
    printf("============ Bitwise Operators / 비트 연산자 ============\n\n");
    
    uint8_t a = 0b11001010;     /* 202 in decimal */
    uint8_t b = 0b10110011;     /* 179 in decimal */
    
    printf("a = ");
    printBinary(a, 8);
    printf(" (%u)\n", a);
    
    printf("b = ");
    printBinary(b, 8);
    printf(" (%u)\n\n", b);
    
    /* 1. AND (&) */
    printf("1. AND (&) - 둘 다 1이면 1:\n");
    printf("   a & b = ");
    printBinary(a & b, 8);
    printf(" (%u)\n\n", a & b);
    
    /* 2. OR (|) */
    printf("2. OR (|) - 하나라도 1이면 1:\n");
    printf("   a | b = ");
    printBinary(a | b, 8);
    printf(" (%u)\n\n", a | b);
    
    /* 3. XOR (^) */
    printf("3. XOR (^) - 서로 다르면 1:\n");
    printf("   a ^ b = ");
    printBinary(a ^ b, 8);
    printf(" (%u)\n\n", a ^ b);
    
    /* 4. NOT (~) */
    printf("4. NOT (~) - 비트 반전:\n");
    printf("   ~a = ");
    printBinary((uint8_t)~a, 8);
    printf(" (%u)\n\n", (uint8_t)~a);
    
    /* 5. Left Shift (<<) */
    printf("5. Left Shift (<<) - 왼쪽으로 이동 (x2 효과):\n");
    printf("   a << 1 = ");
    printBinary((uint8_t)(a << 1), 8);
    printf(" (%u) [a*2]\n", (uint8_t)(a << 1));
    printf("   a << 2 = ");
    printBinary((uint8_t)(a << 2), 8);
    printf(" (%u) [a*4]\n\n", (uint8_t)(a << 2));
    
    /* 6. Right Shift (>>) */
    printf("6. Right Shift (>>) - 오른쪽으로 이동 (/2 효과):\n");
    printf("   a >> 1 = ");
    printBinary(a >> 1, 8);
    printf(" (%u) [a/2]\n", a >> 1);
    printf("   a >> 2 = ");
    printBinary(a >> 2, 8);
    printf(" (%u) [a/4]\n", a >> 2);
    
    printf("==========================================================\n");
}

/* ============================================================================
 * Power of Two Functions / 2의 거듭제곱 관련 함수
 * ============================================================================ */

/**
 * @brief Check if number is power of two / 2의 거듭제곱인지 확인
 * @param n Number to check / 확인할 숫자
 * @return true if power of two / 2의 거듭제곱이면 true
 * 
 * 알고리즘 설명:
 * 2의 거듭제곱은 이진수에서 1이 하나만 있음:
 *   1 = 0001
 *   2 = 0010
 *   4 = 0100
 *   8 = 1000
 * 
 * n-1은 그 1 아래의 모든 비트가 1이 됨:
 *   8   = 1000
 *   8-1 = 0111
 *   8 & 7 = 0000  -> 결과가 0이면 2의 거듭제곱!
 * 
 * 반례 (6은 2의 거듭제곱이 아님):
 *   6   = 0110
 *   6-1 = 0101
 *   6 & 5 = 0100  -> 결과가 0이 아님!
 */
bool isPowerOfTwo(uint32_t n) {
    /* n이 0이면 false (0은 2의 거듭제곱이 아님) */
    /* n == 0 returns false (0 is not power of two) */
    if (n == 0) {
        return false;
    }
    
    /* n & (n-1) == 0 이면 2의 거듭제곱 */
    return (n & (n - 1)) == 0;
}

/**
 * @brief Demonstrate power of two check / 2의 거듭제곱 확인 시연
 */
void demonstratePowerOfTwo(void) {
    printf("\n============ Power of Two Check / 2의 거듭제곱 확인 ============\n\n");
    
    printf("알고리즘: n & (n-1) == 0\n\n");
    
    /* 상세 설명 */
    uint32_t test1 = 8;
    printf("예시 1: n = %u\n", test1);
    printf("   n   = ");
    printBinary(test1, 8);
    printf("\n");
    printf("   n-1 = ");
    printBinary(test1 - 1, 8);
    printf("\n");
    printf("   n & (n-1) = ");
    printBinary(test1 & (test1 - 1), 8);
    printf(" = 0 -> 2의 거듭제곱!\n\n");
    
    uint32_t test2 = 6;
    printf("예시 2: n = %u\n", test2);
    printf("   n   = ");
    printBinary(test2, 8);
    printf("\n");
    printf("   n-1 = ");
    printBinary(test2 - 1, 8);
    printf("\n");
    printf("   n & (n-1) = ");
    printBinary(test2 & (test2 - 1), 8);
    printf(" != 0 -> 2의 거듭제곱 아님!\n\n");
    
    /* 여러 숫자 테스트 */
    printf("테스트 결과:\n");
    printf("%-8s %-12s %-12s\n", "Number", "Binary", "Is Power of 2");
    printf("----------------------------------------\n");
    
    uint32_t testNumbers[] = {0, 1, 2, 3, 4, 5, 8, 10, 16, 32, 64, 100, 128, 256};
    size_t count = sizeof(testNumbers) / sizeof(testNumbers[0]);
    
    for (size_t i = 0; i < count; i++) {
        printf("%-8u ", testNumbers[i]);
        printBinary(testNumbers[i], 8);
        printf("     %-5s\n", isPowerOfTwo(testNumbers[i]) ? "Yes" : "No");
    }
    
    printf("================================================================\n");
}

/**
 * @brief Generate power of two using bit shift / 비트 시프트로 2의 거듭제곱 생성
 * @param exponent Exponent (0 to 31) / 지수 (0~31)
 * @return 2^exponent
 * 
 * 1 << n = 2^n
 * 예: 1 << 3 = 8 (1000 in binary)
 */
uint32_t powerOfTwo(uint32_t exponent) {
    if (exponent >= 32) {
        return 0;   /* Overflow protection */
    }
    return 1U << exponent;
}

/**
 * @brief Find next power of two / 다음 2의 거듭제곱 찾기
 * @param n Input number / 입력 숫자
 * @return Next power of two >= n / n 이상의 가장 작은 2의 거듭제곱
 * 
 * 예: 5 -> 8, 9 -> 16, 16 -> 16
 */
uint32_t nextPowerOfTwo(uint32_t n) {
    if (n == 0) return 1;
    if (isPowerOfTwo(n)) return n;
    
    /* 모든 비트를 1로 채운 후 +1 */
    /* Fill all bits with 1, then add 1 */
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    
    return n;
}

/* ============================================================================
 * Bit Manipulation Functions / 비트 조작 함수
 * ============================================================================ */

/**
 * @brief Set a specific bit / 특정 비트 설정 (1로)
 * @param value Original value / 원본 값
 * @param bit Bit position (0-based) / 비트 위치 (0부터 시작)
 * @return Value with bit set / 비트가 설정된 값
 * 
 * 사용: value | (1 << bit)
 */
uint32_t setBit(uint32_t value, uint8_t bit) {
    return value | (1U << bit);
}

/**
 * @brief Clear a specific bit / 특정 비트 해제 (0으로)
 * @param value Original value / 원본 값
 * @param bit Bit position / 비트 위치
 * @return Value with bit cleared / 비트가 해제된 값
 * 
 * 사용: value & ~(1 << bit)
 */
uint32_t clearBit(uint32_t value, uint8_t bit) {
    return value & ~(1U << bit);
}

/**
 * @brief Toggle a specific bit / 특정 비트 토글 (반전)
 * @param value Original value / 원본 값
 * @param bit Bit position / 비트 위치
 * @return Value with bit toggled / 비트가 토글된 값
 * 
 * 사용: value ^ (1 << bit)
 */
uint32_t toggleBit(uint32_t value, uint8_t bit) {
    return value ^ (1U << bit);
}

/**
 * @brief Check if a specific bit is set / 특정 비트가 설정되어 있는지 확인
 * @param value Value to check / 확인할 값
 * @param bit Bit position / 비트 위치
 * @return true if bit is set / 비트가 설정되어 있으면 true
 * 
 * 사용: (value & (1 << bit)) != 0
 */
bool isBitSet(uint32_t value, uint8_t bit) {
    return (value & (1U << bit)) != 0;
}

/**
 * @brief Demonstrate bit manipulation / 비트 조작 시연
 */
void demonstrateBitManipulation(void) {
    printf("\n============ Bit Manipulation / 비트 조작 ============\n\n");
    
    uint8_t value = 0b00001010;  /* 10 in decimal */
    
    printf("초기값: ");
    printBinary(value, 8);
    printf(" (%u)\n\n", value);
    
    /* Set bit 0 */
    printf("setBit(value, 0) - 비트 0 설정:\n");
    printf("  결과: ");
    uint8_t result = (uint8_t)setBit(value, 0);
    printBinary(result, 8);
    printf(" (%u)\n\n", result);
    
    /* Clear bit 1 */
    printf("clearBit(value, 1) - 비트 1 해제:\n");
    printf("  결과: ");
    result = (uint8_t)clearBit(value, 1);
    printBinary(result, 8);
    printf(" (%u)\n\n", result);
    
    /* Toggle bit 3 */
    printf("toggleBit(value, 3) - 비트 3 토글:\n");
    printf("  결과: ");
    result = (uint8_t)toggleBit(value, 3);
    printBinary(result, 8);
    printf(" (%u)\n\n", result);
    
    /* Check bits */
    printf("isBitSet() - 비트 확인:\n");
    for (int i = 0; i < 8; i++) {
        printf("  비트 %d: %s\n", i, isBitSet(value, i) ? "1 (Set)" : "0 (Clear)");
    }
    
    printf("========================================================\n");
}

/**
 * @brief Create bit mask / 비트 마스크 생성
 * @param bits Number of bits / 비트 수
 * @return Bit mask / 비트 마스크
 * 
 * 예: createMask(3) = 0b111 = 7
 *     createMask(4) = 0b1111 = 15
 * 
 * Circular Buffer에서 사용:
 *   index & mask 로 index % size 대체 가능 (size가 2^n일 때)
 */
uint32_t createMask(uint8_t bits) {
    if (bits >= 32) return 0xFFFFFFFF;
    return (1U << bits) - 1;
}

/**
 * @brief Demonstrate mask creation / 마스크 생성 시연
 */
void demonstrateMaskCreation(void) {
    printf("\n============ Bit Mask Creation / 비트 마스크 생성 ============\n\n");
    
    printf("공식: mask = (1 << n) - 1  (n비트 마스크)\n\n");
    
    printf("%-8s %-12s %-8s\n", "Bits", "Binary", "Decimal");
    printf("-----------------------------\n");
    
    for (int i = 1; i <= 8; i++) {
        uint32_t mask = createMask(i);
        printf("%-8d ", i);
        printBinary(mask, 8);
        printf(" %-8u\n", mask);
    }
    
    printf("\nCircular Buffer에서의 활용:\n");
    printf("  버퍼 크기가 2^n (예: 8)일 때:\n");
    printf("  mask = size - 1 = 7 (0111)\n");
    printf("  index %% 8  ==  index & 7\n");
    printf("  비트 AND가 나눗셈보다 훨씬 빠름!\n");
    
    printf("==============================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 5: Bitwise Operations\n");
    printf("  Level 5: 비트 연산 기초\n");
    printf("========================================\n\n");
    
    /* Test 1: 비트 연산자 */
    printf("[Test 1] Bitwise Operators / 비트 연산자\n");
    demonstrateBitwiseOperators();
    
    /* Test 2: 2의 거듭제곱 확인 */
    printf("\n[Test 2] Power of Two Check / 2의 거듭제곱 확인\n");
    demonstratePowerOfTwo();
    
    /* Test 3: 2의 거듭제곱 생성 */
    printf("\n[Test 3] Generate Power of Two / 2의 거듭제곱 생성\n");
    printf("1 << n = 2^n:\n");
    for (int i = 0; i <= 8; i++) {
        printf("  1 << %d = %u\n", i, powerOfTwo(i));
    }
    
    /* Test 4: 다음 2의 거듭제곱 */
    printf("\n[Test 4] Next Power of Two / 다음 2의 거듭제곱\n");
    uint32_t testNums[] = {0, 1, 2, 3, 5, 7, 9, 15, 17, 100};
    size_t count = sizeof(testNums) / sizeof(testNums[0]);
    
    for (size_t i = 0; i < count; i++) {
        printf("  nextPowerOfTwo(%u) = %u\n", testNums[i], nextPowerOfTwo(testNums[i]));
    }
    
    /* Test 5: 비트 조작 */
    printf("\n[Test 5] Bit Manipulation / 비트 조작\n");
    demonstrateBitManipulation();
    
    /* Test 6: 마스크 생성 */
    printf("\n[Test 6] Bit Mask Creation / 비트 마스크 생성\n");
    demonstrateMaskCreation();
    
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
 * [Test 2] 2의 거듭제곱: 1, 2, 4, 8, 16, 32, 64, 128, 256 = Yes
 * [Test 4] nextPowerOfTwo(5) = 8, nextPowerOfTwo(9) = 16
 * [Test 6] 마스크: size=8일 때 mask=7 (0111)
 * 
 * ============================================================================
 */
