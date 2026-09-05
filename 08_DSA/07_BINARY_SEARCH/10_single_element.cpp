// ============================================================
// SINGLE ELEMENT IN A SORTED ARRAY — Binary Search  (fresh, NAYA trick)
// ============================================================
// sorted array jisme HAR element THEEK 2 baar aata -- SIRF EK element 1 baar.
// wo single (akela) element lauta.  O(log n) chahiye (binary search).
//   [1,1,2,3,3,4,4,8,8]     -> 2
//   [3,3,7,7,10,11,11]      -> 10
//   [1]                     -> 1
//   [1,1,2]                 -> 2
//   [2,3,3]                 -> 2
//


// Tests (nums -> expected):
//   [1,1,2,3,3,4,4,8,8]  -> 2
//   [3,3,7,7,10,11,11]   -> 10
//   [1]                  -> 1
//   [1,1,2]              -> 2
//   [2,3,3]              -> 2
//   [1,1,3,3,5,5,7]      -> 7
//   [1,2,2,3,3]          -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int singleNonDuplicate(vector<int> &nums)
{
    if (nums.size() == 1)
    {
        return nums[0];
    }

    int n = nums.size();
    int low = 0;
    int high = n - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (mid == 0)
            return nums[mid];
            
        if (mid % 2 == 0)
        {
            if (nums[mid] == nums[mid - 1])
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
            if (nums[mid] != nums[mid - 1])
            {
                high = mid - 1;
            }
            else
            {
                low = mid + 1;
            }
        }
    }
    return nums[high];
}

int main()
{
    vector<int> a = {1, 1, 2, 3, 3, 4, 4, 8, 8};
    vector<int> b = {3, 3, 7, 7, 10, 11, 11};
    vector<int> c = {1};
    vector<int> d = {1, 1, 2};
    vector<int> e = {2, 3, 3};
    vector<int> f = {1, 1, 3, 3, 5, 5, 7};
    vector<int> g = {1, 2, 2, 3, 3};

    cout << singleNonDuplicate(a) << " (expected 2)\n";
    cout << singleNonDuplicate(b) << " (expected 10)\n";
    cout << singleNonDuplicate(c) << " (expected 1)\n";
    cout << singleNonDuplicate(d) << " (expected 2)\n";
    cout << singleNonDuplicate(e) << " (expected 2)\n";
    cout << singleNonDuplicate(f) << " (expected 7)\n";
    cout << singleNonDuplicate(g) << " (expected 1)\n";
    return 0;
}
