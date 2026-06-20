// ============================================================
// SEARCH INSERT POSITION   —   Binary Search
// ============================================================
// Sorted array (no duplicates) + target. Agar target HAI -> uska index.
// Agar NAHI -> woh index jahan insert karne se array sorted rahe.
//   [1,3,5,6], target=5 -> 2   (mil gaya)
//   [1,3,5,6], target=2 -> 1   (1 aur 3 ke beech)
//   [1,3,5,6], target=7 -> 4   (sabse aage)
//   [1,3,5,6], target=0 -> 0   (sabse pehle)
//
// SIGNAL: "sorted + position dhoondo" -> Binary Search.
//   (structure tera -- plain BS. Soch: target mil gaya -> mid.
//    nahi mila, loop khatam -> low kahan khada hota? wahi insert-position.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int searchInsert(vector<int> &nums, int target)
{
    int low = 0;
    int high = nums.size() -1;
    int pos = -1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target)
        {
            return mid;
        }
        else if (nums[mid] > target)
        {
            high = mid - 1;
            pos = mid;
        }
        else
        {
            low = mid + 1;
            pos = mid + 1;
        }
        // cout << low << " " << high << " " << mid;
    }
    return pos;
}

int main()
{
    vector<int> a = {1, 3, 5, 6};
    cout << searchInsert(a, 5) << endl; // 2
    cout << searchInsert(a, 2) << endl; // 1
    cout << searchInsert(a, 7) << endl; // 4
    cout << searchInsert(a, 0) << endl; // 0

    vector<int> b = {1};
    cout << searchInsert(b, 0) << endl; // 0
    cout << searchInsert(b, 2) << endl; // 1
    return 0;
}
