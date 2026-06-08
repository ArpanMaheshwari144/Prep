// ============================================================
// MOVE ZEROES   —   Two Pointer (slow / fast)
// ============================================================
// Array `nums` mein saare 0 ko END mein move karo, non-zero ka ORDER same. IN-PLACE.
//
// Example:
//   {0, 1, 0, 3, 12}  ->  {1, 3, 12, 0, 0}
//   {0, 0, 1}         ->  {1, 0, 0}
//
// INTUITION (soch — code TU likhega):
//   Ek "jagah" pointer — jahan agla NON-ZERO baithega.
//   Ek "khoji" pointer — poora array scan karta.
//   Khoji ko NON-ZERO mile -> "jagah" wale se BADAL do (aage le aao), jagah ek aage.
//   Saare non-zero shuruaat ki taraf (order same), zeros khud peeche.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void moveZeroes(vector<int>& nums) {
    int n = nums.size();
    int slow = 0;
    int fast = 0;
    while(fast < n){
        if(nums[fast] != 0){
            swap(nums[slow], nums[fast]);
            slow++;
        }
        fast++;
    }
}

int main() {
    vector<int> nums = {0, 1, 0, 3, 12};
    moveZeroes(nums);
    for (int x : nums) cout << x << " ";   // expected: 1 3 12 0 0
    cout << endl;
    return 0;
}
