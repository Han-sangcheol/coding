/* Zero-Copy Pattern
 * 메모리 복사 없이 데이터 전달 (포인터 공유)
 */

#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 100

typedef struct {
    uint8_t* data;
    size_t size;
} ZeroCopyBuffer;

void Producer_FillBuffer(ZeroCopyBuffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        buf->data[i] = (uint8_t)(i % 256);
    }
    printf("[Producer] 버퍼 채움: %zu bytes\n", buf->size);
}

void Consumer_ProcessBuffer(const ZeroCopyBuffer* buf) {
    printf("[Consumer] 버퍼 처리: %zu bytes (복사 없음!)\n", buf->size);
    printf("  첫 5개: ");
    for (int i = 0; i < 5; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    printf("=== Zero-Copy Pattern ===\n\n");
    
    // 실제 데이터
    uint8_t actual_buffer[BUFFER_SIZE];
    
    // Zero-Copy: 포인터만 공유
    ZeroCopyBuffer zcbuf = {
        .data = actual_buffer,
        .size = BUFFER_SIZE
    };
    
    Producer_FillBuffer(&zcbuf);
    Consumer_ProcessBuffer(&zcbuf);  // 복사 없이 처리
    
    printf("\nZero-Copy: 메모리 복사 없이 데이터 전달\n");
    return 0;
}
