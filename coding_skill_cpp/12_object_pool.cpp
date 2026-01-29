/* C++ Object Pool */
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

template<typename T>
class ObjectPool {
    vector<unique_ptr<T>> pool;
    vector<T*> available;
    
public:
    ObjectPool(size_t size) {
        for (size_t i = 0; i < size; ++i) {
            pool.push_back(make_unique<T>());
            available.push_back(pool.back().get());
        }
    }
    
    T* acquire() {
        if (available.empty()) return nullptr;
        T* obj = available.back();
        available.pop_back();
        return obj;
    }
    
    void release(T* obj) {
        available.push_back(obj);
    }
};

struct Packet { int id; string data; };

int main() {
    cout << "=== C++ Object Pool ===" << endl;
    ObjectPool<Packet> pool(3);
    
    Packet* p1 = pool.acquire();
    p1->id = 1;
    cout << "Acquired packet " << p1->id << endl;
    
    pool.release(p1);
    cout << "Released" << endl;
    
    return 0;
}
