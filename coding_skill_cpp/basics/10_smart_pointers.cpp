/*
 * ============================================================================
 * 파일명: 10_smart_pointers.cpp
 * 주제: 스마트 포인터 (unique_ptr, shared_ptr, weak_ptr)
 * 컴파일: g++ -std=c++17 10_smart_pointers.cpp -o 10_smart
 * ============================================================================
 */

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Resource {
public:
    Resource() { cout << "Resource 생성" << endl; }
    ~Resource() { cout << "Resource 소멸" << endl; }
    void use() { cout << "Resource 사용 중" << endl; }
};

void Example_01_RawPointer() {
    cout << "\n=== 예제 1: Raw 포인터 (C 방식) ===\n" << endl;
    
    Resource* ptr = new Resource();
    ptr->use();
    delete ptr;  // 수동 해제 필수!
    
    // 문제점:
    // 1. delete 깜빡하면 메모리 누수
    // 2. 중복 delete 위험
    // 3. 예외 발생 시 누수
}

void Example_02_UniquePtr() {
    cout << "\n=== 예제 2: unique_ptr (단독 소유) ===\n" << endl;
    
    {
        unique_ptr<Resource> ptr = make_unique<Resource>();
        ptr->use();
        // 블록 끝: 자동 delete!
    }
    cout << "[블록 끝: 자동 소멸]\n" << endl;
    
    // 소유권 이전
    unique_ptr<Resource> ptr1 = make_unique<Resource>();
    unique_ptr<Resource> ptr2 = move(ptr1);  // 이동
    
    // ptr1은 nullptr
    if (ptr1 == nullptr) {
        cout << "ptr1은 이제 nullptr" << endl;
    }
    ptr2->use();
}

void Example_03_SharedPtr() {
    cout << "\n=== 예제 3: shared_ptr (공유 소유) ===\n" << endl;
    
    {
        shared_ptr<Resource> ptr1 = make_shared<Resource>();
        cout << "참조 카운트: " << ptr1.use_count() << endl;
        
        {
            shared_ptr<Resource> ptr2 = ptr1;  // 복사
            cout << "참조 카운트: " << ptr1.use_count() << endl;
            
            shared_ptr<Resource> ptr3 = ptr1;
            cout << "참조 카운트: " << ptr1.use_count() << endl;
        }
        cout << "ptr2, ptr3 소멸 후: " << ptr1.use_count() << endl;
    }
    cout << "[모든 shared_ptr 소멸: Resource 자동 삭제]\n" << endl;
}

void Example_04_WeakPtr() {
    cout << "\n=== 예제 4: weak_ptr (약한 참조) ===\n" << endl;
    
    weak_ptr<Resource> weak;
    
    {
        shared_ptr<Resource> shared = make_shared<Resource>();
        weak = shared;  // weak_ptr로 관찰
        
        cout << "shared_ptr 참조 카운트: " << shared.use_count() << endl;
        
        // weak_ptr 사용
        if (auto locked = weak.lock()) {
            locked->use();
        }
    }
    
    // shared_ptr 소멸 후
    if (weak.expired()) {
        cout << "weak_ptr가 가리키는 객체 소멸됨" << endl;
    }
}

void Example_05_CVsCPP() {
    cout << "\n=== 예제 5: C vs C++ 비교 ===\n" << endl;
    
    // C 방식
    cout << "[C 방식]" << endl;
    Resource* c_ptr = new Resource();
    c_ptr->use();
    delete c_ptr;  // 수동!
    cout << "수동 메모리 관리\n" << endl;
    
    // C++ 방식
    cout << "[C++ 방식]" << endl;
    {
        auto cpp_ptr = make_unique<Resource>();
        cpp_ptr->use();
        // 자동 delete!
    }
    cout << "[자동 메모리 관리]" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 10. 스마트 포인터" << endl;
    cout << "========================================" << endl;
    
    Example_01_RawPointer();
    Example_02_UniquePtr();
    Example_03_SharedPtr();
    Example_04_WeakPtr();
    Example_05_CVsCPP();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 11_move_semantics.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * unique_ptr:
 * - 단독 소유
 * - 복사 불가, 이동 가능
 * - make_unique
 * 
 * shared_ptr:
 * - 공유 소유
 * - 참조 카운팅
 * - make_shared
 * 
 * weak_ptr:
 * - 약한 참조
 * - 순환 참조 방지
 * 
 * 장점:
 * - 자동 메모리 관리
 * - 메모리 누수 방지
 * - 예외 안전
 * 
 * ============================================================================
 */
