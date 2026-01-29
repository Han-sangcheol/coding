/*
 * ============================================================================
 * 파일명: 08_strategy_pattern.c
 * 기능: C언어 Strategy(전략) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Strategy 패턴이란?
 * ============================================================================
 * 
 * 알고리즘을 캡슐화하여 런타임에 교체 가능하게 만드는 패턴
 * 
 * 핵심: 함수 포인터로 알고리즘 전략 변경
 * 
 * ============================================================================
 * 2️⃣ 임베디드에서 Strategy 활용
 * ============================================================================
 * 
 * - 압축 알고리즘 (None/RLE/LZ)
 * - 암호화 알고리즘 (AES/DES/RSA)
 * - 정렬 알고리즘
 * - 통신 프로토콜
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* 압축 전략 인터페이스 */
typedef int (*CompressionStrategy)(const uint8_t* input, size_t input_len, 
                                   uint8_t* output, size_t* output_len);

/* 압축기 구조체 */
typedef struct {
    CompressionStrategy strategy;
    char strategy_name[32];
} Compressor;

/* 압축 없음 전략 */
int Compress_None(const uint8_t* input, size_t input_len, 
                 uint8_t* output, size_t* output_len) {
    memcpy(output, input, input_len);
    *output_len = input_len;
    printf("[Strategy] 압축 없음: %zu → %zu bytes\n", input_len, *output_len);
    return 0;
}

/* RLE 압축 전략 (간단 시뮬레이션) */
int Compress_RLE(const uint8_t* input, size_t input_len, 
                uint8_t* output, size_t* output_len) {
    *output_len = input_len / 2;  // 시뮬레이션
    printf("[Strategy] RLE 압축: %zu → %zu bytes (50%% 압축)\n", 
           input_len, *output_len);
    return 0;
}

/* LZ 압축 전략 (간단 시뮬레이션) */
int Compress_LZ(const uint8_t* input, size_t input_len, 
               uint8_t* output, size_t* output_len) {
    *output_len = input_len / 3;  // 시뮬레이션
    printf("[Strategy] LZ 압축: %zu → %zu bytes (67%% 압축)\n", 
           input_len, *output_len);
    return 0;
}

/* Compressor 초기화 */
void Compressor_Init(Compressor* comp, CompressionStrategy strategy, 
                    const char* name) {
    comp->strategy = strategy;
    strncpy(comp->strategy_name, name, sizeof(comp->strategy_name) - 1);
    printf("[Compressor] 전략 설정: %s\n", name);
}

/* 전략 변경 */
void Compressor_SetStrategy(Compressor* comp, CompressionStrategy strategy, 
                           const char* name) {
    comp->strategy = strategy;
    strncpy(comp->strategy_name, name, sizeof(comp->strategy_name) - 1);
    printf("[Compressor] 전략 변경: %s\n", name);
}

/* 압축 실행 */
int Compressor_Compress(Compressor* comp, const uint8_t* input, size_t input_len,
                       uint8_t* output, size_t* output_len) {
    printf("[Compressor] 압축 시작 (전략: %s)\n", comp->strategy_name);
    return comp->strategy(input, input_len, output, output_len);
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Strategy 패턴 예제\n");
    printf("========================================\n\n");
    
    uint8_t input[1000];
    uint8_t output[1000];
    size_t output_len;
    
    // Compressor 생성
    Compressor compressor;
    Compressor_Init(&compressor, Compress_None, "None");
    
    // 압축 없음
    printf("\n=== 압축 없음 전략 ===\n");
    Compressor_Compress(&compressor, input, 1000, output, &output_len);
    
    // RLE 압축으로 전략 변경
    printf("\n=== RLE 압축 전략으로 변경 ===\n");
    Compressor_SetStrategy(&compressor, Compress_RLE, "RLE");
    Compressor_Compress(&compressor, input, 1000, output, &output_len);
    
    // LZ 압축으로 전략 변경
    printf("\n=== LZ 압축 전략으로 변경 ===\n");
    Compressor_SetStrategy(&compressor, Compress_LZ, "LZ");
    Compressor_Compress(&compressor, input, 1000, output, &output_len);
    
    printf("\n========================================\n");
    printf("Strategy 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Strategy 패턴의 장점
 * ============================================================================
 * 
 * 1. 런타임에 알고리즘 교체 가능
 * 2. 알고리즘 추가/변경이 쉬움
 * 3. 조건문(if/switch) 제거
 * 4. 테스트 용이
 * 
 * ============================================================================
 */
