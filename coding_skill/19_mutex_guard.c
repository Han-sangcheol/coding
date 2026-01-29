/* Mutex Guard Pattern
 * 뮤텍스 자동 잠금/해제 (Scope-based locking)
 */

#include <stdio.h>

typedef struct {
    int locked;
    int owner_id;
} Mutex;

typedef struct {
    Mutex* mutex;
    int task_id;
} MutexGuard;

void Mutex_Init(Mutex* mutex) {
    mutex->locked = 0;
    mutex->owner_id = -1;
}

MutexGuard* MutexGuard_Create(Mutex* mutex, int task_id) {
    MutexGuard* guard = malloc(sizeof(MutexGuard));
    guard->mutex = mutex;
    guard->task_id = task_id;
    
    mutex->locked = 1;
    mutex->owner_id = task_id;
    printf("[MutexGuard] Lock: Task %d\n", task_id);
    return guard;
}

void MutexGuard_Destroy(MutexGuard** guard) {
    if (*guard) {
        (*guard)->mutex->locked = 0;
        printf("[MutexGuard] Unlock: Task %d\n", (*guard)->task_id);
        free(*guard);
        *guard = NULL;
    }
}

void CriticalSection(Mutex* mutex, int task_id) {
    MutexGuard* guard = MutexGuard_Create(mutex, task_id);
    
    printf("  → 크리티컬 섹션 실행 (Task %d)\n", task_id);
    
    MutexGuard_Destroy(&guard);  // 자동 unlock
}

int main(void) {
    printf("=== Mutex Guard Pattern ===\n\n");
    
    Mutex mutex;
    Mutex_Init(&mutex);
    
    CriticalSection(&mutex, 1);
    CriticalSection(&mutex, 2);
    
    printf("\nMutex Guard: 자동 잠금/해제 패턴\n");
    return 0;
}
