/* C++ Tracing - RAII 기반 */
#include <iostream>
#include <string>
using namespace std;

class FunctionTracer {
    string name;
    static int depth;
public:
    FunctionTracer(const string& fname) : name(fname) {
        cout << string(depth++, ' ') << "→ " << name << "()" << endl;
    }
    
    ~FunctionTracer() {
        cout << string(--depth, ' ') << "← " << name << "()" << endl;
    }
};

int FunctionTracer::depth = 0;

#define TRACE() FunctionTracer __tracer__(__func__)

void functionC() {
    TRACE();
    cout << "      FunctionC 실행" << endl;
}

void functionB() {
    TRACE();
    cout << "      FunctionB 실행" << endl;
    functionC();
}

void functionA() {
    TRACE();
    cout << "      FunctionA 실행" << endl;
    functionB();
}

int main() {
    cout << "=== C++ Tracing ===" << endl;
    functionA();
    return 0;
}
