/* C++ Mutex Guard - lock_guard */
#include <iostream>
#include <mutex>
using namespace std;

mutex mtx;

void critical_section() {
    lock_guard<mutex> guard(mtx);  // 자동 lock
    cout << "[Critical] 보호된 영역" << endl;
}  // 자동 unlock

int main() {
    cout << "=== C++ Mutex Guard ===" << endl;
    critical_section();
    cout << "자동 unlock 완료!" << endl;
    return 0;
}
