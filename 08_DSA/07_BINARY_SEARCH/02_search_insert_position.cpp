// ============================================================
// SEARCH INSERT POSITION — Binary Search block #2
// ============================================================
// SORTED array (distinct) + target diya.
//   target mile     -> uska INDEX return karo.
//   target NA mile  -> jahan use INSERT karna chahiye (sorted rahe) wo INDEX return karo.
// O(log n) chahiye.
//
// (approach yahan LIKHA NAHI — khud dimaag se. atko -> "atka hoon yahan" bol.)
//
// Tests (// expected):
//   [1,3,5,6], target=5  -> 2   (mila)
//   [1,3,5,6], target=2  -> 1   (1 aur 3 ke beech)
//   [1,3,5,6], target=7  -> 4   (sabse aage)
//   [1,3,5,6], target=0  -> 0   (sabse pehle)
//   [1],       target=0  -> 0
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int searchInsert(vector<int> &nums, int target)
{
    int low = 0, high = nums.size() - 1; // [1,3,5,6], target=2  -> 1   (1 aur 3 ke beech)
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

int main()
{
    vector<int> a1 = {1, 3, 5, 6};
    vector<int> a2 = {1};

    cout << searchInsert(a1, 5) << " (expected 2)\n";
    cout << searchInsert(a1, 2) << " (expected 1)\n";
    cout << searchInsert(a1, 7) << " (expected 4)\n";
    cout << searchInsert(a1, 0) << " (expected 0)\n";
    cout << searchInsert(a2, 0) << " (expected 0)\n";
    return 0;
}
