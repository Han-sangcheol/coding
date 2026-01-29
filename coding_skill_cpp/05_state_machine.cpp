/* C++ State Machine - enum class + switch */
#include <iostream>
#include <string>
#include <map>
#include <functional>
using namespace std;

enum class State { IDLE, RUNNING, STOPPED };

class StateMachine {
    State current = State::IDLE;
    map<State, function<void()>> onEnter;
    
public:
    void setOnEnter(State s, function<void()> f) { onEnter[s] = f; }
    
    void transition(State next) {
        cout << "[SM] " << static_cast<int>(current) 
             << " → " << static_cast<int>(next) << endl;
        current = next;
        if (onEnter[current]) onEnter[current]();
    }
    
    State getState() const { return current; }
};

int main() {
    cout << "\n=== C++ State Machine ===" << endl;
    
    StateMachine sm;
    sm.setOnEnter(State::RUNNING, []() { cout << "  → 실행 시작!" << endl; });
    sm.setOnEnter(State::STOPPED, []() { cout << "  → 정지됨" << endl; });
    
    sm.transition(State::RUNNING);
    sm.transition(State::STOPPED);
    sm.transition(State::IDLE);
    
    return 0;
}
