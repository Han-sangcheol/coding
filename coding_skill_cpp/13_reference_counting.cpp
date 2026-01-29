/* C++ Reference Counting - shared_ptr */
#include <iostream>
#include <memory>
using namespace std;

struct Buffer {
    Buffer() { cout << "[Buffer] 생성" << endl; }
    ~Buffer() { cout << "[Buffer] 소멸" << endl; }
    int data[100];
};

int main() {
    cout << "=== C++ Reference Counting ===" << endl;
    
    shared_ptr<Buffer> buf1 = make_shared<Buffer>();
    cout << "ref_count: " << buf1.use_count() << endl;
    
    {
        shared_ptr<Buffer> buf2 = buf1;
        cout << "ref_count: " << buf1.use_count() << endl;
    }
    
    cout << "ref_count: " << buf1.use_count() << endl;
    
    return 0;
}
