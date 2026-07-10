// ============================================================
// SEARCH IN ROTATED SORTED ARRAY — Binary Search  (SPACED-REDO, blank)
// ============================================================
// ek sorted array ko kisi pivot pe ROTATE kiya gaya (e.g. [0,1,2,4,5,6,7] -> [4,5,6,7,0,1,2]).
// target diya -> uska INDEX lautao. na mile -> -1.  O(log n) (binary search).
//
// (purana code MAT dekhna -- ye retrieval hai. copy-pen pe socho:
//  mid nikaalo -> kaunsa HALF sorted hai (left ya right)? -> target us sorted half me hai ya nahi?
//  -> accordingly low/high move. blank se khud likho.)
//
// Tests (// expected):
//   [4,5,6,7,0,1,2], target=0  -> 4
//   [4,5,6,7,0,1,2], target=6  -> 2
//   [4,5,6,7,0,1,2], target=3  -> -1
//   [1],             target=1  -> 0
//   [1],             target=0  -> -1
//   [5,1,3],         target=5  -> 0
//   [5,1,3],         target=3  -> 2
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int search(vector<int> &nums, int target)
{
    int low = 0;
    int high = nums.size() - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target)
        {
            return mid;
        }
        else if (nums[low] <= nums[mid])
        {
            if (target >= nums[low] && target <= nums[mid])
            {
                high = mid - 1;
            }
            else
            {
                low = mid + 1;
            }
        }
        else
        {
            if (target >= nums[mid] && target <= nums[high])
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }
    }
    return -1;
}

int main()
{
    vector<int> a = {4, 5, 6, 7, 0, 1, 2};
    vector<int> b = {1};
    vector<int> c = {5, 1, 3};

    cout << search(a, 0) << " (expected 4)\n";
    cout << search(a, 6) << " (expected 2)\n";
    cout << search(a, 3) << " (expected -1)\n";
    cout << search(b, 1) << " (expected 0)\n";
    cout << search(b, 0) << " (expected -1)\n";
    cout << search(c, 5) << " (expected 0)\n";
    cout << search(c, 3) << " (expected 2)\n";
    return 0;
}
