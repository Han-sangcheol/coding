/*
 * ============================================================================
 * 파일명: 04_callback_pattern.c
 * 기능: C언어 Callback(콜백) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Callback 패턴이란?
 * ============================================================================
 * 
 * 함수 포인터를 사용하여 특정 이벤트 발생 시
 * 미리 등록된 함수를 호출하는 패턴
 * 
 * 핵심 키워드:
 *   - 함수 포인터 (Function Pointer)
 *   - 비동기 처리 (Asynchronous)
 *   - 이벤트 기반 (Event-Driven)
 *   - 역호출 (Callback)
 * 
 * 구조:
 *   [이벤트 발생] → [핸들러 호출] → [등록된 Callback 실행]
 * 
 * ============================================================================
 * 2️⃣ 왜 Callback을 쓰나?
 * ============================================================================
 * 
 * 임베디드 시스템은 이벤트 중심 설계가 많음:
 * 
 * ✅ Callback의 장점:
 *    - 인터럽트 처리 (ISR → 상위 로직)
 *    - 비동기 이벤트 처리
 *    - 모듈 간 결합도 감소
 *    - 유연한 확장성
 *    - RTOS 태스크 간 통신
 * 
 * ❌ Callback 없이 구현하면:
 *    - 폴링(Polling)으로 CPU 낭비
 *    - 모듈 간 강한 결합
 *    - 확장 어려움
 * 
 * ============================================================================
 * 3️⃣ 임베디드에서 Callback을 쓰는 경우
 * ============================================================================
 * 
 * 실무 적용 사례:
 *   ✅ 인터럽트 핸들러 (GPIO, UART, Timer)
 *   ✅ RTOS 태스크 통지
 *   ✅ 센서 데이터 수신 완료 통지
 *   ✅ FOTA 다운로드 진행률 콜백
 *   ✅ 통신 프로토콜 패킷 수신
 *   ✅ 타이머 만료 이벤트
 *   ✅ 버튼 누름/릴리즈 이벤트
 * 
 * 👉 "이벤트가 언제 발생할지 모를 때" Callback 필수
 * 
 * ============================================================================
 * 4️⃣ C 언어에서 Callback 구현 방법
 * ============================================================================
 * 
 * 방법 1: 단순 함수 포인터
 * 
 *    typedef void (*EventCallback)(void);
 *    EventCallback callback = NULL;
 *    
 *    void RegisterCallback(EventCallback cb) {
 *        callback = cb;
 *    }
 *    
 *    void TriggerEvent(void) {
 *        if (callback != NULL) {
 *            callback();
 *        }
 *    }
 * 
 * 방법 2: 데이터와 함께 전달
 * 
 *    typedef void (*DataCallback)(int data);
 *    
 *    void OnDataReceived(int value) {
 *        printf("Received: %d\n", value);
 *    }
 *    
 *    DataCallback cb = OnDataReceived;
 *    cb(100);  // "Received: 100"
 * 
 * ============================================================================
 * 5️⃣ Callback vs Polling 비교
 * ============================================================================
 * 
 * 구분            | Polling          | Callback
 * ----------------|------------------|------------------
 * CPU 사용률      | 높음 (계속 체크) | 낮음 (이벤트 시만)
 * 반응 속도       | 느림             | 빠름
 * 구현 복잡도     | 단순             | 중간
 * 전력 소모       | 높음             | 낮음
 * 실시간성        | 낮음             | 높음
 * 
 * ============================================================================
 * 6️⃣ Callback 구현 시 주의사항
 * ============================================================================
 * 
 * ❌ 피해야 할 것:
 *    - Callback 내에서 긴 작업 수행
 *    - ISR에서 printf 같은 블로킹 함수
 *    - Callback 내에서 메모리 할당
 *    - 재진입(Reentrant) 고려 안 함
 * 
 * ✅ 해야 할 것:
 *    - Callback은 최대한 짧게
 *    - ISR에서는 플래그만 설정
 *    - NULL 체크 필수
 *    - Context 전달 고려 (void* user_data)
 * 
 * ============================================================================
 * 7️⃣ Context를 포함한 Callback (중요)
 * ============================================================================
 * 
 * 단순 함수 포인터로는 상태 전달 어려움:
 * 
 *    typedef void (*Callback)(void* context, int data);
 *    
 *    typedef struct {
 *        Callback callback;
 *        void* context;
 *    } CallbackHandler;
 *    
 *    void RegisterCallback(CallbackHandler* handler, 
 *                         Callback cb, void* ctx) {
 *        handler->callback = cb;
 *        handler->context = ctx;
 *    }
 *    
 *    void InvokeCallback(CallbackHandler* handler, int data) {
 *        if (handler->callback != NULL) {
 *            handler->callback(handler->context, data);
 *        }
 *    }
 * 
 * ============================================================================
 * 🔚 한 줄 요약
 * ============================================================================
 * 
 * Callback 패턴은
 * "이벤트 발생 시 미리 등록된 함수를 호출하는 비동기 처리 기술"이다.
 * 
 * ============================================================================
 * 이 예제 파일의 구조
 * ============================================================================
 * 1. 버튼 이벤트 Callback (GPIO 인터럽트 시뮬레이션)
 * 2. 타이머 Callback (주기적 작업)
 * 3. UART 수신 Callback (데이터 수신 처리)
 * 4. Context를 사용한 고급 Callback
 * 5. 사용 예제 (main)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * 1. 버튼 이벤트 Callback - GPIO 인터럽트 시뮬레이션
 * ============================================================================ */

// 버튼 이벤트 타입
typedef enum {
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    BUTTON_LONG_PRESS
} ButtonEvent;

// 버튼 이벤트 콜백 함수 타입
typedef void (*ButtonCallback)(ButtonEvent event);

// 버튼 핸들러 구조체
typedef struct {
    int pin_number;
    ButtonCallback callback;
    int is_pressed;
} ButtonHandler;

// 버튼 핸들러 초기화
void Button_Init(ButtonHandler* handler, int pin, ButtonCallback cb) {
    handler->pin_number = pin;
    handler->callback = cb;
    handler->is_pressed = 0;
    printf("[Button] 버튼 핸들러 초기화 (Pin: %d)\n", pin);
}

// 버튼 이벤트 시뮬레이션
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
    
    // Callback 호출
    if (handler->callback != NULL) {
        handler->callback(event);
    }
}

/* ============================================================================
 * 2. 타이머 Callback - 주기적 작업
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
} Timer;

// 타이머 초기화
void Timer_Init(Timer* timer, int id, int interval, 
                TimerCallback cb, void* data) {
    timer->timer_id = id;
    timer->interval_ms = interval;
    timer->callback = cb;
    timer->user_data = data;
    timer->tick_count = 0;
    printf("[Timer] 타이머 %d 초기화 (주기: %dms)\n", id, interval);
}

// 타이머 틱 (주기적으로 호출됨)
void Timer_Tick(Timer* timer) {
    timer->tick_count++;
    
    // Callback 호출
    if (timer->callback != NULL) {
        timer->callback(timer->user_data);
    }
}

/* ============================================================================
 * 3. UART 수신 Callback - 데이터 수신 처리
 * ============================================================================ */

// UART 데이터 수신 콜백 타입
typedef void (*UartRxCallback)(uint8_t* data, size_t len);

// UART 핸들러 구조체
typedef struct {
    int port;
    UartRxCallback callback;
    uint8_t rx_buffer[256];
    size_t rx_count;
} UartHandler;

// UART 초기화
void Uart_Init(UartHandler* handler, int port, UartRxCallback cb) {
    handler->port = port;
    handler->callback = cb;
    handler->rx_count = 0;
    memset(handler->rx_buffer, 0, sizeof(handler->rx_buffer));
    printf("[UART] UART%d 초기화\n", port);
}

// UART 데이터 수신 시뮬레이션
void Uart_SimulateReceive(UartHandler* handler, const char* data) {
    size_t len = strlen(data);
    
    printf("[UART] UART%d 데이터 수신: \"%s\" (%zu bytes)\n", 
           handler->port, data, len);
    
    // 버퍼에 저장
    memcpy(handler->rx_buffer, data, len);
    handler->rx_count = len;
    
    // Callback 호출
    if (handler->callback != NULL) {
        handler->callback(handler->rx_buffer, handler->rx_count);
    }
}

/* ============================================================================
 * 4. Context를 사용한 고급 Callback
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
 * 메인 함수
 * ============================================================================ */

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("========================================\n");
    printf("Callback 패턴 예제\n");
    printf("========================================\n\n");
    
    // 1. 버튼 이벤트 Callback
    printf("=== 1. 버튼 이벤트 Callback ===\n");
    ButtonHandler button;
    Button_Init(&button, 5, OnButtonEvent);
    
    Button_SimulateEvent(&button, BUTTON_PRESSED);
    Button_SimulateEvent(&button, BUTTON_RELEASED);
    Button_SimulateEvent(&button, BUTTON_LONG_PRESS);
    printf("\n");
    
    // 2. 타이머 Callback
    printf("=== 2. 타이머 Callback ===\n");
    int timer_counter = 0;
    Timer timer;
    Timer_Init(&timer, 1, 100, OnTimerExpired, &timer_counter);
    
    for (int i = 0; i < 5; i++) {
        printf("[Timer] Tick %d\n", i + 1);
        Timer_Tick(&timer);
    }
    printf("\n");
    
    // 3. UART 수신 Callback
    printf("=== 3. UART 수신 Callback ===\n");
    UartHandler uart;
    Uart_Init(&uart, 1, OnUartDataReceived);
    
    Uart_SimulateReceive(&uart, "Hello");
    Uart_SimulateReceive(&uart, "World");
    Uart_SimulateReceive(&uart, "Callback!");
    printf("\n");
    
    // 4. Context를 사용한 센서 Callback
    printf("=== 4. Context를 사용한 센서 Callback ===\n");
    SensorHandler sensor1, sensor2;
    const char* location1 = "실내";
    const char* location2 = "실외";
    
    Sensor_Init(&sensor1, 101, OnSensorDataReceived, (void*)location1);
    Sensor_Init(&sensor2, 102, OnSensorDataReceived, (void*)location2);
    
    Sensor_ReadData(&sensor1);
    Sensor_ReadData(&sensor2);
    printf("\n");
    
    // 5. 다중 Callback
    printf("=== 5. 다중 Callback (여러 핸들러 등록) ===\n");
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
 * Callback 패턴의 장점 총정리
 * ============================================================================
 * 
 * 1. 비동기 이벤트 처리 가능
 * 2. 모듈 간 결합도 감소
 * 3. 유연한 확장성
 * 4. 인터럽트 기반 시스템에 최적
 * 5. CPU 자원 효율적 사용
 * 
 * ============================================================================
 * Callback 패턴의 단점 및 주의사항
 * ============================================================================
 * 
 * 1. Callback 체인이 길어지면 디버깅 어려움
 * 2. 콜백 내 긴 작업 시 전체 시스템 지연
 * 3. 재진입(Reentrant) 문제 고려 필요
 * 4. NULL 포인터 체크 필수
 * 5. Context 관리 복잡도 증가
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
 * 면접 / 기술 면담 대응 포인트
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
