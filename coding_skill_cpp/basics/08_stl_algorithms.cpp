/*
 * ============================================================================
 * 파일명: 08_stl_algorithms.cpp
 * 주제: STL 알고리즘 (sort, find, transform 등)
 * 컴파일: g++ -std=c++17 08_stl_algorithms.cpp -o 08_algorithms
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

void Example_01_Sort() {
    cout << "\n=== 예제 1: sort (정렬) ===\n" << endl;
    
    vector<int> numbers = {5, 2, 8, 1, 9, 3};
    
    cout << "정렬 전: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    sort(numbers.begin(), numbers.end());
    
    cout << "정렬 후: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // 내림차순 정렬
    sort(numbers.begin(), numbers.end(), greater<int>());
    
    cout << "내림차순: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
}

void Example_02_Find() {
    cout << "\n=== 예제 2: find (검색) ===\n" << endl;
    
    vector<int> numbers = {10, 20, 30, 40, 50};
    
    auto it = find(numbers.begin(), numbers.end(), 30);
    
    if (it != numbers.end()) {
        cout << "30을 찾음, 인덱스: " << (it - numbers.begin()) << endl;
    } else {
        cout << "못 찾음" << endl;
    }
}

void Example_03_Transform() {
    cout << "\n=== 예제 3: transform (변환) ===\n" << endl;
    
    vector<int> numbers = {1, 2, 3, 4, 5};
    vector<int> squared(5);
    
    // 각 요소를 제곱
    transform(numbers.begin(), numbers.end(), squared.begin(),
              [](int n) { return n * n; });
    
    cout << "원본: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    cout << "제곱: ";
    for (int n : squared) cout << n << " ";
    cout << endl;
}

void Example_04_Accumulate() {
    cout << "\n=== 예제 4: accumulate (누적) ===\n" << endl;
    
    vector<int> numbers = {1, 2, 3, 4, 5};
    
    int sum = accumulate(numbers.begin(), numbers.end(), 0);
    cout << "합계: " << sum << endl;
    
    int product = accumulate(numbers.begin(), numbers.end(), 1,
                            [](int a, int b) { return a * b; });
    cout << "곱: " << product << endl;
}

void Example_05_CVsCPP() {
    cout << "\n=== 예제 5: C vs C++ 비교 ===\n" << endl;
    
    // C 방식: 수동 정렬
    cout << "[C 방식]" << endl;
    int c_arr[] = {5, 2, 8, 1, 9};
    int n = 5;
    // 버블 정렬 (수동)
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (c_arr[j] > c_arr[j+1]) {
                int temp = c_arr[j];
                c_arr[j] = c_arr[j+1];
                c_arr[j+1] = temp;
            }
        }
    }
    cout << "수동 정렬 (버블 정렬)" << endl;
    
    // C++ 방식: STL
    cout << "\n[C++ 방식]" << endl;
    vector<int> cpp_vec = {5, 2, 8, 1, 9};
    sort(cpp_vec.begin(), cpp_vec.end());  // 1줄!
    cout << "STL sort (최적화된 퀵소트)" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 08. STL 알고리즘" << endl;
    cout << "========================================" << endl;
    
    Example_01_Sort();
    Example_02_Find();
    Example_03_Transform();
    Example_04_Accumulate();
    Example_05_CVsCPP();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 09_lambda_functions.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * sort: 정렬
 * find: 검색
 * transform: 변환
 * accumulate: 누적
 * 
 * 장점:
 * - 최적화된 구현
 * - 코드 간결
 * - 버그 감소
 * 
 * ============================================================================
 */
