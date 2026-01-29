/* HAL (Hardware Abstraction Layer) Pattern
 * 하드웨어 추상화 계층
 */

#include <stdio.h>
#include <stdint.h>

// HAL 인터페이스
typedef struct {
    void (*init)(void);
    void (*write)(uint8_t pin, int value);
    int (*read)(uint8_t pin);
} GPIO_HAL;

// STM32 구현
void STM32_GPIO_Init(void) {
    printf("[STM32] GPIO 초기화\n");
}

void STM32_GPIO_Write(uint8_t pin, int value) {
    printf("[STM32] GPIO Pin %d = %d\n", pin, value);
}

int STM32_GPIO_Read(uint8_t pin) {
    printf("[STM32] GPIO Pin %d 읽기\n", pin);
    return 1;
}

// Nordic 구현
void Nordic_GPIO_Init(void) {
    printf("[Nordic] GPIO 초기화\n");
}

void Nordic_GPIO_Write(uint8_t pin, int value) {
    printf("[Nordic] GPIO Pin %d = %d\n", pin, value);
}

int Nordic_GPIO_Read(uint8_t pin) {
    printf("[Nordic] GPIO Pin %d 읽기\n", pin);
    return 0;
}

// HAL 테이블
GPIO_HAL stm32_hal = {
    .init = STM32_GPIO_Init,
    .write = STM32_GPIO_Write,
    .read = STM32_GPIO_Read
};

GPIO_HAL nordic_hal = {
    .init = Nordic_GPIO_Init,
    .write = Nordic_GPIO_Write,
    .read = Nordic_GPIO_Read
};

// 애플리케이션 (HAL 독립적)
void App_ToggleLED(GPIO_HAL* hal, uint8_t pin) {
    int value = hal->read(pin);
    hal->write(pin, !value);
}

int main(void) {
    printf("=== HAL Pattern ===\n\n");
    
    GPIO_HAL* hal = &stm32_hal;  // STM32 사용
    hal->init();
    App_ToggleLED(hal, 5);
    
    printf("\n");
    
    hal = &nordic_hal;  // Nordic으로 교체
    hal->init();
    App_ToggleLED(hal, 5);
    
    printf("\nHAL: 하드웨어 추상화 패턴\n");
    return 0;
}
