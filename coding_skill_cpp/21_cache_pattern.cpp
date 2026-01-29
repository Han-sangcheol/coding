/* C++ Cache - unordered_map */
#include <iostream>
#include <unordered_map>
using namespace std;

class Cache {
    unordered_map<int, int> cache;
public:
    bool get(int key, int& value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            value = it->second;
            cout << "[Cache] Hit: " << key << endl;
            return true;
        }
        cout << "[Cache] Miss: " << key << endl;
        return false;
    }
    
    void put(int key, int value) {
        cache[key] = value;
    }
};

int main() {
    cout << "=== C++ Cache ===" << endl;
    Cache cache;
    cache.put(1, 100);
    
    int val;
    cache.get(1, val);
    cache.get(2, val);
    
    return 0;
}
