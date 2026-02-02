/*
 * ============================================================================
 * 파일명: 00_function_pointer_basics.c
 * 기능: C언어 함수 포인터(Function Pointer) 완전 정복
 * 
 * 이 파일의 목적:
 *   - 함수 포인터를 처음 배우는 사람도 이해할 수 있도록
 *   - 단계별로 난이도를 높여가며 설명
 *   - 실행 가능한 예제로 직접 확인
 *   - 실무에서 어떻게 사용되는지 이해
 * 
 * ============================================================================
 * 📚 학습 목차
 * ============================================================================
 * 
 * 1단계: 함수 포인터가 뭔가요? (기초 개념)
 * 2단계: 함수 포인터 선언하기
 * 3단계: 함수 포인터 사용하기
 * 4단계: typedef로 간단하게 만들기
 * 5단계: 함수 포인터 배열
 * 6단계: 콜백 함수 (Callback)
 * 7단계: 구조체와 함수 포인터
 * 8단계: 실무 예제 (계산기, 명령 테이블, 상태 머신)
 * 9단계: 대비 포인트
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * 1단계: 함수 포인터가 뭔가요?
 * ============================================================================
 * 
 * 💡 핵심 개념:
 * 
 *   "함수 포인터는 함수의 주소를 저장하는 변수입니다"
 * 
 *   int x = 10;       // x는 정수 10을 저장
 *   int* p = &x;      // p는 변수 x의 주소를 저장
 *   
 *   void func();      // func는 함수
 *   void (*fp)();     // fp는 함수의 주소를 저장 ← 함수 포인터!
 * 
 * ❓ 왜 필요한가?
 * 
 *   1. 런타임에 호출할 함수를 선택할 수 있음
 *   2. 콜백 함수 구현
 *   3. 함수 테이블 구현
 *   4. 다형성(Polymorphism) 구현
 * 
 * ============================================================================
 */

/* 예제를 위한 간단한 함수들 */
void HelloWorld(void) {
    printf("Hello, World!\n");
}

void Goodbye(void) {
    printf("Goodbye!\n");
}

void Step1_Concept(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  1단계: 함수 포인터 개념 이해하기\n");
    printf("════════════════════════════════════════\n\n");
    
    /* 일반적인 함수 호출 */
    printf("[일반 호출] ");
    HelloWorld();
    
    /* 함수의 주소 출력 */
    printf("\n[함수 주소]\n");
    printf("  HelloWorld 함수의 주소: %p\n", (void*)HelloWorld);
    printf("  Goodbye 함수의 주소: %p\n", (void*)Goodbye);

    printf("\n💡 함수도 메모리 어딘가에 저장되어 있어요!\n");
    printf("   이 주소를 변수에 저장할 수 있다면?\n");
    printf("   → 바로 '함수 포인터'입니다!\n");
}

/* ============================================================================
 * 2단계: 함수 포인터 선언하기
 * ============================================================================
 * 
 * 📝 기본 문법:
 * 
 *   반환타입 (*포인터이름)(매개변수타입1, 매개변수타입2, ...);
 * 
 * 예제:
 *   void (*fp)(void);              // void 반환, 매개변수 없음
 *   int (*fp)(int, int);           // int 반환, int 2개 매개변수
 *   float (*fp)(double);           // float 반환, double 1개 매개변수
 * 
 * ⚠️ 주의: 괄호가 중요합니다!
 * 
 *   void (*fp)(void);   ✅ 함수 포인터
 *   void *fp(void);     ❌ void*를 반환하는 함수 (함수 포인터 아님!)
 * 
 * ============================================================================
 */

/* 매개변수가 없는 함수들 */
void PrintA(void) { printf("A\n"); }
void PrintB(void) { printf("B\n"); }

/* 매개변수가 있는 함수들 */
int Add(int a, int b) { return a + b; }
int Subtract(int a, int b) { return a - b; }
int Multiply(int a, int b) { return a * b; }

void Step2_Declaration(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  2단계: 함수 포인터 선언하기\n");
    printf("════════════════════════════════════════\n\n");
    
    /* 예제 1: 매개변수 없는 함수 포인터 */
    printf("[예제 1] void (*fp)(void)\n");
    void (*simple_fp)(void);  // 선언
    
    simple_fp = PrintA;       // 함수 주소 대입 (& 생략 가능)
    printf("  함수 포인터 호출: ");
    simple_fp();              // 함수 호출 (PrintA 실행됨)
    
    simple_fp = PrintB;       // 다른 함수로 변경
    printf("  함수 포인터 호출: ");
    simple_fp();              // PrintB 실행됨
    
    /* 예제 2: 매개변수 있는 함수 포인터 */
    printf("\n[예제 2] int (*fp)(int, int)\n");
    int (*calc_fp)(int, int);  // 선언
    
    calc_fp = Add;
    printf("  10 + 5 = %d\n", calc_fp(10, 5));
    
    calc_fp = Multiply;
    printf("  10 * 5 = %d\n", calc_fp(10, 5));
    
    printf("\n💡 함수 포인터로 런타임에 호출할 함수를 바꿀 수 있어요!\n");
}

/* ============================================================================
 * 3단계: 함수 포인터 사용하기 (더 자세히)
 * ============================================================================
 * 
 * 📌 함수 포인터 사용 4단계:
 * 
 *   1️⃣ 함수 정의
 *   2️⃣ 함수 포인터 선언
 *   3️⃣ 함수 주소 대입
 *   4️⃣ 함수 포인터로 호출
 * 
 * ============================================================================
 */

void PrintSquare(int n) {
    printf("  %d의 제곱 = %d\n", n, n * n);
}

void PrintCube(int n) {
    printf("  %d의 세제곱 = %d\n", n, n * n * n);
}

void Step3_Usage(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  3단계: 함수 포인터 사용 패턴\n");
    printf("════════════════════════════════════════\n\n");
    
    /* 1️⃣ 함수 정의 - 위에 이미 정의됨 (PrintSquare, PrintCube) */
    
    /* 2️⃣ 함수 포인터 선언 */
    void (*operation)(int);    // 선언
    
    /* 3️⃣ 함수 주소 대입 */
    operation = PrintSquare;
    
    /* 4️⃣ 함수 포인터로 호출 */
    printf("[방법 1] operation = PrintSquare\n");
    operation(5);
    
    /* 다른 함수로 교체 */
    operation = PrintCube;
    printf("\n[방법 2] operation = PrintCube\n");
    operation(5);
    
    /* 다양한 호출 방법 */
    printf("\n[함수 포인터 호출 방법 3가지]\n");
    operation = PrintSquare;
    
    printf("1. operation(3)     → ");
    operation(3);
    
    printf("2. (*operation)(4)  → ");
    (*operation)(4);
    
    printf("3. (&operation)(5)는 불가능\n");
    
    printf("\n💡 보통은 operation(n) 형태로 사용해요!\n");
}

/* ============================================================================
 * 4단계: typedef로 간단하게 만들기
 * ============================================================================
 * 
 * 😰 문제: 함수 포인터 문법이 복잡해요!
 * 
 *   void (*fp1)(int);
 *   void (*fp2)(int);
 *   void (*fp3)(int);
 *   
 *   → 매번 이렇게 쓰기 귀찮아요!
 * 
 * 😊 해결: typedef를 사용하세요!
 * 
 *   typedef void (*Operation)(int);
 *   
 *   Operation fp1;
 *   Operation fp2;
 *   Operation fp3;
 *   
 *   → 훨씬 간단하고 읽기 쉬워요!
 * 
 * ============================================================================
 */

/* typedef로 함수 포인터 타입 정의 */
typedef void (*PrintFunc)(const char* msg);
typedef int (*CalcFunc)(int, int);

void PrintMessage(const char* msg) {
    printf("  메시지: %s\n", msg);
}

void PrintError(const char* msg) {
    printf("  [ERROR] %s\n", msg);
}

void Step4_Typedef(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  4단계: typedef로 깔끔하게!\n");
    printf("════════════════════════════════════════\n\n");
    
    /* typedef 없이 사용 */
    printf("[typedef 없이]\n");
    void (*printer1)(const char*);
    printer1 = PrintMessage;
    printer1("안녕하세요");
    
    /* typedef 사용 */
    printf("\n[typedef 사용]\n");
    PrintFunc printer2;  // 훨씬 간단!
    printer2 = PrintMessage;
    printer2("안녕하세요");
    
    printer2 = PrintError;
    printer2("오류 발생!");
    
    /* 계산 함수 typedef */
    printf("\n[계산 함수]\n");
    CalcFunc calc;
    
    calc = Add;
    printf("  100 + 50 = %d\n", calc(100, 50));
    
    calc = Multiply;
    printf("  100 * 50 = %d\n", calc(100, 50));
    
    printf("\n💡 typedef를 쓰면 코드가 훨씬 읽기 쉬워져요!\n");
}

/* ============================================================================
 * 5단계: 함수 포인터 배열
 * ============================================================================
 * 
 * 🎯 함수 포인터를 배열로 만들 수 있어요!
 * 
 *   함수포인터타입 배열이름[크기];
 * 
 * 💼 실무 활용:
 *   - 명령 테이블
 *   - 메뉴 시스템
 *   - 이벤트 핸들러
 * 
 * ============================================================================
 */

void Level1(void) { printf("    레벨 1 실행\n"); }
void Level2(void) { printf("    레벨 2 실행\n"); }
void Level3(void) { printf("    레벨 3 실행\n"); }
void Level4(void) { printf("    레벨 4 실행\n"); }
void Level5(void) { printf("    레벨 5 실행\n"); }

typedef void (*LevelFunc)(void);

void Step5_Array(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  5단계: 함수 포인터 배열\n");
    printf("════════════════════════════════════════\n\n");
    
    /* 함수 포인터 배열 생성 */
    LevelFunc levels[5] = {
        Level1,
        Level2,
        Level3,
        Level4,
        Level5
    };
    
    printf("[게임 레벨 실행]\n");
    for (int i = 0; i < 5; i++) {
        printf("  레벨 %d → ", i + 1);
        levels[i]();  // 배열 인덱스로 함수 호출!
    }
    
    /* 계산기 예제 */
    printf("\n[간단한 계산기]\n");
    CalcFunc operations[3] = { Add, Subtract, Multiply };
    const char* op_names[3] = { "덧셈", "뺄셈", "곱셈" };
    
    int a = 20, b = 4;
    for (int i = 0; i < 3; i++) {
        printf("  %s: %d 결과 = %d\n", 
               op_names[i], a, operations[i](a, b));
    }
    
    printf("\n💡 함수 포인터 배열로 인덱스만 바꿔서 다른 함수 호출!\n");
}

/* ============================================================================
 * 6단계: 콜백 함수 (Callback)
 * ============================================================================
 * 
 * 🎯 콜백이란?
 * 
 *   "다른 함수에 함수 포인터를 전달해서, 그 함수가 나중에 호출하게 하는 것"
 * 
 * 예시:
 *   void ProcessData(int data, void (*callback)(int)) {
 *       // 데이터 처리...
 *       callback(data);  // 처리 완료 후 콜백 호출
 *   }
 * 
 * 💼 실무 활용:
 *   - 이벤트 핸들러
 *   - 비동기 처리
 *   - 인터럽트 핸들러
 * 
 * ============================================================================
 */

/* 콜백 함수들 */
void OnSuccess(int result) {
    printf("    ✅ 성공! 결과: %d\n", result);
}

void OnError(int error_code) {
    printf("    ❌ 오류! 코드: %d\n", error_code);
}

/* 콜백을 받는 함수 */
void ProcessTask(int value, void (*callback)(int)) {
    printf("    작업 처리 중... (값: %d)\n", value);
    
    // 작업 처리 시뮬레이션
    if (value > 0) {
        callback(value * 2);  // 성공 콜백
    } else {
        callback(-1);  // 오류 콜백
    }
}

/* 콜백을 받는 함수 */
void ProcessTask2(int value, void (*callback)(int), void (*callback2)(int)) {
    printf("    작업 처리 중... (값: %d)\n", value);
    
    // 작업 처리 시뮬레이션
    if (value > 0) {
        callback(value * 2);  // 성공 콜백
    } else {
        callback2(-1);  // 오류 콜백
    }
}

/* 배열 정렬 예제 (qsort 스타일) */
int CompareAsc(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int CompareDesc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

void SimpleSortAndPrint(int* arr, int size, int (*compare)(const void*, const void*)) {
    // 간단한 버블 정렬
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compare(&arr[j], &arr[j + 1]) > 0) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    
    // 출력
    printf("    ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void Step6_Callback(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  6단계: 콜백 함수 (Callback)\n");
    printf("════════════════════════════════════════\n\n");
    
    /* 기본 콜백 예제 */
    printf("[예제 1] 성공/실패 콜백\n");
    ProcessTask(10, OnSuccess);
    ProcessTask(-5, OnError);
    ProcessTask2(10, OnSuccess, OnError);
    ProcessTask2(-5, OnSuccess, OnError);
    
    /* 정렬 콜백 예제 */
    printf("\n[예제 2] 정렬 방식을 콜백으로 선택\n");
    int numbers1[] = {5, 2, 8, 1, 9};
    int numbers2[] = {5, 2, 8, 1, 9};
    
    printf("  오름차순: ");
    SimpleSortAndPrint(numbers1, 5, CompareAsc);
    
    printf("  내림차순: ");
    SimpleSortAndPrint(numbers2, 5, CompareDesc);
    
    printf("\n💡 콜백으로 함수의 동작을 유연하게 커스터마이징!\n");
}

/* ============================================================================
 * 7단계: 구조체와 함수 포인터
 * ============================================================================
 * 
 * 🎯 구조체에 함수 포인터를 넣으면?
 * 
 *   → C에서 객체 지향 프로그래밍처럼 사용 가능!
 *   → 다형성(Polymorphism) 구현!
 * 
 * 💼 실무 활용:
 *   - 드라이버 인터페이스
 *   - 가상 함수 테이블
 *   - 플러그인 시스템
 * 
 * ============================================================================
 */

/* 동물 구조체 (함수 포인터 포함) */
typedef struct Animal {
    char name[32];
    void (*speak)(struct Animal* self);
    void (*move)(struct Animal* self);
} Animal;

/* 구체적인 동작 함수들 */
void Dog_Speak(Animal* self) {
    printf("    %s: 멍멍!\n", self->name);
}

void Dog_Move(Animal* self) {
    printf("    %s: 네 발로 달립니다\n", self->name);
}

void Bird_Speak(Animal* self) {
    printf("    %s: 짹짹!\n", self->name);
}

void Bird_Move(Animal* self) {
    printf("    %s: 날아갑니다\n", self->name);
}

/* 동물 생성 함수 */
Animal CreateDog(const char* name) {
    Animal dog;
    printf("CreateDog: %p\n", &dog);
    strcpy(dog.name, name);
    dog.speak = Dog_Speak;
    dog.move = Dog_Move;
    return dog;
}

Animal CreateBird(const char* name) {
    Animal bird;
    printf("CreateBird: %p\n", &bird);
    strcpy(bird.name, name);
    bird.speak = Bird_Speak;
    bird.move = Bird_Move;
    return bird;
}

// 작성해본. 
// Animal CreateAnimal(Animal type, const char* name) {
//     Animal animal;
//     printf("CreateAnimal: %p\n", &animal);
//     strcpy(animal.name, name);
//     animal.speak = type == ANIMAL_DOG ? Dog_Speak : Bird_Speak;
//     animal.move = type == ANIMAL_DOG ? Dog_Move : Bird_Move;
//     return animal;
// }

void Step7_StructWithFP(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  7단계: 구조체 + 함수 포인터\n");
    printf("════════════════════════════════════════\n\n");
    
    /* 동물 객체 생성 */
    Animal dog = CreateDog("바둑이");
    Animal bird = CreateBird("짹짹이");
    printf("dog: %p\n", &dog);
    printf("bird: %p\n", &bird);

    /* 같은 인터페이스로 다른 동작! (다형성) */
    printf("[동물들의 행동]\n");
    
    dog.speak(&dog);
    dog.move(&dog);
    
    printf("\n");
    
    bird.speak(&bird);
    bird.move(&bird);
    
    /* 배열로 관리 */
    printf("\n[동물원 투어]\n");
    Animal zoo[2] = { dog, bird };
    
    for (int i = 0; i < 2; i++) {
        zoo[i].speak(&zoo[i]);
        zoo[i].move(&zoo[i]);
        printf("\n");
    }
    
    printf("💡 C로도 객체지향처럼 다형성을 구현할 수 있어요!\n");
}

/* ============================================================================
 * 8단계: 실무 예제
 * ============================================================================
 */

/* 8-1. 계산기 */
typedef int (*MathOperation)(int, int);

typedef struct Calculator {
    char symbol;
    MathOperation func;
    const char* name;
} Calculator;

int Divide(int a, int b) {
    return b != 0 ? a / b : 0;
}

void Demo_Calculator(void) {
    printf("\n[실무 예제 1] 확장 가능한 계산기\n");
    
    Calculator calc_table[] = {
        { '+', Add, "덧셈" },
        { '-', Subtract, "뺄셈" },
        { '*', Multiply, "곱셈" },
        { '/', Divide, "나눗셈" }
    };
    
    int a = 20, b = 4;
    int table_size = sizeof(calc_table) / sizeof(calc_table[0]);
    
    for (int i = 0; i < table_size; i++) {
        printf("  %d %c %d = %d (%s)\n", 
               a,
               calc_table[i].symbol,
               b,
               calc_table[i].func(a, b),
               calc_table[i].name);
    }
}

// 8-1-추가: 키 입력 받아 계산기 동작하기(만들어봄)
//+, -, *, / 키가 있는 계산기가 있음, 숫자는 2개가 입력되어 있다고 할때 calc_table 을 이용하여 키입력시 처리되는 프로그램 만들기
void Demo_Calculator_Key(void) {
    printf("\n[실무 예제 1-2] 키 입력 계산기 (+, -, *, /)\n");

    int a = 0, b = 0;
    char op;
    Calculator calc_table[] = {
        { '+', Add, "덧셈" },
        { '-', Subtract, "뺄셈" },
        { '*', Multiply, "곱셈" },
        { '/', Divide, "나눗셈" }
    };
    int table_size = sizeof(calc_table) / sizeof(calc_table[0]);
    int found = 0;

    printf("  첫 번째 숫자 입력: ");
    scanf("%d", &a);
    printf("  두 번째 숫자 입력: ");
    scanf("%d", &b);

    // 연산자 입력 및 검증
    while (1) {
        printf("  연산자 입력 (+, -, *, /): ");
        // 앞의 숫자 입력 후 버퍼에 남은 개행 처리
        do {
            op = getchar();
        } while (op == '\n' || op == ' ');

        if (op == '+' || op == '-' || op == '*' || op == '/') {
            break;
        } else {
            printf("  유효하지 않은 연산자입니다. 다시 입력해주세요.\n");
            // 입력 버퍼에 쓰레기값 제거 (필요시)
            //while (getchar() != '\n');
        }
    }

    for (int i = 0; i < table_size; i++) {
        if (calc_table[i].symbol == op) {
            int result = calc_table[i].func(a, b);
            printf("  결과: %d %c %d = %d (%s)\n", a, op, b, result, calc_table[i].name);
            break;
        }
    }
}



/* 8-2. 명령 테이블 (임베디드 시스템) */
typedef void (*CommandHandler)(int param);

void CMD_Reset(int param) {
    printf("    시스템 리셋 실행\n");
}

void CMD_Start(int param) {
    printf("    시스템 시작 (파라미터: %d)\n", param);
}

void CMD_Stop(int param) {
    printf("    시스템 정지\n");
}

typedef struct Command {
    int command_id;
    CommandHandler handler;
    const char* description;        // 설명(사용안함)
} Command;

void Demo_CommandTable(void) {
    printf("\n[실무 예제 2] 명령 테이블 (임베디드)\n");
    
    Command cmd_table[] = {
        { 0x01, CMD_Reset, "시스템 리셋" },
        { 0x02, CMD_Start, "시스템 시작" },
        { 0x03, CMD_Stop, "시스템 정지" }   
    };
    
    int table_size = sizeof(cmd_table) / sizeof(cmd_table[0]);
    
    // 명령 수신 시뮬레이션
    int received_commands[] = { 0x02, 0x03, 0x01 };
    
    for (int i = 0; i < 3; i++) {
        int cmd_id = received_commands[i];
        printf("  명령 수신: 0x%02X\n", cmd_id);
        
        // 명령 테이블에서 찾기
        for (int j = 0; j < table_size; j++) {
            if (cmd_table[j].command_id == cmd_id) {
                cmd_table[j].handler(100);
                break;
            }
        }
    }
}

/* 8-3. 간단한 상태 머신 */
// 상태 정의: 시스템이 가질 수 있는 모든 상태
typedef enum {
    STATE_IDLE,     // 0: 대기 상태
    STATE_RUNNING,  // 1: 실행 상태
    STATE_STOPPED   // 2: 정지 상태
} State;

// 상태 핸들러 함수 포인터 타입: 현재 상태를 처리하고 다음 상태를 반환
typedef State (*StateHandler)(void);

// 상태 핸들러: IDLE(대기) 상태 처리
State StateHandler_Idle(void) {
    printf("    [IDLE] 대기 중...\n");
    return STATE_RUNNING;  // 다음 상태로 전환
}

// 상태 핸들러: RUNNING(실행) 상태 처리
State StateHandler_Running(void) {
    printf("    [RUNNING] 실행 중...\n");
    return STATE_STOPPED;  // 다음 상태로 전환
}

// 상태 핸들러: STOPPED(정지) 상태 처리
State StateHandler_Stopped(void) {
    printf("    [STOPPED] 정지됨\n");
    return STATE_IDLE;  // 다음 상태로 전환
}

void Demo_StateMachine(void) {
    printf("\n[실무 예제 3] 상태 머신\n");
    
    // 상태 핸들러 테이블: 각 상태에 대응하는 핸들러 함수 매핑
    // state_handler_table[STATE_IDLE] -> StateHandler_Idle 함수 호출
    StateHandler state_handler_table[] = {
        StateHandler_Idle,      // [0] STATE_IDLE 핸들러
        StateHandler_Running,   // [1] STATE_RUNNING 핸들러
        StateHandler_Stopped    // [2] STATE_STOPPED 핸들러
    };
    
    State current_state = STATE_IDLE;  // 초기 상태
    
    for (int i = 0; i < 6; i++) {
        printf("  Step %d:\n", i + 1);
        // 현재 상태의 핸들러를 실행하고, 반환된 값을 다음 상태로 설정
        current_state = state_handler_table[current_state]();
        printf("\n");
    }
}

void Step8_RealWorld(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  8단계: 실무 예제\n");
    printf("════════════════════════════════════════\n");
    
    Demo_Calculator();
    Demo_CommandTable();
    Demo_StateMachine();
    
    printf("\n💡 실무에서는 이렇게 함수 포인터를 활용해요!\n");
}

/* ============================================================================
 * 9단계: 대응 포인트
 * ============================================================================
 */

void Step9_Interview(void) {
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  9단계: 대응 포인트\n");
    printf("════════════════════════════════════════\n\n");
    
    printf("📝 자주 나오는 질문들:\n\n");
    
    printf("Q1: 함수 포인터란 무엇인가요?\n");
    printf("A1: 함수의 주소를 저장하는 포인터 변수입니다.\n");
    printf("    런타임에 호출할 함수를 동적으로 선택할 수 있게 해줍니다.\n\n");
    
    printf("Q2: 함수 포인터는 언제 사용하나요?\n");
    printf("A2: 1) 콜백 함수 구현\n");
    printf("    2) 상태 머신 구현\n");
    printf("    3) 명령 테이블 구현\n");
    printf("    4) 플러그인 시스템\n");
    printf("    5) C에서 다형성 구현\n\n");
    
    printf("Q3: void (*fp)(int)와 void *fp(int)의 차이는?\n");
    printf("A3: void (*fp)(int)  → 함수 포인터 (int를 받아 void 반환)\n");
    printf("                         반환값 없음, 함수의 주소를 저장하는 포인터\n");
    printf("    void *fp(int)    → 함수 선언 (int를 받아 void* 반환)\n");
    printf("                         void 포인터를 반환하는 일반 함수\n");
    printf("    ⚠️ void와 void*는 완전히 다릅니다! 괄호 위치가 중요!\n\n");
    
    printf("Q4: typedef를 사용하는 이유는?\n");
    printf("A4: 함수 포인터 문법이 복잡해서 typedef로 타입 별칭을 만들면\n");
    printf("    코드 가독성이 높아지고 유지보수가 쉬워집니다.\n\n");
    
    printf("Q5: 실무에서 사용한 경험은?\n");
    printf("A5: (예시 답변)\n");
    printf("    \"UART 통신 드라이버에서 데이터 수신 완료 시 콜백 함수를\n");
    printf("     호출하도록 구현했습니다. 이를 통해 상위 레이어에서\n");
    printf("     수신 이벤트를 유연하게 처리할 수 있었습니다.\"\n\n");
    
    printf("💡 팁:\n");
    printf("   - 개념을 명확히 설명\n");
    printf("   - 실무 경험과 연결\n");
    printf("   - 간단한 예제 코드 작성 가능해야 함\n");
}

/* ============================================================================
 * 메인 함수
 * ============================================================================
 */

int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                                                            ║\n");
    printf("║     C언어 함수 포인터 완전 정복                            ║\n");
    printf("║     Function Pointer Master Class                          ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    /* 단계별 실행 */
    Step1_Concept();
    Step2_Declaration();
    Step3_Usage();
    Step4_Typedef();
    Step5_Array();
    Step6_Callback();
    Step7_StructWithFP();
    Step8_RealWorld();
    Step9_Interview();
    
    /* 마무리 */
    printf("\n");
    printf("════════════════════════════════════════\n");
    printf("  🎉 축하합니다!\n");
    printf("════════════════════════════════════════\n\n");
    printf("함수 포인터의 모든 것을 학습했습니다!\n\n");
    printf("📚 다음 학습 단계:\n");
    printf("   1. 04_callback_pattern.c - 콜백 패턴 심화\n");
    printf("   2. 05_state_machine.c - 상태 머신 패턴\n");
    printf("   3. 06_factory_pattern.c - 팩토리 패턴\n");
    printf("   4. 07_observer_pattern.c - 옵저버 패턴\n\n");
    
    Demo_Calculator_Key();
    return 0;
}

/*
 * ============================================================================
 * 🎯 학습 요약
 * ============================================================================
 * 
 * ✅ 함수 포인터란?
 *    → 함수의 주소를 저장하는 변수
 * 
 * ✅ 기본 문법:
 *    반환타입 (*포인터이름)(매개변수타입들);
 * 
 * ✅ typedef 활용:
 *    typedef 반환타입 (*타입이름)(매개변수타입들);
 * 
 * ✅ 주요 활용:
 *    - 콜백 함수
 *    - 함수 포인터 배열
 *    - 구조체에 함수 포인터
 *    - 명령 테이블
 *    - 상태 머신
 * 
 * ============================================================================
 * 🔧 컴파일 및 실행
 * ============================================================================
 * 
 * 컴파일:
 *   gcc -o 00_function_pointer.exe 00_function_pointer_basics.c -Wall
 * 
 * 실행:
 *   00_function_pointer.exe
 * 
 * ============================================================================
 * 📚 추가 학습 자료
 * ============================================================================
 * 
 * 이 파일을 완전히 이해했다면:
 * 
 * 1. 다른 디자인 패턴 파일들 (01-31번) 학습
 * 2. 실제 프로젝트에 적용 (센서 드라이버, 통신 스택 등)
 * 3. FreeRTOS 소스 코드 분석
 * 4. Linux Kernel 드라이버 코드 분석
 * 
 * ============================================================================
 * 💡 마지막 팁
 * ============================================================================
 * 
 * 함수 포인터는 처음에는 어려워 보이지만,
 * 몇 번 연습하면 자연스럽게 사용할 수 있습니다!
 * 
 * 핵심은:
 * 1. 문법 이해 (괄호 위치!)
 * 2. typedef 활용 (가독성!)
 * 3. 실무 패턴 숙지 (콜백, 테이블, 상태 머신)
 * 
 * 화이팅! 🚀
 * 
 * ============================================================================
 * 📖 구조체 typedef 작성 방법 비교
 * ============================================================================
 * 
 * C에서 구조체를 typedef로 정의할 때 두 가지 방법이 있습니다.
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 방법 1: 구조체 태그 없이 (비추천)                                        │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * typedef struct {
 *     int command_id;
 *     CommandHandler handler;
 *     const char* description;
 * } Command;
 * 
 * 문제점:
 * 1. 전방 선언 불가능
 *    struct Command* ptr;  // ❌ 에러! 구조체 태그가 없음
 * 
 * 2. 재귀 구조체 정의 불가능
 *    typedef struct {
 *        int data;
 *        Node* next;  // ❌ 에러! Node가 아직 정의되지 않음
 *    } Node;
 * 
 * 3. 디버깅 어려움
 *    - GDB/LLDB에서 타입 이름이 명확하지 않음
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 방법 2: 구조체 태그 명시 (권장)                                          │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * typedef struct Command {
 *     int command_id;
 *     CommandHandler handler;
 *     const char* description;
 * } Command;
 * 
 * 장점:
 * 1. ✅ 전방 선언 가능
 *    struct Command;              // 전방 선언
 *    struct Command* ptr;         // 포인터 사용 가능
 * 
 * 2. ✅ 재귀 구조체 정의 가능
 *    typedef struct Node {
 *        int data;
 *        struct Node* next;       // ✅ 자기 자신 참조 가능
 *    } Node;
 * 
 * 3. ✅ 코드 가독성 향상
 *    - 구조체 이름이 명확히 보임
 *    - 다른 프로그래머가 이해하기 쉬움
 * 
 * 4. ✅ C/C++ 호환성
 *    - C++에서도 동일하게 사용 가능
 * 
 * 5. ✅ 디버깅 편의성
 *    - GDB: print sizeof(struct Command)
 *    - LLDB: type lookup Command
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 실전 예제: 링크드 리스트                                                 │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * // ❌ 불가능 (구조체 태그 없음)
 * typedef struct {
 *     int data;
 *     ListNode* next;  // 에러! ListNode가 아직 정의되지 않음
 * } ListNode;
 * 
 * // ✅ 가능 (구조체 태그 있음)
 * typedef struct ListNode {
 *     int data;
 *     struct ListNode* next;  // struct 키워드로 자기 참조 가능
 * } ListNode;
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 업계 표준 및 코딩 컨벤션                                                 │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * 대부분의 C 프로젝트와 표준에서 구조체 태그 명시를 권장:
 * 
 * 1. Linux Kernel Style Guide
 *    - 항상 구조체 태그 명시
 *    - 예: struct file_operations
 * 
 * 2. MISRA-C 2012 규칙
 *    - Rule 5.7: 태그 이름은 고유해야 함
 *    - 구조체 태그 사용 권장
 * 
 * 3. NASA JPL C Coding Standard
 *    - 모든 구조체는 태그 명시
 *    - typedef와 태그 이름 동일하게 유지
 * 
 * 4. Google C++ Style Guide
 *    - 구조체 태그 명시 (C 호환성)
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 결론                                                                     │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * 항상 다음 형식을 사용하세요:
 * 
 * typedef struct StructName {
 *     // 멤버들...
 * } StructName;
 * 
 * 이것은:
 * - 더 안전하고
 * - 더 유연하며
 * - 업계 표준이며
 * - 디버깅하기 쉽습니다
 * 
 * ============================================================================
 * 
 * ============================================================================
 * 📖 enum typedef 작성 방법 비교
 * ============================================================================
 * 
 * enum(열거형)도 구조체처럼 typedef와 함께 사용할 때 네이밍 규칙이 있습니다.
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 방법 1: enum 태그 없이 (일반적이지만 제한적)                             │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * typedef enum {
 *     STATE_IDLE,
 *     STATE_RUNNING,
 *     STATE_STOPPED
 * } State;
 * 
 * 사용:
 *   State current = STATE_IDLE;  // typedef 이름 사용
 * 
 * 문제점:
 * 1. 전방 선언 불가능
 *    enum State;  // ❌ 에러! enum 태그가 없음
 * 
 * 2. 디버깅 시 타입 이름 명확하지 않음
 *    - GDB/LLDB에서 enum 타입 조회 어려움
 * 
 * 3. C++과의 호환성 제한
 *    - C++은 enum State 형태를 선호
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 방법 2: enum 태그 명시 (권장)                                            │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * typedef enum State {
 *     STATE_IDLE,
 *     STATE_RUNNING,
 *     STATE_STOPPED
 * } State;
 * 
 * 사용:
 *   State current = STATE_IDLE;        // typedef 이름 사용 (간단)
 *   enum State current = STATE_IDLE;   // enum 태그 사용 (명시적)
 * 
 * 장점:
 * 1. ✅ 전방 선언 가능
 *    enum State;              // 전방 선언
 *    State GetState(void);    // 함수 선언
 * 
 * 2. ✅ 디버깅 편의성
 *    - GDB: ptype enum State
 *    - LLDB: type lookup State
 *    - 두 방식 모두 타입 정보 확인 가능
 * 
 * 3. ✅ 코드 가독성 향상
 *    - enum 이름이 명확히 보임
 *    - 다른 프로그래머가 이해하기 쉬움
 * 
 * 4. ✅ C/C++ 호환성
 *    - C++에서도 동일하게 사용 가능
 *    - enum class와 구분 명확
 * 
 * 5. ✅ 스위치문에서 타입 체크
 *    State current = STATE_IDLE;
 *    switch (current) {
 *        case STATE_IDLE: break;
 *        case STATE_RUNNING: break;
 *        case STATE_STOPPED: break;
 *    }
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ enum 네이밍 컨벤션                                                       │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * 1. enum 타입 이름
 *    - PascalCase (일반적): State, CommandType, ErrorCode
 *    - _t 접미사 (Linux 스타일): state_t, command_type_t
 *    - E 접두사 (Hungarian): EState, ECommandType
 * 
 * 2. enum 상수 이름
 *    - 대문자 + 언더스코어 (가장 일반적)
 *    - 타입 이름을 접두사로 사용하여 충돌 방지
 * 
 *    typedef enum State {
 *        STATE_IDLE,        // STATE_ 접두사
 *        STATE_RUNNING,
 *        STATE_STOPPED
 *    } State;
 * 
 *    typedef enum Color {
 *        COLOR_RED,         // COLOR_ 접두사
 *        COLOR_GREEN,
 *        COLOR_BLUE
 *    } Color;
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 실전 예제 비교                                                           │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * // ❌ 태그 없음 - 전방 선언 불가
 * typedef enum {
 *     CMD_START,
 *     CMD_STOP
 * } CommandType;
 * 
 * // ✅ 태그 있음 - 전방 선언 가능
 * typedef enum CommandType {
 *     CMD_START,
 *     CMD_STOP
 * } CommandType;
 * 
 * // 헤더 파일에서
 * enum CommandType;  // 전방 선언 가능
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 업계 표준 및 코딩 컨벤션                                                 │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * 1. Linux Kernel Style Guide
 *    - enum 태그 명시 권장
 *    - 소문자 + 언더스코어 (enum state_type)
 * 
 * 2. MISRA-C 2012 규칙
 *    - Rule 8.12: enum 태그 명시 권장
 *    - 타입 안전성 강화
 * 
 * 3. NASA JPL C Coding Standard
 *    - 모든 enum은 태그 명시
 *    - typedef와 태그 이름 동일하게 유지
 * 
 * 4. Google C++ Style Guide
 *    - enum 태그 명시 (C 호환성)
 *    - enum class 사용 권장 (C++11 이상)
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ struct, enum, union 통일된 패턴                                          │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * 일관성을 위해 struct, enum, union 모두 동일한 패턴 사용:
 * 
 * // 구조체
 * typedef struct StructName {
 *     // 멤버들...
 * } StructName;
 * 
 * // 열거형
 * typedef enum EnumName {
 *     VALUE1,
 *     VALUE2
 * } EnumName;
 * 
 * // 공용체
 * typedef union UnionName {
 *     int i;
 *     float f;
 * } UnionName;
 * 
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ 결론                                                                     │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * enum도 struct와 마찬가지로 다음 형식을 사용하세요:
 * 
 * typedef enum EnumName {
 *     CONSTANT1,
 *     CONSTANT2,
 *     CONSTANT3
 * } EnumName;
 * 
 * 이것은:
 * - 더 안전하고
 * - 더 유연하며
 * - 업계 표준이며
 * - 디버깅하기 쉽고
 * - 전방 선언 가능합니다
 * 
 * ============================================================================
 */
