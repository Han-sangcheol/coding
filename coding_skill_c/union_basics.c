/*
 * ============================================================================
 * 파일명: union_basics.c
 * 주제: Union 완전 이해 (메모리 공유의 모든 것)
 * 컴파일: gcc union_basics.c -o union_basics
 * 
 * ============================================================================
 * 이 파일의 기능
 * ============================================================================
 * 
 * [Union 기초]
 * - Union vs Struct 비교
 * - 메모리 레이아웃 시각화
 * - Union의 크기 계산
 * - 멤버 접근 및 메모리 공유 이해
 * 
 * [실전 활용]
 * - 타입 변환 (Type Punning)
 * - 데이터 파싱 (바이트 단위 접근)
 * - 메모리 절약 (다양한 타입 중 하나만 사용)
 * - 임베디드 활용 (레지스터 접근, 프로토콜 파싱)
 * 
 * ============================================================================
 * Union이란?
 * ============================================================================
 * 
 * 여러 멤버가 **같은 메모리 공간을 공유**하는 자료형
 * 
 * Struct: 각 멤버가 별도 메모리 차지
 * Union:  모든 멤버가 같은 메모리 공유
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ============================================================================
 * 예제 1: Struct vs Union 기본 비교
 * ============================================================================ */

void Example1_BasicComparison(void) {
    printf("\n");
    printf("========================================\n");
    printf("예제 1: Struct vs Union 기본 비교\n");
    printf("========================================\n");
    
    // ===== Struct 정의 =====
    struct MyStruct {
        char c;      // 1 byte
        int i;       // 4 bytes
        float f;     // 4 bytes
    };
    
    // ===== Union 정의 =====
    union MyUnion {
        char c;      // 1 byte
        int i;       // 4 bytes
        float f;     // 4 bytes
    };
    
    printf("\n[크기 비교]\n");
    printf("  sizeof(struct MyStruct) = %zu bytes\n", sizeof(struct MyStruct));
    printf("  sizeof(union MyUnion)   = %zu bytes\n", sizeof(union MyUnion));
    
    printf("\n[설명]\n");
    printf("  Struct: 각 멤버가 별도 메모리 → 1 + 패딩 + 4 + 4 = 12 bytes\n");
    printf("  Union:  모든 멤버가 같은 메모리 공유 → 가장 큰 멤버 크기 = 4 bytes\n");
    
    // ===== Struct 사용 =====
    struct MyStruct s;
    s.c = 'A';
    s.i = 100;
    s.f = 3.14f;
    
    printf("\n[Struct 사용]\n");
    printf("  s.c = '%c'\n", s.c);
    printf("  s.i = %d\n", s.i);
    printf("  s.f = %.2f\n", s.f);
    printf("  → 모든 값이 독립적으로 저장됨!\n");
    
    // ===== Union 사용 =====
    union MyUnion u;
    
    u.c = 'A';
    printf("\n[Union 사용 - 1단계]\n");
    printf("  u.c = '%c' (0x%02X)\n", u.c, (unsigned char)u.c);
    printf("  u.i = %d (쓰레기 값)\n", u.i);
    printf("  u.f = %.2f (쓰레기 값)\n", u.f);
    
    u.i = 100;
    printf("\n[Union 사용 - 2단계]\n");
    printf("  u.i = %d\n", u.i);
    printf("  u.c = '%c' (0x%02X) ← 덮어써짐!\n", u.c, (unsigned char)u.c);
    printf("  u.f = %.2e (쓰레기 값)\n", u.f);
    
    u.f = 3.14f;
    printf("\n[Union 사용 - 3단계]\n");
    printf("  u.f = %.2f\n", u.f);
    printf("  u.c = '%c' (0x%02X) ← 또 덮어써짐!\n", u.c, (unsigned char)u.c);
    printf("  u.i = %d (쓰레기 값)\n", u.i);
    
    printf("\n[결론]\n");
    printf("  Union은 한 번에 하나의 멤버만 유효한 값을 가짐!\n");
    printf("  마지막으로 쓴 멤버만 정확한 값을 읽을 수 있음.\n");
}

/* ============================================================================
 * 예제 2: 메모리 레이아웃 시각화
 * ============================================================================ */

void Example2_MemoryLayout(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 2: 메모리 레이아웃 시각화\n");
    printf("========================================\n");
    
    union MyUnion {
        char c;
        int i;
        float f;
    };
    
    union MyUnion u;
    
    printf("\n[Union 주소 정보]\n");
    printf("  Union 시작 주소:    %p\n", (void*)&u);
    printf("  u.c 주소:           %p (offset: %zu)\n", (void*)&u.c, (char*)&u.c - (char*)&u);
    printf("  u.i 주소:           %p (offset: %zu)\n", (void*)&u.i, (char*)&u.i - (char*)&u);
    printf("  u.f 주소:           %p (offset: %zu)\n", (void*)&u.f, (char*)&u.f - (char*)&u);
    
    printf("\n[중요!] 모든 멤버의 주소가 같음! → 같은 메모리를 공유\n");
    
    // 메모리에 값 쓰기
    u.i = 0x12345678;
    
    printf("\n[메모리 내용 시각화]\n");
    printf("  u.i = 0x%08X 로 설정\n", u.i);
    printf("\n  메모리 레이아웃 (리틀 엔디안):\n");
    printf("  주소          값        멤버\n");
    printf("  ----------  ------  ----------\n");
    
    unsigned char* ptr = (unsigned char*)&u;
    for (int i = 0; i < 4; i++) {
        printf("  %p   0x%02X    ", (void*)(ptr + i), ptr[i]);
        if (i == 0) printf("u.c");
        printf("\n");
    }
    
    printf("\n  u.c = 0x%02X (마지막 바이트만 읽음)\n", (unsigned char)u.c);
    printf("  u.i = 0x%08X (전체 4바이트 읽음)\n", u.i);
    printf("  u.f = %.2e (4바이트를 float로 해석)\n", u.f);
}

/* ============================================================================
 * 예제 3: 실전 활용 1 - 바이트 단위 접근 (Type Punning)
 * ============================================================================ */

void Example3_ByteAccess(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 3: 바이트 단위 접근\n");
    printf("========================================\n");
    
    // int를 바이트 단위로 접근하기
    union IntBytes {
        int32_t value;           // 4 bytes
        uint8_t bytes[4];        // 4 bytes (배열)
    };
    
    union IntBytes data;
    data.value = 0x12345678;
    
    printf("\n[활용 사례: int를 바이트 단위로 분해]\n");
    printf("  data.value = 0x%08X\n", data.value);
    printf("\n  바이트 분해 (리틀 엔디안):\n");
    printf("    bytes[0] = 0x%02X (LSB)\n", data.bytes[0]);
    printf("    bytes[1] = 0x%02X\n", data.bytes[1]);
    printf("    bytes[2] = 0x%02X\n", data.bytes[2]);
    printf("    bytes[3] = 0x%02X (MSB)\n", data.bytes[3]);
    
    printf("\n[역변환: 바이트에서 int로]\n");
    data.bytes[0] = 0xAA;
    data.bytes[1] = 0xBB;
    data.bytes[2] = 0xCC;
    data.bytes[3] = 0xDD;
    printf("  bytes[] = {0xAA, 0xBB, 0xCC, 0xDD}\n");
    printf("  → data.value = 0x%08X\n", data.value);
    
    printf("\n[임베디드 활용]\n");
    printf("  - UART로 받은 4바이트를 int로 변환\n");
    printf("  - CAN 메시지 파싱\n");
    printf("  - 네트워크 패킷 분해/조립\n");
}

/* ============================================================================
 * 예제 4: 실전 활용 2 - 다양한 데이터 타입 처리
 * ============================================================================ */

void Example4_MultipleTypes(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 4: 다양한 데이터 타입 처리\n");
    printf("========================================\n");
    
    // 센서 데이터를 여러 타입으로 표현
    typedef enum {
        DATA_TYPE_INT,
        DATA_TYPE_FLOAT,
        DATA_TYPE_STRING
    } DataType;
    
    typedef struct {
        DataType type;          // 현재 어떤 타입을 사용 중인지
        union {
            int i_value;
            float f_value;
            char s_value[16];
        } data;
    } SensorData;
    
    printf("\n[상황: 센서 데이터가 여러 형태일 수 있음]\n");
    
    // 온도 센서 (float)
    SensorData temp;
    temp.type = DATA_TYPE_FLOAT;
    temp.data.f_value = 25.5f;
    
    printf("\n  온도 센서:\n");
    printf("    type = FLOAT\n");
    printf("    value = %.1f°C\n", temp.data.f_value);
    
    // 카운터 센서 (int)
    SensorData counter;
    counter.type = DATA_TYPE_INT;
    counter.data.i_value = 12345;
    
    printf("\n  카운터 센서:\n");
    printf("    type = INT\n");
    printf("    value = %d\n", counter.data.i_value);
    
    // 상태 센서 (string)
    SensorData status;
    status.type = DATA_TYPE_STRING;
    strncpy(status.data.s_value, "OK", sizeof(status.data.s_value) - 1);
    
    printf("\n  상태 센서:\n");
    printf("    type = STRING\n");
    printf("    value = \"%s\"\n", status.data.s_value);
    
    printf("\n[메모리 절약]\n");
    printf("  Struct 사용 시: sizeof(int) + sizeof(float) + sizeof(char[16])\n");
    printf("                = 4 + 4 + 16 = 24 bytes\n");
    printf("  Union 사용 시:  MAX(4, 4, 16) = 16 bytes\n");
    printf("  실제 크기:      %zu bytes (type 포함)\n", sizeof(SensorData));
    printf("  → 메모리 절약!\n");
}

/* ============================================================================
 * 예제 5: 실전 활용 3 - 레지스터 접근 (임베디드)
 * ============================================================================ */

void Example5_RegisterAccess(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 5: 레지스터 접근 (임베디드)\n");
    printf("========================================\n");
    
    // GPIO 레지스터 시뮬레이션
    typedef union {
        uint32_t all;           // 전체 레지스터 (32bit)
        struct {
            uint32_t pin0  : 1;  // bit 0
            uint32_t pin1  : 1;  // bit 1
            uint32_t pin2  : 1;  // bit 2
            uint32_t pin3  : 1;  // bit 3
            uint32_t pin4  : 1;  // bit 4
            uint32_t pin5  : 1;  // bit 5
            uint32_t pin6  : 1;  // bit 6
            uint32_t pin7  : 1;  // bit 7
            uint32_t reserved : 24; // 나머지 비트
        } bits;
        uint8_t bytes[4];       // 바이트 단위 접근
    } GPIO_Register;
    
    GPIO_Register gpio;
    gpio.all = 0x00000000;
    
    printf("\n[상황: GPIO 포트 제어]\n");
    printf("  초기값: 0x%08X\n", gpio.all);
    
    printf("\n[1. 비트 단위 제어]\n");
    gpio.bits.pin0 = 1;
    gpio.bits.pin3 = 1;
    gpio.bits.pin7 = 1;
    printf("  pin0, pin3, pin7 = HIGH\n");
    printf("  레지스터 값: 0x%08X\n", gpio.all);
    
    printf("\n[2. 전체 레지스터 쓰기]\n");
    gpio.all = 0x000000FF;
    printf("  전체 핀 HIGH (0xFF)\n");
    printf("  레지스터 값: 0x%08X\n", gpio.all);
    printf("  pin0 = %d, pin7 = %d\n", gpio.bits.pin0, gpio.bits.pin7);
    
    printf("\n[3. 바이트 단위 접근]\n");
    gpio.bytes[0] = 0xAA;  // 0b10101010
    printf("  하위 바이트 = 0xAA\n");
    printf("  레지스터 값: 0x%08X\n", gpio.all);
    printf("  pin0=%d, pin1=%d, pin2=%d, pin3=%d\n",
           gpio.bits.pin0, gpio.bits.pin1, gpio.bits.pin2, gpio.bits.pin3);
    
    printf("\n[임베디드 활용]\n");
    printf("  - 레지스터 전체 읽기/쓰기: gpio.all\n");
    printf("  - 특정 비트만 제어: gpio.bits.pin0 = 1\n");
    printf("  - 바이트 단위 접근: gpio.bytes[0]\n");
}

/* ============================================================================
 * 예제 6: 실전 활용 4 - 프로토콜 파싱
 * ============================================================================ */

void Example6_ProtocolParsing(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 6: 프로토콜 파싱\n");
    printf("========================================\n");
    
    // CAN 메시지 시뮬레이션
    typedef union {
        uint8_t raw[8];         // 바이트 배열로 받음
        struct {
            uint16_t sensor_id;
            uint16_t value;
            uint32_t timestamp;
        } parsed;
    } CANMessage;
    
    printf("\n[상황: CAN 버스에서 8바이트 수신]\n");
    
    CANMessage msg;
    
    // UART/CAN에서 바이트 단위로 수신
    msg.raw[0] = 0x01;  // sensor_id (low byte)
    msg.raw[1] = 0x00;  // sensor_id (high byte)
    msg.raw[2] = 0x42;  // value (low byte)
    msg.raw[3] = 0x01;  // value (high byte)
    msg.raw[4] = 0x78;  // timestamp (byte 0)
    msg.raw[5] = 0x56;  // timestamp (byte 1)
    msg.raw[6] = 0x34;  // timestamp (byte 2)
    msg.raw[7] = 0x12;  // timestamp (byte 3)
    
    printf("  수신 데이터 (raw):\n    ");
    for (int i = 0; i < 8; i++) {
        printf("0x%02X ", msg.raw[i]);
    }
    printf("\n");
    
    printf("\n  파싱 결과:\n");
    printf("    sensor_id  = %u\n", msg.parsed.sensor_id);
    printf("    value      = %u\n", msg.parsed.value);
    printf("    timestamp  = 0x%08X\n", msg.parsed.timestamp);
    
    printf("\n[반대 방향: 구조체 → 바이트 배열]\n");
    
    CANMessage send_msg;
    send_msg.parsed.sensor_id = 100;
    send_msg.parsed.value = 9876;
    send_msg.parsed.timestamp = 0xAABBCCDD;
    
    printf("  송신할 데이터:\n");
    printf("    sensor_id  = %u\n", send_msg.parsed.sensor_id);
    printf("    value      = %u\n", send_msg.parsed.value);
    printf("    timestamp  = 0x%08X\n", send_msg.parsed.timestamp);
    
    printf("\n  바이트 배열로 변환 (전송용):\n    ");
    for (int i = 0; i < 8; i++) {
        printf("0x%02X ", send_msg.raw[i]);
    }
    printf("\n");
    
    printf("\n[임베디드 활용]\n");
    printf("  - CAN/UART/SPI 프로토콜 파싱\n");
    printf("  - 바이트 배열 ↔ 구조체 변환\n");
    printf("  - Zero-copy 데이터 처리\n");
}

/* ============================================================================
 * 예제 7: 메모리 풀에서의 Union 활용 (06_factory_pattern_FW.c 연계)
 * ============================================================================ */

void Example7_MemoryPool(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 7: 메모리 풀에서의 Union\n");
    printf("========================================\n");
    
    // 다양한 크기의 센서들
    typedef struct {
        int id;
        char name[20];
    } SensorBase;
    
    typedef struct {
        SensorBase base;
        float value;
    } TempSensor;
    
    typedef struct {
        SensorBase base;
        int readings[10];
    } ArraySensor;
    
    typedef struct {
        SensorBase base;
        char description[100];
    } StringSensor;
    
    // 모든 센서를 담을 수 있는 Union
    typedef union {
        SensorBase base;
        TempSensor temp;
        ArraySensor array;
        StringSensor string;
    } SensorUnion;
    
    printf("\n[각 타입의 크기]\n");
    printf("  sizeof(TempSensor)   = %3zu bytes\n", sizeof(TempSensor));
    printf("  sizeof(ArraySensor)  = %3zu bytes\n", sizeof(ArraySensor));
    printf("  sizeof(StringSensor) = %3zu bytes\n", sizeof(StringSensor));
    
    printf("\n[Union 크기]\n");
    printf("  sizeof(SensorUnion)  = %3zu bytes\n", sizeof(SensorUnion));
    printf("  → 가장 큰 타입 크기 = StringSensor 크기\n");
    
    printf("\n[메모리 풀 비교]\n");
    printf("  각 타입별 배열 사용:\n");
    printf("    TempSensor[10]   = %zu bytes\n", sizeof(TempSensor) * 10);
    printf("    ArraySensor[10]  = %zu bytes\n", sizeof(ArraySensor) * 10);
    printf("    StringSensor[10] = %zu bytes\n", sizeof(StringSensor) * 10);
    printf("    합계              = %zu bytes\n", 
           sizeof(TempSensor) * 10 + sizeof(ArraySensor) * 10 + sizeof(StringSensor) * 10);
    
    printf("\n  Union 배열 사용:\n");
    printf("    SensorUnion[30]  = %zu bytes\n", sizeof(SensorUnion) * 30);
    
    printf("\n[장점]\n");
    printf("  ✅ 하나의 배열로 모든 타입 관리\n");
    printf("  ✅ 타입별 개수 제한 없음 (총 30개 안에서 자유)\n");
    printf("  ✅ 할당/해제 로직 단순화\n");
    
    printf("\n[단점]\n");
    printf("  ❌ 모든 슬롯이 최대 크기 차지\n");
    printf("  ❌ 작은 객체도 큰 메모리 사용\n");
    printf("  → 그래도 펌웨어에서는 예측 가능성이 더 중요!\n");
}

/* ============================================================================
 * 예제 8: Union 주의사항
 * ============================================================================ */

void Example8_Pitfalls(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 8: Union 주의사항\n");
    printf("========================================\n");
    
    union MyUnion {
        int i;
        float f;
        char c;
    };
    
    printf("\n[주의 1] 마지막으로 쓴 멤버만 읽어야 함\n");
    union MyUnion u1;
    u1.i = 100;
    printf("  u1.i = %d (OK)\n", u1.i);
    printf("  u1.f = %.2f (위험! float로 해석하면 쓰레기 값)\n", u1.f);
    
    printf("\n[주의 2] 타입 추적 필요\n");
    printf("  Union만으로는 현재 어떤 타입인지 알 수 없음!\n");
    printf("  → 별도의 타입 태그 필요 (tagged union)\n");
    
    typedef struct {
        enum { TYPE_INT, TYPE_FLOAT, TYPE_CHAR } type;
        union {
            int i;
            float f;
            char c;
        } data;
    } TaggedUnion;
    
    TaggedUnion tu;
    tu.type = TYPE_FLOAT;
    tu.data.f = 3.14f;
    
    printf("\n  Tagged Union 예:\n");
    printf("    type = TYPE_FLOAT\n");
    printf("    data.f = %.2f\n", tu.data.f);
    
    printf("\n[주의 3] 정렬(Alignment)\n");
    union AlignTest {
        char c;      // 1 byte
        int i;       // 4 bytes
        double d;    // 8 bytes
    };
    printf("  sizeof(AlignTest) = %zu bytes\n", sizeof(union AlignTest));
    printf("  → 가장 큰 멤버(double 8)로 정렬\n");
    
    printf("\n[주의 4] 초기화\n");
    printf("  Union은 첫 번째 멤버만 초기화 가능 (C99)\n");
    union MyUnion u2 = { .i = 42 };  // designated initializer
    printf("  u2.i = %d\n", u2.i);
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Union 완전 정복\n");
    printf("========================================\n");
    
    Example1_BasicComparison();
    Example2_MemoryLayout();
    Example3_ByteAccess();
    Example4_MultipleTypes();
    Example5_RegisterAccess();
    Example6_ProtocolParsing();
    Example7_MemoryPool();
    Example8_Pitfalls();
    
    printf("\n\n");
    printf("========================================\n");
    printf("핵심 정리\n");
    printf("========================================\n");
    printf("\n[Union이란?]\n");
    printf("  여러 멤버가 같은 메모리를 공유하는 자료형\n");
    
    printf("\n[크기]\n");
    printf("  sizeof(union) = 가장 큰 멤버의 크기\n");
    
    printf("\n[사용 시기]\n");
    printf("  ✅ 여러 타입 중 하나만 사용할 때\n");
    printf("  ✅ 메모리 절약이 중요할 때\n");
    printf("  ✅ 바이트 단위 접근이 필요할 때\n");
    printf("  ✅ 레지스터/프로토콜 파싱\n");
    printf("  ✅ 메모리 풀 구현\n");
    
    printf("\n[주의사항]\n");
    printf("  ⚠️  마지막으로 쓴 멤버만 읽기\n");
    printf("  ⚠️  타입 추적 필요 (tagged union)\n");
    printf("  ⚠️  정렬 고려\n");
    
    printf("\n[임베디드에서 특히 유용]\n");
    printf("  🎯 메모리 제약\n");
    printf("  🎯 레지스터 제어\n");
    printf("  🎯 프로토콜 파싱\n");
    printf("  🎯 타입 변환\n");
    
    printf("\n========================================\n");
    
    return 0;
}

/* ============================================================================
 * 학습 포인트
 * ============================================================================
 * 
 * 1. Union vs Struct
 *    - Struct: 각 멤버 독립적인 메모리
 *    - Union:  모든 멤버가 같은 메모리 공유
 * 
 * 2. 메모리 크기
 *    - sizeof(union) = MAX(각 멤버의 크기)
 * 
 * 3. 활용 분야
 *    - Type Punning (타입 변환)
 *    - 바이트 단위 접근
 *    - 메모리 절약
 *    - 레지스터 제어
 *    - 프로토콜 파싱
 *    - 메모리 풀
 * 
 * 4. 주의사항
 *    - 마지막으로 쓴 멤버만 읽기
 *    - 타입 태그 사용 권장
 *    - 정렬 고려
 * 
 * 5. 펌웨어에서의 장점
 *    - 제한된 RAM 효율적 사용
 *    - 레지스터 비트 필드 제어
 *    - 통신 프로토콜 처리
 *    - 메모리 풀 구현
 * 
 * ============================================================================
 */
