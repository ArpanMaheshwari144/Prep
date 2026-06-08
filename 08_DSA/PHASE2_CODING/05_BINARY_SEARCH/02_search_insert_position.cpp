// ============================================================
// SEARCH INSERT POSITION               (Pattern: Binary Search — lower bound)
// ============================================================
// Ek SORTED array `nums` (distinct, ascending) aur ek int `target`.
// target mile to uska INDEX return karo. Na mile to woh INDEX return karo
// jahan target ko DAALNA padega (taaki array sorted rahe). O(log n).
//
// Example:
//   nums = [1, 3, 5, 6], target = 5   ->  2   (mil gaya)
//   nums = [1, 3, 5, 6], target = 2   ->  1   (1 aur 3 ke beech)
//   nums = [1, 3, 5, 6], target = 7   ->  4   (end mein)
//   nums = [1, 3, 5, 6], target = 0   ->  0   (shuru mein)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int searchInsert(vector<int> &nums, int target)
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
        return low;
    }
};

int main()
{
    Solution s;
    vector<int> nums = {1, 3, 5, 6};
    cout << s.searchInsert(nums, 5) << endl; // expected: 2
    cout << s.searchInsert(nums, 2) << endl; // expected: 1
    cout << s.searchInsert(nums, 7) << endl; // expected: 4
    cout << s.searchInsert(nums, 0) << endl; // expected: 0
    return 0;
}
