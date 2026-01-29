/* C++ Observer - vector<function> */
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

template<typename T>
class Subject {
    vector<function<void(const T&)>> observers;
    T data;
    
public:
    void attach(function<void(const T&)> obs) {
        observers.push_back(obs);
    }
    
    void setData(const T& newData) {
        data = newData;
        notify();
    }
    
    void notify() {
        cout << "[Subject] 통지: " << observers.size() << "명" << endl;
        for (auto& obs : observers) {
            obs(data);
        }
    }
};

int main() {
    cout << "\n=== C++ Observer Pattern ===" << endl;
    
    Subject<int> subject;
    
    subject.attach([](const int& val) {
        cout << "  → Observer 1: " << val << endl;
    });
    
    subject.attach([](const int& val) {
        cout << "  → Observer 2: " << val * 2 << endl;
    });
    
    subject.setData(10);
    subject.setData(20);
    
    return 0;
}
