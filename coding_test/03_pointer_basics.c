/**
 * ============================================================================
 * [코딩테스트 Level 3] 포인터와 참조에 의한 전달
 * ============================================================================
 * 
 * 문제 설명:
 * 포인터는 C 언어의 핵심 기능으로, 메모리 주소를 직접 다룰 수 있게 해줍니다.
 * 함수에서 여러 값을 반환하거나, 큰 구조체를 효율적으로 전달할 때 필수입니다.
 * 
 * 요구사항:
 * 1. 포인터를 사용하여 두 변수의 값을 교환하는 swap 함수를 작성하세요.
 * 2. 포인터를 통해 함수에서 여러 값을 반환하는 함수를 작성하세요.
 * 3. 배열과 포인터의 관계를 이해하고 활용하세요.
 * 4. NULL 포인터를 적절히 검사하는 코드를 작성하세요.
 * 
 * 학습 포인트:
 * - 포인터 선언과 역참조 (*, &)
 * - 값에 의한 전달 vs 참조에 의한 전달
 * - 함수에서 포인터로 값 반환하기
 * - NULL 포인터 검증
 * - 배열과 포인터의 관계
 * 
 * ============================================================================
 * File: 03_pointer_basics.c
 * 파일명: 03_pointer_basics.c
 * Author / 작성자: [Developer]
 * Date / 작성일: 2026-01-14
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* ============================================================================
 * Windows UTF-8 Console Output Support / 윈도우 UTF-8 콘솔 출력 지원
 * ============================================================================ */
#ifdef _WIN32
    #include <windows.h>
    #include <locale.h>
    #define ENABLE_UTF8_CONSOLE()   do { \
        SetConsoleOutputCP(CP_UTF8); \
        SetConsoleCP(CP_UTF8); \
        setlocale(LC_ALL, ".UTF-8"); \
    } while(0)
#else
    #include <locale.h>
    #define ENABLE_UTF8_CONSOLE()   setlocale(LC_ALL, "")
#endif

/* ============================================================================
 * Error Codes / 에러 코드
 * ============================================================================ */
#define SUCCESS             0
#define ERR_NULL_POINTER    -1
#define ERR_INVALID_PARAM   -2

/* ============================================================================
 * Pointer Basics Demonstration / 포인터 기초 시연
 * ============================================================================ */

/**
 * @brief Demonstrate pointer declaration and dereferencing
 *        포인터 선언과 역참조 시연
 * 
 * 포인터 연산자:
 * - & (주소 연산자): 변수의 메모리 주소를 반환
 * - * (역참조 연산자): 포인터가 가리키는 주소의 값에 접근
 * 
 * Pointer operators:
 * - & (address-of): Returns memory address of a variable
 * - * (dereference): Accesses value at the address pointer points to
 */
void demonstratePointerBasics(void) {
    printf("============ Pointer Basics / 포인터 기초 ============\n");
    
    int32_t value = 42;
    int32_t* ptr = &value;      /* ptr은 value의 주소를 저장 */
                                /* ptr stores address of value */
    // int32_t* ptr2 = 42; /* error */
    // printf("ptr2 = %p\r\n", ptr2);
    
    printf("변수 value:\n");
    printf("  Value / 값:     %d\n", value);
    printf("  Address / 주소: %p\n", (void*)&value);
    
    printf("\n포인터 ptr:\n");
    printf("  ptr 자체의 값 (주소):  %p\n", (void*)ptr);
    printf("  *ptr (역참조한 값):    %d\n", *ptr);
    printf("  &ptr (ptr의 주소):     %p\n", (void*)&ptr);
    
    /* 포인터를 통해 값 변경 */
    /* Modify value through pointer */
    *ptr = 100;
    printf("\n*ptr = 100 실행 후:\n");
    printf("  value = %d (포인터로 변경됨)\n", value);
    
    printf("========================================================\n");
}

/**
 * @brief Swap two values - WRONG way (pass by value)
 *        두 값 교환 - 잘못된 방법 (값에 의한 전달)
 * 
 * 이 함수는 작동하지 않습니다!
 * 매개변수는 복사본이므로 원본에 영향을 주지 않습니다.
 * 
 * This function does NOT work!
 * Parameters are copies, so original values are not affected.
 */
void swapWrong(int32_t a, int32_t b) {
    int32_t temp = a;
    a = b;
    b = temp;
    printf("  [Inside swapWrong] a=%d, b=%d\n", a, b);
    /* 함수가 끝나면 a, b 복사본은 사라짐 */
    /* When function ends, copies a, b are destroyed */
}

/**
 * @brief Swap two values - CORRECT way (pass by pointer)
 *        두 값 교환 - 올바른 방법 (포인터로 전달)
 * @param a Pointer to first value / 첫 번째 값의 포인터
 * @param b Pointer to second value / 두 번째 값의 포인터
 * @return SUCCESS or error code / 성공 또는 에러 코드
 * 
 * 포인터를 사용하면 원본 변수를 직접 수정할 수 있습니다.
 * Using pointers allows direct modification of original variables.
 */
int swapCorrect(int32_t* a, int32_t* b) {

    assert(a != NULL);
    assert(b != NULL);
    /* NULL 포인터 검사 */
    /* NULL pointer check */
    /* NULL 포인터 검사 - 필수! */
    /* NULL pointer check - Essential! */
    if (a == NULL || b == NULL) {
        printf("  [ERROR] NULL pointer detected!\n");
        return ERR_NULL_POINTER;
    }
    
    int32_t temp = *a;      /* a가 가리키는 값을 temp에 저장 */
    *a = *b;                /* b가 가리키는 값을 a가 가리키는 위치에 저장 */
    *b = temp;              /* temp 값을 b가 가리키는 위치에 저장 */
    
    printf("  [Inside swapCorrect] *a=%d, *b=%d\n", *a, *b);
    return SUCCESS;
}

/**
 * @brief Return multiple values through pointers
 *        포인터를 통해 여러 값 반환
 * @param input Input value / 입력 값
 * @param quotient Output: quotient / 출력: 몫
 * @param remainder Output: remainder / 출력: 나머지
 * @param divisor Divisor / 나누는 수
 * @return SUCCESS or error code / 성공 또는 에러 코드
 * 
 * C 함수는 하나의 값만 return할 수 있지만,
 * 포인터 매개변수를 통해 여러 값을 "반환"할 수 있습니다.
 * 
 * C functions can only return one value,
 * but can "return" multiple values through pointer parameters.
 */
int divideWithRemainder(int32_t input, int32_t divisor, 
                        int32_t* quotient, int32_t* remainder) {
    /* NULL 포인터 검사 */
    if (quotient == NULL || remainder == NULL) {
        return ERR_NULL_POINTER;
    }
    
    /* 0으로 나누기 방지 */
    if (divisor == 0) {
        return ERR_INVALID_PARAM;
    }
    
    *quotient = input / divisor;    /* 몫을 포인터가 가리키는 위치에 저장 */
    *remainder = input % divisor;   /* 나머지를 포인터가 가리키는 위치에 저장 */
    
    return SUCCESS;
}

/**
 * @brief Find min and max in array using pointers
 *        포인터를 사용하여 배열의 최소/최대값 찾기
 * @param arr Array of integers / 정수 배열
 * @param size Array size / 배열 크기
 * @param min Output: minimum value / 출력: 최소값
 * @param max Output: maximum value / 출력: 최대값
 * @return SUCCESS or error code / 성공 또는 에러 코드
 */
int findMinMax(const int32_t* arr, size_t size, int32_t* min, int32_t* max) {
    /* NULL 포인터 검사 */
    if (arr == NULL || min == NULL || max == NULL) {
        return ERR_NULL_POINTER;
    }
    
    /* 빈 배열 검사 */
    if (size == 0) {
        return ERR_INVALID_PARAM;
    }
    
    *min = arr[0];
    *max = arr[0];
    
    /* 배열 순회 - 포인터 연산 사용 */
    /* Traverse array - using pointer arithmetic */
    for (size_t i = 1; i < size; i++) {
        if (arr[i] < *min) {
            *min = arr[i];
        }
        if (arr[i] > *max) {
            *max = arr[i];
        }
    }
    
    return SUCCESS;
}

/**
 * @brief Demonstrate array and pointer relationship
 *        배열과 포인터의 관계 시연
 * 
 * 배열 이름은 첫 번째 요소의 주소를 나타냅니다.
 * arr == &arr[0]
 * 
 * Array name represents address of first element.
 * arr == &arr[0]
 */
void demonstrateArrayPointer(void) {
    printf("\n============ Array & Pointer / 배열과 포인터 ============\n");
    
    int32_t arr[5] = {10, 20, 30, 40, 50};
    int32_t* ptr = arr;     /* 배열 이름은 첫 요소의 주소 */
                            /* Array name is address of first element */
    
    printf("arr = %d \r\n", arr);
    printf("arr[0] = %d \r\n", arr[0]);
    printf("ptr = %d\r\n", ptr);
    printf("*ptr = %d \r\n", *ptr);
    printf("ptr = %p \r\n", ptr);
    printf("ptr + 1 = %p\n", ptr + 1);  // 0x1004 (4바이트 증가)
    printf("ptr + 2 = %p\n", ptr + 2);  // 0x1008 (8바이트 증가)

    printf("배열 접근 방법 비교:\n");
    printf("%-12s %-12s %-12s\n", "Index", "arr[i]", "*(ptr+i)");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < 5; i++) {
        printf("%-12d %-12d %-12d\n", i, arr[i], *(ptr + i));
    }
    
    printf("\n주소 비교:\n");
    printf("  arr     = %p\n", (void*)arr);
    printf("  &arr[0] = %p\n", (void*)&arr[0]);
    printf("  ptr     = %p\n", (void*)ptr);
    printf("  -> 모두 같은 주소!\n");
    
    printf("\n포인터 연산 (ptr + 1의 의미):\n");
    printf("  ptr     = %p (arr[0]의 주소)\n", (void*)ptr);
    printf("  ptr + 1 = %p (arr[1]의 주소)\n", (void*)(ptr + 1));
    printf("  차이: %zu bytes (int32_t 크기만큼 증가)\n", 
           (size_t)((char*)(ptr + 1) - (char*)ptr));
    printf("==========================================================\n");
    printf("(char*)(ptr + 1) = %p\r\n", (char*)(ptr + 1));
    printf("(char*)ptr = %p\r\n", (char*)ptr);
    printf("(char*)(ptr + 1) - (char*)ptr = %zu\r\n", (char*)(ptr + 1) - (char*)ptr);
    printf("==========================================================\n");
    printf("sizeof(int32_t) = %zu\r\n", sizeof(int32_t));
    printf("sizeof(char) = %zu\r\n", sizeof(char));
    printf("sizeof(size_t) = %zu\r\n", sizeof(size_t));
    printf("sizeof(void*) = %zu\r\n", sizeof(void*));
    printf("sizeof(void**) = %zu\r\n", sizeof(void**));
    printf("sizeof(void***) = %zu\r\n", sizeof(void***));
    printf("sizeof(void****) = %zu\r\n", sizeof(void****));
    printf("==========================================================\n");
}

/**
 * @brief Demonstrate pointer to pointer (double pointer)
 *        이중 포인터 시연
 * 
 * 이중 포인터: 포인터의 주소를 저장하는 포인터
 * Double pointer: A pointer that stores address of another pointer
 */
void demonstrateDoublePointer(void) {
    printf("\n============ Double Pointer / 이중 포인터 ============\n");
    
    int32_t value = 42;
    int32_t* ptr = &value;      /* 단일 포인터: value의 주소 */
    int32_t** pptr = &ptr;      /* 이중 포인터: ptr의 주소 */
    
    printf("value  = %d\n", value);
    printf("*ptr   = %d (ptr을 역참조)\n", *ptr);
    printf("**pptr = %d (pptr을 두 번 역참조)\n", **pptr);
    
    printf("\n주소 관계:\n");
    printf("  &value = %p\n", (void*)&value);
    printf("  ptr    = %p (value의 주소 저장)\n", (void*)ptr);
    printf("  &ptr   = %p\n", (void*)&ptr);
    printf("  pptr   = %p (ptr의 주소 저장)\n", (void*)pptr);
    
    /* 이중 포인터로 값 변경 */
    **pptr = 999;
    printf("\n**pptr = 999 실행 후:\n");
    printf("  value = %d\n", value);
    
    printf("========================================================\n");
}

/**
 * @brief Demonstrate const with pointers
 *        포인터와 const 키워드 시연
 * 
 * const 위치에 따른 의미:
 * 1. const int* ptr     : 포인터가 가리키는 값을 변경 불가
 * 2. int* const ptr     : 포인터 자체를 변경 불가
 * 3. const int* const ptr : 둘 다 변경 불가
 */
void demonstrateConstPointer(void) {
    printf("\n============ const with Pointers / const와 포인터 ============\n");
    
    int32_t a = 10, b = 20;
    
    /* 1. Pointer to const: 값 변경 불가 */
    const int32_t* ptr1 = &a;
    printf("1. const int* ptr (포인터가 가리키는 값 변경 불가):\n");
    printf("   *ptr1 = %d\n", *ptr1);
    /* *ptr1 = 100; */  /* 컴파일 에러! */
    ptr1 = &b;          /* 포인터는 변경 가능 */
    printf("   ptr1 = &b 후: *ptr1 = %d\n", *ptr1);
    
    /* 2. Const pointer: 포인터 변경 불가 */
    int32_t* const ptr2 = &a;
    printf("\n2. int* const ptr (포인터 자체 변경 불가):\n");
    *ptr2 = 100;        /* 값은 변경 가능 */
    printf("   *ptr2 = 100 후: a = %d\n", a);
    /* ptr2 = &b; */    /* 컴파일 에러! */
    
    /* 3. Const pointer to const: 둘 다 불가 */
    const int32_t* const ptr3 = &a;
    printf("\n3. const int* const ptr (둘 다 변경 불가):\n");
    printf("   *ptr3 = %d (읽기만 가능)\n", *ptr3);
    /* *ptr3 = 200; */  /* 컴파일 에러! */
    /* ptr3 = &b; */    /* 컴파일 에러! */
    
    printf("==============================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 3: Pointer Basics\n");
    printf("  Level 3: 포인터와 참조에 의한 전달\n");
    printf("========================================\n\n");
    
    /* Test 1: 포인터 기초 */
    printf("[Test 1] Pointer Basics / 포인터 기초\n");
    demonstratePointerBasics();
    
    /* Test 2: 값 교환 - 잘못된 방법 vs 올바른 방법 */
    printf("\n[Test 2] Swap Values / 값 교환\n");
    int32_t x = 10, y = 20;
    
    printf("Before swap / 교환 전: x=%d, y=%d\n", x, y);
    
    printf("\nUsing swapWrong (pass by value / 값으로 전달):\n");
    swapWrong(x, y);
    printf("After swapWrong / 후: x=%d, y=%d (변경 안됨!)\n", x, y);
    
    printf("\nUsing swapCorrect (pass by pointer / 포인터로 전달):\n");
    swapCorrect(&x, &y);
    printf("After swapCorrect / 후: x=%d, y=%d (변경됨!)\n", x, y);
    
    /* Test 3: 여러 값 반환 */
    printf("\n[Test 3] Return Multiple Values / 여러 값 반환\n");
    int32_t quot, rem;
    int32_t dividend = 17, divisor = 5;
    
    int result = divideWithRemainder(dividend, divisor, &quot, &rem);
    if (result == SUCCESS) {
        printf("%d / %d = %d remainder %d\n", dividend, divisor, quot, rem);
        printf("%d / %d = %d 나머지 %d\n", dividend, divisor, quot, rem);
    }
    
    /* Test 4: 배열에서 최소/최대값 찾기 */
    printf("\n[Test 4] Find Min/Max in Array / 배열 최소/최대값\n");
    int32_t numbers[] = {23, 1, 45, 7, 89, 12, 56};
    size_t numCount = sizeof(numbers) / sizeof(numbers[0]);
    int32_t minVal, maxVal;
    
    result = findMinMax(numbers, numCount, &minVal, &maxVal);
    if (result == SUCCESS) {
        printf("Array / 배열: ");
        for (size_t i = 0; i < numCount; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\nMin / 최소: %d, Max / 최대: %d\n", minVal, maxVal);
    }
    
    /* Test 5: 배열과 포인터 관계 */
    printf("\n[Test 5] Array & Pointer Relationship / 배열과 포인터 관계\n");
    demonstrateArrayPointer();
    
    /* Test 6: 이중 포인터 */
    printf("\n[Test 6] Double Pointer / 이중 포인터\n");
    demonstrateDoublePointer();
    
    /* Test 7: const와 포인터 */
    printf("\n[Test 7] const with Pointers / const와 포인터\n");
    demonstrateConstPointer();
    
    /* Test 8: NULL 포인터 처리 */
    printf("\n[Test 8] NULL Pointer Handling / NULL 포인터 처리\n");
    result = swapCorrect(NULL, &y);
    printf("swapCorrect(NULL, &y) returned: %d (에러 코드)\n", result);
    
    printf("\n========================================\n");
    printf("  All Tests Completed\n");
    printf("  모든 테스트 완료\n");
    printf("========================================\n");
    
    return 0;
}

/**
 * ============================================================================
 * Expected Output / 예상 출력:
 * ============================================================================
 * 
 * [Test 2] 값 교환
 * - swapWrong: 원본 변경 안됨 (값에 의한 전달)
 * - swapCorrect: 원본 변경됨 (포인터로 전달)
 * 
 * [Test 3] 17 / 5 = 3 나머지 2
 * 
 * [Test 4] Min: 1, Max: 89
 * 
 * ============================================================================
 */
