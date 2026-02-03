/*
 * ============================================================================
 * 파일명: 06_templates_basics.cpp
 * 주제: 템플릿 기초 (C의 매크로와 비교)
 * 컴파일: g++ -std=c++17 06_templates_basics.cpp -o 06_templates
 * 
 * ============================================================================
 * C와의 주요 차이점
 * ============================================================================
 * 
 * C에서는:
 * - 매크로로 제네릭 구현
 * - 타입 안전성 없음
 * - 디버깅 어려움
 * 
 * C++에서는:
 * - 템플릿으로 타입 안전
 * - 컴파일 타임에 생성
 * - 타입 체크 강력
 * 
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

/* ============================================================================
 * 1. 함수 템플릿 기본
 * ============================================================================ */

// C 방식: 매크로
#define C_MAX(a, b) ((a) > (b) ? (a) : (b))

// C++ 방식: 함수 템플릿
template<typename T>
T cpp_max(T a, T b) {
    return a > b ? a : b;
}

void Example_01_FunctionTemplate() {
    cout << "\n=== 예제 1: 함수 템플릿 ===\n" << endl;
    
    // C 매크로
    cout << "[C 매크로]" << endl;
    cout << "max(10, 20) = " << C_MAX(10, 20) << endl;
    cout << "max(3.5, 2.1) = " << C_MAX(3.5, 2.1) << endl;
    
    // 문제: 타입 안전성 없음
    // C_MAX("hello", 10);  // 경고만, 런타임 오류
    
    // C++ 템플릿
    cout << "\n[C++ 템플릿]" << endl;
    cout << "max(10, 20) = " << cpp_max(10, 20) << endl;
    cout << "max(3.5, 2.1) = " << cpp_max(3.5, 2.1) << endl;
    cout << "max('a', 'z') = " << cpp_max('a', 'z') << endl;
    
    // cpp_max("hello", 10);  // ❌ 컴파일 에러! 타입 안전
    
    /*
     * 템플릿 장점:
     * - 타입 안전성
     * - 코드 재사용
     * - 컴파일 타임 생성
     */
}

/* ============================================================================
 * 2. 클래스 템플릿
 * ============================================================================ */

template<typename T>
class Stack {
private:
    vector<T> elements;
    
public:
    void push(const T& elem) {
        elements.push_back(elem);
    }
    
    T pop() {
        if (elements.empty()) {
            throw runtime_error("Stack is empty");
        }
        T elem = elements.back();
        elements.pop_back();
        return elem;
    }
    
    bool empty() const {
        return elements.empty();
    }
    
    size_t size() const {
        return elements.size();
    }
};

void Example_02_ClassTemplate() {
    cout << "\n=== 예제 2: 클래스 템플릿 ===\n" << endl;
    
    Stack<int> int_stack;
    int_stack.push(10);
    int_stack.push(20);
    cout << "int stack pop: " << int_stack.pop() << endl;
    
    Stack<string> str_stack;
    str_stack.push("Hello");
    str_stack.push("World");
    cout << "string stack pop: " << str_stack.pop() << endl;
}

/* ============================================================================
 * 3. 템플릿 특수화
 * ============================================================================ */

// 일반 템플릿
template<typename T>
class Printer {
public:
    void print(const T& value) {
        cout << "일반: " << value << endl;
    }
};

// 특수화: string용
template<>
class Printer<string> {
public:
    void print(const string& value) {
        cout << "문자열 특수화: \"" << value << "\"" << endl;
    }
};

void Example_03_Specialization() {
    cout << "\n=== 예제 3: 템플릿 특수화 ===\n" << endl;
    
    Printer<int> int_printer;
    int_printer.print(42);
    
    Printer<string> str_printer;
    str_printer.print("Hello");
}

/* ============================================================================
 * 4. 실무 예제: 타입 안전 컨테이너
 * ============================================================================ */

template<typename T, int SIZE>
class Array {
private:
    T data[SIZE];
    
public:
    T& operator[](int index) {
        return data[index];
    }
    
    const T& operator[](int index) const {
        return data[index];
    }
    
    int size() const {
        return SIZE;
    }
    
    void fill(const T& value) {
        for (int i = 0; i < SIZE; i++) {
            data[i] = value;
        }
    }
};

void Example_04_RealWorld() {
    cout << "\n=== 예제 4: 실무 예제 ===\n" << endl;
    
    Array<int, 5> int_array;
    int_array.fill(0);
    int_array[0] = 100;
    
    cout << "int_array[0] = " << int_array[0] << endl;
    cout << "size = " << int_array.size() << endl;
    
    Array<double, 3> double_array;
    double_array.fill(3.14);
    cout << "double_array[0] = " << double_array[0] << endl;
}

/* ============================================================================
 * main 함수
 * ============================================================================ */

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 06. 템플릿 기초" << endl;
    cout << "========================================" << endl;
    
    Example_01_FunctionTemplate();
    Example_02_ClassTemplate();
    Example_03_Specialization();
    Example_04_RealWorld();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 07_stl_containers.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * 함수 템플릿:
 * - template<typename T>
 * - 타입 안전
 * - 매크로 대체
 * 
 * 클래스 템플릿:
 * - 제네릭 컨테이너
 * - Stack<int>, Stack<string>
 * 
 * 템플릿 특수화:
 * - 특정 타입에 최적화
 * - template<>
 * 
 * ============================================================================
 */
