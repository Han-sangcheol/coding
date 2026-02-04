/*
 * ============================================================================
 * 파일명: 06_factory_pattern_FW.c
 * 주제: 펌웨어용 Factory Pattern (malloc 없이 정적 메모리 풀 사용)
 * 컴파일: gcc 06_factory_pattern_FW.c -o 06_factory_pattern_FW
 * 
 * ============================================================================
 * 임베디드/펌웨어 환경 특징
 * ============================================================================
 * 
 * 1. malloc/free 사용 금지
 *    - 메모리 단편화 위험
 *    - 할당 실패 시 처리 어려움
 *    - 실시간성 보장 어려움
 * 
 * 2. 정적 메모리 풀 사용
 *    - 컴파일 타임에 메모리 크기 결정
 *    - 빠른 할당/해제
 *    - 예측 가능한 동작
 * 
 * 3. 리소스 제한
 *    - 제한된 RAM
 *    - 센서 개수 제한
 *    - 미리 메모리 할당
 * 
 * ============================================================================
 * 이 파일의 기능
 * ============================================================================
 * 
 * [Factory Pattern - 펌웨어 버전]
 * - 정적 메모리 풀로 센서 관리
 * - 센서 할당/해제 (메모리 풀 기반)
 * - 센서 타입별 생성
 * - 센서 사용 상태 추적
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* ============================================================================
 * 설정 및 제한값
 * ============================================================================ */

#define MAX_SENSORS         10      // 최대 센서 개수 (펌웨어 제한)
#define MAX_SENSOR_NAME     32      // 센서 이름 최대 길이

/* ============================================================================
 * 센서 타입 정의
 * ============================================================================ */

typedef enum SensorType {
    SENSOR_TYPE_TEMPERATURE,
    SENSOR_TYPE_HUMIDITY,
    SENSOR_TYPE_PRESSURE
} SensorType;

/* ============================================================================
 * 공통 센서 인터페이스
 * ============================================================================ */

typedef struct Sensor {
    SensorType type;
    int sensor_id;
    bool is_active;     // ★ 펌웨어용: 센서 활성 상태
    float (*read)(struct Sensor* self);
    void (*init)(struct Sensor* self);
    void (*deinit)(struct Sensor* self);  // destroy → deinit (펌웨어 관례)
    char name[MAX_SENSOR_NAME];
} Sensor;

/* ============================================================================
 * 센서별 구조체 정의 (기존과 동일)
 * ============================================================================ */

// 온도 센서
typedef struct {
    Sensor base;
    float offset;
    float scale;
} TemperatureSensor;

// 습도 센서
typedef struct {
    Sensor base;
    int calibration_value;
} HumiditySensor;

// 압력 센서
typedef struct {
    Sensor base;
    float sea_level_pressure;
} PressureSensor;

/* ============================================================================
 * 유니온을 사용한 메모리 풀 (핵심!)
 * ============================================================================ */

// 모든 센서 타입을 담을 수 있는 유니온
typedef union SensorUnion {
    Sensor base;                    // 공통 인터페이스
    TemperatureSensor temp;         // 온도 센서
    HumiditySensor humid;           // 습도 센서
    PressureSensor press;           // 압력 센서
} SensorUnion;

// 정적 메모리 풀
static SensorUnion g_sensor_pool[MAX_SENSORS];

// 센서 사용 상태 추적
static bool g_sensor_used[MAX_SENSORS] = {false};

/* ============================================================================
 * 메모리 풀 관리 함수
 * ============================================================================ */

/**
 * @brief 사용 가능한 센서 슬롯 찾기
 * @return 사용 가능한 인덱스, 없으면 -1
 */
static int SensorPool_FindFreeSlot(void) {
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (!g_sensor_used[i]) {
            return i;
        }
    }
    return -1;  // 사용 가능한 슬롯 없음
}

/**
 * @brief 센서 슬롯 할당
 * @return 할당된 Sensor 포인터, 실패 시 NULL
 */
static Sensor* SensorPool_Alloc(void) {
    int slot = SensorPool_FindFreeSlot();
    if (slot < 0) {
        printf("[Pool] ❌ 메모리 풀 가득 참! (최대 %d개)\n", MAX_SENSORS);
        return NULL;
    }
    
    g_sensor_used[slot] = true;
    
    // 메모리 초기화
    memset(&g_sensor_pool[slot], 0, sizeof(SensorUnion));
    
    printf("[Pool] ✅ 슬롯[%d] 할당\n", slot);
    return &g_sensor_pool[slot].base;
}

/**
 * @brief 센서 슬롯 해제
 * @param sensor 해제할 센서
 */
static void SensorPool_Free(Sensor* sensor) {
    if (sensor == NULL) {
        return;
    }
    
    // 포인터에서 인덱스 계산
    int slot = ((SensorUnion*)sensor) - g_sensor_pool;
    
    if (slot < 0 || slot >= MAX_SENSORS) {
        printf("[Pool] ❌ 잘못된 센서 포인터\n");
        return;
    }
    
    if (!g_sensor_used[slot]) {
        printf("[Pool] ⚠️ 이미 해제된 슬롯[%d]\n", slot);
        return;
    }
    
    g_sensor_used[slot] = false;
    sensor->is_active = false;
    
    printf("[Pool] ✅ 슬롯[%d] 해제\n", slot);
}

/**
 * @brief 메모리 풀 상태 출력
 */
void SensorPool_PrintStatus(void) {
    int used_count = 0;
    
    printf("\n[Pool] 메모리 풀 상태:\n");
    printf("  총 슬롯: %d\n", MAX_SENSORS);
    
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (g_sensor_used[i]) {
            Sensor* s = &g_sensor_pool[i].base;
            printf("  [%d] 사용중 - %s (ID: %d, 타입: %d)\n", 
                   i, s->name, s->sensor_id, s->type);
            used_count++;
        } else {
            printf("  [%d] 비어있음\n", i);
        }
    }
    
    printf("  사용중: %d개, 남은 슬롯: %d개\n", used_count, MAX_SENSORS - used_count);
}

/* ============================================================================
 * 온도 센서 함수들
 * ============================================================================ */

float Temperature_Read(Sensor* self) {
    TemperatureSensor* ts = (TemperatureSensor*)self;
    
    // 실제 펌웨어에서는 ADC 읽기, I2C 통신 등
    // 여기서는 시뮬레이션
    static uint32_t tick = 0;
    float raw = 20.0f + (tick++ % 100) / 10.0f;
    
    return raw * ts->scale + ts->offset;
}

void Temperature_Init(Sensor* self) {
    TemperatureSensor* ts = (TemperatureSensor*)self;
    ts->offset = 0.0f;
    ts->scale = 1.0f;
    self->is_active = true;
    
    printf("[FW] 온도 센서 %d 초기화 (오프셋: %.2f, 스케일: %.2f)\n", 
           self->sensor_id, ts->offset, ts->scale);
}

void Temperature_Deinit(Sensor* self) {
    printf("[FW] 온도 센서 %d 해제\n", self->sensor_id);
    self->is_active = false;
    // malloc 버전과 달리 free 호출 안 함!
}

/* ============================================================================
 * 습도 센서 함수들
 * ============================================================================ */

float Humidity_Read(Sensor* self) {
    HumiditySensor* hs = (HumiditySensor*)self;
    
    static uint32_t tick = 0;
    float raw = 30.0f + (tick++ % 500) / 10.0f;
    
    return raw + hs->calibration_value;
}

void Humidity_Init(Sensor* self) {
    HumiditySensor* hs = (HumiditySensor*)self;
    hs->calibration_value = 0;
    self->is_active = true;
    
    printf("[FW] 습도 센서 %d 초기화 (보정값: %d)\n", 
           self->sensor_id, hs->calibration_value);
}

void Humidity_Deinit(Sensor* self) {
    printf("[FW] 습도 센서 %d 해제\n", self->sensor_id);
    self->is_active = false;
}

/* ============================================================================
 * 압력 센서 함수들
 * ============================================================================ */

float Pressure_Read(Sensor* self) {
    PressureSensor* ps = (PressureSensor*)self;
    
    static uint32_t tick = 0;
    return ps->sea_level_pressure + (tick++ % 200) / 10.0f - 10.0f;
}

void Pressure_Init(Sensor* self) {
    PressureSensor* ps = (PressureSensor*)self;
    ps->sea_level_pressure = 1013.25f;
    self->is_active = true;
    
    printf("[FW] 압력 센서 %d 초기화 (해수면 기압: %.2f)\n", 
           self->sensor_id, ps->sea_level_pressure);
}

void Pressure_Deinit(Sensor* self) {
    printf("[FW] 압력 센서 %d 해제\n", self->sensor_id);
    self->is_active = false;
}

/* ============================================================================
 * Factory 함수 (메모리 풀 버전)
 * ============================================================================ */

/**
 * @brief 센서 생성 (메모리 풀에서 할당)
 * @param type 센서 타입
 * @param sensor_id 센서 ID
 * @return 생성된 센서 포인터, 실패 시 NULL
 */
Sensor* SensorFactory_Create(SensorType type, int sensor_id) {
    printf("[Factory] 센서 생성 요청: 타입=%d, ID=%d\n", type, sensor_id);
    
    // 메모리 풀에서 할당 (malloc 대신!)
    Sensor* sensor = SensorPool_Alloc();
    if (sensor == NULL) {
        printf("[Factory] ❌ 센서 생성 실패: 메모리 부족\n");
        return NULL;
    }
    
    // 타입별로 초기화
    switch (type) {
        case SENSOR_TYPE_TEMPERATURE: {
            TemperatureSensor* ts = (TemperatureSensor*)sensor;
            ts->base.type = type;
            ts->base.sensor_id = sensor_id;
            ts->base.read = Temperature_Read;
            ts->base.init = Temperature_Init;
            ts->base.deinit = Temperature_Deinit;
            snprintf(ts->base.name, MAX_SENSOR_NAME, "Temperature_%d", sensor_id);
            break;
        }
        
        case SENSOR_TYPE_HUMIDITY: {
            HumiditySensor* hs = (HumiditySensor*)sensor;
            hs->base.type = type;
            hs->base.sensor_id = sensor_id;
            hs->base.read = Humidity_Read;
            hs->base.init = Humidity_Init;
            hs->base.deinit = Humidity_Deinit;
            snprintf(hs->base.name, MAX_SENSOR_NAME, "Humidity_%d", sensor_id);
            break;
        }
        
        case SENSOR_TYPE_PRESSURE: {
            PressureSensor* ps = (PressureSensor*)sensor;
            ps->base.type = type;
            ps->base.sensor_id = sensor_id;
            ps->base.read = Pressure_Read;
            ps->base.init = Pressure_Init;
            ps->base.deinit = Pressure_Deinit;
            snprintf(ps->base.name, MAX_SENSOR_NAME, "Pressure_%d", sensor_id);
            break;
        }
        
        default:
            printf("[Factory] ❌ 알 수 없는 센서 타입: %d\n", type);
            SensorPool_Free(sensor);
            return NULL;
    }
    
    printf("[Factory] ✅ 센서 생성 완료: %s\n", sensor->name);
    return sensor;
}

/**
 * @brief 센서 파괴 (메모리 풀로 반환)
 * @param sensor 파괴할 센서
 */
void SensorFactory_Destroy(Sensor* sensor) {
    if (sensor == NULL) {
        return;
    }
    
    printf("[Factory] 센서 파괴: %s\n", sensor->name);
    
    // deinit 호출
    if (sensor->deinit) {
        sensor->deinit(sensor);
    }
    
    // 메모리 풀로 반환 (free 대신!)
    SensorPool_Free(sensor);
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Factory Pattern - 펌웨어 버전\n");
    printf("(malloc 없이 정적 메모리 풀 사용)\n");
    printf("========================================\n");
    
    // 초기 메모리 풀 상태
    SensorPool_PrintStatus();
    
    // ========================================
    // 테스트 1: 센서 생성 및 사용
    // ========================================
    printf("\n\n=== 테스트 1: 센서 생성 및 사용 ===\n");
    
    Sensor* sensors[5];
    sensors[0] = SensorFactory_Create(SENSOR_TYPE_TEMPERATURE, 101);
    sensors[1] = SensorFactory_Create(SENSOR_TYPE_HUMIDITY, 102);
    sensors[2] = SensorFactory_Create(SENSOR_TYPE_PRESSURE, 103);
    sensors[3] = SensorFactory_Create(SENSOR_TYPE_TEMPERATURE, 104);
    sensors[4] = SensorFactory_Create(SENSOR_TYPE_HUMIDITY, 105);
    
    // 메모리 풀 상태
    SensorPool_PrintStatus();
    
    // 센서 초기화
    printf("\n--- 센서 초기화 ---\n");
    for (int i = 0; i < 5; i++) {
        if (sensors[i]) {
            sensors[i]->init(sensors[i]);
        }
    }
    
    // 센서 읽기
    printf("\n--- 센서 읽기 (첫 번째) ---\n");
    for (int i = 0; i < 5; i++) {
        if (sensors[i] && sensors[i]->is_active) {
            float value = sensors[i]->read(sensors[i]);
            printf("  %s: %.2f\n", sensors[i]->name, value);
        }
    }
    
    printf("\n--- 센서 읽기 (두 번째) ---\n");
    for (int i = 0; i < 5; i++) {
        if (sensors[i] && sensors[i]->is_active) {
            float value = sensors[i]->read(sensors[i]);
            printf("  %s: %.2f\n", sensors[i]->name, value);
        }
    }
    
    // ========================================
    // 테스트 2: 센서 해제 및 재사용
    // ========================================
    printf("\n\n=== 테스트 2: 센서 해제 및 재사용 ===\n");
    
    printf("\n--- 센서 2개 해제 ---\n");
    SensorFactory_Destroy(sensors[1]);
    SensorFactory_Destroy(sensors[3]);
    sensors[1] = NULL;
    sensors[3] = NULL;
    
    SensorPool_PrintStatus();
    
    printf("\n--- 해제된 슬롯 재사용 ---\n");
    sensors[1] = SensorFactory_Create(SENSOR_TYPE_PRESSURE, 201);
    sensors[3] = SensorFactory_Create(SENSOR_TYPE_TEMPERATURE, 202);
    
    if (sensors[1]) sensors[1]->init(sensors[1]);
    if (sensors[3]) sensors[3]->init(sensors[3]);
    
    SensorPool_PrintStatus();
    
    // ========================================
    // 테스트 3: 메모리 풀 한계 테스트
    // ========================================
    printf("\n\n=== 테스트 3: 메모리 풀 한계 테스트 ===\n");
    
    printf("\n--- 최대 개수까지 생성 ---\n");
    Sensor* extra_sensors[MAX_SENSORS];
    int created = 0;
    
    for (int i = 0; i < MAX_SENSORS; i++) {
        extra_sensors[i] = SensorFactory_Create(SENSOR_TYPE_TEMPERATURE, 300 + i);
        if (extra_sensors[i]) {
            created++;
        }
    }
    
    printf("\n생성된 센서: %d개\n", created);
    SensorPool_PrintStatus();
    
    printf("\n--- 하나 더 생성 시도 (실패해야 함) ---\n");
    Sensor* overflow = SensorFactory_Create(SENSOR_TYPE_TEMPERATURE, 999);
    if (overflow == NULL) {
        printf("✅ 예상대로 실패! 메모리 풀 보호 동작 확인\n");
    }
    
    // ========================================
    // 정리
    // ========================================
    printf("\n\n=== 정리 ===\n");
    
    // 모든 센서 해제
    for (int i = 0; i < 5; i++) {
        if (sensors[i]) {
            SensorFactory_Destroy(sensors[i]);
        }
    }
    
    for (int i = 0; i < created; i++) {
        if (extra_sensors[i]) {
            SensorFactory_Destroy(extra_sensors[i]);
        }
    }
    
    printf("\n최종 메모리 풀 상태:\n");
    SensorPool_PrintStatus();
    
    // ========================================
    // 요약
    // ========================================
    printf("\n========================================\n");
    printf("핵심 정리\n");
    printf("========================================\n");
    printf("\n1. malloc/free 사용 안 함\n");
    printf("   - 정적 배열로 메모리 풀 구성\n");
    printf("   - 컴파일 타임에 메모리 크기 확정\n");
    printf("\n2. 메모리 풀 관리\n");
    printf("   - SensorPool_Alloc(): 슬롯 할당\n");
    printf("   - SensorPool_Free(): 슬롯 반환\n");
    printf("   - 사용 상태 추적 (g_sensor_used)\n");
    printf("\n3. 유니온 활용\n");
    printf("   - 모든 센서 타입을 담을 수 있는 크기\n");
    printf("   - 메모리 효율적\n");
    printf("\n4. 펌웨어 장점\n");
    printf("   - 예측 가능한 메모리 사용\n");
    printf("   - 메모리 단편화 없음\n");
    printf("   - 빠른 할당/해제\n");
    printf("   - 최대 개수 제한으로 안정성 확보\n");
    printf("\n5. sizeof 정보\n");
    printf("   - sizeof(SensorUnion):   %zu bytes\n", sizeof(SensorUnion));
    printf("   - sizeof(g_sensor_pool): %zu bytes\n", sizeof(g_sensor_pool));
    printf("   - 전체 메모리:           %zu bytes (%zu KB)\n", 
           sizeof(g_sensor_pool), sizeof(g_sensor_pool) / 1024);
    printf("========================================\n");
    
    return 0;
}

/* ============================================================================
 * 학습 포인트
 * ============================================================================
 * 
 * 1. malloc vs 메모리 풀
 *    malloc:        동적 할당, 단편화 위험, 실시간성 낮음
 *    메모리 풀:     정적 할당, 단편화 없음, 빠름
 * 
 * 2. 유니온 활용
 *    - 여러 타입 중 가장 큰 크기로 할당
 *    - 메모리 효율적
 * 
 * 3. 슬롯 관리
 *    - 사용/미사용 상태 추적
 *    - 빠른 검색 (순차 검색)
 * 
 * 4. 에러 처리
 *    - 메모리 부족 시 NULL 반환
 *    - 명확한 에러 메시지
 * 
 * 5. 펌웨어 관례
 *    - destroy → deinit
 *    - is_active 플래그
 *    - snprintf (버퍼 오버플로 방지)
 * 
 * ============================================================================
 */
