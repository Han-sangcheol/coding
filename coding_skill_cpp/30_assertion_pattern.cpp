/* C++ Assertion - assert, static_assert */
#include <iostream>
#include <cassert>
using namespace std;

// 컴파일 타임 체크
static_assert(sizeof(int) == 4, "int must be 4 bytes");

int divide(int a, int b) {
    assert(b != 0 && "Divisor cannot be zero");
    return a / b;
}

int main() {
    cout << "=== C++ Assertion ===" << endl;
    
    cout << "10 / 2 = " << divide(10, 2) << endl;
    // divide(10, 0);  // Assert 실패 (주석 처리)
    
    return 0;
}
