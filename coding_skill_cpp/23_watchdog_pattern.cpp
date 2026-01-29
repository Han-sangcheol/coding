/* C++ Watchdog - chrono */
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

class Watchdog {
    time_point<steady_clock> lastKick;
    int timeoutSec;
public:
    Watchdog(int timeout) : timeoutSec(timeout) { kick(); }
    void kick() { lastKick = steady_clock::now(); }
    bool check() {
        auto elapsed = duration_cast<seconds>(steady_clock::now() - lastKick).count();
        return elapsed < timeoutSec;
    }
};

int main() {
    cout << "=== C++ Watchdog ===" << endl;
    Watchdog wd(3);
    cout << "Status: " << (wd.check() ? "OK" : "Timeout") << endl;
    return 0;
}
