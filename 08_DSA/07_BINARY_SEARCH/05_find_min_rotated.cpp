// ============================================================
// FIND MINIMUM IN ROTATED SORTED ARRAY — Binary Search block #5
// ============================================================
// ek sorted array ko pivot pe ROTATE kiya (distinct). SABSE CHHOTA element return karo.
//   e.g. [3,4,5,1,2] -> min = 1.   O(log n) chahiye.
//
// (hint — khud socho: mid ko HIGH se compare karo -> min kaunse half me hai,
//  us taraf jao. array pehle se sorted (rotate=0) ka case bhi dhyan.)
//
// Tests (// expected):
//   [3,4,5,1,2]        -> 1
//   [4,5,6,7,0,1,2]    -> 0
//   [11,13,15,17]      -> 11   (rotate nahi hua)
//   [2,1]              -> 1
//   [1]                -> 1
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int findMin(vector<int> &nums)
{
    int low = 0, high = nums.size() - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2; //   [3,4,5,1,2]        -> 1
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
    vector<int> a1 = {3, 4, 5, 1, 2};
    vector<int> a2 = {4, 5, 6, 7, 0, 1, 2};
    vector<int> a3 = {11, 13, 15, 17};
    vector<int> a4 = {2, 1};
    vector<int> a5 = {1};

    cout << findMin(a1) << " (expected 1)\n";
    cout << findMin(a2) << " (expected 0)\n";
    cout << findMin(a3) << " (expected 11)\n";
    cout << findMin(a4) << " (expected 1)\n";
    cout << findMin(a5) << " (expected 1)\n";
    return 0;
}
