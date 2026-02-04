/*
 * ============================================================================
 * 파일명: typedef_and_qualifiers.c
 * 주제: typedef, const, static 완전 정복
 * 컴파일: gcc typedef_and_qualifiers.c -o typedef_and_qualifiers
 * 
 * ============================================================================
 * 이 파일의 기능
 * ============================================================================
 * 
 * [typedef]
 * - typedef의 개념과 목적
 * - struct/union/enum에 typedef 사용
 * - typedef를 사용하는 이유
 * - typedef vs #define 차이
 * 
 * [const]
 * - 상수 선언
 * - const 포인터 vs 포인터 to const
 * - 함수 매개변수의 const
 * - const의 위치에 따른 의미
 * 
 * [static]
 * - 지역 변수에서의 static
 * - 전역 변수/함수에서의 static
 * - 파일 범위 제한
 * - 메모리 수명
 * 
 * ============================================================================
 * typedef란?
 * ============================================================================
 * 
 * "type definition"의 약자
 * → 기존 타입에 새로운 이름(별칭)을 부여
 * 
 * 목적:
 * 1. 코드 가독성 향상
 * 2. 복잡한 타입 선언 단순화
 * 3. 이식성 향상 (플랫폼 독립적)
 * 4. 타입 변경 용이
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ============================================================================
 * 예제 1: typedef 기본 개념
 * ============================================================================ */

void Example1_TypedefBasics(void) {
    printf("\n");
    printf("========================================\n");
    printf("예제 1: typedef 기본 개념\n");
    printf("========================================\n");
    
    // ===== typedef 없이 =====
    printf("\n[typedef 없이 변수 선언]\n");
    unsigned long int counter1;
    unsigned long int counter2;
    counter1 = 100;
    counter2 = 200;
    printf("  counter1 = %lu\n", counter1);
    printf("  counter2 = %lu\n", counter2);
    printf("  → 타입 이름이 길고 반복적!\n");
    
    // ===== typedef 사용 =====
    typedef unsigned long int Counter;  // 별칭 정의!
    
    printf("\n[typedef 사용]\n");
    Counter cnt1;  // Counter = unsigned long int
    Counter cnt2;
    cnt1 = 100;
    cnt2 = 200;
    printf("  cnt1 = %lu\n", cnt1);
    printf("  cnt2 = %lu\n", cnt2);
    printf("  → 간결하고 읽기 쉬움!\n");
    
    // ===== 다양한 typedef 예시 =====
    printf("\n[다양한 typedef 예시]\n");
    
    typedef int Integer;
    typedef float Real;
    typedef char* String;
    typedef int Array10[10];
    
    Integer num = 42;
    Real pi = 3.14f;
    String name = "C언어";
    Array10 arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    printf("  Integer num = %d\n", num);
    printf("  Real pi = %.2f\n", pi);
    printf("  String name = %s\n", name);
    printf("  Array10 arr[0] = %d\n", arr[0]);
}

/* ============================================================================
 * 예제 2: struct에 typedef를 사용하는 이유
 * ============================================================================ */

void Example2_TypedefStruct(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 2: struct에 typedef\n");
    printf("========================================\n");
    
    // ===== 방법 1: typedef 없이 =====
    struct Person1 {
        char name[20];
        int age;
    };
    
    printf("\n[방법 1: typedef 없이]\n");
    struct Person1 p1;  // ★ 'struct' 키워드 필수!
    strcpy(p1.name, "김철수");
    p1.age = 30;
    printf("  struct Person1 p1;  // 'struct' 필수\n");
    printf("  이름: %s, 나이: %d\n", p1.name, p1.age);
    
    // ===== 방법 2: typedef 사용 (일반적) =====
    typedef struct {
        char name[20];
        int age;
    } Person2;
    
    printf("\n[방법 2: typedef 사용 (태그 없음)]\n");
    Person2 p2;  // ★ 'struct' 불필요!
    strcpy(p2.name, "이영희");
    p2.age = 25;
    printf("  Person2 p2;  // 간결!\n");
    printf("  이름: %s, 나이: %d\n", p2.name, p2.age);
    
    // ===== 방법 3: typedef + 태그 (권장) =====
    typedef struct Person3 {
        char name[20];
        int age;
    } Person3;
    
    printf("\n[방법 3: typedef + 태그 (권장)]\n");
    Person3 p3;
    strcpy(p3.name, "박민수");
    p3.age = 35;
    printf("  typedef struct Person3 { ... } Person3;\n");
    printf("  이름: %s, 나이: %d\n", p3.name, p3.age);
    
    printf("\n[왜 방법 3이 권장되는가?]\n");
    printf("  1. 전방 선언 가능\n");
    printf("  2. 자기 참조 가능 (linked list 등)\n");
    printf("  3. 디버깅 시 타입 이름 표시\n");
    
    // ===== 자기 참조 예시 =====
    typedef struct Node {
        int data;
        struct Node* next;  // ★ 자기 참조! 태그 필요
    } Node;
    
    printf("\n[자기 참조 예시: Linked List]\n");
    Node n1 = {10, NULL};
    Node n2 = {20, NULL};
    n1.next = &n2;
    printf("  n1.data = %d, n1.next->data = %d\n", n1.data, n1.next->data);
}

/* ============================================================================
 * 예제 3: union과 enum에 typedef
 * ============================================================================ */

void Example3_TypedefUnionEnum(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 3: union과 enum에 typedef\n");
    printf("========================================\n");
    
    // ===== Union with typedef =====
    printf("\n[Union with typedef]\n");
    
    // typedef 없이
    union Data1 {
        int i;
        float f;
    };
    union Data1 d1;  // 'union' 키워드 필요
    
    // typedef 사용
    typedef union Data2 {
        int i;
        float f;
    } Data2;
    Data2 d2;  // 'union' 불필요
    
    d2.i = 100;
    printf("  Data2 d2;  // typedef 사용\n");
    printf("  d2.i = %d\n", d2.i);
    
    // ===== Enum with typedef =====
    printf("\n[Enum with typedef]\n");
    
    // typedef 없이
    enum Color1 { RED1, GREEN1, BLUE1 };
    enum Color1 c1;  // 'enum' 키워드 필요
    
    // typedef 사용
    typedef enum Color2 {
        RED2, 
        GREEN2, 
        BLUE2
    } Color2;
    Color2 c2;  // 'enum' 불필요
    
    c2 = RED2;
    printf("  Color2 c2;  // typedef 사용\n");
    printf("  c2 = RED2 (%d)\n", c2);
    
    printf("\n[결론]\n");
    printf("  typedef를 사용하면:\n");
    printf("  ✅ struct/union/enum 키워드 생략 가능\n");
    printf("  ✅ 코드가 간결해짐\n");
    printf("  ✅ C++처럼 타입 이름만으로 선언 가능\n");
}

/* ============================================================================
 * 예제 4: typedef를 사용하는 이유
 * ============================================================================ */

void Example4_WhyTypedef(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 4: typedef를 사용하는 이유\n");
    printf("========================================\n");
    
    printf("\n[이유 1: 이식성 (Portability)]\n");
    printf("  플랫폼마다 int 크기가 다를 수 있음\n");
    printf("  → typedef로 추상화!\n");
    
    // 플랫폼별 정의 (한 곳만 수정하면 됨)
    #ifdef PLATFORM_32BIT
        typedef int Size;
    #else
        typedef long long Size;
    #endif
    
    printf("\n  typedef로 정의:\n");
    printf("    32bit: typedef int Size;\n");
    printf("    64bit: typedef long long Size;\n");
    printf("  → 코드는 'Size' 사용, 플랫폼별 자동 대응!\n");
    
    printf("\n[이유 2: 복잡한 타입 단순화]\n");
    
    // 복잡한 함수 포인터
    printf("\n  복잡한 함수 포인터:\n");
    printf("    int (*callback)(int, float);  // 읽기 어려움\n");
    
    typedef int (*Callback)(int, float);
    printf("\n  typedef 사용:\n");
    printf("    typedef int (*Callback)(int, float);\n");
    printf("    Callback cb;  // 간단!\n");
    
    printf("\n[이유 3: 의미 전달]\n");
    printf("  int distance;     // 단위가 뭐지?\n");
    printf("  Millimeter dist;  // mm 단위임을 명확히!\n");
    
    typedef int Millimeter;
    typedef int Second;
    
    Millimeter dist = 1000;
    Second time = 60;
    
    printf("  Millimeter dist = %d mm\n", dist);
    printf("  Second time = %d s\n", time);
    
    printf("\n[이유 4: 타입 변경 용이]\n");
    printf("  나중에 float로 바꾸고 싶다면?\n");
    printf("  → typedef 한 줄만 수정!\n");
    printf("    typedef float Millimeter;  // 전체 코드 자동 변경\n");
}

/* ============================================================================
 * 예제 5: const의 의미와 사용법
 * ============================================================================ */

void Example5_Const(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 5: const (상수, 읽기 전용)\n");
    printf("========================================\n");
    
    printf("\n[const란?]\n");
    printf("  'constant'의 약자\n");
    printf("  → 값을 변경할 수 없음 (읽기 전용)\n");
    
    // ===== 기본 사용 =====
    printf("\n[1. 기본 const 변수]\n");
    const int MAX_SIZE = 100;
    printf("  const int MAX_SIZE = 100;\n");
    printf("  MAX_SIZE = %d\n", MAX_SIZE);
    // MAX_SIZE = 200;  // ❌ 컴파일 에러!
    printf("  MAX_SIZE = 200;  // ❌ 에러! 변경 불가\n");
    
    // ===== const 포인터 =====
    printf("\n[2. const 포인터 (복잡!)]\n");
    
    int value = 10;
    
    // 2-1: 포인터가 가리키는 값이 const
    const int* ptr1 = &value;
    printf("\n  const int* ptr1;\n");
    printf("  → *ptr1은 변경 불가, ptr1은 변경 가능\n");
    printf("    *ptr1 = 20;  // ❌ 에러\n");
    printf("    ptr1 = ...;  // ✅ OK\n");
    
    // 2-2: 포인터 자체가 const
    int* const ptr2 = &value;
    printf("\n  int* const ptr2;\n");
    printf("  → ptr2는 변경 불가, *ptr2는 변경 가능\n");
    printf("    *ptr2 = 20;  // ✅ OK\n");
    printf("    ptr2 = ...;  // ❌ 에러\n");
    *ptr2 = 20;
    printf("    실제 변경: *ptr2 = 20 → value = %d\n", value);
    
    // 2-3: 둘 다 const
    const int* const ptr3 = &value;
    printf("\n  const int* const ptr3;\n");
    printf("  → 둘 다 변경 불가!\n");
    printf("    *ptr3 = 30;  // ❌ 에러\n");
    printf("    ptr3 = ...;  // ❌ 에러\n");
    
    // ===== 외우는 방법 =====
    printf("\n[★ const 외우는 방법 ★]\n");
    printf("  오른쪽에서 왼쪽으로 읽기!\n");
    printf("\n  const int* ptr;\n");
    printf("  → ptr은 int에 대한 포인터, int가 const\n");
    printf("  → '가리키는 값'이 const\n");
    printf("\n  int* const ptr;\n");
    printf("  → ptr은 const, int*\n");
    printf("  → '포인터 자체'가 const\n");
}

/* ============================================================================
 * 예제 6: const 실전 활용
 * ============================================================================ */

void PrintArray(const int* arr, int size) {
    printf("  배열: [");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
    
    // arr[0] = 999;  // ❌ 에러! const이므로 수정 불가
}

void Example6_ConstPractical(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 6: const 실전 활용\n");
    printf("========================================\n");
    
    printf("\n[함수 매개변수에 const]\n");
    printf("  void PrintArray(const int* arr, int size);\n");
    printf("  → 함수 내에서 배열을 수정하지 않음을 보장\n");
    
    int numbers[] = {10, 20, 30, 40, 50};
    PrintArray(numbers, 5);
    
    printf("\n[const의 장점]\n");
    printf("  ✅ 실수로 수정하는 것 방지\n");
    printf("  ✅ 컴파일러 최적화 가능\n");
    printf("  ✅ 의도 명확히 전달 (읽기 전용)\n");
    
    // ===== const struct =====
    typedef struct {
        int x;
        int y;
    } Point;
    
    const Point origin = {0, 0};
    printf("\n[const struct]\n");
    printf("  const Point origin = {0, 0};\n");
    printf("  origin.x = %d, origin.y = %d\n", origin.x, origin.y);
    // origin.x = 10;  // ❌ 에러!
    printf("  origin.x = 10;  // ❌ 에러! 변경 불가\n");
}

/* ============================================================================
 * 예제 7: static의 의미와 사용법
 * ============================================================================ */

// 전역 변수
int g_global = 100;
static int g_static_global = 200;

void Example7_Static(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 7: static (범위/수명 제어)\n");
    printf("========================================\n");
    
    printf("\n[static이란?]\n");
    printf("  사용 위치에 따라 의미가 다름!\n");
    printf("  1. 지역 변수: 수명 연장 (함수 종료 후에도 유지)\n");
    printf("  2. 전역 변수/함수: 파일 범위 제한\n");
    
    printf("\n[1. 지역 변수에서의 static]\n");
    
    printf("\n  일반 지역 변수:\n");
    for (int i = 0; i < 3; i++) {
        int count = 0;
        count++;
        printf("    count = %d (매번 초기화됨)\n", count);
    }
    
    printf("\n  static 지역 변수:\n");
    for (int i = 0; i < 3; i++) {
        static int count = 0;  // ★ 최초 1번만 초기화!
        count++;
        printf("    count = %d (값이 유지됨!)\n", count);
    }
    
    printf("\n[메모리 위치]\n");
    printf("  일반 지역 변수: 스택 (Stack)\n");
    printf("  static 변수:    데이터 영역 (Data Segment)\n");
    printf("  → 프로그램 종료까지 유지!\n");
}

/* ============================================================================
 * 예제 8: static 함수 호출 카운터
 * ============================================================================ */

int GetUniqueID(void) {
    static int id_counter = 0;  // ★ 한 번만 초기화
    id_counter++;
    return id_counter;
}

void Example8_StaticCounter(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 8: static 활용 - 호출 카운터\n");
    printf("========================================\n");
    
    printf("\n[실전 활용: 고유 ID 생성기]\n");
    printf("  int GetUniqueID(void) {\n");
    printf("    static int id_counter = 0;\n");
    printf("    id_counter++;\n");
    printf("    return id_counter;\n");
    printf("  }\n");
    
    printf("\n  호출 결과:\n");
    for (int i = 0; i < 5; i++) {
        printf("    ID %d: %d\n", i + 1, GetUniqueID());
    }
    
    printf("\n[왜 static을 쓰는가?]\n");
    printf("  전역 변수 없이 상태 유지 가능!\n");
    printf("  → 전역 변수보다 안전 (외부 접근 불가)\n");
}

/* ============================================================================
 * 예제 9: static 전역 변수/함수 (파일 범위 제한)
 * ============================================================================ */

// 이 파일에서만 접근 가능한 함수
static void PrivateFunction(void) {
    printf("  이 함수는 파일 내부에서만 호출 가능\n");
}

// 일반 함수 (다른 파일에서 extern으로 접근 가능)
void PublicFunction(void) {
    printf("  이 함수는 다른 파일에서도 접근 가능\n");
}

void Example9_StaticFileScope(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 9: static 전역 변수/함수\n");
    printf("========================================\n");
    
    printf("\n[2. 전역 변수/함수에서의 static]\n");
    printf("  → 해당 파일 내부에서만 접근 가능\n");
    
    printf("\n  일반 전역 변수:\n");
    printf("    int g_global = 100;\n");
    printf("    → 다른 파일에서 'extern int g_global;'로 접근 가능\n");
    
    printf("\n  static 전역 변수:\n");
    printf("    static int g_static_global = 200;\n");
    printf("    → 이 파일 내부에서만 접근 가능 (캡슐화!)\n");
    
    printf("\n[함수도 마찬가지]\n");
    printf("  static void PrivateFunction(void);  // 파일 내부만\n");
    printf("  void PublicFunction(void);          // 외부 접근 가능\n");
    
    PrivateFunction();
    PublicFunction();
    
    printf("\n[왜 사용하는가?]\n");
    printf("  ✅ 이름 충돌 방지\n");
    printf("  ✅ 내부 구현 숨김 (캡슐화)\n");
    printf("  ✅ 의도하지 않은 외부 접근 차단\n");
}

/* ============================================================================
 * 예제 10: const + static 조합
 * ============================================================================ */

void Example10_ConstStatic(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 10: const + static 조합\n");
    printf("========================================\n");
    
    // 파일 내부에서만 접근 가능한 상수
    static const int MAX_BUFFER_SIZE = 256;
    static const char* VERSION = "1.0.0";
    
    printf("\n[static const 조합]\n");
    printf("  static const int MAX_BUFFER_SIZE = 256;\n");
    printf("  static const char* VERSION = \"1.0.0\";\n");
    
    printf("\n  의미:\n");
    printf("  - const:  값 변경 불가\n");
    printf("  - static: 이 파일에서만 접근 가능\n");
    
    printf("\n  결과:\n");
    printf("    MAX_BUFFER_SIZE = %d\n", MAX_BUFFER_SIZE);
    printf("    VERSION = %s\n", VERSION);
    
    printf("\n[임베디드에서 자주 사용]\n");
    printf("  static const uint8_t DEVICE_ID = 0x42;\n");
    printf("  static const uint32_t BAUDRATE = 115200;\n");
    printf("  → 변경 불가 + 파일 내부 전용\n");
}

/* ============================================================================
 * 예제 11: typedef vs #define
 * ============================================================================ */

void Example11_TypedefVsDefine(void) {
    printf("\n\n");
    printf("========================================\n");
    printf("예제 11: typedef vs #define\n");
    printf("========================================\n");
    
    printf("\n[비교]\n");
    
    #define INT_DEFINE int
    typedef int INT_TYPEDEF;
    
    printf("  #define INT_DEFINE int\n");
    printf("  typedef int INT_TYPEDEF;\n");
    
    printf("\n[차이점 1: 전처리기 vs 컴파일러]\n");
    printf("  #define:  전처리 단계에서 텍스트 치환\n");
    printf("  typedef:  컴파일러가 타입으로 인식\n");
    
    printf("\n[차이점 2: 포인터 선언]\n");
    
    #define PTR_DEFINE int*
    typedef int* PTR_TYPEDEF;
    
    PTR_DEFINE a, b;     // int* a, b → a는 포인터, b는 int!
    PTR_TYPEDEF c, d;    // 둘 다 포인터!
    
    printf("  #define PTR_DEFINE int*\n");
    printf("  PTR_DEFINE a, b;  → int* a, b (b는 int!)\n");
    printf("\n  typedef int* PTR_TYPEDEF;\n");
    printf("  PTR_TYPEDEF c, d;  → 둘 다 int* (일관성!)\n");
    
    printf("\n[차이점 3: 타입 체크]\n");
    printf("  #define:  타입 체크 없음 (단순 치환)\n");
    printf("  typedef:  타입 체크 있음 (컴파일러)\n");
    
    printf("\n[결론]\n");
    printf("  ✅ 타입 별칭은 typedef 사용 (안전)\n");
    printf("  ✅ #define은 상수 정의에 사용\n");
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("typedef, const, static 완전 정복\n");
    printf("========================================\n");
    
    Example1_TypedefBasics();
    Example2_TypedefStruct();
    Example3_TypedefUnionEnum();
    Example4_WhyTypedef();
    Example5_Const();
    Example6_ConstPractical();
    Example7_Static();
    Example8_StaticCounter();
    Example9_StaticFileScope();
    Example10_ConstStatic();
    Example11_TypedefVsDefine();
    
    printf("\n\n");
    printf("========================================\n");
    printf("핵심 정리\n");
    printf("========================================\n");
    
    printf("\n[typedef]\n");
    printf("  목적: 기존 타입에 새 이름 부여\n");
    printf("  이유:\n");
    printf("    ✅ 코드 간결화 (struct/union/enum 키워드 생략)\n");
    printf("    ✅ 이식성 향상\n");
    printf("    ✅ 복잡한 타입 단순화\n");
    printf("    ✅ 의미 전달\n");
    printf("  사용:\n");
    printf("    typedef struct Point { int x, y; } Point;\n");
    printf("    typedef union Data { ... } Data;\n");
    printf("    typedef enum Color { ... } Color;\n");
    
    printf("\n[const]\n");
    printf("  의미: 상수, 읽기 전용\n");
    printf("  위치:\n");
    printf("    const int* ptr;        // 가리키는 값이 const\n");
    printf("    int* const ptr;        // 포인터가 const\n");
    printf("    const int* const ptr;  // 둘 다 const\n");
    printf("  장점:\n");
    printf("    ✅ 실수 방지\n");
    printf("    ✅ 컴파일러 최적화\n");
    printf("    ✅ 의도 명확\n");
    
    printf("\n[static]\n");
    printf("  지역 변수:\n");
    printf("    - 수명 연장 (프로그램 종료까지)\n");
    printf("    - 값 유지 (함수 호출 간)\n");
    printf("  전역 변수/함수:\n");
    printf("    - 파일 범위 제한\n");
    printf("    - 외부 접근 차단 (캡슐화)\n");
    printf("  장점:\n");
    printf("    ✅ 전역 변수 없이 상태 유지\n");
    printf("    ✅ 이름 충돌 방지\n");
    printf("    ✅ 내부 구현 숨김\n");
    
    printf("\n[조합 사용]\n");
    printf("  typedef struct Sensor { ... } Sensor;  // 구조체 타입\n");
    printf("  const int MAX = 100;                   // 상수\n");
    printf("  static const int ID = 42;              // 파일 전용 상수\n");
    printf("  static void Init(void);                // 파일 전용 함수\n");
    
    printf("\n========================================\n");
    
    return 0;
}

/* ============================================================================
 * 학습 포인트
 * ============================================================================
 * 
 * 1. typedef
 *    - 타입의 별칭 (alias)
 *    - struct/union/enum과 함께 사용하면 키워드 생략 가능
 *    - 코드 가독성, 이식성, 유지보수성 향상
 * 
 * 2. const
 *    - 값을 변경할 수 없음 (읽기 전용)
 *    - 포인터와 함께 사용 시 위치 중요
 *    - 함수 매개변수, 반환값에 사용하여 의도 명확히
 * 
 * 3. static
 *    - 지역: 수명 연장, 값 유지
 *    - 전역: 파일 범위 제한, 캡슐화
 *    - 임베디드에서 전역 변수 대신 사용 권장
 * 
 * 4. 조합
 *    - typedef struct: 구조체 타입 정의
 *    - const static: 파일 전용 상수
 *    - static 함수: 내부 헬퍼 함수
 * 
 * 5. 임베디드 코딩 스타일
 *    - typedef로 타입 명확히
 *    - const로 상수 정의
 *    - static으로 내부 구현 숨김
 * 
 * ============================================================================
 */
