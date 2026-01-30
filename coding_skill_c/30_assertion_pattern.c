/* Assertion Pattern
 * 런타임 검증
 */

#include <stdio.h>
#include <stdlib.h>

// Assert 매크로
#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("[ASSERT FAILED] %s:%d - %s\n", __FILE__, __LINE__, message); \
            printf("  조건: %s\n", #condition); \
            abort(); \
        } \
    } while(0)

// 에러 코드
typedef enum {
    ERR_OK = 0,
    ERR_NULL_PTR = -1,
    ERR_INVALID_PARAM = -2
} ErrorCode;

// 포인터 검증
int ProcessData(int* data, int size) {
    ASSERT(data != NULL, "데이터 포인터가 NULL입니다");
    ASSERT(size > 0, "크기가 0 이하입니다");
    ASSERT(size <= 100, "크기가 너무 큽니다");
    
    printf("[ProcessData] 검증 통과 - 데이터 처리 중\n");
    return ERR_OK;
}

int main(void) {
    printf("=== Assertion Pattern ===\n\n");
    
    int data[10] = {0};
    
    printf("=== 정상 케이스 ===\n");
    ProcessData(data, 10);
    
    printf("\n=== 비정상 케이스 (주석 해제 시 종료) ===\n");
    // ProcessData(NULL, 10);  // Assert 실패
    // ProcessData(data, 0);    // Assert 실패
    // ProcessData(data, 1000); // Assert 실패
    
    printf("\nAssertion: 런타임 검증 패턴\n");
    printf("디버그 빌드에서 사용, 릴리즈에서는 제거 가능\n");
    return 0;
}
