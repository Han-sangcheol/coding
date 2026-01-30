/*
 * ============================================================================
 * 파일명: 13_reference_counting.c
 * 기능: C언어 Reference Counting(참조 카운팅) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Reference Counting 패턴이란?
 * ============================================================================
 * 
 * 객체의 참조 횟수를 추적하여 자동으로 메모리를 관리하는 패턴
 * 
 * 핵심:
 *   - ref_count: 참조 카운터
 *   - AddRef(): 참조 증가
 *   - Release(): 참조 감소 (0이 되면 자동 해제)
 * 
 * ============================================================================
 * 2️⃣ 임베디드에서 Reference Counting 활용
 * ============================================================================
 * 
 * - 공유 버퍼 관리
 * - 센서 데이터 공유
 * - 설정 객체 공유
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 참조 카운팅 객체 */
typedef struct {
    int ref_count;
    char* data;
    size_t size;
} RefCountedBuffer;

/* 버퍼 생성 */
RefCountedBuffer* Buffer_Create(size_t size) {
    RefCountedBuffer* buf = (RefCountedBuffer*)malloc(sizeof(RefCountedBuffer));
    if (buf) {
        buf->ref_count = 1;  // 초기 참조 카운트 = 1
        buf->data = (char*)malloc(size);
        buf->size = size;
        printf("[RefCount] 버퍼 생성: %p (참조=%d)\n", (void*)buf, buf->ref_count);
    }
    return buf;
}

/* 참조 증가 */
void Buffer_AddRef(RefCountedBuffer* buf) {
    if (buf) {
        buf->ref_count++;
        printf("[RefCount] 참조 증가: %p (참조=%d)\n", (void*)buf, buf->ref_count);
    }
}

/* 참조 감소 및 자동 해제 */
void Buffer_Release(RefCountedBuffer* buf) {
    if (buf) {
        buf->ref_count--;
        printf("[RefCount] 참조 감소: %p (참조=%d)\n", (void*)buf, buf->ref_count);
        
        if (buf->ref_count == 0) {
            printf("[RefCount] 참조 카운트 0 → 자동 해제: %p\n", (void*)buf);
            free(buf->data);
            free(buf);
        }
    }
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Reference Counting 패턴 예제\n");
    printf("========================================\n\n");
    
    // 버퍼 생성
    RefCountedBuffer* buf = Buffer_Create(100);
    strcpy(buf->data, "Shared Data");
    
    // 모듈 A가 참조
    printf("\n=== 모듈 A가 참조 ===\n");
    Buffer_AddRef(buf);
    
    // 모듈 B가 참조
    printf("\n=== 모듈 B가 참조 ===\n");
    Buffer_AddRef(buf);
    
    // 모듈 A가 해제
    printf("\n=== 모듈 A가 해제 ===\n");
    Buffer_Release(buf);
    
    // 모듈 B가 해제
    printf("\n=== 모듈 B가 해제 ===\n");
    Buffer_Release(buf);
    
    // 원본 해제 (자동으로 메모리 해제됨)
    printf("\n=== 원본 해제 ===\n");
    Buffer_Release(buf);
    
    printf("\n========================================\n");
    printf("Reference Counting 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Reference Counting의 장점
 * ============================================================================
 * 
 * 1. 자동 메모리 관리
 * 2. 공유 객체 관리 용이
 * 3. 메모리 누수 방지
 * 
 * 주의사항:
 * - 순환 참조 문제 (Circular Reference)
 * - 멀티스레드 환경에서 원자적 연산 필요
 * 
 * ============================================================================
 */
