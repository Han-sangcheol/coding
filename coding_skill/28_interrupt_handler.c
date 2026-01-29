/* Interrupt Handler Pattern
 * 인터럽트 최소화 및 분리
 */

#include <stdio.h>
#include <stdint.h>

// ISR 플래그
volatile int button_pressed = 0;
volatile uint8_t uart_rx_data = 0;

// ISR (Interrupt Service Routine) - 최소한의 작업만
void GPIO_ISR(void) {
    // ✅ 좋은 예: 플래그만 설정
    button_pressed = 1;
    printf("[ISR] 버튼 인터럽트 - 플래그 설정\n");
    
    // ❌ 나쁜 예: ISR에서 긴 작업 (주석 처리)
    // ProcessButtonEvent();  // 절대 하지 말 것!
}

void UART_ISR(void) {
    // ✅ 빠르게 데이터만 저장
    uart_rx_data = 0x42;  // 하드웨어에서 읽음
    printf("[ISR] UART 인터럽트 - 데이터 저장\n");
}

// 메인 루프에서 실제 처리
void MainLoop(void) {
    // 버튼 이벤트 처리
    if (button_pressed) {
        button_pressed = 0;
        printf("[MainLoop] 버튼 이벤트 처리\n");
        // 여기서 실제 긴 작업 수행
    }
    
    // UART 데이터 처리
    if (uart_rx_data != 0) {
        printf("[MainLoop] UART 데이터 처리: 0x%02X\n", uart_rx_data);
        uart_rx_data = 0;
    }
}

int main(void) {
    printf("=== Interrupt Handler Pattern ===\n\n");
    
    // ISR 시뮬레이션
    GPIO_ISR();
    UART_ISR();
    
    // 메인 루프에서 처리
    for (int i = 0; i < 2; i++) {
        MainLoop();
    }
    
    printf("\nInterrupt Handler: ISR 최소화 패턴\n");
    printf("원칙: ISR에서는 플래그만, 메인 루프에서 실제 처리\n");
    return 0;
}
