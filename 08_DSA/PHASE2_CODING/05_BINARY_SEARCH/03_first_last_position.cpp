// ============================================================
// FIRST AND LAST POSITION              (Pattern: Binary Search x2)
// ============================================================
// Ek SORTED array `nums` (DUPLICATES ho sakte) aur ek int `target`.
// target ka FIRST index aur LAST index return karo: {first, last}.
// target na mile to {-1, -1}.  O(log n).
//
// Example:
//   nums = [5, 7, 7, 8, 8, 10], target = 8   ->  {3, 4}
//   nums = [5, 7, 7, 8, 8, 10], target = 6   ->  {-1, -1}
//   nums = [],                  target = 0   ->  {-1, -1}
//
// Idea (tu ne recognize kiya): 2 binary searches —
//   ek FIRST occurrence ke liye, ek LAST ke liye.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int bsForFirst(vector<int> &nums, int target)
    {
        int first = -1;
        int n = nums.size();
        int low = 0;
        int high = n - 1;
        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            if (nums[mid] == target)
            {
                first = mid;
                high = mid - 1;
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
        return first;
    }

    int bsForLast(vector<int> &nums, int target)
    {
        int last = -1;
        int n = nums.size();
        int low = 0;
        int high = n - 1;
        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            if (nums[mid] == target)
            {
                last = mid;
                low = mid + 1;
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
        return last;
    }
    vector<int> searchRange(vector<int> &nums, int target)
    {
        vector<int> ans;
        ans.push_back(bsForFirst(nums, target));
        ans.push_back(bsForLast(nums, target));
        return ans;
    }
};

// ---- test harness ----
int main()
{
    Solution s;
    vector<int> a = {5, 7, 7, 8, 8, 10};
    auto r1 = s.searchRange(a, 8);
    cout << "{" << r1[0] << ", " << r1[1] << "}" << endl; // expected: {3, 4}
    auto r2 = s.searchRange(a, 6);
    cout << "{" << r2[0] << ", " << r2[1] << "}" << endl; // expected: {-1, -1}
    return 0;
}
