/*
 * ============================================================================
 * 파일명: 09_command_pattern.c
 * 기능: C언어 Command(명령) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Command 패턴이란?
 * ============================================================================
 * 
 * 요청을 객체로 캡슐화하여 매개변수화, 큐잉, 로깅, 실행 취소 지원
 * 
 * 핵심:
 *   - 명령을 객체화
 *   - 실행(Execute), 취소(Undo) 지원
 *   - 명령 큐잉 가능
 * 
 * ============================================================================
 * 2️⃣ 임베디드에서 Command 활용
 * ============================================================================
 * 
 * - 모터 제어 (전진/후진/정지)
 * - LED 제어 (켜기/끄기/밝기 조절)
 * - 명령 큐 (작업 스케줄링)
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Command 인터페이스 */
typedef struct Command {
    void (*execute)(struct Command* self);
    void (*undo)(struct Command* self);
    char name[32];
} Command;

/* LED 상태 */
typedef struct {
    int is_on;
    int brightness;
} LED;

/* LED On 명령 */
typedef struct {
    Command base;
    LED* led;
} LedOnCommand;

void LedOn_Execute(Command* self) {
    LedOnCommand* cmd = (LedOnCommand*)self;
    cmd->led->is_on = 1;
    printf("[Command] LED ON 실행\n");
}

void LedOn_Undo(Command* self) {
    LedOnCommand* cmd = (LedOnCommand*)self;
    cmd->led->is_on = 0;
    printf("[Command] LED ON 취소 (OFF)\n");
}

LedOnCommand* LedOnCommand_Create(LED* led) {
    LedOnCommand* cmd = (LedOnCommand*)malloc(sizeof(LedOnCommand));
    cmd->base.execute = LedOn_Execute;
    cmd->base.undo = LedOn_Undo;
    strcpy(cmd->base.name, "LED_ON");
    cmd->led = led;
    return cmd;
}

/* LED Off 명령 */
typedef struct {
    Command base;
    LED* led;
} LedOffCommand;

void LedOff_Execute(Command* self) {
    LedOffCommand* cmd = (LedOffCommand*)self;
    cmd->led->is_on = 0;
    printf("[Command] LED OFF 실행\n");
}

void LedOff_Undo(Command* self) {
    LedOffCommand* cmd = (LedOffCommand*)self;
    cmd->led->is_on = 1;
    printf("[Command] LED OFF 취소 (ON)\n");
}

LedOffCommand* LedOffCommand_Create(LED* led) {
    LedOffCommand* cmd = (LedOffCommand*)malloc(sizeof(LedOffCommand));
    cmd->base.execute = LedOff_Execute;
    cmd->base.undo = LedOff_Undo;
    strcpy(cmd->base.name, "LED_OFF");
    cmd->led = led;
    return cmd;
}

/* Command Invoker (명령 실행자) */
#define MAX_HISTORY 10

typedef struct {
    Command* history[MAX_HISTORY];
    int history_count;
} CommandInvoker;

void Invoker_Init(CommandInvoker* invoker) {
    invoker->history_count = 0;
    for (int i = 0; i < MAX_HISTORY; i++) {
        invoker->history[i] = NULL;
    }
    printf("[Invoker] 명령 실행자 초기화\n");
}

void Invoker_Execute(CommandInvoker* invoker, Command* cmd) {
    printf("[Invoker] 명령 실행: %s\n", cmd->name);
    cmd->execute(cmd);
    
    // 히스토리에 저장
    if (invoker->history_count < MAX_HISTORY) {
        invoker->history[invoker->history_count++] = cmd;
    }
}

void Invoker_Undo(CommandInvoker* invoker) {
    if (invoker->history_count > 0) {
        Command* last_cmd = invoker->history[--invoker->history_count];
        printf("[Invoker] 명령 취소: %s\n", last_cmd->name);
        last_cmd->undo(last_cmd);
    } else {
        printf("[Invoker] 취소할 명령이 없습니다\n");
    }
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Command 패턴 예제\n");
    printf("========================================\n\n");
    
    // LED 생성
    LED led = { .is_on = 0, .brightness = 0 };
    
    // 명령 생성
    LedOnCommand* on_cmd = LedOnCommand_Create(&led);
    LedOffCommand* off_cmd = LedOffCommand_Create(&led);
    
    // Invoker 생성
    CommandInvoker invoker;
    Invoker_Init(&invoker);
    
    // 명령 실행
    printf("\n=== 명령 실행 ===\n");
    Invoker_Execute(&invoker, (Command*)on_cmd);
    printf("LED 상태: %s\n", led.is_on ? "ON" : "OFF");
    
    Invoker_Execute(&invoker, (Command*)off_cmd);
    printf("LED 상태: %s\n", led.is_on ? "ON" : "OFF");
    
    // 명령 취소 (Undo)
    printf("\n=== 명령 취소 (Undo) ===\n");
    Invoker_Undo(&invoker);
    printf("LED 상태: %s\n", led.is_on ? "ON" : "OFF");
    
    Invoker_Undo(&invoker);
    printf("LED 상태: %s\n", led.is_on ? "ON" : "OFF");
    
    // 정리
    free(on_cmd);
    free(off_cmd);
    
    printf("\n========================================\n");
    printf("Command 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Command 패턴의 장점
 * ============================================================================
 * 
 * 1. 요청을 객체로 캡슐화
 * 2. 실행 취소(Undo) 지원
 * 3. 명령 큐잉, 로깅 가능
 * 4. 매크로 명령 (여러 명령 조합)
 * 
 * ============================================================================
 */
