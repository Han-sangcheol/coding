/* C++ Linked List - std::list 사용 */
#include <iostream>
#include <list>
using namespace std;

int main() {
    cout << "=== C++ Linked List (std::list) ===" << endl;
    
    list<int> mylist;
    mylist.push_back(10);
    mylist.push_back(20);
    mylist.push_front(5);
    
    for (int val : mylist) {
        cout << val << " ";
    }
    cout << endl;
    
    return 0;
}
