// ============================================================
// BINARY SEARCH   —   Binary Search (pattern start)
// ============================================================
// Sorted array + target. Return index, ya -1. O(log n).
//   [-1,0,3,5,9,12], target=9  -> 4      target=2 -> -1
//
// SIGNAL: "sorted + search" -> Binary Search (har step aadha kaato).
//   (Structure TU banayega — lo/hi/mid, compare, half chhodo. No pseudo-code.)
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
    vector<int> a = {-1, 0, 3, 5, 9, 12};
    cout << search(a, 9) << endl;  // 4
    cout << search(a, 2) << endl;  // -1
    cout << search(a, -1) << endl; // 0

    vector<int> b = {5};
    cout << search(b, 5) << endl;  // 0
    cout << search(b, -5) << endl; // -1
    return 0;
}
