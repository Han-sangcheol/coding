/*
 * 타입 캐스팅 이해를 위한 간단한 예제
 * 컴파일: gcc type_casting_example.c -o type_casting_example
 */

#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * 예제 1: 기본 구조체
 * ============================================================================ */

// 동물 (부모)
typedef struct Animal {
    char name[20];
    int age;
} Animal;

// 개 (자식)
typedef struct Dog {
    Animal base;  // ★ 첫 번째 멤버가 Animal
    char breed[20];
    char* (*getBreed)(struct Dog* self);
} Dog;

// 고양이 (자식)
typedef struct Cat {
    Animal base;  // ★ 첫 번째 멤버가 Animal
    int lives;
} Cat;

char* Dog_GetBreed(Dog* self) {
    return self->breed;
}

/* ============================================================================
 * 예제 1 테스트
 * ============================================================================ */

void Example_01_Basic() {
    printf("\n=== 예제 1: 기본 타입 캐스팅 ===\n\n");
    
    // 1. Dog 만들기
    Dog* my_dog = (Dog*)malloc(sizeof(Dog));
    sprintf(my_dog->base.name, "멍멍이");
    my_dog->base.age = 3;
    sprintf(my_dog->breed, "진돗개");
    my_dog->getBreed = Dog_GetBreed;
    
    printf("Dog 정보:\n");
    printf("  이름: %s\n", my_dog->base.name);
    printf("  나이: %d\n", my_dog->base.age);
    printf("  품종: %s\n", my_dog->breed);
    
    // 2. Dog*를 Animal*로 변환 ★★★
    Animal* animal = (Animal*)my_dog;
    
    printf("\nAnimal*로 변환 후:\n");
    printf("  이름: %s\n", animal->name);
    printf("  나이: %d\n", animal->age);
    // printf("  품종: %s\n", animal->breed);  // ❌ 에러! Animal에는 breed가 없음
    
    // ★ Dog*로 다시 변환해서 getBreed 함수 호출하면 접근 불가능한 breed 접근 가능
    // 이게 바로 sensor = (Sensor*)ts; 가 하는 일입니다!
    printf("  품종: %s\n", my_dog->getBreed((Dog*)animal)); 

    


    // 3. 다시 Dog*로 변환
    Dog* back_to_dog = (Dog*)animal;
    printf("\n다시 Dog*로 변환:\n");
    printf("  품종: %s\n", back_to_dog->breed);  // ✅ 다시 접근 가능!
    
    free(my_dog);
}

/* ============================================================================
 * 예제 2: 메모리 주소 확인
 * ============================================================================ */

void Example_02_Memory() {
    printf("\n=== 예제 2: 메모리 주소 확인 ===\n\n");
    
    Dog* my_dog = (Dog*)malloc(sizeof(Dog));
    sprintf(my_dog->base.name, "바둑이");
    my_dog->base.age = 5;
    sprintf(my_dog->breed, "리트리버");
    
    printf("Dog* 주소:         %p\n", (void*)my_dog);
    printf("Dog->base 주소:    %p\n", (void*)&(my_dog->base));
    printf("Dog->breed 주소:   %p\n", (void*)&(my_dog->breed));
    
    // Animal*로 캐스팅
    Animal* animal = (Animal*)my_dog;
    printf("\nAnimal* 주소:      %p\n", (void*)animal);
    
    printf("\n💡 주목! Dog*와 Animal*의 주소가 같습니다!\n");
    printf("   왜냐하면 Dog의 첫 번째 멤버가 Animal이기 때문입니다.\n");
    
    free(my_dog);
}

/* ============================================================================
 * 예제 3: 왜 이렇게 하는가? (다형성)
 * ============================================================================ */

// 동물 울음소리 출력 함수 (Animal*만 받음)
void PrintAnimalInfo(Animal* animal) {
    printf("  동물: %s, 나이: %d세\n", animal->name, animal->age);
}

void Example_03_Polymorphism() {
    printf("\n=== 예제 3: 다형성 (여러 타입을 하나로) ===\n\n");
    
    // 개와 고양이 생성
    Dog* dog = (Dog*)malloc(sizeof(Dog));
    sprintf(dog->base.name, "멍멍이");
    dog->base.age = 3;
    sprintf(dog->breed, "푸들");
    
    Cat* cat = (Cat*)malloc(sizeof(Cat));
    sprintf(cat->base.name, "야옹이");
    cat->base.age = 2;
    cat->lives = 9;
    
    // 둘 다 Animal*로 변환해서 같은 함수에 전달!
    printf("같은 함수로 처리:\n");
    PrintAnimalInfo((Animal*)dog);  // ★ Dog* → Animal*
    PrintAnimalInfo((Animal*)cat);  // ★ Cat* → Animal*
    
    printf("\n💡 서로 다른 타입(Dog, Cat)을 하나의 타입(Animal)로 통일!\n");
    
    free(dog);
    free(cat);
}

/* ============================================================================
 * 예제 4: 배열에 여러 타입 저장
 * ============================================================================ */

void Example_04_Array() {
    printf("\n=== 예제 4: 배열에 여러 타입 저장 ===\n\n");
    
    // Animal* 배열 (여러 타입을 담을 수 있음!)
    Animal* animals[3];
    
    // Dog 생성
    Dog* dog = (Dog*)malloc(sizeof(Dog));
    sprintf(dog->base.name, "멍멍이");
    dog->base.age = 3;
    animals[0] = (Animal*)dog;  // ★ Dog* → Animal*
    
    // Cat 생성
    Cat* cat = (Cat*)malloc(sizeof(Cat));
    sprintf(cat->base.name, "야옹이");
    cat->base.age = 2;
    animals[1] = (Animal*)cat;  // ★ Cat* → Animal*
    
    // 또 다른 Dog
    Dog* dog2 = (Dog*)malloc(sizeof(Dog));
    sprintf(dog2->base.name, "바둑이");
    dog2->base.age = 5;
    animals[2] = (Animal*)dog2;  // ★ Dog* → Animal*
    
    // 배열 순회 (모두 Animal*로 다룸)
    printf("모든 동물 정보:\n");
    for (int i = 0; i < 3; i++) {
        printf("  [%d] %s (%d세)\n", i, animals[i]->name, animals[i]->age);
    }
    
    printf("\n💡 Dog와 Cat을 하나의 배열에 저장했습니다!\n");
    
    free(dog);
    free(cat);
    free(dog2);
}

/* ============================================================================
 * 예제 5: Factory 패턴 간단 버전
 * ============================================================================ */

typedef enum AnimalType {
    TYPE_DOG,
    TYPE_CAT
} AnimalType;

// Factory 함수 (Animal*를 반환)
Animal* CreateAnimal(AnimalType type, const char* name, int age) {
    Animal* result = NULL;
    
    if (type == TYPE_DOG) {
        Dog* dog = (Dog*)malloc(sizeof(Dog));
        sprintf(dog->base.name, "%s", name);
        dog->base.age = age;
        sprintf(dog->breed, "믹스");
        
        result = (Animal*)dog;  // ★★★ 이게 핵심!
    }
    else if (type == TYPE_CAT) {
        Cat* cat = (Cat*)malloc(sizeof(Cat));
        sprintf(cat->base.name, "%s", name);
        cat->base.age = age;
        cat->lives = 9;
        
        result = (Animal*)cat;  // ★★★ 이게 핵심!
    }
    
    return result;  // Animal*로 반환
}

void Example_05_Factory() {
    printf("\n=== 예제 5: Factory 패턴 ===\n\n");
    
    // Factory로 동물 생성 (타입만 다르게)
    Animal* animal1 = CreateAnimal(TYPE_DOG, "멍멍이", 3);
    Animal* animal2 = CreateAnimal(TYPE_CAT, "야옹이", 2);
    Animal* animal3 = CreateAnimal(TYPE_DOG, "바둑이", 5);
    
    printf("Factory로 생성된 동물들:\n");
    printf("  %s (%d세)\n", animal1->name, animal1->age);
    printf("  %s (%d세)\n", animal2->name, animal2->age);
    printf("  %s (%d세)\n", animal3->name, animal3->age);
    
    printf("\n💡 CreateAnimal 함수는 Dog나 Cat을 만들지만,\n");
    printf("   모두 Animal*로 반환합니다!\n");
    printf("   이게 바로 sensor = (Sensor*)ts; 가 하는 일입니다!\n");
    
    free(animal1);
    free(animal2);
    free(animal3);
}

/* ============================================================================
 * Main
 * ============================================================================ */

int main() {
    printf("========================================\n");
    printf("타입 캐스팅 이해하기\n");
    printf("========================================\n");
    
    Example_01_Basic();
    Example_02_Memory();
    Example_03_Polymorphism();
    Example_04_Array();
    Example_05_Factory();
    
    printf("\n========================================\n");
    printf("결론:\n");
    printf("========================================\n");
    printf("sensor = (Sensor*)ts; 는:\n");
    printf("  TemperatureSensor*를 Sensor*로 변환합니다.\n");
    printf("  왜? 여러 센서 타입을 하나로 다루기 위해!\n");
    printf("  어떻게? base가 첫 번째 멤버라 주소가 같아서!\n");
    printf("========================================\n");
    
    return 0;
}
