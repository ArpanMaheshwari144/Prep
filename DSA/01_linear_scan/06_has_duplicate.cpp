// Q: Array mein koi duplicate element hai ya nahi check karo
#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

class Solution {
public:
    bool hasDuplicate(vector<int>& nums) {
        // set mein dekhe hue elements rakhenge
        unordered_set<int> st;
        for(int i=0;i<nums.size(); i++){
            // agar ye element pehle se set mein hai toh duplicate mil gaya
            if(st.count(nums[i])){
                return true;
            }
            // nahi mila toh set mein daal do
            st.insert(nums[i]);
        }
        // koi duplicate nahi mila
        return false;

    }
};

int main() {
    Solution sol;

    vector<int> t1 = {3, 7, 1, 9, 4, 9, 2};
    cout << "Test 1: " << (sol.hasDuplicate(t1) ? "true" : "false");
    cout << (sol.hasDuplicate(t1) == true ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 2, 3, 4};
    cout << "Test 2: " << (sol.hasDuplicate(t2) ? "true" : "false");
    cout << (sol.hasDuplicate(t2) == false ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {5, 5};
    cout << "Test 3: " << (sol.hasDuplicate(t3) ? "true" : "false");
    cout << (sol.hasDuplicate(t3) == true ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1};
    cout << "Test 4: " << (sol.hasDuplicate(t4) ? "true" : "false");
    cout << (sol.hasDuplicate(t4) == false ? "  PASS" : "  FAIL") << endl;

    return 0;
}
