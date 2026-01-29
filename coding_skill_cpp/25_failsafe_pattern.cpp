/* C++ Failsafe - enum class */
#include <iostream>
using namespace std;

enum class Mode { NORMAL, DEGRADED, SAFE };

class System {
    Mode mode = Mode::NORMAL;
    int errors = 0;
public:
    void reportError() {
        errors++;
        if (errors >= 3 && mode == Mode::NORMAL) {
            mode = Mode::DEGRADED;
            cout << "[Failsafe] → DEGRADED" << endl;
        }
    }
    
    void operate() {
        cout << "[System] Mode: " << static_cast<int>(mode) << endl;
    }
};

int main() {
    cout << "=== C++ Failsafe ===" << endl;
    System sys;
    for (int i = 0; i < 5; ++i) sys.reportError();
    sys.operate();
    return 0;
}
