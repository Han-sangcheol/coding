/* Lazy Initialization Pattern
 * 필요할 때만 초기화 (지연 초기화)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* data;
    int size;
    int initialized;
} LazyBuffer;

void LazyBuffer_Init(LazyBuffer* buf, int size) {
    buf->data = NULL;
    buf->size = size;
    buf->initialized = 0;
    printf("[LazyBuffer] 구조체 생성 (아직 초기화 안됨)\n");
}

int* LazyBuffer_Get(LazyBuffer* buf) {
    if (!buf->initialized) {
        printf("[LazyBuffer] 첫 접근 → 이제 초기화!\n");
        buf->data = (int*)calloc(buf->size, sizeof(int));
        buf->initialized = 1;
    } else {
        printf("[LazyBuffer] 이미 초기화됨 → 기존 데이터 반환\n");
    }
    return buf->data;
}

void LazyBuffer_Destroy(LazyBuffer* buf) {
    if (buf->initialized) {
        free(buf->data);
        printf("[LazyBuffer] 해제\n");
    }
}

int main(void) {
    printf("=== Lazy Initialization Pattern ===\n\n");
    
    LazyBuffer buf;
    LazyBuffer_Init(&buf, 100);
    
    printf("\n1st access:\n");
    int* data1 = LazyBuffer_Get(&buf);
    
    printf("\n2nd access:\n");
    int* data2 = LazyBuffer_Get(&buf);
    
    LazyBuffer_Destroy(&buf);
    
    printf("\nLazy Init: 필요할 때만 초기화\n");
    return 0;
}
