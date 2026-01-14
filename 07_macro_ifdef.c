/**
 * ============================================================================
 * [코딩테스트 Level 7] 매크로와 조건부 컴파일
 * ============================================================================
 * 
 * 문제 설명:
 * 전처리기 매크로는 컴파일 전에 코드를 변환합니다.
 * 조건부 컴파일을 사용하면 플랫폼별, 빌드 설정별로 다른 코드를 포함할 수 있습니다.
 * 임베디드 시스템에서 디버그/릴리즈 빌드, 플랫폼 분기에 필수적입니다.
 * 
 * 요구사항:
 * 1. #define으로 상수와 함수형 매크로를 정의하세요.
 * 2. #ifdef, #ifndef, #else, #endif로 조건부 컴파일을 구현하세요.
 * 3. 플랫폼별 코드 분기 (Windows/Linux/ARM)를 구현하세요.
 * 4. DEBUG 모드에서만 실행되는 코드를 작성하세요.
 * 
 * 학습 포인트:
 * - #define 상수 및 함수형 매크로
 * - #ifdef, #ifndef, #else, #endif, #elif
 * - 미리 정의된 매크로 (_WIN32, __linux__, __ARM_ARCH 등)
 * - 매크로의 장단점과 주의사항
 * 
 * ============================================================================
 * File: 07_macro_ifdef.c
 * 파일명: 07_macro_ifdef.c
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
 * 1. Object-like Macros (상수 매크로)
 * ============================================================================ */

/* 단순 상수 정의 */
/* Simple constant definitions */
#define BUFFER_SIZE         256
#define MAX_SENSORS         8
#define DEFAULT_TIMEOUT_MS  1000
#define PI                  3.14159265359

/* 문자열 상수 */
/* String constants */
#define VERSION_STRING      "1.0.0"
#define DEVICE_NAME         "SensorHub"

/* 에러 코드 상수 */
/* Error code constants */
#define ERR_SUCCESS         0
#define ERR_FAILURE         -1
#define ERR_TIMEOUT         -2
#define ERR_INVALID_PARAM   -3

/* ============================================================================
 * 2. Function-like Macros (함수형 매크로)
 * ============================================================================ */

/**
 * 기본 함수형 매크로
 * Basic function-like macros
 * 
 * 주의: 매개변수에 괄호를 사용해야 우선순위 문제 방지
 * Note: Use parentheses around parameters to prevent precedence issues
 */
#define MAX(a, b)           ((a) > (b) ? (a) : (b))
#define MIN(a, b)           ((a) < (b) ? (a) : (b))
#define ABS(x)              ((x) < 0 ? -(x) : (x))
#define SQUARE(x)           ((x) * (x))

/* 배열 크기 계산 */
/* Calculate array size */
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

/* 비트 연산 매크로 */
/* Bit operation macros */
#define SET_BIT(val, bit)       ((val) | (1U << (bit)))
#define CLEAR_BIT(val, bit)     ((val) & ~(1U << (bit)))
#define TOGGLE_BIT(val, bit)    ((val) ^ (1U << (bit)))
#define CHECK_BIT(val, bit)     (((val) >> (bit)) & 1U)

/* 범위 체크 매크로 */
/* Range check macro */
#define IN_RANGE(val, min, max) ((val) >= (min) && (val) <= (max))

/* 메모리 정렬 매크로 */
/* Memory alignment macro */
#define ALIGN_UP(x, align)      (((x) + ((align) - 1)) & ~((align) - 1))

/* ============================================================================
 * 3. Multi-line Macros (여러 줄 매크로)
 * ============================================================================ */

/**
 * do { ... } while(0) 패턴
 * - 여러 문장을 안전하게 하나의 매크로로 감싸기
 * - if-else에서 안전하게 사용 가능
 * 
 * do { ... } while(0) pattern
 * - Safely wrap multiple statements in one macro
 * - Safe to use in if-else statements
 */
#define SWAP(a, b) do { \
    typeof(a) _temp = (a); \
    (a) = (b); \
    (b) = _temp; \
} while(0)

/* 범위 클램프 매크로 */
/* Range clamp macro */
#define CLAMP(val, min, max) do { \
    if ((val) < (min)) (val) = (min); \
    else if ((val) > (max)) (val) = (max); \
} while(0)

/* ============================================================================
 * 4. Debug Macros (디버그 매크로)
 * ============================================================================ */

/* 디버그 모드 활성화 (컴파일 시 -DDEBUG_MODE로 정의 가능) */
/* Enable debug mode (can be defined with -DDEBUG_MODE at compile time) */
#define DEBUG_MODE

#ifdef DEBUG_MODE
    #define DEBUG_PRINT(fmt, ...) \
        printf("[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
    
    #define DEBUG_LOG(msg)      printf("[DEBUG] %s\n", msg)
    #define DEBUG_VALUE(var)    printf("[DEBUG] " #var " = %d\n", var)
    #define ASSERT(cond)        do { \
        if (!(cond)) { \
            printf("[ASSERT FAILED] %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        } \
    } while(0)
#else
    /* 릴리즈 모드에서는 빈 매크로 */
    /* Empty macros in release mode */
    #define DEBUG_PRINT(fmt, ...)
    #define DEBUG_LOG(msg)
    #define DEBUG_VALUE(var)
    #define ASSERT(cond)
#endif

/* ============================================================================
 * 5. Conditional Compilation (조건부 컴파일)
 * ============================================================================ */

/**
 * 플랫폼 감지 매크로
 * Platform detection macros
 */
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_NAME   "Windows"
    #define PLATFORM_ID     1
#elif defined(__linux__)
    #define PLATFORM_NAME   "Linux"
    #define PLATFORM_ID     2
#elif defined(__APPLE__)
    #define PLATFORM_NAME   "macOS"
    #define PLATFORM_ID     3
#elif defined(__ARM_ARCH)
    #define PLATFORM_NAME   "ARM Embedded"
    #define PLATFORM_ID     4
#else
    #define PLATFORM_NAME   "Unknown"
    #define PLATFORM_ID     0
#endif

/**
 * 아키텍처 감지
 * Architecture detection
 */
#if defined(__x86_64__) || defined(_M_X64)
    #define ARCH_NAME       "x86_64"
    #define ARCH_BITS       64
#elif defined(__i386__) || defined(_M_IX86)
    #define ARCH_NAME       "x86"
    #define ARCH_BITS       32
#elif defined(__arm__)
    #define ARCH_NAME       "ARM"
    #define ARCH_BITS       32
#elif defined(__aarch64__)
    #define ARCH_NAME       "ARM64"
    #define ARCH_BITS       64
#else
    #define ARCH_NAME       "Unknown"
    #define ARCH_BITS       0
#endif

/* ============================================================================
 * 6. Feature Toggle Macros (기능 토글 매크로)
 * ============================================================================ */

/* 기능 활성화/비활성화 */
/* Feature enable/disable */
#define FEATURE_LOGGING         1
#define FEATURE_STATISTICS      1
#define FEATURE_ENCRYPTION      0   /* 비활성화 */

#if FEATURE_LOGGING
    #define LOG(msg)    printf("[LOG] %s\n", msg)
#else
    #define LOG(msg)
#endif

#if FEATURE_STATISTICS
    #define STAT_INCREMENT(counter)     ((counter)++)
    #define STAT_DECREMENT(counter)     ((counter)--)
#else
    #define STAT_INCREMENT(counter)
    #define STAT_DECREMENT(counter)
#endif

/* ============================================================================
 * 7. Static/Dynamic Allocation Toggle (정적/동적 할당 토글)
 * ============================================================================ */

/* 임베디드 환경에서 동적 할당 비활성화 옵션 */
/* Option to disable dynamic allocation in embedded environments */
/* #define USE_STATIC_ALLOCATION */

#ifdef USE_STATIC_ALLOCATION
    #define ALLOCATE_BUFFER(size)   static uint8_t _buffer[size]
    #define FREE_BUFFER(ptr)        /* No-op */
#else
    #include <stdlib.h>
    #define ALLOCATE_BUFFER(size)   malloc(size)
    #define FREE_BUFFER(ptr)        free(ptr)
#endif

/* ============================================================================
 * Demonstration Functions / 시연 함수
 * ============================================================================ */

/**
 * @brief Demonstrate object-like macros / 상수 매크로 시연
 */
void demonstrateConstantMacros(void) {
    printf("============ Constant Macros / 상수 매크로 ============\n\n");
    
    printf("정수 상수:\n");
    printf("  BUFFER_SIZE = %d\n", BUFFER_SIZE);
    printf("  MAX_SENSORS = %d\n", MAX_SENSORS);
    printf("  DEFAULT_TIMEOUT_MS = %d\n", DEFAULT_TIMEOUT_MS);
    
    printf("\n실수 상수:\n");
    printf("  PI = %.10f\n", PI);
    
    printf("\n문자열 상수:\n");
    printf("  VERSION_STRING = \"%s\"\n", VERSION_STRING);
    printf("  DEVICE_NAME = \"%s\"\n", DEVICE_NAME);
    
    printf("\n에러 코드:\n");
    printf("  ERR_SUCCESS = %d\n", ERR_SUCCESS);
    printf("  ERR_FAILURE = %d\n", ERR_FAILURE);
    printf("  ERR_TIMEOUT = %d\n", ERR_TIMEOUT);
    
    printf("========================================================\n");
}

/**
 * @brief Demonstrate function-like macros / 함수형 매크로 시연
 */
void demonstrateFunctionMacros(void) {
    printf("\n============ Function-like Macros / 함수형 매크로 ============\n\n");
    
    int a = 10, b = 20;
    
    printf("기본 연산 매크로:\n");
    printf("  a = %d, b = %d\n", a, b);
    printf("  MAX(a, b) = %d\n", MAX(a, b));
    printf("  MIN(a, b) = %d\n", MIN(a, b));
    printf("  ABS(-15) = %d\n", ABS(-15));
    printf("  SQUARE(5) = %d\n", SQUARE(5));
    
    printf("\n배열 크기 매크로:\n");
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    printf("  int arr[] = {1,2,3,4,5,6,7}\n");
    printf("  ARRAY_SIZE(arr) = %zu\n", ARRAY_SIZE(arr));
    
    printf("\n비트 연산 매크로:\n");
    uint8_t val = 0b00001010;
    printf("  val = 0b00001010 (%u)\n", val);
    printf("  SET_BIT(val, 0) = %u\n", SET_BIT(val, 0));
    printf("  CLEAR_BIT(val, 1) = %u\n", CLEAR_BIT(val, 1));
    printf("  TOGGLE_BIT(val, 3) = %u\n", TOGGLE_BIT(val, 3));
    printf("  CHECK_BIT(val, 1) = %u\n", CHECK_BIT(val, 1));
    
    printf("\n범위 체크 매크로:\n");
    printf("  IN_RANGE(15, 10, 20) = %s\n", IN_RANGE(15, 10, 20) ? "true" : "false");
    printf("  IN_RANGE(25, 10, 20) = %s\n", IN_RANGE(25, 10, 20) ? "true" : "false");
    
    printf("\n메모리 정렬 매크로:\n");
    printf("  ALIGN_UP(13, 8) = %d (13을 8의 배수로 올림)\n", ALIGN_UP(13, 8));
    printf("  ALIGN_UP(16, 8) = %d\n", ALIGN_UP(16, 8));
    
    printf("==============================================================\n");
}

/**
 * @brief Demonstrate debug macros / 디버그 매크로 시연
 */
void demonstrateDebugMacros(void) {
    printf("\n============ Debug Macros / 디버그 매크로 ============\n\n");
    
#ifdef DEBUG_MODE
    printf("DEBUG_MODE 활성화됨\n\n");
#else
    printf("DEBUG_MODE 비활성화됨\n\n");
#endif
    
    int sensorValue = 42;
    
    DEBUG_PRINT("센서 값 읽기 시작");
    DEBUG_VALUE(sensorValue);
    DEBUG_PRINT("센서 값: %d", sensorValue);
    
    ASSERT(sensorValue > 0);
    ASSERT(sensorValue < 0);    /* 이 ASSERT는 실패함 */
    
    printf("\n# 연산자 (Stringizing):\n");
    printf("  #var -> 변수 이름을 문자열로 변환\n");
    printf("  DEBUG_VALUE(sensorValue) -> \"sensorValue = 42\"\n");
    
    printf("========================================================\n");
}

/**
 * @brief Demonstrate conditional compilation / 조건부 컴파일 시연
 */
void demonstrateConditionalCompilation(void) {
    printf("\n============ Conditional Compilation / 조건부 컴파일 ============\n\n");
    
    printf("플랫폼 정보:\n");
    printf("  PLATFORM_NAME = \"%s\"\n", PLATFORM_NAME);
    printf("  PLATFORM_ID = %d\n", PLATFORM_ID);
    
    printf("\n아키텍처 정보:\n");
    printf("  ARCH_NAME = \"%s\"\n", ARCH_NAME);
    printf("  ARCH_BITS = %d\n", ARCH_BITS);
    
    printf("\n미리 정의된 매크로:\n");
    printf("  __FILE__ = \"%s\"\n", __FILE__);
    printf("  __LINE__ = %d\n", __LINE__);
    printf("  __DATE__ = \"%s\"\n", __DATE__);
    printf("  __TIME__ = \"%s\"\n", __TIME__);
    
#ifdef __STDC_VERSION__
    printf("  __STDC_VERSION__ = %ldL\n", __STDC_VERSION__);
#endif
    
    printf("\n기능 토글 상태:\n");
    printf("  FEATURE_LOGGING = %d\n", FEATURE_LOGGING);
    printf("  FEATURE_STATISTICS = %d\n", FEATURE_STATISTICS);
    printf("  FEATURE_ENCRYPTION = %d\n", FEATURE_ENCRYPTION);
    
    /* 조건부 로깅 */
    LOG("이 메시지는 FEATURE_LOGGING이 1일 때만 출력됨");
    
    /* 조건부 통계 */
    int counter = 0;
    STAT_INCREMENT(counter);
    STAT_INCREMENT(counter);
    printf("\n  counter after 2 STAT_INCREMENT: %d\n", counter);
    
    printf("================================================================\n");
}

/**
 * @brief Demonstrate macro pitfalls / 매크로 주의사항 시연
 */
void demonstrateMacroPitfalls(void) {
    printf("\n============ Macro Pitfalls / 매크로 주의사항 ============\n\n");
    
    printf("1. 괄호 누락 문제:\n");
    printf("   잘못된 정의: #define SQUARE(x) x * x\n");
    printf("   SQUARE(3+2) = 3+2 * 3+2 = 3+6+2 = 11 (의도: 25)\n");
    printf("   올바른 정의: #define SQUARE(x) ((x) * (x))\n");
    printf("   SQUARE(3+2) = ((3+2) * (3+2)) = 25 ✓\n\n");
    
    printf("2. 부작용 문제:\n");
    printf("   int i = 3;\n");
    printf("   SQUARE(i++) 호출 시:\n");
    printf("   -> ((i++) * (i++)) = (3 * 4) or (4 * 3)?\n");
    printf("   -> 정의되지 않은 동작! 함수 대신 매크로의 한계\n\n");
    
    printf("3. 세미콜론 문제:\n");
    printf("   if (cond)\n");
    printf("       MACRO();  // 매크로가 { ... }로 끝나면 문제\n");
    printf("   else\n");
    printf("       ...       // else가 분리될 수 있음\n");
    printf("   해결책: do { ... } while(0) 패턴 사용\n\n");
    
    printf("4. 디버깅 어려움:\n");
    printf("   매크로는 전처리기에서 확장되어 디버거에서 추적 어려움\n");
    printf("   해결책: inline 함수 사용 고려\n");
    
    printf("==========================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 7: Macros & Conditional Compile\n");
    printf("  Level 7: 매크로와 조건부 컴파일\n");
    printf("========================================\n\n");
    
    /* Test 1: 상수 매크로 */
    printf("[Test 1] Constant Macros\n");
    demonstrateConstantMacros();
    
    /* Test 2: 함수형 매크로 */
    printf("\n[Test 2] Function-like Macros\n");
    demonstrateFunctionMacros();
    
    /* Test 3: 디버그 매크로 */
    printf("\n[Test 3] Debug Macros\n");
    demonstrateDebugMacros();
    
    /* Test 4: 조건부 컴파일 */
    printf("\n[Test 4] Conditional Compilation\n");
    demonstrateConditionalCompilation();
    
    /* Test 5: 매크로 주의사항 */
    printf("\n[Test 5] Macro Pitfalls\n");
    demonstrateMacroPitfalls();
    
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
 * [Test 1] BUFFER_SIZE, VERSION_STRING 등 상수 출력
 * [Test 2] MAX, MIN, ARRAY_SIZE 등 함수형 매크로 결과
 * [Test 3] DEBUG_PRINT, ASSERT 동작 확인
 * [Test 4] 플랫폼/아키텍처 정보 출력
 * [Test 5] 매크로 사용 시 주의사항 설명
 * 
 * ============================================================================
 */
