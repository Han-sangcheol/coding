/*
 * ============================================================================
 * 파일명: 00_function_pointer_basics.cpp
 * 기능: C++ 함수 객체와 람다 완전 정복
 * 
 * C 버전과의 주요 차이점:
 *   - 함수 포인터 → std::function (타입 안전, 유연함)
 *   - typedef → using (C++11, 더 읽기 쉬움)
 *   - 구조체 + 함수 포인터 → 클래스 + 가상 함수
 *   - 콜백 → 람다 표현식
 *   - 배열 → std::vector, std::array
 * 
 * C++ 추가 기능:
 *   - 람다 캡처 (값, 참조)
 *   - std::bind
 *   - 멤버 함수 포인터
 *   - 함수 객체 (Functor)
 *   - 완벽한 전달 (Perfect Forwarding)
 * 
 * ============================================================================
 * 📚 학습 목차
 * ============================================================================
 * 
 * 1단계: C++ std::function 기초
 * 2단계: 람다 표현식
 * 3단계: 함수 객체 (Functor)
 * 4단계: std::bind와 partial application
 * 5단계: 클래스 기반 Callback
 * 6단계: 템플릿 콜백
 * 7단계: 실무 예제 (Observer, Command, Strategy)
 * 8단계: C vs C++ 비교
 * 
 * ============================================================================
 */

#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <chrono>

using namespace std;

namespace patterns {

/* ============================================================================
 * 1단계: C++ std::function 기초
 * ============================================================================
 * 
 * C에서의 함수 포인터:
 *   void (*fp)(int);
 * 
 * C++에서의 std::function:
 *   std::function<void(int)>
 * 
 * 장점:
 *   - 타입 안전
 *   - 람다, 함수 객체, 멤버 함수 모두 저장 가능
 *   - 상태를 가질 수 있음 (람다 캡처)
 * 
 * ============================================================================
 */

void HelloWorld() {
    cout << "Hello, World!" << endl;
}

void Goodbye() {
    cout << "Goodbye!" << endl;
}

void Step1_StdFunction() {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  1단계: std::function 기초" << endl;
    cout << "========================================\n" << endl;
    
    // C 스타일 함수 포인터
    cout << "[C 스타일]" << endl;
    void (*fp_old)(void) = HelloWorld;
    fp_old();
    
    // C++ std::function
    cout << "\n[C++ std::function]" << endl;
    function<void()> fp = HelloWorld;
    fp();
    
    fp = Goodbye;
    fp();
    
    // 매개변수가 있는 함수
    cout << "\n[매개변수가 있는 함수]" << endl;
    function<int(int, int)> calc;
    
    calc = [](int a, int b) { return a + b; };
    cout << "10 + 5 = " << calc(10, 5) << endl;
    
    calc = [](int a, int b) { return a * b; };
    cout << "10 * 5 = " << calc(10, 5) << endl;
    
    cout << "\n💡 std::function은 타입 안전하고 유연합니다!" << endl;
}

/* ============================================================================
 * 2단계: 람다 표현식
 * ============================================================================
 * 
 * C에는 없는 C++의 강력한 기능!
 * 
 * 기본 문법:
 *   [캡처](매개변수) -> 반환타입 { 본문 }
 * 
 * 캡처 종류:
 *   []        - 아무것도 캡처 안 함
 *   [=]       - 값으로 모두 캡처
 *   [&]       - 참조로 모두 캡처
 *   [x, &y]   - x는 값, y는 참조로 캡처
 *   [this]    - 멤버 변수 접근
 * 
 * ============================================================================
 */

void Step2_Lambda() {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  2단계: 람다 표현식" << endl;
    cout << "========================================\n" << endl;
    
    // 기본 람다
    cout << "[기본 람다]" << endl;
    auto greet = []() {
        cout << "Hello from lambda!" << endl;
    };
    greet();
    
    // 매개변수가 있는 람다
    cout << "\n[매개변수 람다]" << endl;
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "3 + 7 = " << add(3, 7) << endl;
    
    // 캡처 - 값으로
    cout << "\n[값 캡처]" << endl;
    int multiplier = 10;
    auto multiply = [multiplier](int x) {
        return x * multiplier;
    };
    cout << "5 * 10 = " << multiply(5) << endl;
    
    // 캡처 - 참조로
    cout << "\n[참조 캡처]" << endl;
    int counter = 0;
    auto increment = [&counter]() {
        counter++;
        cout << "Counter: " << counter << endl;
    };
    increment();
    increment();
    increment();
    
    // 제네릭 람다 (C++14)
    cout << "\n[제네릭 람다 (auto)]" << endl;
    auto generic_add = [](auto a, auto b) {
        return a + b;
    };
    cout << "정수: " << generic_add(10, 20) << endl;
    cout << "실수: " << generic_add(3.14, 2.86) << endl;
    
    cout << "\n💡 람다는 C++에서 콜백을 매우 쉽게 만들어줍니다!" << endl;
}

/* ============================================================================
 * 3단계: 함수 객체 (Functor)
 * ============================================================================
 * 
 * operator()를 오버로드한 클래스
 * 
 * 장점:
 *   - 상태를 가질 수 있음
 *   - 람다보다 복잡한 로직에 적합
 *   - 템플릿과 함께 사용 가능
 * 
 * ============================================================================
 */

class Adder {
    int offset;
public:
    Adder(int offset) : offset(offset) {}
    
    int operator()(int x) const {
        return x + offset;
    }
};

class Counter {
    int count = 0;
public:
    void operator()() {
        count++;
        cout << "  Count: " << count << endl;
    }
    
    int getCount() const { return count; }
};

void Step3_Functor() {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  3단계: 함수 객체 (Functor)" << endl;
    cout << "========================================\n" << endl;
    
    // Adder 사용
    cout << "[Adder 함수 객체]" << endl;
    Adder add10(10);
    Adder add100(100);
    
    cout << "5 + 10 = " << add10(5) << endl;
    cout << "5 + 100 = " << add100(5) << endl;
    
    // Counter 사용
    cout << "\n[Counter 함수 객체]" << endl;
    Counter counter;
    counter();
    counter();
    counter();
    cout << "Total: " << counter.getCount() << endl;
    
    // STL 알고리즘과 함께 사용
    cout << "\n[STL 알고리즘과 함께]" << endl;
    vector<int> numbers = {1, 2, 3, 4, 5};
    
    cout << "원본: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // 각 원소에 10 더하기
    transform(numbers.begin(), numbers.end(), numbers.begin(), Adder(10));
    
    cout << "+10: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    cout << "\n💡 함수 객체는 상태를 가진 콜백을 만들 수 있습니다!" << endl;
}

/* ============================================================================
 * 4단계: std::bind와 partial application
 * ============================================================================
 */

int Divide(int a, int b) {
    return a / b;
}

void PrintMessage(const string& prefix, const string& message) {
    cout << prefix << ": " << message << endl;
}

void Step4_Bind() {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  4단계: std::bind" << endl;
    cout << "========================================\n" << endl;
    
    // 기본 bind
    cout << "[기본 bind]" << endl;
    auto divide_by_2 = bind(Divide, placeholders::_1, 2);
    cout << "10 / 2 = " << divide_by_2(10) << endl;
    cout << "20 / 2 = " << divide_by_2(20) << endl;
    
    // 메시지 프리픽스 고정
    cout << "\n[메시지 프리픽스 고정]" << endl;
    auto info = bind(PrintMessage, "[INFO]", placeholders::_1);
    auto error = bind(PrintMessage, "[ERROR]", placeholders::_1);
    
    info("시스템 시작");
    error("오류 발생!");
    
    // 람다가 더 간단한 경우
    cout << "\n[람다로 동일한 효과]" << endl;
    auto divide_by_2_lambda = [](int x) { return Divide(x, 2); };
    cout << "15 / 2 = " << divide_by_2_lambda(15) << endl;
    
    cout << "\n💡 bind는 유용하지만, 대부분 람다가 더 간단합니다!" << endl;
}

/* ============================================================================
 * 5단계: 클래스 기반 Callback
 * ============================================================================
 */

class Button {
    function<void()> onClick;
public:
    void setOnClick(function<void()> callback) {
        onClick = callback;
    }
    
    void click() {
        cout << "[Button] 클릭됨!" << endl;
        if (onClick) {
            onClick();
        }
    }
};

class EventHandler {
    string name;
public:
    EventHandler(const string& name) : name(name) {}
    
    void onEvent() {
        cout << "  → [" << name << "] 이벤트 처리" << endl;
    }
};

void Step5_ClassCallback() {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  5단계: 클래스 기반 Callback" << endl;
    cout << "========================================\n" << endl;
    
    Button button;
    
    // 일반 함수
    cout << "[일반 함수 콜백]" << endl;
    button.setOnClick([]() {
        cout << "  → 버튼이 눌렸습니다!" << endl;
    });
    button.click();
    
    // 멤버 함수
    cout << "\n[멤버 함수 콜백]" << endl;
    EventHandler handler("MyHandler");
    button.setOnClick([&handler]() {
        handler.onEvent();
    });
    button.click();
    
    // 복잡한 로직
    cout << "\n[복잡한 콜백]" << endl;
    int click_count = 0;
    button.setOnClick([&click_count]() {
        click_count++;
        cout << "  → 클릭 횟수: " << click_count << endl;
        if (click_count >= 3) {
            cout << "  → 3회 클릭 달성!" << endl;
        }
    });
    button.click();
    button.click();
    button.click();
    
    cout << "\n💡 C++의 람다로 상태를 가진 콜백을 쉽게 만들 수 있습니다!" << endl;
}

/* ============================================================================
 * 6단계: 템플릿 콜백
 * ============================================================================
 */

template<typename T>
class Observable {
    vector<function<void(const T&)>> observers;
public:
    void attach(function<void(const T&)> observer) {
        observers.push_back(observer);
    }
    
    void notify(const T& data) {
        cout << "[Observable] 통지: " << observers.size() << "개 옵저버" << endl;
        for (auto& obs : observers) {
            obs(data);
        }
    }
};

void Step6_TemplateCallback() {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  6단계: 템플릿 콜백" << endl;
    cout << "========================================\n" << endl;
    
    // int 타입 Observable
    cout << "[정수 Observable]" << endl;
    Observable<int> intObservable;
    
    intObservable.attach([](const int& value) {
        cout << "  → Observer 1: " << value << endl;
    });
    
    intObservable.attach([](const int& value) {
        cout << "  → Observer 2: " << value * 2 << endl;
    });
    
    intObservable.notify(10);
    
    // string 타입 Observable
    cout << "\n[문자열 Observable]" << endl;
    Observable<string> stringObservable;
    
    stringObservable.attach([](const string& msg) {
        cout << "  → Logger: " << msg << endl;
    });
    
    stringObservable.attach([](const string& msg) {
        cout << "  → Display: [" << msg << "]" << endl;
    });
    
    stringObservable.notify("Hello, Template!");
    
    cout << "\n💡 템플릿으로 타입 안전한 제네릭 콜백을 만들 수 있습니다!" << endl;
}

/* ============================================================================
 * 7단계: 실무 예제
 * ============================================================================
 */

// Command Pattern
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class LambdaCommand : public Command {
    function<void()> executeFunc;
    function<void()> undoFunc;
public:
    LambdaCommand(function<void()> exec, function<void()> undo)
        : executeFunc(exec), undoFunc(undo) {}
    
    void execute() override { executeFunc(); }
    void undo() override { undoFunc(); }
};

class CommandInvoker {
    vector<unique_ptr<Command>> history;
public:
    void execute(unique_ptr<Command> cmd) {
        cout << "[Invoker] 명령 실행" << endl;
        cmd->execute();
        history.push_back(move(cmd));
    }
    
    void undo() {
        if (!history.empty()) {
            cout << "[Invoker] 명령 취소" << endl;
            history.back()->undo();
            history.pop_back();
        }
    }
};

// Strategy Pattern
class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(vector<int>& data) = 0;
};

class Sorter {
    unique_ptr<SortStrategy> strategy;
public:
    void setStrategy(unique_ptr<SortStrategy> s) {
        strategy = move(s);
    }
    
    void sort(vector<int>& data) {
        if (strategy) {
            strategy->sort(data);
        }
    }
};

// 하지만 C++에서는 람다가 더 간단!
class SimpleSorter {
    function<void(vector<int>&)> strategy;
public:
    void setStrategy(function<void(vector<int>&)> s) {
        strategy = s;
    }
    
    void sort(vector<int>& data) {
        if (strategy) {
            strategy(data);
        }
    }
};

void Step7_RealWorld() {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  7단계: 실무 예제" << endl;
    cout << "========================================\n" << endl;
    
    // Command Pattern
    cout << "[Command Pattern]" << endl;
    int value = 0;
    CommandInvoker invoker;
    
    auto increment = make_unique<LambdaCommand>(
        [&value]() {
            value++;
            cout << "  Value++ = " << value << endl;
        },
        [&value]() {
            value--;
            cout << "  Value-- = " << value << endl;
        }
    );
    
    invoker.execute(move(increment));
    invoker.execute(make_unique<LambdaCommand>(
        [&value]() { value += 10; cout << "  Value+10 = " << value << endl; },
        [&value]() { value -= 10; cout << "  Value-10 = " << value << endl; }
    ));
    
    invoker.undo();
    invoker.undo();
    
    // Strategy Pattern (람다 버전)
    cout << "\n[Strategy Pattern (람다)]" << endl;
    SimpleSorter sorter;
    vector<int> numbers = {5, 2, 8, 1, 9};
    
    cout << "원본: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    sorter.setStrategy([](vector<int>& data) {
        sort(data.begin(), data.end());
    });
    sorter.sort(numbers);
    
    cout << "오름차순: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    sorter.setStrategy([](vector<int>& data) {
        sort(data.begin(), data.end(), greater<int>());
    });
    sorter.sort(numbers);
    
    cout << "내림차순: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    cout << "\n💡 C++의 람다는 디자인 패턴을 매우 간결하게 만듭니다!" << endl;
}

/* ============================================================================
 * 8단계: C vs C++ 비교
 * ============================================================================
 */

void Step8_Comparison() {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  8단계: C vs C++ 비교" << endl;
    cout << "========================================\n" << endl;
    
    cout << "C 함수 포인터 vs C++ std::function:" << endl;
    cout << "\n[C 버전]" << endl;
    cout << "  void (*fp)(int);  // 함수 포인터" << endl;
    cout << "  fp = my_function;" << endl;
    cout << "  fp(10);" << endl;
    
    cout << "\n[C++ 버전]" << endl;
    cout << "  std::function<void(int)> fp;  // 타입 안전" << endl;
    cout << "  fp = [](int x) { /* 람다 */ };" << endl;
    cout << "  fp(10);" << endl;
    
    cout << "\n장점 비교:" << endl;
    cout << "\nC 함수 포인터:" << endl;
    cout << "  ✓ 가볍고 빠름" << endl;
    cout << "  ✓ 임베디드에 적합" << endl;
    cout << "  ✗ 타입 안전성 낮음" << endl;
    cout << "  ✗ 상태를 가질 수 없음" << endl;
    
    cout << "\nC++ std::function + 람다:" << endl;
    cout << "  ✓ 타입 안전" << endl;
    cout << "  ✓ 람다 캡처로 상태 보유" << endl;
    cout << "  ✓ 유연하고 표현력 높음" << endl;
    cout << "  ✗ 오버헤드 존재 (가상 함수 호출)" << endl;
    cout << "  ✗ 임베디드에는 과할 수 있음" << endl;
    
    cout << "\n언제 무엇을 사용할까?" << endl;
    cout << "  • 임베디드, 성능 중요 → C 함수 포인터" << endl;
    cout << "  • 애플리케이션, 유연성 중요 → C++ std::function" << endl;
    cout << "  • 간단한 콜백 → 람다" << endl;
    cout << "  • 복잡한 상태 → 함수 객체" << endl;
}

} // namespace patterns

/* ============================================================================
 * 메인 함수
 * ============================================================================
 */

int main() {
    using namespace patterns;
    
    cout << "\n";
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                                                            ║" << endl;
    cout << "║     C++ 함수 객체와 람다 완전 정복                         ║" << endl;
    cout << "║     (C 함수 포인터의 현대적 대안)                          ║" << endl;
    cout << "║                                                            ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    
    Step1_StdFunction();
    Step2_Lambda();
    Step3_Functor();
    Step4_Bind();
    Step5_ClassCallback();
    Step6_TemplateCallback();
    Step7_RealWorld();
    Step8_Comparison();
    
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  학습 완료!" << endl;
    cout << "========================================\n" << endl;
    cout << "C++의 람다와 std::function은" << endl;
    cout << "C의 함수 포인터보다 훨씬 강력하고 안전합니다!" << endl;
    cout << "\n다음 학습:" << endl;
    cout << "  01_wrapper_pattern.cpp" << endl;
    cout << "  04_callback_pattern.cpp" << endl;
    cout << "  05_state_machine.cpp" << endl;
    cout << "\n";
    
    return 0;
}

/*
 * ============================================================================
 * 컴파일 및 실행
 * ============================================================================
 * 
 * 컴파일 (C++17):
 *   g++ -std=c++17 -o 00_function_pointer 00_function_pointer_basics.cpp
 * 
 * 실행:
 *   ./00_function_pointer
 * 
 * ============================================================================
 * C vs C++ 최종 정리
 * ============================================================================
 * 
 * C 함수 포인터:
 *   - 간단하고 빠름
 *   - 임베디드에 최적
 *   - 메모리 효율적
 * 
 * C++ std::function + 람다:
 *   - 타입 안전
 *   - 표현력 높음
 *   - 상태를 가질 수 있음
 *   - 템플릿과 조합 가능
 * 
 * 실무 조언:
 *   - 임베디드 → C 함수 포인터
 *   - 데스크톱/서버 → C++ 람다
 *   - 혼합 사용도 가능!
 * 
 * ============================================================================
 */
