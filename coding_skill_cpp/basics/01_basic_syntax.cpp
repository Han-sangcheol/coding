/*
 * ============================================================================
 * 파일명: 01_basic_syntax.cpp
 * 주제: C++의 기본 문법 (C와의 차이점)
 * 컴파일: g++ -std=c++17 01_basic_syntax.cpp -o 01_basic_syntax
 * 
 * ============================================================================
 * 이 챕터에서 배울 내용
 * ============================================================================
 * 
 * - iostream vs stdio (cout vs printf)
 * - namespace 개념
 * - auto 키워드 (자동 타입 추론)
 * - std::string (C++의 문자열 클래스)
 * - bool 타입 (C++의 진짜 boolean)
 * 
 * ============================================================================
 * C와의 주요 차이점
 * ============================================================================
 * 
 * C에서는:
 * - printf/scanf로 입출력
 * - char*로 문자열 관리 (수동)
 * - 타입을 항상 명시
 * - bool이 없음 (int 사용)
 * 
 * C++에서는:
 * - cout/cin으로 입출력 (타입 안전)
 * - std::string으로 자동 메모리 관리
 * - auto로 타입 추론 가능
 * - bool 타입 내장
 * 
 * 왜 C++가 더 나은가?
 * - 타입 안전성: cout은 타입 체크를 컴파일 타임에 수행
 * - 메모리 안전성: string은 자동으로 메모리 관리
 * - 코드 간결성: auto로 긴 타입명 생략
 * - 가독성: 연산자 오버로딩으로 자연스러운 코드
 * 
 * ============================================================================
 */

#include <iostream>   // cout, cin
#include <string>     // std::string
#include <vector>     // std::vector (예제용)

// C 스타일 (비교용)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ============================================================================
 * 1. 입출력: printf vs cout
 * ============================================================================ */

void Example_01_IO() {
    std::cout << "\n=== 예제 1: 입출력 비교 ===\n" << std::endl;
    
    // C 방식
    printf("[C] printf: Hello, World!\n");
    printf("[C] 정수: %d, 실수: %.2f, 문자열: %s\n", 42, 3.14, "test");
    
    // C++ 방식
    std::cout << "[C++] cout: Hello, World!" << std::endl;
    std::cout << "[C++] 정수: " << 42 
              << ", 실수: " << 3.14 
              << ", 문자열: " << "test" << std::endl;
    
    /* 
     * C와의 차이점:
     * 
     * C (printf):
     * - 포맷 문자열 필요 (%d, %f, %s)
     * - 타입 불일치 시 런타임 에러 가능
     * - 잘못된 포맷은 undefined behavior
     * 
     * C++ (cout):
     * - 포맷 문자열 불필요
     * - 타입 자동 추론 (연산자 오버로딩)
     * - 컴파일 타임에 타입 체크
     * - << 연산자로 체이닝 가능
     */
    
    // 타입 안전성 비교
    int num = 100;
    
    // C: 타입 불일치 가능 (위험!)
    // printf("%s\n", num);  // 컴파일 경고만, 실행 시 크래시
    
    // C++: 타입 불일치 불가능 (안전!)
    std::cout << num << std::endl;  // 항상 안전
}

/* ============================================================================
 * 2. namespace: 이름 충돌 방지
 * ============================================================================ */

// C 방식: 이름 충돌 방지를 위해 접두사 사용
void c_print_message() {
    printf("C: 접두사로 구분\n");
}

void c_logger_print_message() {
    printf("C: logger의 print\n");
}

// C++ 방식: namespace로 구분
namespace my_lib {
    void print_message() {
        std::cout << "C++: my_lib의 print" << std::endl;
    }
}

namespace logger {
    void print_message() {
        std::cout << "C++: logger의 print" << std::endl;
    }
}

void Example_02_Namespace() {
    std::cout << "\n=== 예제 2: namespace ===\n" << std::endl;
    
    // C 방식
    c_print_message();
    c_logger_print_message();
    
    // C++ 방식
    my_lib::print_message();
    logger::print_message();
    
    /*
     * namespace의 장점:
     * 
     * 1. 이름 충돌 방지
     *    - 여러 라이브러리에 같은 함수명 존재 가능
     *    - namespace로 명확하게 구분
     * 
     * 2. 코드 구조화
     *    - 관련 함수들을 namespace로 그룹화
     *    - 모듈화 향상
     * 
     * 3. using 선언
     *    - using namespace std; (전체)
     *    - using std::cout; (선택적)
     */
    
    // using 선언 예제
    {
        using namespace std;  // 이 블록 안에서만 유효
        cout << "std:: 생략 가능" << endl;
    }
    
    // 블록 밖에서는 다시 std:: 필요
    std::cout << "std:: 다시 필요" << std::endl;
}

/* ============================================================================
 * 3. auto 키워드: 타입 자동 추론
 * ============================================================================ */

void Example_03_Auto() {
    std::cout << "\n=== 예제 3: auto 키워드 ===\n" << std::endl;
    
    // C 방식: 타입 명시 필수
    int c_num = 42;
    double c_pi = 3.14159;
    const char* c_str = "Hello";
    
    std::cout << "[C] int: " << c_num << std::endl;
    std::cout << "[C] double: " << c_pi << std::endl;
    std::cout << "[C] const char*: " << c_str << std::endl;
    
    // C++ 방식: auto로 타입 추론
    auto cpp_num = 42;           // int로 추론
    auto cpp_pi = 3.14159;       // double로 추론
    auto cpp_str = "Hello";      // const char*로 추론
    auto cpp_string = std::string("Hello");  // std::string
    
    std::cout << "[C++] auto (int): " << cpp_num << std::endl;
    std::cout << "[C++] auto (double): " << cpp_pi << std::endl;
    std::cout << "[C++] auto (const char*): " << cpp_str << std::endl;
    std::cout << "[C++] auto (std::string): " << cpp_string << std::endl;
    
    /*
     * auto의 장점:
     * 
     * 1. 긴 타입명 생략
     *    std::vector<std::pair<int, std::string>>::iterator it;
     *    auto it = vec.begin();  // 훨씬 짧음!
     * 
     * 2. 리팩토링 용이
     *    - 반환 타입 변경 시 auto는 자동 적응
     * 
     * 3. 템플릿 코드 간결화
     *    - 복잡한 타입을 auto로 숨김
     */
    
    // 복잡한 타입 예제
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // C 스타일 (C++에서도 가능)
    for (std::vector<int>::iterator it = numbers.begin(); 
         it != numbers.end(); ++it) {
        // 타입명이 너무 김!
    }
    
    // auto 사용
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        // 간결함!
    }
    
    // range-based for (더 간결!)
    for (auto num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    /*
     * 주의사항:
     * - auto는 참조를 제거함
     * - auto& 또는 const auto&로 참조 유지
     */
    
    std::string str = "test";
    auto copy = str;        // 복사본 생성
    auto& ref = str;        // 참조
    const auto& cref = str; // const 참조
    
    copy[0] = 'T';  // copy만 변경
    std::cout << "원본: " << str << ", 복사본: " << copy << std::endl;
}

/* ============================================================================
 * 4. std::string: C++의 문자열
 * ============================================================================ */

void Example_04_String() {
    std::cout << "\n=== 예제 4: std::string ===\n" << std::endl;
    
    // ---- C 방식: char* (수동 관리) ----
    {
        std::cout << "[C 방식]" << std::endl;
        
        // 정적 문자열
        const char* c_str1 = "Hello";
        
        // 동적 문자열 (malloc)
        char* c_str2 = (char*)malloc(100);
        strcpy(c_str2, "World");
        
        // 연결 (strcat)
        char* c_result = (char*)malloc(200);
        strcpy(c_result, c_str1);
        strcat(c_result, " ");
        strcat(c_result, c_str2);
        
        printf("C 결과: %s\n", c_result);
        
        // 수동 해제 필수!
        free(c_str2);
        free(c_result);
        
        /*
         * C 방식의 문제점:
         * - malloc/free 수동 관리
         * - 버퍼 오버플로 위험
         * - 길이 계산 필요
         * - 메모리 누수 가능
         */
    }
    
    // ---- C++ 방식: std::string (자동 관리) ----
    {
        std::cout << "\n[C++ 방식]" << std::endl;
        
        // 생성
        std::string cpp_str1 = "Hello";
        std::string cpp_str2("World");  // 생성자
        std::string cpp_str3 = cpp_str1;  // 복사 (자동)
        
        // 연결 (+ 연산자)
        std::string cpp_result = cpp_str1 + " " + cpp_str2;
        
        std::cout << "C++ 결과: " << cpp_result << std::endl;
        
        // 메모리 자동 해제됨!
        
        /*
         * std::string의 장점:
         * - 자동 메모리 관리
         * - 버퍼 오버플로 방지
         * - + 연산자로 간편한 연결
         * - 다양한 멤버 함수
         */
    }
    
    // ---- std::string 주요 기능 ----
    {
        std::cout << "\n[std::string 기능]" << std::endl;
        
        std::string str = "Hello, C++ World!";
        
        // 길이
        std::cout << "길이: " << str.length() << std::endl;
        std::cout << "크기: " << str.size() << std::endl;
        
        // 부분 문자열
        std::cout << "부분 문자열: " << str.substr(0, 5) << std::endl;
        
        // 검색
        size_t pos = str.find("C++");
        if (pos != std::string::npos) {
            std::cout << "C++ 발견 위치: " << pos << std::endl;
        }
        
        // 대체
        std::string str2 = str;
        str2.replace(pos, 3, "Python");
        std::cout << "대체 후: " << str2 << std::endl;
        
        // 추가
        str += " Amazing!";
        std::cout << "추가 후: " << str << std::endl;
        
        // C 문자열과 호환
        const char* c_str = str.c_str();
        printf("C 함수에서 사용: %s\n", c_str);
    }
    
    // ---- C vs C++ 비교: 문자열 연결 ----
    {
        std::cout << "\n[성능 비교: 문자열 1000번 연결]" << std::endl;
        
        // C 방식 (비효율적)
        /*
        char* c_result = (char*)malloc(1);
        c_result[0] = '\0';
        for (int i = 0; i < 1000; i++) {
            char temp[10];
            sprintf(temp, "%d", i);
            char* new_result = (char*)malloc(strlen(c_result) + strlen(temp) + 1);
            strcpy(new_result, c_result);
            strcat(new_result, temp);
            free(c_result);
            c_result = new_result;
        }
        free(c_result);
        // 매번 malloc/free!
        */
        
        // C++ 방식 (효율적)
        std::string cpp_result;
        for (int i = 0; i < 1000; i++) {
            cpp_result += std::to_string(i);
        }
        // 자동 메모리 관리!
        
        std::cout << "C++: 자동 메모리 관리로 간단하고 안전!" << std::endl;
    }
}

/* ============================================================================
 * 5. bool 타입: C++의 진짜 boolean
 * ============================================================================ */

void Example_05_Bool() {
    std::cout << "\n=== 예제 5: bool 타입 ===\n" << std::endl;
    
    // C 방식: int로 boolean 표현
    {
        std::cout << "[C 방식]" << std::endl;
        
        int c_true = 1;
        int c_false = 0;
        
        if (c_true) {
            printf("C: true (실제로는 1)\n");
        }
        
        printf("C true 크기: %zu bytes\n", sizeof(c_true));  // 4 bytes
        
        // 문제: 2, 3, -1도 모두 true
        int weird = 42;
        if (weird) {
            printf("C: 42도 true!\n");
        }
    }
    
    // C++ 방식: 진짜 bool 타입
    {
        std::cout << "\n[C++ 방식]" << std::endl;
        
        bool cpp_true = true;
        bool cpp_false = false;
        
        if (cpp_true) {
            std::cout << "C++: true" << std::endl;
        }
        
        std::cout << "C++ bool 크기: " << sizeof(cpp_true) 
                  << " byte" << std::endl;  // 1 byte
        
        // 명확한 true/false
        bool result = (10 > 5);
        std::cout << "10 > 5 = " << (result ? "true" : "false") << std::endl;
        
        // boolalpha 조작자로 true/false 출력
        std::cout << std::boolalpha;  // true/false로 출력
        std::cout << "result = " << result << std::endl;
        std::cout << std::noboolalpha;  // 다시 1/0으로 출력
        std::cout << "result = " << result << std::endl;
    }
    
    /*
     * bool의 장점:
     * - 타입 안전성: bool은 true/false만 가능
     * - 메모리 효율: 1 byte (int는 4 bytes)
     * - 코드 가독성: 의도가 명확
     */
}

/* ============================================================================
 * 6. 실무 예제: C vs C++ 비교
 * ============================================================================ */

// C 방식: 구조체 + 함수
typedef struct {
    char name[50];
    int age;
    double salary;
} Employee_C;

void Employee_C_Print(const Employee_C* emp) {
    printf("C: %s, %d세, %.2f원\n", emp->name, emp->age, emp->salary);
}

// C++ 방식: 클래스 (간단 버전, 다음 챕터에서 자세히)
class Employee_CPP {
public:
    std::string name;
    int age;
    double salary;
    
    void print() const {
        std::cout << "C++: " << name << ", " << age << "세, " 
                  << salary << "원" << std::endl;
    }
};

void Example_06_RealWorld() {
    std::cout << "\n=== 예제 6: 실무 예제 ===\n" << std::endl;
    
    // C 방식
    Employee_C c_emp;
    strncpy(c_emp.name, "홍길동", sizeof(c_emp.name) - 1);
    c_emp.name[sizeof(c_emp.name) - 1] = '\0';  // null 종료 보장
    c_emp.age = 30;
    c_emp.salary = 5000000.0;
    Employee_C_Print(&c_emp);
    
    // C++ 방식
    Employee_CPP cpp_emp;
    cpp_emp.name = "홍길동";  // 간단!
    cpp_emp.age = 30;
    cpp_emp.salary = 5000000.0;
    cpp_emp.print();
    
    /*
     * 비교:
     * - C: strncpy, 버퍼 관리, null 종료 신경 써야 함
     * - C++: 그냥 = 로 대입, 자동 관리
     */
}

/* ============================================================================
 * 7. 주의사항 및 모범 사례
 * ============================================================================ */

void Example_07_BestPractices() {
    std::cout << "\n=== 예제 7: 모범 사례 ===\n" << std::endl;
    
    // 1. using namespace std; 사용 주의
    {
        std::cout << "[1] using namespace std 주의" << std::endl;
        
        // ❌ 헤더 파일에서 절대 사용 금지
        // ❌ 전역 스코프에서 사용 지양
        
        // ✅ 함수 내부에서만 사용
        // ✅ 또는 특정 항목만 using
        using std::cout;
        using std::endl;
        cout << "선택적 using은 OK" << endl;
    }
    
    // 2. auto 남용 주의
    {
        std::cout << "\n[2] auto 적절히 사용" << std::endl;
        
        // ✅ 좋은 사용
        auto str = std::string("Good");  // 타입 명확
        auto it = str.begin();            // iterator 타입 복잡
        
        // ❌ 나쁜 사용
        // auto x = 10;  // int인지 long인지 불명확
        // 명시적 타입이 나을 때도 있음
        
        int count = 10;  // 명확한 의도
        std::cout << "타입이 명확할 때는 명시" << std::endl;
    }
    
    // 3. string vs const char*
    {
        std::cout << "\n[3] 문자열 타입 선택" << std::endl;
        
        // ✅ 변경 가능한 문자열: std::string
        std::string mutable_str = "Hello";
        mutable_str += " World";
        
        // ✅ 상수 문자열: const char* 또는 std::string
        const char* literal = "Hello";  // 리터럴
        const std::string str_literal = "Hello";  // std::string도 가능
        
        // ⚠️ C 함수와 호환 필요 시: c_str() 사용
        std::string cpp_str = "test";
        printf("C 함수: %s\n", cpp_str.c_str());
    }
    
    // 4. 초기화 방식
    {
        std::cout << "\n[4] 초기화 권장 방식" << std::endl;
        
        // C++11 이후: 중괄호 초기화 (Uniform Initialization)
        int a{10};              // ✅ 권장
        std::string b{"Hello"}; // ✅ 권장
        
        // 기존 방식도 OK
        int c = 10;
        std::string d = "Hello";
        
        std::cout << "중괄호 초기화: 타입 안전성 향상" << std::endl;
    }
}

/* ============================================================================
 * main 함수
 * ============================================================================ */

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C++ 기초: 01. 기본 문법" << std::endl;
    std::cout << "========================================" << std::endl;
    
    Example_01_IO();
    Example_02_Namespace();
    Example_03_Auto();
    Example_04_String();
    Example_05_Bool();
    Example_06_RealWorld();
    Example_07_BestPractices();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "학습 완료!" << std::endl;
    std::cout << "다음: 02_references_vs_pointers.cpp" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

/* ============================================================================
 * 연습 문제
 * ============================================================================
 * 
 * 1. C의 printf를 사용한 코드를 cout으로 변환해보세요.
 * 2. auto를 사용하여 복잡한 타입을 간결하게 만들어보세요.
 * 3. char*로 작성된 문자열 코드를 std::string으로 변환해보세요.
 * 4. namespace를 만들어 함수들을 그룹화해보세요.
 * 
 * ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * 1. cout/cin: 타입 안전한 입출력
 * 2. namespace: 이름 충돌 방지
 * 3. auto: 타입 자동 추론
 * 4. std::string: 자동 메모리 관리
 * 5. bool: 진짜 boolean 타입
 * 
 * C에서 C++로 넘어갈 때:
 * - printf → cout
 * - char* → std::string
 * - int (bool용) → bool
 * - 수동 메모리 관리 → 자동 관리
 * 
 * ============================================================================
 */
