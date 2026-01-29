/* C++ Event Queue - queue<function> */
#include <iostream>
#include <queue>
#include <functional>
using namespace std;

class EventQueue {
    queue<function<void()>> events;
public:
    void push(function<void()> event) {
        events.push(event);
        cout << "[Queue] 이벤트 추가 (크기: " << events.size() << ")" << endl;
    }
    
    void process() {
        while (!events.empty()) {
            cout << "[Queue] 이벤트 처리" << endl;
            events.front()();
            events.pop();
        }
    }
};

int main() {
    cout << "\n=== C++ Event Queue ===" << endl;
    
    EventQueue queue;
    
    queue.push([]() { cout << "  → Event 1" << endl; });
    queue.push([]() { cout << "  → Event 2" << endl; });
    queue.push([]() { cout << "  → Event 3" << endl; });
    
    queue.process();
    
    return 0;
}
