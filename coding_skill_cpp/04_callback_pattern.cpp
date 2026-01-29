/* C++ Callback - std::function과 람다 */
#include <iostream>
#include <functional>
#include <vector>
using namespace std;

// 콜백 타입 정의
using Callback = function<void(int)>;
using DataCallback = function<void(const string&)>;

class Button {
    Callback onClick;
public:
    void setOnClick(Callback cb) { onClick = cb; }
    void click() {
        cout << "[Button] 클릭!" << endl;
        if (onClick) onClick(1);
    }
};

class Observable {
    vector<Callback> observers;
public:
    void attach(Callback cb) { observers.push_back(cb); }
    void notify(int data) {
        cout << "[Observable] 통지: " << observers.size() << "명" << endl;
        for (auto& obs : observers) obs(data);
    }
};

int main() {
    cout << "\n=== C++ Callback Pattern ===" << endl;
    
    // 람다 콜백
    Button btn;
    btn.setOnClick([](int x) { cout << "  → 콜백 호출: " << x << endl; });
    btn.click();
    
    // 다중 콜백
    Observable obs;
    obs.attach([](int x) { cout << "  → Observer 1: " << x << endl; });
    obs.attach([](int x) { cout << "  → Observer 2: " << x * 2 << endl; });
    obs.notify(10);
    
    return 0;
}
