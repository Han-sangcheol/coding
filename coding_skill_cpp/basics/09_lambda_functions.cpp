/*
 * ============================================================================
 * 파일명: 09_lambda_functions.cpp
 * 주제: 람다 함수 (C의 함수 포인터와 비교)
 * 컴파일: g++ -std=c++17 09_lambda_functions.cpp -o 09_lambda
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

void Example_01_BasicLambda() {
    cout << "\n=== 예제 1: 기본 람다 ===\n" << endl;
    
    // 기본 람다
    auto hello = []() {
        cout << "Hello Lambda!" << endl;
    };
    hello();
    
    // 매개변수 있는 람다
    auto add = [](int a, int b) {
        return a + b;
    };
    cout << "10 + 20 = " << add(10, 20) << endl;
    
    // 반환 타입 명시
    auto multiply = [](int a, int b) -> int {
        return a * b;
    };
    cout << "10 * 20 = " << multiply(10, 20) << endl;
}

void Example_02_Capture() {
    cout << "\n=== 예제 2: 캡처 ===\n" << endl;
    
    int x = 10;
    int y = 20;
    
    // 값 캡처 (복사)
    auto lambda1 = [x, y]() {
        cout << "값 캡처: x=" << x << ", y=" << y << endl;
        // x = 100;  // ❌ 에러! 수정 불가
    };
    lambda1();
    
    // 참조 캡처
    auto lambda2 = [&x, &y]() {
        cout << "참조 캡처: x=" << x << ", y=" << y << endl;
        x = 100;  // ✅ 수정 가능
    };
    lambda2();
    cout << "x가 변경됨: " << x << endl;
    
    // 모든 변수 캡처
    auto lambda3 = [=]() {  // 모든 변수 값 캡처
        cout << "x=" << x << ", y=" << y << endl;
    };
    
    auto lambda4 = [&]() {  // 모든 변수 참조 캡처
        x = 200;
    };
    lambda4();
    cout << "x=" << x << endl;
}

void Example_03_WithSTL() {
    cout << "\n=== 예제 3: STL과 함께 사용 ===\n" << endl;
    
    vector<int> numbers = {5, 2, 8, 1, 9, 3, 7};
    
    // sort with lambda
    sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a > b;  // 내림차순
    });
    
    cout << "내림차순: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // for_each with lambda
    for_each(numbers.begin(), numbers.end(), [](int n) {
        // cout << n * 2 << " ";
    });
    
    // count_if with lambda
    int count = count_if(numbers.begin(), numbers.end(), [](int n) {
        return n > 5;
    });
    cout << "5보다 큰 수: " << count << "개" << endl;
}

void Example_04_CVsCPP() {
    cout << "\n=== 예제 4: C vs C++ 비교 ===\n" << endl;
    
    // C 방식: 함수 포인터
    cout << "[C 방식]" << endl;
    // 별도 함수 정의 필요
    // int compare(const void* a, const void* b) { ... }
    // qsort(arr, n, sizeof(int), compare);
    cout << "별도 함수 정의 필요" << endl;
    
    // C++ 방식: 람다
    cout << "\n[C++ 방식]" << endl;
    vector<int> vec = {3, 1, 4};
    sort(vec.begin(), vec.end(), [](int a, int b) {
        return a < b;
    });
    cout << "인라인으로 간결하게 작성" << endl;
}

void Example_05_StdFunction() {
    cout << "\n=== 예제 5: std::function ===\n" << endl;
    
    // 람다를 변수에 저장
    function<int(int, int)> op;
    
    op = [](int a, int b) { return a + b; };
    cout << "10 + 20 = " << op(10, 20) << endl;
    
    op = [](int a, int b) { return a * b; };
    cout << "10 * 20 = " << op(10, 20) << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 09. 람다 함수" << endl;
    cout << "========================================" << endl;
    
    Example_01_BasicLambda();
    Example_02_Capture();
    Example_03_WithSTL();
    Example_04_CVsCPP();
    Example_05_StdFunction();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 10_smart_pointers.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * 람다 기본: []() { }
 * 캡처:
 *   [=] 값 캡처
 *   [&] 참조 캡처
 * 
 * STL과 함께:
 *   sort, for_each, count_if
 * 
 * std::function:
 *   람다를 변수에 저장
 * 
 * ============================================================================
 */
