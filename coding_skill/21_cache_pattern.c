/* Cache Pattern
 * 자주 사용하는 데이터 캐싱
 */

#include <stdio.h>
#include <string.h>

#define CACHE_SIZE 5

typedef struct {
    int key;
    int value;
    int valid;
} CacheEntry;

typedef struct {
    CacheEntry entries[CACHE_SIZE];
    int hits;
    int misses;
} Cache;

void Cache_Init(Cache* cache) {
    memset(cache->entries, 0, sizeof(cache->entries));
    cache->hits = 0;
    cache->misses = 0;
    printf("[Cache] 초기화\n");
}

int Cache_Get(Cache* cache, int key, int* value) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache->entries[i].valid && cache->entries[i].key == key) {
            *value = cache->entries[i].value;
            cache->hits++;
            printf("[Cache] Hit: key=%d, value=%d\n", key, *value);
            return 0;
        }
    }
    cache->misses++;
    printf("[Cache] Miss: key=%d\n", key);
    return -1;
}

void Cache_Put(Cache* cache, int key, int value) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (!cache->entries[i].valid) {
            cache->entries[i].key = key;
            cache->entries[i].value = value;
            cache->entries[i].valid = 1;
            printf("[Cache] Put: key=%d, value=%d\n", key, value);
            return;
        }
    }
    printf("[Cache] 캐시 가득 참\n");
}

int main(void) {
    printf("=== Cache Pattern ===\n\n");
    
    Cache cache;
    Cache_Init(&cache);
    
    Cache_Put(&cache, 1, 100);
    Cache_Put(&cache, 2, 200);
    
    int value;
    Cache_Get(&cache, 1, &value);
    Cache_Get(&cache, 2, &value);
    Cache_Get(&cache, 3, &value);  // Miss
    
    printf("\nHit rate: %d/%d\n", cache.hits, cache.hits + cache.misses);
    printf("\nCache: 데이터 캐싱 패턴\n");
    return 0;
}
