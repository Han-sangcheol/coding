/*
 * C++ Singleton Pattern - Meyers Singleton (Thread-safe)
 * C 버전: static 변수 + 초기화 플래그
 * C++ 버전: 지역 static (C++11 이후 thread-safe 보장)
 */

#include <iostream>
#include <memory>
#include <mutex>
#include <string>

using namespace std;

namespace patterns {

// 1. Meyers Singleton (가장 권장)
class Logger {
    Logger() { cout << "[Logger] 생성" << endl; }
    
public:
    // 복사/이동 방지
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    static Logger& getInstance() {
        static Logger instance;  // C++11: thread-safe!
        return instance;
    }
    
    void log(const string& msg) {
        cout << "[LOG] " << msg << endl;
    }
};

// 2. CRTP를 이용한 Singleton Base
template<typename T>
class Singleton {
protected:
    Singleton() = default;
    
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
    static T& getInstance() {
        static T instance;
        return instance;
    }
};

class Config : public Singleton<Config> {
    friend class Singleton<Config>;
    string version = "1.0.0";
    
    Config() { cout << "[Config] 생성" << endl; }
    
public:
    string getVersion() const { return version; }
    void setVersion(const string& v) { version = v; }
};

// 3. Thread-safe Singleton (명시적 mutex)
class Database {
    static unique_ptr<Database> instance;
    static mutex mtx;
    
    Database() { cout << "[Database] 생성" << endl; }
    
public:
    static Database& getInstance() {
        if (!instance) {
            lock_guard<mutex> lock(mtx);
            if (!instance) {
                instance = unique_ptr<Database>(new Database());
            }
        }
        return *instance;
    }
    
    void query(const string& sql) {
        cout << "[DB] Query: " << sql << endl;
    }
};

unique_ptr<Database> Database::instance = nullptr;
mutex Database::mtx;

} // namespace patterns

int main() {
    using namespace patterns;
    
    cout << "\n=== C++ Singleton Pattern ===" << endl;
    
    // Meyers Singleton
    cout << "\n1. Meyers Singleton:" << endl;
    Logger::getInstance().log("Message 1");
    Logger::getInstance().log("Message 2");
    
    // CRTP Singleton
    cout << "\n2. CRTP Singleton:" << endl;
    cout << "Version: " << Config::getInstance().getVersion() << endl;
    
    // Thread-safe Singleton
    cout << "\n3. Thread-safe Singleton:" << endl;
    Database::getInstance().query("SELECT * FROM users");
    
    cout << "\n모두 동일한 인스턴스 사용!" << endl;
    
    return 0;
}
