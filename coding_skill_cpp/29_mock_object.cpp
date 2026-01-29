/* C++ Mock Object */
#include <iostream>
using namespace std;

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual float read() = 0;
};

class RealSensor : public ISensor {
public:
    float read() override {
        cout << "[Real] Hardware read" << endl;
        return 25.5f;
    }
};

class MockSensor : public ISensor {
public:
    float read() override {
        cout << "[Mock] Test data" << endl;
        return 20.0f;
    }
};

void processTemperature(ISensor& sensor) {
    float temp = sensor.read();
    cout << "[App] Temperature: " << temp << "°C" << endl;
}

int main() {
    cout << "=== C++ Mock Object ===" << endl;
    
    RealSensor real;
    MockSensor mock;
    
    processTemperature(real);
    processTemperature(mock);
    
    return 0;
}
