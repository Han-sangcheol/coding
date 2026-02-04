/*
 * 타입 캐스팅의 메모리 처리 과정 이해하기
 * 컴파일: gcc memory_layout_demo.c -o memory_layout_demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ============================================================================
 * 예제 1: 기본 메모리 레이아웃
 * ============================================================================ */

typedef struct {
    char name[20];
    int age;
} Animal;

typedef struct {
    Animal base;  // ★ 첫 번째 멤버
    char breed[20];
} Dog;

void Example_01_MemoryLayout() {
    printf("\n=== 예제 1: 메모리 레이아웃 ===\n\n");
    
    // Dog 할당
    Dog* my_dog = (Dog*)malloc(sizeof(Dog));
    strcpy(my_dog->base.name, "멍멍이");
    my_dog->base.age = 3;
    strcpy(my_dog->breed, "진돗개");
    
    // 메모리 주소 출력
    printf("메모리 주소:\n");
    printf("  my_dog (Dog*):           %p\n", (void*)my_dog);
    printf("  &my_dog->base (Animal*): %p\n", (void*)&(my_dog->base));
    printf("  &my_dog->base.name:      %p\n", (void*)&(my_dog->base.name));
    printf("  &my_dog->base.age:       %p\n", (void*)&(my_dog->base.age));
    printf("  &my_dog->breed:          %p\n", (void*)&(my_dog->breed));
    
    printf("\n바이트 오프셋:\n");
    printf("  base.name:  offset %ld\n", (char*)&(my_dog->base.name) - (char*)my_dog);
    printf("  base.age:   offset %ld\n", (char*)&(my_dog->base.age) - (char*)my_dog);
    printf("  breed:      offset %ld\n", (char*)&(my_dog->breed) - (char*)my_dog);
    
    printf("\n크기:\n");
    printf("  sizeof(Animal): %zu bytes\n", sizeof(Animal));
    printf("  sizeof(Dog):    %zu bytes\n", sizeof(Dog));
    
    // 타입 캐스팅
    Animal* animal = (Animal*)my_dog;
    
    printf("\n타입 캐스팅 후:\n");
    printf("  animal (Animal*):        %p\n", (void*)animal);
    printf("  &animal->name:           %p\n", (void*)&(animal->name));
    printf("  &animal->age:            %p\n", (void*)&(animal->age));
    
    printf("\n💡 주목: my_dog와 animal의 주소가 같습니다!\n");
    printf("   이유: Dog의 첫 번째 멤버가 Animal이기 때문\n");
    
    free(my_dog);
}

/* ============================================================================
 * 예제 2: 메모리 바이트 단위로 보기
 * ============================================================================ */

void Example_02_ByteLevel() {
    printf("\n=== 예제 2: 바이트 단위 메모리 ===\n\n");
    
    Dog* dog = (Dog*)malloc(sizeof(Dog));
    strcpy(dog->base.name, "ABC");
    dog->base.age = 5;
    strcpy(dog->breed, "XYZ");
    
    printf("Dog 메모리 (16진수로):\n");
    unsigned char* bytes = (unsigned char*)dog;
    for (size_t i = 0; i < sizeof(Dog); i++) {
        printf("%02X ", bytes[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
    
    printf("\nDog* dog = %p\n", (void*)dog);
    printf("Animal* animal = (Animal*)dog = %p\n", (void*)(Animal*)dog);
    
    printf("\n💡 캐스팅 = 주소는 그대로, 타입만 변경!\n");
    
    free(dog);
}

/* ============================================================================
 * 예제 3: Sensor 실전 예제
 * ============================================================================ */

typedef struct Sensor {
    int sensor_id;
    char name[16];
    float (*read)(struct Sensor* self);
} Sensor;

typedef struct {
    Sensor base;     // ★ 0번 오프셋 (시작 위치)
    float offset;
    float scale;
} TemperatureSensor;

float Temp_Read(Sensor* self) {
    return 25.0f;
}

void Example_03_SensorMemory() {
    printf("\n=== 예제 3: Sensor 메모리 레이아웃 ===\n\n");
    
    // 1. TemperatureSensor 할당
    TemperatureSensor* ts = (TemperatureSensor*)malloc(sizeof(TemperatureSensor));
    
    printf("1단계: malloc으로 메모리 할당\n");
    printf("   주소: %p\n", (void*)ts);
    printf("   크기: %zu bytes\n\n", sizeof(TemperatureSensor));
    
    // 메모리 구조 출력
    printf("메모리 구조:\n");
    printf("  ┌─────────────────────────────┐ ← %p (시작)\n", (void*)ts);
    printf("  │ Sensor base                 │\n");
    printf("  │   sensor_id   (4 bytes)     │ ← offset 0\n");
    printf("  │   name[16]    (16 bytes)    │ ← offset 4\n");
    printf("  │   read*       (8 bytes)     │ ← offset 20\n");
    printf("  ├─────────────────────────────┤ ← offset 28\n");
    printf("  │ float offset  (4 bytes)     │\n");
    printf("  │ float scale   (4 bytes)     │\n");
    printf("  └─────────────────────────────┘ ← offset 36\n\n");
    
    // 초기화
    ts->base.sensor_id = 101;
    strcpy(ts->base.name, "Temp_101");
    ts->base.read = Temp_Read;
    ts->offset = 2.5f;
    ts->scale = 1.0f;
    
    printf("2단계: 데이터 초기화\n");
    printf("   ts->base.sensor_id = %d\n", ts->base.sensor_id);
    printf("   ts->base.name = %s\n", ts->base.name);
    printf("   ts->offset = %.2f\n", ts->offset);
    printf("   ts->scale = %.2f\n\n", ts->scale);
    
    // 3. Sensor*로 캐스팅
    Sensor* sensor = (Sensor*)ts;
    
    printf("3단계: Sensor*로 타입 캐스팅\n");
    printf("   TemperatureSensor* ts = %p\n", (void*)ts);
    printf("   Sensor* sensor        = %p\n", (void*)sensor);
    printf("   → 주소 동일!\n\n");
    
    printf("4단계: Sensor*로 접근\n");
    printf("   sensor->sensor_id = %d\n", sensor->sensor_id);
    printf("   sensor->name = %s\n", sensor->name);
    printf("   sensor는 offset, scale을 모름!\n\n");
    
    printf("5단계: 메모리는 그대로!\n");
    printf("   메모리는 변하지 않았습니다.\n");
    printf("   단지 컴파일러가 '처음 28바이트만 보라'고 해석할 뿐!\n\n");
    
    // 실제 메모리 주소 확인
    printf("실제 주소:\n");
    printf("   ts:             %p\n", (void*)ts);
    printf("   &ts->base:      %p (offset 0)\n", (void*)&(ts->base));
    printf("   &ts->offset:    %p (offset %ld)\n", 
           (void*)&(ts->offset), 
           (char*)&(ts->offset) - (char*)ts);
    printf("   sensor:         %p\n", (void*)sensor);
    
    free(ts);
}

/* ============================================================================
 * 예제 4: 포인터 캐스팅의 본질
 * ============================================================================ */

void Example_04_WhatHappens() {
    printf("\n=== 예제 4: 캐스팅 시 실제로 일어나는 일 ===\n\n");
    
    TemperatureSensor* ts = (TemperatureSensor*)malloc(sizeof(TemperatureSensor));
    ts->base.sensor_id = 101;
    strcpy(ts->base.name, "TempSensor");
    ts->offset = 1.5f;
    
    printf("메모리 주소 (숫자로):\n");
    uintptr_t addr_ts = (uintptr_t)ts;
    printf("  ts (TemperatureSensor*): 0x%lx (%lu)\n", addr_ts, addr_ts);
    
    // Sensor*로 캐스팅
    Sensor* sensor = (Sensor*)ts;
    uintptr_t addr_sensor = (uintptr_t)sensor;
    printf("  sensor (Sensor*):        0x%lx (%lu)\n", addr_sensor, addr_sensor);
    
    printf("\n변경된 것:\n");
    printf("  주소: %s\n", (addr_ts == addr_sensor) ? "변경 없음" : "변경됨");
    printf("  메모리 내용: 변경 없음\n");
    printf("  타입 정보: TemperatureSensor* → Sensor*\n");
    
    printf("\n💡 캐스팅은 단순히 '타입 정보만 변경'합니다!\n");
    printf("   메모리 주소도, 내용도 변하지 않습니다.\n");
    printf("   컴파일러가 해당 주소를 '어떻게 해석할지'만 바뀝니다.\n");
    
    free(ts);
}

/* ============================================================================
 * 예제 5: 시각적 비유
 * ============================================================================ */

void Example_05_Analogy() {
    printf("\n=== 예제 5: 비유로 이해하기 ===\n\n");
    
    Dog* dog = (Dog*)malloc(sizeof(Dog));
    strcpy(dog->base.name, "바둑이");
    dog->base.age = 5;
    strcpy(dog->breed, "진돗개");
    
    printf("실제 메모리 (집):\n");
    printf("  ┌──────────────────┐ ← 주소: %p\n", (void*)dog);
    printf("  │ name: 바둑이     │  1층\n");
    printf("  │ age: 5           │  1층\n");
    printf("  ├──────────────────┤\n");
    printf("  │ breed: 진돗개    │  2층\n");
    printf("  └──────────────────┘\n\n");
    
    printf("Dog*로 볼 때:\n");
    printf("  → 1층과 2층 모두 볼 수 있음\n");
    printf("  → name, age, breed 모두 접근 가능\n\n");
    
    Animal* animal = (Animal*)dog;
    
    printf("Animal*로 캐스팅 후:\n");
    printf("  → 같은 집(주소: %p)\n", (void*)animal);
    printf("  → 하지만 1층만 볼 수 있음\n");
    printf("  → name, age만 접근 가능\n");
    printf("  → breed는 2층이라 안 보임\n\n");
    
    printf("💡 집(메모리)은 그대로인데,\n");
    printf("   보는 시각(타입)만 바뀐 것입니다!\n");
    
    free(dog);
}

/* ============================================================================
 * 예제 6: malloc과 캐스팅의 전체 과정
 * ============================================================================ */

void Example_06_FullProcess() {
    printf("\n=== 예제 6: 전체 과정 단계별 ===\n\n");
    
    printf("1단계: sizeof 계산\n");
    printf("   sizeof(TemperatureSensor) = %zu bytes\n\n", sizeof(TemperatureSensor));
    
    printf("2단계: malloc 호출\n");
    TemperatureSensor* ts = (TemperatureSensor*)malloc(sizeof(TemperatureSensor));
    printf("   힙에서 %zu bytes 할당\n", sizeof(TemperatureSensor));
    printf("   할당된 주소: %p\n", (void*)ts);
    printf("   ts 변수에 이 주소 저장\n\n");
    
    printf("3단계: 데이터 쓰기\n");
    ts->base.sensor_id = 101;
    strcpy(ts->base.name, "Sensor");
    ts->offset = 2.5f;
    printf("   메모리에 데이터 기록\n");
    printf("   [%p] sensor_id = %d\n", (void*)&(ts->base.sensor_id), ts->base.sensor_id);
    printf("   [%p] offset = %.2f\n\n", (void*)&(ts->offset), ts->offset);
    
    printf("4단계: 타입 캐스팅\n");
    printf("   Sensor* sensor = (Sensor*)ts;\n");
    Sensor* sensor = (Sensor*)ts;
    printf("   CPU 동작: 없음! (주소만 복사)\n");
    printf("   메모리 변경: 없음!\n");
    printf("   컴파일러 해석 변경:\n");
    printf("     - ts로 접근: TemperatureSensor 타입으로 해석\n");
    printf("     - sensor로 접근: Sensor 타입으로 해석\n\n");
    
    printf("5단계: 접근 비교\n");
    printf("   ts->base.sensor_id:  %d (TemperatureSensor* 타입)\n", ts->base.sensor_id);
    printf("   sensor->sensor_id:   %d (Sensor* 타입)\n", sensor->sensor_id);
    printf("   → 같은 메모리 읽음!\n\n");
    
    printf("   ts->offset:          %.2f (접근 가능)\n", ts->offset);
    printf("   sensor->offset:      (컴파일 에러! Sensor에 offset 없음)\n\n");
    
    printf("6단계: free\n");
    free(ts);  // 또는 free(sensor); - 주소가 같으므로 같은 효과
    printf("   힙 메모리 해제\n");
    printf("   주소: %p\n\n", (void*)ts);
}

/* ============================================================================
 * Main
 * ============================================================================ */

int main() {
    printf("========================================\n");
    printf("타입 캐스팅의 메모리 처리 과정\n");
    printf("========================================\n");
    
    Example_01_MemoryLayout();
    Example_02_ByteLevel();
    Example_03_SensorMemory();
    Example_04_WhatHappens();
    Example_05_Analogy();
    Example_06_FullProcess();
    
    printf("\n========================================\n");
    printf("핵심 정리\n");
    printf("========================================\n");
    printf("\n타입 캐스팅 시 일어나는 일:\n\n");
    printf("1. 메모리 주소: 변경 없음\n");
    printf("   TemperatureSensor* ts = 0x1000\n");
    printf("   Sensor* sensor = (Sensor*)ts\n");
    printf("   → sensor도 0x1000\n\n");
    
    printf("2. 메모리 내용: 변경 없음\n");
    printf("   실제 데이터는 그대로\n\n");
    
    printf("3. 타입 정보만 변경:\n");
    printf("   컴파일러가 해당 주소를\n");
    printf("   - ts: TemperatureSensor로 해석\n");
    printf("   - sensor: Sensor로 해석\n\n");
    
    printf("4. 접근 범위 제한:\n");
    printf("   - ts: 전체 구조체 접근 가능\n");
    printf("   - sensor: base 부분만 접근 가능\n\n");
    
    printf("5. CPU 관점:\n");
    printf("   - 캐스팅 자체는 아무 연산도 안 함\n");
    printf("   - 단지 주소를 다른 변수에 복사\n");
    printf("   - 메모리 읽기/쓰기는 동일하게 동작\n\n");
    
    printf("비유:\n");
    printf("  같은 집(메모리)을\n");
    printf("  - 1층 + 2층 전체로 보기 (TemperatureSensor*)\n");
    printf("  - 1층만 보기 (Sensor*)\n");
    printf("  집은 그대로, 보는 방식만 다름!\n");
    printf("========================================\n");
    
    return 0;
}
