// ============================================================
// SEARCH IN ROTATED SORTED ARRAY — BLANK REDO (no peek!)
// ============================================================
// sorted array ko pivot pe ROTATE kiya (distinct). target ka INDEX return karo,
// nahi mile -> -1.  O(log n).
//
// ---- ARPAN KI APPROACH ----
//  mid==target -> return. warna kaunsa half sorted dekho:
//  nums[low]<=nums[mid] -> LEFT sorted -> target [low..mid] me -> high=mid-1, warna low=mid+1.
//  else RIGHT sorted -> target [mid..high] me -> low=mid+1, warna high=mid-1.
//  aadha-aadha kaat-te ho -> O(log n). mila nahi -> -1.
//
// Tests (// expected):
//   [4,5,6,7,0,1,2], target=0  -> 4
//   [4,5,6,7,0,1,2], target=6  -> 2
//   [4,5,6,7,0,1,2], target=3  -> -1
//   [1],             target=0  -> -1
//   [1],             target=1  -> 0
//   [5,1,3],         target=5  -> 0
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
        int mid = low + (high - low) / 2; //   [4,5,6,7,0,1,2], target=0  -> 4
        if (nums[mid] == target)
        {
            return mid;
        }

        if (nums[low] <= nums[mid])
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
    vector<int> a1 = {4, 5, 6, 7, 0, 1, 2};
    vector<int> a2 = {1};
    vector<int> a3 = {5, 1, 3};

    cout << search(a1, 0) << " (expected 4)\n";
    cout << search(a1, 6) << " (expected 2)\n";
    cout << search(a1, 3) << " (expected -1)\n";
    cout << search(a2, 0) << " (expected -1)\n";
    cout << search(a2, 1) << " (expected 0)\n";
    cout << search(a3, 5) << " (expected 0)\n";
    return 0;
}
