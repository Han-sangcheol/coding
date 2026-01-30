/*
 * ============================================================================
 * 파일명: 03_singleton_pattern.c
 * 기능: C언어 Singleton(싱글톤) 패턴 구현 예제
 * 
 * ============================================================================
 * 1️⃣ Singleton 패턴이란?
 * ============================================================================
 * 
 * 프로그램 전체에서 단 하나의 인스턴스만 존재하도록 보장하는 패턴
 * 
 * 핵심 키워드:
 *   - 단 하나의 인스턴스 (Only One Instance)
 *   - 전역 접근 (Global Access)
 *   - 지연 초기화 (Lazy Initialization)
 * 
 * 구조:
 *   [여러 모듈들]
 *        ↓ ↓ ↓
 *   [Singleton 객체] ← 모두가 같은 인스턴스 사용
 *        ↓
 *   [공유 리소스]
 * 
 * ============================================================================
 * 2️⃣ 왜 Singleton을 쓰나?
 * ============================================================================
 * 
 * C 언어에서는 전역 변수로도 가능하지만, Singleton이 더 나은 이유:
 * 
 * ❌ 전역 변수의 문제점:
 *    - 초기화 시점 제어 어려움
 *    - 중복 초기화 가능
 *    - 네임스페이스 오염
 *    - 의존성 숨김
 * 
 * ✅ Singleton의 장점:
 *    - 초기화 시점 제어 가능
 *    - 하나의 인스턴스만 보장
 *    - 명시적 접근
 *    - 필요할 때만 생성 (Lazy Init)
 *    - 멀티스레드 안전성 추가 가능
 * 
 * ============================================================================
 * 3️⃣ 임베디드에서 Singleton을 쓰는 경우
 * ============================================================================
 * 
 * 실무 적용 사례:
 *   ✅ 설정 관리자 (Config Manager)
 *   ✅ 로거 (Logger)
 *   ✅ GPIO 컨트롤러
 *   ✅ UART 드라이버 인스턴스
 *   ✅ 시스템 상태 관리자
 *   ✅ 에러 핸들러
 *   ✅ 타이머 관리자
 *   ✅ 메모리 풀 관리자
 * 
 * 👉 "하드웨어는 하나"인 경우 대부분 Singleton
 * 
 * ============================================================================
 * 4️⃣ C 언어에서 Singleton 구현 방법
 * ============================================================================
 * 
 * 방법 1: Static 변수 + Getter 함수
 * 
 *    typedef struct {
 *        int value;
 *    } Config;
 * 
 *    Config* GetConfig(void) {
 *        static Config instance = {0};  // 단 한 번만 초기화
 *        return &instance;
 *    }
 * 
 * 방법 2: Static 변수 + 초기화 플래그
 * 
 *    static Config instance;
 *    static int initialized = 0;
 * 
 *    Config* GetConfig(void) {
 *        if (!initialized) {
 *            InitConfig(&instance);
 *            initialized = 1;
 *        }
 *        return &instance;
 *    }
 * 
 * ============================================================================
 * 5️⃣ Singleton vs 전역 변수 비교
 * ============================================================================
 * 
 * 구분            | 전역 변수      | Singleton
 * ----------------|----------------|------------------
 * 초기화 시점     | 시작 시        | 첫 사용 시 가능
 * 중복 방지       | ❌             | ✅
 * 접근 제어       | 어려움         | 가능
 * 네임스페이스    | 오염           | 깔끔
 * RTOS 안전성     | 추가 작업 필요 | 구현 가능
 * 
 * ============================================================================
 * 6️⃣ Singleton 구현 시 주의사항
 * ============================================================================
 * 
 * ❌ 피해야 할 것:
 *    - 너무 많은 책임 부여 (God Object)
 *    - 멀티스레드 환경에서 동기화 누락
 *    - 테스트 어려움 (의존성 주입 고려)
 * 
 * ✅ 해야 할 것:
 *    - 단일 책임 원칙 준수
 *    - RTOS 환경에서는 Mutex 사용
 *    - 초기화 실패 처리
 *    - 명확한 인터페이스 제공
 * 
 * ============================================================================
 * 7️⃣ RTOS 환경에서의 Singleton (중요)
 * ============================================================================
 * 
 * 멀티태스크 환경에서는 동기화 필수:
 * 
 *    static Config instance;
 *    static int initialized = 0;
 *    static osMutexId_t config_mutex;
 * 
 *    Config* GetConfig(void) {
 *        osMutexAcquire(config_mutex, osWaitForever);
 *        if (!initialized) {
 *            InitConfig(&instance);
 *            initialized = 1;
 *        }
 *        osMutexRelease(config_mutex);
 *        return &instance;
 *    }
 * 
 * ============================================================================
 * 8️⃣ 면접에서 이렇게 말하면 강력함
 * ============================================================================
 * 
 * "Singleton은 전역 변수와 달리 초기화 시점을 제어할 수 있고,
 *  하나의 인스턴스만 보장할 수 있어서 임베디드 시스템의
 *  하드웨어 리소스 관리에 적합합니다. 특히 RTOS 환경에서는
 *  Mutex를 사용해 스레드 안전성을 확보했습니다."
 * 
 * ============================================================================
 * 🔚 한 줄 요약
 * ============================================================================
 * 
 * Singleton 패턴은
 * "전역적으로 하나의 인스턴스만 존재하도록 보장하는 기술"이다.
 * 
 * ============================================================================
 * 이 예제 파일의 구조
 * ============================================================================
 * 1. 로깅 시스템 (Logger Singleton)
 * 2. 설정 관리자 (Config Manager Singleton)
 * 3. 시스템 상태 모니터 (System Monitor Singleton)
 * 4. 사용 예제 (main)
 * 5. 실행 결과 확인
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

/* ============================================================================
 * 1. Logger Singleton - 로깅 시스템
 * ============================================================================ */

// 로그 레벨 정의
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL
} LogLevel;

// Logger 구조체
typedef struct {
    LogLevel min_level;        // 최소 로그 레벨
    int log_count;             // 로그 카운터
    FILE* log_file;            // 로그 파일 포인터
    char log_filename[256];    // 로그 파일명
    int console_output;        // 콘솔 출력 여부
} Logger;

// Logger Singleton 인스턴스 (static으로 숨김)
static Logger logger_instance = {
    .min_level = LOG_LEVEL_INFO,
    .log_count = 0,
    .log_file = NULL,
    .console_output = 1
};

// Logger 초기화 플래그
static int logger_initialized = 0;

// Logger 초기화 함수
int Logger_Init(const char* filename, LogLevel min_level) {
    if (logger_initialized) {
        printf("[Logger] 이미 초기화되었습니다.\n");
        return 0;  // 이미 초기화됨
    }
    
    logger_instance.min_level = min_level;
    logger_instance.log_count = 0;
    logger_instance.console_output = 1;
    
    if (filename != NULL) {
        strncpy(logger_instance.log_filename, filename, sizeof(logger_instance.log_filename) - 1);
        logger_instance.log_file = fopen(filename, "a");
        if (logger_instance.log_file == NULL) {
            printf("[Logger] 로그 파일 열기 실패: %s\n", filename);
            return -1;
        }
        printf("[Logger] 로그 파일 초기화 성공: %s\n", filename);
    }
    
    logger_initialized = 1;
    printf("[Logger] Singleton 초기화 완료\n");
    return 0;
}

// Logger 인스턴스 가져오기 (Singleton 접근자)
Logger* Logger_GetInstance(void) {
    if (!logger_initialized) {
        Logger_Init(NULL, LOG_LEVEL_INFO);  // 기본 설정으로 초기화
    }
    return &logger_instance;
}

// 로그 레벨을 문자열로 변환
const char* Logger_LevelToString(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG:    return "DEBUG";
        case LOG_LEVEL_INFO:     return "INFO";
        case LOG_LEVEL_WARNING:  return "WARNING";
        case LOG_LEVEL_ERROR:    return "ERROR";
        case LOG_LEVEL_CRITICAL: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

// 로그 메시지 출력
void Logger_Log(LogLevel level, const char* tag, const char* format, ...) {
    Logger* logger = Logger_GetInstance();
    
    // 최소 레벨보다 낮으면 무시
    if (level < logger->min_level) {
        return;
    }
    
    // 시간 정보
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char time_buffer[64];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    // 가변 인자 처리
    va_list args;
    va_start(args, format);
    
    // 로그 메시지 생성
    char message[1024];
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    
    // 콘솔 출력
    if (logger->console_output) {
        printf("[%s] [%s] [%s] %s\n", 
               time_buffer, 
               Logger_LevelToString(level), 
               tag, 
               message);
    }
    
    // 파일 출력
    if (logger->log_file != NULL) {
        fprintf(logger->log_file, "[%s] [%s] [%s] %s\n", 
                time_buffer, 
                Logger_LevelToString(level), 
                tag, 
                message);
        fflush(logger->log_file);
    }
    
    logger->log_count++;
}

// Logger 종료
void Logger_Shutdown(void) {
    Logger* logger = Logger_GetInstance();
    
    if (logger->log_file != NULL) {
        fclose(logger->log_file);
        logger->log_file = NULL;
        printf("[Logger] 로그 파일 닫기 완료\n");
    }
    
    printf("[Logger] 총 로그 수: %d\n", logger->log_count);
    logger_initialized = 0;
}

/* ============================================================================
 * 2. Config Manager Singleton - 설정 관리자
 * ============================================================================ */

// 설정 항목 구조체
typedef struct {
    char device_name[64];
    int device_id;
    int baud_rate;
    int timeout_ms;
    int retry_count;
    float threshold_value;
} Config;

// Config Manager 구조체
typedef struct {
    Config config;
    int is_loaded;
    char config_file[256];
} ConfigManager;

// ConfigManager Singleton 인스턴스
static ConfigManager config_manager_instance;
static int config_manager_initialized = 0;

// ConfigManager 초기화
int ConfigManager_Init(void) {
    if (config_manager_initialized) {
        printf("[ConfigManager] 이미 초기화되었습니다.\n");
        return 0;
    }
    
    // 기본 설정값
    strncpy(config_manager_instance.config.device_name, "EmbeddedDevice", 63);
    config_manager_instance.config.device_id = 1001;
    config_manager_instance.config.baud_rate = 115200;
    config_manager_instance.config.timeout_ms = 1000;
    config_manager_instance.config.retry_count = 3;
    config_manager_instance.config.threshold_value = 25.5f;
    
    config_manager_instance.is_loaded = 1;
    
    config_manager_initialized = 1;
    printf("[ConfigManager] Singleton 초기화 완료\n");
    
    return 0;
}

// ConfigManager 인스턴스 가져오기
ConfigManager* ConfigManager_GetInstance(void) {
    if (!config_manager_initialized) {
        ConfigManager_Init();
    }
    return &config_manager_instance;
}

// 설정값 가져오기
Config* ConfigManager_GetConfig(void) {
    ConfigManager* cm = ConfigManager_GetInstance();
    return &cm->config;
}

// 설정값 출력
void ConfigManager_Print(void) {
    Config* config = ConfigManager_GetConfig();
    
    printf("\n=== 현재 시스템 설정 ===\n");
    printf("장치 이름: %s\n", config->device_name);
    printf("장치 ID: %d\n", config->device_id);
    printf("통신 속도: %d bps\n", config->baud_rate);
    printf("타임아웃: %d ms\n", config->timeout_ms);
    printf("재시도 횟수: %d\n", config->retry_count);
    printf("임계값: %.2f\n", config->threshold_value);
    printf("=======================\n\n");
}

// 설정값 업데이트
void ConfigManager_SetBaudRate(int baud_rate) {
    Config* config = ConfigManager_GetConfig();
    config->baud_rate = baud_rate;
    Logger_Log(LOG_LEVEL_INFO, "ConfigManager", "통신 속도 변경: %d bps", baud_rate);
}

void ConfigManager_SetTimeout(int timeout_ms) {
    Config* config = ConfigManager_GetConfig();
    config->timeout_ms = timeout_ms;
    Logger_Log(LOG_LEVEL_INFO, "ConfigManager", "타임아웃 변경: %d ms", timeout_ms);
}

/* ============================================================================
 * 3. System Monitor Singleton - 시스템 상태 모니터
 * ============================================================================ */

// 시스템 상태 구조체
typedef struct {
    int cpu_usage;           // CPU 사용률 (%)
    int memory_usage;        // 메모리 사용률 (%)
    int temperature;         // 온도 (°C)
    int uptime_seconds;      // 가동 시간 (초)
    int error_count;         // 에러 카운트
    time_t last_update;      // 마지막 업데이트 시간
} SystemStatus;

// SystemMonitor 구조체
typedef struct {
    SystemStatus status;
    int monitoring_enabled;
} SystemMonitor;

// SystemMonitor Singleton 인스턴스
static SystemMonitor system_monitor_instance;
static int system_monitor_initialized = 0;

// SystemMonitor 초기화
int SystemMonitor_Init(void) {
    if (system_monitor_initialized) {
        printf("[SystemMonitor] 이미 초기화되었습니다.\n");
        return 0;
    }
    
    system_monitor_instance.status.cpu_usage = 0;
    system_monitor_instance.status.memory_usage = 0;
    system_monitor_instance.status.temperature = 25;
    system_monitor_instance.status.uptime_seconds = 0;
    system_monitor_instance.status.error_count = 0;
    system_monitor_instance.status.last_update = time(NULL);
    system_monitor_instance.monitoring_enabled = 1;
    
    system_monitor_initialized = 1;
    printf("[SystemMonitor] Singleton 초기화 완료\n");
    
    return 0;
}

// SystemMonitor 인스턴스 가져오기
SystemMonitor* SystemMonitor_GetInstance(void) {
    if (!system_monitor_initialized) {
        SystemMonitor_Init();
    }
    return &system_monitor_instance;
}

// 시스템 상태 업데이트 (시뮬레이션)
void SystemMonitor_Update(void) {
    SystemMonitor* monitor = SystemMonitor_GetInstance();
    
    if (!monitor->monitoring_enabled) {
        return;
    }
    
    // 시뮬레이션: 랜덤 값 생성
    monitor->status.cpu_usage = 10 + (rand() % 70);
    monitor->status.memory_usage = 30 + (rand() % 50);
    monitor->status.temperature = 25 + (rand() % 40);
    monitor->status.uptime_seconds++;
    monitor->status.last_update = time(NULL);
    
    // 임계값 체크
    Config* config = ConfigManager_GetConfig();
    if (monitor->status.temperature > config->threshold_value) {
        monitor->status.error_count++;
        Logger_Log(LOG_LEVEL_WARNING, "SystemMonitor", 
                   "온도 경고: %d°C (임계값: %.2f°C)", 
                   monitor->status.temperature, 
                   config->threshold_value);
    }
}

// 시스템 상태 출력
void SystemMonitor_PrintStatus(void) {
    SystemMonitor* monitor = SystemMonitor_GetInstance();
    SystemStatus* status = &monitor->status;
    
    printf("\n=== 시스템 상태 ===\n");
    printf("CPU 사용률: %d%%\n", status->cpu_usage);
    printf("메모리 사용률: %d%%\n", status->memory_usage);
    printf("온도: %d°C\n", status->temperature);
    printf("가동 시간: %d초\n", status->uptime_seconds);
    printf("에러 카운트: %d\n", status->error_count);
    printf("===================\n\n");
}

/* ============================================================================
 * 사용 예제
 * ============================================================================ */

// 다른 모듈에서 Singleton 사용 시뮬레이션
void Module_A_Process(void) {
    Logger_Log(LOG_LEVEL_INFO, "Module_A", "작업 시작");
    
    Config* config = ConfigManager_GetConfig();
    Logger_Log(LOG_LEVEL_DEBUG, "Module_A", 
               "설정 확인: 장치 ID = %d", config->device_id);
    
    Logger_Log(LOG_LEVEL_INFO, "Module_A", "작업 완료");
}

void Module_B_Process(void) {
    Logger_Log(LOG_LEVEL_INFO, "Module_B", "데이터 처리 중");
    
    SystemMonitor_Update();
    
    Logger_Log(LOG_LEVEL_INFO, "Module_B", "데이터 처리 완료");
}

void Module_C_Process(void) {
    Logger_Log(LOG_LEVEL_INFO, "Module_C", "통신 시작");
    
    Config* config = ConfigManager_GetConfig();
    Logger_Log(LOG_LEVEL_DEBUG, "Module_C", 
               "통신 설정: %d bps, 타임아웃 %d ms", 
               config->baud_rate, config->timeout_ms);
    
    Logger_Log(LOG_LEVEL_INFO, "Module_C", "통신 완료");
}

/* ============================================================================
 * 메인 함수
 * ============================================================================ */

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("========================================\n");
    printf("Singleton 패턴 예제\n");
    printf("========================================\n\n");
    
    // 1. Logger 초기화
    printf("=== 1. Logger Singleton 초기화 ===\n");
    Logger_Init("system.log", LOG_LEVEL_DEBUG);
    
    // 중복 초기화 시도 (무시됨)
    Logger_Init("another.log", LOG_LEVEL_INFO);
    
    printf("\n");
    
    // 2. ConfigManager 사용
    printf("=== 2. ConfigManager Singleton 사용 ===\n");
    ConfigManager_Print();
    
    // 설정 변경
    ConfigManager_SetBaudRate(921600);
    ConfigManager_SetTimeout(2000);
    ConfigManager_Print();
    
    // 3. SystemMonitor 사용
    printf("=== 3. SystemMonitor Singleton 사용 ===\n");
    for (int i = 0; i < 5; i++) {
        SystemMonitor_Update();
    }
    SystemMonitor_PrintStatus();
    
    // 4. 여러 모듈에서 Singleton 사용
    printf("=== 4. 다중 모듈에서 Singleton 사용 ===\n");
    Module_A_Process();
    Module_B_Process();
    Module_C_Process();
    
    printf("\n");
    
    // 5. 최종 상태 확인
    printf("=== 5. 최종 시스템 상태 ===\n");
    SystemMonitor_PrintStatus();
    
    // 6. Singleton 인스턴스 동일성 확인
    printf("=== 6. Singleton 인스턴스 동일성 확인 ===\n");
    Logger* logger1 = Logger_GetInstance();
    Logger* logger2 = Logger_GetInstance();
    Logger* logger3 = Logger_GetInstance();
    
    printf("logger1 주소: %p\n", (void*)logger1);
    printf("logger2 주소: %p\n", (void*)logger2);
    printf("logger3 주소: %p\n", (void*)logger3);
    
    if (logger1 == logger2 && logger2 == logger3) {
        printf("✅ 모든 인스턴스가 동일합니다 (Singleton 보장)\n");
    } else {
        printf("❌ 인스턴스가 다릅니다 (Singleton 실패)\n");
    }
    
    printf("\n");
    
    // 7. 종료
    Logger_Shutdown();
    
    printf("\n========================================\n");
    printf("Singleton 패턴 예제 종료\n");
    printf("========================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * Singleton 패턴의 장점 총정리
 * ============================================================================
 * 
 * 1. 전역 접근 가능하면서도 네임스페이스 오염 방지
 * 2. 하나의 인스턴스만 보장
 * 3. 초기화 시점 제어 가능 (Lazy Initialization)
 * 4. 리소스 절약 (메모리, 파일 핸들 등)
 * 5. 공유 상태 관리 용이
 * 
 * ============================================================================
 * Singleton 패턴의 단점 및 주의사항
 * ============================================================================
 * 
 * 1. 전역 상태로 인한 테스트 어려움
 * 2. 멀티스레드 환경에서 동기화 필요
 * 3. 과도한 사용 시 결합도 증가
 * 4. 상태 변경 시 디버깅 어려움
 * 5. 의존성 숨김 (명시적이지 않음)
 * 
 * 👉 필요한 경우에만 신중하게 사용
 * 
 * ============================================================================
 * 임베디드 FW 개발에서 Singleton 패턴의 실전 활용
 * ============================================================================
 * 
 * 1. 로깅 시스템
 *    - 전역 로거 인스턴스
 *    - 모든 모듈에서 동일한 로그 파일 사용
 * 
 * 2. 설정 관리자
 *    - Flash에서 읽은 설정을 RAM에 캐싱
 *    - 모든 모듈이 동일한 설정 참조
 * 
 * 3. 하드웨어 드라이버
 *    - GPIO 컨트롤러
 *    - UART/SPI/I2C 드라이버 인스턴스
 *    - 하드웨어는 물리적으로 하나
 * 
 * 4. 시스템 모니터
 *    - CPU/메모리/온도 모니터링
 *    - 전역 시스템 상태 관리
 * 
 * 5. 에러 핸들러
 *    - 전역 에러 카운터
 *    - 에러 이력 관리
 * 
 * 6. 타이머 관리자
 *    - 시스템 틱 관리
 *    - 소프트웨어 타이머 스케줄링
 * 
 * 7. 통신 프로토콜 스택
 *    - CAN/Ethernet 스택 인스턴스
 *    - 단일 통신 채널 관리
 * 
 * ============================================================================
 * RTOS 환경에서의 Singleton (Thread-Safe)
 * ============================================================================
 * 
 * FreeRTOS 예제:
 * 
 *    #include "FreeRTOS.h"
 *    #include "semphr.h"
 * 
 *    static Logger logger_instance;
 *    static int logger_initialized = 0;
 *    static SemaphoreHandle_t logger_mutex = NULL;
 * 
 *    Logger* Logger_GetInstance(void) {
 *        if (logger_mutex == NULL) {
 *            logger_mutex = xSemaphoreCreateMutex();
 *        }
 *        
 *        xSemaphoreTake(logger_mutex, portMAX_DELAY);
 *        
 *        if (!logger_initialized) {
 *            Logger_Init_Internal(&logger_instance);
 *            logger_initialized = 1;
 *        }
 *        
 *        xSemaphoreGive(logger_mutex);
 *        
 *        return &logger_instance;
 *    }
 * 
 * ============================================================================
 * C와 C++의 Singleton 차이
 * ============================================================================
 * 
 * C++:
 *    class Logger {
 *    public:
 *        static Logger& GetInstance() {
 *            static Logger instance;  // Meyer's Singleton (Thread-safe in C++11)
 *            return instance;
 *        }
 *    private:
 *        Logger() {}  // 생성자 private
 *    };
 * 
 * C:
 *    - 명시적 함수 작성 필요
 *    - static 변수 + getter 함수 패턴
 *    - 초기화 플래그 수동 관리
 * 
 * ============================================================================
 * 면접 / 기술 면담 대응 포인트
 * ============================================================================
 * 
 * Q: Singleton 패턴을 사용한 경험이 있나요?
 * A: "네, 로깅 시스템과 설정 관리자를 Singleton으로 구현했습니다.
 *     모든 모듈이 동일한 로거 인스턴스를 사용하도록 하여 로그 파일의
 *     일관성을 보장했고, RTOS 환경에서는 Mutex를 사용해 스레드 안전성을
 *     확보했습니다."
 * 
 * Q: Singleton과 전역 변수의 차이는?
 * A: "전역 변수는 초기화 시점 제어가 어렵고 중복 초기화가 가능하지만,
 *     Singleton은 초기화 시점을 명시적으로 제어할 수 있고 하나의
 *     인스턴스만 보장합니다. 또한 getter 함수를 통한 접근으로
 *     더 명확한 의존성 표현이 가능합니다."
 * 
 * Q: Singleton의 단점과 대안은?
 * A: "Singleton은 전역 상태로 인한 테스트 어려움과 높은 결합도가
 *     단점입니다. 대안으로는 의존성 주입(DI)을 사용하거나, 필요한
 *     모듈에만 인스턴스를 전달하는 방식을 사용할 수 있습니다.
 *     하지만 임베디드 환경에서는 실용성을 고려해 신중하게 사용합니다."
 * 
 * Q: 멀티스레드 환경에서 Singleton을 어떻게 구현하나요?
 * A: "RTOS 환경에서는 Mutex나 Semaphore를 사용해 초기화 부분을
 *     임계 영역으로 보호합니다. Double-Checked Locking 패턴을 사용할
 *     수도 있지만, C에서는 메모리 배리어 이슈가 있어 단순하게
 *     Mutex로 전체를 보호하는 방식을 선호합니다."
 * 
 * ============================================================================
 * 실전 체크리스트
 * ============================================================================
 * 
 * Singleton을 사용하기 전에 확인:
 * 
 * □ 정말로 전역 인스턴스가 필요한가?
 * □ 여러 인스턴스가 문제를 일으키는가?
 * □ 초기화 시점 제어가 필요한가?
 * □ 멀티스레드 환경에서 안전한가?
 * □ 테스트 가능성을 고려했는가?
 * 
 * 👉 3개 이상 체크되면 Singleton 적용 고려
 * 
 * ============================================================================
 * 이 예제에서 배운 핵심 포인트
 * ============================================================================
 * 
 * 1. static 변수로 단일 인스턴스 보장
 * 2. 초기화 플래그로 중복 초기화 방지
 * 3. getter 함수로 전역 접근 제공
 * 4. 실무에서 자주 사용하는 3가지 Singleton (Logger, Config, Monitor)
 * 5. 포인터 비교로 인스턴스 동일성 확인
 * 
 * ============================================================================
 * 다음 학습 단계 추천
 * ============================================================================
 * 
 * 1. 04_callback_pattern.c - 함수 포인터 기반 이벤트 처리
 * 2. 05_state_machine.c - 상태 기반 시스템 설계
 * 3. RTOS Mutex를 사용한 Thread-Safe Singleton 구현
 * 4. Factory 패턴과 Singleton 조합
 * 5. 의존성 주입(DI)과 Singleton 비교
 * 
 * ============================================================================
 */
