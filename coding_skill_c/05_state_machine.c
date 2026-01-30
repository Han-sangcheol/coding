/*
 * ============================================================================
 * 파일명: 05_state_machine.c
 * 기능: C언어 State Machine(상태 머신) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ State Machine 패턴이란?
 * ============================================================================
 * 
 * 시스템의 상태(State)와 상태 간 전이(Transition)를 명확하게
 * 정의하여 복잡한 동작을 구조화하는 패턴
 * 
 * 핵심 키워드:
 *   - 상태 (State)
 *   - 전이 (Transition)
 *   - 이벤트 (Event)
 *   - 액션 (Action)
 * 
 * 구조:
 *   [State A] --Event→ [State B] --Event→ [State C]
 * 
 * ============================================================================
 * 2️⃣ 왜 State Machine을 쓰나?
 * ============================================================================
 * 
 * 복잡한 시퀀스 제어를 깔끔하게:
 * 
 * ✅ State Machine의 장점:
 *    - 복잡한 로직을 명확하게 구조화
 *    - 상태 전이 조건 명시적
 *    - 디버깅 용이 (현재 상태 추적)
 *    - 테스트 용이 (상태별 테스트)
 *    - 문서화 쉬움 (상태 다이어그램)
 * 
 * ❌ State Machine 없이 구현하면:
 *    - if/else 지옥
 *    - 플래그 난무
 *    - 디버깅 지옥
 *    - 확장 어려움
 * 
 * ============================================================================
 * 3️⃣ 임베디드에서 State Machine을 쓰는 경우
 * ============================================================================
 * 
 * 실무 적용 사례:
 *   ✅ FOTA (펌웨어 업데이트): Idle → Download → Verify → Update → Complete
 *   ✅ 통신 프로토콜: Connect → Authenticate → Transfer → Disconnect
 *   ✅ 모터 제어: Stop → Start → Run → Brake → Stop
 *   ✅ 배터리 충전: Idle → Charging → Full → Maintain
 *   ✅ 센서 초기화: PowerOn → Config → Calibrate → Ready
 *   ✅ 버튼 디바운싱: Released → Pressed → Held → Released
 * 
 * 👉 "복잡한 시퀀스가 있는 모든 것"에 State Machine 필수
 * 
 * ============================================================================
 * 4️⃣ C 언어에서 State Machine 구현 방법
 * ============================================================================
 * 
 * 방법 1: enum + switch/case (가장 기본)
 * 
 *    typedef enum { IDLE, RUNNING, STOPPED } State;
 *    State current_state = IDLE;
 *    
 *    void ProcessEvent(Event evt) {
 *        switch (current_state) {
 *            case IDLE:
 *                if (evt == EVT_START) current_state = RUNNING;
 *                break;
 *            case RUNNING:
 *                if (evt == EVT_STOP) current_state = STOPPED;
 *                break;
 *        }
 *    }
 * 
 * 방법 2: 함수 포인터 테이블 (확장성 좋음)
 * 
 *    typedef State (*StateFunc)(Event evt);
 *    
 *    State StateIdle(Event evt);
 *    State StateRunning(Event evt);
 *    
 *    StateFunc state_table[] = {
 *        StateIdle,
 *        StateRunning
 *    };
 * 
 * ============================================================================
 * 5️⃣ State Machine 설계 원칙
 * ============================================================================
 * 
 * 원칙                | 설명
 * --------------------|----------------------------------
 * 단일 상태           | 한 번에 하나의 상태만
 * 명시적 전이         | 전이 조건 명확히
 * 진입/진출 액션      | 상태 변경 시 실행할 작업
 * 이벤트 기반         | 외부 이벤트로 전이
 * 
 * ============================================================================
 * 6️⃣ State Machine 구현 시 주의사항
 * ============================================================================
 * 
 * ❌ 피해야 할 것:
 *    - 상태 폭발 (너무 많은 상태)
 *    - 순환 의존성
 *    - 전역 변수 남용
 *    - 재진입 고려 안 함
 * 
 * ✅ 해야 할 것:
 *    - 상태는 최소화
 *    - 전이 조건은 명확히
 *    - 진입/진출 액션 활용
 *    - 로깅으로 상태 추적
 * 
 * ============================================================================
 * 🔚 한 줄 요약
 * ============================================================================
 * 
 * State Machine 패턴은
 * "복잡한 시퀀스를 상태와 전이로 명확하게 구조화하는 기술"이다.
 * 
 * ============================================================================
 * 이 예제 파일의 구조
 * ============================================================================
 * 1. 기본 State Machine - 버튼 디바운싱
 * 2. FOTA State Machine - 펌웨어 업데이트
 * 3. 통신 프로토콜 State Machine
 * 4. 계층형 State Machine (HSM)
 * 5. 사용 예제 (main)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

/* ============================================================================
 * 1. 기본 State Machine - 버튼 디바운싱
 * ============================================================================ */

// 버튼 상태
typedef enum {
    BTN_RELEASED,
    BTN_PRESSED,
    BTN_HELD
} ButtonState;

// 버튼 이벤트
typedef enum {
    BTN_EVT_PRESS,
    BTN_EVT_RELEASE,
    BTN_EVT_TIMEOUT
} ButtonEvent;

// 버튼 상태 머신
typedef struct {
    ButtonState current_state;
    int hold_counter;
    int hold_threshold;
} ButtonStateMachine;

// 버튼 상태 머신 초기화
void ButtonSM_Init(ButtonStateMachine* sm, int hold_threshold) {
    sm->current_state = BTN_RELEASED;
    sm->hold_counter = 0;
    sm->hold_threshold = hold_threshold;
    printf("[ButtonSM] 초기화: 상태=%d\n", sm->current_state);
}

// 버튼 이벤트 처리
void ButtonSM_ProcessEvent(ButtonStateMachine* sm, ButtonEvent event) {
    ButtonState old_state = sm->current_state;
    
    switch (sm->current_state) {
        case BTN_RELEASED:
            if (event == BTN_EVT_PRESS) {
                sm->current_state = BTN_PRESSED;
                sm->hold_counter = 0;
                printf("[ButtonSM] 액션: 버튼 눌림 감지\n");
            }
            break;
            
        case BTN_PRESSED:
            if (event == BTN_EVT_RELEASE) {
                sm->current_state = BTN_RELEASED;
                printf("[ButtonSM] 액션: 짧은 누름 처리\n");
            }
            else if (event == BTN_EVT_TIMEOUT) {
                sm->hold_counter++;
                if (sm->hold_counter >= sm->hold_threshold) {
                    sm->current_state = BTN_HELD;
                    printf("[ButtonSM] 액션: 길게 누름 감지\n");
                }
            }
            break;
            
        case BTN_HELD:
            if (event == BTN_EVT_RELEASE) {
                sm->current_state = BTN_RELEASED;
                printf("[ButtonSM] 액션: 길게 누름 릴리즈\n");
            }
            break;
    }
    
    if (old_state != sm->current_state) {
        printf("[ButtonSM] 상태 전이: %d → %d\n", old_state, sm->current_state);
    }
}

/* ============================================================================
 * 2. FOTA State Machine - 펌웨어 업데이트
 * ============================================================================ */

// FOTA 상태
typedef enum {
    FOTA_IDLE,
    FOTA_DOWNLOADING,
    FOTA_VERIFYING,
    FOTA_UPDATING,
    FOTA_COMPLETED,
    FOTA_ERROR
} FotaState;

// FOTA 이벤트
typedef enum {
    FOTA_EVT_START,
    FOTA_EVT_DATA_RECEIVED,
    FOTA_EVT_DOWNLOAD_COMPLETE,
    FOTA_EVT_VERIFY_OK,
    FOTA_EVT_VERIFY_FAIL,
    FOTA_EVT_UPDATE_DONE,
    FOTA_EVT_ERROR,
    FOTA_EVT_RESET
} FotaEvent;

// FOTA 상태 머신
typedef struct {
    FotaState current_state;
    int download_progress;
    int total_size;
    int received_size;
    uint32_t checksum;
} FotaStateMachine;

// FOTA 상태를 문자열로 변환
const char* FotaState_ToString(FotaState state) {
    switch (state) {
        case FOTA_IDLE: return "IDLE";
        case FOTA_DOWNLOADING: return "DOWNLOADING";
        case FOTA_VERIFYING: return "VERIFYING";
        case FOTA_UPDATING: return "UPDATING";
        case FOTA_COMPLETED: return "COMPLETED";
        case FOTA_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// FOTA 상태 머신 초기화
void FotaSM_Init(FotaStateMachine* sm) {
    sm->current_state = FOTA_IDLE;
    sm->download_progress = 0;
    sm->total_size = 0;
    sm->received_size = 0;
    sm->checksum = 0;
    printf("[FOTA] 초기화: 상태=%s\n", FotaState_ToString(sm->current_state));
}

// 진입 액션
void FotaSM_OnEnter(FotaState state) {
    printf("[FOTA] 진입 액션: %s 상태 시작\n", FotaState_ToString(state));
    
    switch (state) {
        case FOTA_DOWNLOADING:
            printf("  → 다운로드 버퍼 준비\n");
            break;
        case FOTA_VERIFYING:
            printf("  → CRC 체크 시작\n");
            break;
        case FOTA_UPDATING:
            printf("  → Flash 쓰기 준비\n");
            break;
        case FOTA_COMPLETED:
            printf("  → 재시작 준비\n");
            break;
        default:
            break;
    }
}

// 진출 액션
void FotaSM_OnExit(FotaState state) {
    printf("[FOTA] 진출 액션: %s 상태 종료\n", FotaState_ToString(state));
}

// FOTA 이벤트 처리
void FotaSM_ProcessEvent(FotaStateMachine* sm, FotaEvent event) {
    FotaState old_state = sm->current_state;
    
    switch (sm->current_state) {
        case FOTA_IDLE:
            if (event == FOTA_EVT_START) {
                FotaSM_OnExit(sm->current_state);
                sm->current_state = FOTA_DOWNLOADING;
                sm->received_size = 0;
                sm->total_size = 1000;  // 시뮬레이션
                FotaSM_OnEnter(sm->current_state);
            }
            break;
            
        case FOTA_DOWNLOADING:
            if (event == FOTA_EVT_DATA_RECEIVED) {
                sm->received_size += 100;  // 시뮬레이션
                sm->download_progress = (sm->received_size * 100) / sm->total_size;
                printf("[FOTA] 다운로드 진행: %d%%\n", sm->download_progress);
            }
            else if (event == FOTA_EVT_DOWNLOAD_COMPLETE) {
                FotaSM_OnExit(sm->current_state);
                sm->current_state = FOTA_VERIFYING;
                FotaSM_OnEnter(sm->current_state);
            }
            else if (event == FOTA_EVT_ERROR) {
                FotaSM_OnExit(sm->current_state);
                sm->current_state = FOTA_ERROR;
                FotaSM_OnEnter(sm->current_state);
            }
            break;
            
        case FOTA_VERIFYING:
            if (event == FOTA_EVT_VERIFY_OK) {
                FotaSM_OnExit(sm->current_state);
                sm->current_state = FOTA_UPDATING;
                FotaSM_OnEnter(sm->current_state);
            }
            else if (event == FOTA_EVT_VERIFY_FAIL) {
                FotaSM_OnExit(sm->current_state);
                sm->current_state = FOTA_ERROR;
                FotaSM_OnEnter(sm->current_state);
            }
            break;
            
        case FOTA_UPDATING:
            if (event == FOTA_EVT_UPDATE_DONE) {
                FotaSM_OnExit(sm->current_state);
                sm->current_state = FOTA_COMPLETED;
                FotaSM_OnEnter(sm->current_state);
            }
            else if (event == FOTA_EVT_ERROR) {
                FotaSM_OnExit(sm->current_state);
                sm->current_state = FOTA_ERROR;
                FotaSM_OnEnter(sm->current_state);
            }
            break;
            
        case FOTA_COMPLETED:
        case FOTA_ERROR:
            if (event == FOTA_EVT_RESET) {
                FotaSM_OnExit(sm->current_state);
                sm->current_state = FOTA_IDLE;
                FotaSM_OnEnter(sm->current_state);
            }
            break;
    }
    
    if (old_state != sm->current_state) {
        printf("[FOTA] ★ 상태 전이: %s → %s\n", 
               FotaState_ToString(old_state), 
               FotaState_ToString(sm->current_state));
    }
}

/* ============================================================================
 * 3. 통신 프로토콜 State Machine
 * ============================================================================ */

// 통신 상태
typedef enum {
    COMM_DISCONNECTED,
    COMM_CONNECTING,
    COMM_CONNECTED,
    COMM_AUTHENTICATING,
    COMM_AUTHENTICATED,
    COMM_TRANSFERRING,
    COMM_DISCONNECTING
} CommState;

// 통신 이벤트
typedef enum {
    COMM_EVT_CONNECT,
    COMM_EVT_CONNECTED,
    COMM_EVT_AUTH_START,
    COMM_EVT_AUTH_OK,
    COMM_EVT_AUTH_FAIL,
    COMM_EVT_DATA_SEND,
    COMM_EVT_DATA_SENT,
    COMM_EVT_DISCONNECT,
    COMM_EVT_TIMEOUT,
    COMM_EVT_ERROR
} CommEvent;

// 통신 상태 머신
typedef struct {
    CommState current_state;
    int retry_count;
    int max_retries;
    char session_token[32];
} CommStateMachine;

// 통신 상태를 문자열로 변환
const char* CommState_ToString(CommState state) {
    switch (state) {
        case COMM_DISCONNECTED: return "DISCONNECTED";
        case COMM_CONNECTING: return "CONNECTING";
        case COMM_CONNECTED: return "CONNECTED";
        case COMM_AUTHENTICATING: return "AUTHENTICATING";
        case COMM_AUTHENTICATED: return "AUTHENTICATED";
        case COMM_TRANSFERRING: return "TRANSFERRING";
        case COMM_DISCONNECTING: return "DISCONNECTING";
        default: return "UNKNOWN";
    }
}

// 통신 상태 머신 초기화
void CommSM_Init(CommStateMachine* sm) {
    sm->current_state = COMM_DISCONNECTED;
    sm->retry_count = 0;
    sm->max_retries = 3;
    memset(sm->session_token, 0, sizeof(sm->session_token));
    printf("[CommSM] 초기화: 상태=%s\n", CommState_ToString(sm->current_state));
}

// 통신 이벤트 처리
void CommSM_ProcessEvent(CommStateMachine* sm, CommEvent event) {
    CommState old_state = sm->current_state;
    
    switch (sm->current_state) {
        case COMM_DISCONNECTED:
            if (event == COMM_EVT_CONNECT) {
                sm->current_state = COMM_CONNECTING;
                sm->retry_count = 0;
                printf("[CommSM] 액션: 연결 시도 중...\n");
            }
            break;
            
        case COMM_CONNECTING:
            if (event == COMM_EVT_CONNECTED) {
                sm->current_state = COMM_CONNECTED;
                printf("[CommSM] 액션: 연결 성공\n");
            }
            else if (event == COMM_EVT_TIMEOUT || event == COMM_EVT_ERROR) {
                sm->retry_count++;
                if (sm->retry_count >= sm->max_retries) {
                    sm->current_state = COMM_DISCONNECTED;
                    printf("[CommSM] 액션: 최대 재시도 초과, 연결 실패\n");
                } else {
                    printf("[CommSM] 액션: 재시도 %d/%d\n", 
                           sm->retry_count, sm->max_retries);
                }
            }
            break;
            
        case COMM_CONNECTED:
            if (event == COMM_EVT_AUTH_START) {
                sm->current_state = COMM_AUTHENTICATING;
                printf("[CommSM] 액션: 인증 시작\n");
            }
            else if (event == COMM_EVT_DISCONNECT) {
                sm->current_state = COMM_DISCONNECTING;
            }
            break;
            
        case COMM_AUTHENTICATING:
            if (event == COMM_EVT_AUTH_OK) {
                sm->current_state = COMM_AUTHENTICATED;
                snprintf(sm->session_token, sizeof(sm->session_token), "TOKEN_%d", rand());
                printf("[CommSM] 액션: 인증 성공 (토큰: %s)\n", sm->session_token);
            }
            else if (event == COMM_EVT_AUTH_FAIL) {
                sm->current_state = COMM_CONNECTED;
                printf("[CommSM] 액션: 인증 실패, 재시도 가능\n");
            }
            break;
            
        case COMM_AUTHENTICATED:
            if (event == COMM_EVT_DATA_SEND) {
                sm->current_state = COMM_TRANSFERRING;
                printf("[CommSM] 액션: 데이터 전송 중...\n");
            }
            else if (event == COMM_EVT_DISCONNECT) {
                sm->current_state = COMM_DISCONNECTING;
            }
            break;
            
        case COMM_TRANSFERRING:
            if (event == COMM_EVT_DATA_SENT) {
                sm->current_state = COMM_AUTHENTICATED;
                printf("[CommSM] 액션: 데이터 전송 완료\n");
            }
            else if (event == COMM_EVT_ERROR) {
                sm->current_state = COMM_CONNECTED;
                printf("[CommSM] 액션: 전송 오류, 재인증 필요\n");
            }
            break;
            
        case COMM_DISCONNECTING:
            sm->current_state = COMM_DISCONNECTED;
            memset(sm->session_token, 0, sizeof(sm->session_token));
            printf("[CommSM] 액션: 연결 종료\n");
            break;
    }
    
    if (old_state != sm->current_state) {
        printf("[CommSM] ★ 상태 전이: %s → %s\n", 
               CommState_ToString(old_state), 
               CommState_ToString(sm->current_state));
    }
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("========================================\n");
    printf("State Machine 패턴 예제\n");
    printf("========================================\n\n");
    
    // 1. 버튼 디바운싱 State Machine
    printf("=== 1. 버튼 디바운싱 State Machine ===\n");
    ButtonStateMachine button_sm;
    ButtonSM_Init(&button_sm, 3);
    
    ButtonSM_ProcessEvent(&button_sm, BTN_EVT_PRESS);
    ButtonSM_ProcessEvent(&button_sm, BTN_EVT_TIMEOUT);
    ButtonSM_ProcessEvent(&button_sm, BTN_EVT_TIMEOUT);
    ButtonSM_ProcessEvent(&button_sm, BTN_EVT_TIMEOUT);  // HELD 상태로
    ButtonSM_ProcessEvent(&button_sm, BTN_EVT_RELEASE);
    printf("\n");
    
    // 2. FOTA State Machine
    printf("=== 2. FOTA (펌웨어 업데이트) State Machine ===\n");
    FotaStateMachine fota_sm;
    FotaSM_Init(&fota_sm);
    
    FotaSM_ProcessEvent(&fota_sm, FOTA_EVT_START);
    
    // 다운로드 시뮬레이션
    for (int i = 0; i < 10; i++) {
        FotaSM_ProcessEvent(&fota_sm, FOTA_EVT_DATA_RECEIVED);
    }
    FotaSM_ProcessEvent(&fota_sm, FOTA_EVT_DOWNLOAD_COMPLETE);
    
    // 검증 및 업데이트
    FotaSM_ProcessEvent(&fota_sm, FOTA_EVT_VERIFY_OK);
    FotaSM_ProcessEvent(&fota_sm, FOTA_EVT_UPDATE_DONE);
    
    printf("\n");
    
    // 3. 통신 프로토콜 State Machine
    printf("=== 3. 통신 프로토콜 State Machine ===\n");
    CommStateMachine comm_sm;
    CommSM_Init(&comm_sm);
    
    CommSM_ProcessEvent(&comm_sm, COMM_EVT_CONNECT);
    CommSM_ProcessEvent(&comm_sm, COMM_EVT_CONNECTED);
    CommSM_ProcessEvent(&comm_sm, COMM_EVT_AUTH_START);
    CommSM_ProcessEvent(&comm_sm, COMM_EVT_AUTH_OK);
    CommSM_ProcessEvent(&comm_sm, COMM_EVT_DATA_SEND);
    CommSM_ProcessEvent(&comm_sm, COMM_EVT_DATA_SENT);
    CommSM_ProcessEvent(&comm_sm, COMM_EVT_DISCONNECT);
    
    printf("\n");
    
    // 4. 오류 처리 시나리오
    printf("=== 4. 오류 처리 시나리오 ===\n");
    FotaStateMachine fota_sm2;
    FotaSM_Init(&fota_sm2);
    
    FotaSM_ProcessEvent(&fota_sm2, FOTA_EVT_START);
    FotaSM_ProcessEvent(&fota_sm2, FOTA_EVT_DATA_RECEIVED);
    FotaSM_ProcessEvent(&fota_sm2, FOTA_EVT_ERROR);  // 다운로드 중 오류
    FotaSM_ProcessEvent(&fota_sm2, FOTA_EVT_RESET);  // 초기화
    
    printf("\n========================================\n");
    printf("State Machine 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * State Machine 패턴의 장점 총정리
 * ============================================================================
 * 
 * 1. 복잡한 로직을 명확하게 구조화
 * 2. 현재 상태 추적 용이 (디버깅 편함)
 * 3. 상태 다이어그램으로 문서화 가능
 * 4. 테스트 용이 (상태별 테스트)
 * 5. 확장성 좋음 (새 상태 추가 쉬움)
 * 
 * ============================================================================
 * State Machine 패턴의 주의사항
 * ============================================================================
 * 
 * 1. 상태가 너무 많으면 복잡도 증가
 * 2. 전이 조건이 복잡하면 가독성 저하
 * 3. 재진입 고려 필요 (RTOS 환경)
 * 4. 메모리 사용량 고려 (큰 상태 데이터)
 * 
 * ============================================================================
 * 임베디드 FW 개발에서 State Machine의 실전 활용
 * ============================================================================
 * 
 * 1. FOTA (펌웨어 업데이트)
 *    Idle → Download → Verify → Update → Complete
 * 
 * 2. 통신 프로토콜
 *    Disconnect → Connect → Auth → Transfer → Disconnect
 * 
 * 3. 모터 제어
 *    Stop → Accel → Run → Decel → Brake → Stop
 * 
 * 4. 배터리 충전
 *    Idle → PreCharge → FastCharge → TopOff → Full
 * 
 * 5. 센서 초기화
 *    PowerOff → PowerOn → Config → Calibrate → Ready
 * 
 * 6. LCD 디스플레이
 *    Off → Init → Idle → Display → Sleep → Off
 * 
 * ============================================================================
 * 면접 / 기술 면담 대응 포인트
 * ============================================================================
 * 
 * Q: State Machine을 사용한 경험이 있나요?
 * A: "네, FOTA 시스템에서 펌웨어 업데이트 절차를 State Machine으로
 *     구현했습니다. Idle, Download, Verify, Update, Complete 상태로
 *     나누어 각 단계에서 오류 처리와 롤백이 명확하게 구현되었고,
 *     디버깅 시 현재 상태만 확인하면 문제를 빠르게 찾을 수 있었습니다."
 * 
 * Q: State Machine을 왜 사용하나요?
 * A: "복잡한 시퀀스를 if/else로 구현하면 플래그가 많아지고 디버깅이
 *     어려워집니다. State Machine으로 상태와 전이를 명확히 정의하면
 *     코드 가독성이 높아지고, 상태 다이어그램으로 문서화도 쉬워집니다."
 * 
 * Q: switch/case vs 함수 포인터 테이블?
 * A: "switch/case는 단순하고 명확하지만, 상태가 많아지면 코드가 길어집니다.
 *     함수 포인터 테이블은 확장성이 좋지만 복잡도가 높아집니다.
 *     일반적으로 switch/case로 시작하고, 필요 시 함수 포인터로 리팩토링합니다."
 * 
 * Q: 진입/진출 액션의 목적은?
 * A: "상태가 바뀔 때마다 초기화나 정리 작업이 필요합니다.
 *     진입 액션은 상태 시작 시 필요한 설정을, 진출 액션은 상태 종료 시
 *     정리 작업을 수행하여 각 상태의 전제 조건을 보장합니다."
 * 
 * ============================================================================
 * 실전 체크리스트
 * ============================================================================
 * 
 * State Machine 적용 전 확인:
 * 
 * □ 시스템이 명확한 단계로 나뉘는가?
 * □ 단계 간 전이 조건이 명확한가?
 * □ 현재 상태에 따라 동작이 달라지는가?
 * □ if/else가 3단계 이상 중첩되는가?
 * □ 플래그 변수가 3개 이상인가?
 * 
 * 👉 3개 이상 체크되면 State Machine 적용 추천
 * 
 * ============================================================================
 * 다음 학습 단계 추천
 * ============================================================================
 * 
 * 1. 계층형 State Machine (HSM)
 * 2. State Machine + Callback 통합
 * 3. State Machine + Event Queue
 * 4. UML State Diagram 작성
 * 5. 실제 FOTA 시스템 구현
 * 
 * ============================================================================
 */
