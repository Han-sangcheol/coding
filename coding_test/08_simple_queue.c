/**
 * ============================================================================
 * [코딩테스트 Level 8] 배열 기반 선형 큐 (FIFO)
 * ============================================================================
 * 
 * 문제 설명:
 * 큐(Queue)는 먼저 들어온 데이터가 먼저 나가는 FIFO(First In First Out) 자료구조입니다.
 * 이번 문제에서는 배열을 사용한 "선형 큐"를 구현하고, 그 한계를 이해합니다.
 * 이 한계를 해결한 것이 바로 다음 단계의 Circular Buffer입니다.
 * 
 * 요구사항:
 * 1. 배열 기반 큐를 구현하세요.
 * 2. enqueue() - 데이터 삽입
 * 3. dequeue() - 데이터 추출
 * 4. isEmpty(), isFull() - 상태 확인
 * 5. 선형 큐의 문제점(공간 낭비)을 이해하세요.
 * 
 * 학습 포인트:
 * - FIFO 자료구조의 개념
 * - front, rear 인덱스 관리
 * - 선형 큐의 한계 (dequeue 후 공간 재사용 불가)
 * - Circular Queue의 필요성 이해
 * 
 * ============================================================================
 * File: 08_simple_queue.c
 * 파일명: 08_simple_queue.c
 * Author / 작성자: [Developer]
 * Date / 작성일: 2026-01-14
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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
 * Constants and Error Codes / 상수 및 에러 코드
 * ============================================================================ */
#define QUEUE_CAPACITY      8

#define Q_SUCCESS           0
#define Q_ERROR_FULL        -1
#define Q_ERROR_EMPTY       -2
#define Q_ERROR_NULL        -3

/* ============================================================================
 * Queue Structure Definition / 큐 구조체 정의
 * ============================================================================ */

/**
 * Linear Queue Structure / 선형 큐 구조체
 * 
 * 메모리 레이아웃:
 * +----+----+----+----+----+----+----+----+
 * |    |    | D1 | D2 | D3 |    |    |    |
 * +----+----+----+----+----+----+----+----+
 *            ^              ^
 *          front          rear
 * 
 * - front: 다음에 꺼낼 위치 (dequeue)
 * - rear: 다음에 넣을 위치 (enqueue)
 * - front == rear: 큐가 비어있음
 */
typedef struct {
    int32_t     data[QUEUE_CAPACITY];   /* Data array / 데이터 배열 */
    int32_t     front;                  /* Front index (dequeue position) */
                                        /* 앞 인덱스 (꺼내는 위치) */
    int32_t     rear;                   /* Rear index (enqueue position) */
                                        /* 뒤 인덱스 (넣는 위치) */
    int32_t     count;                  /* Current element count / 현재 요소 개수 */
} LinearQueue_t;

/* ============================================================================
 * Queue Function Implementations / 큐 함수 구현
 * ============================================================================ */

/**
 * @brief Initialize queue / 큐 초기화
 * @param queue Pointer to queue / 큐 포인터
 */
void queue_init(LinearQueue_t* queue) {
    if (queue == NULL) return;
    
    memset(queue->data, 0, sizeof(queue->data));
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
    
    printf("[INIT] Queue initialized (capacity: %d)\n", QUEUE_CAPACITY);
}

/**
 * @brief Check if queue is empty / 큐가 비어있는지 확인
 * @param queue Pointer to queue / 큐 포인터
 * @return true if empty / 비어있으면 true
 */
bool queue_isEmpty(const LinearQueue_t* queue) {
    if (queue == NULL) return true;
    return (queue->count == 0);
}

/**
 * @brief Check if queue is full / 큐가 가득 찼는지 확인
 * @param queue Pointer to queue / 큐 포인터
 * @return true if full / 가득 찼으면 true
 * 
 * 선형 큐의 문제점:
 * rear가 배열 끝에 도달하면 앞에 빈 공간이 있어도 "가득 참"으로 판단
 * 
 * Linear queue problem:
 * When rear reaches end, queue is considered "full" even if front has empty space
 */
bool queue_isFull(const LinearQueue_t* queue) {
    if (queue == NULL) return false;
    /* 선형 큐: rear가 배열 끝에 도달하면 가득 참 */
    return (queue->rear >= QUEUE_CAPACITY);
}

/**
 * @brief Add element to queue / 큐에 요소 추가 (Enqueue)
 * @param queue Pointer to queue / 큐 포인터
 * @param value Value to add / 추가할 값
 * @return Q_SUCCESS or error code / 성공 또는 에러 코드
 */
int queue_enqueue(LinearQueue_t* queue, int32_t value) {
    if (queue == NULL) {
        return Q_ERROR_NULL;
    }
    
    if (queue_isFull(queue)) {
        printf("[ERROR] Queue is full! Cannot enqueue %d\n", value);
        return Q_ERROR_FULL;
    }
    
    /* 데이터 삽입 후 rear 증가 */
    queue->data[queue->rear] = value;
    queue->rear++;
    queue->count++;
    
    printf("[ENQUEUE] Added %d at index %d (count: %d)\n", 
           value, queue->rear - 1, queue->count);
    
    return Q_SUCCESS;
}

/**
 * @brief Remove element from queue / 큐에서 요소 제거 (Dequeue)
 * @param queue Pointer to queue / 큐 포인터
 * @param value Pointer to store removed value / 제거된 값을 저장할 포인터
 * @return Q_SUCCESS or error code / 성공 또는 에러 코드
 */
int queue_dequeue(LinearQueue_t* queue, int32_t* value) {
    if (queue == NULL || value == NULL) {
        return Q_ERROR_NULL;
    }
    
    if (queue_isEmpty(queue)) {
        printf("[ERROR] Queue is empty! Cannot dequeue\n");
        return Q_ERROR_EMPTY;
    }
    
    /* 데이터 추출 후 front 증가 */
    *value = queue->data[queue->front];
    queue->front++;
    queue->count--;
    
    printf("[DEQUEUE] Removed %d from index %d (count: %d)\n", 
           *value, queue->front - 1, queue->count);
    
    return Q_SUCCESS;
}

/**
 * @brief Peek front element without removing / 제거하지 않고 앞 요소 확인
 * @param queue Pointer to queue / 큐 포인터
 * @param value Pointer to store peeked value / 확인된 값을 저장할 포인터
 * @return Q_SUCCESS or error code / 성공 또는 에러 코드
 */
int queue_peek(const LinearQueue_t* queue, int32_t* value) {
    if (queue == NULL || value == NULL) {
        return Q_ERROR_NULL;
    }
    
    if (queue_isEmpty(queue)) {
        return Q_ERROR_EMPTY;
    }
    
    *value = queue->data[queue->front];
    return Q_SUCCESS;
}

/**
 * @brief Get current queue count / 현재 큐의 요소 개수 반환
 * @param queue Pointer to queue / 큐 포인터
 * @return Element count / 요소 개수
 */
int32_t queue_getCount(const LinearQueue_t* queue) {
    if (queue == NULL) return 0;
    return queue->count;
}

/**
 * @brief Print queue status / 큐 상태 출력
 * @param queue Pointer to queue / 큐 포인터
 */
void queue_printStatus(const LinearQueue_t* queue) {
    if (queue == NULL) {
        printf("[ERROR] NULL queue\n");
        return;
    }
    
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│      Queue Status / 큐 상태         │\n");
    printf("├─────────────────────────────────────┤\n");
    printf("│ Front Index:  %d                    │\n", queue->front);
    printf("│ Rear Index:   %d                    │\n", queue->rear);
    printf("│ Count:        %d                    │\n", queue->count);
    printf("│ Capacity:     %d                    │\n", QUEUE_CAPACITY);
    printf("│ Is Empty:     %-5s                 │\n", queue_isEmpty(queue) ? "Yes" : "No");
    printf("│ Is Full:      %-5s                 │\n", queue_isFull(queue) ? "Yes" : "No");
    printf("└─────────────────────────────────────┘\n");
}

/**
 * @brief Visualize queue / 큐 시각화
 * @param queue Pointer to queue / 큐 포인터
 */
void queue_visualize(const LinearQueue_t* queue) {
    if (queue == NULL) return;
    
    printf("\n큐 시각화:\n");
    printf("Index: ");
    for (int i = 0; i < QUEUE_CAPACITY; i++) {
        printf("%4d ", i);
    }
    printf("\n");
    
    printf("Data:  ");
    for (int i = 0; i < QUEUE_CAPACITY; i++) {
        if (i >= queue->front && i < queue->rear) {
            printf("%4d ", queue->data[i]);
        } else if (i < queue->front) {
            printf("  X  ");     /* 이미 dequeue됨 (낭비된 공간) */
        } else {
            printf("  -  ");     /* 비어있음 */
        }
    }
    printf("\n");
    
    printf("       ");
    for (int i = 0; i < QUEUE_CAPACITY; i++) {
        if (i == queue->front && i == queue->rear) {
            printf(" F,R ");
        } else if (i == queue->front) {
            printf("  F  ");
        } else if (i == queue->rear) {
            printf("  R  ");
        } else {
            printf("     ");
        }
    }
    printf("\n");
    
    printf("(F=Front, R=Rear, X=Wasted space / 낭비된 공간)\n");
}

/* ============================================================================
 * Demonstration Functions / 시연 함수
 * ============================================================================ */

/**
 * @brief Demonstrate linear queue operations / 선형 큐 연산 시연
 */
void demonstrateLinearQueue(void) {
    printf("============ Linear Queue Operations / 선형 큐 연산 ============\n\n");
    
    LinearQueue_t queue;
    queue_init(&queue);
    
    /* Enqueue 5개 */
    printf("\n[Step 1] Enqueue 5 items / 5개 삽입\n");
    for (int i = 1; i <= 5; i++) {
        queue_enqueue(&queue, i * 10);
    }
    queue_visualize(&queue);
    queue_printStatus(&queue);
    
    /* Dequeue 3개 */
    printf("\n[Step 2] Dequeue 3 items / 3개 추출\n");
    int32_t value;
    for (int i = 0; i < 3; i++) {
        queue_dequeue(&queue, &value);
    }
    queue_visualize(&queue);
    queue_printStatus(&queue);
    
    printf("=============================================================\n");
}

/**
 * @brief Demonstrate linear queue limitation / 선형 큐의 한계 시연
 * 
 * 문제점: Dequeue 후 앞쪽 공간이 낭비됨
 * Problem: Space is wasted after dequeue
 */
void demonstrateLinearQueueLimitation(void) {
    printf("\n============ Linear Queue Limitation / 선형 큐의 한계 ============\n\n");
    
    LinearQueue_t queue;
    queue_init(&queue);
    
    /* 1. 먼저 꽉 채우기 */
    printf("[Step 1] Fill the queue / 큐 가득 채우기\n");
    for (int i = 1; i <= QUEUE_CAPACITY; i++) {
        queue_enqueue(&queue, i * 10);
    }
    queue_visualize(&queue);
    
    /* 2. 일부 제거 */
    printf("\n[Step 2] Dequeue half / 절반 추출\n");
    int32_t value;
    for (int i = 0; i < QUEUE_CAPACITY / 2; i++) {
        queue_dequeue(&queue, &value);
    }
    queue_visualize(&queue);
    
    /* 3. 더 추가 시도 - 실패! */
    printf("\n[Step 3] Try to enqueue more / 추가 삽입 시도\n");
    printf("앞에 %d개의 빈 공간이 있지만...\n", queue.front);
    int result = queue_enqueue(&queue, 999);
    
    if (result == Q_ERROR_FULL) {
        printf("\n┌────────────────────────────────────────────────────┐\n");
        printf("│  문제 발견! / Problem Found!                       │\n");
        printf("├────────────────────────────────────────────────────┤\n");
        printf("│  - 앞에 빈 공간이 있음: %d칸                        │\n", queue.front);
        printf("│  - 실제 데이터: %d개                                │\n", queue.count);
        printf("│  - 하지만 큐는 \"가득 참\"으로 판단                   │\n");
        printf("│  - 공간 낭비 발생!                                 │\n");
        printf("├────────────────────────────────────────────────────┤\n");
        printf("│  해결책: Circular Queue (환형 큐) 사용             │\n");
        printf("│  -> rear가 끝에 도달하면 앞으로 돌아감             │\n");
        printf("└────────────────────────────────────────────────────┘\n");
    }
    
    queue_visualize(&queue);
    
    printf("================================================================\n");
}

/**
 * @brief Compare Linear vs Circular Queue / 선형 vs 환형 큐 비교
 */
void compareLinearVsCircular(void) {
    printf("\n============ Linear vs Circular Queue / 선형 vs 환형 큐 ============\n\n");
    
    printf("┌────────────────────┬────────────────────────────────────┐\n");
    printf("│      항목          │  선형 큐      │  환형 큐 (Circular) │\n");
    printf("├────────────────────┼───────────────┼────────────────────┤\n");
    printf("│ 공간 활용          │  낭비 발생    │  완전 활용         │\n");
    printf("│ 인덱스 관리        │  단순 증가    │  %% or & 연산       │\n");
    printf("│ Full 조건          │  rear >= size │  count == capacity │\n");
    printf("│ 메모리 효율        │  낮음         │  높음              │\n");
    printf("│ 구현 복잡도        │  단순         │  약간 복잡         │\n");
    printf("│ 실제 사용          │  거의 안함    │  대부분 사용       │\n");
    printf("└────────────────────┴───────────────┴────────────────────┘\n");
    
    printf("\nCircular Queue 핵심 아이디어:\n");
    printf("  1. rear = (rear + 1) %% capacity  (모듈로 연산)\n");
    printf("  2. rear = (rear + 1) & mask      (비트 마스킹, 더 빠름)\n");
    printf("  -> 배열 끝에 도달하면 자동으로 처음으로 돌아감!\n");
    
    printf("===================================================================\n");
}

/* ============================================================================
 * Main Function (Test) / 메인 함수 (테스트)
 * ============================================================================ */
int main(void) {
    /* Enable UTF-8 console output / UTF-8 콘솔 출력 활성화 */
    ENABLE_UTF8_CONSOLE();
    
    printf("========================================\n");
    printf("  Level 8: Simple Linear Queue (FIFO)\n");
    printf("  Level 8: 배열 기반 선형 큐\n");
    printf("========================================\n\n");
    
    /* Test 1: 큐 기본 연산 */
    printf("[Test 1] Basic Queue Operations / 기본 큐 연산\n");
    demonstrateLinearQueue();
    
    /* Test 2: 선형 큐의 한계 */
    printf("\n[Test 2] Linear Queue Limitation / 선형 큐의 한계\n");
    demonstrateLinearQueueLimitation();
    
    /* Test 3: 선형 vs 환형 큐 비교 */
    printf("\n[Test 3] Linear vs Circular Queue / 선형 vs 환형 큐 비교\n");
    compareLinearVsCircular();
    
    printf("\n========================================\n");
    printf("  다음 단계: Circular Buffer 구현!\n");
    printf("  Next: Implement Circular Buffer!\n");
    printf("========================================\n");
    
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
 * [Test 1] 기본 enqueue/dequeue 동작 확인
 * [Test 2] 선형 큐의 공간 낭비 문제 확인
 *          - front 앞에 빈 공간이 있어도 enqueue 불가
 * [Test 3] 선형 큐 vs 환형 큐 비교 표
 * 
 * ============================================================================
 */
