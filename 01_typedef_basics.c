/**
 * ============================================================================
 * [코딩테스트 Level 1] typedef와 고정 크기 자료형
 * ============================================================================
 * 
 * 문제 설명:
 * 임베디드 시스템에서는 플랫폼에 따라 int, long 등의 크기가 달라질 수 있습니다.
 * 이를 해결하기 위해 고정 크기 자료형(int8_t, int16_t, int32_t 등)을 사용합니다.
 * 또한 typedef를 사용하여 의미 있는 타입 이름을 정의하면 코드의 가독성이 향상됩니다.
 * 
 * 요구사항:
 * 1. 센서 데이터를 저장할 타입 SensorData_t를 int32_t로 정의하세요.
 * 2. 온도 데이터를 저장할 타입 Temperature_t를 int16_t로 정의하세요.
 * 3. 에러 코드를 저장할 타입 ErrorCode_t를 int8_t로 정의하세요.
 * 4. 각 타입의 크기를 sizeof()로 확인하는 함수를 작성하세요.
 * 5. 각 타입의 최대/최소값을 출력하는 함수를 작성하세요.
 * 
 * 학습 포인트:
 * - typedef의 사용법과 장점
 * - stdint.h의 고정 크기 자료형
 * - limits.h 또는 stdint.h의 최대/최소값 매크로
 * - sizeof 연산자
 * 
 * ============================================================================
 * File: 01_typedef_basics.c
 * 파일명: 01_typedef_basics.c
 * Author / 작성자: [Developer]
 * Date / 작성일: 2026-01-14
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>     /* int8_t, int16_t, int32_t, uint8_t, etc. */
                        /* 고정 크기 정수형 타입 정의 */
#include <stdbool.h>    /* bool, true, false */
                        /* 불리언 타입 정의 */
#include <limits.h>     /* INT_MAX, INT_MIN, etc. */
                        /* 정수형 한계값 정의 */

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
 * Type Definitions using typedef / typedef를 사용한 타입 정의
 * ============================================================================ */

/**
 * TODO: 아래 typedef 문을 완성하세요.
 * TODO: Complete the typedef statements below.
 * 
 * 힌트: typedef 기존타입 새로운타입이름;
 * Hint: typedef existing_type new_type_name;
 */

/* 센서 데이터 타입: 32비트 부호있는 정수 */
/* Sensor data type: 32-bit signed integer */
typedef int32_t SensorData_t;

/* 온도 데이터 타입: 16비트 부호있는 정수 (소수점 1자리를 정수로 표현, 예: 25.5도 -> 255) */
/* Temperature type: 16-bit signed integer (1 decimal place as integer, e.g., 25.5°C -> 255) */
typedef int16_t Temperature_t;

/* 에러 코드 타입: 8비트 부호있는 정수 */
/* Error code type: 8-bit signed integer */
typedef int8_t ErrorCode_t;

/* 버퍼 인덱스 타입: 32비트 부호없는 정수 */
/* Buffer index type: 32-bit unsigned integer */
typedef uint32_t BufferIndex_t;

/* 불리언 타입 별칭 (가독성 향상용) */
/* Boolean type alias (for readability) */
typedef bool Status_t;

/* ============================================================================
 * Error Code Definitions / 에러 코드 정의
 * ============================================================================ */
#define ERR_SUCCESS         ((ErrorCode_t)0)    /* 성공 / Success */
#define ERR_INVALID_PARAM   ((ErrorCode_t)-1)   /* 잘못된 매개변수 / Invalid parameter */
#define ERR_OVERFLOW        ((ErrorCode_t)-2)   /* 오버플로우 / Overflow */
#define ERR_UNDERFLOW       ((ErrorCode_t)-3)   /* 언더플로우 / Underflow */

/* ============================================================================
 * Function Declarations / 함수 선언
 * ============================================================================ */

/**
 * @brief Print size of each typedef type / 각 typedef 타입의 크기 출력
 * 
 * sizeof() 연산자 설명:
 * - 컴파일 타임에 타입이나 변수의 바이트 크기를 반환
 * - Returns the byte size of a type or variable at compile time
 */
void printTypeSizes(void) {
    printf("============ Type Sizes / 타입 크기 ============\n");
    printf("sizeof(SensorData_t)  = %zu bytes / 바이트\n", sizeof(SensorData_t));
    printf("sizeof(Temperature_t) = %zu bytes / 바이트\n", sizeof(Temperature_t));
    printf("sizeof(ErrorCode_t)   = %zu bytes / 바이트\n", sizeof(ErrorCode_t));
    printf("sizeof(BufferIndex_t) = %zu bytes / 바이트\n", sizeof(BufferIndex_t));
    printf("sizeof(Status_t)      = %zu bytes / 바이트\n", sizeof(Status_t));
    printf("================================================\n");
}

/**
 * @brief Print min/max values of each type / 각 타입의 최소/최대값 출력
 * 
 * stdint.h 매크로:
 * - INT8_MIN, INT8_MAX: int8_t의 최소/최대값
 * - INT16_MIN, INT16_MAX: int16_t의 최소/최대값
 * - INT32_MIN, INT32_MAX: int32_t의 최소/최대값
 * - UINT32_MAX: uint32_t의 최대값 (최소값은 0)
 */
void printTypeRanges(void) {
    printf("============ Type Ranges / 타입 범위 ============\n");
    
    /* SensorData_t (int32_t) 범위 */
    printf("SensorData_t (int32_t):\n");
    printf("  Min / 최소: %d\n", INT32_MIN);
    printf("  Max / 최대: %d\n", INT32_MAX);
    
    /* Temperature_t (int16_t) 범위 */
    printf("Temperature_t (int16_t):\n");
    printf("  Min / 최소: %d\n", INT16_MIN);
    printf("  Max / 최대: %d\n", INT16_MAX);
    
    /* ErrorCode_t (int8_t) 범위 */
    printf("ErrorCode_t (int8_t):\n");
    printf("  Min / 최소: %d\n", INT8_MIN);
    printf("  Max / 최대: %d\n", INT8_MAX);
    
    /* BufferIndex_t (uint32_t) 범위 */
    printf("BufferIndex_t (uint32_t):\n");
    printf("  Min / 최소: %u\n", 0);
    printf("  Max / 최대: %u\n", UINT32_MAX);
    
    printf("=================================================\n");
}

/**
 * @brief Convert temperature to display format / 온도를 표시 형식으로 변환
 * @param temp Temperature value (x10, e.g., 255 = 25.5°C)
 *             온도 값 (x10, 예: 255 = 25.5도)
 * 
 * 예제: Temperature_t에 255를 저장하면 25.5도를 의미
 * Example: Storing 255 in Temperature_t means 25.5°C
 */
void printTemperature(Temperature_t temp) {
    /* 정수부와 소수부 분리 */
    /* Separate integer and decimal parts */
    int16_t integer_part = temp / 10;
    int16_t decimal_part = temp % 10;
    
    /* 음수 처리 */
    /* Handle negative numbers */
    if (temp < 0 && decimal_part != 0) {
        decimal_part = -decimal_part;   /* 소수부는 항상 양수로 표시 */
    }
    
    printf("Temperature / 온도: %d.%d C\n", integer_part, decimal_part);
}

/**
 * @brief Validate sensor data range / 센서 데이터 범위 검증
 * @param data Sensor data to validate / 검증할 센서 데이터
 * @param min Minimum valid value / 최소 유효값
 * @param max Maximum valid value / 최대 유효값
 * @return ERR_SUCCESS if valid, error code otherwise
 *         유효하면 ERR_SUCCESS, 아니면 에러 코드
 */
ErrorCode_t validateSensorData(SensorData_t data, SensorData_t min, SensorData_t max) {
    if (data < min) {
        printf("[ERROR] Data underflow: %d < %d\n", data, min);
        printf("[ERROR] 데이터 언더플로우: %d < %d\n", data, min);
        return ERR_UNDERFLOW;
    }
    
    if (data > max) {
        printf("[ERROR] Data overflow: %d > %d\n", data, max);
        printf("[ERROR] 데이터 오버플로우: %d > %d\n", data, max);
        return ERR_OVERFLOW;
    }
    
    return ERR_SUCCESS;
}

/**
 * @brief Demonstrate typedef advantages / typedef의 장점 시연
 */
void demonstrateTypedefAdvantages(void) {
    printf("============ typedef Advantages / typedef 장점 ============\n");
    
    /* 1. 코드 가독성 향상 */
    /* 1. Improved code readability */
    printf("1. Readability / 가독성:\n");
    printf("   Before: int32_t sensorValue;\n");
    printf("   After:  SensorData_t sensorValue;\n");
    printf("   -> 변수의 용도가 명확해짐 / Purpose of variable is clearer\n\n");
    
    /* 2. 유지보수 용이성 */
    /* 2. Easier maintenance */
    printf("2. Maintainability / 유지보수성:\n");
    printf("   typedef int32_t SensorData_t;  // 한 곳만 수정하면 됨\n");
    printf("   -> 타입 변경 시 typedef만 수정 / Only modify typedef when changing type\n\n");
    
    /* 3. 플랫폼 독립성 */
    /* 3. Platform independence */
    printf("3. Platform Independence / 플랫폼 독립성:\n");
    printf("   int: 플랫폼마다 크기가 다름 (16/32/64 bit)\n");
    printf("   int32_t: 항상 32비트 보장\n");
    printf("   -> 임베디드 시스템에서 필수 / Essential for embedded systems\n");
    
    printf("===========================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 1: typedef and Fixed-Size Types\n");
    printf("  Level 1: typedef와 고정 크기 자료형\n");
    printf("========================================\n\n");
    
    /* Test 1: 타입 크기 확인 */
    printf("[Test 1] Type Sizes / 타입 크기 확인\n");
    printTypeSizes();
    
    /* Test 2: 타입 범위 확인 */
    printf("\n[Test 2] Type Ranges / 타입 범위 확인\n");
    printTypeRanges();
    
    /* Test 3: 변수 선언 및 사용 */
    printf("\n[Test 3] Variable Declaration / 변수 선언 및 사용\n");
    SensorData_t sensorValue = 12345;
    Temperature_t currentTemp = 255;    /* 25.5도 */
    ErrorCode_t result = ERR_SUCCESS;
    BufferIndex_t index = 0;
    Status_t isValid = true;
    
    printf("sensorValue = %d\n", sensorValue);
    printTemperature(currentTemp);
    printf("result = %d (ERR_SUCCESS)\n", result);
    printf("index = %u\n", index);
    printf("isValid = %s\n", isValid ? "true" : "false");
    
    /* Test 4: 센서 데이터 검증 */
    printf("\n[Test 4] Sensor Data Validation / 센서 데이터 검증\n");
    SensorData_t testData1 = 500;
    SensorData_t testData2 = -100;
    SensorData_t testData3 = 2000;
    
    printf("Validating %d (range: 0 ~ 1000):\n", testData1);
    result = validateSensorData(testData1, 0, 1000);
    printf("Result: %s\n\n", result == ERR_SUCCESS ? "VALID" : "INVALID");
    
    printf("Validating %d (range: 0 ~ 1000):\n", testData2);
    result = validateSensorData(testData2, 0, 1000);
    printf("Result: %s\n\n", result == ERR_SUCCESS ? "VALID" : "INVALID");
    
    printf("Validating %d (range: 0 ~ 1000):\n", testData3);
    result = validateSensorData(testData3, 0, 1000);
    printf("Result: %s\n", result == ERR_SUCCESS ? "VALID" : "INVALID");
    
    /* Test 5: typedef 장점 설명 */
    printf("\n[Test 5] typedef Advantages / typedef 장점\n");
    demonstrateTypedefAdvantages();
    
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
 * [Test 1] Type Sizes / 타입 크기 확인
 * sizeof(SensorData_t)  = 4 bytes
 * sizeof(Temperature_t) = 2 bytes
 * sizeof(ErrorCode_t)   = 1 bytes
 * sizeof(BufferIndex_t) = 4 bytes
 * sizeof(Status_t)      = 1 bytes
 * 
 * [Test 2] Type Ranges / 타입 범위 확인
 * SensorData_t: -2147483648 ~ 2147483647
 * Temperature_t: -32768 ~ 32767
 * ErrorCode_t: -128 ~ 127
 * BufferIndex_t: 0 ~ 4294967295
 * 
 * ============================================================================
 */
