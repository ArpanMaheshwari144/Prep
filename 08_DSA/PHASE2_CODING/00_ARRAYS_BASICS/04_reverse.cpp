// ============================================================
// REVERSE ARRAY  —  Arrays basics  (PRODUCTION: memory se likh)
// ============================================================
// Array `nums` ko IN-PLACE reverse karo (Two Pointer opposite-ends).
//
// Example:
//   {3, 7, 2, 9, 4, 1}  ->  {1, 4, 9, 2, 7, 3}
//
// SKELETON (yaad): left=0, right=last → while(left<right): swap, left++, right--
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void reverseArray(vector<int> &nums)
{
    int low = 0;
    int high = nums.size() - 1;
    while (low < high)
    {
        swap(nums[low], nums[high]);
        low++;
        high--;
    }
}

int main()
{
    vector<int> nums = {3, 7, 2, 9, 4, 1};
    reverseArray(nums);
    for (int x : nums)
        cout << x << " "; // expected: 1 4 9 2 7 3
    cout << endl;
    return 0;
}
