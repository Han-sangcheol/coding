/**
 * ============================================================================
 * [코딩테스트 Level 2] 구조체 정의와 초기화
 * ============================================================================
 * 
 * 문제 설명:
 * 구조체(struct)는 관련된 데이터를 하나로 묶어 관리할 수 있게 해줍니다.
 * 임베디드 시스템에서 센서 데이터, 설정 정보, 상태 정보 등을 구조체로 관리합니다.
 * 
 * 요구사항:
 * 1. 센서 정보를 담는 구조체 SensorInfo_t를 정의하세요.
 *    - id: 센서 ID (uint8_t)
 *    - name: 센서 이름 (char[16])
 *    - value: 현재 값 (int32_t)
 *    - isActive: 활성화 여부 (bool)
 * 
 * 2. 구조체를 초기화하는 함수를 작성하세요.
 * 3. 구조체 멤버에 접근하여 값을 출력하는 함수를 작성하세요.
 * 4. 구조체 배열을 사용하여 여러 센서를 관리하세요.
 * 
 * 학습 포인트:
 * - typedef struct 문법
 * - 구조체 멤버 접근 (. 연산자)
 * - 구조체 초기화 방법 (선언 시, 개별 할당)
 * - 구조체 배열
 * 
 * ============================================================================
 * File: 02_struct_definition.c
 * 파일명: 02_struct_definition.c
 * Author / 작성자: [Developer]
 * Date / 작성일: 2026-01-14
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>     /* strcpy, memset */

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
#define MAX_SENSOR_NAME     16      /* Maximum sensor name length / 센서 이름 최대 길이 */
#define MAX_SENSORS         4       /* Maximum number of sensors / 최대 센서 개수 */

/* ============================================================================
 * Structure Definitions / 구조체 정의
 * ============================================================================ */

/**
 * Sensor Information Structure / 센서 정보 구조체
 * 
 * 구조체 정의 방법:
 * 1. struct 키워드로 구조체 정의
 * 2. typedef로 별칭 부여 (사용 시 struct 키워드 생략 가능)
 * 
 * Memory Layout / 메모리 레이아웃:
 * +----+----------------+-------+----------+
 * | id |     name       | value | isActive |
 * +----+----------------+-------+----------+
 * | 1B |      16B       |  4B   |    1B    |  (+ padding)
 * +----+----------------+-------+----------+
 */
typedef struct {
    uint8_t     id;                     /* Sensor ID / 센서 ID (0~255) */
    char        name[MAX_SENSOR_NAME];  /* Sensor name / 센서 이름 */
    int32_t     value;                  /* Current value / 현재 측정값 */
    bool        isActive;               /* Active status / 활성화 상태 */
} SensorInfo_t;

/**
 * Sensor Configuration Structure / 센서 설정 구조체
 * 
 * 중첩 구조체 예시: 구조체 안에 다른 구조체 포함 가능
 * Nested struct example: A struct can contain another struct
 */
typedef struct {
    int32_t     minValue;       /* Minimum valid value / 최소 유효값 */
    int32_t     maxValue;       /* Maximum valid value / 최대 유효값 */
    uint16_t    sampleRate;     /* Sample rate in Hz / 샘플링 주기 (Hz) */
    bool        enableAlarm;    /* Alarm enable flag / 알람 활성화 여부 */
} SensorConfig_t;

/**
 * Complete Sensor Structure / 완전한 센서 구조체
 * 
 * 구조체 안에 다른 구조체를 멤버로 포함
 * Contains another struct as a member
 */
typedef struct {
    SensorInfo_t    info;       /* Sensor information / 센서 정보 */
    SensorConfig_t  config;     /* Sensor configuration / 센서 설정 */
} Sensor_t;

/* ============================================================================
 * Function Declarations / 함수 선언
 * ============================================================================ */

/**
 * @brief Initialize sensor info structure / 센서 정보 구조체 초기화
 * @param sensor Pointer to sensor info / 센서 정보 포인터
 * @param id Sensor ID / 센서 ID
 * @param name Sensor name / 센서 이름
 * 
 * 구조체 초기화 방법:
 * 1. 선언 시 중괄호로 초기화: SensorInfo_t s = {1, "temp", 0, false};
 * 2. 지정 초기화(C99): SensorInfo_t s = {.id = 1, .name = "temp"};
 * 3. 함수로 초기화 (이 함수처럼)
 */
void initSensorInfo(SensorInfo_t* sensor, uint8_t id, const char* name) {
    /* NULL 포인터 검사 */
    /* NULL pointer check */
    if (sensor == NULL || name == NULL) {
        return;
    }
    
    /* 구조체 전체를 0으로 초기화 */
    /* Initialize entire struct to 0 */
    memset(sensor, 0, sizeof(SensorInfo_t));
    
    /* 개별 멤버 설정 */
    /* Set individual members */
    sensor->id = id;
    
    /* 문자열 복사 (버퍼 오버플로우 방지) */
    /* Copy string (prevent buffer overflow) */
    strncpy(sensor->name, name, MAX_SENSOR_NAME - 1);
    sensor->name[MAX_SENSOR_NAME - 1] = '\0';   /* Null terminator 보장 */
    
    sensor->value = 0;
    sensor->isActive = false;
}

/**
 * @brief Initialize sensor config structure / 센서 설정 구조체 초기화
 * @param config Pointer to config / 설정 포인터
 * @param minVal Minimum value / 최소값
 * @param maxVal Maximum value / 최대값
 * @param sampleRate Sample rate / 샘플링 주기
 */
void initSensorConfig(SensorConfig_t* config, int32_t minVal, int32_t maxVal, uint16_t sampleRate) {
    if (config == NULL) {
        return;
    }
    
    config->minValue = minVal;
    config->maxValue = maxVal;
    config->sampleRate = sampleRate;
    config->enableAlarm = false;
}

/**
 * @brief Print sensor info / 센서 정보 출력
 * @param sensor Pointer to sensor info / 센서 정보 포인터
 * 
 * 멤버 접근:
 * - 구조체 변수: sensor.id
 * - 구조체 포인터: sensor->id (또는 (*sensor).id)
 */
void printSensorInfo(const SensorInfo_t* sensor) {
    if (sensor == NULL) {
        printf("[ERROR] NULL sensor pointer\n");
        return;
    }
    
    printf("┌─────────────────────────────────┐\n");
    printf("│ Sensor Information / 센서 정보 │\n");
    printf("├─────────────────────────────────┤\n");
    printf("│ ID:       %3u                   │\n", sensor->id);
    printf("│ Name:     %-16s    │\n", sensor->name);
    printf("│ Value:    %10d            │\n", sensor->value);
    printf("│ Active:   %-5s                 │\n", sensor->isActive ? "Yes" : "No");
    printf("└─────────────────────────────────┘\n");
}

/**
 * @brief Print sensor config / 센서 설정 출력
 * @param config Pointer to config / 설정 포인터
 */
void printSensorConfig(const SensorConfig_t* config) {
    if (config == NULL) {
        printf("[ERROR] NULL config pointer\n");
        return;
    }
    
    printf("┌─────────────────────────────────┐\n");
    printf("│ Sensor Config / 센서 설정      │\n");
    printf("├─────────────────────────────────┤\n");
    printf("│ Min Value:    %10d        │\n", config->minValue);
    printf("│ Max Value:    %10d        │\n", config->maxValue);
    printf("│ Sample Rate:  %5u Hz          │\n", config->sampleRate);
    printf("│ Alarm:        %-5s             │\n", config->enableAlarm ? "On" : "Off");
    printf("└─────────────────────────────────┘\n");
}

/**
 * @brief Update sensor value / 센서 값 업데이트
 * @param sensor Pointer to sensor / 센서 포인터
 * @param newValue New value / 새로운 값
 */
void updateSensorValue(SensorInfo_t* sensor, int32_t newValue) {
    if (sensor == NULL) {
        return;
    }
    
    sensor->value = newValue;
    printf("[UPDATE] Sensor '%s' value: %d\n", sensor->name, sensor->value);
}

/**
 * @brief Set sensor active status / 센서 활성화 상태 설정
 * @param sensor Pointer to sensor / 센서 포인터
 * @param active Active status / 활성화 상태
 */
void setSensorActive(SensorInfo_t* sensor, bool active) {
    if (sensor == NULL) {
        return;
    }
    
    sensor->isActive = active;
    printf("[STATUS] Sensor '%s' is now %s\n", 
           sensor->name, 
           active ? "ACTIVE / 활성화" : "INACTIVE / 비활성화");
}

/**
 * @brief Print sensor array / 센서 배열 출력
 * @param sensors Array of sensors / 센서 배열
 * @param count Number of sensors / 센서 개수
 */
void printSensorArray(const SensorInfo_t sensors[], size_t count) {
    printf("\n============ Sensor Array / 센서 배열 ============\n");
    printf("%-4s %-16s %10s %8s\n", "ID", "Name", "Value", "Active");
    printf("------------------------------------------------\n");
    
    for (size_t i = 0; i < count; i++) {
        printf("%-4u %-16s %10d %8s\n",
               sensors[i].id,
               sensors[i].name,
               sensors[i].value,
               sensors[i].isActive ? "Yes" : "No");
    }
    printf("================================================\n");
}

/**
 * @brief Calculate struct size with padding / 패딩을 포함한 구조체 크기 계산
 * 
 * 구조체 패딩 설명:
 * - 컴파일러는 메모리 정렬을 위해 패딩 바이트를 추가할 수 있음
 * - 각 멤버는 자신의 크기의 배수 주소에 배치됨
 * - Compiler may add padding bytes for memory alignment
 * - Each member is placed at an address that is a multiple of its size
 */
void demonstrateStructPadding(void) {
    printf("\n============ Struct Padding / 구조체 패딩 ============\n");
    
    /* 각 멤버 크기 */
    printf("Individual member sizes / 개별 멤버 크기:\n");
    printf("  uint8_t id:        %zu byte\n", sizeof(uint8_t));
    printf("  char name[16]:     %zu bytes\n", sizeof(char) * MAX_SENSOR_NAME);
    printf("  int32_t value:     %zu bytes\n", sizeof(int32_t));
    printf("  bool isActive:     %zu byte\n", sizeof(bool));
    printf("  Sum / 합계:        %zu bytes\n", 
           sizeof(uint8_t) + MAX_SENSOR_NAME + sizeof(int32_t) + sizeof(bool));
    
    /* 실제 구조체 크기 (패딩 포함) */
    printf("\nActual struct size / 실제 구조체 크기:\n");
    printf("  sizeof(SensorInfo_t) = %zu bytes\n", sizeof(SensorInfo_t));
    printf("  (Difference is padding / 차이는 패딩)\n");
    
    printf("=====================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 2: Struct Definition\n");
    printf("  Level 2: 구조체 정의와 초기화\n");
    printf("========================================\n\n");
    
    /* Test 1: 구조체 선언 및 초기화 (방법 1: 함수 사용) */
    printf("[Test 1] Initialize using function / 함수로 초기화\n");
    SensorInfo_t tempSensor;
    initSensorInfo(&tempSensor, 1, "Temperature");
    printSensorInfo(&tempSensor);
    
    /* Test 2: 구조체 초기화 (방법 2: 선언 시 초기화) */
    printf("\n[Test 2] Initialize at declaration / 선언 시 초기화\n");
    SensorInfo_t humiditySensor = {
        .id = 2,
        .name = "Humidity",
        .value = 650,       /* 65.0% */
        .isActive = true
    };
    printSensorInfo(&humiditySensor);
    
    /* Test 3: 구조체 멤버 수정 */
    printf("\n[Test 3] Modify struct members / 구조체 멤버 수정\n");
    updateSensorValue(&tempSensor, 255);    /* 25.5도 */
    setSensorActive(&tempSensor, true);
    printSensorInfo(&tempSensor);
    
    /* Test 4: 중첩 구조체 사용 */
    printf("\n[Test 4] Nested struct / 중첩 구조체\n");
    Sensor_t pressureSensor;
    initSensorInfo(&pressureSensor.info, 3, "Pressure");
    initSensorConfig(&pressureSensor.config, 0, 1000, 100);
    
    printf("Complete Sensor / 완전한 센서:\n");
    printSensorInfo(&pressureSensor.info);
    printSensorConfig(&pressureSensor.config);
    
    /* Test 5: 구조체 배열 */
    printf("\n[Test 5] Struct Array / 구조체 배열\n");
    SensorInfo_t sensors[MAX_SENSORS];
    
    initSensorInfo(&sensors[0], 1, "Temp_1");
    sensors[0].value = 250;
    sensors[0].isActive = true;
    
    initSensorInfo(&sensors[1], 2, "Temp_2");
    sensors[1].value = 265;
    sensors[1].isActive = true;
    
    initSensorInfo(&sensors[2], 3, "Humidity");
    sensors[2].value = 550;
    sensors[2].isActive = false;
    
    initSensorInfo(&sensors[3], 4, "Pressure");
    sensors[3].value = 1013;
    sensors[3].isActive = true;
    
    printSensorArray(sensors, MAX_SENSORS);
    
    /* Test 6: 구조체 패딩 확인 */
    printf("\n[Test 6] Struct Padding / 구조체 패딩\n");
    demonstrateStructPadding();
    
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
 * [Test 1] 함수로 초기화한 센서 정보 출력
 * [Test 2] 선언 시 초기화한 센서 정보 출력
 * [Test 3] 멤버 수정 후 센서 정보 출력
 * [Test 4] 중첩 구조체 (info + config) 출력
 * [Test 5] 4개 센서 배열 출력
 * [Test 6] 구조체 패딩 분석 결과
 * 
 * ============================================================================
 */
