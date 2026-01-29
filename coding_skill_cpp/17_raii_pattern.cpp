/* C++ RAII - 소멸자 자동 호출 */
#include <iostream>
#include <fstream>
using namespace std;

class FileResource {
    FILE* file;
public:
    FileResource(const char* filename) {
        file = fopen(filename, "w");
        cout << "[RAII] 파일 열림" << endl;
    }
    ~FileResource() {
        if (file) fclose(file);
        cout << "[RAII] 파일 자동 닫힘" << endl;
    }
};

int main() {
    cout << "=== C++ RAII ===" << endl;
    {
        FileResource res("test.txt");
    }  // 자동 소멸
    cout << "블록 종료!" << endl;
    return 0;
}
