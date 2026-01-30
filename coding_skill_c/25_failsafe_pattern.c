/* Failsafe Pattern
 * 오류 발생 시 안전 모드 전환
 */

#include <stdio.h>

typedef enum {
    MODE_NORMAL,
    MODE_DEGRADED,
    MODE_SAFE
} SystemMode;

typedef struct {
    SystemMode mode;
    int error_count;
    int error_threshold;
} FailsafeSystem;

void Failsafe_Init(FailsafeSystem* sys, int threshold) {
    sys->mode = MODE_NORMAL;
    sys->error_count = 0;
    sys->error_threshold = threshold;
    printf("[Failsafe] 초기화: 임계값=%d\n", threshold);
}

void Failsafe_ReportError(FailsafeSystem* sys) {
    sys->error_count++;
    printf("[Failsafe] 오류 발생 (%d/%d)\n", 
           sys->error_count, sys->error_threshold);
    
    if (sys->error_count >= sys->error_threshold) {
        if (sys->mode == MODE_NORMAL) {
            sys->mode = MODE_DEGRADED;
            printf("[Failsafe] → 성능 저하 모드 전환\n");
        } else if (sys->mode == MODE_DEGRADED) {
            sys->mode = MODE_SAFE;
            printf("[Failsafe] → 안전 모드 전환 (비상 정지)\n");
        }
        sys->error_count = 0;
    }
}

void Failsafe_Operate(FailsafeSystem* sys) {
    switch (sys->mode) {
        case MODE_NORMAL:
            printf("[Failsafe] 정상 동작\n");
            break;
        case MODE_DEGRADED:
            printf("[Failsafe] 성능 저하 모드 (기본 기능만)\n");
            break;
        case MODE_SAFE:
            printf("[Failsafe] 안전 모드 (정지 상태)\n");
            break;
    }
}

int main(void) {
    printf("=== Failsafe Pattern ===\n\n");
    
    FailsafeSystem sys;
    Failsafe_Init(&sys, 3);
    
    Failsafe_Operate(&sys);
    
    // 오류 시뮬레이션
    for (int i = 0; i < 6; i++) {
        Failsafe_ReportError(&sys);
        Failsafe_Operate(&sys);
    }
    
    printf("\nFailsafe: 안전 모드 전환 패턴\n");
    return 0;
}
