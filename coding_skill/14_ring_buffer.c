/*
 * ============================================================================
 * 파일명: 14_ring_buffer.c
 * 기능: C언어 Ring Buffer(순환 버퍼) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Ring Buffer 패턴이란?
 * ============================================================================
 * 
 * 고정 크기 배열을 순환하며 사용하는 FIFO 버퍼
 * 
 * 핵심:
 *   - 고정 크기 (메모리 예측 가능)
 *   - FIFO (First In First Out)
 *   - Head/Tail 포인터
 *   - 순환 구조 (Circular)
 * 
 * ============================================================================
 * 2️⃣ 왜 Ring Buffer를 쓰나?
 * ============================================================================
 * 
 * ✅ Ring Buffer의 장점:
 *   - 고정 메모리 사용
 *   - O(1) 추가/제거
 *   - 메모리 단편화 없음
 *   - 실시간 시스템에 적합
 * 
 * ============================================================================
 * 3️⃣ 임베디드에서 Ring Buffer 활용
 * ============================================================================
 * 
 * 실무 활용:
 *   ✅ UART 수신 버퍼
 *   ✅ DMA 버퍼
 *   ✅ 오디오 스트리밍
 *   ✅ 센서 데이터 버퍼링
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RING_BUFFER_SIZE 8

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    int head;
    int tail;
    int count;
} RingBuffer;

/* Ring Buffer 초기화 */
void RingBuffer_Init(RingBuffer* rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    memset(rb->buffer, 0, RING_BUFFER_SIZE);
    printf("[RingBuffer] 초기화: 크기=%d\n", RING_BUFFER_SIZE);
}

/* 데이터 추가 (Push) */
int RingBuffer_Push(RingBuffer* rb, uint8_t data) {
    if (rb->count >= RING_BUFFER_SIZE) {
        printf("[RingBuffer] Push 실패: 버퍼 가득 참\n");
        return -1;
    }
    
    rb->buffer[rb->tail] = data;
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
    rb->count++;
    
    printf("[RingBuffer] Push: 0x%02X (count=%d)\n", data, rb->count);
    return 0;
}

/* 데이터 꺼내기 (Pop) */
int RingBuffer_Pop(RingBuffer* rb, uint8_t* data) {
    if (rb->count == 0) {
        printf("[RingBuffer] Pop 실패: 버퍼 비어있음\n");
        return -1;
    }
    
    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
    rb->count--;
    
    printf("[RingBuffer] Pop: 0x%02X (count=%d)\n", *data, rb->count);
    return 0;
}

/* 버퍼 상태 출력 */
void RingBuffer_PrintStatus(RingBuffer* rb) {
    printf("[RingBuffer] 상태: head=%d, tail=%d, count=%d/%d\n",
           rb->head, rb->tail, rb->count, RING_BUFFER_SIZE);
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Ring Buffer 패턴 예제\n");
    printf("========================================\n\n");
    
    RingBuffer rb;
    RingBuffer_Init(&rb);
    
    // 데이터 추가
    printf("\n=== 데이터 추가 ===\n");
    for (int i = 1; i <= 5; i++) {
        RingBuffer_Push(&rb, i * 10);
    }
    RingBuffer_PrintStatus(&rb);
    
    // 데이터 꺼내기
    printf("\n=== 데이터 꺼내기 ===\n");
    uint8_t data;
    for (int i = 0; i < 3; i++) {
        if (RingBuffer_Pop(&rb, &data) == 0) {
            printf("  → 꺼낸 데이터: %d\n", data);
        }
    }
    RingBuffer_PrintStatus(&rb);
    
    // 순환 동작 테스트
    printf("\n=== 순환 동작 테스트 ===\n");
    for (int i = 6; i <= 10; i++) {
        RingBuffer_Push(&rb, i * 10);
    }
    RingBuffer_PrintStatus(&rb);
    
    printf("\n========================================\n");
    printf("Ring Buffer 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Ring Buffer의 장점
 * ============================================================================
 * 
 * 1. 고정 메모리 (예측 가능)
 * 2. O(1) 추가/제거
 * 3. 메모리 단편화 없음
 * 4. 실시간 시스템 적합
 * 
 * ============================================================================
 * 면접 대응
 * ============================================================================
 * 
 * Q: Ring Buffer를 왜 사용하나요?
 * A: "UART 수신처럼 연속적으로 데이터가 들어오는 경우, Ring Buffer로
 *     고정 메모리를 순환하며 사용하면 메모리 단편화 없이 효율적으로
 *     버퍼링할 수 있습니다."
 * 
 * ============================================================================
 */
