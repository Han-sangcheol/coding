/*
 * ============================================================================
 * 파일명: 12_object_pool.c
 * 기능: C언어 Object Pool 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Object Pool 패턴이란?
 * ============================================================================
 * 
 * 자주 생성/삭제되는 객체를 미리 만들어두고 재사용하는 패턴
 * 
 * Memory Pool vs Object Pool:
 *   - Memory Pool: 단순 메모리 블록
 *   - Object Pool: 초기화된 객체
 * 
 * ============================================================================
 * 2️⃣ 임베디드에서 Object Pool 활용
 * ============================================================================
 * 
 * - 패킷 객체 재사용
 * - 이벤트 객체 풀
 * - 센서 데이터 버퍼
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_PACKETS 8

/* 패킷 객체 */
typedef struct Packet {
    int id;
    uint8_t data[64];
    size_t length;
    int in_use;
} Packet;

/* Object Pool */
typedef struct {
    Packet packets[MAX_PACKETS];
    int next_id;
} PacketPool;

/* 풀 초기화 */
void PacketPool_Init(PacketPool* pool) {
    for (int i = 0; i < MAX_PACKETS; i++) {
        pool->packets[i].id = -1;
        pool->packets[i].length = 0;
        pool->packets[i].in_use = 0;
        memset(pool->packets[i].data, 0, sizeof(pool->packets[i].data));
    }
    pool->next_id = 1;
    printf("[PacketPool] 초기화: %d 패킷 객체\n", MAX_PACKETS);
}

/* 패킷 획득 */
Packet* PacketPool_Acquire(PacketPool* pool) {
    for (int i = 0; i < MAX_PACKETS; i++) {
        if (!pool->packets[i].in_use) {
            pool->packets[i].in_use = 1;
            pool->packets[i].id = pool->next_id++;
            pool->packets[i].length = 0;
            printf("[PacketPool] 패킷 획득: ID=%d\n", pool->packets[i].id);
            return &pool->packets[i];
        }
    }
    printf("[PacketPool] 획득 실패: 풀이 가득 참\n");
    return NULL;
}

/* 패킷 반환 */
void PacketPool_Release(PacketPool* pool, Packet* packet) {
    if (packet) {
        printf("[PacketPool] 패킷 반환: ID=%d\n", packet->id);
        packet->in_use = 0;
        packet->id = -1;
        packet->length = 0;
        memset(packet->data, 0, sizeof(packet->data));
    }
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Object Pool 패턴 예제\n");
    printf("========================================\n\n");
    
    PacketPool pool;
    PacketPool_Init(&pool);
    
    // 패킷 획득 및 사용
    printf("\n=== 패킷 획득 및 사용 ===\n");
    Packet* pkt1 = PacketPool_Acquire(&pool);
    Packet* pkt2 = PacketPool_Acquire(&pool);
    
    if (pkt1) {
        strcpy((char*)pkt1->data, "Hello");
        pkt1->length = 5;
        printf("패킷 %d: %s\n", pkt1->id, pkt1->data);
    }
    
    if (pkt2) {
        strcpy((char*)pkt2->data, "World");
        pkt2->length = 5;
        printf("패킷 %d: %s\n", pkt2->id, pkt2->data);
    }
    
    // 패킷 반환
    printf("\n=== 패킷 반환 ===\n");
    PacketPool_Release(&pool, pkt1);
    PacketPool_Release(&pool, pkt2);
    
    // 재사용
    printf("\n=== 패킷 재사용 ===\n");
    Packet* pkt3 = PacketPool_Acquire(&pool);
    if (pkt3) {
        printf("재사용 패킷 ID: %d\n", pkt3->id);
        PacketPool_Release(&pool, pkt3);
    }
    
    printf("\n========================================\n");
    printf("Object Pool 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Object Pool의 장점
 * ============================================================================
 * 
 * 1. 객체 생성/소멸 오버헤드 제거
 * 2. 메모리 단편화 방지
 * 3. 예측 가능한 성능
 * 4. 초기화 비용 절감
 * 
 * ============================================================================
 */
