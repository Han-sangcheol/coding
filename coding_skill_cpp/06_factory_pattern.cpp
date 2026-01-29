/* C++ Factory - 템플릿과 unique_ptr */
#include <iostream>
#include <memory>
#include <map>
#include <string>
using namespace std;

class Product {
public:
    virtual ~Product() = default;
    virtual void use() = 0;
};

class ProductA : public Product {
public:
    void use() override { cout << "  → ProductA 사용" << endl; }
};

class ProductB : public Product {
public:
    void use() override { cout << "  → ProductB 사용" << endl; }
};

class Factory {
public:
    static unique_ptr<Product> create(const string& type) {
        if (type == "A") return make_unique<ProductA>();
        if (type == "B") return make_unique<ProductB>();
        return nullptr;
    }
};

// 템플릿 팩토리
template<typename T>
class GenericFactory {
public:
    static unique_ptr<T> create() {
        return make_unique<T>();
    }
};

int main() {
    cout << "\n=== C++ Factory Pattern ===" << endl;
    
    auto productA = Factory::create("A");
    auto productB = Factory::create("B");
    
    productA->use();
    productB->use();
    
    // 템플릿 팩토리
    auto product = GenericFactory<ProductA>::create();
    product->use();
    
    return 0;
}
