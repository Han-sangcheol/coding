/*
 * 다형성의 진짜 목적 이해하기
 * 컴파일: gcc polymorphism_demo.c -o polymorphism_demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * 구조체 정의
 * ============================================================================ */

// 동물 (공통 인터페이스)
typedef struct Animal {
    char name[20];
    void (*speak)(struct Animal* self);  // ★ 함수 포인터
    void (*eat)(struct Animal* self);
} Animal;

// 개
typedef struct Dog {
    Animal base;
    char breed[20];
} Dog;

// 고양이
typedef struct Cat {
    Animal base;
    int lives;
} Cat;

/* ============================================================================
 * 각 동물별 함수 (다르게 동작!)
 * ============================================================================ */

void Dog_Speak(Animal* self) {
    Dog* dog = (Dog*)self;
    printf("%s: 멍멍! (품종: %s)\n", self->name, dog->breed);
}

void Dog_Eat(Animal* self) {
    printf("%s: 사료를 먹습니다\n", self->name);
}

void Cat_Speak(Animal* self) {
    Cat* cat = (Cat*)self;
    printf("%s: 야옹~ (남은 목숨: %d)\n", self->name, cat->lives);
}

void Cat_Eat(Animal* self) {
    printf("%s: 생선을 먹습니다\n", self->name);
}

/* ============================================================================
 * 예제 1: 다형성의 힘!
 * ============================================================================ */

void Example_01_Polymorphism() {
    printf("\n=== 예제 1: 다형성 (여러 타입을 하나로) ===\n\n");
    
    // 1. 개 생성
    Dog* dog = (Dog*)malloc(sizeof(Dog));
    strcpy(dog->base.name, "멍멍이");
    dog->base.speak = Dog_Speak;
    dog->base.eat = Dog_Eat;
    strcpy(dog->breed, "진돗개");
    
    // 2. 고양이 생성
    Cat* cat = (Cat*)malloc(sizeof(Cat));
    strcpy(cat->base.name, "야옹이");
    cat->base.speak = Cat_Speak;
    cat->base.eat = Cat_Eat;
    cat->lives = 9;
    
    // 3. Animal* 배열에 저장 ★★★
    Animal* animals[2];
    animals[0] = (Animal*)dog;  // Dog* → Animal*
    animals[1] = (Animal*)cat;  // Cat* → Animal*
    
    // 4. 모두 같은 방식으로 호출!
    printf("모든 동물들:\n");
    for (int i = 0; i < 2; i++) {
        animals[i]->speak(animals[i]);  // ★ 하지만 다르게 동작!
        animals[i]->eat(animals[i]);
        printf("\n");
    }
    
    printf("💡 같은 코드로 호출했지만, 각자 다르게 동작합니다!\n");
    
    free(dog);
    free(cat);
}

/* ============================================================================
 * 예제 2: 공통 기능만 사용 (캡슐화)
 * ============================================================================ */

// 동물들을 처리하는 함수 (Animal*만 받음)
void ProcessAnimal(Animal* animal) {
    printf("동물 처리: %s\n", animal->name);
    animal->speak(animal);
    animal->eat(animal);
    printf("---\n");
}

void Example_02_CommonInterface() {
    printf("\n=== 예제 2: 공통 인터페이스 ===\n\n");
    
    Dog* dog = (Dog*)malloc(sizeof(Dog));
    strcpy(dog->base.name, "바둑이");
    dog->base.speak = Dog_Speak;
    dog->base.eat = Dog_Eat;
    strcpy(dog->breed, "리트리버");
    
    Cat* cat = (Cat*)malloc(sizeof(Cat));
    strcpy(cat->base.name, "나비");
    cat->base.speak = Cat_Speak;
    cat->base.eat = Cat_Eat;
    cat->lives = 7;
    
    // ProcessAnimal 함수는 Dog인지 Cat인지 몰라도 됨!
    ProcessAnimal((Animal*)dog);
    ProcessAnimal((Animal*)cat);
    
    printf("💡 ProcessAnimal은 breed나 lives를 모르지만,\n");
    printf("   공통 기능(speak, eat)은 사용 가능합니다!\n");
    
    free(dog);
    free(cat);
}

/* ============================================================================
 * 예제 3: 필요하면 다시 변환 가능!
 * ============================================================================ */

void Example_03_DownCasting() {
    printf("\n=== 예제 3: 필요하면 원래 타입으로 변환 ===\n\n");
    
    Dog* dog = (Dog*)malloc(sizeof(Dog));
    strcpy(dog->base.name, "초코");
    strcpy(dog->breed, "푸들");
    
    // Animal*로 변환
    Animal* animal = (Animal*)dog;
    printf("Animal*로 다룰 때: %s\n", animal->name);
    // animal->breed  // ❌ 접근 불가
    
    // 다시 Dog*로 변환 (필요할 때)
    Dog* back_to_dog = (Dog*)animal;
    printf("Dog*로 변환 후: 품종 = %s\n", back_to_dog->breed);  // ✅ 접근 가능!
    
    printf("\n💡 필요하면 언제든지 원래 타입으로 되돌릴 수 있습니다!\n");
    
    free(dog);
}

/* ============================================================================
 * 예제 4: Factory 패턴 실전
 * ============================================================================ */

typedef enum AnimalType {
    TYPE_DOG,
    TYPE_CAT
} AnimalType;

// Factory 함수
Animal* CreateAnimal(AnimalType type, const char* name) {
    if (type == TYPE_DOG) {
        Dog* dog = (Dog*)malloc(sizeof(Dog));
        strcpy(dog->base.name, name);
        dog->base.speak = Dog_Speak;
        dog->base.eat = Dog_Eat;
        strcpy(dog->breed, "믹스");
        return (Animal*)dog;  // ★ Animal*로 반환
    }
    else {
        Cat* cat = (Cat*)malloc(sizeof(Cat));
        strcpy(cat->base.name, name);
        cat->base.speak = Cat_Speak;
        cat->base.eat = Cat_Eat;
        cat->lives = 9;
        return (Animal*)cat;  // ★ Animal*로 반환
    }
}

void Example_04_Factory() {
    printf("\n=== 예제 4: Factory 패턴의 진짜 목적 ===\n\n");
    
    // Factory로 동물들 생성
    Animal* zoo[5];
    zoo[0] = CreateAnimal(TYPE_DOG, "멍멍1");
    zoo[1] = CreateAnimal(TYPE_CAT, "야옹1");
    zoo[2] = CreateAnimal(TYPE_DOG, "멍멍2");
    zoo[3] = CreateAnimal(TYPE_CAT, "야옹2");
    zoo[4] = CreateAnimal(TYPE_DOG, "멍멍3");
    
    printf("동물원의 모든 동물들:\n");
    for (int i = 0; i < 5; i++) {
        printf("[%d] ", i);
        zoo[i]->speak(zoo[i]);
    }
    
    printf("\n💡 이것이 Factory의 목적입니다:\n");
    printf("   - 여러 타입(Dog, Cat)을 만들지만\n");
    printf("   - 모두 Animal*로 반환해서\n");
    printf("   - 하나의 배열에 저장하고\n");
    printf("   - 같은 방식으로 처리!\n");
    
    for (int i = 0; i < 5; i++) {
        free(zoo[i]);
    }
}

/* ============================================================================
 * 예제 5: Sensor 예제로 다시 보기
 * ============================================================================ */

typedef struct Sensor {
    int id;
    float (*read)(struct Sensor* self);
} Sensor;

typedef struct TempSensor {
    Sensor base;
    float offset;  // 온도 센서만의 고유 속성
} TempSensor;

typedef struct HumiditySensor {
    Sensor base;
    int calibration;  // 습도 센서만의 고유 속성
} HumiditySensor;

float Temp_Read(Sensor* self) {
    TempSensor* ts = (TempSensor*)self;
    return 25.0f + ts->offset;  // offset 사용
}

float Humidity_Read(Sensor* self) {
    HumiditySensor* hs = (HumiditySensor*)self;
    return 60.0f + hs->calibration;  // calibration 사용
}

void Example_05_SensorExample() {
    printf("\n=== 예제 5: Sensor 예제 ===\n\n");
    
    TempSensor* temp = (TempSensor*)malloc(sizeof(TempSensor));
    temp->base.id = 101;
    temp->base.read = Temp_Read;
    temp->offset = 2.5f;
    
    HumiditySensor* humid = (HumiditySensor*)malloc(sizeof(HumiditySensor));
    humid->base.id = 102;
    humid->base.read = Humidity_Read;
    humid->calibration = 5;
    
    // Sensor* 배열
    Sensor* sensors[2];
    sensors[0] = (Sensor*)temp;
    sensors[1] = (Sensor*)humid;
    
    // 같은 방식으로 읽기
    printf("모든 센서 읽기:\n");
    for (int i = 0; i < 2; i++) {
        float value = sensors[i]->read(sensors[i]);
        printf("  센서[%d]: %.2f\n", sensors[i]->id, value);
    }
    
    printf("\n💡 Sensor*로 다루면:\n");
    printf("   - offset, calibration에 직접 접근 안 됨 (의도적!)\n");
    printf("   - 하지만 read() 함수는 내부에서 접근 가능\n");
    printf("   - 이것이 캡슐화입니다!\n");
    
    free(temp);
    free(humid);
}

/* ============================================================================
 * Main
 * ============================================================================ */

int main() {
    printf("========================================\n");
    printf("다형성의 진짜 목적\n");
    printf("========================================\n");
    
    Example_01_Polymorphism();
    Example_02_CommonInterface();
    Example_03_DownCasting();
    Example_04_Factory();
    Example_05_SensorExample();
    
    printf("\n========================================\n");
    printf("핵심 정리\n");
    printf("========================================\n");
    printf("Animal*로 변환하는 이유:\n");
    printf("  1. 여러 타입을 하나로 다룸 (배열 저장)\n");
    printf("  2. 공통 인터페이스만 노출 (캡슐화)\n");
    printf("  3. 같은 코드로 처리 (코드 재사용)\n");
    printf("  4. 하지만 각자 다르게 동작 (다형성)\n");
    printf("  5. 필요하면 다시 변환 가능 (타입 캐스팅)\n");
    printf("\n");
    printf("breed 접근 못하는 이유:\n");
    printf("  - 의도적! 공통 기능만 사용하게 함\n");
    printf("  - 하지만 함수 내부에서는 접근 가능\n");
    printf("  - 필요하면 Dog*로 다시 변환 가능\n");
    printf("========================================\n");
    
    return 0;
}
