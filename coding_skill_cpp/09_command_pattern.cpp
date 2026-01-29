/* C++ Command - 람다 명령 */
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
using namespace std;

struct Command {
    function<void()> execute;
    function<void()> undo;
};

class Invoker {
    vector<Command> history;
public:
    void execute(Command cmd) {
        cmd.execute();
        history.push_back(cmd);
    }
    
    void undo() {
        if (!history.empty()) {
            history.back().undo();
            history.pop_back();
        }
    }
};

int main() {
    cout << "\n=== C++ Command ===" << endl;
    int value = 0;
    
    Invoker invoker;
    invoker.execute({
        [&]() { value++; cout << "Value++ = " << value << endl; },
        [&]() { value--; cout << "Value-- = " << value << endl; }
    });
    
    invoker.execute({
        [&]() { value += 10; cout << "Value+10 = " << value << endl; },
        [&]() { value -= 10; cout << "Value-10 = " << value << endl; }
    });
    
    invoker.undo();
    invoker.undo();
    
    return 0;
}
