/* C++ Double Buffer - std::swap */
#include <iostream>
#include <array>
#include <algorithm>
using namespace std;

template<typename T, size_t Size>
class DoubleBuffer {
    array<T, Size> bufferA, bufferB;
    array<T, Size>* front = &bufferA;
    array<T, Size>* back = &bufferB;
    
public:
    void write(const T& data, size_t index) {
        if (index < Size) (*back)[index] = data;
    }
    
    T read(size_t index) const {
        return (index < Size) ? (*front)[index] : T{};
    }
    
    void swap() {
        std::swap(front, back);
        cout << "[DoubleBuffer] Swapped!" << endl;
    }
};

int main() {
    cout << "=== C++ Double Buffer ===" << endl;
    DoubleBuffer<int, 5> db;
    
    db.write(10, 0);
    db.write(20, 1);
    db.swap();
    
    cout << "Read: " << db.read(0) << ", " << db.read(1) << endl;
    
    return 0;
}
