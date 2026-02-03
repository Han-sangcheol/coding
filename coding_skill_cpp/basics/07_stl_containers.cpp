/*
 * ============================================================================
 * 파일명: 07_stl_containers.cpp
 * 주제: STL 컨테이너 (vector, map, set 등)
 * 컴파일: g++ -std=c++17 07_stl_containers.cpp -o 07_stl
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

void Example_01_Vector() {
    cout << "\n=== 예제 1: vector (동적 배열) ===\n" << endl;
    
    // C 배열 vs vector
    int c_array[5] = {1, 2, 3, 4, 5};  // 고정 크기
    
    vector<int> vec = {1, 2, 3, 4, 5};  // 동적 크기
    vec.push_back(6);  // 추가
    vec.push_back(7);
    
    cout << "vector 크기: " << vec.size() << endl;
    cout << "요소: ";
    for (int val : vec) {
        cout << val << " ";
    }
    cout << endl;
    
    /*
     * vector 장점:
     * - 동적 크기
     * - 자동 메모리 관리
     * - 범위 체크 (at())
     * - 다양한 메서드
     */
}

void Example_02_Map() {
    cout << "\n=== 예제 2: map (키-값 쌍) ===\n" << endl;
    
    map<string, int> ages;
    ages["홍길동"] = 30;
    ages["김철수"] = 25;
    ages["이영희"] = 28;
    
    cout << "홍길동의 나이: " << ages["홍길동"] << endl;
    
    // 순회
    for (const auto& pair : ages) {
        cout << pair.first << ": " << pair.second << "세" << endl;
    }
    
    // 키 존재 확인
    if (ages.find("박민수") == ages.end()) {
        cout << "박민수는 없음" << endl;
    }
}

void Example_03_Set() {
    cout << "\n=== 예제 3: set (집합, 중복 없음) ===\n" << endl;
    
    set<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6};  // 중복 제거, 자동 정렬
    
    cout << "set 요소: ";
    for (int num : numbers) {
        cout << num << " ";  // 1 2 3 4 5 6 9
    }
    cout << endl;
    
    numbers.insert(7);
    numbers.insert(1);  // 중복, 추가 안 됨
    
    cout << "크기: " << numbers.size() << endl;
}

void Example_04_Comparison() {
    cout << "\n=== 예제 4: C vs C++ 비교 ===\n" << endl;
    
    // C: 수동 배열 관리
    cout << "[C 방식]" << endl;
    int* c_arr = (int*)malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) {
        c_arr[i] = i * 10;
    }
    // ... 사용 ...
    free(c_arr);  // 수동 해제
    cout << "수동 malloc/free" << endl;
    
    // C++: STL vector
    cout << "\n[C++ 방식]" << endl;
    vector<int> cpp_vec(5);
    for (int i = 0; i < 5; i++) {
        cpp_vec[i] = i * 10;
    }
    // ... 사용 ...
    // 자동 해제!
    cout << "자동 메모리 관리" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 07. STL 컨테이너" << endl;
    cout << "========================================" << endl;
    
    Example_01_Vector();
    Example_02_Map();
    Example_03_Set();
    Example_04_Comparison();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 08_stl_algorithms.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * vector: 동적 배열
 * map: 키-값 쌍 (해시맵)
 * set: 집합 (중복 없음, 정렬)
 * 
 * 장점:
 * - 자동 메모리 관리
 * - 타입 안전
 * - 다양한 메서드
 * 
 * ============================================================================
 */
