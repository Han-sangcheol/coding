/* C++ Memory Pool - custom allocator */
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

template<typename T, size_t BlockSize = 4096>
class MemoryPool {
    struct Block { uint8_t data[sizeof(T)]; Block* next; };
    Block* freeList = nullptr;
    vector<unique_ptr<uint8_t[]>> chunks;
    
public:
    T* allocate() {
        if (!freeList) {
            auto chunk = make_unique<uint8_t[]>(BlockSize);
            Block* block = reinterpret_cast<Block*>(chunk.get());
            for (size_t i = 0; i < BlockSize / sizeof(Block) - 1; ++i) {
                block[i].next = &block[i + 1];
            }
            block[BlockSize / sizeof(Block) - 1].next = nullptr;
            freeList = block;
            chunks.push_back(move(chunk));
        }
        
        Block* block = freeList;
        freeList = block->next;
        return reinterpret_cast<T*>(block);
    }
    
    void deallocate(T* ptr) {
        Block* block = reinterpret_cast<Block*>(ptr);
        block->next = freeList;
        freeList = block;
    }
};

int main() {
    cout << "=== C++ Memory Pool ===" << endl;
    MemoryPool<int> pool;
    
    int* p1 = pool.allocate();
    *p1 = 100;
    cout << "Allocated: " << *p1 << endl;
    
    pool.deallocate(p1);
    cout << "Deallocated" << endl;
    
    return 0;
}
