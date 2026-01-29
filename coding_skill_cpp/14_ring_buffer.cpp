/* C++ Ring Buffer - template */
#include <iostream>
#include <array>
using namespace std;

template<typename T, size_t Size>
class RingBuffer {
    array<T, Size> buffer;
    size_t head = 0, tail = 0, count = 0;
    
public:
    bool push(const T& item) {
        if (count >= Size) return false;
        buffer[tail] = item;
        tail = (tail + 1) % Size;
        count++;
        return true;
    }
    
    bool pop(T& item) {
        if (count == 0) return false;
        item = buffer[head];
        head = (head + 1) % Size;
        count--;
        return true;
    }
    
    size_t size() const { return count; }
};

int main() {
    cout << "=== C++ Ring Buffer ===" << endl;
    RingBuffer<int, 5> rb;
    
    for (int i = 1; i <= 3; ++i) rb.push(i * 10);
    
    int val;
    while (rb.pop(val)) {
        cout << "Pop: " << val << endl;
    }
    
    return 0;
}
