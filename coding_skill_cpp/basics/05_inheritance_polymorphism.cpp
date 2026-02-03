/*
 * ============================================================================
 * 파일명: 05_inheritance_polymorphism.cpp
 * 주제: 상속과 다형성 (C의 함수 포인터 패턴과 비교)
 * 컴파일: g++ -std=c++17 05_inheritance_polymorphism.cpp -o 05_inheritance
 * 
 * ============================================================================
 * 이 챕터에서 배울 내용
 * ============================================================================
 * 
 * - 상속(Inheritance) 기본
 * - virtual 함수
 * - 다형성(Polymorphism)
 * - 순수 가상 함수 (인터페이스)
 * - override와 final 키워드
 * 
 * ============================================================================
 * C와의 주요 차이점
 * ============================================================================
 * 
 * C에서는:
 * - 함수 포인터로 다형성 구현
 * - vtable을 수동으로 관리
 * - 타입 캐스팅 필요
 * - 코드가 복잡
 * 
 * C++에서는:
 * - virtual 키워드로 간단히
 * - 컴파일러가 vtable 자동 생성
 * - 타입 안전
 * - 코드가 직관적
 * 
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/* ============================================================================
 * 1. 기본 상속
 * ============================================================================ */

// 부모 클래스 (Base Class)
class Animal {
protected:  // 자식 클래스에서 접근 가능
    string name;
    int age;
    
public:
    Animal(const string& n, int a) : name(n), age(a) {
        cout << "Animal 생성: " << name << endl;
    }
    
    virtual ~Animal() {
        cout << "Animal 소멸: " << name << endl;
    }
    
    void eat() const {
        cout << name << "가 먹이를 먹습니다" << endl;
    }
    
    void sleep() const {
        cout << name << "가 잠을 잡니다" << endl;
    }
    
    // 가상 함수 (자식에서 재정의 가능)
    virtual void makeSound() const {
        cout << name << ": 동물 소리" << endl;
    }
    
    virtual void printInfo() const {
        cout << "동물: " << name << ", 나이: " << age << endl;
    }
};

// 자식 클래스 1: Dog
class Dog : public Animal {
private:
    string breed;
    
public:
    Dog(const string& n, int a, const string& b) 
        : Animal(n, a), breed(b) {
        cout << "Dog 생성: " << name << " (" << breed << ")" << endl;
    }
    
    ~Dog() {
        cout << "Dog 소멸: " << name << endl;
    }
    
    // 가상 함수 재정의 (override)
    void makeSound() const override {
        cout << name << ": 멍멍!" << endl;
    }
    
    void printInfo() const override {
        cout << "개: " << name << ", 나이: " << age 
             << ", 품종: " << breed << endl;
    }
    
    // Dog만의 함수
    void fetch() const {
        cout << name << "가 공을 가져옵니다" << endl;
    }
};

// 자식 클래스 2: Cat
class Cat : public Animal {
private:
    bool is_indoor;
    
public:
    Cat(const string& n, int a, bool indoor) 
        : Animal(n, a), is_indoor(indoor) {
        cout << "Cat 생성: " << name << endl;
    }
    
    ~Cat() {
        cout << "Cat 소멸: " << name << endl;
    }
    
    void makeSound() const override {
        cout << name << ": 야옹~" << endl;
    }
    
    void printInfo() const override {
        cout << "고양이: " << name << ", 나이: " << age 
             << ", " << (is_indoor ? "실내" : "실외") << endl;
    }
    
    void climb() const {
        cout << name << "가 나무에 오릅니다" << endl;
    }
};

void Example_01_BasicInheritance() {
    cout << "\n=== 예제 1: 기본 상속 ===\n" << endl;
    
    Dog dog("멍멍이", 3, "진돗개");
    Cat cat("야옹이", 2, true);
    
    cout << "\n--- 부모로부터 상속받은 함수 ---" << endl;
    dog.eat();
    cat.sleep();
    
    cout << "\n--- 재정의된 가상 함수 ---" << endl;
    dog.makeSound();
    cat.makeSound();
    
    cout << "\n--- 각 클래스 고유 함수 ---" << endl;
    dog.fetch();
    cat.climb();
    
    cout << endl;
}

/* ============================================================================
 * 2. 다형성 (Polymorphism)
 * ============================================================================ */

void Example_02_Polymorphism() {
    cout << "\n=== 예제 2: 다형성 ===\n" << endl;
    
    // 부모 타입 포인터로 자식 객체 가리키기
    Animal* animals[3];
    animals[0] = new Dog("바둑이", 5, "리트리버");
    animals[1] = new Cat("나비", 3, false);
    animals[2] = new Dog("초코", 2, "푸들");
    
    cout << "\n--- 다형성 동작 ---" << endl;
    for (int i = 0; i < 3; i++) {
        animals[i]->printInfo();
        animals[i]->makeSound();
        cout << endl;
    }
    
    // 메모리 해제
    for (int i = 0; i < 3; i++) {
        delete animals[i];  // virtual 소멸자 덕분에 올바른 소멸
    }
    
    /*
     * 다형성의 핵심:
     * - 부모 타입으로 자식 객체를 다룸
     * - 실행 시간에 올바른 함수 호출 (동적 바인딩)
     * - 코드 재사용성과 확장성
     */
}

/* ============================================================================
 * 3. 순수 가상 함수와 추상 클래스
 * ============================================================================ */

// 추상 클래스 (Abstract Class) - 인터페이스
class Shape {
protected:
    string name;
    
public:
    Shape(const string& n) : name(n) {}
    virtual ~Shape() {}
    
    // 순수 가상 함수 (Pure Virtual Function)
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    
    // 일반 가상 함수
    virtual void print() const {
        cout << name << " - 면적: " << area() 
             << ", 둘레: " << perimeter() << endl;
    }
};

class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : Shape("원"), radius(r) {}
    
    double area() const override {
        return 3.14159 * radius * radius;
    }
    
    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }
};

class Rectangle : public Shape {
private:
    double width;
    double height;
    
public:
    Rectangle(double w, double h) : Shape("사각형"), width(w), height(h) {}
    
    double area() const override {
        return width * height;
    }
    
    double perimeter() const override {
        return 2 * (width + height);
    }
};

void Example_03_AbstractClass() {
    cout << "\n=== 예제 3: 추상 클래스 (인터페이스) ===\n" << endl;
    
    // Shape shape;  // ❌ 에러! 추상 클래스는 객체 생성 불가
    
    vector<Shape*> shapes;
    shapes.push_back(new Circle(5.0));
    shapes.push_back(new Rectangle(4.0, 6.0));
    shapes.push_back(new Circle(3.0));
    
    cout << "모든 도형 정보:" << endl;
    for (Shape* shape : shapes) {
        shape->print();
    }
    
    // 메모리 해제
    for (Shape* shape : shapes) {
        delete shape;
    }
    
    /*
     * 추상 클래스:
     * - 순수 가상 함수 포함
     * - 객체 생성 불가
     * - 인터페이스 역할
     * 
     * 순수 가상 함수:
     * - = 0 으로 선언
     * - 자식 클래스에서 반드시 구현
     */
}

/* ============================================================================
 * 4. C vs C++ 비교: 센서 예제
 * ============================================================================ */

// C 방식: 함수 포인터
typedef struct Sensor_C {
    const char* name;
    double (*read)(struct Sensor_C* self);
    void (*calibrate)(struct Sensor_C* self);
    void* impl_data;
} Sensor_C;

double TempSensor_Read(Sensor_C* self) {
    return 25.5;
}

void TempSensor_Calibrate(Sensor_C* self) {
    printf("[C] 온도 센서 보정\n");
}

// C++ 방식: 상속과 다형성
class Sensor_CPP {
protected:
    string name;
    
public:
    Sensor_CPP(const string& n) : name(n) {}
    virtual ~Sensor_CPP() {}
    
    virtual double read() = 0;  // 순수 가상 함수
    virtual void calibrate() = 0;
    
    string getName() const { return name; }
};

class TempSensor_CPP : public Sensor_CPP {
public:
    TempSensor_CPP() : Sensor_CPP("온도센서") {}
    
    double read() override {
        return 25.5;
    }
    
    void calibrate() override {
        cout << "[C++] 온도 센서 보정" << endl;
    }
};

class HumiditySensor_CPP : public Sensor_CPP {
public:
    HumiditySensor_CPP() : Sensor_CPP("습도센서") {}
    
    double read() override {
        return 60.0;
    }
    
    void calibrate() override {
        cout << "[C++] 습도 센서 보정" << endl;
    }
};

void Example_04_CVsCPP() {
    cout << "\n=== 예제 4: C vs C++ 센서 비교 ===\n" << endl;
    
    // C 방식
    {
        cout << "[C 방식]" << endl;
        Sensor_C temp_sensor;
        temp_sensor.name = "온도센서";
        temp_sensor.read = TempSensor_Read;
        temp_sensor.calibrate = TempSensor_Calibrate;
        temp_sensor.impl_data = NULL;
        
        printf("센서: %s, 값: %.2f\n", temp_sensor.name, temp_sensor.read(&temp_sensor));
        temp_sensor.calibrate(&temp_sensor);
    }
    
    // C++ 방식
    {
        cout << "\n[C++ 방식]" << endl;
        vector<Sensor_CPP*> sensors;
        sensors.push_back(new TempSensor_CPP());
        sensors.push_back(new HumiditySensor_CPP());
        
        for (Sensor_CPP* sensor : sensors) {
            cout << "센서: " << sensor->getName() 
                 << ", 값: " << sensor->read() << endl;
            sensor->calibrate();
        }
        
        for (Sensor_CPP* sensor : sensors) {
            delete sensor;
        }
    }
}

/* ============================================================================
 * 5. override와 final
 * ============================================================================ */

class Base {
public:
    virtual void func1() {
        cout << "Base::func1()" << endl;
    }
    
    virtual void func2() final {  // 재정의 금지
        cout << "Base::func2() - final!" << endl;
    }
};

class Derived : public Base {
public:
    void func1() override {  // 명시적 override
        cout << "Derived::func1()" << endl;
    }
    
    // void func2() override {  // ❌ 에러! final 함수는 재정의 불가
    // }
};

class FinalClass final {  // 상속 금지
public:
    void doSomething() {
        cout << "FinalClass는 상속 불가" << endl;
    }
};

// class CannotDeriveThis : public FinalClass {  // ❌ 에러!
// };

void Example_05_OverrideFinal() {
    cout << "\n=== 예제 5: override와 final ===\n" << endl;
    
    Base* ptr = new Derived();
    ptr->func1();  // Derived::func1() 호출
    ptr->func2();  // Base::func2() 호출 (final)
    delete ptr;
    
    /*
     * override:
     * - 가상 함수 재정의를 명시
     * - 컴파일 타임에 검증
     * - 실수 방지
     * 
     * final:
     * - 함수: 재정의 금지
     * - 클래스: 상속 금지
     */
}

/* ============================================================================
 * 6. 가상 소멸자의 중요성
 * ============================================================================ */

class Base_NoVirtual {
public:
    ~Base_NoVirtual() {
        cout << "Base_NoVirtual 소멸" << endl;
    }
};

class Derived_NoVirtual : public Base_NoVirtual {
private:
    int* data;
    
public:
    Derived_NoVirtual() {
        data = new int[100];
        cout << "Derived_NoVirtual 생성" << endl;
    }
    
    ~Derived_NoVirtual() {
        delete[] data;
        cout << "Derived_NoVirtual 소멸" << endl;
    }
};

class Base_Virtual {
public:
    virtual ~Base_Virtual() {
        cout << "Base_Virtual 소멸" << endl;
    }
};

class Derived_Virtual : public Base_Virtual {
private:
    int* data;
    
public:
    Derived_Virtual() {
        data = new int[100];
        cout << "Derived_Virtual 생성" << endl;
    }
    
    ~Derived_Virtual() override {
        delete[] data;
        cout << "Derived_Virtual 소멸" << endl;
    }
};

void Example_06_VirtualDestructor() {
    cout << "\n=== 예제 6: 가상 소멸자 ===\n" << endl;
    
    cout << "[❌ 비가상 소멸자 - 메모리 누수!]" << endl;
    {
        Base_NoVirtual* ptr = new Derived_NoVirtual();
        delete ptr;  // Derived 소멸자 호출 안 됨! 메모리 누수!
    }
    
    cout << "\n[✅ 가상 소멸자 - 안전]" << endl;
    {
        Base_Virtual* ptr = new Derived_Virtual();
        delete ptr;  // Derived 소멸자 올바르게 호출!
    }
    
    /*
     * 가상 소멸자:
     * - 다형성 사용 시 반드시 필요
     * - 부모 포인터로 delete 할 때 자식 소멸자 호출
     * - 메모리 누수 방지
     * 
     * 규칙:
     * - 가상 함수가 하나라도 있으면 소멸자도 virtual
     */
}

/* ============================================================================
 * main 함수
 * ============================================================================ */

int main() {
    cout << "========================================" << endl;
    cout << "C++ 기초: 05. 상속과 다형성" << endl;
    cout << "========================================" << endl;
    
    Example_01_BasicInheritance();
    Example_02_Polymorphism();
    Example_03_AbstractClass();
    Example_04_CVsCPP();
    Example_05_OverrideFinal();
    Example_06_VirtualDestructor();
    
    cout << "\n========================================" << endl;
    cout << "학습 완료!" << endl;
    cout << "다음: 06_templates_basics.cpp" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/* ============================================================================
 * 핵심 요약
 * ============================================================================
 * 
 * 상속:
 * - class Derived : public Base
 * - 코드 재사용
 * - is-a 관계
 * 
 * 다형성:
 * - virtual 키워드
 * - 동적 바인딩
 * - 부모 타입으로 자식 객체 다루기
 * 
 * 추상 클래스:
 * - 순수 가상 함수 (= 0)
 * - 인터페이스 역할
 * - 객체 생성 불가
 * 
 * override/final:
 * - override: 재정의 명시
 * - final: 재정의/상속 금지
 * 
 * 가상 소멸자:
 * - 다형성 사용 시 필수
 * - 메모리 누수 방지
 * 
 * ============================================================================
 */
