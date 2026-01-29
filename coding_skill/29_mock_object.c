/* Mock Object Pattern
 * 테스트용 가짜 객체
 */

#include <stdio.h>
#include <stdint.h>

// 실제 센서 인터페이스
typedef struct {
    float (*read)(void);
} Sensor;

// 실제 센서 구현
float RealSensor_Read(void) {
    printf("[RealSensor] 실제 하드웨어에서 읽기\n");
    return 25.5f;  // 실제 온도
}

// Mock 센서 구현 (테스트용)
float MockSensor_Read(void) {
    printf("[MockSensor] 시뮬레이션 데이터 반환\n");
    return 20.0f;  // 고정된 테스트 값
}

// 실제 센서
Sensor real_sensor = {
    .read = RealSensor_Read
};

// Mock 센서
Sensor mock_sensor = {
    .read = MockSensor_Read
};

// 테스트 대상 함수
void ProcessTemperature(Sensor* sensor) {
    float temp = sensor->read();
    printf("[App] 온도 처리: %.1f°C\n", temp);
    
    if (temp > 30.0f) {
        printf("[App] 경고: 온도 높음!\n");
    }
}

int main(void) {
    printf("=== Mock Object Pattern ===\n\n");
    
    printf("=== 실제 센서 사용 ===\n");
    ProcessTemperature(&real_sensor);
    
    printf("\n=== Mock 센서로 테스트 ===\n");
    ProcessTemperature(&mock_sensor);
    
    printf("\nMock Object: 테스트용 가짜 객체 패턴\n");
    printf("장점: 하드웨어 없이도 테스트 가능\n");
    return 0;
}
