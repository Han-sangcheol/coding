/*
 * ============================================================================
 * 파일명: 08_strategy_pattern.c
 * 기능: C언어 Strategy(전략) 패턴 구현 - 실제 압축 알고리즘 적용
 * 
 * ============================================================================
 * 1️⃣ Strategy 패턴이란?
 * ============================================================================
 * 
 * 알고리즘을 캡슐화하여 런타임에 교체 가능하게 만드는 패턴
 * 
 * 핵심: 함수 포인터로 알고리즘 전략 변경
 * 
 * 구조:
 *   [Context] → [Strategy 인터페이스] ← [구체적 전략들]
 *                    (함수 포인터)        - Strategy A
 *                                        - Strategy B
 *                                        - Strategy C
 * 
 * ============================================================================
 * 2️⃣ 임베디드에서 Strategy 활용
 * ============================================================================
 * 
 * - 압축 알고리즘 (None/RLE/LZ) ← 이 예제에서 구현
 * - 암호화 알고리즘 (AES/DES/RSA)
 * - 정렬 알고리즘 (Quick/Merge/Bubble)
 * - 통신 프로토콜 (UART/SPI/I2C)
 * 
 * ============================================================================
 * 3️⃣ 이 파일의 구현 내용
 * ============================================================================
 * 
 * [실제 압축 알고리즘 구현]
 *   1. None: 압축 없음 (원본 그대로 복사)
 *   2. RLE: Run-Length Encoding (연속된 바이트를 개수로 압축)
 *   3. LZ77: 간단한 Dictionary 방식 압축 (반복 패턴 참조)
 * 
 * [실제 테스트 데이터]
 *   - 반복 패턴이 많은 데이터로 압축 효과 확인
 *   - 압축률 계산 및 출력
 *   - 원본 데이터와 압축 데이터 비교
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_BUFFER_SIZE 2048

/* ============================================================================
 * Strategy 패턴의 핵심: 압축 전략 인터페이스 (함수 포인터)
 * ============================================================================
 * 
 * 모든 압축 알고리즘이 따라야 할 공통 인터페이스
 * 
 * 파라미터:
 *   - input: 압축할 원본 데이터
 *   - input_len: 원본 데이터 크기
 *   - output: 압축된 데이터를 저장할 버퍼
 *   - output_len: 압축된 데이터 크기 (출력)
 * 
 * 반환값: 0=성공, -1=실패
 * 
 * ============================================================================ */
typedef int (*CompressionStrategy)(const uint8_t* input, size_t input_len, 
                                   uint8_t* output, size_t* output_len);

/* ============================================================================
 * Compressor 구조체 (Context)
 * ============================================================================
 * 
 * Strategy 패턴의 Context 역할
 *   - strategy: 현재 선택된 압축 알고리즘 (함수 포인터)
 *   - strategy_name: 전략 이름 (디버깅용)
 * 
 * ============================================================================ */
typedef struct {
    CompressionStrategy strategy;  // 현재 압축 전략 (함수 포인터)
    char strategy_name[32];         // 전략 이름
} Compressor;

/* ============================================================================
 * 전략 1: 압축 없음 (None)
 * ============================================================================
 * 
 * 가장 단순한 전략: 원본 데이터를 그대로 복사
 * 
 * 사용 상황:
 *   - 이미 압축된 데이터 (압축 효과 없음)
 *   - 압축 시간을 절약하고 싶을 때
 *   - 디버깅 목적
 * 
 * ============================================================================ */
int Compress_None(const uint8_t* input, size_t input_len, 
                 uint8_t* output, size_t* output_len) {
    if (!input || !output || !output_len) return -1;
    
    memcpy(output, input, input_len);
    *output_len = input_len;
    
    printf("[None] 압축 없음: %zu → %zu bytes (압축률: 0.0%%)\n", 
           input_len, *output_len);
    return 0;
}

/* ============================================================================
 * 전략 2: RLE (Run-Length Encoding) 압축
 * ============================================================================
 * 
 * 알고리즘:
 *   - 연속된 동일한 바이트를 [카운트][값] 형태로 압축
 *   - 예: AAAABBB → 0x04 0x41 0x03 0x42
 *         (4개의 'A', 3개의 'B')
 * 
 * 압축 형식:
 *   [카운트(1바이트)][데이터(1바이트)] 반복
 *   - 카운트 범위: 1~255
 *   - 256개 이상 연속되면 여러 블록으로 나눔
 * 
 * 적용 분야:
 *   - 이미지 데이터 (BMP 등)
 *   - 로그 파일
 *   - 센서 데이터 (같은 값이 연속될 때)
 * 
 * ============================================================================ */
int Compress_RLE(const uint8_t* input, size_t input_len, 
                uint8_t* output, size_t* output_len) {
    if (!input || !output || !output_len || input_len == 0) return -1;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        // 연속된 동일 바이트 카운트 (최대 255개)
        while (i + count < input_len && 
               input[i + count] == current && 
               count < 255) {
            count++;
        }
        
        // [카운트][값] 형태로 저장
        if (out_idx + 2 > MAX_BUFFER_SIZE) {
            return -1;  // 버퍼 오버플로우
        }
        
        output[out_idx++] = count;      // 개수
        output[out_idx++] = current;    // 값
        
        i += count;
    }
    
    *output_len = out_idx;
    
    float ratio = (1.0f - ((float)*output_len / input_len)) * 100.0f;
    printf("[RLE] 압축: %zu → %zu bytes (압축률: %.1f%%)\n", 
           input_len, *output_len, ratio);
    
    return 0;
}

/* ============================================================================
 * 전략 3: 간단한 LZ77 스타일 압축
 * ============================================================================
 * 
 * 알고리즘:
 *   - 이전에 나온 패턴을 찾아서 참조로 대체
 *   - 슬라이딩 윈도우 방식 (간단 버전)
 * 
 * 압축 형식:
 *   - 리터럴 바이트: 0x00 [바이트]
 *   - 참조: 0x01 [거리(1바이트)][길이(1바이트)]
 * 
 * 예시:
 *   "ABCABC" → 0x00 A 0x00 B 0x00 C 0x01 0x03 0x03
 *              (리터럴 ABC, 그 다음 3바이트 전을 3바이트 복사)
 * 
 * 실무 적용:
 *   - 텍스트 압축
 *   - 펌웨어 업데이트 데이터
 *   - 네트워크 패킷
 * 
 * ============================================================================ */
int Compress_LZ(const uint8_t* input, size_t input_len, 
               uint8_t* output, size_t* output_len) {
    if (!input || !output || !output_len || input_len == 0) return -1;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    #define MIN_MATCH_LEN 3     // 최소 매칭 길이
    #define WINDOW_SIZE 255     // 검색 윈도우 크기
    
    while (i < input_len) {
        int best_match_dist = 0;
        int best_match_len = 0;
        
        // 이전 데이터에서 매칭 패턴 검색
        int search_start = (i > WINDOW_SIZE) ? (i - WINDOW_SIZE) : 0;
        
        for (int j = search_start; j < (int)i; j++) {
            int match_len = 0;
            
            // 현재 위치와 이전 위치의 패턴 비교
            while (i + match_len < input_len && 
                   j + match_len < (int)i &&
                   input[j + match_len] == input[i + match_len] &&
                   match_len < 255) {
                match_len++;
            }
            
            // 더 긴 매칭 발견
            if (match_len >= MIN_MATCH_LEN && match_len > best_match_len) {
                best_match_len = match_len;
                best_match_dist = i - j;
            }
        }
        
        // 매칭 발견시 참조로 저장, 아니면 리터럴로 저장
        if (best_match_len >= MIN_MATCH_LEN) {
            // 참조: [0x01][거리][길이]
            if (out_idx + 3 > MAX_BUFFER_SIZE) return -1;
            
            output[out_idx++] = 0x01;                           // 참조 마커
            output[out_idx++] = (uint8_t)best_match_dist;      // 거리
            output[out_idx++] = (uint8_t)best_match_len;       // 길이
            i += best_match_len;
        } else {
            // 리터럴: [0x00][바이트]
            if (out_idx + 2 > MAX_BUFFER_SIZE) return -1;
            
            output[out_idx++] = 0x00;           // 리터럴 마커
            output[out_idx++] = input[i];       // 실제 바이트
            i++;
        }
    }
    
    *output_len = out_idx;
    
    float ratio = (1.0f - ((float)*output_len / input_len)) * 100.0f;
    printf("[LZ77] 압축: %zu → %zu bytes (압축률: %.1f%%)\n", 
           input_len, *output_len, ratio);
    
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

/* ============================================================================
 * 압축 실행 함수
 * ============================================================================
 * 
 * Strategy 패턴의 핵심: 함수 포인터를 통해 선택된 전략 실행
 * 
 * ============================================================================ */
int Compressor_Compress(Compressor* comp, const uint8_t* input, size_t input_len,
                       uint8_t* output, size_t* output_len) {
    printf("\n[Compressor] 압축 시작 (전략: %s)\n", comp->strategy_name);
    
    // 핵심: strategy 함수 포인터 호출 (다형성)
    int result = comp->strategy(input, input_len, output, output_len);
    
    if (result == 0) {
        printf("[Compressor] 압축 완료\n");
    } else {
        printf("[Compressor] 압축 실패\n");
    }
    
    return result;
}

/* ============================================================================
 * 유틸리티: 데이터 출력 함수
 * ============================================================================ */
void Print_Data(const char* label, const uint8_t* data, size_t len, int max_print) {
    printf("%s (%zu bytes): ", label, len);
    
    for (size_t i = 0; i < len && i < (size_t)max_print; i++) {
        if (data[i] >= 32 && data[i] <= 126) {
            printf("%c", data[i]);  // 출력 가능한 ASCII
        } else {
            printf("[%02X]", data[i]);  // 16진수
        }
    }
    
    if (len > (size_t)max_print) {
        printf("... (생략)");
    }
    printf("\n");
}

/* ============================================================================
 * 테스트 데이터 생성 함수들
 * ============================================================================ */

// 테스트 1: 반복 패턴 데이터 (RLE에 유리)
void Generate_RepetitiveData(uint8_t* buffer, size_t* len) {
    size_t idx = 0;
    
    // AAAAAAA...BBB...CCC... 형태
    for (int i = 0; i < 20; i++) buffer[idx++] = 'A';
    for (int i = 0; i < 15; i++) buffer[idx++] = 'B';
    for (int i = 0; i < 25; i++) buffer[idx++] = 'C';
    for (int i = 0; i < 10; i++) buffer[idx++] = 'D';
    for (int i = 0; i < 30; i++) buffer[idx++] = '0';
    for (int i = 0; i < 20; i++) buffer[idx++] = '1';
    
    *len = idx;
    printf("생성된 데이터: 연속 반복 패턴 (%zu bytes)\n", *len);
}

// 테스트 2: 반복 문자열 패턴 (LZ에 유리)
void Generate_PatternData(uint8_t* buffer, size_t* len) {
    size_t idx = 0;
    const char* pattern = "HELLO";
    
    // "HELLOHELLOHELLO..." 형태
    for (int i = 0; i < 30; i++) {
        for (int j = 0; pattern[j] != '\0'; j++) {
            buffer[idx++] = pattern[j];
        }
    }
    
    *len = idx;
    printf("생성된 데이터: 반복 문자열 패턴 (%zu bytes)\n", *len);
}

// 테스트 3: 센서 데이터 시뮬레이션
void Generate_SensorData(uint8_t* buffer, size_t* len) {
    size_t idx = 0;
    
    // 온도 센서: 25도 근처에서 변동
    for (int i = 0; i < 50; i++) {
        buffer[idx++] = 25;  // 대부분 25도
    }
    buffer[idx++] = 26;
    buffer[idx++] = 26;
    for (int i = 0; i < 30; i++) {
        buffer[idx++] = 25;
    }
    buffer[idx++] = 24;
    for (int i = 0; i < 20; i++) {
        buffer[idx++] = 25;
    }
    
    *len = idx;
    printf("생성된 데이터: 센서 데이터 시뮬레이션 (%zu bytes)\n", *len);
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Strategy 패턴 - 실제 압축 알고리즘 예제\n");
    printf("========================================\n\n");
    
    uint8_t input[MAX_BUFFER_SIZE];
    uint8_t output[MAX_BUFFER_SIZE];
    size_t input_len;
    size_t output_len;
    
    // Compressor 생성 (Context)
    Compressor compressor;
    Compressor_Init(&compressor, Compress_None, "None");
    
    // ========================================================================
    // 테스트 1: 연속 반복 데이터 (RLE에 최적)
    // ========================================================================
    printf("\n========================================\n");
    printf("테스트 1: 연속 반복 데이터\n");
    printf("========================================\n");
    
    Generate_RepetitiveData(input, &input_len);
    Print_Data("원본", input, input_len, 40);
    
    // 압축 없음
    Compressor_SetStrategy(&compressor, Compress_None, "None");
    Compressor_Compress(&compressor, input, input_len, output, &output_len);
    Print_Data("압축 결과", output, output_len, 40);
    
    // RLE 압축 (연속 데이터에 효과적)
    Compressor_SetStrategy(&compressor, Compress_RLE, "RLE");
    Compressor_Compress(&compressor, input, input_len, output, &output_len);
    Print_Data("압축 결과", output, output_len, 40);
    
    // LZ 압축
    Compressor_SetStrategy(&compressor, Compress_LZ, "LZ77");
    Compressor_Compress(&compressor, input, input_len, output, &output_len);
    Print_Data("압축 결과", output, output_len, 40);
    
    // ========================================================================
    // 테스트 2: 반복 문자열 패턴 (LZ에 최적)
    // ========================================================================
    printf("\n========================================\n");
    printf("테스트 2: 반복 문자열 패턴\n");
    printf("========================================\n");
    
    Generate_PatternData(input, &input_len);
    Print_Data("원본", input, input_len, 40);
    
    // 압축 없음
    Compressor_SetStrategy(&compressor, Compress_None, "None");
    Compressor_Compress(&compressor, input, input_len, output, &output_len);
    
    // RLE 압축 (반복 문자열에는 비효율적)
    Compressor_SetStrategy(&compressor, Compress_RLE, "RLE");
    Compressor_Compress(&compressor, input, input_len, output, &output_len);
    
    // LZ 압축 (반복 패턴에 효과적)
    Compressor_SetStrategy(&compressor, Compress_LZ, "LZ77");
    Compressor_Compress(&compressor, input, input_len, output, &output_len);
    Print_Data("압축 결과", output, output_len, 40);
    
    // ========================================================================
    // 테스트 3: 센서 데이터 (실무 시나리오)
    // ========================================================================
    printf("\n========================================\n");
    printf("테스트 3: 센서 데이터 (임베디드 실무)\n");
    printf("========================================\n");
    
    Generate_SensorData(input, &input_len);
    Print_Data("원본", input, input_len, 40);
    
    // RLE 압축 (센서 데이터에 매우 효과적)
    Compressor_SetStrategy(&compressor, Compress_RLE, "RLE");
    Compressor_Compress(&compressor, input, input_len, output, &output_len);
    Print_Data("압축 결과", output, output_len, 40);
    
    // LZ 압축
    Compressor_SetStrategy(&compressor, Compress_LZ, "LZ77");
    Compressor_Compress(&compressor, input, input_len, output, &output_len);
    
    // ========================================================================
    // 요약: Strategy 패턴의 장점
    // ========================================================================
    printf("\n========================================\n");
    printf("Strategy 패턴의 핵심 장점\n");
    printf("========================================\n");
    printf("✅ 런타임에 알고리즘 동적 변경\n");
    printf("✅ 데이터 특성에 따라 최적 전략 선택\n");
    printf("✅ 새로운 압축 알고리즘 추가 용이\n");
    printf("✅ if/switch 조건문 제거로 깔끔한 코드\n");
    printf("✅ 각 알고리즘을 독립적으로 테스트 가능\n");
    
    printf("\n========================================\n");
    printf("Strategy 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Strategy 패턴의 장점 및 학습 포인트
 * ============================================================================
 * 
 * 1️⃣ 런타임에 알고리즘 교체 가능
 *    - 함수 포인터를 통해 실행 중 전략 변경
 *    - 조건문(if/switch) 없이 깔끔한 구조
 * 
 * 2️⃣ 알고리즘 추가/변경이 쉬움
 *    - 새로운 압축 알고리즘 추가 시 기존 코드 수정 불필요
 *    - CompressionStrategy 인터페이스만 따르면 됨
 * 
 * 3️⃣ 개방-폐쇄 원칙(OCP) 준수
 *    - 확장에는 열려있고, 수정에는 닫혀있음
 *    - 새 전략 추가 = 새 함수만 작성
 * 
 * 4️⃣ 테스트 용이
 *    - 각 압축 알고리즘을 독립적으로 테스트
 *    - Mock 전략으로 단위 테스트 가능
 * 
 * 5️⃣ 실무 적용 사례
 *    - 데이터 특성에 따라 최적 압축 알고리즘 선택
 *    - 암호화 알고리즘 전환 (AES/DES/RSA)
 *    - 통신 프로토콜 전환 (UART/SPI/I2C)
 *    - 정렬 알고리즘 선택 (Quick/Merge/Bubble)
 * 
 * ============================================================================
 * 직접 작성할 때 체크리스트
 * ============================================================================
 * 
 * [1단계] 전략 인터페이스 정의
 *   □ 모든 전략이 따를 공통 함수 시그니처 정의
 *   □ typedef로 함수 포인터 타입 선언
 * 
 * [2단계] Context 구조체 작성
 *   □ 전략 함수 포인터 멤버 추가
 *   □ 필요한 상태 데이터 추가
 * 
 * [3단계] 구체적인 전략 함수들 구현
 *   □ 각 전략이 동일한 인터페이스 준수
 *   □ 각자의 알고리즘 로직 구현
 * 
 * [4단계] Context 함수 작성
 *   □ Init: 초기 전략 설정
 *   □ SetStrategy: 전략 변경
 *   □ Execute: 현재 전략 실행 (함수 포인터 호출)
 * 
 * [5단계] 사용
 *   □ Context 생성
 *   □ 초기 전략 설정
 *   □ 필요시 SetStrategy로 전략 변경
 *   □ Execute로 현재 전략 실행
 * 
 * ============================================================================
 * 압축 알고리즘 비교
 * ============================================================================
 * 
 * [RLE - Run-Length Encoding]
 *   장점: 구현 간단, 빠름, 연속 데이터에 효과적
 *   단점: 연속되지 않은 데이터는 오히려 커질 수 있음
 *   적용: 센서 데이터, 단순 이미지, 로그 파일
 * 
 * [LZ77 - Lempel-Ziv]
 *   장점: 반복 패턴에 매우 효과적, 범용성 높음
 *   단점: 구현 복잡, 처리 시간 길음, 메모리 필요
 *   적용: 텍스트, 펌웨어, 일반 데이터
 * 
 * [None - 압축 없음]
 *   장점: 빠름, 단순
 *   단점: 압축 효과 없음
 *   적용: 이미 압축된 데이터, 디버깅, 시간 중요한 경우
 * 
 * ============================================================================
 * 질문 대응
 * ============================================================================
 * 
 * Q: Strategy 패턴을 사용한 경험은?
 * A: "임베디드 시스템에서 센서 데이터를 전송할 때 Strategy 패턴으로
 *     압축 알고리즘을 구현했습니다. 센서 특성에 따라 RLE, LZ 등을
 *     런타임에 선택할 수 있어 전송 효율을 최적화할 수 있었습니다.
 *     또한 새로운 압축 알고리즘 추가 시 기존 코드 수정 없이
 *     함수만 추가하면 되어 유지보수가 쉬웠습니다."
 * 
 * ============================================================================
 */
