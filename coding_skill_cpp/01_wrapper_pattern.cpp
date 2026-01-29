/*
 * ============================================================================
 * 파일명: 01_wrapper_pattern.cpp
 * 기능: C++ Wrapper(래퍼) 패턴 - RAII와 스마트 포인터 활용
 * 
 * C 버전과의 차이점:
 *   - 구조체 + 함수 포인터 → 클래스 + 가상 함수
 *   - 수동 메모리 관리 → RAII + 스마트 포인터
 *   - 로깅 래퍼 → 데코레이터 패턴 + 템플릿
 *   - 인터페이스 → 추상 기본 클래스
 * 
 * C++ 추가 기능:
 *   - RAII (Resource Acquisition Is Initialization)
 *   - unique_ptr, shared_ptr로 자동 리소스 관리
 *   - 템플릿 래퍼
 *   - Proxy 패턴
 *   - PIMPL (Pointer to Implementation)
 * 
 * ============================================================================
 */

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <chrono>
#include <vector>
#include <functional>

using namespace std;
using namespace chrono;

namespace patterns {

/* ============================================================================
 * 1. RAII 파일 래퍼
 * ============================================================================
 * 
 * C에서는 fopen/fclose를 수동으로 호출
 * C++에서는 생성자/소멸자가 자동으로 관리
 * 
 * ============================================================================
 */

class File {
    FILE* file;
    string filename;
    
public:
    File(const string& filename, const string& mode)
        : filename(filename), file(nullptr) {
        file = fopen(filename.c_str(), mode.c_str());
        if (file) {
            cout << "[File] Opened: " << filename << endl;
        } else {
            cerr << "[File] Failed to open: " << filename << endl;
        }
    }
    
    ~File() {
        if (file) {
            fclose(file);
            cout << "[File] Closed: " << filename << endl;
        }
    }
    
    // 복사 방지
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    
    // 이동 허용
    File(File&& other) noexcept
        : file(other.file), filename(move(other.filename)) {
        other.file = nullptr;
    }
    
    bool isOpen() const { return file != nullptr; }
    
    void write(const string& data) {
        if (file) {
            fputs(data.c_str(), file);
            cout << "[File] Wrote " << data.size() << " bytes" << endl;
        }
    }
    
    string readAll() {
        if (!file) return "";
        
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        string content(size, '\0');
        fread(&content[0], 1, size, file);
        
        cout << "[File] Read " << size << " bytes" << endl;
        return content;
    }
};

/* ============================================================================
 * 2. 스마트 포인터와 커스텀 deleter
 * ============================================================================
 */

// FILE*를 위한 커스텀 deleter
struct FileDeleter {
    void operator()(FILE* fp) const {
        if (fp) {
            cout << "[FileDeleter] Closing file" << endl;
            fclose(fp);
        }
    }
};

using FilePtr = unique_ptr<FILE, FileDeleter>;

FilePtr openFile(const string& filename, const string& mode) {
    FILE* fp = fopen(filename.c_str(), mode.c_str());
    if (fp) {
        cout << "[FilePtr] Opened: " << filename << endl;
    }
    return FilePtr(fp);
}

/* ============================================================================
 * 3. 템플릿 로깅 래퍼
 * ============================================================================
 */

template<typename T>
class LoggingWrapper {
    T wrapped;
    string name;
    
public:
    LoggingWrapper(T obj, const string& name)
        : wrapped(obj), name(name) {}
    
    // 함수 호출 래핑
    template<typename... Args>
    auto operator()(Args&&... args) {
        cout << "[LOG] Calling " << name << endl;
        auto result = wrapped(forward<Args>(args)...);
        cout << "[LOG] " << name << " returned" << endl;
        return result;
    }
};

template<typename T>
auto makeLoggingWrapper(T func, const string& name) {
    return LoggingWrapper<T>(func, name);
}

/* ============================================================================
 * 4. 프록시 패턴 - 지연 초기화
 * ============================================================================
 */

class ExpensiveResource {
public:
    ExpensiveResource() {
        cout << "[ExpensiveResource] 초기화 중..." << endl;
        // 시뮬레이션: 비용이 큰 초기화
    }
    
    void doWork() {
        cout << "[ExpensiveResource] 작업 수행" << endl;
    }
};

class ResourceProxy {
    unique_ptr<ExpensiveResource> resource;
    
public:
    ResourceProxy() : resource(nullptr) {
        cout << "[Proxy] 생성 (리소스는 아직 생성 안됨)" << endl;
    }
    
    void doWork() {
        if (!resource) {
            cout << "[Proxy] 첫 호출! 리소스 생성" << endl;
            resource = make_unique<ExpensiveResource>();
        }
        resource->doWork();
    }
};

/* ============================================================================
 * 5. 인터페이스 기반 래퍼
 * ============================================================================
 */

// 추상 인터페이스
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const string& message) = 0;
};

// 콘솔 로거 구현
class ConsoleLogger : public ILogger {
public:
    void log(const string& message) override {
        cout << "[Console] " << message << endl;
    }
};

// 파일 로거 구현
class FileLogger : public ILogger {
    ofstream file;
    
public:
    FileLogger(const string& filename) : file(filename) {}
    
    void log(const string& message) override {
        if (file.is_open()) {
            file << "[File] " << message << endl;
        }
    }
};

// 로거 래퍼 (타임스탬프 추가)
class TimestampLoggerWrapper : public ILogger {
    unique_ptr<ILogger> logger;
    
public:
    TimestampLoggerWrapper(unique_ptr<ILogger> logger)
        : logger(move(logger)) {}
    
    void log(const string& message) override {
        auto now = system_clock::now();
        auto time_t = system_clock::to_time_t(now);
        string timestamp = ctime(&time_t);
        timestamp.pop_back();  // 개행 제거
        
        logger->log("[" + timestamp + "] " + message);
    }
};

/* ============================================================================
 * 6. PIMPL (Pointer to Implementation) 패턴
 * ============================================================================
 */

// 구현부 (보통 .cpp 파일에 숨김)
class WidgetImpl {
public:
    void doSomething() {
        cout << "[WidgetImpl] 내부 구현 실행" << endl;
    }
};

// 인터페이스 (헤더 파일에 노출)
class Widget {
    unique_ptr<WidgetImpl> pImpl;
    
public:
    Widget() : pImpl(make_unique<WidgetImpl>()) {
        cout << "[Widget] 생성 (구현 숨김)" << endl;
    }
    
    ~Widget() {
        cout << "[Widget] 소멸" << endl;
    }
    
    void doSomething() {
        pImpl->doSomething();
    }
};

/* ============================================================================
 * 7. 함수 래퍼 - 성능 측정
 * ============================================================================
 */

template<typename Func>
class TimingWrapper {
    Func func;
    string name;
    
public:
    TimingWrapper(Func f, const string& name)
        : func(f), name(name) {}
    
    template<typename... Args>
    auto operator()(Args&&... args) {
        auto start = high_resolution_clock::now();
        auto result = func(forward<Args>(args)...);
        auto end = high_resolution_clock::now();
        
        auto duration = duration_cast<microseconds>(end - start);
        cout << "[Timing] " << name << " took " 
             << duration.count() << " μs" << endl;
        
        return result;
    }
};

template<typename Func>
auto makeTimingWrapper(Func func, const string& name) {
    return TimingWrapper<Func>(func, name);
}

/* ============================================================================
 * 사용 예제
 * ============================================================================
 */

void demo_raii_file() {
    cout << "\n=== RAII 파일 래퍼 ===" << endl;
    
    {
        File file("test.txt", "w");
        if (file.isOpen()) {
            file.write("Hello from C++!\n");
            file.write("RAII is awesome!\n");
        }
    }  // 여기서 자동으로 파일 닫힘
    
    cout << "\n파일이 자동으로 닫혔습니다!" << endl;
}

void demo_smart_pointer() {
    cout << "\n=== 스마트 포인터 커스텀 deleter ===" << endl;
    
    {
        auto file = openFile("test2.txt", "w");
        if (file) {
            fputs("Hello from smart pointer!\n", file.get());
        }
    }  // 자동으로 deleter 호출
    
    cout << "\n스마트 포인터가 자동으로 파일을 닫았습니다!" << endl;
}

void demo_logging_wrapper() {
    cout << "\n=== 로깅 래퍼 ===" << endl;
    
    auto add = [](int a, int b) { return a + b; };
    auto logged_add = makeLoggingWrapper(add, "add");
    
    int result = logged_add(10, 20);
    cout << "Result: " << result << endl;
}

void demo_proxy() {
    cout << "\n=== 프록시 패턴 (지연 초기화) ===" << endl;
    
    ResourceProxy proxy;
    cout << "\n첫 번째 호출:" << endl;
    proxy.doWork();
    
    cout << "\n두 번째 호출:" << endl;
    proxy.doWork();
}

void demo_interface_wrapper() {
    cout << "\n=== 인터페이스 기반 래퍼 ===" << endl;
    
    auto console = make_unique<ConsoleLogger>();
    auto wrapped = make_unique<TimestampLoggerWrapper>(move(console));
    
    wrapped->log("시스템 시작");
    wrapped->log("작업 완료");
}

void demo_pimpl() {
    cout << "\n=== PIMPL 패턴 ===" << endl;
    
    Widget widget;
    widget.doSomething();
}

void demo_timing_wrapper() {
    cout << "\n=== 타이밍 래퍼 ===" << endl;
    
    auto slow_function = []() {
        int sum = 0;
        for (int i = 0; i < 1000000; ++i) {
            sum += i;
        }
        return sum;
    };
    
    auto timed = makeTimingWrapper(slow_function, "slow_function");
    int result = timed();
    cout << "Result: " << result << endl;
}

} // namespace patterns

int main() {
    using namespace patterns;
    
    cout << "\n";
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                                                            ║" << endl;
    cout << "║     C++ Wrapper 패턴 - RAII와 스마트 포인터               ║" << endl;
    cout << "║                                                            ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    
    demo_raii_file();
    demo_smart_pointer();
    demo_logging_wrapper();
    demo_proxy();
    demo_interface_wrapper();
    demo_pimpl();
    demo_timing_wrapper();
    
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  C++ Wrapper 패턴 완료!" << endl;
    cout << "========================================" << endl;
    cout << "\nC++의 RAII와 스마트 포인터는" << endl;
    cout << "자동 리소스 관리를 제공하여" << endl;
    cout << "C보다 훨씬 안전하고 편리합니다!" << endl;
    cout << endl;
    
    return 0;
}

/*
 * ============================================================================
 * 컴파일 및 실행
 * ============================================================================
 * 
 * 컴파일:
 *   g++ -std=c++17 -o 01_wrapper 01_wrapper_pattern.cpp
 * 
 * 실행:
 *   ./01_wrapper
 * 
 * ============================================================================
 * C vs C++ Wrapper 비교
 * ============================================================================
 * 
 * C Wrapper:
 *   - 수동으로 open/close
 *   - 함수 포인터로 추상화
 *   - 메모리 누수 위험
 * 
 * C++ Wrapper:
 *   - RAII로 자동 관리
 *   - 가상 함수로 다형성
 *   - 스마트 포인터로 안전
 *   - 템플릿으로 제네릭
 * 
 * 핵심:
 *   C++에서 Wrapper = RAII + 스마트 포인터
 * 
 * ============================================================================
 */
