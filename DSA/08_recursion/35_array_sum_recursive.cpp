// Q: Array ka sum nikalo recursion se — bina loop ke
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // Array ka sum nikaal recursively — loop nahi lagana
    // Base case: index == n → return 0
    int arraySum(vector<int>& nums, int index) {
        // base case — array khatam ho gaya toh 0 return karo
        if(index == nums.size()){
            return 0;
        }

        // current element + baaki array ka sum
        int sum = nums[index] + arraySum(nums, index+1);
        return sum;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {3, 1, 4, 1, 5};
    cout << "Test 1: " << sol.arraySum(t1, 0);
    cout << (sol.arraySum(t1, 0) == 14 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {10};
    cout << "Test 2: " << sol.arraySum(t2, 0);
    cout << (sol.arraySum(t2, 0) == 10 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {-1, -2, -3};
    cout << "Test 3: " << sol.arraySum(t3, 0);
    cout << (sol.arraySum(t3, 0) == -6 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {};
    cout << "Test 4: " << sol.arraySum(t4, 0);
    cout << (sol.arraySum(t4, 0) == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
