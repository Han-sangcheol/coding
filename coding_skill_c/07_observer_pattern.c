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
 *   - Subject (주체) : 관찰 대상이 되는 객체, 상태를 가지고 있음
 *   - Observer (관찰자) : Subject를 구독하는 객체들
 *   - Notify (알림) : Subject가 Observer들에게 변경을 알림
 *   - Subscribe/Unsubscribe (구독/구독 취소) : Attach/Detach
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
 * 4️⃣ 코드 구조 개요 (이 파일의 구성)
 * ============================================================================
 * 
 * [1단계] Observer 인터페이스 정의
 *         - 모든 관찰자가 공통으로 가져야 할 구조
 *         - update() 함수 포인터가 핵심
 * 
 * [2단계] Subject 구조체 정의
 *         - 관찰 대상 (온도 센서)
 *         - Observer 리스트를 배열로 관리
 * 
 * [3단계] Subject 관련 함수들
 *         - Init, Attach, Detach, Notify, SetTemperature
 * 
 * [4단계] 구체적인 Observer 구현들
 *         - DisplayObserver: 화면에 표시
 *         - LoggerObserver: 파일에 기록
 *         - AlarmObserver: 임계값 확인 후 경고
 * 
 * [5단계] main() 함수에서 실제 사용
 *         - Subject 생성 → Observer 생성 → Attach → 온도 변경
 * 
 * ============================================================================
 * 5️⃣ 전체 데이터 흐름 다이어그램
 * ============================================================================
 * 
 * [초기화 단계]
 *   Subject_Init()
 *      ↓
 *   observers[] 배열 초기화
 * 
 * [등록 단계]
 *   DisplayObserver_Create()  →  display1
 *   LoggerObserver_Create()   →  logger
 *   AlarmObserver_Create()    →  alarm
 *      ↓
 *   Subject_Attach(&subject, (Observer*)display1)
 *   Subject_Attach(&subject, (Observer*)logger)
 *   Subject_Attach(&subject, (Observer*)alarm)
 *      ↓
 *   observers[] = {display1, logger, alarm, ...}
 * 
 * [알림 단계]
 *   Subject_SetTemperature(30.0)
 *      ↓
 *   subject.temperature = 30.0
 *      ↓
 *   Subject_Notify()
 *      ↓
 *   ┌─────────────────────────────────────┐
 *   │ for (i=0; i<observer_count; i++)    │
 *   │   observers[i]->update(...)         │
 *   └─────────────────────────────────────┘
 *      ↓        ↓         ↓
 *   Display   Logger   Alarm
 *   _Update   _Update  _Update
 *      ↓        ↓         ↓
 *   화면표시   파일기록   임계값체크
 * 
 * [핵심] 함수 포인터를 통한 다형성
 *   - observers[0]->update  →  Display_Update() 호출
 *   - observers[1]->update  →  Logger_Update() 호출
 *   - observers[2]->update  →  Alarm_Update() 호출
 * 
 * ============================================================================
 * 6️⃣ 메모리 구조 (상속 구현)
 * ============================================================================
 * 
 * DisplayObserver 메모리 레이아웃:
 * 
 *   주소 0x1000  ┌─────────────────────┐
 *                │ Observer base       │ ← Observer*로 캐스팅 가능
 *                │  - update (8 bytes) │
 *                │  - name[32]         │
 *   주소 0x1028  ├─────────────────────┤
 *                │ display_id (4 bytes)│ ← DisplayObserver만의 데이터
 *   주소 0x102C  └─────────────────────┘
 * 
 * Subject의 observers[] 배열:
 * 
 *   observers[0] = 0x1000  (DisplayObserver*)
 *   observers[1] = 0x2000  (LoggerObserver*)
 *   observers[2] = 0x3000  (AlarmObserver*)
 *   
 *   → 모두 Observer*로 취급
 *   → update() 호출시 각자의 함수가 실행 (다형성)
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_OBSERVERS 10

/* ============================================================================
 * Observer 인터페이스 (기본 틀)
 * ============================================================================
 * 
 * 역할: 모든 관찰자(Observer)가 공통으로 가져야 할 기본 구조
 * 
 * 핵심 개념:
 *   - 이것은 '추상 클래스'와 같은 역할 (C++의 interface 개념)
 *   - 구체적인 Observer들은 이 구조체를 '상속'받아 확장
 * 
 * 메모리 구조:
 *   +------------------+
 *   | update (함수포인터) |  8 bytes (64bit) - Subject가 이걸 호출
 *   +------------------+
 *   | name[32]         |  32 bytes - Observer 식별용 이름
 *   +------------------+
 *   총 40 bytes
 * 
 * 사용 방법:
 *   1. update 함수 포인터를 구체적인 함수로 설정
 *   2. Subject가 Notify할 때 이 update()를 호출
 *   3. update(self, data)에서 self는 자기 자신, data는 Subject의 상태
 * 
 * ============================================================================ */
typedef struct Observer {
    void (*update)(struct Observer* self, void* data);  // 핵심: 콜백 함수 포인터
    char name[32];  // Observer 구분용 이름
} Observer;

/* ============================================================================
 * Subject 구조체 (관찰 대상)
 * ============================================================================
 * 
 * 역할: 상태를 가지고 있으며, 상태 변화시 Observer들에게 알림
 * 
 * 핵심 개념:
 *   - '발행자(Publisher)' 역할
 *   - Observer 리스트를 배열로 관리 (등록된 모든 구독자들)
 *   - 상태(temperature)가 변경되면 자동으로 Notify 호출
 * 
 * 메모리 구조:
 *   +---------------------------+
 *   | observers[10]             |  80 bytes (포인터 배열 10개)
 *   +---------------------------+
 *   | observer_count            |  4 bytes - 현재 등록된 Observer 개수
 *   +---------------------------+
 *   | temperature               |  4 bytes - 관찰 대상 상태 (온도값)
 *   +---------------------------+
 *   총 88 bytes
 * 
 * 핵심 동작:
 *   1. Observer 배열에 구독자들의 포인터를 저장
 *   2. 상태(temperature) 변경시 모든 Observer의 update() 호출
 *   3. Observer들은 구체 타입을 몰라도 됨 (다형성)
 * 
 * ============================================================================ */
typedef struct {
    Observer* observers[MAX_OBSERVERS];  // Observer 포인터 배열 (구독자 리스트)
    int observer_count;                   // 현재 등록된 Observer 개수
    float temperature;                    // 관찰 대상 상태값 (온도)
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

/* ============================================================================
 * 모든 Observer에게 알림 (핵심 함수!)
 * ============================================================================
 * 
 * 데이터 흐름:
 *   Subject.temperature 변경
 *      ↓
 *   Subject_Notify() 호출
 *      ↓
 *   observers 배열을 순회하며 각 Observer의 update() 호출
 *      ↓
 *   각 Observer가 자신의 방식대로 처리
 *      - DisplayObserver: 화면에 표시
 *      - LoggerObserver: 파일에 기록
 *      - AlarmObserver: 임계값 체크
 * 
 * 함수 포인터 호출 메커니즘:
 *   observers[i]->update(observers[i], &temperature)
 *                  ↑
 *   이 update는 각 Observer의 구체적인 함수를 가리킴
 *   예: Display_Update, Logger_Update, Alarm_Update
 * 
 * ============================================================================ */
void Subject_Notify(TemperatureSubject* subject) {
    printf("[Subject] Notify: %.1f°C → %d개 Observer에게 알림\n", 
           subject->temperature, subject->observer_count);
    
    // 등록된 모든 Observer의 update() 함수 포인터 호출
    for (int i = 0; i < subject->observer_count; i++) {
        if (subject->observers[i] && subject->observers[i]->update) {
            // 핵심: 각 Observer의 update() 호출 (다형성)
            // self: Observer 자신의 포인터, data: 온도 데이터 포인터
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
 * ============================================================================
 * 
 * C에서 상속 구현 방법:
 *   - 첫 번째 멤버로 '기본 클래스' 구조체를 포함
 *   - 이를 통해 포인터 캐스팅으로 다형성 구현
 * 
 * 예시:
 *   DisplayObserver* display = ...;
 *   Observer* base = (Observer*)display;  // 업캐스팅
 *   base->update(...)  // 실제로는 Display_Update() 호출됨
 * 
 * ============================================================================ */

/* ----------------------------------------------------------------------------
 * DisplayObserver: 화면 표시용 Observer
 * ----------------------------------------------------------------------------
 * 
 * 메모리 구조 (상속 구현):
 *   +------------------+  ← DisplayObserver 시작 주소
 *   | Observer base    |  40 bytes (update 함수포인터 + name)
 *   |   - update       |
 *   |   - name[32]     |
 *   +------------------+  ← 여기까지는 Observer와 동일 구조
 *   | display_id       |  4 bytes (추가 멤버)
 *   +------------------+
 *   총 44 bytes
 * 
 * 핵심 개념:
 *   - DisplayObserver를 Observer*로 캐스팅 가능
 *   - Subject는 Observer*로만 다루므로 구체 타입을 몰라도 됨
 *   - update 함수 포인터로 Display_Update()를 호출
 * 
 * ---------------------------------------------------------------------------- */
typedef struct {
    Observer base;      // 첫 번째 멤버: 기본 Observer (상속)
    int display_id;     // 추가 멤버: 디스플레이 ID
} DisplayObserver;

/* ----------------------------------------------------------------------------
 * Display_Update: DisplayObserver의 update 구현
 * ----------------------------------------------------------------------------
 * 
 * 호출 경로:
 *   Subject_Notify() 
 *     → observers[i]->update(observers[i], &temperature)
 *       → Display_Update() 호출됨
 * 
 * 파라미터:
 *   - self: Observer* 타입이지만 실제로는 DisplayObserver*
 *   - data: void* 타입이지만 실제로는 float* (온도 데이터)
 * 
 * 처리 과정:
 *   1. self를 DisplayObserver*로 다운캐스팅 (실제 타입 복원)
 *   2. data를 float*로 캐스팅 (실제 데이터 타입 복원)
 *   3. 화면에 온도 표시 (여기서는 printf로 시뮬레이션)
 * 
 * ---------------------------------------------------------------------------- */
void Display_Update(Observer* self, void* data) {
    // 다운캐스팅: Observer* → DisplayObserver*
    DisplayObserver* display = (DisplayObserver*)self;
    
    // 데이터 타입 복원: void* → float*
    float* temp = (float*)data;
    
    // 실제 처리: 화면에 온도 표시
    printf("  → [Display %d] 화면 업데이트: %.1f°C\n", display->display_id, *temp);
}

/* ----------------------------------------------------------------------------
 * DisplayObserver_Create: DisplayObserver 생성 함수
 * ----------------------------------------------------------------------------
 * 
 * 생성 과정:
 *   1. 메모리 할당 (malloc)
 *   2. base.update 함수 포인터 설정 (핵심!)
 *   3. base.name 설정 (Observer 식별용)
 *   4. display_id 설정 (DisplayObserver만의 추가 데이터)
 * 
 * 반환:
 *   - DisplayObserver* 타입으로 반환
 *   - 사용할 때 (Observer*)로 캐스팅하여 Subject에 등록
 * 
 * ---------------------------------------------------------------------------- */
DisplayObserver* DisplayObserver_Create(int id) {
    // 1. 메모리 할당
    DisplayObserver* observer = (DisplayObserver*)malloc(sizeof(DisplayObserver));
    
    // 2. update 함수 포인터 설정 (핵심: 이 함수가 호출될 것)
    observer->base.update = Display_Update;
    
    // 3. Observer 이름 설정
    snprintf(observer->base.name, sizeof(observer->base.name), "Display%d", id);
    
    // 4. DisplayObserver만의 멤버 설정
    observer->display_id = id;
    
    return observer;
}

/* ----------------------------------------------------------------------------
 * LoggerObserver: 로그 기록용 Observer
 * ----------------------------------------------------------------------------
 * 
 * 메모리 구조:
 *   +------------------+
 *   | Observer base    |  40 bytes
 *   +------------------+
 *   | FILE* log_file   |  8 bytes (파일 포인터)
 *   +------------------+
 *   총 48 bytes
 * 
 * 역할:
 *   - 온도 변화를 파일에 기록
 *   - 타임스탬프와 함께 로그 저장
 * 
 * ---------------------------------------------------------------------------- */
typedef struct {
    Observer base;      // 기본 Observer
    FILE* log_file;     // 로그 파일 포인터 (추가 멤버)
} LoggerObserver;

/* ----------------------------------------------------------------------------
 * Logger_Update: 온도를 파일에 기록
 * ---------------------------------------------------------------------------- */
void Logger_Update(Observer* self, void* data) {
    // 다운캐스팅
    LoggerObserver* logger = (LoggerObserver*)self;
    float* temp = (float*)data;
    
    // 타임스탬프 생성
    time_t now = time(NULL);
    
    // 콘솔 출력
    printf("  → [Logger] 로그 기록: %.1f°C\n", *temp);
    
    // 파일에 기록 (실제 로깅)
    if (logger->log_file) {
        fprintf(logger->log_file, "[%ld] Temperature: %.1f°C\n", now, *temp);
        fflush(logger->log_file);  // 즉시 디스크에 쓰기
    }
}

/* ----------------------------------------------------------------------------
 * LoggerObserver_Create: 파일을 열고 LoggerObserver 생성
 * ---------------------------------------------------------------------------- */
LoggerObserver* LoggerObserver_Create(const char* filename) {
    LoggerObserver* observer = (LoggerObserver*)malloc(sizeof(LoggerObserver));
    
    // update 함수 포인터 설정
    observer->base.update = Logger_Update;
    snprintf(observer->base.name, sizeof(observer->base.name), "Logger");
    
    // 로그 파일 열기 (append 모드)
    observer->log_file = fopen(filename, "a");
    
    return observer;
}

/* ----------------------------------------------------------------------------
 * AlarmObserver: 임계값 체크용 Observer
 * ----------------------------------------------------------------------------
 * 
 * 메모리 구조:
 *   +------------------+
 *   | Observer base    |  40 bytes
 *   +------------------+
 *   | float threshold  |  4 bytes (임계값)
 *   +------------------+
 *   총 44 bytes
 * 
 * 역할:
 *   - 온도가 설정된 임계값을 초과하면 경고 발생
 *   - 조건부 처리의 예시 (Observer마다 다른 로직 가능)
 * 
 * ---------------------------------------------------------------------------- */
typedef struct {
    Observer base;      // 기본 Observer
    float threshold;    // 경고 임계값 (추가 멤버)
} AlarmObserver;

/* ----------------------------------------------------------------------------
 * Alarm_Update: 임계값 체크 및 경고
 * 
 * 특징:
 *   - 다른 Observer와 달리 조건부 처리
 *   - 같은 update() 호출이지만 각자 다른 동작 (다형성)
 * 
 * ---------------------------------------------------------------------------- */
void Alarm_Update(Observer* self, void* data) {
    // 다운캐스팅
    AlarmObserver* alarm = (AlarmObserver*)self;
    float* temp = (float*)data;
    
    // 임계값 체크 (조건부 처리)
    if (*temp > alarm->threshold) {
        printf("  → [Alarm] ⚠️  경고! 온도가 임계값(%.1f°C)을 초과했습니다: %.1f°C\n", 
               alarm->threshold, *temp);
    } else {
        printf("  → [Alarm] 온도 정상: %.1f°C\n", *temp);
    }
}

/* ----------------------------------------------------------------------------
 * AlarmObserver_Create: 임계값을 설정하고 AlarmObserver 생성
 * ---------------------------------------------------------------------------- */
AlarmObserver* AlarmObserver_Create(float threshold) {
    AlarmObserver* observer = (AlarmObserver*)malloc(sizeof(AlarmObserver));
    
    // update 함수 포인터 설정
    observer->base.update = Alarm_Update;
    snprintf(observer->base.name, sizeof(observer->base.name), "Alarm");
    
    // 임계값 설정
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
    
    // 1. Subject(감시대상) 생성 및 초기화: 온도 센서를 감시할 주체(Subject) 준비
    TemperatureSubject temp_subject;
    Subject_Init(&temp_subject);  // 내부적으로 Observer 리스트 초기화

    printf("\n=== Observer 등록 ===\n");
    // 2. Observer(관찰자) 객체들 생성
    //    각 Observer는 온도 변화를 다르게 처리
    DisplayObserver* display1 = DisplayObserver_Create(1); // 디스플레이 패널 1번
    DisplayObserver* display2 = DisplayObserver_Create(2); // 디스플레이 패널 2번
    LoggerObserver* logger = LoggerObserver_Create("temp_log.txt"); // 파일로 기록하는 로거
    AlarmObserver* alarm = AlarmObserver_Create(30.0f); // 임계값 설정(30도 이상시 경고)

    // 3. Observer들을 Subject에 등록(Attach)
    //    온도 변동이 있을 때 각 Observer의 update()가 자동 호출됨
    Subject_Attach(&temp_subject, (Observer*)display1);
    Subject_Attach(&temp_subject, (Observer*)display2);
    Subject_Attach(&temp_subject, (Observer*)logger);
    Subject_Attach(&temp_subject, (Observer*)alarm);

    // 4. 온도 변경 이벤트 시뮬레이션
    printf("\n=== 온도 변경 이벤트 ===\n");
    Subject_SetTemperature(&temp_subject, 25.0f); // 각 Observer에 25도 전달 및 알림

    printf("\n");
    Subject_SetTemperature(&temp_subject, 28.5f); // 28.5도 알림

    printf("\n");
    Subject_SetTemperature(&temp_subject, 32.0f);  // 32도 알림, Alarm에서 임계값 경고 발생

    // 5. Observer 동적 제거
    printf("\n=== Observer 제거 ===\n");
    Subject_Detach(&temp_subject, (Observer*)display2); // 디스플레이2 해제

    printf("\n");
    Subject_SetTemperature(&temp_subject, 27.0f); // 나머지 Observer만 알림 수신

    // 6. 리소스 해제 (메모리 정리)
    free(display1);
    free(display2);
    if (logger->log_file) fclose(logger->log_file); // 파일 닫기
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
 *    - Subject는 Observer의 구체적인 타입을 몰라도 됨
 *    - Observer* 포인터만으로 처리
 * 
 * 2. 런타임에 동적으로 Observer 추가/제거
 *    - Attach/Detach로 구독 관리
 *    - 실행 중 필요에 따라 구독자 변경 가능
 * 
 * 3. 일대다 관계를 쉽게 구현
 *    - 하나의 Subject가 여러 Observer에게 알림
 *    - 브로드캐스트 통신
 * 
 * 4. 확장성
 *    - 새로운 Observer 타입 추가가 쉬움
 *    - Subject 코드 수정 없이 Observer만 추가
 * 
 * ============================================================================
 * 직접 작성할 때 체크리스트
 * ============================================================================
 * 
 * [1단계] Observer 인터페이스 정의
 *   □ update 함수 포인터 포함
 *   □ 필요한 공통 데이터 추가 (예: name)
 * 
 * [2단계] Subject 구조체 정의
 *   □ Observer 포인터 배열 (구독자 리스트)
 *   □ observer_count (현재 등록 개수)
 *   □ 관찰 대상 상태 데이터 (예: temperature)
 * 
 * [3단계] Subject 함수 작성
 *   □ Init: 초기화
 *   □ Attach: Observer 등록
 *   □ Detach: Observer 제거
 *   □ Notify: 모든 Observer의 update() 호출
 *   □ SetState: 상태 변경 + Notify 호출
 * 
 * [4단계] 구체적인 Observer 구현
 *   □ 첫 번째 멤버로 Observer base 포함 (상속)
 *   □ 추가 멤버 정의 (각 Observer만의 데이터)
 *   □ Update 함수 구현 (실제 동작)
 *   □ Create 함수 구현 (메모리 할당 + update 함수 포인터 설정)
 * 
 * [5단계] 사용
 *   □ Subject 생성 및 Init
 *   □ Observer들 Create
 *   □ Attach로 Subject에 등록
 *   □ 상태 변경시 자동 알림 확인
 *   □ 필요시 Detach
 *   □ 메모리 해제
 * 
 * ============================================================================
 * 핵심 포인트
 * ============================================================================
 * 
 * 1. 함수 포인터를 통한 다형성
 *    - C++의 가상 함수와 같은 효과
 *    - observer->update()가 각 Observer의 구체 함수 호출
 * 
 * 2. 구조체 상속 구현
 *    - 첫 번째 멤버로 기본 구조체 포함
 *    - 포인터 캐스팅으로 업캐스팅/다운캐스팅
 * 
 * 3. void* 포인터 활용
 *    - 다양한 타입의 데이터 전달
 *    - 사용할 때 적절한 타입으로 캐스팅 필요
 * 
 * 4. 메모리 관리 주의
 *    - malloc으로 할당한 Observer는 반드시 free
 *    - FILE* 같은 리소스도 close 필요
 * 
 * ============================================================================
 * 질문 대응
 * ============================================================================
 * 
 * Q: Observer 패턴을 사용한 경험은?
 * A: "센서 데이터를 여러 모듈(UI, 로거, 알람)이 동시에 사용해야 해서
 *     Observer 패턴으로 구현했습니다. 센서 모듈은 구독자가 누구인지
 *     몰라도 되고, 런타임에 새 구독자를 추가할 수 있어 유연했습니다."
 * 
 * ============================================================================
 */
