/* C++ Strategy - std::function */
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;

class Sorter {
    function<void(vector<int>&)> strategy;
public:
    void setStrategy(function<void(vector<int>&)> s) { strategy = s; }
    void sort(vector<int>& data) { if (strategy) strategy(data); }
};

int main() {
    cout << "\n=== C++ Strategy ===" << endl;
    Sorter sorter;
    vector<int> nums = {5, 2, 8, 1};
    
    sorter.setStrategy([](vector<int>& d) { sort(d.begin(), d.end()); });
    sorter.sort(nums);
    cout << "오름차순: "; for (int n : nums) cout << n << " "; cout << endl;
    
    sorter.setStrategy([](vector<int>& d) { sort(d.begin(), d.end(), greater<int>()); });
    sorter.sort(nums);
    cout << "내림차순: "; for (int n : nums) cout << n << " "; cout << endl;
    
    return 0;
}
