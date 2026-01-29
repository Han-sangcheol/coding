/* C++ Interrupt Handler - atomic */
#include <iostream>
#include <atomic>
using namespace std;

atomic<bool> buttonPressed{false};

void GPIO_ISR() {
    buttonPressed = true;  // atomic operation
    cout << "[ISR] Flag set" << endl;
}

void MainLoop() {
    if (buttonPressed.exchange(false)) {
        cout << "[MainLoop] Processing event" << endl;
    }
}

int main() {
    cout << "=== C++ Interrupt Handler ===" << endl;
    GPIO_ISR();
    MainLoop();
    return 0;
}
