/* C++ Driver Interface */
#include <iostream>
using namespace std;

class IDriver {
public:
    virtual ~IDriver() = default;
    virtual int open() = 0;
    virtual int close() = 0;
    virtual int read(uint8_t* buf, size_t len) = 0;
    virtual int write(const uint8_t* buf, size_t len) = 0;
};

class UARTDriver : public IDriver {
public:
    int open() override { cout << "[UART] Open" << endl; return 0; }
    int close() override { cout << "[UART] Close" << endl; return 0; }
    int read(uint8_t* buf, size_t len) override { return len; }
    int write(const uint8_t* buf, size_t len) override { return len; }
};

int main() {
    cout << "=== C++ Driver Interface ===" << endl;
    UARTDriver uart;
    uart.open();
    uart.close();
    return 0;
}
