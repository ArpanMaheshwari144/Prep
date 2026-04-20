// Q: Array mein sirf EVEN numbers ka sum nikalo
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int evenSum(vector<int>& nums) {
        int sum = 0;
        // har element dekho
        for(int i=0;i<nums.size();i++){
            // even hai toh sum mein add, odd hai toh skip
            nums[i] % 2 == 0 ? sum += nums[i] : 0;
        }
        // sirf even elements ka sum return karo
        return sum;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {5, 3, 8, 1, 9, 2};
    cout << "Test 1: " << sol.evenSum(t1);
    cout << (sol.evenSum(t1) == 10 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 3, 5};
    cout << "Test 2: " << sol.evenSum(t2);
    cout << (sol.evenSum(t2) == 0 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {2, 4, 6};
    cout << "Test 3: " << sol.evenSum(t3);
    cout << (sol.evenSum(t3) == 12 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {-2, 3, -4};
    cout << "Test 4: " << sol.evenSum(t4);
    cout << (sol.evenSum(t4) == -6 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
