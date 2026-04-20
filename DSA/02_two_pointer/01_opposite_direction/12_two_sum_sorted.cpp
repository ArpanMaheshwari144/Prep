// Q: Sorted array mein 2 elements dhundho jinke sum = target — two pointer
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n = nums.size();
        // default ans -1,-1 — agar nahi mila toh ye return hoga
        vector<int> ans(2, -1);
        int low = 0;
        int high = n-1;
        while(low <= high){
            // sum target ke barabar — mil gaya answer
            if(nums[low] + nums[high] == target){
               ans[0] = low;
               ans[1] = high;
               return ans;
            }
            // sum chhota hai — left pointer aage badhao (bada chahiye)
            else if(nums[low] + nums[high] < target){
                low++;
            }
            // sum bada hai — right pointer peeche le jao (chhota chahiye)
            else{
                high--;
            }
        }
        // nahi mila toh -1,-1 return
        return ans;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {1, 3, 5, 7, 10, 12};
    vector<int> r1 = sol.twoSum(t1, 8);
    cout << "Test 1: [" << r1[0] << "," << r1[1] << "]";
    cout << (r1[0] == 0 && r1[1] == 3 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {2, 7, 11, 15};
    vector<int> r2 = sol.twoSum(t2, 9);
    cout << "Test 2: [" << r2[0] << "," << r2[1] << "]";
    cout << (r2[0] == 0 && r2[1] == 1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {1, 2, 3, 4, 5};
    vector<int> r3 = sol.twoSum(t3, 9);
    cout << "Test 3: [" << r3[0] << "," << r3[1] << "]";
    cout << (r3[0] == 3 && r3[1] == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {-3, -1, 0, 2, 5};
    vector<int> r4 = sol.twoSum(t4, 1);
    cout << "Test 4: [" << r4[0] << "," << r4[1] << "]";
    cout << (r4[0] == 1 && r4[1] == 3 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
