// ============================================================
// REMOVE DUPLICATES FROM SORTED ARRAY      (Pattern: Two Pointer)
// ============================================================
// Ek SORTED array `nums` diya hai. Duplicates IN-PLACE hatao taaki
// har UNIQUE element sirf EK baar aaye. Unique elements ki count `k`
// return karo. nums ke pehle `k` elements mein unique values (order
// barkarar) honi chahiye. `k` ke baad ke elements se koi farak nahi.
//
// Example 1:
//   nums = [1, 1, 2]               -> return 2, nums = [1, 2, _]
// Example 2:
//   nums = [0,0,1,1,1,2,2,3,3,4]   -> return 5, nums = [0,1,2,3,4,...]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int slow = 0;
        int fast = 0;
        while(fast < nums.size()){
            if(nums[slow] != nums[fast]){
                slow++;
                nums[slow] = nums[fast];
            }
            fast++;
        }
        return slow+1;

    }
};

// ---- test harness (run karke verify — yeh solution nahi, bas runner) ----
int main() {
    Solution s;
    vector<int> nums = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int k = s.removeDuplicates(nums);

    cout << "k = " << k << endl;          // expected: 5
    cout << "first k: [";
    for (int i = 0; i < k; i++)
        cout << nums[i] << (i + 1 < k ? ", " : "");
    cout << "]" << endl;                  // expected: [0, 1, 2, 3, 4]
    return 0;
}
