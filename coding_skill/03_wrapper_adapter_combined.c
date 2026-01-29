/*
 * ============================================================================
 * 파일명: 03_wrapper_adapter_combined.c
 * 기능: Wrapper 패턴 + Adapter 패턴 통합 예제
 * 
 * ============================================================================
 * 🎯 이 파일의 목적
 * ============================================================================
 * 
 * Wrapper와 Adapter를 함께 사용하여
 * "실전 임베디드 시스템"의 계층 구조를 보여줍니다.
 * 
 * ============================================================================
 * 1️⃣ Wrapper + Adapter 통합 구조 (핵심)
 * ============================================================================
 * 
 * 실제 프로젝트에서는 두 패턴을 단독으로 쓰는 경우가 거의 없습니다.
 * 대부분 다음과 같은 계층 구조를 사용합니다:
 * 
 *   [Application Layer]
 *         ↓
 *   [Wrapper Layer]      ← 정책, 의미, 로깅, 검증
 *         ↓
 *   [Adapter Layer]      ← 인터페이스 통일, 형식 변환
 *         ↓
 *   [Driver/HAL Layer]   ← 실제 하드웨어 제어
 * 
 * ============================================================================
 * 2️⃣ 각 계층의 역할 (명확한 구분)
 * ============================================================================
 * 
 * Application Layer (앱 계층):
 *   - "무엇을 할지" 결정
 *   - 비즈니스 로직
 *   - 예: "온도가 30도 이상이면 경고"
 * 
 * Wrapper Layer (래퍼 계층):
 *   - "어떻게 사용할지" 정의
 *   - 로깅, 검증, 에러 처리
 *   - 의미 있는 이름 제공
 *   - 예: FOTA_Receive() → 내부에서 통신 처리
 * 
 * Adapter Layer (어댑터 계층):
 *   - "서로 다른 것을 연결"
 *   - 인터페이스 형식 변환
 *   - 예: UART/CAN/USB → 공통 Comm 인터페이스
 * 
 * Driver/HAL Layer (드라이버 계층):
 *   - "실제 동작" 수행
 *   - 하드웨어 직접 제어
 *   - 예: HAL_UART_Receive(), CAN_ReadFrame()
 * 
 * ============================================================================
 * 3️⃣ FOTA 시스템 예제 (실전)
 * ============================================================================
 * 
 * FOTA 업데이트를 UART, CAN, USB 모두 지원하는 경우:
 * 
 * Application:
 *   FOTA_Update();  // 앱은 통신 방식 모름
 * 
 * Wrapper:
 *   int FOTA_Receive(uint8_t *buf, uint32_t len)
 *   {
 *       // 로깅
 *       Log("[FOTA] 수신 시작: %d bytes", len);
 *       
 *       // 실제 수신 (Adapter 호출)
 *       int ret = comm->read(buf, len);
 *       
 *       // 검증
 *       if (ret < 0) {
 *           Log("[FOTA] 수신 실패");
 *           return -1;
 *       }
 *       
 *       return ret;
 *   }
 * 
 * Adapter (UART):
 *   int uart_read(uint8_t *buf, uint32_t len)
 *   {
 *       return HAL_UART_Receive(&huart1, buf, len, 1000);
 *   }
 * 
 * Adapter (CAN):
 *   int can_read(uint8_t *buf, uint32_t len)
 *   {
 *       return CAN_ReceiveFrame(buf, len);
 *   }
 * 
 * 사용:
 *   CommIf uart_comm = { .read = uart_read };
 *   CommIf can_comm = { .read = can_read };
 *   
 *   // UART 사용 시
 *   comm = &uart_comm;
 *   FOTA_Receive(buf, 256);  // Wrapper 호출
 *   
 *   // CAN 사용 시 (코드 변경 없음!)
 *   comm = &can_comm;
 *   FOTA_Receive(buf, 256);  // 동일한 Wrapper 호출
 * 
 * ============================================================================
 * 4️⃣ 패턴 적용 순서 (중요)
 * ============================================================================
 * 
 * Step 1: Adapter 먼저 설계
 *   → 서로 다른 드라이버들을 공통 인터페이스로 통일
 * 
 * Step 2: Wrapper 추가
 *   → 통일된 인터페이스에 로깅, 검증, 에러 처리 추가
 * 
 * Step 3: Application 작성
 *   → Wrapper만 호출, Adapter나 Driver는 직접 호출 ❌
 * 
 * 📌 역순으로 하면 망합니다!
 * 
 * ============================================================================
 * 5️⃣ 이 예제의 시나리오
 * ============================================================================
 * 
 * 상황:
 *   - 3가지 센서: 온도(레거시), 습도(신규), 압력(다른 방식)
 *   - 각 센서는 완전히 다른 API 사용
 *   - 센서 모니터링 시스템 구축 필요
 * 
 * 적용:
 *   1. Adapter: 3가지 센서를 공통 Sensor 인터페이스로 통일
 *   2. Wrapper: 로깅, 데이터 검증, 에러 카운팅 추가
 *   3. App: 모니터링 시스템 (모든 센서를 동일하게 처리)
 * 
 * ============================================================================
 * 6️⃣ 실무 적용 시 주의사항
 * ============================================================================
 * 
 * ✅ 해야 할 것:
 *   - 각 계층의 역할을 명확히 구분
 *   - Adapter는 얇고 단순하게 (변환만)
 *   - Wrapper는 정책과 부가 기능만
 *   - Application은 최상위 인터페이스만 호출
 * 
 * ❌ 하지 말아야 할 것:
 *   - Adapter에 로직 추가
 *   - Wrapper에서 Driver 직접 호출
 *   - Application에서 Adapter 직접 호출
 *   - 계층 건너뛰기
 * 
 * ============================================================================
 * 7️⃣ 이렇게 말하면 면접 통과
 * ============================================================================
 * 
 * "프로젝트에서 여러 통신 방식을 지원해야 했습니다.
 *  
 *  먼저 Adapter 계층으로 UART, CAN, USB의 인터페이스를 통일했고,
 *  Wrapper 계층에서 로깅과 에러 처리를 추가했습니다.
 *  
 *  덕분에 상위 FOTA 로직은 통신 방식이 바뀌어도 단 한 줄도 수정하지
 *  않았고, 새로운 통신 방식 추가도 Adapter만 구현하면 됐습니다."
 * 
 * ============================================================================
 * 8️⃣ 이 파일에서 배울 수 있는 것
 * ============================================================================
 * 
 * 1. 다층 아키텍처 설계
 * 2. 확장 가능한 시스템 구조
 * 3. 유지보수가 쉬운 코드 작성법
 * 4. 실전 임베디드 시스템 구조
 * 5. C에서 객체지향 패턴 구현
 * 
 * ============================================================================
 * 🔚 한 줄 요약
 * ============================================================================
 * 
 * Wrapper + Adapter = 
 * "변화에 강하고, 확장 가능하고, 유지보수 쉬운 임베디드 시스템의 정석"
 * 
 * ============================================================================
 * 이 예제 파일의 구조
 * ============================================================================
 * 
 * 1. 공통 센서 인터페이스 정의
 * 2. 3가지 다른 센서 구현 (온도-레거시, 습도-신규, 압력-다른방식)
 * 3. Adapter 레이어 (각 센서를 공통 인터페이스로 변환)
 * 4. Wrapper 레이어 (로깅, 검증, 에러 처리 추가)
 * 5. 센서 모니터링 시스템 (Application)
 * 6. 실행 및 결과 확인
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* ============================================================================
 * 1. 공통 센서 인터페이스 정의
 * ============================================================================ */

// 센서 데이터 구조체
typedef struct {
    float value;           // 측정값
    int sensor_id;        // 센서 ID
    time_t timestamp;     // 측정 시간
    char unit[16];        // 단위
    int is_valid;         // 데이터 유효성
} SensorReading;

// 센서 상태
typedef enum {
    SENSOR_UNINITIALIZED = 0,
    SENSOR_READY = 1,
    SENSOR_ERROR = 2,
    SENSOR_SHUTDOWN = 3
} SensorStatus;

// 센서 인터페이스
typedef struct Sensor {
    void* device;                                    // 실제 장치
    SensorReading (*read)(struct Sensor* self);      // 읽기
    int (*init)(struct Sensor* self);                // 초기화
    void (*close)(struct Sensor* self);              // 종료
    SensorStatus status;                             // 상태
    char name[64];                                   // 이름
    int error_count;                                 // 에러 카운트
} Sensor;

/* ============================================================================
 * 2. 다양한 센서 구현 (서로 다른 인터페이스)
 * ============================================================================ */

// === 2-1. 온도 센서 (화씨 단위, 레거시) ===
typedef struct {
    int id;
    int active;
    float last_temp_f;
} TempSensorOld;

int temp_old_start(TempSensorOld* dev, int id) {
    dev->id = id;
    dev->active = 1;
    dev->last_temp_f = 72.0f;
    printf("[온도센서-레거시 %d] 시작\n", id);
    return 0;
}

float temp_old_get_fahrenheit(TempSensorOld* dev) {
    if (!dev->active) return -999.0f;
    // 시뮬레이션: 70°F ~ 75°F
    dev->last_temp_f = 70.0f + (rand() % 50) / 10.0f;
    return dev->last_temp_f;
}

void temp_old_stop(TempSensorOld* dev) {
    dev->active = 0;
    printf("[온도센서-레거시 %d] 정지\n", dev->id);
}

// === 2-2. 습도 센서 (퍼센트, 신규) ===
typedef struct {
    unsigned int device_id;
    char state[16];
    double humidity_percent;
} HumiditySensorNew;

int humidity_new_initialize(HumiditySensorNew* dev, unsigned int id) {
    dev->device_id = id;
    strcpy(dev->state, "READY");
    dev->humidity_percent = 50.0;
    printf("[습도센서-신규 %u] 초기화\n", id);
    return 1;
}

double humidity_new_measure(HumiditySensorNew* dev, int* error) {
    if (strcmp(dev->state, "READY") != 0) {
        *error = -1;
        return 0.0;
    }
    *error = 0;
    // 시뮬레이션: 40% ~ 60%
    dev->humidity_percent = 40.0 + (rand() % 200) / 10.0;
    return dev->humidity_percent;
}

void humidity_new_terminate(HumiditySensorNew* dev) {
    strcpy(dev->state, "OFF");
    printf("[습도센서-신규 %u] 종료\n", dev->device_id);
}

// === 2-3. 압력 센서 (kPa 단위, 다른 방식) ===
typedef struct {
    int sensor_num;
    int enabled;
    float pressure_kpa;
} PressureSensor;

void pressure_power_on(PressureSensor* dev, int num) {
    dev->sensor_num = num;
    dev->enabled = 1;
    dev->pressure_kpa = 101.3f;
    printf("[압력센서 %d] 전원 ON\n", num);
}

float pressure_read_kpa(PressureSensor* dev) {
    if (!dev->enabled) return 0.0f;
    // 시뮬레이션: 100.0 kPa ~ 102.0 kPa
    dev->pressure_kpa = 100.0f + (rand() % 20) / 10.0f;
    return dev->pressure_kpa;
}

void pressure_power_off(PressureSensor* dev) {
    dev->enabled = 0;
    printf("[압력센서 %d] 전원 OFF\n", dev->sensor_num);
}

/* ============================================================================
 * 3. Adapter 레이어 - 통일된 인터페이스로 변환
 * ============================================================================ */

// === 온도 센서 Adapter ===
SensorReading temp_adapter_read(Sensor* sensor) {
    TempSensorOld* dev = (TempSensorOld*)sensor->device;
    SensorReading reading;
    
    float fahrenheit = temp_old_get_fahrenheit(dev);
    reading.value = (fahrenheit - 32.0f) * 5.0f / 9.0f;  // 섭씨로 변환
    reading.sensor_id = dev->id;
    reading.timestamp = time(NULL);
    strcpy(reading.unit, "°C");
    reading.is_valid = (fahrenheit > -100.0f) ? 1 : 0;
    
    return reading;
}

int temp_adapter_init(Sensor* sensor) {
    TempSensorOld* dev = (TempSensorOld*)sensor->device;
    return temp_old_start(dev, dev->id);
}

void temp_adapter_close(Sensor* sensor) {
    TempSensorOld* dev = (TempSensorOld*)sensor->device;
    temp_old_stop(dev);
}

// === 습도 센서 Adapter ===
SensorReading humidity_adapter_read(Sensor* sensor) {
    HumiditySensorNew* dev = (HumiditySensorNew*)sensor->device;
    SensorReading reading;
    
    int error;
    double humidity = humidity_new_measure(dev, &error);
    
    reading.value = (float)humidity;
    reading.sensor_id = (int)dev->device_id;
    reading.timestamp = time(NULL);
    strcpy(reading.unit, "%");
    reading.is_valid = (error == 0) ? 1 : 0;
    
    return reading;
}

int humidity_adapter_init(Sensor* sensor) {
    HumiditySensorNew* dev = (HumiditySensorNew*)sensor->device;
    return humidity_new_initialize(dev, dev->device_id);
}

void humidity_adapter_close(Sensor* sensor) {
    HumiditySensorNew* dev = (HumiditySensorNew*)sensor->device;
    humidity_new_terminate(dev);
}

// === 압력 센서 Adapter ===
SensorReading pressure_adapter_read(Sensor* sensor) {
    PressureSensor* dev = (PressureSensor*)sensor->device;
    SensorReading reading;
    
    float pressure = pressure_read_kpa(dev);
    reading.value = pressure;
    reading.sensor_id = dev->sensor_num;
    reading.timestamp = time(NULL);
    strcpy(reading.unit, "kPa");
    reading.is_valid = (pressure > 0.0f) ? 1 : 0;
    
    return reading;
}

int pressure_adapter_init(Sensor* sensor) {
    PressureSensor* dev = (PressureSensor*)sensor->device;
    pressure_power_on(dev, dev->sensor_num);
    return 0;
}

void pressure_adapter_close(Sensor* sensor) {
    PressureSensor* dev = (PressureSensor*)sensor->device;
    pressure_power_off(dev);
}

/* ============================================================================
 * 4. Wrapper 레이어 - 로깅, 검증, 에러 처리
 * ============================================================================ */

// 로그 레벨
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// 로그 출력 함수
void log_message(LogLevel level, const char* sensor_name, const char* message) {
    const char* level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_buf[32];
    strftime(time_buf, sizeof(time_buf), "%H:%M:%S", tm_info);
    
    printf("[%s] [%s] [%s] %s\n", time_buf, level_str[level], sensor_name, message);
}

// 데이터 검증 함수
int validate_sensor_data(SensorReading* reading, float min, float max) {
    if (!reading->is_valid) {
        return 0;
    }
    if (reading->value < min || reading->value > max) {
        return 0;
    }
    return 1;
}

// Wrapper - 센서 읽기 (로깅 및 검증 추가)
SensorReading wrapped_sensor_read(Sensor* sensor, float min_valid, float max_valid) {
    char log_msg[256];
    
    snprintf(log_msg, sizeof(log_msg), "데이터 읽기 시작");
    log_message(LOG_DEBUG, sensor->name, log_msg);
    
    // 센서 상태 확인
    if (sensor->status != SENSOR_READY) {
        snprintf(log_msg, sizeof(log_msg), "센서가 준비되지 않음 (상태: %d)", sensor->status);
        log_message(LOG_ERROR, sensor->name, log_msg);
        
        SensorReading error_reading = {0};
        error_reading.is_valid = 0;
        sensor->error_count++;
        return error_reading;
    }
    
    // 실제 데이터 읽기
    SensorReading reading = sensor->read(sensor);
    
    // 데이터 검증
    if (!validate_sensor_data(&reading, min_valid, max_valid)) {
        snprintf(log_msg, sizeof(log_msg), 
                "유효하지 않은 데이터: %.2f %s (범위: %.2f ~ %.2f)", 
                reading.value, reading.unit, min_valid, max_valid);
        log_message(LOG_WARNING, sensor->name, log_msg);
        reading.is_valid = 0;
        sensor->error_count++;
    } else {
        snprintf(log_msg, sizeof(log_msg), "데이터 읽기 성공: %.2f %s", 
                reading.value, reading.unit);
        log_message(LOG_INFO, sensor->name, log_msg);
    }
    
    return reading;
}

// Wrapper - 센서 초기화 (로깅 추가)
int wrapped_sensor_init(Sensor* sensor) {
    char log_msg[256];
    
    snprintf(log_msg, sizeof(log_msg), "초기화 시작");
    log_message(LOG_INFO, sensor->name, log_msg);
    
    int result = sensor->init(sensor);
    
    if (result == 0 || result == 1) {
        sensor->status = SENSOR_READY;
        sensor->error_count = 0;
        snprintf(log_msg, sizeof(log_msg), "초기화 성공");
        log_message(LOG_INFO, sensor->name, log_msg);
    } else {
        sensor->status = SENSOR_ERROR;
        snprintf(log_msg, sizeof(log_msg), "초기화 실패 (코드: %d)", result);
        log_message(LOG_ERROR, sensor->name, log_msg);
    }
    
    return result;
}

// Wrapper - 센서 종료 (로깅 추가)
void wrapped_sensor_close(Sensor* sensor) {
    char log_msg[256];
    
    snprintf(log_msg, sizeof(log_msg), "종료 시작 (총 에러 횟수: %d)", sensor->error_count);
    log_message(LOG_INFO, sensor->name, log_msg);
    
    sensor->close(sensor);
    sensor->status = SENSOR_SHUTDOWN;
    
    snprintf(log_msg, sizeof(log_msg), "종료 완료");
    log_message(LOG_INFO, sensor->name, log_msg);
}

/* ============================================================================
 * 5. 센서 생성 팩토리 함수들
 * ============================================================================ */

Sensor* create_temp_sensor(int id) {
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    TempSensorOld* device = (TempSensorOld*)malloc(sizeof(TempSensorOld));
    
    device->id = id;
    device->active = 0;
    
    sensor->device = device;
    sensor->read = temp_adapter_read;
    sensor->init = temp_adapter_init;
    sensor->close = temp_adapter_close;
    sensor->status = SENSOR_UNINITIALIZED;
    sensor->error_count = 0;
    snprintf(sensor->name, sizeof(sensor->name), "Temperature-%d", id);
    
    return sensor;
}

Sensor* create_humidity_sensor(unsigned int id) {
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    HumiditySensorNew* device = (HumiditySensorNew*)malloc(sizeof(HumiditySensorNew));
    
    device->device_id = id;
    strcpy(device->state, "OFF");
    
    sensor->device = device;
    sensor->read = humidity_adapter_read;
    sensor->init = humidity_adapter_init;
    sensor->close = humidity_adapter_close;
    sensor->status = SENSOR_UNINITIALIZED;
    sensor->error_count = 0;
    snprintf(sensor->name, sizeof(sensor->name), "Humidity-%u", id);
    
    return sensor;
}

Sensor* create_pressure_sensor(int id) {
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    PressureSensor* device = (PressureSensor*)malloc(sizeof(PressureSensor));
    
    device->sensor_num = id;
    device->enabled = 0;
    
    sensor->device = device;
    sensor->read = pressure_adapter_read;
    sensor->init = pressure_adapter_init;
    sensor->close = pressure_adapter_close;
    sensor->status = SENSOR_UNINITIALIZED;
    sensor->error_count = 0;
    snprintf(sensor->name, sizeof(sensor->name), "Pressure-%d", id);
    
    return sensor;
}

void destroy_sensor(Sensor* sensor) {
    if (sensor != NULL) {
        if (sensor->device != NULL) {
            free(sensor->device);
        }
        free(sensor);
    }
}

/* ============================================================================
 * 6. 센서 모니터링 시스템
 * ============================================================================ */

typedef struct {
    Sensor** sensors;
    int sensor_count;
    int monitoring_active;
} MonitoringSystem;

MonitoringSystem* create_monitoring_system(int count) {
    MonitoringSystem* system = (MonitoringSystem*)malloc(sizeof(MonitoringSystem));
    system->sensors = (Sensor**)malloc(sizeof(Sensor*) * count);
    system->sensor_count = count;
    system->monitoring_active = 0;
    return system;
}

void monitoring_system_add_sensor(MonitoringSystem* system, int index, Sensor* sensor) {
    if (index >= 0 && index < system->sensor_count) {
        system->sensors[index] = sensor;
    }
}

void monitoring_system_start(MonitoringSystem* system) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   센서 모니터링 시스템 시작            ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    for (int i = 0; i < system->sensor_count; i++) {
        wrapped_sensor_init(system->sensors[i]);
    }
    
    system->monitoring_active = 1;
    printf("\n");
}

void monitoring_system_collect_data(MonitoringSystem* system) {
    printf("╔════════════════════════════════════════╗\n");
    printf("║       데이터 수집 사이클               ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    // 각 센서별 유효 범위 설정
    float min_values[] = {15.0f, 30.0f, 95.0f};  // 온도, 습도, 압력 최소값
    float max_values[] = {35.0f, 70.0f, 105.0f}; // 온도, 습도, 압력 최대값
    
    for (int i = 0; i < system->sensor_count; i++) {
        SensorReading reading = wrapped_sensor_read(system->sensors[i], 
                                                     min_values[i], 
                                                     max_values[i]);
        if (reading.is_valid) {
            printf("  ✓ %s: %.2f %s\n", 
                   system->sensors[i]->name, reading.value, reading.unit);
        } else {
            printf("  ✗ %s: 데이터 오류\n", system->sensors[i]->name);
        }
    }
    printf("\n");
}

void monitoring_system_stop(MonitoringSystem* system) {
    printf("╔════════════════════════════════════════╗\n");
    printf("║   센서 모니터링 시스템 종료            ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    for (int i = 0; i < system->sensor_count; i++) {
        wrapped_sensor_close(system->sensors[i]);
        destroy_sensor(system->sensors[i]);
    }
    
    system->monitoring_active = 0;
}

void destroy_monitoring_system(MonitoringSystem* system) {
    if (system != NULL) {
        if (system->sensors != NULL) {
            free(system->sensors);
        }
        free(system);
    }
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                                                            ║\n");
    printf("║     Wrapper + Adapter 패턴 통합 예제                      ║\n");
    printf("║     센서 모니터링 시스템                                  ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // 모니터링 시스템 생성
    MonitoringSystem* system = create_monitoring_system(3);
    
    // 다양한 센서 추가
    monitoring_system_add_sensor(system, 0, create_temp_sensor(101));
    monitoring_system_add_sensor(system, 1, create_humidity_sensor(201));
    monitoring_system_add_sensor(system, 2, create_pressure_sensor(301));
    
    // 시스템 시작
    monitoring_system_start(system);
    
    // 5회 데이터 수집
    for (int i = 1; i <= 5; i++) {
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("  수집 사이클 #%d\n", i);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        monitoring_system_collect_data(system);
    }
    
    // 시스템 종료
    monitoring_system_stop(system);
    
    // 메모리 정리
    destroy_monitoring_system(system);
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  패턴 적용 요약:                                           ║\n");
    printf("║  1. Adapter: 서로 다른 센서 인터페이스를 통일             ║\n");
    printf("║  2. Wrapper: 로깅, 검증, 에러 처리 기능 추가              ║\n");
    printf("║  3. 결과: 확장 가능하고 유지보수가 쉬운 시스템            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    return 0;
}

/*
 * ============================================================================
 * Wrapper + Adapter 패턴 통합의 장점 총정리
 * ============================================================================
 * 
 * 1. Adapter의 장점 (인터페이스 통일):
 *    - 서로 다른 하드웨어/라이브러리를 통일된 인터페이스로 사용
 *    - 새로운 센서/드라이버 추가가 용이
 *    - 기존 코드 수정 없이 확장 가능
 *    - 멀티 벤더 환경에 강함
 * 
 * 2. Wrapper의 장점 (품질 보장):
 *    - 중앙화된 로깅 및 모니터링
 *    - 일관된 에러 처리
 *    - 데이터 검증 및 필터링
 *    - 디버깅 및 문제 추적 용이
 *    - 의미 있는 인터페이스 제공
 * 
 * 3. 통합의 시너지 효과:
 *    - Adapter로 다양성 흡수
 *    - Wrapper로 품질 보장
 *    - 계층 분리로 책임 명확화
 *    - 유지보수성과 확장성 극대화
 *    - 테스트 가능한 구조
 *    - 실무에서 즉시 적용 가능한 구조
 * 
 * ============================================================================
 * 실전 임베디드 FW 개발 적용 사례
 * ============================================================================
 * 
 * 1. FOTA (Firmware Over The Air) 시스템
 *    [Application]
 *      FOTA_Update()
 *    [Wrapper]
 *      FOTA_Receive() - 로깅, 검증, 에러 처리
 *    [Adapter]
 *      UART_Adapter, CAN_Adapter, USB_Adapter - 통신 방식 통일
 *    [Driver]
 *      HAL_UART, CAN_Driver, USB_CDC - 실제 통신
 * 
 * 2. 센서 모니터링 시스템 (이 예제!)
 *    [Application]
 *      MonitoringSystem
 *    [Wrapper]
 *      wrapped_sensor_read() - 로깅, 범위 검증
 *    [Adapter]
 *      temp_adapter, humidity_adapter, pressure_adapter - 인터페이스 통일
 *    [Driver]
 *      TempSensor (화씨), HumiditySensor (신규), PressureSensor (kPa)
 * 
 * 3. 통신 프로토콜 스택
 *    [Application]
 *      Network_Send(data)
 *    [Wrapper]
 *      Network_Write() - 로깅, 재전송, 타임아웃 처리
 *    [Adapter]
 *      Ethernet_Adapter, WiFi_Adapter, Cellular_Adapter
 *    [Driver]
 *      ETH_HAL, ESP32_WiFi, 4G_Modem
 * 
 * 4. Flash 메모리 관리
 *    [Application]
 *      Config_Save()
 *    [Wrapper]
 *      Flash_Write() - 로깅, 검증, 에러 복구
 *    [Adapter]
 *      InternalFlash_Adapter, ExternalFlash_Adapter, EEPROM_Adapter
 *    [Driver]
 *      STM32_Flash, W25Q128, AT24C256
 * 
 * 5. 모터 제어 시스템
 *    [Application]
 *      Motor_SetSpeed(speed)
 *    [Wrapper]
 *      Motor_Control() - 로깅, 안전 범위 체크, 램핑
 *    [Adapter]
 *      PWM_Adapter, CAN_Motor_Adapter, I2C_Motor_Adapter
 *    [Driver]
 *      PWM_Driver, CAN_Motor, PCA9685
 * 
 * ============================================================================
 * 계층별 코드 작성 가이드
 * ============================================================================
 * 
 * [Application Layer - 앱 계층]
 *   역할: 비즈니스 로직
 *   예시:
 *     if (temperature > 30.0f) {
 *         LED_On();
 *         Alert_Send("온도 과열");
 *     }
 *   
 *   작성 시 주의:
 *     ✅ Wrapper만 호출
 *     ❌ Driver나 HAL 직접 호출 금지
 *     ❌ 하드웨어 의존적인 코드 금지
 * 
 * [Wrapper Layer - 래퍼 계층]
 *   역할: 정책, 로깅, 검증, 에러 처리
 *   예시:
 *     float Sensor_Read(Sensor *s) {
 *         Log("[SENSOR] Reading...");
 *         float val = s->read(s);  // Adapter 호출
 *         if (val < MIN || val > MAX) {
 *             Log("[SENSOR] Out of range!");
 *             return ERROR;
 *         }
 *         return val;
 *     }
 *   
 *   작성 시 주의:
 *     ✅ 로깅, 검증, 에러 처리
 *     ✅ 의미 있는 함수 이름
 *     ❌ 하드웨어 직접 제어 금지
 *     ❌ 비대한 로직 금지 (얇게 유지)
 * 
 * [Adapter Layer - 어댑터 계층]
 *   역할: 인터페이스 형식 변환
 *   예시:
 *     float temp_adapter_read(Sensor *s) {
 *         float fahrenheit = old_sensor_read_f(s->device);
 *         return (fahrenheit - 32.0f) * 5.0f / 9.0f;  // 화씨→섭씨
 *     }
 *   
 *   작성 시 주의:
 *     ✅ 단순한 변환만
 *     ✅ 타입 캐스팅, 단위 변환
 *     ❌ 로직, 상태, 정책 금지
 *     ❌ 메모리 할당 최소화
 * 
 * [Driver/HAL Layer - 드라이버 계층]
 *   역할: 하드웨어 직접 제어
 *   예시:
 *     int HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *buf, 
 *                          uint16_t len, uint32_t timeout);
 *   
 *   작성 시 주의:
 *     ✅ 하드웨어 레지스터 접근
 *     ✅ 실제 I/O 동작
 *     ❌ 비즈니스 로직 금지
 * 
 * ============================================================================
 * 이 구조의 핵심 가치
 * ============================================================================
 * 
 * 1. 변경에 강함:
 *    - 하드웨어 교체: Adapter만 수정
 *    - 정책 변경: Wrapper만 수정
 *    - 기능 추가: Application만 수정
 * 
 * 2. 테스트 가능:
 *    - Mock Driver 작성 쉬움
 *    - 각 계층 독립 테스트 가능
 *    - 통합 테스트도 용이
 * 
 * 3. 유지보수 쉬움:
 *    - 책임이 명확히 분리됨
 *    - 버그 위치 찾기 쉬움
 *    - 새 개발자 온보딩 용이
 * 
 * 4. 확장성:
 *    - 새 드라이버 추가: Adapter 하나만 추가
 *    - 기존 코드 영향 없음
 *    - 점진적 개선 가능
 * 
 * ============================================================================
 * 면접 / 기술 면담 완벽 대응 (실전 답변)
 * ============================================================================
 * 
 * Q: Wrapper와 Adapter를 함께 쓴 경험이 있나요?
 * A: "네, FOTA 업데이트 시스템을 구축할 때 사용했습니다.
 *     
 *     먼저 Adapter 계층에서 UART, CAN, USB 통신을 공통 인터페이스로
 *     통일했고, Wrapper 계층에서 로깅, 재전송, 타임아웃 처리를 추가했습니다.
 *     
 *     덕분에 상위 FOTA 로직은 통신 방식과 완전히 독립되었고,
 *     새로운 통신 방식 추가도 Adapter만 구현하면 됐습니다."
 * 
 * Q: 계층을 나누는 기준은?
 * A: "역할에 따라 명확히 구분합니다.
 *     
 *     Adapter는 '형식 변환'만, Wrapper는 '정책과 부가 기능'만,
 *     Application은 '비즈니스 로직'만 담당합니다.
 *     
 *     이렇게 하면 변경 사항이 생겨도 해당 계층만 수정하면 되고,
 *     다른 계층에 영향을 주지 않습니다."
 * 
 * Q: 실무에서 가장 큰 효과는?
 * A: "하드웨어 교체 시 개발 시간이 획기적으로 줄었습니다.
 *     
 *     예를 들어 MCU를 STM32에서 Nordic으로 바꿀 때,
 *     Adapter 계층만 재작성하고 상위 로직은 전혀 수정하지 않았습니다.
 *     
 *     또한 단위 테스트가 쉬워져서 버그도 크게 줄었습니다."
 * 
 * Q: 주의할 점은?
 * A: "계층 건너뛰기를 절대 하면 안 됩니다.
 *     
 *     Application에서 Driver를 직접 호출하거나,
 *     Wrapper에서 HAL을 직접 쓰면 계층 구조가 무너집니다.
 *     
 *     또한 Adapter에 로직을 넣으면 안 되고, 순수하게 변환만
 *     해야 합니다. 얇고 단순하게 유지하는 것이 핵심입니다."
 * 
 * ============================================================================
 * 다음 학습 단계 추천
 * ============================================================================
 * 
 * 1. 직접 실습:
 *    - 자신의 프로젝트에 이 구조 적용해보기
 *    - UART/CAN/USB 통합 통신 계층 구현
 *    - 센서 통합 시스템 구축
 * 
 * 2. 고급 패턴:
 *    - Factory 패턴 추가 (센서 생성 자동화)
 *    - Observer 패턴 (이벤트 기반 모니터링)
 *    - Strategy 패턴 (알고리즘 교체)
 * 
 * 3. 실전 프로젝트:
 *    - RTOS 추상화 계층
 *    - 멀티 프로토콜 통신 스택
 *    - HAL 이식성 계층
 * 
 * 4. 테스트:
 *    - Unit Test 작성
 *    - Mock Driver 구현
 *    - 통합 테스트 자동화
 * 
 * ============================================================================
 * 이 파일을 완전히 이해했다면
 * ============================================================================
 * 
 * ✅ 다층 아키텍처를 설계할 수 있다
 * ✅ 각 계층의 역할을 명확히 구분할 수 있다
 * ✅ 변경에 강한 시스템을 만들 수 있다
 * ✅ 실무 프로젝트에 바로 적용할 수 있다
 * ✅ 면접에서 자신 있게 설명할 수 있다
 * 
 * 👉 당신은 이제 임베디드 시스템 아키텍트 수준입니다!
 * 
 * ============================================================================
 */
