/*
 * ============================================================================
 * 파일명: 03_classes_objects.cpp
 * 주제: C++의 클래스와 객체 (C의 struct와 비교)
 * 컴파일: g++ -std=c++17 03_classes_objects.cpp -o 03_classes
 * 
 * ============================================================================
 * 이 챕터에서 배울 내용
 * ============================================================================
 * 
 * - struct vs class
 * - 멤버 함수 (메서드)
 * - 접근 제어자 (public, private, protected)
 * - this 포인터
 * - 캡슐화(Encapsulation)
 * 
 * ============================================================================
 * C와의 주요 차이점
 * ============================================================================
 * 
 * C에서는:
 * - struct만 가능 (데이터만)
 * - 함수는 별도로 선언
 * - 접근 제어 없음 (모두 public)
 * - 수동으로 this 전달
 * 
 * C++에서는:
 * - class 사용 (데이터 + 함수)
 * - 멤버 함수 내장
 * - private/public으로 캡슐화
 * - this 자동 전달
 * 
 * 왜 C++가 더 나은가?
 * - 데이터와 함수가 하나로 묶임
 * - 정보 은닉으로 실수 방지
 * - 인터페이스와 구현 분리
 * - 유지보수 용이
 * 
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <cmath>

// C 스타일 (비교용)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

/* ============================================================================
 * 1. struct vs class: 기본 개념
 * ============================================================================ */

// C 방식: struct + 외부 함수
typedef struct {
    double x;
    double y;
} Point_C;

double Point_C_Distance(const Point_C* p1, const Point_C* p2) {
    double dx = p1->x - p2->x;
    double dy = p1->y - p2->y;
    return sqrt(dx * dx + dy * dy);
}

void Point_C_Print(const Point_C* p) {
    printf("C Point: (%.2f, %.2f)\n", p->x, p->y);
}

// C++ 방식 1: struct (C와 호환 + 멤버 함수)
struct Point_CPP_Struct {
    double x;
    double y;
    
    // 멤버 함수 (메서드)
    double distance(const Point_CPP_Struct& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
    
    void print() const {
        cout << "C++ Point (struct): (" << x << ", " << y << ")" << endl;
    }
};

// C++ 방식 2: class (캡슐화)
class Point_CPP_Class {
private:  // 외부에서 접근 불가
    double x;
    double y;
    
public:   // 외부에서 접근 가능
    // 생성자는 다음 챕터에서 자세히
    Point_CPP_Class(double x_val, double y_val) : x(x_val), y(y_val) {}
    
    // Getter (읽기)
    double getX() const { return x; }
    double getY() const { return y; }
    
    // Setter (쓰기)
    void setX(double val) { x = val; }
    void setY(double val) { y = val; }
    
    double distance(const Point_CPP_Class& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
    
    void print() const {
        cout << "C++ Point (class): (" << x << ", " << y << ")" << endl;
    }
};

void Example_01_StructVsClass() {
    cout << "\n=== 예제 1: struct vs class ===\n" << endl;
    
    // C 방식
    {
        cout << "[C 방식]" << endl;
        Point_C p1 = {0.0, 0.0};
        Point_C p2 = {3.0, 4.0};
        
        Point_C_Print(&p1);
        Point_C_Print(&p2);
        
        double dist = Point_C_Distance(&p1, &p2);
        printf("거리: %.2f\n", dist);
        
        // 문제: 직접 접근 가능 (실수 유발)
        p1.x = -999;  // 유효하지 않은 값도 가능!
    }
    
    // C++ struct 방식
    {
        cout << "\n[C++ struct 방식]" << endl;
        Point_CPP_Struct p1 = {0.0, 0.0};
        Point_CPP_Struct p2 = {3.0, 4.0};
        
        p1.print();
        p2.print();
        
        double dist = p1.distance(p2);
        cout << "거리: " << dist << endl;
        
        // 여전히 직접 접근 가능
        p1.x = -999;
    }
    
    // C++ class 방식
    {
        cout << "\n[C++ class 방식 (캡슐화)]" << endl;
        Point_CPP_Class p1(0.0, 0.0);
        Point_CPP_Class p2(3.0, 4.0);
        
        p1.print();
        p2.print();
        
        double dist = p1.distance(p2);
        cout << "거리: " << dist << endl;
        
        // 직접 접근 불가!
        // p1.x = -999;  // ❌ 컴파일 에러!
        
        // Setter를 통해서만 수정 가능
        p1.setX(1.0);  // ✅ OK
        p1.print();
    }
    
    /*
     * struct vs class:
     * 
     * struct:
     * - 기본 접근 제어자: public
     * - C와의 호환성
     * - 간단한 데이터 묶음에 적합
     * 
     * class:
     * - 기본 접근 제어자: private
     * - 캡슐화 강조
     * - 복잡한 객체에 적합
     * 
     * 기술적으로는 같지만, 관례적으로:
     * - struct: Plain Old Data (POD)
     * - class: 복잡한 로직 포함
     */
}

/* ============================================================================
 * 2. 접근 제어자: public, private, protected
 * ============================================================================ */

class BankAccount {
private:  // 외부 접근 불가 (정보 은닉)
    string owner;
    double balance;
    string account_number;
    
    // private 멤버 함수 (내부 구현)
    bool isValidAmount(double amount) const {
        return amount > 0 && amount < 1000000000;
    }
    
public:   // 외부 접근 가능 (인터페이스)
    // 생성자
    BankAccount(const string& name, const string& account) 
        : owner(name), balance(0.0), account_number(account) {}
    
    // 입금
    bool deposit(double amount) {
        if (!isValidAmount(amount)) {
            cout << "❌ 유효하지 않은 금액" << endl;
            return false;
        }
        
        balance += amount;
        cout << "✅ " << amount << "원 입금됨 (잔액: " 
             << balance << "원)" << endl;
        return true;
    }
    
    // 출금
    bool withdraw(double amount) {
        if (!isValidAmount(amount)) {
            cout << "❌ 유효하지 않은 금액" << endl;
            return false;
        }
        
        if (balance < amount) {
            cout << "❌ 잔액 부족" << endl;
            return false;
        }
        
        balance -= amount;
        cout << "✅ " << amount << "원 출금됨 (잔액: " 
             << balance << "원)" << endl;
        return true;
    }
    
    // 조회
    double getBalance() const {
        return balance;
    }
    
    void printInfo() const {
        cout << "계좌 정보: " << owner 
             << " (" << account_number << ")" 
             << ", 잔액: " << balance << "원" << endl;
    }
};

void Example_02_AccessControl() {
    cout << "\n=== 예제 2: 접근 제어자 ===\n" << endl;
    
    BankAccount account("홍길동", "123-456-789");
    
    account.printInfo();
    
    // public 메서드는 접근 가능
    account.deposit(10000);
    account.withdraw(3000);
    account.printInfo();
    
    // private 멤버는 접근 불가
    // account.balance = 1000000000;  // ❌ 컴파일 에러!
    // account.owner = "해커";         // ❌ 컴파일 에러!
    
    // Getter로만 읽기 가능
    cout << "현재 잔액: " << account.getBalance() << "원" << endl;
    
    /*
     * 접근 제어의 장점:
     * 
     * 1. 정보 은닉 (Information Hiding)
     *    - 내부 구현 숨김
     *    - 외부에서 잘못된 값 설정 방지
     * 
     * 2. 인터페이스와 구현 분리
     *    - public: 사용자가 사용하는 인터페이스
     *    - private: 내부 구현 (변경 가능)
     * 
     * 3. 유지보수 용이
     *    - private 멤버는 자유롭게 변경 가능
     *    - public 인터페이스만 유지하면 됨
     */
}

/* ============================================================================
 * 3. this 포인터
 * ============================================================================ */

// C 방식: 명시적 this 전달
typedef struct {
    int id;
    double value;
} Data_C;

void Data_C_SetValue(Data_C* this_ptr, double val) {
    this_ptr->value = val;  // 명시적 사용
}

void Data_C_Print(const Data_C* this_ptr) {
    printf("C: ID=%d, Value=%.2f\n", this_ptr->id, this_ptr->value);
}

// C++ 방식: 암시적 this 포인터
class Data_CPP {
private:
    int id;
    double value;
    
public:
    Data_CPP(int i, double v) : id(i), value(v) {}
    
    void setValue(double val) {
        // this는 자동으로 전달됨
        this->value = val;  // 명시적 사용 가능
        value = val;        // 또는 생략 가능
    }
    
    // 이름 충돌 시 this 사용
    void setId(int id) {
        this->id = id;  // this->id는 멤버, id는 매개변수
    }
    
    // this 반환 (체이닝)
    Data_CPP& multiplyValue(double factor) {
        value *= factor;
        return *this;  // 자기 자신 반환
    }
    
    void print() const {
        cout << "C++: ID=" << id << ", Value=" << value << endl;
    }
};

void Example_03_ThisPointer() {
    cout << "\n=== 예제 3: this 포인터 ===\n" << endl;
    
    // C 방식
    {
        cout << "[C 방식]" << endl;
        Data_C data = {1, 100.0};
        
        Data_C_Print(&data);
        Data_C_SetValue(&data, 200.0);  // this 명시적 전달
        Data_C_Print(&data);
    }
    
    // C++ 방식
    {
        cout << "\n[C++ 방식]" << endl;
        Data_CPP data(1, 100.0);
        
        data.print();
        data.setValue(200.0);  // this 자동 전달!
        data.print();
        
        // this 반환을 활용한 체이닝
        data.multiplyValue(2.0)
            .multiplyValue(1.5)
            .multiplyValue(2.0);
        
        data.print();
    }
    
    /*
     * this 포인터:
     * 
     * - 멤버 함수의 숨겨진 첫 번째 매개변수
     * - 호출한 객체를 가리킴
     * - 명시적 사용: this->member
     * - 생략 가능: member (자동으로 this->member)
     * 
     * 사용 시기:
     * 1. 매개변수와 멤버 이름이 같을 때
     * 2. 자기 자신을 반환할 때 (메서드 체이닝)
     * 3. 포인터가 필요할 때
     */
}

/* ============================================================================
 * 4. 실무 예제: C vs C++ 비교
 * ============================================================================ */

// C 방식: 센서 관리
typedef struct {
    int id;
    char name[50];
    double value;
    int status;  // 0: OFF, 1: ON
} Sensor_C;

void Sensor_C_Init(Sensor_C* sensor, int id, const char* name) {
    sensor->id = id;
    strncpy(sensor->name, name, sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name) - 1] = '\0';
    sensor->value = 0.0;
    sensor->status = 0;
}

void Sensor_C_TurnOn(Sensor_C* sensor) {
    sensor->status = 1;
    printf("[C] %s 센서 켜짐\n", sensor->name);
}

void Sensor_C_TurnOff(Sensor_C* sensor) {
    sensor->status = 0;
    printf("[C] %s 센서 꺼짐\n", sensor->name);
}

double Sensor_C_Read(Sensor_C* sensor) {
    if (sensor->status == 0) {
        printf("[C] 센서가 꺼져 있음!\n");
        return -1.0;
    }
    // 실제로는 하드웨어에서 읽음
    sensor->value = 25.5;  // 가상의 값
    return sensor->value;
}

// C++ 방식: 센서 클래스
class Sensor_CPP {
private:
    int id;
    string name;
    double value;
    bool status;
    
    // private 헬퍼 함수
    void log(const string& message) const {
        cout << "[C++] " << name << ": " << message << endl;
    }
    
public:
    Sensor_CPP(int id_val, const string& name_val) 
        : id(id_val), name(name_val), value(0.0), status(false) {}
    
    void turnOn() {
        status = true;
        log("센서 켜짐");
    }
    
    void turnOff() {
        status = false;
        log("센서 꺼짐");
    }
    
    double read() {
        if (!status) {
            log("센서가 꺼져 있음!");
            return -1.0;
        }
        // 실제로는 하드웨어에서 읽음
        value = 25.5;  // 가상의 값
        return value;
    }
    
    // Getter
    int getId() const { return id; }
    string getName() const { return name; }
    bool isOn() const { return status; }
    
    void printInfo() const {
        cout << "센서 ID: " << id << ", 이름: " << name 
             << ", 상태: " << (status ? "ON" : "OFF")
             << ", 값: " << value << endl;
    }
};

void Example_04_RealWorld() {
    cout << "\n=== 예제 4: 실무 예제 (센서 관리) ===\n" << endl;
    
    // C 방식
    {
        cout << "[C 방식]" << endl;
        Sensor_C sensor;
        Sensor_C_Init(&sensor, 101, "온도센서");
        
        Sensor_C_TurnOn(&sensor);
        double val = Sensor_C_Read(&sensor);
        printf("읽은 값: %.2f\n", val);
        
        Sensor_C_TurnOff(&sensor);
        Sensor_C_Read(&sensor);  // 에러 메시지
    }
    
    // C++ 방식
    {
        cout << "\n[C++ 방식]" << endl;
        Sensor_CPP sensor(101, "온도센서");
        
        sensor.turnOn();
        double val = sensor.read();
        cout << "읽은 값: " << val << endl;
        
        sensor.printInfo();
        
        sensor.turnOff();
        sensor.read();  // 에러 메시지
    }
    
    /*
     * C vs C++ 비교:
     * 
     * C:
     * - 함수 이름이 길어짐 (Sensor_C_TurnOn)
     * - 포인터를 항상 전달해야 함
     * - 접근 제어 없음 (sensor.status 직접 수정 가능)
     * 
     * C++:
     * - 간결한 이름 (sensor.turnOn())
     * - 포인터 전달 자동 (this)
     * - 캡슐화 (status는 private)
     */
}

/* ============================================================================
 * 5. const 멤버 함수
 * ============================================================================ */

class Counter {
private:
    int count;
    
public:
    Counter() : count(0) {}
    
    void increment() {  // 비const 멤버 함수
        count++;
    }
    
    int getCount() const {  // const 멤버 함수
        // count++;  // ❌ 에러! const 함수는 멤버 수정 불가
        return count;
    }
    
    void print() const {  // const 멤버 함수
        cout << "Count: " << count << endl;
    }
};

void Example_05_ConstMemberFunctions() {
    cout << "\n=== 예제 5: const 멤버 함수 ===\n" << endl;
    
    Counter c;
    c.increment();
    c.increment();
    c.print();  // const 함수 호출
    
    // const 객체
    const Counter c_const;
    // c_const.increment();  // ❌ 에러! const 객체는 비const 함수 호출 불가
    c_const.print();         // ✅ OK! const 함수 호출 가능
    cout << "const Count: " << c_const.getCount() << endl;
    
    /*
     * const 멤버 함수:
     * 
     * - 멤버 변수를 수정하지 않음을 보장
     * - const 객체는 const 멤버 함수만 호출 가능
     * - Getter는 항상 const로 선언
     * 
     * 권장 사항:
     * - 읽기 전용 함수는 const로 선언
     * - 컴파일 타임에 실수 방지
     */
}

/* ============================================================================
 * 6. struct와 class의 실제 사용 예
 * ============================================================================ */

// struct 사용 예: 간단한 데이터 묶음 (POD)
struct Point2D {
    double x;
    double y;
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// class 사용 예: 복잡한 로직 포함
class Image {
private:
    int width;
    int height;
    Color* pixels;  // 동적 배열
    
public:
    Image(int w, int h) : width(w), height(h) {
        pixels = new Color[w * h];
    }
    
    ~Image() {
        delete[] pixels;
    }
    
    void setPixel(int x, int y, const Color& c) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y * width + x] = c;
        }
    }
    
    Color getPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return pixels[y * width + x];
        }
        return {0, 0, 0, 0};
    }
    
    void printInfo() const {
        cout << "Image: " << width << "x" << height << endl;
    }
};

void Example_06_StructVsClassUsage() {
    cout << "\n=== 예제 6: struct vs class 실제 사용 ===\n" << endl;
    
    // struct: 간단한 데이터
    Point2D point = {10.0, 20.0};
    Color red = {255, 0, 0, 255};
    
    cout << "Point: (" << point.x << ", " << point.y << ")" << endl;
    cout << "Color: R=" << (int)red.r << ", G=" << (int)red.g 
         << ", B=" << (int)red.b << endl;
    
    // class: 복잡한 객체
    Image img(800, 600);
    img.printInfo();
    img.setPixel(100, 100, red);
    Color pixel = img.getPixel(100, 100);
    cout << "Pixel at (100, 100): R=" << (int)pixel.r << endl;
    
    /*
     * 실무 가이드라인:
     * 
     * struct 사용:
     * - 단순 데이터 묶음 (POD)
     * - 함수 없거나 간단한 유틸리티 함수만
     * - C와의 호환성 필요
     * 예: Point, Color, Vector, Config
     * 
     * class 사용:
     * - 복잡한 로직 포함
     * - 캡슐화 필요
     * - 상속 계층 구조
     * 예: Image, Database, NetworkManager
     */
}

/* ============================================================================
 * main 함수
 * ============================================================================ */

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 03. 클래스와 객체" << endl;
    cout << "========================================" << endl;
    
    Example_01_StructVsClass();
    Example_02_AccessControl();
    Example_03_ThisPointer();
    Example_04_RealWorld();
    Example_05_ConstMemberFunctions();
    Example_06_StructVsClassUsage();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 04_constructors_destructors.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 연습 문제
 * ============================================================================
 * 
 * 1. C의 struct + 함수를 class로 변환해보세요.
 * 2. private 멤버와 public getter/setter를 가진 클래스를 만들어보세요.
 * 3. this 포인터를 활용한 메서드 체이닝을 구현해보세요.
 * 4. const 멤버 함수와 비const 멤버 함수의 차이를 실험해보세요.
 * 
 * ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * struct vs class:
 * - 기본 접근 제어자만 다름 (struct: public, class: private)
 * - struct: 간단한 데이터 (POD)
 * - class: 복잡한 로직 + 캡슐화
 * 
 * 접근 제어자:
 * - private: 내부 구현 (외부 접근 불가)
 * - public: 인터페이스 (외부 접근 가능)
 * - protected: 상속 (다음 챕터에서)
 * 
 * this 포인터:
 * - 자동으로 전달됨
 * - 자기 자신을 가리킴
 * - 메서드 체이닝에 활용
 * 
 * const 멤버 함수:
 * - 멤버 변수 수정 불가
 * - const 객체에서 호출 가능
 * - Getter는 항상 const로
 * 
 * ============================================================================
 */
