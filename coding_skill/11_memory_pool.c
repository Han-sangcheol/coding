/*
 * ============================================================================
 * 파일명: 11_memory_pool.c
 * 기능: C언어 Memory Pool 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Memory Pool 패턴이란?
 * ============================================================================
 * 
 * 고정 크기 메모리 블록을 사전 할당하여 빠르고 예측 가능한 메모리 관리
 * 
 * 핵심:
 *   - 사전 할당 (Pre-allocation)
 *   - 고정 크기 블록
 *   - O(1) 할당/해제
 *   - 단편화 방지
 * 
 * ============================================================================
 * 2️⃣ 왜 Memory Pool을 쓰나?
 * ============================================================================
 * 
 * malloc/free의 문제점:
 *   ❌ 할당 시간 예측 불가
 *   ❌ 메모리 단편화
 *   ❌ 실시간 시스템에 부적합
 * 
 * Memory Pool의 장점:
 *   ✅ 결정론적 성능 (Deterministic)
 *   ✅ 단편화 없음
 *   ✅ 빠른 할당/해제
 *   ✅ 메모리 오버헤드 감소
 * 
 * ============================================================================
 * 3️⃣ 임베디드에서 Memory Pool 활용
 * ============================================================================
 * 
 * - 패킷 버퍼 풀
 * - 태스크 스택 풀
 * - 메시지 큐 버퍼
 * - 실시간 데이터 처리
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define POOL_BLOCK_SIZE 64
#define POOL_BLOCK_COUNT 10

/* 메모리 블록 구조체 */
typedef struct MemBlock {
    struct MemBlock* next;  // 다음 free 블록
    uint8_t data[POOL_BLOCK_SIZE];
} MemBlock;

/* 메모리 풀 구조체 */
typedef struct {
    MemBlock* free_list;    // Free 블록 리스트
    MemBlock blocks[POOL_BLOCK_COUNT];  // 실제 메모리 블록들
    int allocated_count;
    int total_blocks;
} MemoryPool;

/* 메모리 풀 초기화 */
void MemPool_Init(MemoryPool* pool) {
    pool->free_list = NULL;
    pool->allocated_count = 0;
    pool->total_blocks = POOL_BLOCK_COUNT;
    
    // 모든 블록을 free list에 연결
    for (int i = 0; i < POOL_BLOCK_COUNT; i++) {
        pool->blocks[i].next = pool->free_list;
        pool->free_list = &pool->blocks[i];
    }
    
    printf("[MemPool] 초기화: %d 블록 (블록 크기: %d bytes)\n", 
           POOL_BLOCK_COUNT, POOL_BLOCK_SIZE);
}

/* 메모리 할당 */
void* MemPool_Alloc(MemoryPool* pool) {
    if (pool->free_list == NULL) {
        printf("[MemPool] 할당 실패: 풀이 가득 참\n");
        return NULL;
    }
    
    // Free list에서 블록 하나 가져오기
    MemBlock* block = pool->free_list;
    pool->free_list = block->next;
    pool->allocated_count++;
    
    printf("[MemPool] 할당 성공: 블록 %p (사용 중: %d/%d)\n", 
           (void*)block, pool->allocated_count, pool->total_blocks);
    
    return block->data;
}

/* 메모리 해제 */
void MemPool_Free(MemoryPool* pool, void* ptr) {
    if (ptr == NULL) {
        return;
    }
    
    // data 포인터에서 MemBlock 포인터 계산
    MemBlock* block = (MemBlock*)((uint8_t*)ptr - offsetof(MemBlock, data));
    
    // Free list에 다시 추가
    block->next = pool->free_list;
    pool->free_list = block;
    pool->allocated_count--;
    
    printf("[MemPool] 해제 완료: 블록 %p (사용 중: %d/%d)\n", 
           (void*)block, pool->allocated_count, pool->total_blocks);
}

/* 풀 상태 출력 */
void MemPool_PrintStatus(MemoryPool* pool) {
    printf("[MemPool] 상태: 사용 중 %d / 전체 %d 블록 (여유: %d)\n",
           pool->allocated_count, pool->total_blocks,
           pool->total_blocks - pool->allocated_count);
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Memory Pool 패턴 예제\n");
    printf("========================================\n\n");
    
    MemoryPool pool;
    MemPool_Init(&pool);
    
    // 메모리 할당 테스트
    printf("\n=== 메모리 할당 테스트 ===\n");
    void* buffers[5];
    for (int i = 0; i < 5; i++) {
        buffers[i] = MemPool_Alloc(&pool);
        if (buffers[i]) {
            sprintf((char*)buffers[i], "Buffer_%d", i);
        }
    }
    
    MemPool_PrintStatus(&pool);
    
    // 메모리 사용
    printf("\n=== 할당된 메모리 사용 ===\n");
    for (int i = 0; i < 5; i++) {
        if (buffers[i]) {
            printf("Buffer %d: %s\n", i, (char*)buffers[i]);
        }
    }
    
    // 메모리 해제
    printf("\n=== 메모리 해제 ===\n");
    for (int i = 0; i < 5; i++) {
        MemPool_Free(&pool, buffers[i]);
    }
    
    MemPool_PrintStatus(&pool);
    
    // 재할당 테스트
    printf("\n=== 재할당 테스트 ===\n");
    void* buf = MemPool_Alloc(&pool);
    printf("재할당된 메모리: %p\n", buf);
    MemPool_Free(&pool, buf);
    
    printf("\n========================================\n");
    printf("Memory Pool 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Memory Pool 패턴의 장점
 * ============================================================================
 * 
 * 1. 결정론적 성능 (O(1) 할당/해제)
 * 2. 메모리 단편화 방지
 * 3. 실시간 시스템에 적합
 * 4. 메모리 오버헤드 감소
 * 
 * ============================================================================
 * 면접 대응
 * ============================================================================
 * 
 * Q: Memory Pool을 왜 사용하나요?
 * A: "실시간 시스템에서 malloc/free는 할당 시간이 예측 불가하고
 *     단편화 문제가 있습니다. Memory Pool은 사전 할당으로 O(1) 성능과
 *     예측 가능한 메모리 관리를 제공합니다."
 * 
 * ============================================================================
 */
