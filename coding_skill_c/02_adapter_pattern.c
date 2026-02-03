/*
 * ============================================================================
 * 파일명: 02_adapter_pattern.c
 * 기능: C언어 Adapter(어댑터) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Adapter 패턴이란?
 * ============================================================================
 * 
 * 이미 존재하는 인터페이스를
 * 내가 원하는 인터페이스로 "변환"해주는 패턴
 * 
 * 핵심 키워드:
 *   - 변환 (Translate)
 *   - 호환 (Compatibility)
 *   - 기존 코드 유지
 * 
 * 구조:
 *   [기존 모듈 A] ──❌──> [내 코드]
 *           │
 *        Adapter
 *           │
 *   [내가 원하는 인터페이스]
 * 
 * ============================================================================
 * 2️⃣ Wrapper vs Adapter (핵심 차이)
 * ============================================================================
 * 
 * 구분        | Wrapper              | Adapter
 * ------------|----------------------|------------------------
 * 목적        | 감싸서 숨김          | 형식 변환
 * 인터페이스  | 새로 정의            | 기존 ↔ 새 인터페이스 연결
 * 사용 이유   | 추상화               | 호환성
 * HAL 사용    | HAL 숨김             | HAL 형식 맞춤
 * 비유        | 포장                 | 변환 젠더
 * 
 * 📌 Adapter는 "이미 존재하는 인터페이스가 마음에 안 들 때" 쓴다
 * 
 * ============================================================================
 * 3️⃣ 언제 Adapter를 쓰는가? (임베디드 현실 예)
 * ============================================================================
 * 
 * ✅ 이런 상황이면 Adapter:
 * 
 *   - UART 드라이버 A / B가 인터페이스 다름
 *   - CAN 스택 회사마다 API 다름
 *   - 외부 라이브러리 API 변경 불가
 *   - 기존 프로젝트 코드 유지해야 함
 *   - HAL ↔ Legacy Driver 혼용
 * 
 * ============================================================================
 * 4️⃣ 가장 직관적인 Adapter 예제
 * ============================================================================
 * 
 * 상황:
 *   기존 UART 드라이버:
 *       int legacy_uart_recv(uint8_t *buf, int size);
 * 
 *   새 프로젝트 표준 인터페이스:
 *       int Comm_Read(uint8_t *buf, uint32_t len);
 * 
 * ============================================================================
 * 5️⃣ Adapter 구현
 * ============================================================================
 * 
 * adapter_uart.c:
 * 
 *   #include "comm.h"
 *   #include "legacy_uart.h"
 * 
 *   int Comm_Read(uint8_t *buf, uint32_t len)
 *   {
 *       return legacy_uart_recv(buf, (int)len);
 *   }
 * 
 * 📌 기존 코드 수정 ❌
 *    새 인터페이스에 맞게 "변환"만
 * 
 * ============================================================================
 * 6️⃣ 구조체 + 함수 포인터 Adapter (실전 핵심)
 * ============================================================================
 * 
 * 공통 인터페이스:
 *   typedef struct {
 *       int (*read)(uint8_t *buf, uint32_t len);
 *       int (*write)(uint8_t *buf, uint32_t len);
 *   } CommIf;
 * 
 * UART Adapter:
 *   int uart_adapter_read(uint8_t *buf, uint32_t len)
 *   {
 *       return HAL_UART_Receive(&huart1, buf, len, 1000);
 *   }
 * 
 * CAN Adapter:
 *   int can_adapter_read(uint8_t *buf, uint32_t len)
 *   {
 *       return CAN_ReceiveFrame(buf, len);
 *   }
 * 
 * 사용부 (변경 없음!):
 *   CommIf *comm = &uart_comm;
 *   comm->read(rx_buf, 128);
 * 
 * 📌 이게 바로 C에서 Adapter의 정석 구현
 * 
 * ============================================================================
 * 7️⃣ Wrapper + Adapter 같이 쓰는 구조 (중요)
 * ============================================================================
 * 
 * 실제 프로젝트는 단독 사용 거의 없음
 * 
 *   [Application]
 *        ↓
 *   [Wrapper]      ← 정책 / 의미
 *        ↓
 *   [Adapter]      ← 형식 변환
 *        ↓
 *   [Driver/HAL]
 * 
 * 예: FOTA
 *   - Wrapper: FOTA_Receive()
 *   - Adapter: UART ↔ CAN 수신 방식 통일
 * 
 * ============================================================================
 * 8️⃣ 잘못된 Adapter 설계 (주의)
 * ============================================================================
 * 
 * ❌ Adapter 안에 로직, 상태, 정책 포함
 * ❌ Adapter에서 메모리 할당
 * ❌ 여러 인터페이스를 하나로 뭉침
 * 
 * 👉 Adapter는 "얇고 단순"해야 합니다
 * 
 * ============================================================================
 * 9️⃣ 이렇게 말하면 강력함
 * ============================================================================
 * 
 * "Wrapper는 변경 가능성을 가두는 용도고,
 *  Adapter는 기존 인터페이스를 새로운 규약에 맞게 변환하는 용도로 사용했습니다."
 * 
 * 또는
 * 
 * "MCU나 통신 스택이 달라져도
 *  상위 로직은 공통 인터페이스만 바라보도록 Adapter 계층을 분리했습니다."
 * 
 * ============================================================================
 * 🔚 한 줄 요약
 * ============================================================================
 * 
 * Adapter 패턴은
 * '바꿀 수 없는 코드를, 내가 원하는 형태로 맞추는 기술'이다.
 * 
 * ============================================================================
 * 이 예제 파일의 구조
 * ============================================================================
 * 
 * 시나리오:
 *   - 서로 다른 온도 센서 라이브러리들이 있음 (레거시 센서, 신규 센서)
 *   - 각 센서는 다른 인터페이스와 데이터 형식을 사용
 *   - Adapter를 통해 통일된 인터페이스로 모든 센서를 사용
 * 
 * 구조:
 *   1. 통일된 인터페이스 정의 (타겟 인터페이스)
 *   2. 레거시 센서 라이브러리 (호환되지 않는 인터페이스 - 화씨 온도)
 *   3. 신규 센서 라이브러리 (다른 형식의 인터페이스 - 켈빈 온도)
 *   4. Adapter 구현 (각 센서를 통일된 인터페이스로 변환 - 섭씨 통일)
 *   5. 사용 예제 (모든 센서를 동일한 방식으로 사용)
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * 1. 통일된 인터페이스 정의 (타겟 인터페이스)
 * - 모든 센서가 이 인터페이스를 따르도록 할 것
 * ============================================================================ */

// 센서 데이터 구조체 (표준화된 형식)
typedef struct {
    float temperature;      // 섭씨 온도
    int sensor_id;         // 센서 ID
    time_t timestamp;      // 측정 시간
    char status[32];       // 센서 상태
} SensorData;

// 센서 인터페이스 (함수 포인터를 포함한 구조체)
typedef struct Sensor {
    void* device;          // 실제 센서 장치 포인터
    SensorData (*read)(struct Sensor* self);              // 데이터 읽기
    int (*initialize)(struct Sensor* self);               // 초기화
    void (*shutdown)(struct Sensor* self);                // 종료
    char name[64];         // 센서 이름
} Sensor;

/* ============================================================================
 * 2. 레거시 센서 라이브러리 (Old Sensor)
 * - 화씨 온도를 사용
 * - 다른 함수 명명 규칙
 * - 다른 데이터 구조
 * ============================================================================ */

// 레거시 센서 장치 구조체
typedef struct {
    int device_id;
    int is_active;
} OldSensorDevice;

// 레거시 센서 초기화
int old_sensor_init(OldSensorDevice* device, int id) {
    device->device_id = id;
    device->is_active = 1;
    printf("[레거시 센서 %d] 초기화 완료\n", id);
    return 0;
}

// 레거시 센서 데이터 읽기 (화씨 온도 반환)
float old_sensor_get_fahrenheit(OldSensorDevice* device) {
    if (!device->is_active) {
        return -999.0f;
    }
    // 시뮬레이션: 68°F ~ 86°F (20°C ~ 30°C)
    return 68.0f + (rand() % 180) / 10.0f;
}

// 레거시 센서 상태 확인
int old_sensor_check_status(OldSensorDevice* device) {
    return device->is_active;
}

// 레거시 센서 종료
void old_sensor_close(OldSensorDevice* device) {
    device->is_active = 0;
    printf("[레거시 센서 %d] 종료\n", device->device_id);
}

/* ============================================================================
 * 3. 신규 센서 라이브러리 (New Sensor)
 * - 섭씨 온도를 사용하지만 켈빈 단위로 반환
 * - 다른 함수 명명 규칙
 * - 에러 코드 반환 방식
 * ============================================================================ */

// 신규 센서 장치 구조체
typedef struct {
    unsigned int id;
    char state[16];
} NewSensorDevice;

// 신규 센서 시작
int new_sensor_start(NewSensorDevice* device, unsigned int id) {
    device->id = id;
    strcpy(device->state, "RUNNING");
    printf("[신규 센서 %u] 시작 완료\n", id);
    return 1;  // 성공
}

// 신규 센서 데이터 읽기 (켈빈 온도 반환, 에러 코드 포인터로 전달)
double new_sensor_read_kelvin(NewSensorDevice* device, int* error_code) {
    if (strcmp(device->state, "RUNNING") != 0) {
        *error_code = -1;
        return 0.0;
    }
    *error_code = 0;
    // 시뮬레이션: 293.15K ~ 303.15K (20°C ~ 30°C)
    return 293.15 + (rand() % 100) / 10.0;
}

// 신규 센서 상태 문자열 가져오기
const char* new_sensor_get_state(NewSensorDevice* device) {
    return device->state;
}

// 신규 센서 정지
void new_sensor_stop(NewSensorDevice* device) {
    strcpy(device->state, "STOPPED");
    printf("[신규 센서 %u] 정지\n", device->id);
}

/* ============================================================================
 * 4. Adapter 구현
 * - 레거시 센서와 신규 센서를 통일된 인터페이스로 변환
 * ============================================================================ */

// 레거시 센서 Adapter - 데이터 읽기
SensorData old_sensor_adapter_read(Sensor* sensor) {
    OldSensorDevice* device = (OldSensorDevice*)sensor->device;
    SensorData data;
    
    // 화씨를 섭씨로 변환
    float fahrenheit = old_sensor_get_fahrenheit(device);
    data.temperature = (fahrenheit - 32.0f) * 5.0f / 9.0f;
    data.sensor_id = device->device_id;
    data.timestamp = time(NULL);
    
    if (old_sensor_check_status(device)) {
        strcpy(data.status, "ACTIVE");
    } else {
        strcpy(data.status, "INACTIVE");
    }
    
    return data;
}

// 레거시 센서 Adapter - 초기화
int old_sensor_adapter_init(Sensor* sensor) {
    OldSensorDevice* device = (OldSensorDevice*)sensor->device;
    return old_sensor_init(device, device->device_id);
}

// 레거시 센서 Adapter - 종료
void old_sensor_adapter_shutdown(Sensor* sensor) {
    OldSensorDevice* device = (OldSensorDevice*)sensor->device;
    old_sensor_close(device);
}

// 신규 센서 Adapter - 데이터 읽기
SensorData new_sensor_adapter_read(Sensor* sensor) {
    NewSensorDevice* device = (NewSensorDevice*)sensor->device;
    SensorData data;
    
    int error_code;
    // 켈빈을 섭씨로 변환
    double kelvin = new_sensor_read_kelvin(device, &error_code);
    
    if (error_code == 0) {
        data.temperature = (float)(kelvin - 273.15);
        data.sensor_id = (int)device->id;
        data.timestamp = time(NULL);
        strcpy(data.status, new_sensor_get_state(device));
    } else {
        data.temperature = -999.0f;
        data.sensor_id = (int)device->id;
        data.timestamp = time(NULL);
        strcpy(data.status, "ERROR");
    }
    
    return data;
}

// 신규 센서 Adapter - 초기화
int new_sensor_adapter_init(Sensor* sensor) {
    NewSensorDevice* device = (NewSensorDevice*)sensor->device;
    return new_sensor_start(device, device->id);
}

// 신규 센서 Adapter - 종료
void new_sensor_adapter_shutdown(Sensor* sensor) {
    NewSensorDevice* device = (NewSensorDevice*)sensor->device;
    new_sensor_stop(device);
}

// 레거시 센서 Adapter 생성 함수
Sensor* create_old_sensor_adapter(int sensor_id) {
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    OldSensorDevice* device = (OldSensorDevice*)malloc(sizeof(OldSensorDevice));
    
    device->device_id = sensor_id;
    device->is_active = 0;
    
    sensor->device = device;
    sensor->read = old_sensor_adapter_read;
    sensor->initialize = old_sensor_adapter_init;
    sensor->shutdown = old_sensor_adapter_shutdown;
    snprintf(sensor->name, sizeof(sensor->name), "Old Sensor #%d", sensor_id);
    
    return sensor;
}

// 신규 센서 Adapter 생성 함수
Sensor* create_new_sensor_adapter(unsigned int sensor_id) {
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    NewSensorDevice* device = (NewSensorDevice*)malloc(sizeof(NewSensorDevice));
    
    device->id = sensor_id;
    strcpy(device->state, "IDLE");
    
    sensor->device = device;
    sensor->read = new_sensor_adapter_read;
    sensor->initialize = new_sensor_adapter_init;
    sensor->shutdown = new_sensor_adapter_shutdown;
    snprintf(sensor->name, sizeof(sensor->name), "New Sensor #%u", sensor_id);
    
    return sensor;
}

// 센서 제거 함수
void destroy_sensor(Sensor* sensor) {
    if (sensor != NULL) {
        if (sensor->device != NULL) {
            free(sensor->device);
        }
        free(sensor);
    }
}

/* ============================================================================
 * 5. 통일된 인터페이스로 센서 사용하기
 * ============================================================================ */

void print_sensor_data(const char* prefix, SensorData data) {
    char* time_str = ctime(&data.timestamp);
    time_str[strlen(time_str) - 1] = '\0';  // 개행 제거
    
    printf("%s\n", prefix);
    printf("  센서 ID: %d\n", data.sensor_id);
    printf("  온도: %.2f°C\n", data.temperature);
    printf("  상태: %s\n", data.status);
    printf("  측정 시간: %s\n", time_str);
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("========================================\n");
    printf("Adapter 패턴 예제\n");
    printf("========================================\n\n");
    
    // 센서 배열 (통일된 인터페이스)
    Sensor* sensors[4];
    
    // 다양한 센서 생성
    sensors[0] = create_old_sensor_adapter(101);
    sensors[1] = create_old_sensor_adapter(102);
    sensors[2] = create_new_sensor_adapter(201);
    sensors[3] = create_new_sensor_adapter(202);
    
    printf("=== 모든 센서 초기화 ===\n");
    for (int i = 0; i < 4; i++) {
        printf("초기화: %s\n", sensors[i]->name);
        sensors[i]->initialize(sensors[i]);
    }
    
    printf("\n=== 모든 센서에서 데이터 읽기 (통일된 방식) ===\n");
    for (int i = 0; i < 4; i++) {
        SensorData data = sensors[i]->read(sensors[i]);
        print_sensor_data(sensors[i]->name, data);
        printf("\n");
    }
    
    printf("=== 3회 측정 시뮬레이션 ===\n");
    for (int round = 1; round <= 3; round++) {
        printf("--- 측정 %d회차 ---\n", round);
        float total_temp = 0.0f;
        int count = 0;
        
        for (int i = 0; i < 4; i++) {
            SensorData data = sensors[i]->read(sensors[i]);
            printf("%s: %.2f°C\n", sensors[i]->name, data.temperature);
            total_temp += data.temperature;
            count++;
        }
        
        printf("평균 온도: %.2f°C\n\n", total_temp / count);
    }
    
    printf("=== 모든 센서 종료 ===\n");
    for (int i = 0; i < 4; i++) {
        printf("종료: %s\n", sensors[i]->name);
        sensors[i]->shutdown(sensors[i]);
        destroy_sensor(sensors[i]);
    }
    
    printf("\n========================================\n");
    printf("Adapter 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Adapter 패턴의 장점 총정리
 * ============================================================================
 * 
 * 1. 기존 코드를 수정하지 않고 새로운 인터페이스로 사용 가능
 * 2. 서로 다른 라이브러리를 통일된 방식으로 사용 가능
 * 3. 레거시 시스템과 신규 시스템의 통합이 용이
 * 4. 코드의 재사용성과 유지보수성 향상
 * 5. 멀티 벤더 환경에서 강력함
 * 6. 플랫폼 이식성 향상
 * 
 * ============================================================================
 * 임베디드 FW 개발에서 Adapter 패턴의 실전 활용
 * ============================================================================
 * 
 * 1. 서로 다른 드라이버 통합
 *    - UART 드라이버 A, B, C → 공통 Comm 인터페이스
 *    - CAN 스택 (Vector, Microchip, NXP) → 통일된 API
 * 
 * 2. MCU 플랫폼 간 이식
 *    - STM32 HAL ↔ Nordic SDK ↔ ESP32 IDF
 *    - 공통 인터페이스로 상위 앱 코드 재사용
 * 
 * 3. 센서 드라이버 통합
 *    - 온도 센서: Analog, I2C, SPI → 통일된 Sensor 인터페이스
 *    - 제조사별 다른 API → 프로젝트 표준 API로 변환
 * 
 * 4. 통신 프로토콜 추상화
 *    - UART / CAN / USB / Ethernet → CommLayer
 *    - 상위 FOTA 로직은 통신 방식 무관
 * 
 * 5. RTOS 전환
 *    - FreeRTOS → ThreadX → Zephyr
 *    - Adapter 계층으로 앱 코드 보호
 * 
 * 6. 외부 라이브러리 통합
 *    - JSON 파서 A → JSON 파서 B 교체 시
 *    - Crypto 라이브러리 변경 시
 * 
 * ============================================================================
 * Adapter 패턴 vs Wrapper 패턴 - 실전 구분법
 * ============================================================================
 * 
 * 상황                               | 선택
 * -----------------------------------|------------------
 * 인터페이스가 서로 다름             | Adapter
 * 기능은 같은데 숨기고 싶음          | Wrapper
 * MCU 교체 대비                      | Wrapper
 * 드라이버 A/B 통합                  | Adapter
 * 로깅/검증 추가                     | Wrapper
 * HAL API 형식 맞춤                  | Adapter
 * 
 * 📌 실전 팁: 대부분은 Adapter + Wrapper 동시 사용
 * 
 * ============================================================================
 * 질문 / 기술 면담 대응 포인트
 * ============================================================================
 * 
 * Q: Adapter 패턴을 사용한 경험이 있나요?
 * A: "네, 프로젝트에서 여러 제조사의 CAN 스택을 통합할 때 사용했습니다.
 *     Vector와 Microchip의 API가 완전히 달라서 공통 인터페이스를 정의하고
 *     각 스택에 맞는 Adapter를 구현했습니다. 덕분에 CAN 스택을 교체할 때
 *     상위 통신 로직은 전혀 수정하지 않았습니다."
 * 
 * Q: Wrapper와 Adapter를 언제 구분해서 쓰나요?
 * A: "Wrapper는 하나의 구현을 숨기고 간소화할 때, Adapter는 여러 개의
 *     서로 다른 인터페이스를 통합할 때 사용합니다. 실무에서는 Adapter로
 *     인터페이스를 맞춘 후 Wrapper로 추가 기능을 제공하는 구조가 많습니다."
 * 
 * Q: Adapter 설계 시 가장 중요한 것은?
 * A: "Adapter는 최대한 얇고 단순해야 합니다. 로직, 상태, 정책을 넣으면
 *     안 되고, 순수하게 인터페이스 변환만 담당해야 합니다. 복잡도가
 *     증가하면 버그와 유지보수 비용이 급격히 늘어납니다."
 * 
 * ============================================================================
 * 이 예제에서 배운 핵심 패턴
 * ============================================================================
 * 
 * 1. 함수 포인터 기반 인터페이스
 *    → C에서 다형성 구현하는 정석
 * 
 * 2. 타입 변환 (화씨→섭씨, 켈빈→섭씨)
 *    → Adapter의 실제 변환 로직 예제
 * 
 * 3. 통일된 인터페이스로 다양한 구현 사용
 *    → 멀티 벤더 환경의 핵심 기법
 * 
 * 4. 팩토리 패턴과의 조합
 *    → create_xxx_adapter() 함수들
 * 
 * ============================================================================
 * 실전 프로젝트 적용 체크리스트
 * ============================================================================
 * 
 * □ 통합해야 할 라이브러리/드라이버가 2개 이상인가?
 * □ 각각의 인터페이스가 서로 다른가?
 * □ 기존 코드를 수정할 수 없는가?
 * □ 향후 추가 드라이버가 들어올 가능성이 있는가?
 * □ 상위 로직이 특정 드라이버에 의존하면 안 되는가?
 * 
 * 👉 3개 이상 체크되면 Adapter 패턴 적용 추천
 * 
 * ============================================================================
 * 다음 학습 단계 추천
 * ============================================================================
 * 
 * 1. 03_wrapper_adapter_combined.c - 두 패턴 통합 사용
 * 2. CAN/UART/USB 통신 Adapter 구현 실습
 * 3. RTOS 추상화 레이어 설계
 * 4. 센서 드라이버 통합 프로젝트
 * 5. HAL 이식성 향상 설계
 * 
 * ============================================================================
 */
