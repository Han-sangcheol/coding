/* C++ Zero-Copy - move semantics */
#include <iostream>
#include <vector>
using namespace std;

class Buffer {
    vector<int> data;
public:
    Buffer(size_t size) : data(size) {
        cout << "[Buffer] 생성 (size: " << size << ")" << endl;
    }
    
    Buffer(Buffer&& other) noexcept : data(move(other.data)) {
        cout << "[Buffer] 이동 (복사 없음!)" << endl;
    }
    
    size_t size() const { return data.size(); }
};

Buffer createBuffer(size_t size) {
    return Buffer(size);  // RVO/이동
}

int main() {
    cout << "=== C++ Zero-Copy (move) ===" << endl;
    Buffer buf = createBuffer(1000);
    cout << "Size: " << buf.size() << endl;
    return 0;
}
