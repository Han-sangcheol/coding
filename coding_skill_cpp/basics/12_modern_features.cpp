/*
 * ============================================================================
 * 파일명: 12_modern_features.cpp
 * 주제: 모던 C++ 기능 (C++11/14/17)
 * 컴파일: g++ -std=c++17 12_modern_features.cpp -o 12_modern
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <variant>

using namespace std;

void Example_01_Nullptr() {
    cout << "\n=== 예제 1: nullptr ===\n" << endl;
    
    // C 방식
    int* c_ptr = NULL;  // 실제로는 0
    
    // C++ 방식
    int* cpp_ptr = nullptr;  // 진짜 null 포인터
    
    if (cpp_ptr == nullptr) {
        cout << "nullptr 체크" << endl;
    }
    
    /*
     * nullptr 장점:
     * - 타입 안전
     * - NULL(0)과 혼동 없음
     */
}

void Example_02_Auto() {
    cout << "\n=== 예제 2: auto ===\n" << endl;
    
    auto i = 42;              // int
    auto d = 3.14;            // double
    auto s = string("Hello"); // string
    
    vector<int> vec = {1, 2, 3};
    
    // 복잡한 타입 간결화
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

void Example_03_RangeBasedFor() {
    cout << "\n=== 예제 3: Range-based for ===\n" << endl;
    
    vector<int> numbers = {1, 2, 3, 4, 5};
    
    // C 방식
    cout << "[C 방식]" << endl;
    for (size_t i = 0; i < numbers.size(); i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;
    
    // C++ 방식
    cout << "\n[C++ 방식]" << endl;
    for (int n : numbers) {
        cout << n << " ";
    }
    cout << endl;
    
    // 참조로 순회 (수정 가능)
    for (int& n : numbers) {
        n *= 2;
    }
    
    cout << "2배 후: ";
    for (const auto& n : numbers) {
        cout << n << " ";
    }
    cout << endl;
}

void Example_04_Constexpr() {
    cout << "\n=== 예제 4: constexpr ===\n" << endl;
    
    // 컴파일 타임 상수
    constexpr int size = 10;
    int array[size];  // ✅ OK, 컴파일 타임에 결정
    
    constexpr int square(int x) {
        return x * x;
    }
    
    constexpr int result = square(5);  // 컴파일 타임에 계산
    cout << "5의 제곱: " << result << endl;
    
    /*
     * constexpr:
     * - 컴파일 타임 계산
     * - 성능 향상
     * - const보다 강력
     */
}

void Example_05_StructuredBindings() {
    cout << "\n=== 예제 5: Structured Bindings (C++17) ===\n" << endl;
    
    auto pair = make_pair(10, string("Hello"));
    
    // C++14 이전
    int value = pair.first;
    string text = pair.second;
    
    // C++17
    auto [val, txt] = pair;
    cout << "value: " << val << ", text: " << txt << endl;
    
    // map 순회
    map<string, int> ages = {{"홍길동", 30}, {"김철수", 25}};
    for (const auto& [name, age] : ages) {
        cout << name << ": " << age << "세" << endl;
    }
}

void Example_06_Optional() {
    cout << "\n=== 예제 6: std::optional (C++17) ===\n" << endl;
    
    auto divide = [](double a, double b) -> optional<double> {
        if (b == 0.0) {
            return nullopt;  // 값 없음
        }
        return a / b;
    };
    
    if (auto result = divide(10.0, 2.0)) {
        cout << "10 / 2 = " << *result << endl;
    }
    
    if (auto result = divide(10.0, 0.0)) {
        cout << "결과: " << *result << endl;
    } else {
        cout << "0으로 나눌 수 없음" << endl;
    }
}

void Example_07_Variant() {
    cout << "\n=== 예제 7: std::variant (C++17) ===\n" << endl;
    
    variant<int, double, string> var;
    
    var = 42;
    cout << "int: " << get<int>(var) << endl;
    
    var = 3.14;
    cout << "double: " << get<double>(var) << endl;
    
    var = string("Hello");
    cout << "string: " << get<string>(var) << endl;
    
    // 타입 확인
    if (holds_alternative<string>(var)) {
        cout << "현재 string 타입" << endl;
    }
}

void Example_08_CVsCPP() {
    cout << "\n=== 예제 8: C vs C++ 비교 ===\n" << endl;
    
    // C 방식
    cout << "[C 방식]" << endl;
    int c_arr[] = {1, 2, 3};
    for (int i = 0; i < 3; i++) {
        printf("%d ", c_arr[i]);
    }
    printf("\n");
    
    // C++ 방식
    cout << "\n[C++ 방식]" << endl;
    vector<int> cpp_vec = {1, 2, 3};
    for (const auto& val : cpp_vec) {
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 12. 모던 C++ 기능" << endl;
    cout << "========================================" << endl;
    
    Example_01_Nullptr();
    Example_02_Auto();
    Example_03_RangeBasedFor();
    Example_04_Constexpr();
    Example_05_StructuredBindings();
    Example_06_Optional();
    Example_07_Variant();
    Example_08_CVsCPP();
    
    cout << "\n========================================" << endl;
    cout << "🎉 모든 C++ 기초 학습 완료!" << endl;
    cout << "다음: 상위 디렉토리의 패턴 학습" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * C++11:
 * - nullptr, auto
 * - range-based for
 * - lambda
 * - move semantics
 * - smart pointers
 * 
 * C++14:
 * - generic lambda
 * - auto 반환 타입
 * 
 * C++17:
 * - structured bindings
 * - optional
 * - variant
 * - if with initializer
 * 
 * 모던 C++:
 * - 안전성 ↑
 * - 간결성 ↑
 * - 성능 ↑
 * 
 * ============================================================================
 */
