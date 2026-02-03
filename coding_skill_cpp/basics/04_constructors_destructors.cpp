/*
 * ============================================================================
 * 파일명: 04_constructors_destructors.cpp
 * 주제: 생성자와 소멸자, RAII (Resource Acquisition Is Initialization)
 * 컴파일: g++ -std=c++17 04_constructors_destructors.cpp -o 04_constructors
 * 
 * ============================================================================
 * 이 챕터에서 배울 내용
 * ============================================================================
 * 
 * - 생성자(Constructor) 종류
 * - 소멸자(Destructor)
 * - 초기화 리스트(Initializer List)
 * - RAII 패턴
 * - 복사 생성자와 대입 연산자
 * 
 * ============================================================================
 * C와의 주요 차이점
 * ============================================================================
 * 
 * C에서는:
 * - init/destroy 함수 수동 호출
 * - malloc/free 수동 관리
 * - 초기화 깜빡하면 버그
 * - 리소스 누수 위험
 * 
 * C++에서는:
 * - 생성자/소멸자 자동 호출
 * - 자동 리소스 관리
 * - 컴파일러가 강제
 * - RAII로 안전성 보장
 * 
 * 왜 C++가 더 나은가?
 * - 자동 초기화/정리
 * - 예외 안전성
 * - 리소스 누수 방지
 * - 코드 간결
 * 
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <cstring>

// C 스타일 (비교용)
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/* ============================================================================
 * 1. 기본 생성자와 소멸자
 * ============================================================================ */

// C 방식: 수동 init/destroy
typedef struct {
    char* data;
    int size;
} Buffer_C;

void Buffer_C_Init(Buffer_C* buf, int size) {
    buf->size = size;
    buf->data = (char*)malloc(size);
    printf("[C] Buffer 초기화: %d bytes\n", size);
}

void Buffer_C_Destroy(Buffer_C* buf) {
    printf("[C] Buffer 해제: %d bytes\n", buf->size);
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
}

// C++ 방식: 자동 생성자/소멸자
class Buffer_CPP {
private:
    char* data;
    int size;
    
public:
    // 생성자 (Constructor)
    Buffer_CPP(int s) : size(s) {
        data = new char[size];
        cout << "[C++] Buffer 생성: " << size << " bytes" << endl;
    }
    
    // 소멸자 (Destructor)
    ~Buffer_CPP() {
        cout << "[C++] Buffer 소멸: " << size << " bytes" << endl;
        delete[] data;
    }
    
    int getSize() const { return size; }
};

void Example_01_BasicConstructorDestructor() {
    cout << "\n=== 예제 1: 기본 생성자/소멸자 ===\n" << endl;
    
    // C 방식
    {
        cout << "[C 방식]" << endl;
        Buffer_C buf;
        Buffer_C_Init(&buf, 100);
        
        // ... 사용 ...
        
        Buffer_C_Destroy(&buf);  // 수동 호출 필수!
        // 깜빡하면 메모리 누수!
    }
    
    // C++ 방식
    {
        cout << "\n[C++ 방식]" << endl;
        Buffer_CPP buf(100);  // 생성자 자동 호출
        
        // ... 사용 ...
        
        // 소멸자 자동 호출! (블록 끝)
    }
    cout << "[C++] 블록 끝: 소멸자 자동 호출됨" << endl;
    
    /*
     * 생성자/소멸자의 장점:
     * 
     * 1. 자동 호출
     *    - 객체 생성 시: 생성자 자동 호출
     *    - 객체 소멸 시: 소멸자 자동 호출
     * 
     * 2. 초기화 보장
     *    - 생성자 없으면 컴파일 에러
     *    - 초기화 깜빡할 수 없음
     * 
     * 3. 리소스 안전성
     *    - 소멸자에서 자동 정리
     *    - 메모리 누수 방지
     */
}

/* ============================================================================
 * 2. 초기화 리스트 (Initializer List)
 * ============================================================================ */

class Student {
private:
    const int id;        // const 멤버
    string name;
    int& ref_score;      // 참조 멤버
    
public:
    // ❌ 잘못된 방법: 본문에서 초기화
    /*
    Student(int i, string n, int& score) {
        id = i;           // ❌ const 멤버 대입 불가!
        name = n;         // ✅ OK, 하지만 비효율
        ref_score = score; // ❌ 참조 재할당 불가!
    }
    */
    
    // ✅ 올바른 방법: 초기화 리스트
    Student(int i, const string& n, int& score) 
        : id(i), name(n), ref_score(score) {
        // 멤버들이 이미 초기화됨
        cout << "학생 생성: " << name << " (ID: " << id << ")" << endl;
    }
    
    void printInfo() const {
        cout << "ID: " << id << ", 이름: " << name 
             << ", 점수 참조: " << ref_score << endl;
    }
};

void Example_02_InitializerList() {
    cout << "\n=== 예제 2: 초기화 리스트 ===\n" << endl;
    
    int external_score = 95;
    Student student(12345, "홍길동", external_score);
    student.printInfo();
    
    external_score = 100;
    student.printInfo();  // 참조이므로 변경 반영
    
    /*
     * 초기화 리스트를 사용해야 하는 경우:
     * 
     * 1. const 멤버
     * 2. 참조 멤버
     * 3. 기본 생성자가 없는 멤버
     * 4. 부모 클래스 생성자 호출
     * 
     * 초기화 리스트의 장점:
     * - 효율적: 직접 초기화 (복사 없음)
     * - 필수: const/참조 멤버
     * - 명확: 초기화 순서 명시
     */
}

/* ============================================================================
 * 3. 생성자 오버로딩
 * ============================================================================ */

class Rectangle {
private:
    double width;
    double height;
    
public:
    // 기본 생성자 (Default Constructor)
    Rectangle() : width(1.0), height(1.0) {
        cout << "기본 생성자: 1x1 사각형" << endl;
    }
    
    // 매개변수 생성자 (Parameterized Constructor)
    Rectangle(double w, double h) : width(w), height(h) {
        cout << "매개변수 생성자: " << w << "x" << h << " 사각형" << endl;
    }
    
    // 정사각형용 생성자
    explicit Rectangle(double side) : width(side), height(side) {
        cout << "정사각형 생성자: " << side << "x" << side << endl;
    }
    
    double area() const {
        return width * height;
    }
    
    void print() const {
        cout << "사각형: " << width << "x" << height 
             << ", 면적: " << area() << endl;
    }
};

void Example_03_ConstructorOverloading() {
    cout << "\n=== 예제 3: 생성자 오버로딩 ===\n" << endl;
    
    Rectangle r1;              // 기본 생성자
    Rectangle r2(5.0, 3.0);    // 매개변수 생성자
    Rectangle r3(4.0);         // 정사각형 생성자
    
    r1.print();
    r2.print();
    r3.print();
    
    /*
     * explicit 키워드:
     * - 암시적 형변환 방지
     * - Rectangle r = 5.0; // ❌ explicit이면 에러
     * - Rectangle r(5.0);  // ✅ 명시적 호출은 OK
     */
}

/* ============================================================================
 * 4. 복사 생성자 (Copy Constructor)
 * ============================================================================ */

class DynamicArray {
private:
    int* data;
    int size;
    
public:
    // 생성자
    DynamicArray(int s) : size(s) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = i;
        }
        cout << "배열 생성: " << size << " 요소" << endl;
    }
    
    // 복사 생성자 (Copy Constructor)
    DynamicArray(const DynamicArray& other) : size(other.size) {
        data = new int[size];  // 깊은 복사!
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "배열 복사: " << size << " 요소" << endl;
    }
    
    // 복사 대입 연산자 (Copy Assignment Operator)
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;  // 자기 대입 방지
        
        // 기존 메모리 해제
        delete[] data;
        
        // 새로 할당 및 복사
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        
        cout << "배열 대입: " << size << " 요소" << endl;
        return *this;
    }
    
    // 소멸자
    ~DynamicArray() {
        cout << "배열 소멸: " << size << " 요소" << endl;
        delete[] data;
    }
    
    void print() const {
        cout << "배열: [";
        for (int i = 0; i < size; i++) {
            cout << data[i];
            if (i < size - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
    
    void set(int index, int value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        }
    }
};

void Example_04_CopyConstructor() {
    cout << "\n=== 예제 4: 복사 생성자 ===\n" << endl;
    
    DynamicArray arr1(5);
    arr1.print();
    
    // 복사 생성자 호출
    DynamicArray arr2 = arr1;  // 또는 DynamicArray arr2(arr1);
    arr2.print();
    
    // arr1과 arr2는 독립적
    arr1.set(0, 999);
    cout << "\narr1[0]을 999로 변경 후:" << endl;
    arr1.print();
    arr2.print();  // arr2는 변경 안 됨 (깊은 복사)
    
    // 복사 대입 연산자 호출
    DynamicArray arr3(3);
    arr3 = arr1;  // 대입
    arr3.print();
    
    /*
     * Rule of Three:
     * 
     * 다음 중 하나를 정의하면, 나머지도 정의해야 함:
     * 1. 소멸자
     * 2. 복사 생성자
     * 3. 복사 대입 연산자
     * 
     * 이유: 포인터 멤버가 있으면 얕은 복사 문제 발생
     */
}

/* ============================================================================
 * 5. RAII (Resource Acquisition Is Initialization)
 * ============================================================================ */

// C 방식: 수동 파일 관리
void C_FileExample() {
    FILE* file = fopen("test.txt", "w");
    if (file == NULL) {
        printf("파일 열기 실패\n");
        return;
    }
    
    fprintf(file, "Hello\n");
    
    // ... 복잡한 로직 ...
    // 중간에 return 하면? → 파일 닫기 깜빡!
    
    fclose(file);  // 수동 호출 필수
}

// C++ 방식: RAII 패턴
class FileRAII {
private:
    FILE* file;
    string filename;
    
public:
    FileRAII(const string& fname, const char* mode) : filename(fname) {
        file = fopen(fname.c_str(), mode);
        if (file) {
            cout << "[RAII] 파일 열기: " << filename << endl;
        } else {
            cout << "[RAII] 파일 열기 실패: " << filename << endl;
        }
    }
    
    ~FileRAII() {
        if (file) {
            fclose(file);
            cout << "[RAII] 파일 닫기: " << filename << endl;
        }
    }
    
    // 복사 방지 (파일 핸들은 복사 불가)
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;
    
    bool isOpen() const {
        return file != nullptr;
    }
    
    void write(const string& text) {
        if (file) {
            fprintf(file, "%s", text.c_str());
        }
    }
};

void CPP_FileExample() {
    FileRAII file("test_raii.txt", "w");
    
    if (file.isOpen()) {
        file.write("Hello from RAII\n");
        
        // ... 복잡한 로직 ...
        // 어디서 return 해도 소멸자가 자동으로 파일 닫음!
        
    }
    // 블록 끝: 자동으로 파일 닫힘
}

void Example_05_RAII() {
    cout << "\n=== 예제 5: RAII 패턴 ===\n" << endl;
    
    cout << "[C++ RAII 사용]" << endl;
    CPP_FileExample();
    cout << "[블록 끝: 파일 자동 닫힘]\n" << endl;
    
    /*
     * RAII (Resource Acquisition Is Initialization):
     * 
     * 핵심 개념:
     * - 리소스 획득 = 초기화
     * - 생성자에서 리소스 획득
     * - 소멸자에서 리소스 해제
     * 
     * 장점:
     * 1. 자동 리소스 관리
     * 2. 예외 안전성 (예외 발생해도 소멸자 호출)
     * 3. 리소스 누수 방지
     * 4. 깜빡할 수 없음
     * 
     * 적용 대상:
     * - 파일 (FILE*)
     * - 메모리 (malloc/new)
     * - 뮤텍스 (mutex)
     * - 소켓 (socket)
     * - 모든 리소스!
     */
}

/* ============================================================================
 * 6. 실무 예제: 스마트 락 (Mutex RAII)
 * ============================================================================ */

// 가상의 Mutex (실제로는 <mutex> 사용)
class SimpleMutex {
public:
    void lock() {
        cout << "🔒 뮤텍스 잠금" << endl;
    }
    
    void unlock() {
        cout << "🔓 뮤텍스 해제" << endl;
    }
};

// C 방식: 수동 lock/unlock
void C_ThreadSafeFunction(SimpleMutex* mutex) {
    mutex->lock();
    
    // ... 임계 영역 ...
    cout << "작업 수행 중..." << endl;
    
    // 중간에 return 하면? → unlock 깜빡!
    
    mutex->unlock();  // 수동 호출 필수
}

// C++ 방식: RAII (Lock Guard)
class LockGuard {
private:
    SimpleMutex& mutex;
    
public:
    explicit LockGuard(SimpleMutex& m) : mutex(m) {
        mutex.lock();
    }
    
    ~LockGuard() {
        mutex.unlock();
    }
    
    // 복사 방지
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

void CPP_ThreadSafeFunction(SimpleMutex& mutex) {
    LockGuard guard(mutex);  // 자동 lock
    
    // ... 임계 영역 ...
    cout << "작업 수행 중..." << endl;
    
    // 어디서 return 해도 자동 unlock!
    
    // 블록 끝: 자동 unlock
}

void Example_06_RealWorldRAII() {
    cout << "\n=== 예제 6: 실무 RAII (Lock Guard) ===\n" << endl;
    
    SimpleMutex mutex;
    
    cout << "[C 방식]" << endl;
    C_ThreadSafeFunction(&mutex);
    
    cout << "\n[C++ RAII 방식]" << endl;
    CPP_ThreadSafeFunction(mutex);
    cout << "[블록 끝: 자동 unlock]" << endl;
    
    /*
     * 실제 C++에서:
     * std::lock_guard<std::mutex> guard(mutex);
     * 
     * 이것이 바로 RAII 패턴!
     */
}

/* ============================================================================
 * 7. = default, = delete
 * ============================================================================ */

class NonCopyable {
private:
    int* data;
    
public:
    NonCopyable() : data(new int(0)) {
        cout << "NonCopyable 생성" << endl;
    }
    
    ~NonCopyable() {
        delete data;
        cout << "NonCopyable 소멸" << endl;
    }
    
    // 복사 금지
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    
    // 이동은 허용 (다음 챕터에서)
    // NonCopyable(NonCopyable&&) = default;
    // NonCopyable& operator=(NonCopyable&&) = default;
};

void Example_07_DefaultDelete() {
    cout << "\n=== 예제 7: = default, = delete ===\n" << endl;
    
    NonCopyable obj1;
    
    // NonCopyable obj2 = obj1;  // ❌ 에러! 복사 금지
    // NonCopyable obj3;
    // obj3 = obj1;              // ❌ 에러! 대입 금지
    
    cout << "복사가 금지되어 안전함" << endl;
    
    /*
     * = default:
     * - 컴파일러가 자동 생성
     * - 명시적으로 기본 동작 요청
     * 
     * = delete:
     * - 함수 삭제 (사용 불가)
     * - 복사 방지, 특정 오버로딩 방지
     * 
     * 활용:
     * - 싱글톤 패턴 (복사 금지)
     * - RAII 클래스 (복사 금지)
     * - 이동만 허용 (복사 금지)
     */
}

/* ============================================================================
 * main 함수
 * ============================================================================ */

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 04. 생성자와 소멸자" << endl;
    cout << "========================================" << endl;
    
    Example_01_BasicConstructorDestructor();
    Example_02_InitializerList();
    Example_03_ConstructorOverloading();
    Example_04_CopyConstructor();
    Example_05_RAII();
    Example_06_RealWorldRAII();
    Example_07_DefaultDelete();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 05_inheritance_polymorphism.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 연습 문제
 * ============================================================================
 * 
 * 1. C의 init/destroy 함수를 생성자/소멸자로 변환해보세요.
 * 2. RAII 패턴으로 리소스를 안전하게 관리하는 클래스를 만들어보세요.
 * 3. 복사 생성자와 복사 대입 연산자를 구현해보세요.
 * 4. 초기화 리스트를 사용하는 클래스를 만들어보세요.
 * 
 * ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * 생성자:
 * - 객체 생성 시 자동 호출
 * - 멤버 초기화 담당
 * - 오버로딩 가능
 * 
 * 소멸자:
 * - 객체 소멸 시 자동 호출
 * - 리소스 해제 담당
 * - 오버로딩 불가 (하나만)
 * 
 * 초기화 리스트:
 * - const/참조 멤버 필수
 * - 효율적 초기화
 * 
 * RAII:
 * - 생성자에서 리소스 획득
 * - 소멸자에서 리소스 해제
 * - 자동 관리, 안전성
 * 
 * Rule of Three:
 * - 소멸자
 * - 복사 생성자
 * - 복사 대입 연산자
 * 
 * ============================================================================
 */
