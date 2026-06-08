// ============================================================
// MOVE ZEROES                          (Pattern: Two Pointer - slow/fast)
// ============================================================
// Ek array `nums` diya hai. Saare 0's ko END mein move karo,
// non-zero elements ka ORDER barkarar rakhte hue. IN-PLACE karo
// (naya array nahi banana).
//
// Example:
//   nums = [0, 1, 0, 3, 12]   ->  [1, 3, 12, 0, 0]
//   nums = [0]                ->  [0]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0;
        int fast = 0;
        while(fast < nums.size()){
            if(nums[fast] != 0){
                swap(nums[slow], nums[fast]);
                slow++;
            }
            fast++;
        }
    }
};

int main() {
    Solution s;
    vector<int> nums = {0, 1, 0, 3, 12};
    s.moveZeroes(nums);
    cout << "[";
    for (int i = 0; i < (int)nums.size(); i++)
        cout << nums[i] << (i + 1 < (int)nums.size() ? ", " : "");
    cout << "]" << endl;   // expected: [1, 3, 12, 0, 0]
    return 0;
}
