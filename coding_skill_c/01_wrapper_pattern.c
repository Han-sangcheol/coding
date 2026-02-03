/*
 * ============================================================================
 * 파일명: 01_wrapper_pattern.c
 * 기능: C언어 Wrapper(래퍼) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Wrapper(래퍼) 패턴이란?
 * ============================================================================
 * 
 * 💡 핵심 개념:
 * 
 * "특정 기능이 있을 때, 이 기능을 사용할 때 추가적으로 실행되거나
 *  조건 검사 등을 같이 하기 위해 감싸는 패턴 적용 방식"
 * 
 * 기존 함수, 라이브러리, 하드웨어 접근 코드를 한 겹 감싸서
 * 사용자는 '단순한 인터페이스'만 쓰게 만드는 설계 패턴
 * 
 * 구조:
 *   [사용자 코드]
 *        ↓
 *   [Wrapper 함수]   ← 추상화 계층 (추가 기능, 검증, 로깅 등)
 *        ↓
 *   [HAL / Driver / Library / HW]  ← 실제 기능
 * 
 * Wrapper가 추가로 제공하는 기능:
 *   ✅ 조건 검사 (유효성 검증, NULL 체크 등)
 *   ✅ 에러 처리 (에러 코드 변환, 복구 로직)
 *   ✅ 로깅/디버깅 (함수 호출 추적, 파라미터 로그)
 *   ✅ 리소스 관리 (자동 초기화, 정리)
 *   ✅ 성능 측정 (실행 시간 측정)
 *   ✅ 재시도 로직 (통신 실패 시 재시도)
 * 
 * ============================================================================
 * 2️⃣ 왜 C에서 Wrapper 패턴을 쓰나?
 * ============================================================================
 * 
 * C는 객체지향이 없기 때문에, 직접 접근하면 유지보수가 지옥이 됩니다.
 * 
 * Wrapper를 쓰는 이유:
 *   ✅ 하드웨어 변경 대비 (MCU 교체)
 *   ✅ HAL / 드라이버 교체 대비
 *   ✅ 테스트 / Mock 가능
 *   ✅ 의존성 분리 (결합도 감소)
 *   ✅ 인터페이스 고정
 * 
 * 👉 특히 임베디드 + FW 개발 시 매우 중요합니다.
 * 
 * ============================================================================
 * 3️⃣ 가장 기본적인 Wrapper 예제
 * ============================================================================
 * 
 * ❌ 나쁜 예 (직접 HAL 호출):
 *    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
 *    
 *    문제점:
 *    - HAL 변경되면 전부 수정
 *    - 테스트 불가능
 *    - 의미가 코드에 드러나지 않음
 * 
 * ✅ Wrapper 적용:
 * 
 *    (1) wrapper.h
 *        void LED_On(void);
 *        void LED_Off(void);
 * 
 *    (2) wrapper.c
 *        void LED_On(void) {
 *            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
 *        }
 * 
 *    (3) 사용자 코드
 *        LED_On();  // 📌 사용자는 GPIO도, HAL도 모름
 * 
 * ============================================================================
 * 4️⃣ Wrapper의 핵심 가치 (중요)
 * ============================================================================
 * 
 * ❗ Wrapper는 "기능"이 아니라 "경계"다
 * 
 * 구분        | 역할
 * ------------|----------------------------------
 * App         | "무엇을 할지" (비즈니스 로직)
 * Wrapper     | "어떻게 접근할지" (인터페이스)
 * Driver/HAL  | "실제 동작" (하드웨어 제어)
 * 
 * ============================================================================
 * 5️⃣ MCU / 통신 변경에 강한 Wrapper 구조
 * ============================================================================
 * 
 * UART → CAN 변경 예:
 * 
 *    wrapper 인터페이스:
 *        void FOTA_Receive(uint8_t *buf, uint32_t len);
 * 
 *    UART 기반 구현:
 *        void FOTA_Receive(uint8_t *buf, uint32_t len) {
 *            HAL_UART_Receive(&huart1, buf, len, HAL_MAX_DELAY);
 *        }
 * 
 *    CAN 기반 구현 (나중에 교체):
 *        void FOTA_Receive(uint8_t *buf, uint32_t len) {
 *            CAN_Receive(buf, len);
 *        }
 * 
 *    📌 상위 로직은 단 한 줄도 수정 안 함
 * 
 * ============================================================================
 * 6️⃣ 구조체 + 함수 포인터 Wrapper (고급)
 * ============================================================================
 * 
 * 👉 HAL 추상화에서 가장 많이 쓰는 패턴
 * 
 * 정의:
 *    typedef struct {
 *        int (*read)(uint8_t *buf, uint32_t len);
 *        int (*write)(uint8_t *buf, uint32_t len);
 *    } CommOps;
 * 
 * UART 구현:
 *    int uart_read(uint8_t *buf, uint32_t len) {
 *        return HAL_UART_Receive(&huart1, buf, len, 1000);
 *    }
 * 
 *    CommOps comm = {
 *        .read = uart_read,
 *        .write = uart_write
 *    };
 * 
 * 사용:
 *    comm.read(rx_buf, 128);
 * 
 * 📌 이건 C에서 객체지향 흉내 내는 핵심 기법
 * 
 * ============================================================================
 * 7️⃣ Wrapper vs Adapter 차이
 * ============================================================================
 * 
 * 구분            | Wrapper              | Adapter
 * ----------------|----------------------|------------------------
 * 목적            | 숨김, 단순화         | 인터페이스 변환
 * 기존 API 변경   | ❌                   | ❌
 * 인터페이스      | 새로 정의            | 기존 맞춤
 * 임베디드        | 매우 흔함            | 드라이버 통합 시 사용
 * 
 * ============================================================================
 * 8️⃣ 임베디드에서 Wrapper 설계 시 주의점
 * ============================================================================
 * 
 * ❌ 피해야 할 것:
 *    - Wrapper 안에서 로직 비대화
 *    - 상태 관리까지 넣어버림
 *    - HAL 함수 그대로 노출
 * 
 * ✅ 해야 할 것:
 *    - Wrapper는 얇아야 한다
 *    - 정책은 App 계층
 *    - HW 의존성은 Wrapper까지만
 * 
 * ============================================================================
 * 9️⃣ 지금 상황에 딱 맞는 적용 예
 * ============================================================================
 * 
 * 임베디드 FW 개발에서 자주 다루는 것들에 Wrapper 적용:
 * 
 *   - FOTA 통신 (UART ↔ CAN)
 *   - Flash Write / Erase
 *   - Motor Driver
 *   - Sensor Read
 *   - RTOS Delay / Time
 * 
 * ============================================================================
 * 🔚 한 줄 요약
 * ============================================================================
 * 
 * C 언어 Wrapper 패턴은
 * "변할 수 있는 것을, 한 곳에 가두는 기술"이다.
 * 
 * ============================================================================
 * 이 예제 파일의 구조
 * ============================================================================
 * 1. 로깅 유틸리티
 * 2. Wrapper 함수들 - 파일 I/O (fopen, fclose)
 * 3. Wrapper 함수들 - 메모리 관리 (malloc, free)
 * 4. 사용 예제 (main)
 * 5. 실행 결과 확인
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * 로깅 유틸리티
 * ============================================================================ */

// 로그 레벨 정의
typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// 로그 출력 함수
void log_message(LogLevel level, const char* function_name, const char* message) {
    const char* level_str[] = {"INFO", "WARNING", "ERROR"};
    time_t now = time(NULL);
    char* time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';  // 개행 제거
    
    printf("[%s] [%s] %s: %s\n", time_str, level_str[level], function_name, message);
}

/* ============================================================================
 * 원본 함수들 (표준 라이브러리 함수들 - 이미 존재)
 * 여기서는 FILE*, malloc, free 등의 표준 함수들을 사용
 * ============================================================================ */

/* ============================================================================
 * Wrapper 함수들 - 로깅 기능이 추가된 래퍼
 * ============================================================================ */

// fopen의 wrapper - 파일 열기 작업에 로깅 추가
FILE* wrapped_fopen(const char* filename, const char* mode) {
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "파일 열기 시도: %s (모드: %s)", filename, mode);
    log_message(LOG_INFO, "wrapped_fopen", log_msg);
    
    FILE* file = fopen(filename, mode);
    
    if (file == NULL) {
        snprintf(log_msg, sizeof(log_msg), "파일 열기 실패: %s", filename);
        log_message(LOG_ERROR, "wrapped_fopen", log_msg);
    } else {
        snprintf(log_msg, sizeof(log_msg), "파일 열기 성공: %s", filename);
        log_message(LOG_INFO, "wrapped_fopen", log_msg);
    }
    
    return file;
}

// fclose의 wrapper - 파일 닫기 작업에 로깅 추가
int wrapped_fclose(FILE* file, const char* filename) {
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "파일 닫기 시도: %s", filename);
    log_message(LOG_INFO, "wrapped_fclose", log_msg);
    
    int result = fclose(file);
    
    if (result != 0) {
        snprintf(log_msg, sizeof(log_msg), "파일 닫기 실패: %s", filename);
        log_message(LOG_ERROR, "wrapped_fclose", log_msg);
    } else {
        snprintf(log_msg, sizeof(log_msg), "파일 닫기 성공: %s", filename);
        log_message(LOG_INFO, "wrapped_fclose", log_msg);
    }
    
    return result;
}

// malloc의 wrapper - 메모리 할당에 로깅 및 NULL 체크 추가
void* wrapped_malloc(size_t size, const char* caller) {
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "메모리 할당 요청: %zu bytes (호출자: %s)", size, caller);
    log_message(LOG_INFO, "wrapped_malloc", log_msg);
    
    void* ptr = malloc(size);
    
    if (ptr == NULL) {
        snprintf(log_msg, sizeof(log_msg), "메모리 할당 실패: %zu bytes", size);
        log_message(LOG_ERROR, "wrapped_malloc", log_msg);
    } else {
        snprintf(log_msg, sizeof(log_msg), "메모리 할당 성공: %zu bytes (주소: %p)", size, ptr);
        log_message(LOG_INFO, "wrapped_malloc", log_msg);
    }
    
    return ptr;
}

// free의 wrapper - 메모리 해제에 로깅 추가
void wrapped_free(void* ptr, const char* caller) {
    char log_msg[256];
    
    if (ptr == NULL) {
        snprintf(log_msg, sizeof(log_msg), "NULL 포인터 해제 시도 (호출자: %s)", caller);
        log_message(LOG_WARNING, "wrapped_free", log_msg);
        return;
    }
    
    snprintf(log_msg, sizeof(log_msg), "메모리 해제: 주소 %p (호출자: %s)", ptr, caller);
    log_message(LOG_INFO, "wrapped_free", log_msg);
    
    free(ptr);
}

/* ============================================================================
 * 사용 예제
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Wrapper 패턴 예제\n");
    printf("========================================\n\n");
    
    // 1. 파일 I/O wrapper 사용 예제
    printf("=== 파일 I/O Wrapper 예제 ===\n");
    const char* filename = "test_wrapper.txt";
    
    // 파일 쓰기
    FILE* file = wrapped_fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "Wrapper 패턴 테스트 데이터\n");
        fprintf(file, "로깅 기능이 추가된 파일 I/O\n");
        wrapped_fclose(file, filename);
    }
    printf("file: %p\n", file);

    printf("\n");
    
    // 파일 읽기
    file = wrapped_fopen(filename, "r");
    if (file != NULL) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("읽은 데이터: %s", buffer);
        }
        wrapped_fclose(file, filename);
    }
    
    printf("\n");
    
    // 존재하지 않는 파일 열기 시도 (에러 로깅 테스트)
    file = wrapped_fopen("nonexistent_file.txt", "r");
    
    printf("\n=== 메모리 관리 Wrapper 예제 ===\n");
    
    // 메모리 할당 및 해제
    int* numbers = (int*)wrapped_malloc(sizeof(int) * 10, "main");
    if (numbers != NULL) {
        // 데이터 사용
        for (int i = 0; i < 10; i++) {
            numbers[i] = i * 10;
        }
        printf("할당된 메모리 사용 완료\n");
        
        // 메모리 해제
        wrapped_free(numbers, "main");
    }
    
    printf("\n");
    
    // NULL 포인터 해제 시도 (경고 로깅 테스트)
    wrapped_free(NULL, "main");
    
    printf("\n========================================\n");
    printf("Wrapper 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Wrapper 패턴의 장점 총정리
 * ============================================================================
 * 
 * 1. 원본 함수를 수정하지 않고 기능 추가 가능
 * 2. 로깅, 디버깅, 모니터링 기능을 쉽게 추가
 * 3. 에러 처리를 중앙화할 수 있음
 * 4. 코드 재사용성 향상
 * 5. 하드웨어 변경에 강한 구조
 * 6. 테스트 및 Mock 작성 용이
 * 
 * ============================================================================
 * 임베디드 FW 개발에서 Wrapper 패턴의 실전 활용
 * ============================================================================
 * 
 * 1. HAL(Hardware Abstraction Layer) 래핑
 *    - STM32 HAL → Custom HAL
 *    - GPIO, UART, SPI, I2C 등 통일된 인터페이스 제공
 * 
 * 2. RTOS 함수 래핑
 *    - osDelay() → Delay_ms()
 *    - FreeRTOS → 다른 RTOS로 교체 시 상위 코드 변경 없음
 * 
 * 3. 통신 프로토콜 래핑
 *    - FOTA_Receive() 내부에서 UART/CAN/USB 교체 가능
 *    - 상위 로직은 통신 방식 변경에 영향 받지 않음
 * 
 * 4. Flash 메모리 래핑
 *    - Flash_Write() / Flash_Erase()
 *    - 내부 Flash ↔ 외부 Flash 교체 용이
 * 
 * 5. 센서/액추에이터 래핑
 *    - Motor_SetSpeed()
 *    - Sensor_ReadTemperature()
 *    - 하드웨어 교체 시 Wrapper만 수정
 * 
 * ============================================================================
 * 질문 / 기술 면담 대응 포인트
 * ============================================================================
 * 
 * Q: Wrapper 패턴을 사용한 경험이 있나요?
 * A: "네, FOTA 업데이트 기능 구현 시 통신 인터페이스를 Wrapper로 추상화했습니다.
 *     초기에는 UART를 사용했지만, Wrapper 덕분에 CAN 통신으로 변경할 때
 *     상위 FOTA 로직은 단 한 줄도 수정하지 않았습니다."
 * 
 * Q: Wrapper와 Adapter의 차이는?
 * A: "Wrapper는 기존 기능을 '숨기고 단순화'하는 것이 목적이고,
 *     Adapter는 '호환되지 않는 인터페이스를 연결'하는 것이 목적입니다.
 *     임베디드에서는 Wrapper를 더 많이 사용합니다."
 * 
 * Q: Wrapper 설계 시 주의할 점은?
 * A: "Wrapper는 얇게 유지해야 합니다. 로직이 비대해지면 Wrapper의 의미가
 *     사라지고 유지보수가 어려워집니다. 정책은 App 계층에 두고,
 *     Wrapper는 순수하게 인터페이스 역할만 해야 합니다."
 * 
 * ============================================================================
 * 다음 학습 단계 추천
 * ============================================================================
 * 
 * 1. 함수 포인터 기반 Wrapper (고급)
 * 2. FOTA 시스템 전체 Wrapper 설계
 * 3. STM32 HAL 추상화 계층 설계
 * 4. FreeRTOS + Wrapper 패턴 통합
 * 5. Unit Test를 위한 Mock Wrapper 작성
 * 
 * ============================================================================
 */
