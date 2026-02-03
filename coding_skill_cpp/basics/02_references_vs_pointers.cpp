/*
 * ============================================================================
 * 파일명: 02_references_vs_pointers.cpp
 * 주제: C++의 참조(Reference) vs C의 포인터(Pointer)
 * 컴파일: g++ -std=c++17 02_references_vs_pointers.cpp -o 02_references
 * 
 * ============================================================================
 * 이 챕터에서 배울 내용
 * ============================================================================
 * 
 * - 참조자(&)란 무엇인가
 * - 포인터와 참조의 차이
 * - 함수 매개변수로서의 참조
 * - const 참조
 * - 참조를 사용해야 하는 경우
 * 
 * ============================================================================
 * C와의 주요 차이점
 * ============================================================================
 * 
 * C에서는:
 * - 포인터만 사용 가능
 * - 함수에 변수를 전달하려면 포인터 필수
 * - NULL 포인터 체크 필요
 * - *와 &를 명시적으로 사용
 * 
 * C++에서는:
 * - 참조자 사용 가능 (포인터보다 안전)
 * - 함수 매개변수로 참조 사용 가능
 * - NULL 불가능 (더 안전)
 * - 변수처럼 사용 (*, & 불필요)
 * 
 * 왜 참조가 더 나은가?
 * - NULL 체크 불필요 (항상 유효한 객체)
 * - 문법이 깔끔 (*, & 생략 가능)
 * - 복사 오버헤드 없음
 * - 실수로 NULL 전달 방지
 * 
 * ============================================================================
 */

#include <iostream>
#include <string>

// C 스타일 (비교용)
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/* ============================================================================
 * 1. 참조자 기본 개념
 * ============================================================================ */

void Example_01_BasicReference() {
    cout << "\n=== 예제 1: 참조자 기본 ===\n" << endl;
    
    // 일반 변수
    int original = 10;
    
    // C 스타일: 포인터
    int* ptr = &original;  // 주소를 저장
    cout << "[C 포인터]" << endl;
    cout << "  원본: " << original << endl;
    cout << "  포인터 주소: " << ptr << endl;
    cout << "  *ptr: " << *ptr << endl;
    *ptr = 20;  // 역참조하여 변경
    cout << "  변경 후 원본: " << original << endl;
    
    // C++ 스타일: 참조
    int& ref = original;  // 별명(alias) 생성
    cout << "\n[C++ 참조]" << endl;
    cout << "  원본: " << original << endl;
    cout << "  ref: " << ref << endl;
    ref = 30;  // 역참조 없이 바로 변경!
    cout << "  변경 후 원본: " << original << endl;
    
    /*
     * 핵심 차이점:
     * 
     * 포인터 (int*):
     * - 주소를 저장하는 변수
     * - *를 사용하여 역참조
     * - NULL 가능
     * - 재할당 가능
     * 
     * 참조 (int&):
     * - 변수의 별명
     * - 역참조 불필요 (자동)
     * - NULL 불가능 (반드시 초기화)
     * - 재할당 불가능 (한 번 바인딩하면 고정)
     */
    
    // 참조는 반드시 초기화해야 함
    // int& invalid_ref;  // ❌ 컴파일 에러!
    
    // 참조는 재할당 불가
    int another = 100;
    ref = another;  // ref가 another를 가리키는 게 아님!
                    // original의 값이 100으로 변경될 뿐
    cout << "\n원본: " << original << ", another: " << another << endl;
}

/* ============================================================================
 * 2. 함수 매개변수: Pass by Value vs Pointer vs Reference
 * ============================================================================ */

// C 방식 1: Pass by Value (값 복사)
void c_swap_byvalue(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    // a, b는 복사본이므로 원본 변경 안 됨!
}

// C 방식 2: Pass by Pointer (주소 전달)
void c_swap_bypointer(int* a, int* b) {
    if (a == NULL || b == NULL) return;  // NULL 체크 필수!
    
    int temp = *a;
    *a = *b;
    *b = temp;
}

// C++ 방식: Pass by Reference (참조 전달)
void cpp_swap_byreference(int& a, int& b) {
    // NULL 체크 불필요!
    int temp = a;  // *, & 생략 가능
    a = b;
    b = temp;
}

void Example_02_FunctionParameters() {
    cout << "\n=== 예제 2: 함수 매개변수 ===\n" << endl;
    
    // 테스트 1: Pass by Value
    {
        int x = 10, y = 20;
        cout << "[Pass by Value]" << endl;
        cout << "  변경 전: x=" << x << ", y=" << y << endl;
        c_swap_byvalue(x, y);
        cout << "  변경 후: x=" << x << ", y=" << y << endl;
        cout << "  → 원본 변경 안 됨!" << endl;
    }
    
    // 테스트 2: Pass by Pointer
    {
        int x = 10, y = 20;
        cout << "\n[Pass by Pointer (C)]" << endl;
        cout << "  변경 전: x=" << x << ", y=" << y << endl;
        c_swap_bypointer(&x, &y);  // & 필요
        cout << "  변경 후: x=" << x << ", y=" << y << endl;
        cout << "  → 원본 변경됨, 하지만 &를 붙여야 함" << endl;
    }
    
    // 테스트 3: Pass by Reference
    {
        int x = 10, y = 20;
        cout << "\n[Pass by Reference (C++)]" << endl;
        cout << "  변경 전: x=" << x << ", y=" << y << endl;
        cpp_swap_byreference(x, y);  // & 불필요!
        cout << "  변경 후: x=" << x << ", y=" << y << endl;
        cout << "  → 원본 변경됨, 깔끔한 문법!" << endl;
    }
    
    /*
     * 비교:
     * 
     * Pass by Value:
     * - 복사 비용 발생
     * - 원본 변경 안 됨
     * - 작은 타입(int, double)에 적합
     * 
     * Pass by Pointer:
     * - NULL 체크 필요
     * - *, & 명시적 사용
     * - C와의 호환성
     * 
     * Pass by Reference:
     * - NULL 불가능 (더 안전)
     * - 깔끔한 문법
     * - 복사 없음
     * - C++의 권장 방식
     */
}

/* ============================================================================
 * 3. const 참조: 읽기 전용 참조
 * ============================================================================ */

// 큰 객체 전달 시 복사 오버헤드
void c_print_string_byvalue(string str) {
    // str이 복사됨 → 큰 문자열이면 느림!
    cout << str << endl;
}

// C 방식: const 포인터
void c_print_string_bypointer(const char* str) {
    if (str != NULL) {
        printf("%s\n", str);
    }
}

// C++ 방식: const 참조
void cpp_print_string_byreference(const string& str) {
    // 복사 없음, 읽기 전용
    cout << str << endl;
}

void Example_03_ConstReference() {
    cout << "\n=== 예제 3: const 참조 ===\n" << endl;
    
    string large_string = "매우 긴 문자열입니다. " + string(1000, 'A');
    
    cout << "[복사 발생 (느림)]" << endl;
    c_print_string_byvalue(large_string);  // 복사!
    
    cout << "\n[const 참조 (빠름)]" << endl;
    cpp_print_string_byreference(large_string);  // 복사 없음!
    
    /*
     * const 참조의 장점:
     * 
     * 1. 복사 오버헤드 없음
     *    - 큰 객체를 전달할 때 효율적
     * 
     * 2. 읽기 전용
     *    - 함수 내에서 변경 불가
     *    - 실수 방지
     * 
     * 3. 임시 객체 바인딩 가능
     *    - rvalue도 받을 수 있음
     */
    
    // const 참조는 임시 객체도 받을 수 있음
    const string& temp_ref = string("임시 객체");
    cout << "\n임시 객체: " << temp_ref << endl;
    
    // 일반 참조는 불가능
    // string& invalid = string("임시");  // ❌ 에러!
    
    /*
     * 실무 가이드라인:
     * 
     * - 작은 타입 (int, double): Pass by Value
     *   void func(int x);
     * 
     * - 큰 객체, 읽기 전용: const 참조
     *   void func(const string& s);
     * 
     * - 수정 필요: 참조
     *   void func(string& s);
     * 
     * - C 호환 필요: 포인터
     *   void func(int* p);
     */
}

/* ============================================================================
 * 4. 반환값으로서의 참조
 * ============================================================================ */

// ❌ 위험: 지역 변수의 참조 반환
int& dangerous_return() {
    int local = 10;
    return local;  // ❌ 댕글링 참조! local은 함수 끝나면 소멸
}

// ✅ 안전: 전역/멤버 변수의 참조 반환
int global_value = 100;

int& safe_return() {
    return global_value;  // ✅ 전역 변수는 OK
}

// 실무 예제: 배열 인덱스 참조 반환 (vector의 operator[])
class SimpleVector {
private:
    int data[10];
    
public:
    SimpleVector() {
        for (int i = 0; i < 10; i++) {
            data[i] = i;
        }
    }
    
    // 참조 반환 → 읽기/쓰기 모두 가능
    int& at(int index) {
        return data[index];
    }
    
    // const 버전 → 읽기만 가능
    const int& at(int index) const {
        return data[index];
    }
};

void Example_04_ReturnReference() {
    cout << "\n=== 예제 4: 참조 반환 ===\n" << endl;
    
    // 전역 변수 참조
    int& ref = safe_return();
    cout << "전역 변수: " << ref << endl;
    ref = 200;  // 전역 변수 수정
    cout << "수정 후: " << global_value << endl;
    
    // 배열 인덱스 참조 (STL vector처럼)
    SimpleVector vec;
    cout << "\n배열[5]: " << vec.at(5) << endl;
    vec.at(5) = 999;  // 참조를 통한 수정
    cout << "수정 후 배열[5]: " << vec.at(5) << endl;
    
    /*
     * 참조 반환 사용 시기:
     * 
     * ✅ 사용:
     * - 클래스 멤버 변수 접근
     * - 컨테이너 요소 접근
     * - 연산자 오버로딩 (operator=, operator[])
     * 
     * ❌ 사용 금지:
     * - 지역 변수 반환
     * - 임시 객체 반환
     */
}

/* ============================================================================
 * 5. 포인터 vs 참조: 언제 무엇을 사용할까?
 * ============================================================================ */

void Example_05_WhenToUse() {
    cout << "\n=== 예제 5: 포인터 vs 참조 선택 가이드 ===\n" << endl;
    
    cout << "포인터를 사용해야 하는 경우:" << endl;
    cout << "  1. NULL 가능성이 있을 때" << endl;
    cout << "  2. 재할당이 필요할 때" << endl;
    cout << "  3. 포인터 연산이 필요할 때 (배열)" << endl;
    cout << "  4. C API와 호환해야 할 때" << endl;
    cout << "  5. 동적 메모리 관리 (new/delete)" << endl;
    
    cout << "\n참조를 사용해야 하는 경우:" << endl;
    cout << "  1. 항상 유효한 객체를 가리킬 때" << endl;
    cout << "  2. 함수 매개변수 (큰 객체)" << endl;
    cout << "  3. 연산자 오버로딩" << endl;
    cout << "  4. 깔끔한 문법을 원할 때" << endl;
    cout << "  5. NULL 체크를 피하고 싶을 때" << endl;
    
    // 예제: NULL 가능성
    {
        cout << "\n[예제: NULL 가능성]" << endl;
        
        int* maybe_null = nullptr;
        
        // 포인터: NULL 체크 필요
        if (maybe_null != nullptr) {
            cout << "값: " << *maybe_null << endl;
        } else {
            cout << "NULL입니다" << endl;
        }
        
        // 참조: NULL 불가능
        int value = 10;
        int& always_valid = value;
        cout << "참조는 항상 유효: " << always_valid << endl;
    }
}

/* ============================================================================
 * 6. 실무 예제: 데이터 구조
 * ============================================================================ */

// C 방식: 포인터 기반
typedef struct Node_C {
    int data;
    struct Node_C* next;  // 포인터
} Node_C;

void C_PrintList(const Node_C* head) {
    if (head == NULL) return;  // NULL 체크
    
    printf("[C 리스트] ");
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

// C++ 방식: 참조 + 포인터 혼용
struct Node_CPP {
    int data;
    Node_CPP* next;  // 포인터 (NULL 가능)
    
    Node_CPP(int d) : data(d), next(nullptr) {}
};

void CPP_PrintList(const Node_CPP* head) {
    if (head == nullptr) return;
    
    cout << "[C++ 리스트] ";
    while (head != nullptr) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
}

// 참조를 사용한 함수 (리스트가 존재함을 보장)
void CPP_AddToFront(Node_CPP*& head, int value) {
    // Node_CPP*& : 포인터에 대한 참조!
    // head 자체를 수정 가능
    Node_CPP* new_node = new Node_CPP(value);
    new_node->next = head;
    head = new_node;
}

void Example_06_RealWorld() {
    cout << "\n=== 예제 6: 실무 예제 (연결 리스트) ===\n" << endl;
    
    // C 방식
    {
        Node_C n3 = {3, NULL};
        Node_C n2 = {2, &n3};
        Node_C n1 = {1, &n2};
        
        C_PrintList(&n1);
    }
    
    // C++ 방식
    {
        Node_CPP* head = new Node_CPP(1);
        head->next = new Node_CPP(2);
        head->next->next = new Node_CPP(3);
        
        CPP_PrintList(head);
        
        // 참조를 사용한 수정
        CPP_AddToFront(head, 0);
        CPP_PrintList(head);
        
        // 메모리 해제 (실제로는 소멸자에서 처리)
        while (head != nullptr) {
            Node_CPP* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    /*
     * 실무에서:
     * - 포인터: 다음 노드 (NULL 가능)
     * - 참조: 함수 매개변수 (head 수정 시)
     * 
     * 이렇게 혼용하는 것이 일반적!
     */
}

/* ============================================================================
 * 7. 주의사항
 * ============================================================================ */

void Example_07_CommonMistakes() {
    cout << "\n=== 예제 7: 흔한 실수 ===\n" << endl;
    
    // 실수 1: 지역 변수 참조 반환
    cout << "[1] 댕글링 참조 주의" << endl;
    // int& bad = dangerous_return();  // ❌ 위험!
    // cout << bad << endl;  // Undefined Behavior
    cout << "  → 지역 변수의 참조 반환 금지!" << endl;
    
    // 실수 2: NULL 참조 (불가능)
    cout << "\n[2] NULL 참조는 없음" << endl;
    int* ptr = nullptr;
    // int& ref = *ptr;  // ❌ Undefined Behavior
    cout << "  → 참조는 항상 유효한 객체여야 함!" << endl;
    
    // 실수 3: 참조 재할당 시도
    cout << "\n[3] 참조는 재할당 불가" << endl;
    int a = 10, b = 20;
    int& ref = a;
    ref = b;  // a의 값이 20이 됨, ref가 b를 가리키는 게 아님!
    cout << "  a = " << a << ", b = " << b << endl;
    cout << "  → ref는 여전히 a를 가리킴!" << endl;
    
    // 올바른 방법: 포인터 사용
    int* p = &a;
    p = &b;  // 이제 b를 가리킴
    cout << "  포인터는 재할당 가능: " << *p << endl;
}

/* ============================================================================
 * main 함수
 * ============================================================================ */

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 02. 참조 vs 포인터" << endl;
    cout << "========================================" << endl;
    
    Example_01_BasicReference();
    Example_02_FunctionParameters();
    Example_03_ConstReference();
    Example_04_ReturnReference();
    Example_05_WhenToUse();
    Example_06_RealWorld();
    Example_07_CommonMistakes();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 03_classes_objects.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 연습 문제
 * ============================================================================
 * 
 * 1. 포인터를 사용한 swap 함수를 참조를 사용하여 다시 작성하세요.
 * 2. const 참조를 사용하여 큰 객체를 효율적으로 전달하는 함수를 만드세요.
 * 3. 배열 요소에 접근하는 at() 함수를 참조 반환으로 구현하세요.
 * 4. 포인터와 참조를 혼용한 간단한 자료구조를 만들어보세요.
 * 
 * ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * 참조 (Reference):
 * - 변수의 별명
 * - NULL 불가능
 * - 재할당 불가능
 * - 자동 역참조
 * - 함수 매개변수에 적합
 * 
 * 포인터 (Pointer):
 * - 주소를 저장
 * - NULL 가능
 * - 재할당 가능
 * - 명시적 역참조 (*) 필요
 * - 동적 메모리, 배열에 적합
 * 
 * 사용 가이드:
 * - 작은 타입: Pass by Value
 * - 큰 객체 (읽기): const 참조
 * - 큰 객체 (쓰기): 참조
 * - NULL 가능: 포인터
 * - C 호환: 포인터
 * 
 * ============================================================================
 */
