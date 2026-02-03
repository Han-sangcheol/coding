/*
 * ============================================================================
 * 파일명: 04_callback_pattern.c
 * 기능: C언어 Callback(콜백) 패턴 구현 예제 - 인터럽트 기반 이벤트 처리
 * 
 * ============================================================================
 * 📌 학습 목표
 * ============================================================================
 * 
 * 임베디드 시스템에서 **인터럽트 발생 → ISR → 콜백 호출** 흐름 이해
 * 
 * ⚠️ 중요: 이 예제는 학습용 시뮬레이션입니다
 * - 실제 하드웨어 없이 콜백 개념을 학습하기 위한 코드
 * - 각 예제마다 "실제 임베디드에서는 어떻게 동작하는지" 주석으로 설명
 * - Sleep/printf로 시뮬레이션하지만, 실제로는 HW 인터럽트가 트리거
 * 
 * ============================================================================
 * 1️⃣ Callback 패턴이란?
 * ============================================================================
 * 
 * 함수 포인터를 사용하여 인터럽트 발생 시
 * 미리 등록된 함수를 자동으로 호출하는 패턴
 * 
 * 핵심 개념:
 *   - 함수 포인터 (Function Pointer)
 *   - 인터럽트 서비스 루틴 (ISR)
 *   - 이벤트 기반 설계 (Event-Driven)
 *   - 역호출 (Callback)
 * 
 * 실제 임베디드 동작 흐름:
 *   [HW 이벤트 발생] → [인터럽트 요청] → [ISR 진입] → [등록된 Callback 호출]
 * 
 * 예시:
 *   버튼 누름 → GPIO 인터럽트 → EXTI_IRQHandler → OnButtonPressed() 콜백
 * 
 * ============================================================================
 * 2️⃣ 왜 임베디드에서 Callback을 쓰나?
 * ============================================================================
 * 
 * 임베디드 시스템은 하드웨어 이벤트(인터럽트)를 효율적으로 처리해야 함
 * 
 * ✅ 인터럽트 + Callback 방식:
 *    ┌─────────────────────────────────────────────┐
 *    │ 1. HW 이벤트 발생 (버튼 누름, 타이머 만료) │
 *    │         ↓                                    │
 *    │ 2. 인터럽트 요청 (IRQ)                      │
 *    │         ↓                                    │
 *    │ 3. CPU가 ISR로 점프                         │
 *    │         ↓                                    │
 *    │ 4. ISR에서 등록된 콜백 호출                 │
 *    │         ↓                                    │
 *    │ 5. 사용자 정의 로직 실행                    │
 *    └─────────────────────────────────────────────┘
 * 
 *    장점:
 *    - CPU가 다른 작업 가능 (대기 안 함)
 *    - 실시간 반응 (이벤트 발생 즉시 처리)
 *    - 전력 효율 (Sleep 모드 가능)
 *    - 모듈 간 결합도 감소
 * 
 * ❌ Polling 방식 (Callback 없이):
 *    while(1) {
 *        if (button_pressed) { ... }  // 계속 확인
 *        if (timer_expired) { ... }   // CPU 낭비
 *    }
 * 
 *    단점:
 *    - CPU 100% 사용 (계속 체크)
 *    - 반응 느림 (루프 한 바퀴 돌아야 확인)
 *    - 전력 낭비 (Sleep 불가)
 *    - 이벤트 놓칠 수 있음
 * 
 * ============================================================================
 * 3️⃣ 임베디드에서 Callback이 사용되는 실제 사례 (인터럽트 중심)
 * ============================================================================
 * 
 * 케이스 1: GPIO 인터럽트 (버튼, 센서 신호)
 *   ┌──────────────────────────────────────────────────────┐
 *   │ 버튼 누름 → GPIO 핀 상태 변화                      │
 *   │           ↓                                          │
 *   │ EXTI 인터럽트 발생                                   │
 *   │           ↓                                          │
 *   │ EXTI15_10_IRQHandler() 진입                         │
 *   │           ↓                                          │
 *   │ HAL_GPIO_EXTI_IRQHandler() 호출                     │
 *   │           ↓                                          │
 *   │ HAL_GPIO_EXTI_Callback() 콜백 호출 ← 사용자 구현  │
 *   └──────────────────────────────────────────────────────┘
 * 
 * 케이스 2: 타이머 인터럽트 (주기적 작업)
 *   ┌──────────────────────────────────────────────────────┐
 *   │ 타이머 카운터 만료                                   │
 *   │           ↓                                          │
 *   │ TIM2 Update 인터럽트 발생                           │
 *   │           ↓                                          │
 *   │ TIM2_IRQHandler() 진입                              │
 *   │           ↓                                          │
 *   │ HAL_TIM_IRQHandler(&htim2) 호출                     │
 *   │           ↓                                          │
 *   │ HAL_TIM_PeriodElapsedCallback() 콜백 ← 사용자 구현 │
 *   └──────────────────────────────────────────────────────┘
 * 
 * 케이스 3: UART 인터럽트 (데이터 수신)
 *   ┌──────────────────────────────────────────────────────┐
 *   │ UART RX 버퍼에 데이터 수신                         │
 *   │           ↓                                          │
 *   │ USART1 인터럽트 발생                                │
 *   │           ↓                                          │
 *   │ USART1_IRQHandler() 진입                            │
 *   │           ↓                                          │
 *   │ HAL_UART_IRQHandler(&huart1) 호출                   │
 *   │           ↓                                          │
 *   │ HAL_UART_RxCpltCallback() 콜백 호출 ← 사용자 구현 │
 *   └──────────────────────────────────────────────────────┘
 * 
 * 👉 핵심: "하드웨어 이벤트 → 인터럽트 → ISR → 콜백" 흐름
 * 
 * ============================================================================
 * 4️⃣ ISR에서 Callback을 호출하는 구현 방법
 * ============================================================================
 * 
 * 방법 1: 단순 함수 포인터 (파라미터 없음)
 * 
 *    // 1. 콜백 타입 정의
 *    typedef void (*ButtonCallback)(void);
 *    
 *    // 2. 전역 또는 구조체에 콜백 저장
 *    ButtonCallback g_button_callback = NULL;
 *    
 *    // 3. 사용자가 콜백 등록
 *    void GPIO_RegisterCallback(ButtonCallback cb) {
 *        g_button_callback = cb;
 *    }
 *    
 *    // 4. ISR에서 콜백 호출
 *    void EXTI15_10_IRQHandler(void) {
 *        // 인터럽트 플래그 클리어
 *        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
 *        
 *        // 등록된 콜백 호출
 *        if (g_button_callback != NULL) {
 *            g_button_callback();  // ← 사용자 함수 실행
 *        }
 *    }
 * 
 * 방법 2: 데이터와 함께 전달 (파라미터 있음)
 * 
 *    // 1. 콜백 타입 정의 (데이터 포함)
 *    typedef void (*UartCallback)(uint8_t* data, uint16_t size);
 *    
 *    // 2. 콜백 저장
 *    UartCallback g_uart_rx_callback = NULL;
 *    
 *    // 3. ISR에서 데이터와 함께 콜백 호출
 *    void USART1_IRQHandler(void) {
 *        if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
 *            uint8_t data = USART1->DR;  // 수신 데이터 읽기
 *            
 *            if (g_uart_rx_callback != NULL) {
 *                g_uart_rx_callback(&data, 1);  // ← 데이터 전달
 *            }
 *        }
 *    }
 * 
 * ============================================================================
 * 5️⃣ Callback vs Polling 비교 (인터럽트 vs 폴링)
 * ============================================================================
 * 
 * 구분            | Polling (폴링)     | Callback (인터럽트)
 * ----------------|--------------------|-----------------------
 * 동작 방식       | 루프에서 계속 체크 | HW 이벤트 발생 시만
 * CPU 사용률      | 높음 (100%)        | 낮음 (이벤트 시만)
 * 반응 속도       | 느림 (루프 주기)   | 빠름 (즉시)
 * 전력 소모       | 높음 (Sleep 불가)  | 낮음 (Sleep 가능)
 * 실시간성        | 낮음               | 높음
 * 구현 복잡도     | 단순               | 중간 (ISR 처리)
 * 이벤트 놓칠확률 | 있음               | 없음
 * 
 * 예시 코드 비교:
 * 
 *   // Polling 방식
 *   while(1) {
 *       if (GPIO_ReadPin(BUTTON_PIN) == LOW) {
 *           // 버튼 처리 (CPU 계속 확인)
 *       }
 *   }
 * 
 *   // Callback 방식 (인터럽트)
 *   void EXTI_IRQHandler(void) {
 *       button_callback();  // 버튼 눌림 시만 실행
 *   }
 * 
 * ============================================================================
 * 6️⃣ ISR에서 Callback 구현 시 주의사항 (매우 중요!)
 * ============================================================================
 * 
 * ❌ ISR/Callback에서 절대 하면 안 되는 것:
 *    1. 긴 작업 수행 (10us 이상)
 *       - 다른 인터럽트 블로킹
 *       - 시스템 응답 지연
 *    
 *    2. 블로킹 함수 호출
 *       - printf() ❌ (UART 송신 대기)
 *       - delay() ❌ (시간 낭비)
 *       - malloc() ❌ (힙 락 대기)
 *    
 *    3. 복잡한 연산
 *       - 부동소수점 연산 (FPU context 저장 오버헤드)
 *       - 복잡한 알고리즘
 * 
 * ✅ ISR/Callback에서 해야 할 것:
 *    1. 플래그만 설정
 *       volatile uint8_t data_ready = 1;
 *    
 *    2. 데이터만 버퍼에 저장
 *       rx_buffer[rx_idx++] = UART->DR;
 *    
 *    3. RTOS 태스크 통지
 *       xTaskNotifyFromISR(task_handle, ...);
 *    
 *    4. NULL 체크 필수
 *       if (callback != NULL) callback();
 *    
 *    5. 인터럽트 플래그 클리어
 *       __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
 * 
 * 권장 패턴:
 *   void ISR_Handler(void) {
 *       // 1. 플래그 클리어
 *       // 2. 데이터 저장 or 플래그 설정
 *       // 3. 태스크 통지 (RTOS)
 *       // 끝! (빠르게 종료)
 *   }
 *   
 *   // 실제 처리는 메인 루프나 태스크에서
 *   void main_loop(void) {
 *       if (data_ready) {
 *           process_data();  // 긴 작업
 *           data_ready = 0;
 *       }
 *   }
 * 
 * ============================================================================
 * 7️⃣ Context(사용자 데이터)를 포함한 Callback (중요)
 * ============================================================================
 * 
 * 문제: 전역 변수 없이 ISR에서 특정 객체의 상태에 접근하려면?
 * 해결: void* user_data로 context 전달
 * 
 * 예시: 여러 타이머가 각자 다른 카운터를 증가시켜야 할 때
 * 
 *    // 1. Context를 받는 콜백 타입 정의
 *    typedef void (*TimerCallback)(void* context);
 *    
 *    // 2. 핸들러 구조체에 context 저장
 *    typedef struct {
 *        TimerCallback callback;
 *        void* context;       // 사용자 데이터
 *    } TimerHandler;
 *    
 *    // 3. 콜백 등록 시 context 함께 등록
 *    void Timer_RegisterCallback(TimerHandler* handler, 
 *                               TimerCallback cb, 
 *                               void* ctx) {
 *        handler->callback = cb;
 *        handler->context = ctx;
 *    }
 *    
 *    // 4. ISR에서 context와 함께 콜백 호출
 *    void TIM2_IRQHandler(void) {
 *        if (handler->callback != NULL) {
 *            handler->callback(handler->context);  // context 전달
 *        }
 *    }
 *    
 *    // 5. 사용자 콜백에서 context 사용
 *    void OnTimerTick(void* context) {
 *        int* counter = (int*)context;  // context를 카운터로 사용
 *        (*counter)++;
 *    }
 * 
 * 장점:
 *   - 전역 변수 불필요
 *   - 여러 인스턴스 각자 다른 context 사용 가능
 *   - 재사용성 증가
 * 
 * ============================================================================
 * 🔚 한 줄 요약
 * ============================================================================
 * 
 * Callback 패턴은
 * "하드웨어 인터럽트 발생 시 ISR에서 미리 등록된 함수를 호출하는
 *  이벤트 기반 프로그래밍 기법"이다.
 * 
 * 핵심: 인터럽트 → ISR → 콜백 호출
 * 
 * ============================================================================
 * 📁 이 예제 파일의 구조
 * ============================================================================
 * 
 * 각 예제는 "실제 임베디드 동작"을 시뮬레이션합니다.
 * 
 * 1. 버튼 이벤트 Callback
 *    실제: GPIO 인터럽트 → EXTI_IRQHandler → 콜백
 *    여기: Button_SimulateEvent() → 콜백
 * 
 * 2. 타이머 Callback
 *    실제: 타이머 만료 인터럽트 → TIM_IRQHandler → 콜백
 *    여기: Sleep() 루프 → 콜백 (스레드로 주기적 호출 시뮬레이션)
 * 
 * 3. UART 수신 Callback
 *    실제: UART RX 인터럽트 → USART_IRQHandler → 콜백
 *    여기: Uart_SimulateReceive() → 콜백
 * 
 * 4. 센서 Callback (Context 사용)
 *    실제: ADC 완료 인터럽트 → ADC_IRQHandler → 콜백(context)
 *    여기: Sensor_ReadData() → 콜백(context)
 * 
 * 5. 다중 Callback
 *    실제: 하나의 인터럽트에 여러 콜백 등록 (Observer 패턴)
 *    여기: 여러 콜백 순차 호출
 * 
 * ⚠️ 모든 예제는 학습용 시뮬레이션 (실제 HW 없이 개념 학습)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <windows.h>  // Sleep 함수 사용

/* ============================================================================
 * 전역 변수 - 프로그램 시작 시간
 * ============================================================================ */
static ULONGLONG program_start_time = 0;  // GetTickCount64() 사용

/* ============================================================================
 * 1. 버튼 이벤트 Callback - GPIO 인터럽트 시뮬레이션
 * ============================================================================
 * 
 * 실제 임베디드에서의 동작:
 * 
 *   1. GPIO 초기화 및 인터럽트 설정
 *      - GPIO 핀을 입력 모드로 설정
 *      - EXTI(External Interrupt) 라인에 연결
 *      - 트리거 모드 설정 (Rising/Falling Edge)
 *      - NVIC에서 인터럽트 활성화
 * 
 *   2. 버튼 누름 → 물리적 신호 변화
 *      - GPIO 핀 전압 변화 (HIGH → LOW 또는 반대)
 *      - EXTI 하드웨어가 엣지 감지
 * 
 *   3. 인터럽트 발생
 *      - EXTI가 NVIC에 인터럽트 요청
 *      - CPU가 현재 작업 중단
 *      - EXTI15_10_IRQHandler() 진입
 * 
 *   4. ISR에서 콜백 호출
 *      void EXTI15_10_IRQHandler(void) {
 *          if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13)) {
 *              __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);  // 플래그 클리어
 *              
 *              if (button_callback != NULL) {
 *                  button_callback(BUTTON_PRESSED);    // ← 콜백 호출
 *              }
 *          }
 *      }
 * 
 * 이 예제에서:
 *   - Button_SimulateEvent()로 인터럽트 발생을 시뮬레이션
 *   - 실제 GPIO나 EXTI 없이 콜백 호출 흐름만 재현
 * ============================================================================ */

// 버튼 이벤트 타입
typedef enum ButtonEvent{
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    BUTTON_LONG_PRESS
} ButtonEvent;

// 버튼 이벤트 콜백 함수 타입
typedef void (*ButtonCallback)(ButtonEvent event);

// 버튼 핸들러 구조체
typedef struct ButtonHandler{
    int pin_number;
    ButtonCallback callback;
    int is_pressed;
} ButtonHandler;

// 버튼 핸들러 초기화
// 실제: GPIO_Init() + HAL_GPIO_EXTI_RegisterCallback()
void Button_Init(ButtonHandler* handler, int pin, ButtonCallback cb) {
    handler->pin_number = pin;
    handler->callback = cb;        // 콜백 등록 (ISR에서 호출될 함수)
    handler->is_pressed = 0;
    printf("[Button] 버튼 핸들러 초기화 (Pin: %d)\n", pin);
}

// 버튼 이벤트 시뮬레이션
// 실제: 버튼 누름 → EXTI 인터럽트 → EXTI_IRQHandler → 이 콜백 호출
void Button_SimulateEvent(ButtonHandler* handler, ButtonEvent event) {
    printf("[Button] Pin %d 이벤트 발생: ", handler->pin_number);
    
    switch (event) {
        case BUTTON_PRESSED:
            printf("눌림\n");
            handler->is_pressed = 1;
            break;
        case BUTTON_RELEASED:
            printf("릴리즈\n");
            handler->is_pressed = 0;
            break;
        case BUTTON_LONG_PRESS:
            printf("길게 누름\n");
            break;
    }
    
    // ⚠️ 실제로는 ISR(EXTI_IRQHandler) 안에서 이 부분이 실행됨
    if (handler->callback != NULL) {
        handler->callback(event);  // ← ISR에서 사용자 콜백 호출
    }
}
}

/* ============================================================================
 * 2. 타이머 Callback - 주기적 작업 (학습용 시뮬레이션)
 * ============================================================================
 * 
 * ⚠️ 주의: 이것은 실제 하드웨어 타이머가 아닙니다!
 * 
 * 실제 임베디드에서는:
 *   - MCU의 HW 타이머 레지스터 설정 (TIMx->CR1, TIMx->PSC 등)
 *   - 타이머 인터럽트 활성화 (NVIC_EnableIRQ)
 *   - ISR에서 콜백 호출 (TIM2_IRQHandler → callback())
 * 
 * 이 예제에서는:
 *   - Sleep()으로 주기적 대기를 시뮬레이션
 *   - 스레드에서 interval_ms마다 콜백 호출
 *   - 개념 이해용 (실제 타이머 동작 방식 X)
 * 
 * 목적: 타이머 콜백 패턴의 개념과 사용법 학습
 * ============================================================================ */

// 타이머 콜백 함수 타입
typedef void (*TimerCallback)(void* user_data);

// 타이머 구조체
typedef struct {
    int timer_id;
    int interval_ms;
    TimerCallback callback;
    void* user_data;
    int tick_count;
    HANDLE thread_handle;   // 타이머 스레드 핸들
    volatile int is_running; // 타이머 실행 상태
} Timer;

// 타이머 스레드 함수 (주기적으로 콜백 호출)
// ⚠️ 실제 타이머가 아닌 Sleep() 기반 시뮬레이션
DWORD WINAPI Timer_ThreadFunc(LPVOID param) {
    Timer* timer = (Timer*)param;
    
    while (timer->is_running) {
        // ⚠️ 주의: Sleep()은 정확한 타이머가 아닙니다
        // 실제로는 HW 타이머 인터럽트가 발생하는 시점에 콜백이 호출됨
        // 여기서는 개념 이해를 위해 Sleep()으로 주기적 호출을 시뮬레이션
        Sleep(timer->interval_ms);
        
        if (!timer->is_running) break;
        
        // 틱 카운터 증가
        timer->tick_count++;
        
        /* 타이머 틱 시간 출력 (밀리초 단위) */
        ULONGLONG current_time = GetTickCount64();
        ULONGLONG elapsed_ms = current_time - program_start_time;
        printf("[Timer] Tick %d (프로그램 시작부터: %llu ms)\n", 
               timer->tick_count, elapsed_ms);
        
        // Callback 호출 (실제로는 ISR에서 호출됨)
        if (timer->callback != NULL) {
            timer->callback(timer->user_data);
        }
    }
    
    return 0;
}

// 타이머 초기화
void Timer_Init(Timer* timer, int id, int interval, 
                TimerCallback cb, void* data) {
    timer->timer_id = id;
    timer->interval_ms = interval;
    timer->callback = cb;
    timer->user_data = data;
    timer->tick_count = 0;
    timer->thread_handle = NULL;
    timer->is_running = 0;
    printf("[Timer] 타이머 %d 초기화 (주기: %dms)\n", id, interval);
}

// 타이머 시작 (별도 스레드에서 주기적으로 콜백 호출)
// ⚠️ 실제 타이머: HAL_TIM_Base_Start_IT(&htim2) 같은 함수로 HW 타이머 시작
// 이 예제: Sleep() 루프를 실행하는 스레드 생성 (시뮬레이션)
void Timer_Start(Timer* timer) {
    if (timer->is_running) {
        printf("[Timer] 타이머 %d 이미 실행 중\n", timer->timer_id);
        return;
    }
    
    timer->is_running = 1;
    
    // Windows 스레드 생성 (Sleep 루프 실행용)
    // 실제 임베디드에서는 HW 타이머 레지스터 설정
    timer->thread_handle = CreateThread(
        NULL,                   // 기본 보안 속성
        0,                      // 기본 스택 크기
        Timer_ThreadFunc,       // 스레드 함수 (Sleep 루프)
        timer,                  // 스레드 파라미터
        0,                      // 즉시 실행
        NULL                    // 스레드 ID 불필요
    );
    
    if (timer->thread_handle == NULL) {
        printf("[Timer] 타이머 %d 시작 실패\n", timer->timer_id);
        timer->is_running = 0;
    } else {
        printf("[Timer] 타이머 %d 시작\n", timer->timer_id);
    }
}

// 타이머 중지
// ⚠️ 실제 타이머: HAL_TIM_Base_Stop_IT(&htim2) 같은 함수로 HW 타이머 중지
// 이 예제: 스레드 종료 신호를 보내고 종료 대기 (시뮬레이션)
void Timer_Stop(Timer* timer) {
    if (!timer->is_running) {
        return;
    }
    
    printf("[Timer] 타이머 %d 중지 요청\n", timer->timer_id);
    
    // is_running을 0으로 설정하여 스레드에게 종료 신호
    // 실제로는 타이머 레지스터의 Enable 비트를 0으로 설정
    timer->is_running = 0;
    
    // 스레드 종료 대기 (실제로는 필요 없음, 시뮬레이션용)
    if (timer->thread_handle != NULL) {
        WaitForSingleObject(timer->thread_handle, INFINITE);
        CloseHandle(timer->thread_handle);
        timer->thread_handle = NULL;
    }
    
    printf("[Timer] 타이머 %d 중지됨 (총 %d 틱)\n", 
           timer->timer_id, timer->tick_count);
}

/* ============================================================================
 * 3. UART 수신 Callback - 데이터 수신 처리
 * ============================================================================
 * 
 * 실제 임베디드에서의 동작:
 * 
 *   1. UART 초기화 및 인터럽트 설정
 *      - UART 레지스터 설정 (보드레이트, 데이터 비트, 패리티 등)
 *      - UART RX 인터럽트 활성화
 *      - NVIC에서 USART 인터럽트 활성화
 * 
 *   2. 데이터 수신
 *      - UART RX 핀으로 데이터 수신
 *      - UART 하드웨어가 RX 버퍼에 데이터 저장
 *      - RXNE(Rx Not Empty) 플래그 자동 설정
 * 
 *   3. 인터럽트 발생
 *      - RXNE 플래그로 인터럽트 요청
 *      - USART1_IRQHandler() 진입
 * 
 *   4. ISR에서 콜백 호출
 *      void USART1_IRQHandler(void) {
 *          if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
 *              uint8_t data = USART1->DR;  // 수신 데이터 읽기
 *              
 *              if (uart_callback != NULL) {
 *                  uart_callback(&data, 1);  // ← 콜백 호출
 *              }
 *          }
 *      }
 * 
 * 이 예제에서:
 *   - Uart_SimulateReceive()로 데이터 수신을 시뮬레이션
 *   - 실제 UART 하드웨어 없이 콜백 호출 흐름만 재현
 * ============================================================================ */

// UART 데이터 수신 콜백 타입
typedef void (*UartRxCallback)(uint8_t* data, size_t len);

// UART 핸들러 구조체
typedef struct {
    int port;
    UartRxCallback callback;  // ISR에서 호출될 콜백
    uint8_t rx_buffer[256];   // 수신 버퍼 (ISR에서 채움)
    size_t rx_count;          // 수신 데이터 길이
} UartHandler;

// UART 초기화
// 실제: HAL_UART_Init() + HAL_UART_Receive_IT() + 콜백 등록
void Uart_Init(UartHandler* handler, int port, UartRxCallback cb) {
    handler->port = port;
    handler->callback = cb;   // 콜백 등록 (ISR에서 호출될 함수)
    handler->rx_count = 0;
    memset(handler->rx_buffer, 0, sizeof(handler->rx_buffer));
    printf("[UART] UART%d 초기화\n", port);
}

// UART 데이터 수신 시뮬레이션
// 실제: UART RX 핀으로 데이터 수신 → USART_IRQHandler → 콜백 호출
void Uart_SimulateReceive(UartHandler* handler, const char* data) {
    size_t len = strlen(data);
    
    printf("[UART] UART%d 데이터 수신: \"%s\" (%zu bytes)\n", 
           handler->port, data, len);
    
    // 버퍼에 저장 (실제로는 UART 하드웨어가 RX 버퍼에 자동 저장)
    memcpy(handler->rx_buffer, data, len);
    handler->rx_count = len;
    
    // ⚠️ 실제로는 ISR(USART_IRQHandler) 안에서 이 부분이 실행됨
    if (handler->callback != NULL) {
        handler->callback(handler->rx_buffer, handler->rx_count);  // ← ISR에서 콜백 호출
    }
}

/* ============================================================================
 * 4. Context를 사용한 센서 Callback
 * ============================================================================
 * 
 * 실제 임베디드에서의 동작:
 * 
 *   1. ADC/I2C 초기화 및 인터럽트 설정
 *      - ADC 변환 완료 인터럽트 또는 I2C 수신 완료 인터럽트 활성화
 * 
 *   2. 센서 데이터 읽기 시작
 *      - ADC_Start() 또는 I2C_MemRead_IT() 호출
 * 
 *   3. 변환/수신 완료 시 인터럽트 발생
 *      - ADC_IRQHandler() 또는 I2C_IRQHandler() 진입
 * 
 *   4. ISR에서 Context와 함께 콜백 호출
 *      void ADC_IRQHandler(void) {
 *          SensorData data = { .value = ADC1->DR };
 *          
 *          // Context(예: 센서 위치 정보)와 함께 콜백 호출
 *          if (sensor_callback != NULL) {
 *              sensor_callback(sensor_context, &data);
 *          }
 *      }
 * 
 * 이 예제에서:
 *   - Sensor_ReadData()로 센서 읽기를 시뮬레이션
 *   - Context(위치 정보 등)와 함께 콜백 호출
 * ============================================================================ */

// 센서 데이터 구조체
typedef struct {
    float temperature;
    float humidity;
    int sensor_id;
} SensorData;

// 센서 이벤트 콜백 타입 (Context 포함)
typedef void (*SensorCallback)(void* context, const SensorData* data);

// 센서 핸들러 구조체
typedef struct {
    int sensor_id;
    SensorCallback callback;
    void* context;
    SensorData last_data;
} SensorHandler;

// 센서 초기화
void Sensor_Init(SensorHandler* handler, int id, 
                 SensorCallback cb, void* ctx) {
    handler->sensor_id = id;
    handler->callback = cb;
    handler->context = ctx;
    handler->last_data.temperature = 0.0f;
    handler->last_data.humidity = 0.0f;
    handler->last_data.sensor_id = id;
    printf("[Sensor] 센서 %d 초기화\n", id);
}

// 센서 데이터 읽기 (시뮬레이션)
void Sensor_ReadData(SensorHandler* handler) {
    // 시뮬레이션: 랜덤 데이터 생성
    handler->last_data.temperature = 20.0f + (rand() % 150) / 10.0f;
    handler->last_data.humidity = 30.0f + (rand() % 500) / 10.0f;
    handler->last_data.sensor_id = handler->sensor_id;
    
    printf("[Sensor] 센서 %d 데이터: 온도=%.1f°C, 습도=%.1f%%\n",
           handler->sensor_id,
           handler->last_data.temperature,
           handler->last_data.humidity);
    
    // Callback 호출 (Context와 함께)
    if (handler->callback != NULL) {
        handler->callback(handler->context, &handler->last_data);
    }
}

/* ============================================================================
 * 사용자 정의 Callback 함수들
 * ============================================================================ */

// 버튼 이벤트 처리 콜백
void OnButtonEvent(ButtonEvent event) {
    printf("  → [Callback] 버튼 이벤트 처리: ");
    switch (event) {
        case BUTTON_PRESSED:
            printf("LED ON\n");
            break;
        case BUTTON_RELEASED:
            printf("LED OFF\n");
            break;
        case BUTTON_LONG_PRESS:
            printf("설정 모드 진입\n");
            break;
    }
}

// 타이머 만료 콜백
void OnTimerExpired(void* user_data) {
    int* counter = (int*)user_data;
    (*counter)++;
    printf("  → [Callback] 타이머 만료 (카운터: %d)\n", *counter);
}

// UART 데이터 수신 콜백
void OnUartDataReceived(uint8_t* data, size_t len) {
    printf("  → [Callback] UART 데이터 처리: ");
    for (size_t i = 0; i < len; i++) {
        printf("%c", data[i]);
    }
    printf("\n");
}

// 센서 데이터 수신 콜백 (Context 포함)
void OnSensorDataReceived(void* context, const SensorData* data) {
    const char* location = (const char*)context;
    printf("  → [Callback] 센서 데이터 처리 (위치: %s)\n", location);
    printf("     센서 ID: %d, 온도: %.1f°C, 습도: %.1f%%\n",
           data->sensor_id, data->temperature, data->humidity);
}

/* ============================================================================
 * 다중 Callback 관리 (Observer 패턴과 유사)
 * ============================================================================ */

#define MAX_CALLBACKS 5

typedef struct {
    ButtonCallback callbacks[MAX_CALLBACKS];
    int count;
} MultiButtonHandler;

void MultiButton_Init(MultiButtonHandler* handler) {
    handler->count = 0;
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        handler->callbacks[i] = NULL;
    }
    printf("[MultiButton] 다중 콜백 핸들러 초기화\n");
}

int MultiButton_AddCallback(MultiButtonHandler* handler, ButtonCallback cb) {
    if (handler->count >= MAX_CALLBACKS) {
        printf("[MultiButton] 콜백 등록 실패: 최대 개수 초과\n");
        return -1;
    }
    
    handler->callbacks[handler->count++] = cb;
    printf("[MultiButton] 콜백 등록 완료 (총 %d개)\n", handler->count);
    return 0;
}

void MultiButton_TriggerEvent(MultiButtonHandler* handler, ButtonEvent event) {
    printf("[MultiButton] 이벤트 발생, %d개 콜백 호출\n", handler->count);
    
    for (int i = 0; i < handler->count; i++) {
        if (handler->callbacks[i] != NULL) {
            handler->callbacks[i](event);
        }
    }
}

// 추가 콜백 함수들
void OnButton_LogEvent(ButtonEvent event) {
    printf("  → [Log Callback] 이벤트 로그 기록: %d\n", event);
}

void OnButton_UpdateDisplay(ButtonEvent event) {
    printf("  → [Display Callback] 화면 업데이트\n");
}

void OnButton_SendNotification(ButtonEvent event) {
    printf("  → [Notification Callback] 알림 전송\n");
}

/* ============================================================================
 * 메인 함수 - 각 예제 실행
 * ============================================================================
 * 
 * 이 main 함수는 학습용 시뮬레이션입니다.
 * 
 * 실제 임베디드 시스템에서는:
 *   - main()에서 초기화 + 무한 루프
 *   - 인터럽트 발생 시 자동으로 ISR 진입
 *   - ISR에서 콜백 호출 (비동기)
 * 
 * 이 예제에서는:
 *   - Simulate 함수로 인터럽트 발생을 흉내냄
 *   - 동기적으로 콜백 호출 (개념 학습용)
 * ============================================================================ */

int main(void) {
    // 프로그램 시작 시간 기록 (밀리초 단위)
    program_start_time = GetTickCount64();
    
    srand((unsigned int)time(NULL));
    
    printf("========================================\n");
    printf("Callback 패턴 예제 (인터럽트 기반)\n");
    printf("========================================\n");
    printf("⚠️ 학습용 시뮬레이션\n");
    printf("실제: HW 이벤트 → 인터럽트 → ISR → 콜백\n");
    printf("여기: Simulate 함수 → 콜백 (개념 학습)\n");
    printf("========================================\n\n");
    
    // 1. 버튼 이벤트 Callback (GPIO 인터럽트 시뮬레이션)
    printf("=== 1. GPIO 인터럽트 Callback ===\n");
    printf("실제: 버튼 누름 → EXTI 인터럽트 → ISR → 콜백\n\n");
    ButtonHandler button;
    Button_Init(&button, 5, OnButtonEvent);
    
    Button_SimulateEvent(&button, BUTTON_PRESSED);
    Button_SimulateEvent(&button, BUTTON_RELEASED);
    Button_SimulateEvent(&button, BUTTON_LONG_PRESS);
    printf("\n");
    
    // 2. 타이머 Callback (주기적 인터럽트 시뮬레이션)
    printf("=== 2. 타이머 인터럽트 Callback ===\n");
    printf("실제: 타이머 만료 → TIM 인터럽트 → ISR → 콜백\n");
    printf("여기: Sleep 루프 → 콜백 (시뮬레이션)\n\n");
    
    int timer_counter = 0;
    Timer timer;
    uint32_t interval_ms = 100;
    Timer_Init(&timer, 1, interval_ms, OnTimerExpired, &timer_counter);
    
    // 타이머 시작 (Sleep 루프를 실행하는 스레드 생성)
    // 실제로는 HW 타이머 레지스터 설정
    Timer_Start(&timer);
    
    // 메인 스레드는 대기 (타이머 스레드가 interval_ms마다 콜백 호출)
    // 실제로는 메인 루프에서 다른 작업 수행, ISR에서 콜백 호출
    Sleep(interval_ms * (5+1));  // 5번 실행 + 1번 대기
    
    // 타이머 중지
    Timer_Stop(&timer);
    printf("\n");
    
    // 3. UART 수신 Callback (데이터 수신 인터럽트 시뮬레이션)
    printf("=== 3. UART 수신 인터럽트 Callback ===\n");
    printf("실제: 데이터 수신 → UART 인터럽트 → ISR → 콜백\n\n");
    
    UartHandler uart;
    Uart_Init(&uart, 1, OnUartDataReceived);
    
    Uart_SimulateReceive(&uart, "Hello");
    Uart_SimulateReceive(&uart, "World");
    Uart_SimulateReceive(&uart, "Callback!");
    printf("\n");
    
    // 4. Context를 사용한 센서 Callback (사용자 데이터 전달)
    printf("=== 4. 센서 Callback (Context 전달) ===\n");
    printf("실제: ADC 완료 → ADC 인터럽트 → ISR → 콜백(context)\n");
    printf("목적: 전역 변수 없이 각 센서마다 다른 정보 전달\n\n");
    
    SensorHandler sensor1, sensor2;
    const char* location1 = "실내";
    const char* location2 = "실외";
    
    Sensor_Init(&sensor1, 101, OnSensorDataReceived, (void*)location1);
    Sensor_Init(&sensor2, 102, OnSensorDataReceived, (void*)location2);
    
    Sensor_ReadData(&sensor1);
    Sensor_ReadData(&sensor2);
    printf("\n");
    
    // 5. 다중 Callback (Observer 패턴)
    printf("=== 5. 다중 Callback (여러 핸들러 등록) ===\n");
    printf("실제: 하나의 인터럽트에 여러 콜백 등록\n");
    printf("예: GPIO 인터럽트 → LED 제어 + 로그 + 알림 + 화면 업데이트\n\n");
    
    MultiButtonHandler multi_button;
    MultiButton_Init(&multi_button);
    
    MultiButton_AddCallback(&multi_button, OnButtonEvent);
    MultiButton_AddCallback(&multi_button, OnButton_LogEvent);
    MultiButton_AddCallback(&multi_button, OnButton_UpdateDisplay);
    MultiButton_AddCallback(&multi_button, OnButton_SendNotification);
    
    printf("\n버튼 눌림 이벤트 발생:\n");
    MultiButton_TriggerEvent(&multi_button, BUTTON_PRESSED);
    
    printf("\n========================================\n");
    printf("Callback 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * 📌 Callback 패턴(인터럽트 기반)의 장점 총정리
 * ============================================================================
 * 
 * 1. ⚡ 즉시 반응 (실시간성)
 *    - 하드웨어 이벤트 발생 즉시 ISR 진입 → 콜백 실행
 *    - Polling 대비 반응 속도 월등
 * 
 * 2. 💻 CPU 효율성
 *    - 이벤트 없을 때 Sleep 모드 진입 가능
 *    - 폴링처럼 계속 체크 안 해도 됨 (CPU 사용률 ↓)
 * 
 * 3. 🔋 전력 효율
 *    - Sleep 모드 활용으로 전력 소비 최소화
 *    - 배터리 구동 IoT 기기에 필수
 * 
 * 4. 🧩 모듈화
 *    - ISR과 사용자 로직 분리
 *    - 하드웨어 드라이버와 애플리케이션 로직 독립적
 * 
 * 5. 🔄 유연한 확장
 *    - 런타임에 콜백 변경 가능
 *    - 여러 콜백 등록 가능 (Observer 패턴)
 * 
 * ============================================================================
 * ⚠️ ISR/Callback 사용 시 주의사항 (매우 중요!)
 * ============================================================================
 * 
 * 1. ❌ ISR은 최대한 짧게! (10us 이내 권장)
 *    - 긴 작업은 플래그만 설정 → 메인 루프에서 처리
 *    - 다른 인터럽트 블로킹 방지
 * 
 * 2. ❌ ISR에서 블로킹 함수 금지
 *    - printf() ❌ (UART 송신 대기)
 *    - malloc() ❌ (힙 락 대기)
 *    - delay() ❌ (시간 낭비)
 * 
 * 3. ✅ volatile 키워드 사용
 *    - ISR과 메인 루프가 공유하는 변수는 반드시 volatile
 *    - 컴파일러 최적화로 인한 버그 방지
 * 
 * 4. ✅ 재진입(Reentrant) 고려
 *    - 같은 함수가 ISR에서 호출될 수 있음
 *    - 전역 변수 사용 시 주의
 * 
 * 5. ✅ NULL 체크 필수
 *    - if (callback != NULL) callback();
 *    - 등록 안 된 콜백 호출 시 크래시
 * 
 * 6. ✅ 인터럽트 우선순위 설정
 *    - NVIC에서 우선순위 적절히 설정
 *    - 중요한 인터럽트가 블로킹되지 않도록
 * 
 * ============================================================================
 * 임베디드 FW 개발에서 Callback 패턴의 실전 활용
 * ============================================================================
 * 
 * 1. GPIO 인터럽트
 *    - 버튼, 스위치 이벤트 처리
 *    - 외부 신호 감지
 * 
 * 2. 통신 프로토콜
 *    - UART/SPI/I2C 데이터 수신
 *    - CAN 메시지 수신
 *    - 프로토콜 파싱 완료 통지
 * 
 * 3. 타이머/RTC
 *    - 주기적 작업 스케줄링
 *    - 알람, 타임아웃 처리
 * 
 * 4. RTOS 이벤트
 *    - 태스크 간 통신
 *    - 세마포어, 큐 이벤트
 * 
 * 5. 센서 드라이버
 *    - 데이터 수신 완료 통지
 *    - 임계값 초과 알림
 * 
 * 6. FOTA
 *    - 다운로드 진행률 콜백
 *    - 업데이트 완료 통지
 * 
 * ============================================================================
 * ISR에서 Callback 사용 시 주의사항
 * ============================================================================
 * 
 * STM32 HAL 예제:
 * 
 *    // ISR (Interrupt Service Routine)
 *    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
 *        // ❌ 나쁜 예: ISR에서 긴 작업
 *        // ProcessData();  // 오래 걸림!
 *        
 *        // ✅ 좋은 예: 플래그만 설정
 *        button_pressed_flag = 1;
 *    }
 *    
 *    // 메인 루프 또는 RTOS 태스크
 *    void MainLoop(void) {
 *        if (button_pressed_flag) {
 *            button_pressed_flag = 0;
 *            
 *            // 여기서 실제 처리
 *            if (button_callback != NULL) {
 *                button_callback(BUTTON_PRESSED);
 *            }
 *        }
 *    }
 * 
 * ============================================================================
 * FreeRTOS에서 Callback 패턴
 * ============================================================================
 * 
 *    // 타이머 콜백
 *    void vTimerCallback(TimerHandle_t xTimer) {
 *        // 타이머 컨텍스트 가져오기
 *        void* context = pvTimerGetTimerID(xTimer);
 *        
 *        // 사용자 콜백 호출
 *        TimerCallback cb = (TimerCallback)context;
 *        if (cb != NULL) {
 *            cb();
 *        }
 *    }
 *    
 *    // 사용
 *    TimerHandle_t timer = xTimerCreate("Timer", 
 *                                       pdMS_TO_TICKS(1000),
 *                                       pdTRUE,
 *                                       (void*)MyCallback,
 *                                       vTimerCallback);
 * 
 * ============================================================================
 * 질문 / 기술 면담 대응 포인트
 * ============================================================================
 * 
 * Q: Callback 패턴을 사용한 경험이 있나요?
 * A: "네, UART 통신에서 데이터 수신 완료 시 콜백으로 상위 프로토콜
 *     파싱 함수를 호출하도록 구현했습니다. 인터럽트에서는 플래그만
 *     설정하고, 메인 태스크에서 콜백을 호출하여 ISR 실행 시간을
 *     최소화했습니다."
 * 
 * Q: Callback과 Polling의 차이는?
 * A: "Polling은 주기적으로 상태를 확인하여 CPU를 계속 사용하지만,
 *     Callback은 이벤트 발생 시에만 실행되어 CPU 효율이 높습니다.
 *     임베디드 시스템에서는 전력 소모 측면에서도 Callback이 유리합니다."
 * 
 * Q: ISR에서 Callback 사용 시 주의할 점은?
 * A: "ISR에서는 실행 시간을 최소화해야 하므로, 콜백 내에서 긴 작업,
 *     메모리 할당, 블로킹 함수 호출을 피해야 합니다. 플래그만 설정하고
 *     실제 처리는 메인 루프나 RTOS 태스크에서 하는 것이 좋습니다."
 * 
 * Q: Context를 사용하는 이유는?
 * A: "Callback 함수가 상태를 유지할 수 없기 때문에, Context를 통해
 *     필요한 데이터를 전달합니다. 예를 들어 여러 센서가 같은 콜백을
 *     사용할 때 Context로 센서를 구분할 수 있습니다."
 * 
 * ============================================================================
 * 실전 체크리스트
 * ============================================================================
 * 
 * Callback 구현 전 확인:
 * 
 * □ 이벤트 발생 시점이 불규칙한가?
 * □ 폴링으로 구현 시 CPU 낭비가 큰가?
 * □ 여러 모듈이 동일 이벤트를 처리하는가?
 * □ ISR에서 호출될 가능성이 있는가?
 * □ Context 전달이 필요한가?
 * 
 * 👉 3개 이상 체크되면 Callback 패턴 적용 추천
 * 
 * ============================================================================
 * 다음 학습 단계 추천
 * ============================================================================
 * 
 * 1. 05_state_machine.c - Callback과 함께 사용하는 상태 머신
 * 2. 07_observer_pattern.c - 다중 Callback 관리
 * 3. 10_event_queue.c - Callback + 이벤트 큐
 * 4. FreeRTOS Queue + Callback 통합
 * 5. 인터럽트 기반 드라이버 설계
 * 
 * ============================================================================
 */
