// ============================================================
// FIND MINIMUM IN ROTATED SORTED ARRAY — Binary Search block #5
// ============================================================
// ek sorted array ko pivot pe ROTATE kiya (distinct). SABSE CHHOTA element return karo.
//   e.g. [3,4,5,1,2] -> min = 1.   O(log n) chahiye.
//
//
// Tests (// expected):
//   [3,4,5,1,2]        -> 1
//   [4,5,6,7,0,1,2]    -> 0
//   [11,13,15,17]      -> 11   (rotate nahi hua)
//   [2,1]              -> 1
//   [1]                -> 1
//   [3,1,2]            -> 1    (ye pehle FAIL hota tha high=mid-1 se; high=mid fix ke baad sahi)
//   [5,1,2,3,4]        -> 1    (pivot shuru ke paas)
//   [2,3,4,5,1]        -> 1    (pivot end pe)
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int findMin(vector<int> &nums)
{
    int low = 0, high = nums.size() - 1;
    while (low < high) //  vector<int> a6 = {3, 1, 2};
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] > nums[high])
        {
            low = mid + 1;
        }
        else
        {
            // dekho jab high = mid karna hia tab low < high rakhna hamesha
            high = mid;
        }
    }
    return nums[low];
}

int main()
{
    vector<int> a1 = {3, 4, 5, 1, 2};
    vector<int> a2 = {4, 5, 6, 7, 0, 1, 2};
    vector<int> a3 = {11, 13, 15, 17};
    vector<int> a4 = {2, 1};
    vector<int> a5 = {1};
    vector<int> a6 = {3, 1, 2}; 
    vector<int> a7 = {5, 1, 2, 3, 4};
    vector<int> a8 = {2, 3, 4, 5, 1};

    cout << findMin(a1) << " (expected 1)\n";
    cout << findMin(a2) << " (expected 0)\n";
    cout << findMin(a3) << " (expected 11)\n";
    cout << findMin(a4) << " (expected 1)\n";
    cout << findMin(a5) << " (expected 1)\n";
    cout << findMin(a6) << " (expected 1)\n";
    cout << findMin(a7) << " (expected 1)\n";
    cout << findMin(a8) << " (expected 1)\n";
    return 0;
}
