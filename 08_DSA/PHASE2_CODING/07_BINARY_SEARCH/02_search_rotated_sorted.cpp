// ============================================================
// SEARCH IN ROTATED SORTED ARRAY   —   Binary Search (modified)
// ============================================================
// Sorted array kisi pivot pe rotate. target ka index, ya -1. O(log n).
//   [4,5,6,7,0,1,2], target=0 -> 4    target=3 -> -1
//
// SIGNAL: "rotated sorted + search O(log n)" -> modified Binary Search.
//   KEY (do alag sawaal — exact conditions TU likhega):
//     mid pe nums[mid]==target? -> mil gaya.
//     warna 2 sawaal:
//       1. kaunsa half SORTED? (nums[low] vs nums[mid] — target nahi)
//       2. us SORTED half ki RANGE mein target hai? -> wahan jao : doosre half jao
//   (<= vs < range-check dhyaan se. No pseudo-code — structure tera.)
// ============================================================

#include <bits/stdc++.h>
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
        if (nums[low] <= nums[mid])
        {
            if (nums[low] <= target && target < nums[mid])
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
            if (nums[mid] <= target && target < nums[high])
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
    cout << search(a, 0) << endl; // 4
    cout << search(a, 3) << endl; // -1
    cout << search(a, 4) << endl; // 0
    cout << search(a, 2) << endl; // 6
    cout << search(a, 7) << endl; // 3

    vector<int> b = {1};
    cout << search(b, 1) << endl; // 0
    return 0;
}
