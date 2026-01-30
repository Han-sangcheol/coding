/* Watchdog Pattern
 * 시스템 무한 루프 감지 및 리셋
 */

#include <stdio.h>
#include <time.h>

typedef struct {
    time_t last_kick;
    int timeout_sec;
    int enabled;
} Watchdog;

void Watchdog_Init(Watchdog* wd, int timeout) {
    wd->last_kick = time(NULL);
    wd->timeout_sec = timeout;
    wd->enabled = 1;
    printf("[Watchdog] 초기화: 타임아웃 %d초\n", timeout);
}

void Watchdog_Kick(Watchdog* wd) {
    wd->last_kick = time(NULL);
    printf("[Watchdog] Kick! (타이머 리셋)\n");
}

void Watchdog_Check(Watchdog* wd) {
    if (!wd->enabled) return;
    
    time_t now = time(NULL);
    int elapsed = (int)(now - wd->last_kick);
    
    if (elapsed >= wd->timeout_sec) {
        printf("[Watchdog] ⚠️  타임아웃! 시스템 리셋 필요\n");
        // 실제로는 시스템 리셋 수행
    } else {
        printf("[Watchdog] 정상 (경과: %d/%d초)\n", elapsed, wd->timeout_sec);
    }
}

int main(void) {
    printf("=== Watchdog Pattern ===\n\n");
    
    Watchdog wd;
    Watchdog_Init(&wd, 3);
    
    // 정상 동작
    Watchdog_Check(&wd);
    Watchdog_Kick(&wd);
    Watchdog_Check(&wd);
    
    printf("\nWatchdog: 무한 루프 감지 패턴\n");
    return 0;
}
