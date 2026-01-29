/*
 * ============================================================================
 * 파일명: 10_event_queue.c
 * 기능: C언어 Event Queue(이벤트 큐) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Event Queue 패턴이란?
 * ============================================================================
 * 
 * 이벤트를 큐에 저장하고 순차적으로 처리하는 패턴
 * 
 * 핵심:
 *   - 이벤트 큐잉
 *   - 비동기 처리
 *   - 우선순위 처리
 * 
 * ============================================================================
 * 2️⃣ 임베디드에서 Event Queue 활용
 * ============================================================================
 * 
 * - ISR에서 이벤트 발생 → 메인 루프에서 처리
 * - RTOS 메시지 큐
 * - UI 이벤트 처리
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_EVENTS 20

/* 이벤트 타입 */
typedef enum {
    EVENT_BUTTON_PRESSED,
    EVENT_BUTTON_RELEASED,
    EVENT_TIMER_EXPIRED,
    EVENT_DATA_RECEIVED,
    EVENT_ERROR
} EventType;

/* 이벤트 구조체 */
typedef struct {
    EventType type;
    int data;
    char message[64];
} Event;

/* 이벤트 큐 */
typedef struct {
    Event events[MAX_EVENTS];
    int head;
    int tail;
    int count;
} EventQueue;

/* 이벤트 큐 초기화 */
void EventQueue_Init(EventQueue* queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    printf("[EventQueue] 이벤트 큐 초기화\n");
}

/* 이벤트 추가 (Enqueue) */
int EventQueue_Push(EventQueue* queue, const Event* event) {
    if (queue->count >= MAX_EVENTS) {
        printf("[EventQueue] 큐가 가득 참\n");
        return -1;
    }
    
    queue->events[queue->tail] = *event;
    queue->tail = (queue->tail + 1) % MAX_EVENTS;
    queue->count++;
    
    printf("[EventQueue] 이벤트 추가: 타입=%d, 데이터=%d (큐 크기: %d)\n", 
           event->type, event->data, queue->count);
    return 0;
}

/* 이벤트 꺼내기 (Dequeue) */
int EventQueue_Pop(EventQueue* queue, Event* event) {
    if (queue->count == 0) {
        return -1;  // 큐가 비어있음
    }
    
    *event = queue->events[queue->head];
    queue->head = (queue->head + 1) % MAX_EVENTS;
    queue->count--;
    
    return 0;
}

/* 이벤트 처리 핸들러 */
void EventHandler_Process(const Event* event) {
    switch (event->type) {
        case EVENT_BUTTON_PRESSED:
            printf("  → [Handler] 버튼 눌림 처리 (데이터: %d)\n", event->data);
            break;
        case EVENT_BUTTON_RELEASED:
            printf("  → [Handler] 버튼 릴리즈 처리\n");
            break;
        case EVENT_TIMER_EXPIRED:
            printf("  → [Handler] 타이머 만료 처리\n");
            break;
        case EVENT_DATA_RECEIVED:
            printf("  → [Handler] 데이터 수신: %s\n", event->message);
            break;
        case EVENT_ERROR:
            printf("  → [Handler] 오류 처리: %s\n", event->message);
            break;
    }
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Event Queue 패턴 예제\n");
    printf("========================================\n\n");
    
    EventQueue queue;
    EventQueue_Init(&queue);
    
    // 이벤트 생성 및 추가
    printf("\n=== 이벤트 추가 ===\n");
    
    Event e1 = { EVENT_BUTTON_PRESSED, 5, "" };
    EventQueue_Push(&queue, &e1);
    
    Event e2 = { EVENT_TIMER_EXPIRED, 0, "" };
    EventQueue_Push(&queue, &e2);
    
    Event e3 = { EVENT_DATA_RECEIVED, 0, "Hello World" };
    EventQueue_Push(&queue, &e3);
    
    Event e4 = { EVENT_BUTTON_RELEASED, 0, "" };
    EventQueue_Push(&queue, &e4);
    
    // 이벤트 처리
    printf("\n=== 이벤트 처리 ===\n");
    Event event;
    while (EventQueue_Pop(&queue, &event) == 0) {
        printf("[EventQueue] 이벤트 처리 시작 (타입: %d)\n", event.type);
        EventHandler_Process(&event);
    }
    
    printf("\n모든 이벤트 처리 완료\n");
    
    printf("\n========================================\n");
    printf("Event Queue 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Event Queue 패턴의 장점
 * ============================================================================
 * 
 * 1. 비동기 이벤트 처리
 * 2. ISR과 메인 루프 분리
 * 3. 이벤트 순서 보장
 * 4. 버퍼링 효과
 * 
 * ============================================================================
 */
