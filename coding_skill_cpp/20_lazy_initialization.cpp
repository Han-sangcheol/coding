/* C++ Lazy Init - optional */
#include <iostream>
#include <optional>
using namespace std;

class LazyResource {
    optional<int> data;
public:
    int& get() {
        if (!data) {
            cout << "[Lazy] 첫 접근! 초기화" << endl;
            data = 100;
        }
        return *data;
    }
};

int main() {
    cout << "=== C++ Lazy Initialization ===" << endl;
    LazyResource res;
    cout << "1st access: " << res.get() << endl;
    cout << "2nd access: " << res.get() << endl;
    return 0;
}
