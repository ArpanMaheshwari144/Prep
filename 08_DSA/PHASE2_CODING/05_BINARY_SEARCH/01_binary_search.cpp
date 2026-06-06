// ============================================================
// BINARY SEARCH                        (Pattern: Binary Search)
// ============================================================
// Ek SORTED array `nums` (ascending) aur ek int `target` diya hai.
// target ka INDEX return karo agar mile; nahi mile to -1.  O(log n).
//
// Example:
//   nums = [-1, 0, 3, 5, 9, 12], target = 9   ->  4
//   nums = [-1, 0, 3, 5, 9, 12], target = 2   -> -1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int search(vector<int> &nums, int target)
    {
        int n = nums.size();
        int low = 0;
        int high = n - 1;

        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            if (nums[mid] == target)
            {
                return mid;
            }
            else if (nums[mid] < target)
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }
        return -1;
    }
};

// ---- test harness ----
int main()
{
    Solution s;
    vector<int> nums = {-1, 0, 3, 5, 9, 12};
    cout << s.search(nums, 9) << endl; // expected: 4
    cout << s.search(nums, 2) << endl; // expected: -1
    return 0;
}
