// ============================================================
// TWO SUM II — INPUT SORTED — Two Pointer  (fresh)
// ============================================================
// SORTED array + target diya. do numbers dhoondo jinka sum == target.
// unke 1-BASED index lautao [i, j]. (exactly ek solution hai.)
//   [2,7,11,15], target=9  -> [1,2]   (2+7=9)
//
// ---- ARPAN KI APPROACH ----
//  sorted hai to hashmap ki zaroorat nahi. 2 pointer left=0, right=end.
//  sum == target -> return {left+1, right+1} (1-based index).
//  sum < target -> left++ (bada chahiye). sum > target -> right-- (chhota chahiye).
//
// Tests (array, target -> expected 1-based):
//   [2,7,11,15], t=9    -> 1 2
//   [2,3,4], t=6        -> 1 3
//   [-1,0], t=-1        -> 1 2
//   [1,2,3,4,4,9,56,90], t=8 -> 4 5
//   [5,25,75], t=100    -> 2 3
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> twoSum(vector<int> &nums, int target)
{
    int left = 0, right = nums.size() - 1;
    while (left < right)
    {
        if (nums[left] + nums[right] == target)
        {
            return {left + 1, right + 1};
        }
        else if (nums[left] + nums[right] < target)
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    return {-1, -1};
}

int main()
{
    vector<int> a = {2, 7, 11, 15};
    vector<int> b = {2, 3, 4};
    vector<int> c = {-1, 0};
    vector<int> d = {1, 2, 3, 4, 4, 9, 56, 90};
    vector<int> e = {5, 25, 75};

    auto p = [](vector<int> r)
    { for (int x : r) cout << x << " "; cout << "\n"; };

    p(twoSum(a, 9));   // 1 2
    p(twoSum(b, 6));   // 1 3
    p(twoSum(c, -1));  // 1 2
    p(twoSum(d, 8));   // 4 5
    p(twoSum(e, 100)); // 2 3
    return 0;
}
