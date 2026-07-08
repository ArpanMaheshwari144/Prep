// ============================================================
// BINARY SEARCH (basic) — pattern block #1
// ============================================================
// SORTED array nums + ek target diya. target ka INDEX return karo.
// nahi mile -> -1 return karo.  O(log n) chahiye (linear scan nahi).
//
// (approach yahan LIKHA NAHI — khud dimaag se. atko -> "atka hoon yahan" bol, direction dunga.)
//
// Tests (// expected):
//   [-1,0,3,5,9,12], target=9   -> 4
//   [-1,0,3,5,9,12], target=2   -> -1
//   [5],             target=5   -> 0
//   [5],             target=-5  -> -1
//   [1,2,3,4,5],     target=1   -> 0
//   [1,2,3,4,5],     target=5   -> 4
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int search(vector<int> &nums, int target)
{
    int low = 0, high = nums.size() - 1;
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

int main()
{
    vector<int> a1 = {-1, 0, 3, 5, 9, 12};
    vector<int> a2 = {5};
    vector<int> a3 = {1, 2, 3, 4, 5};

    cout << search(a1, 9) << " (expected 4)\n";
    cout << search(a1, 2) << " (expected -1)\n";
    cout << search(a2, 5) << " (expected 0)\n";
    cout << search(a2, -5) << " (expected -1)\n";
    cout << search(a3, 1) << " (expected 0)\n";
    cout << search(a3, 5) << " (expected 4)\n";
    return 0;
}
