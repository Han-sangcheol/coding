/* Semaphore Pattern (RTOS 동기화)
 * 공유 리소스 접근 제어
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_COUNT 3

typedef struct {
    int count;
    int max_count;
} Semaphore;

void Sem_Init(Semaphore* sem, int max) {
    sem->count = max;
    sem->max_count = max;
    printf("[Semaphore] 초기화: max=%d\n", max);
}

int Sem_Wait(Semaphore* sem) {
    if (sem->count > 0) {
        sem->count--;
        printf("[Semaphore] Wait: count=%d\n", sem->count);
        return 0;
    }
    printf("[Semaphore] Wait 실패: 리소스 없음\n");
    return -1;
}

void Sem_Signal(Semaphore* sem) {
    if (sem->count < sem->max_count) {
        sem->count++;
        printf("[Semaphore] Signal: count=%d\n", sem->count);
    }
}

int main(void) {
    printf("=== Semaphore Pattern ===\n\n");
    
    Semaphore sem;
    Sem_Init(&sem, MAX_COUNT);
    
    Sem_Wait(&sem);
    Sem_Wait(&sem);
    Sem_Wait(&sem);
    Sem_Wait(&sem);  // 실패
    
    Sem_Signal(&sem);
    Sem_Wait(&sem);  // 성공
    
    printf("\nSemaphore: RTOS 동기화 패턴\n");
    return 0;
}
