/* C++ HAL - 추상 클래스 */
#include <iostream>
using namespace std;

class IGPIO {
public:
    virtual ~IGPIO() = default;
    virtual void write(int pin, bool value) = 0;
};

class STM32_GPIO : public IGPIO {
public:
    void write(int pin, bool value) override {
        cout << "[STM32] GPIO " << pin << " = " << value << endl;
    }
};

class Nordic_GPIO : public IGPIO {
public:
    void write(int pin, bool value) override {
        cout << "[Nordic] GPIO " << pin << " = " << value << endl;
    }
};

void app_toggle_led(IGPIO& hal) {
    hal.write(5, true);
}

int main() {
    cout << "=== C++ HAL ===" << endl;
    STM32_GPIO stm32;
    Nordic_GPIO nordic;
    
    app_toggle_led(stm32);
    app_toggle_led(nordic);
    
    return 0;
}
