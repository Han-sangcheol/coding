/*
 * ============================================================================
 * 파일명: 07_observer_pattern.c
 * 기능: C언어 Observer(옵저버) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Observer 패턴이란?
 * ============================================================================
 * 
 * 한 객체의 상태 변화를 관찰하는 여러 옵저버들에게
 * 자동으로 알림을 보내는 패턴 (발행-구독 패턴)
 * 
 * 핵심 키워드:
 *   - Subject (주체)
 *   - Observer (관찰자)
 *   - Notify (알림)
 *   - Subscribe/Unsubscribe (구독/구독 취소)
 * 
 * 구조:
 *   [Subject] --notify--> [Observer1, Observer2, Observer3...]
 * 
 * ============================================================================
 * 2️⃣ 왜 Observer를 쓰나?
 * ============================================================================
 * 
 * ✅ Observer의 장점:
 *    - 느슨한 결합 (Subject는 Observer 구체 타입 몰라도 됨)
 *    - 동적으로 Observer 추가/제거
 *    - 일대다 관계 처리
 *    - 브로드캐스트 통신
 * 
 * ============================================================================
 * 3️⃣ 임베디드에서 Observer를 쓰는 경우
 * ============================================================================
 * 
 * 실무 적용 사례:
 *   ✅ 센서 데이터를 여러 모듈이 구독
 *   ✅ 시스템 이벤트 브로드캐스트
 *   ✅ 상태 변화를 UI/로거/통신 모듈에 통지
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_OBSERVERS 10

/* Observer 인터페이스 */
typedef struct Observer {
    void (*update)(struct Observer* self, void* data);
    char name[32];
} Observer;

/* Subject 구조체 */
typedef struct {
    Observer* observers[MAX_OBSERVERS];
    int observer_count;
    float temperature;
} TemperatureSubject;

/* Subject 초기화 */
void Subject_Init(TemperatureSubject* subject) {
    subject->observer_count = 0;
    subject->temperature = 25.0f;
    for (int i = 0; i < MAX_OBSERVERS; i++) {
        subject->observers[i] = NULL;
    }
    printf("[Subject] 온도 Subject 초기화\n");
}

/* Observer 등록 */
int Subject_Attach(TemperatureSubject* subject, Observer* observer) {
    if (subject->observer_count >= MAX_OBSERVERS) {
        printf("[Subject] Observer 등록 실패: 최대 개수 초과\n");
        return -1;
    }
    
    subject->observers[subject->observer_count++] = observer;
    printf("[Subject] Observer 등록: %s (총 %d개)\n", 
           observer->name, subject->observer_count);
    return 0;
}

/* Observer 제거 */
int Subject_Detach(TemperatureSubject* subject, Observer* observer) {
    for (int i = 0; i < subject->observer_count; i++) {
        if (subject->observers[i] == observer) {
            // 배열에서 제거
            for (int j = i; j < subject->observer_count - 1; j++) {
                subject->observers[j] = subject->observers[j + 1];
            }
            subject->observers[--subject->observer_count] = NULL;
            printf("[Subject] Observer 제거: %s\n", observer->name);
            return 0;
        }
    }
    return -1;
}

/* 모든 Observer에게 알림 */
void Subject_Notify(TemperatureSubject* subject) {
    printf("[Subject] Notify: %.1f°C → %d개 Observer에게 알림\n", 
           subject->temperature, subject->observer_count);
    
    for (int i = 0; i < subject->observer_count; i++) {
        if (subject->observers[i] && subject->observers[i]->update) {
            subject->observers[i]->update(subject->observers[i], 
                                         &subject->temperature);
        }
    }
}

/* 온도 설정 (상태 변경 시 자동 알림) */
void Subject_SetTemperature(TemperatureSubject* subject, float temp) {
    subject->temperature = temp;
    Subject_Notify(subject);
}

/* ============================================================================
 * 구체적인 Observer 구현들
 * ============================================================================ */

// Display Observer
typedef struct {
    Observer base;
    int display_id;
} DisplayObserver;

void Display_Update(Observer* self, void* data) {
    DisplayObserver* display = (DisplayObserver*)self;
    float* temp = (float*)data;
    printf("  → [Display %d] 화면 업데이트: %.1f°C\n", display->display_id, *temp);
}

DisplayObserver* DisplayObserver_Create(int id) {
    DisplayObserver* observer = (DisplayObserver*)malloc(sizeof(DisplayObserver));
    observer->base.update = Display_Update;
    snprintf(observer->base.name, sizeof(observer->base.name), "Display%d", id);
    observer->display_id = id;
    return observer;
}

// Logger Observer
typedef struct {
    Observer base;
    FILE* log_file;
} LoggerObserver;

void Logger_Update(Observer* self, void* data) {
    LoggerObserver* logger = (LoggerObserver*)self;
    float* temp = (float*)data;
    time_t now = time(NULL);
    printf("  → [Logger] 로그 기록: %.1f°C\n", *temp);
    
    if (logger->log_file) {
        fprintf(logger->log_file, "[%ld] Temperature: %.1f°C\n", now, *temp);
        fflush(logger->log_file);
    }
}

LoggerObserver* LoggerObserver_Create(const char* filename) {
    LoggerObserver* observer = (LoggerObserver*)malloc(sizeof(LoggerObserver));
    observer->base.update = Logger_Update;
    snprintf(observer->base.name, sizeof(observer->base.name), "Logger");
    observer->log_file = fopen(filename, "a");
    return observer;
}

// Alarm Observer
typedef struct {
    Observer base;
    float threshold;
} AlarmObserver;

void Alarm_Update(Observer* self, void* data) {
    AlarmObserver* alarm = (AlarmObserver*)self;
    float* temp = (float*)data;
    
    if (*temp > alarm->threshold) {
        printf("  → [Alarm] ⚠️  경고! 온도가 임계값(%.1f°C)을 초과했습니다: %.1f°C\n", 
               alarm->threshold, *temp);
    } else {
        printf("  → [Alarm] 온도 정상: %.1f°C\n", *temp);
    }
}

AlarmObserver* AlarmObserver_Create(float threshold) {
    AlarmObserver* observer = (AlarmObserver*)malloc(sizeof(AlarmObserver));
    observer->base.update = Alarm_Update;
    snprintf(observer->base.name, sizeof(observer->base.name), "Alarm");
    observer->threshold = threshold;
    return observer;
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    printf("========================================\n");
    printf("Observer 패턴 예제\n");
    printf("========================================\n\n");
    
    // Subject 생성
    TemperatureSubject temp_subject;
    Subject_Init(&temp_subject);
    
    printf("\n=== Observer 등록 ===\n");
    // Observer들 생성 및 등록
    DisplayObserver* display1 = DisplayObserver_Create(1);
    DisplayObserver* display2 = DisplayObserver_Create(2);
    LoggerObserver* logger = LoggerObserver_Create("temp_log.txt");
    AlarmObserver* alarm = AlarmObserver_Create(30.0f);
    
    Subject_Attach(&temp_subject, (Observer*)display1);
    Subject_Attach(&temp_subject, (Observer*)display2);
    Subject_Attach(&temp_subject, (Observer*)logger);
    Subject_Attach(&temp_subject, (Observer*)alarm);
    
    // 온도 변경 시뮬레이션
    printf("\n=== 온도 변경 이벤트 ===\n");
    Subject_SetTemperature(&temp_subject, 25.0f);
    
    printf("\n");
    Subject_SetTemperature(&temp_subject, 28.5f);
    
    printf("\n");
    Subject_SetTemperature(&temp_subject, 32.0f);  // 알람 발생
    
    // Observer 제거
    printf("\n=== Observer 제거 ===\n");
    Subject_Detach(&temp_subject, (Observer*)display2);
    
    printf("\n");
    Subject_SetTemperature(&temp_subject, 27.0f);
    
    // 정리
    free(display1);
    free(display2);
    if (logger->log_file) fclose(logger->log_file);
    free(logger);
    free(alarm);
    
    printf("\n========================================\n");
    printf("Observer 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Observer 패턴의 장점
 * ============================================================================
 * 
 * 1. Subject와 Observer의 느슨한 결합
 * 2. 런타임에 동적으로 Observer 추가/제거
 * 3. 일대다 관계를 쉽게 구현
 * 4. 브로드캐스트 통신 지원
 * 
 * ============================================================================
 * 면접 대응
 * ============================================================================
 * 
 * Q: Observer 패턴을 사용한 경험은?
 * A: "센서 데이터를 여러 모듈(UI, 로거, 알람)이 동시에 사용해야 해서
 *     Observer 패턴으로 구현했습니다. 센서 모듈은 구독자가 누구인지
 *     몰라도 되고, 런타임에 새 구독자를 추가할 수 있어 유연했습니다."
 * 
 * ============================================================================
 */
