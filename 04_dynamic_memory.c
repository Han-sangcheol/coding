/**
 * ============================================================================
 * [코딩테스트 Level 4] 동적 메모리 할당
 * ============================================================================
 * 
 * 문제 설명:
 * 동적 메모리 할당은 프로그램 실행 중에 필요한 만큼의 메모리를 할당받는 기술입니다.
 * 임베디드 시스템에서는 주로 초기화 시에만 동적 할당을 사용하고,
 * 이후에는 정적 메모리를 사용하는 것이 일반적입니다.
 * 
 * 요구사항:
 * 1. malloc()으로 정수 배열을 동적 할당하세요.
 * 2. calloc()으로 0으로 초기화된 배열을 할당하세요.
 * 3. realloc()으로 배열 크기를 변경하세요.
 * 4. free()로 할당된 메모리를 해제하세요.
 * 5. 메모리 누수를 방지하는 패턴을 익히세요.
 * 
 * 학습 포인트:
 * - malloc(), calloc(), realloc(), free() 사용법
 * - 메모리 누수 방지
 * - memset()으로 초기화
 * - 동적 할당 실패 처리
 * 
 * ============================================================================
 * File: 04_dynamic_memory.c
 * 파일명: 04_dynamic_memory.c
 * Author / 작성자: [Developer]
 * Date / 작성일: 2026-01-14
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>     /* malloc, calloc, realloc, free */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>     /* memset, memcpy */

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
#define ERR_ALLOC_FAILED    -2
#define ERR_INVALID_SIZE    -3

/* ============================================================================
 * Structure for Dynamic Buffer / 동적 버퍼 구조체
 * ============================================================================ */

/**
 * Dynamic Integer Array Structure / 동적 정수 배열 구조체
 * 
 * 이 구조체는 동적 배열의 전형적인 패턴을 보여줍니다:
 * - 데이터 포인터 (동적 할당된 메모리)
 * - 현재 사용 중인 크기
 * - 할당된 용량
 */
typedef struct {
    int32_t*    data;       /* Pointer to dynamically allocated array / 동적 할당 배열 포인터 */
    size_t      size;       /* Current number of elements / 현재 요소 개수 */
    size_t      capacity;   /* Allocated capacity / 할당된 용량 */
} DynamicArray_t;

/* ============================================================================
 * Function Implementations / 함수 구현
 * ============================================================================ */

/**
 * @brief Demonstrate malloc() function / malloc() 함수 시연
 * 
 * malloc (Memory Allocation):
 * - 지정된 바이트 수만큼 메모리 할당
 * - 할당된 메모리는 초기화되지 않음 (쓰레기 값 포함)
 * - 실패 시 NULL 반환
 * 
 * 형식: void* malloc(size_t size);
 */
void demonstrateMalloc(void) {
    printf("============ malloc() Demo / malloc() 시연 ============\n");
    
    size_t count = 5;
    
    /* malloc으로 정수 배열 할당 */
    /* Allocate integer array with malloc */
    int32_t* arr = (int32_t*)malloc(count * sizeof(int32_t));
    
    /* 할당 실패 검사 - 필수! */
    /* Check allocation failure - Essential! */
    if (arr == NULL) {
        printf("[ERROR] malloc failed! / 메모리 할당 실패!\n");
        return;
    }
    
    printf("malloc 할당 성공: %zu bytes (%zu integers)\n", 
           count * sizeof(int32_t), count);
    
    /* malloc은 초기화하지 않음 - 쓰레기 값 출력 */
    /* malloc doesn't initialize - garbage values */
    printf("초기화 전 (쓰레기 값): ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    /* memset으로 0 초기화 */
    /* Initialize to 0 with memset */
    memset(arr, 0, count * sizeof(int32_t));
    printf("memset 후: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    /* 값 할당 */
    for (size_t i = 0; i < count; i++) {
        arr[i] = (int32_t)(i * 10);
    }
    printf("값 할당 후: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    /* 메모리 해제 - 필수! */
    /* Free memory - Essential! */
    free(arr);
    arr = NULL;     /* Dangling pointer 방지 / Prevent dangling pointer */
    printf("메모리 해제 완료 (arr = NULL)\n");
    
    printf("========================================================\n");
}

/**
 * @brief Demonstrate calloc() function / calloc() 함수 시연
 * 
 * calloc (Contiguous Allocation):
 * - 지정된 개수 × 크기만큼 메모리 할당
 * - 할당된 메모리를 0으로 초기화함
 * - 실패 시 NULL 반환
 * 
 * 형식: void* calloc(size_t count, size_t size);
 */
void demonstrateCalloc(void) {
    printf("\n============ calloc() Demo / calloc() 시연 ============\n");
    
    size_t count = 5;
    
    /* calloc으로 정수 배열 할당 (자동으로 0 초기화) */
    /* Allocate integer array with calloc (automatically initialized to 0) */
    int32_t* arr = (int32_t*)calloc(count, sizeof(int32_t));
    
    if (arr == NULL) {
        printf("[ERROR] calloc failed!\n");
        return;
    }
    
    printf("calloc 할당 성공: %zu integers\n", count);
    
    /* calloc은 자동으로 0 초기화 */
    /* calloc automatically initializes to 0 */
    printf("calloc 직후 (0으로 초기화됨): ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    printf("\nmalloc vs calloc 비교:\n");
    printf("  malloc: 초기화 안함, memset 필요\n");
    printf("  calloc: 자동 0 초기화, 약간 느림\n");
    
    free(arr);
    arr = NULL;
    printf("\n메모리 해제 완료\n");
    
    printf("========================================================\n");
}

/**
 * @brief Demonstrate realloc() function / realloc() 함수 시연
 * 
 * realloc (Re-allocation):
 * - 기존 메모리 블록의 크기를 변경
 * - 기존 데이터는 유지됨 (새 크기가 더 작으면 잘림)
 * - 새로운 주소를 반환할 수 있음 (기존 주소와 다를 수 있음!)
 * - 실패 시 NULL 반환, 기존 메모리는 유지
 * 
 * 형식: void* realloc(void* ptr, size_t new_size);
 */
void demonstrateRealloc(void) {
    printf("\n============ realloc() Demo / realloc() 시연 ============\n");
    
    size_t initialSize = 3;
    size_t newSize = 6;
    
    /* 초기 배열 할당 */
    int32_t* arr = (int32_t*)malloc(initialSize * sizeof(int32_t));
    if (arr == NULL) {
        printf("[ERROR] Initial malloc failed!\n");
        return;
    }
    
    /* 초기 값 설정 */
    for (size_t i = 0; i < initialSize; i++) {
        arr[i] = (int32_t)((i + 1) * 100);
    }
    
    printf("초기 배열 (크기 %zu): ", initialSize);
    for (size_t i = 0; i < initialSize; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("초기 주소: %p\n", (void*)arr);
    
    /* realloc으로 크기 확장 */
    /* Expand size with realloc */
    int32_t* newArr = (int32_t*)realloc(arr, newSize * sizeof(int32_t));
    
    /* 중요: realloc 실패 처리 패턴 */
    /* Important: realloc failure handling pattern */
    if (newArr == NULL) {
        printf("[ERROR] realloc failed! 기존 메모리는 유지됨\n");
        free(arr);  /* 원본 메모리 해제 필요 */
        return;
    }
    
    arr = newArr;   /* 새 포인터 사용 */
    
    printf("\nrealloc 후 (크기 %zu): ", newSize);
    printf("(처음 %zu개는 유지됨)\n", initialSize);
    printf("새 주소: %p\n", (void*)arr);
    
    /* 확장된 부분 초기화 */
    for (size_t i = initialSize; i < newSize; i++) {
        arr[i] = 0;
    }
    
    printf("배열 내용: ");
    for (size_t i = 0; i < newSize; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    free(arr);
    arr = NULL;
    printf("\n메모리 해제 완료\n");
    
    printf("=========================================================\n");
}

/**
 * @brief Initialize dynamic array / 동적 배열 초기화
 * @param array Pointer to DynamicArray structure / DynamicArray 구조체 포인터
 * @param initialCapacity Initial capacity / 초기 용량
 * @return SUCCESS or error code / 성공 또는 에러 코드
 */
int dynamicArrayInit(DynamicArray_t* array, size_t initialCapacity) {
    if (array == NULL) {
        return ERR_NULL_POINTER;
    }
    
    if (initialCapacity == 0) {
        return ERR_INVALID_SIZE;
    }
    
    /* 메모리 할당 */
    array->data = (int32_t*)calloc(initialCapacity, sizeof(int32_t));
    if (array->data == NULL) {
        return ERR_ALLOC_FAILED;
    }
    
    array->size = 0;
    array->capacity = initialCapacity;
    
    printf("[INIT] DynamicArray: capacity=%zu\n", initialCapacity);
    return SUCCESS;
}

/**
 * @brief Add element to dynamic array / 동적 배열에 요소 추가
 * @param array Pointer to DynamicArray / DynamicArray 포인터
 * @param value Value to add / 추가할 값
 * @return SUCCESS or error code / 성공 또는 에러 코드
 * 
 * 자동 확장: 용량이 부족하면 2배로 확장
 * Auto-expansion: Doubles capacity when full
 */
int dynamicArrayPush(DynamicArray_t* array, int32_t value) {
    if (array == NULL || array->data == NULL) {
        return ERR_NULL_POINTER;
    }
    
    /* 용량 초과 시 확장 */
    /* Expand when capacity exceeded */
    if (array->size >= array->capacity) {
        size_t newCapacity = array->capacity * 2;
        
        int32_t* newData = (int32_t*)realloc(array->data, 
                                              newCapacity * sizeof(int32_t));
        if (newData == NULL) {
            return ERR_ALLOC_FAILED;
        }
        
        array->data = newData;
        array->capacity = newCapacity;
        printf("[EXPAND] Capacity: %zu -> %zu\n", array->capacity / 2, newCapacity);
    }
    
    /* 요소 추가 */
    array->data[array->size] = value;
    array->size++;
    
    return SUCCESS;
}

/**
 * @brief Free dynamic array / 동적 배열 해제
 * @param array Pointer to DynamicArray / DynamicArray 포인터
 */
void dynamicArrayFree(DynamicArray_t* array) {
    if (array == NULL) {
        return;
    }
    
    if (array->data != NULL) {
        free(array->data);
        array->data = NULL;
    }
    
    array->size = 0;
    array->capacity = 0;
    
    printf("[FREE] DynamicArray memory released\n");
}

/**
 * @brief Print dynamic array / 동적 배열 출력
 * @param array Pointer to DynamicArray / DynamicArray 포인터
 */
void dynamicArrayPrint(const DynamicArray_t* array) {
    if (array == NULL || array->data == NULL) {
        printf("[ERROR] Invalid array\n");
        return;
    }
    
    printf("DynamicArray [size=%zu, capacity=%zu]: ", 
           array->size, array->capacity);
    
    for (size_t i = 0; i < array->size; i++) {
        printf("%d ", array->data[i]);
    }
    printf("\n");
}

/**
 * @brief Demonstrate memory leak prevention patterns
 *        메모리 누수 방지 패턴 시연
 */
void demonstrateMemoryLeakPrevention(void) {
    printf("\n============ Memory Leak Prevention / 메모리 누수 방지 ============\n");
    
    printf("패턴 1: 할당 직후 NULL 검사\n");
    printf("  int* p = malloc(...);\n");
    printf("  if (p == NULL) { /* 에러 처리 */ }\n\n");
    
    printf("패턴 2: 해제 후 NULL 할당\n");
    printf("  free(p);\n");
    printf("  p = NULL;  /* Dangling pointer 방지 */\n\n");
    
    printf("패턴 3: 함수 종료 전 해제 (goto cleanup 패턴)\n");
    printf("  int func() {\n");
    printf("      int* p = malloc(...);\n");
    printf("      if (error) goto cleanup;\n");
    printf("      // ... 코드 ...\n");
    printf("  cleanup:\n");
    printf("      free(p);\n");
    printf("      return result;\n");
    printf("  }\n\n");
    
    printf("패턴 4: 구조체 초기화/해제 함수 쌍\n");
    printf("  init() / deinit() 또는 create() / destroy()\n");
    
    printf("=====================================================================\n");
}

/**
 * @brief Demonstrate double free problem / 이중 해제 문제 시연
 */
void demonstrateDoubleFree(void) {
    printf("\n============ Double Free Problem / 이중 해제 문제 ============\n");
    
    printf("위험한 코드 (하지 마세요!):\n");
    printf("  int* p = malloc(100);\n");
    printf("  free(p);    // 첫 번째 해제\n");
    printf("  free(p);    // 두 번째 해제 -> 정의되지 않은 동작!\n\n");
    
    printf("안전한 코드:\n");
    printf("  int* p = malloc(100);\n");
    printf("  free(p);\n");
    printf("  p = NULL;   // NULL로 설정\n");
    printf("  free(p);    // NULL free는 안전함 (아무 동작 안함)\n");
    
    /* 실제 시연 - 안전한 버전 */
    int32_t* p = (int32_t*)malloc(100);
    free(p);
    p = NULL;
    free(p);    /* NULL free는 안전 */
    printf("\nfree(NULL)은 안전하게 무시됨 ✓\n");
    
    printf("================================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 4: Dynamic Memory Allocation\n");
    printf("  Level 4: 동적 메모리 할당\n");
    printf("========================================\n\n");
    
    /* Test 1: malloc() 시연 */
    printf("[Test 1] malloc() Function\n");
    demonstrateMalloc();
    
    /* Test 2: calloc() 시연 */
    printf("\n[Test 2] calloc() Function\n");
    demonstrateCalloc();
    
    /* Test 3: realloc() 시연 */
    printf("\n[Test 3] realloc() Function\n");
    demonstrateRealloc();
    
    /* Test 4: 동적 배열 구조체 사용 */
    printf("\n[Test 4] DynamicArray Structure / 동적 배열 구조체\n");
    DynamicArray_t myArray;
    
    if (dynamicArrayInit(&myArray, 4) == SUCCESS) {
        /* 요소 추가 - 자동 확장 테스트 */
        for (int i = 1; i <= 10; i++) {
            dynamicArrayPush(&myArray, i * 10);
            dynamicArrayPrint(&myArray);
        }
        
        /* 메모리 해제 */
        dynamicArrayFree(&myArray);
    }
    
    /* Test 5: 메모리 누수 방지 패턴 */
    printf("\n[Test 5] Memory Leak Prevention Patterns\n");
    demonstrateMemoryLeakPrevention();
    
    /* Test 6: 이중 해제 문제 */
    printf("\n[Test 6] Double Free Problem\n");
    demonstrateDoubleFree();
    
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
 * [Test 1] malloc: 초기화되지 않은 메모리 -> memset으로 초기화
 * [Test 2] calloc: 자동으로 0 초기화됨
 * [Test 3] realloc: 기존 데이터 유지하면서 크기 변경
 * [Test 4] DynamicArray: 용량 초과 시 자동 2배 확장
 * 
 * ============================================================================
 */
