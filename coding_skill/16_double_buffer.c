/*
 * ============================================================================
 * 파일명: 16_double_buffer.c
 * 기능: C언어 Double Buffer(이중 버퍼) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Double Buffer 패턴이란?
 * ============================================================================
 * 
 * 두 개의 버퍼를 교대로 사용하여 읽기/쓰기 충돌 방지
 * 
 * 핵심:
 *   - Front Buffer: 읽기용
 *   - Back Buffer: 쓰기용
 *   - Swap: 버퍼 교체
 * 
 * ============================================================================
 * 2️⃣ 임베디드에서 Double Buffer 활용
 * ============================================================================
 * 
 * - DMA 전송
 * - 그래픽 렌더링
 * - 오디오 버퍼
 * - ADC 데이터 수집
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUFFER_SIZE 10

typedef struct {
    uint8_t buffer_a[BUFFER_SIZE];
    uint8_t buffer_b[BUFFER_SIZE];
    uint8_t* front_buffer;  // 읽기용
    uint8_t* back_buffer;   // 쓰기용
} DoubleBuffer;

/* Double Buffer 초기화 */
void DoubleBuffer_Init(DoubleBuffer* db) {
    memset(db->buffer_a, 0, BUFFER_SIZE);
    memset(db->buffer_b, 0, BUFFER_SIZE);
    db->front_buffer = db->buffer_a;
    db->back_buffer = db->buffer_b;
    
    printf("[DoubleBuffer] 초기화\n");
    printf("  Front: %p\n", (void*)db->front_buffer);
    printf("  Back:  %p\n", (void*)db->back_buffer);
}

/* 백 버퍼에 데이터 쓰기 */
void DoubleBuffer_Write(DoubleBuffer* db, const uint8_t* data, size_t len) {
    if (len > BUFFER_SIZE) len = BUFFER_SIZE;
    memcpy(db->back_buffer, data, len);
    printf("[DoubleBuffer] Write: %zu bytes → Back buffer\n", len);
}

/* 프론트 버퍼에서 데이터 읽기 */
void DoubleBuffer_Read(DoubleBuffer* db, uint8_t* data, size_t len) {
    if (len > BUFFER_SIZE) len = BUFFER_SIZE;
    memcpy(data, db->front_buffer, len);
    printf("[DoubleBuffer] Read: %zu bytes ← Front buffer\n", len);
}

/* 버퍼 스왑 */
void DoubleBuffer_Swap(DoubleBuffer* db) {
    uint8_t* temp = db->front_buffer;
    db->front_buffer = db->back_buffer;
    db->back_buffer = temp;
    
    printf("[DoubleBuffer] Swap!\n");
    printf("  Front: %p\n", (void*)db->front_buffer);
    printf("  Back:  %p\n", (void*)db->back_buffer);
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Double Buffer 패턴 예제\n");
    printf("========================================\n\n");
    
    DoubleBuffer db;
    DoubleBuffer_Init(&db);
    
    uint8_t write_data[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint8_t read_data[BUFFER_SIZE];
    
    // 1. 백 버퍼에 쓰기
    printf("\n=== Step 1: 백 버퍼에 쓰기 ===\n");
    DoubleBuffer_Write(&db, write_data, BUFFER_SIZE);
    
    // 2. 버퍼 스왑
    printf("\n=== Step 2: 버퍼 스왑 ===\n");
    DoubleBuffer_Swap(&db);
    
    // 3. 프론트 버퍼에서 읽기
    printf("\n=== Step 3: 프론트 버퍼에서 읽기 ===\n");
    DoubleBuffer_Read(&db, read_data, BUFFER_SIZE);
    
    printf("읽은 데이터: ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", read_data[i]);
    }
    printf("\n");
    
    printf("\n========================================\n");
    printf("Double Buffer 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Double Buffer의 장점
 * ============================================================================
 * 
 * 1. 읽기/쓰기 충돌 방지
 * 2. 데이터 일관성 보장
 * 3. 동시 접근 가능
 * 4. 티어링(Tearing) 방지
 * 
 * ============================================================================
 * 면접 대응
 * ============================================================================
 * 
 * Q: Double Buffer를 왜 사용하나요?
 * A: "DMA가 버퍼에 데이터를 쓰는 동안 CPU가 같은 버퍼를 읽으면
 *     데이터가 깨질 수 있습니다. Double Buffer로 쓰기와 읽기를
 *     분리하면 안전하게 동시 처리할 수 있습니다."
 * 
 * ============================================================================
 */
