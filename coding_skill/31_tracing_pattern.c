/* Tracing Pattern
 * 함수 호출 추적
 */

#include <stdio.h>
#include <time.h>

static int trace_depth = 0;

#define TRACE_ENTER() \
    do { \
        for (int i = 0; i < trace_depth; i++) printf("  "); \
        printf("→ %s() 진입\n", __func__); \
        trace_depth++; \
    } while(0)

#define TRACE_EXIT() \
    do { \
        trace_depth--; \
        for (int i = 0; i < trace_depth; i++) printf("  "); \
        printf("← %s() 종료\n", __func__); \
    } while(0)

void FunctionC(void) {
    TRACE_ENTER();
    printf("    FunctionC 실행 중\n");
    TRACE_EXIT();
}

void FunctionB(void) {
    TRACE_ENTER();
    printf("    FunctionB 실행 중\n");
    FunctionC();
    TRACE_EXIT();
}

void FunctionA(void) {
    TRACE_ENTER();
    printf("    FunctionA 실행 중\n");
    FunctionB();
    TRACE_EXIT();
}

int main(void) {
    printf("=== Tracing Pattern ===\n\n");
    
    printf("함수 호출 추적:\n");
    FunctionA();
    
    printf("\nTracing: 함수 호출 추적 패턴\n");
    printf("용도: 디버깅, 성능 분석, 호출 흐름 파악\n");
    return 0;
}
