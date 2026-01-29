/*
 * ============================================================================
 * 파일명: 06_factory_pattern.c
 * 기능: C언어 Factory(팩토리) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Factory 패턴이란?
 * ============================================================================
 * 
 * 객체 생성 로직을 별도의 팩토리 함수로 분리하여
 * 사용자는 생성 방법을 몰라도 객체를 생성할 수 있게 하는 패턴
 * 
 * 핵심 키워드:
 *   - 객체 생성 캡슐화
 *   - 타입별 생성 로직 분리
 *   - 유연한 확장성
 *   - 의존성 감소
 * 
 * 구조:
 *   [사용자] → [Factory] → [객체 A/B/C 생성]
 * 
 * ============================================================================
 * 2️⃣ 왜 Factory를 쓰나?
 * ============================================================================
 * 
 * ✅ Factory의 장점:
 *    - 생성 로직 중앙 관리
 *    - 타입에 따른 생성 분기 제거
 *    - 새 타입 추가 쉬움
 *    - 초기화 로직 통일
 *    - 메모리 관리 일원화
 * 
 * ❌ Factory 없이 구현하면:
 *    - 생성 코드 중복
 *    - if/else로 타입 체크
 *    - 확장 어려움
 * 
 * ============================================================================
 * 3️⃣ 임베디드에서 Factory를 쓰는 경우
 * ============================================================================
 * 
 * 실무 적용 사례:
 *   ✅ 센서 객체 생성 (온도/습도/압력)
 *   ✅ 통신 드라이버 생성 (UART/SPI/I2C)
 *   ✅ 프로토콜 핸들러 생성 (CAN/Modbus/MQTT)
 *   ✅ 메모리 할당자 생성 (Pool/Heap/Static)
 *   ✅ 디스플레이 드라이버 생성 (LCD/OLED/LED)
 * 
 * 👉 "여러 타입의 객체를 생성할 때" Factory 필수
 * 
 * ============================================================================
 * 🔚 한 줄 요약
 * ============================================================================
 * 
 * Factory 패턴은
 * "객체 생성 로직을 캡슐화하여 유연하게 확장 가능하게 하는 기술"이다.
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * 센서 Factory 예제
 * ============================================================================ */

// 센서 타입
typedef enum {
    SENSOR_TYPE_TEMPERATURE,
    SENSOR_TYPE_HUMIDITY,
    SENSOR_TYPE_PRESSURE
} SensorType;

// 공통 센서 인터페이스
typedef struct Sensor {
    SensorType type;
    int sensor_id;
    float (*read)(struct Sensor* self);
    void (*init)(struct Sensor* self);
    void (*destroy)(struct Sensor* self);
    char name[32];
} Sensor;

// 온도 센서 구조체
typedef struct {
    Sensor base;  // 공통 인터페이스
    float offset;
    float scale;
} TemperatureSensor;

// 습도 센서 구조체
typedef struct {
    Sensor base;
    int calibration_value;
} HumiditySensor;

// 압력 센서 구조체
typedef struct {
    Sensor base;
    float sea_level_pressure;
} PressureSensor;

// 온도 센서 함수들
float Temperature_Read(Sensor* self) {
    TemperatureSensor* ts = (TemperatureSensor*)self;
    float raw = 20.0f + (rand() % 100) / 10.0f;
    return raw * ts->scale + ts->offset;
}

void Temperature_Init(Sensor* self) {
    TemperatureSensor* ts = (TemperatureSensor*)self;
    ts->offset = 0.0f;
    ts->scale = 1.0f;
    printf("[Factory] 온도 센서 %d 초기화\n", self->sensor_id);
}

void Temperature_Destroy(Sensor* self) {
    printf("[Factory] 온도 센서 %d 해제\n", self->sensor_id);
    free(self);
}

// 습도 센서 함수들
float Humidity_Read(Sensor* self) {
    HumiditySensor* hs = (HumiditySensor*)self;
    float raw = 30.0f + (rand() % 500) / 10.0f;
    return raw + hs->calibration_value;
}

void Humidity_Init(Sensor* self) {
    HumiditySensor* hs = (HumiditySensor*)self;
    hs->calibration_value = 0;
    printf("[Factory] 습도 센서 %d 초기화\n", self->sensor_id);
}

void Humidity_Destroy(Sensor* self) {
    printf("[Factory] 습도 센서 %d 해제\n", self->sensor_id);
    free(self);
}

// 압력 센서 함수들
float Pressure_Read(Sensor* self) {
    PressureSensor* ps = (PressureSensor*)self;
    return 1013.25f + (rand() % 200) / 10.0f - 10.0f;
}

void Pressure_Init(Sensor* self) {
    PressureSensor* ps = (PressureSensor*)self;
    ps->sea_level_pressure = 1013.25f;
    printf("[Factory] 압력 센서 %d 초기화\n", self->sensor_id);
}

void Pressure_Destroy(Sensor* self) {
    printf("[Factory] 압력 센서 %d 해제\n", self->sensor_id);
    free(self);
}

// ★ Factory 함수 ★
Sensor* SensorFactory_Create(SensorType type, int sensor_id) {
    Sensor* sensor = NULL;
    
    printf("[Factory] 센서 생성 요청: 타입=%d, ID=%d\n", type, sensor_id);
    
    switch (type) {
        case SENSOR_TYPE_TEMPERATURE: {
            TemperatureSensor* ts = (TemperatureSensor*)malloc(sizeof(TemperatureSensor));
            if (ts) {
                ts->base.type = type;
                ts->base.sensor_id = sensor_id;
                ts->base.read = Temperature_Read;
                ts->base.init = Temperature_Init;
                ts->base.destroy = Temperature_Destroy;
                snprintf(ts->base.name, sizeof(ts->base.name), "Temperature_%d", sensor_id);
                sensor = (Sensor*)ts;
            }
            break;
        }
        
        case SENSOR_TYPE_HUMIDITY: {
            HumiditySensor* hs = (HumiditySensor*)malloc(sizeof(HumiditySensor));
            if (hs) {
                hs->base.type = type;
                hs->base.sensor_id = sensor_id;
                hs->base.read = Humidity_Read;
                hs->base.init = Humidity_Init;
                hs->base.destroy = Humidity_Destroy;
                snprintf(hs->base.name, sizeof(hs->base.name), "Humidity_%d", sensor_id);
                sensor = (Sensor*)hs;
            }
            break;
        }
        
        case SENSOR_TYPE_PRESSURE: {
            PressureSensor* ps = (PressureSensor*)malloc(sizeof(PressureSensor));
            if (ps) {
                ps->base.type = type;
                ps->base.sensor_id = sensor_id;
                ps->base.read = Pressure_Read;
                ps->base.init = Pressure_Init;
                ps->base.destroy = Pressure_Destroy;
                snprintf(ps->base.name, sizeof(ps->base.name), "Pressure_%d", sensor_id);
                sensor = (Sensor*)ps;
            }
            break;
        }
        
        default:
            printf("[Factory] 알 수 없는 센서 타입: %d\n", type);
            return NULL;
    }
    
    if (sensor) {
        sensor->init(sensor);
        printf("[Factory] ✓ 센서 생성 완료: %s\n", sensor->name);
    }
    
    return sensor;
}

/* ============================================================================
 * 통신 드라이버 Factory 예제
 * ============================================================================
 */

// 통신 타입
typedef enum {
    COMM_TYPE_UART,
    COMM_TYPE_SPI,
    COMM_TYPE_I2C
} CommType;

// 공통 통신 인터페이스
typedef struct CommDriver {
    CommType type;
    int port;
    int (*send)(struct CommDriver* self, const uint8_t* data, size_t len);
    int (*receive)(struct CommDriver* self, uint8_t* buffer, size_t len);
    void (*destroy)(struct CommDriver* self);
} CommDriver;

// UART 드라이버
typedef struct {
    CommDriver base;
    int baud_rate;
} UartDriver;

int Uart_Send(CommDriver* self, const uint8_t* data, size_t len) {
    UartDriver* uart = (UartDriver*)self;
    printf("[UART%d] 전송: %zu bytes (속도: %d)\n", self->port, len, uart->baud_rate);
    return (int)len;
}

int Uart_Receive(CommDriver* self, uint8_t* buffer, size_t len) {
    printf("[UART%d] 수신: %zu bytes\n", self->port, len);
    return (int)len;
}

void Uart_Destroy(CommDriver* self) {
    printf("[UART%d] 드라이버 해제\n", self->port);
    free(self);
}

// SPI 드라이버
typedef struct {
    CommDriver base;
    int clock_speed;
} SpiDriver;

int Spi_Send(CommDriver* self, const uint8_t* data, size_t len) {
    SpiDriver* spi = (SpiDriver*)self;
    printf("[SPI%d] 전송: %zu bytes (클럭: %d)\n", self->port, len, spi->clock_speed);
    return (int)len;
}

int Spi_Receive(CommDriver* self, uint8_t* buffer, size_t len) {
    printf("[SPI%d] 수신: %zu bytes\n", self->port, len);
    return (int)len;
}

void Spi_Destroy(CommDriver* self) {
    printf("[SPI%d] 드라이버 해제\n", self->port);
    free(self);
}

// I2C 드라이버
typedef struct {
    CommDriver base;
    uint8_t slave_address;
} I2cDriver;

int I2c_Send(CommDriver* self, const uint8_t* data, size_t len) {
    I2cDriver* i2c = (I2cDriver*)self;
    printf("[I2C%d] 전송: %zu bytes (주소: 0x%02X)\n", self->port, len, i2c->slave_address);
    return (int)len;
}

int I2c_Receive(CommDriver* self, uint8_t* buffer, size_t len) {
    printf("[I2C%d] 수신: %zu bytes\n", self->port, len);
    return (int)len;
}

void I2c_Destroy(CommDriver* self) {
    printf("[I2C%d] 드라이버 해제\n", self->port);
    free(self);
}

// ★ 통신 드라이버 Factory ★
CommDriver* CommFactory_Create(CommType type, int port) {
    CommDriver* driver = NULL;
    
    printf("[CommFactory] 드라이버 생성: 타입=%d, 포트=%d\n", type, port);
    
    switch (type) {
        case COMM_TYPE_UART: {
            UartDriver* uart = (UartDriver*)malloc(sizeof(UartDriver));
            if (uart) {
                uart->base.type = type;
                uart->base.port = port;
                uart->base.send = Uart_Send;
                uart->base.receive = Uart_Receive;
                uart->base.destroy = Uart_Destroy;
                uart->baud_rate = 115200;
                driver = (CommDriver*)uart;
                printf("[CommFactory] ✓ UART 드라이버 생성 완료\n");
            }
            break;
        }
        
        case COMM_TYPE_SPI: {
            SpiDriver* spi = (SpiDriver*)malloc(sizeof(SpiDriver));
            if (spi) {
                spi->base.type = type;
                spi->base.port = port;
                spi->base.send = Spi_Send;
                spi->base.receive = Spi_Receive;
                spi->base.destroy = Spi_Destroy;
                spi->clock_speed = 1000000;
                driver = (CommDriver*)spi;
                printf("[CommFactory] ✓ SPI 드라이버 생성 완료\n");
            }
            break;
        }
        
        case COMM_TYPE_I2C: {
            I2cDriver* i2c = (I2cDriver*)malloc(sizeof(I2cDriver));
            if (i2c) {
                i2c->base.type = type;
                i2c->base.port = port;
                i2c->base.send = I2c_Send;
                i2c->base.receive = I2c_Receive;
                i2c->base.destroy = I2c_Destroy;
                i2c->slave_address = 0x50;
                driver = (CommDriver*)i2c;
                printf("[CommFactory] ✓ I2C 드라이버 생성 완료\n");
            }
            break;
        }
        
        default:
            printf("[CommFactory] 알 수 없는 통신 타입: %d\n", type);
            return NULL;
    }
    
    return driver;
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("========================================\n");
    printf("Factory 패턴 예제\n");
    printf("========================================\n\n");
    
    // 1. 센서 Factory 사용
    printf("=== 1. 센서 Factory ===\n");
    Sensor* sensors[5];
    
    sensors[0] = SensorFactory_Create(SENSOR_TYPE_TEMPERATURE, 101);
    sensors[1] = SensorFactory_Create(SENSOR_TYPE_TEMPERATURE, 102);
    sensors[2] = SensorFactory_Create(SENSOR_TYPE_HUMIDITY, 201);
    sensors[3] = SensorFactory_Create(SENSOR_TYPE_PRESSURE, 301);
    sensors[4] = SensorFactory_Create(SENSOR_TYPE_PRESSURE, 302);
    
    printf("\n센서 데이터 읽기:\n");
    for (int i = 0; i < 5; i++) {
        if (sensors[i]) {
            float value = sensors[i]->read(sensors[i]);
            printf("  %s: %.2f\n", sensors[i]->name, value);
        }
    }
    
    printf("\n센서 해제:\n");
    for (int i = 0; i < 5; i++) {
        if (sensors[i]) {
            sensors[i]->destroy(sensors[i]);
        }
    }
    
    printf("\n");
    
    // 2. 통신 드라이버 Factory 사용
    printf("=== 2. 통신 드라이버 Factory ===\n");
    CommDriver* drivers[3];
    
    drivers[0] = CommFactory_Create(COMM_TYPE_UART, 1);
    drivers[1] = CommFactory_Create(COMM_TYPE_SPI, 2);
    drivers[2] = CommFactory_Create(COMM_TYPE_I2C, 1);
    
    printf("\n통신 테스트:\n");
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04};
    for (int i = 0; i < 3; i++) {
        if (drivers[i]) {
            drivers[i]->send(drivers[i], test_data, sizeof(test_data));
        }
    }
    
    printf("\n드라이버 해제:\n");
    for (int i = 0; i < 3; i++) {
        if (drivers[i]) {
            drivers[i]->destroy(drivers[i]);
        }
    }
    
    printf("\n========================================\n");
    printf("Factory 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Factory 패턴의 장점 총정리
 * ============================================================================
 * 
 * 1. 객체 생성 로직 중앙 관리
 * 2. 타입별 생성 분기 제거
 * 3. 새 타입 추가 시 Factory만 수정
 * 4. 초기화 로직 통일
 * 5. 메모리 관리 일원화
 * 
 * ============================================================================
 * 임베디드에서 Factory의 실전 활용
 * ============================================================================
 * 
 * 1. 센서 드라이버 생성
 * 2. 통신 프로토콜 핸들러
 * 3. 메모리 할당자
 * 4. 디스플레이 드라이버
 * 5. 파일 시스템 인터페이스
 * 
 * ============================================================================
 * 면접 대응
 * ============================================================================
 * 
 * Q: Factory 패턴을 왜 사용하나요?
 * A: "객체 생성 로직을 한 곳에 모아 관리하면, 새로운 타입 추가 시
 *     Factory만 수정하면 됩니다. 사용자 코드는 변경 없이
 *     새 타입을 사용할 수 있어 유지보수가 쉬워집니다."
 * 
 * ============================================================================
 */
