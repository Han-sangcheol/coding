/* C++ Semaphore - std::counting_semaphore (C++20) */
#include <iostream>
using namespace std;

// 간단한 Semaphore 구현
class Semaphore {
    int count;
public:
    Semaphore(int max) : count(max) {}
    bool wait() { if (count > 0) { count--; return true; } return false; }
    void signal() { count++; }
    int getCount() const { return count; }
};

int main() {
    cout << "=== C++ Semaphore ===" << endl;
    Semaphore sem(2);
    
    sem.wait();
    cout << "Count: " << sem.getCount() << endl;
    sem.signal();
    cout << "Count: " << sem.getCount() << endl;
    
    return 0;
}
