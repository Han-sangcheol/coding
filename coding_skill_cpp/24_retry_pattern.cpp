/* C++ Retry - 템플릿 함수 */
#include <iostream>
#include <functional>
using namespace std;

template<typename Func>
bool retry(Func func, int maxRetries) {
    for (int i = 1; i <= maxRetries; ++i) {
        try {
            func();
            return true;
        } catch (...) {
            cout << "[Retry] Attempt " << i << " failed" << endl;
        }
    }
    return false;
}

int main() {
    cout << "=== C++ Retry ===" << endl;
    int attempt = 0;
    
    retry([&attempt]() {
        attempt++;
        if (attempt < 3) throw runtime_error("Fail");
        cout << "[Success] Attempt " << attempt << endl;
    }, 5);
    
    return 0;
}
