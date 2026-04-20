// Q: Array sorted hai ya nahi check karo (non-decreasing order)
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    bool isSorted(vector<int>& nums) {
        // index 1 se start — pichle wale se compare karo
        for(int i=1;i<nums.size();i++){
            // agar pichla element aage wale se bada hai toh sorted nahi
            if(nums[i-1] > nums[i]){
                return false;
            }
        }
        // sabhi pairs sahi order mein hain — sorted hai
        return true;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {1, 3, 2, 5, 4};
    cout << "Test 1: " << (sol.isSorted(t1) ? "true" : "false");
    cout << (sol.isSorted(t1) == false ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 2, 3, 4, 5};
    cout << "Test 2: " << (sol.isSorted(t2) ? "true" : "false");
    cout << (sol.isSorted(t2) == true ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {5, 4, 3, 2, 1};
    cout << "Test 3: " << (sol.isSorted(t3) ? "true" : "false");
    cout << (sol.isSorted(t3) == false ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1, 1, 2, 2, 3};
    cout << "Test 4: " << (sol.isSorted(t4) ? "true" : "false");
    cout << (sol.isSorted(t4) == true ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {7};
    cout << "Test 5: " << (sol.isSorted(t5) ? "true" : "false");
    cout << (sol.isSorted(t5) == true ? "  PASS" : "  FAIL") << endl;

    return 0;
}
