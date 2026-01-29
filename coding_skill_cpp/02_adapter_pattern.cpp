/*
 * C++ Adapter Pattern - 다중 상속과 템플릿
 * C 버전: 구조체 + 함수 포인터
 * C++ 버전: 다중 상속, 템플릿, 추상 클래스
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace patterns {

// 타겟 인터페이스
class ICelsiusSensor {
public:
    virtual ~ICelsiusSensor() = default;
    virtual float readCelsius() = 0;
    virtual string getName() const = 0;
};

// Adaptee 1: 화씨 센서
class FahrenheitSensor {
public:
    float readFahrenheit() { return 77.0f; }
    string getSensorName() { return "Fahrenheit Sensor"; }
};

// Adaptee 2: 켈빈 센서
class KelvinSensor {
public:
    float getKelvin() { return 300.0f; }
    string id() { return "Kelvin Sensor"; }
};

// Adapter 1: 화씨 → 섭씨
class FahrenheitAdapter : public ICelsiusSensor {
    unique_ptr<FahrenheitSensor> sensor;
public:
    FahrenheitAdapter() : sensor(make_unique<FahrenheitSensor>()) {}
    
    float readCelsius() override {
        float f = sensor->readFahrenheit();
        return (f - 32.0f) * 5.0f / 9.0f;
    }
    
    string getName() const override { return sensor->getSensorName(); }
};

// Adapter 2: 켈빈 → 섭씨
class KelvinAdapter : public ICelsiusSensor {
    unique_ptr<KelvinSensor> sensor;
public:
    KelvinAdapter() : sensor(make_unique<KelvinSensor>()) {}
    
    float readCelsius() override {
        return sensor->getKelvin() - 273.15f;
    }
    
    string getName() const override { return sensor->id(); }
};

// 템플릿 Adapter
template<typename T, typename Converter>
class GenericAdapter : public ICelsiusSensor {
    unique_ptr<T> sensor;
    Converter converter;
    string name;
    
public:
    GenericAdapter(const string& name) : sensor(make_unique<T>()), name(name) {}
    
    float readCelsius() override {
        return converter(*sensor);
    }
    
    string getName() const override { return name; }
};

// 클라이언트 코드
void processTemperature(ICelsiusSensor& sensor) {
    cout << sensor.getName() << ": " << sensor.readCelsius() << "°C" << endl;
}

} // namespace patterns

int main() {
    using namespace patterns;
    
    cout << "\n=== C++ Adapter Pattern ===" << endl;
    
    FahrenheitAdapter f_adapter;
    KelvinAdapter k_adapter;
    
    processTemperature(f_adapter);
    processTemperature(k_adapter);
    
    cout << "\n다형성으로 통일된 인터페이스 사용!" << endl;
    
    return 0;
}
