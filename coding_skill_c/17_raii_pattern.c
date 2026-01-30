/* RAII (Resource Acquisition Is Initialization) Pattern in C
 * 리소스를 획득할 때 초기화하고, 자동으로 해제되도록 관리
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE* file;
    char filename[64];
} FileHandle;

FileHandle* File_Open(const char* filename) {
    FileHandle* handle = malloc(sizeof(FileHandle));
    handle->file = fopen(filename, "w");
    snprintf(handle->filename, 64, "%s", filename);
    printf("[RAII] 파일 열기: %s\n", filename);
    return handle;
}

void File_Close(FileHandle** handle) {
    if (*handle) {
        if ((*handle)->file) {
            fclose((*handle)->file);
            printf("[RAII] 파일 닫기: %s\n", (*handle)->filename);
        }
        free(*handle);
        *handle = NULL;
    }
}

int main(void) {
    printf("=== RAII Pattern ===\n\n");
    
    FileHandle* file = File_Open("test_raii.txt");
    fprintf(file->file, "RAII Test\n");
    File_Close(&file);  // 자동 해제
    
    printf("\nRAII: 리소스 자동 관리 패턴\n");
    return 0;
}
