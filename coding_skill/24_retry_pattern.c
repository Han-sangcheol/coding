/* Retry Pattern
 * 실패 시 재시도 로직
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int max_retries;
    int retry_count;
    int delay_ms;
} RetryConfig;

int UnreliableOperation(int* attempt) {
    (*attempt)++;
    printf("[Operation] 시도 %d...\n", *attempt);
    
    // 3번째 시도에서 성공하도록 시뮬레이션
    if (*attempt >= 3) {
        printf("[Operation] 성공!\n");
        return 0;
    }
    
    printf("[Operation] 실패\n");
    return -1;
}

int Retry_Execute(RetryConfig* config, int (*operation)(int*)) {
    int attempt = 0;
    
    for (int i = 0; i <= config->max_retries; i++) {
        if (operation(&attempt) == 0) {
            printf("[Retry] 성공 (시도 횟수: %d)\n", attempt);
            return 0;
        }
        
        if (i < config->max_retries) {
            printf("[Retry] 재시도 중... (%d/%d)\n", i + 1, config->max_retries);
        }
    }
    
    printf("[Retry] 최대 재시도 횟수 초과\n");
    return -1;
}

int main(void) {
    printf("=== Retry Pattern ===\n\n");
    
    RetryConfig config = {
        .max_retries = 5,
        .delay_ms = 100
    };
    
    Retry_Execute(&config, UnreliableOperation);
    
    printf("\nRetry: 실패 시 재시도 패턴\n");
    return 0;
}
