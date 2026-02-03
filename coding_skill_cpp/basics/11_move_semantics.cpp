/*
 * ============================================================================
 * 파일명: 11_move_semantics.cpp
 * 주제: 이동 의미론 (Move Semantics)
 * 컴파일: g++ -std=c++17 11_move_semantics.cpp -o 11_move
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class BigData {
private:
    int* data;
    size_t size;
    
public:
    // 생성자
    BigData(size_t s) : size(s) {
        data = new int[size];
        cout << "BigData 생성 (" << size << " 요소)" << endl;
    }
    
    // 소멸자
    ~BigData() {
        delete[] data;
        cout << "BigData 소멸" << endl;
    }
    
    // 복사 생성자 (느림)
    BigData(const BigData& other) : size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "BigData 복사 (느림)" << endl;
    }
    
    // 이동 생성자 (빠름)
    BigData(BigData&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "BigData 이동 (빠름)" << endl;
    }
    
    // 복사 대입 연산자
    BigData& operator=(const BigData& other) {
        if (this == &other) return *this;
        
        delete[] data;
        size = other.size;
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "BigData 복사 대입" << endl;
        return *this;
    }
    
    // 이동 대입 연산자
    BigData& operator=(BigData&& other) noexcept {
        if (this == &other) return *this;
        
        delete[] data;
        data = other.data;
        size = other.size;
        
        other.data = nullptr;
        other.size = 0;
        
        cout << "BigData 이동 대입" << endl;
        return *this;
    }
};

void Example_01_CopyVsMove() {
    cout << "\n=== 예제 1: 복사 vs 이동 ===\n" << endl;
    
    cout << "[복사]" << endl;
    BigData data1(1000000);
    BigData data2 = data1;  // 복사 생성자 (느림)
    
    cout << "\n[이동]" << endl;
    BigData data3(1000000);
    BigData data4 = move(data3);  // 이동 생성자 (빠름)
}

void Example_02_Lvalue_Rvalue() {
    cout << "\n=== 예제 2: lvalue vs rvalue ===\n" << endl;
    
    int x = 10;        // x는 lvalue
    int y = x + 5;     // x+5는 rvalue (임시값)
    
    // lvalue: 이름이 있고, 주소를 가질 수 있음
    int* ptr = &x;  // ✅ OK
    
    // rvalue: 임시값, 주소를 가질 수 없음
    // int* ptr2 = &(x + 5);  // ❌ 에러!
    
    cout << "lvalue: " << x << endl;
    cout << "rvalue (임시값): " << (x + 5) << endl;
}

void Example_03_StdMove() {
    cout << "\n=== 예제 3: std::move ===\n" << endl;
    
    string str1 = "Hello, World!";
    cout << "str1: " << str1 << endl;
    
    string str2 = move(str1);  // 이동
    cout << "str2: " << str2 << endl;
    cout << "str1: " << str1 << " (비어있음)" << endl;
    
    // str1은 유효하지만 비어있는 상태
}

void Example_04_VectorPerformance() {
    cout << "\n=== 예제 4: vector 성능 ===\n" << endl;
    
    vector<BigData> vec;
    
    cout << "[복사로 추가 (느림)]" << endl;
    BigData data1(100000);
    vec.push_back(data1);  // 복사
    
    cout << "\n[이동으로 추가 (빠름)]" << endl;
    vec.push_back(BigData(100000));  // 임시 객체, 자동 이동
    
    cout << "\n[명시적 이동]" << endl;
    BigData data2(100000);
    vec.push_back(move(data2));  // 명시적 이동
}

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 11. 이동 의미론" << endl;
    cout << "========================================" << endl;
    
    Example_01_CopyVsMove();
    Example_02_Lvalue_Rvalue();
    Example_03_StdMove();
    Example_04_VectorPerformance();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 12_modern_features.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * lvalue: 이름이 있는 값
 * rvalue: 임시값
 * 
 * 이동 생성자: T(T&& other)
 * 이동 대입: T& operator=(T&& other)
 * 
 * std::move:
 * - lvalue를 rvalue로 변환
 * - 소유권 이전
 * 
 * 장점:
 * - 불필요한 복사 제거
 * - 성능 향상
 * 
 * ============================================================================
 */
