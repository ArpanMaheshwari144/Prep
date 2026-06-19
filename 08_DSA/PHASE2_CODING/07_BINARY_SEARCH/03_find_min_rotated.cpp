// ============================================================
// FIND MINIMUM IN ROTATED SORTED ARRAY   —   Binary Search (modified)
// ============================================================
// Sorted array kisi pivot pe rotate (no duplicates). MINIMUM value return karo. O(log n).
//   [3,4,5,1,2]      -> 1
//   [4,5,6,7,0,1,2]  -> 0
//   [11,13,15,17]    -> 11   (rotate nahi hua bhi ho sakta)
//
// SIGNAL: "rotated sorted + min dhoondo, O(log n)" -> modified Binary Search.
//   Soch (structure TERA — no pseudo-code):
//     - min = pivot (jahan se chhota wala part shuru hota).
//     - mid pe khade ho. ek SIDE compare karke decide karo min kis half mein hai,
//       doosra half phenk do. (nums[mid] ko kis cheez se compare karega? socho —
//       low wale se ya high wale se? jo bina-target ke "kaunsa half sorted/clean" bataye.)
//     - jab range simat jaaye -> wahi min.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int findMin(vector<int> &nums)
{
    int low = 0;
    int high = nums.size() - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] > nums[high])
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return nums[low];
}

int main()
{
    vector<int> a = {3, 4, 5, 1, 2};
    cout << findMin(a) << endl; // 1

    vector<int> b = {4, 5, 6, 7, 0, 1, 2};
    cout << findMin(b) << endl; // 0

    vector<int> c = {11, 13, 15, 17};
    cout << findMin(c) << endl; // 11

    vector<int> d = {2, 1};
    cout << findMin(d) << endl; // 1

    vector<int> e = {1};
    cout << findMin(e) << endl; // 1
    return 0;
}
