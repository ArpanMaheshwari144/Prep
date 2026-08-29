// ============================================================
// SORT COLORS  (Dutch National Flag — 3 pointer)
// ============================================================
// Array me sirf 0, 1, 2. IN-PLACE sort karo (0s → 1s → 2s).
// Library sort() NAHI. ideal = single pass.
//
// ---- ARPAN KI APPROACH ----
//  Dutch flag 3 pointer: low=0, mid=0, high=end. mid se scan (mid<=high).
//  nums[mid]==0 -> swap(low,mid), low++,mid++.  ==1 -> sirf mid++.
//  ==2 -> swap(mid,high), high-- (mid NAHI badhao, high se aaya element abhi check hona baaki).
//
// Tests (// expected):
//   [2,0,2,1,1,0]  -> 0 0 1 1 2 2
//   [2,0,1]        -> 0 1 2
//   [0]            -> 0
//   [1,1,1]        -> 1 1 1
//   [2,2,1,0,0]    -> 0 0 1 2 2
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

void sortColors(vector<int> &nums)
{
    int low = 0;
    int mid = 0;
    int high = nums.size() - 1;
    while (mid <= high) // {1, 2, 0};
    {
        if (nums[mid] == 0)
        {
            swap(nums[low], nums[mid]);
            low++;
            mid++;
        }
        else if (nums[mid] == 1)
        {
            mid++;
        }
        else
        {
            // yaha mid++ isliye nahi kyuki jo high se element ayega ukos bhi process karna hai
            swap(nums[mid], nums[high]);
            high--;
        }
    }
}

void print(vector<int> &v)
{
    for (int x : v)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    vector<int> a1 = {2, 0, 2, 1, 1, 0};
    vector<int> a2 = {2, 0, 1};
    vector<int> a3 = {0};
    vector<int> a4 = {1, 1, 1};
    vector<int> a5 = {2, 2, 1, 0, 0};
    vector<int> a6 = {1, 2, 0};       // <-- naya test
    vector<int> a7 = {2, 1, 0};       // <-- naya test

    sortColors(a1);
    print(a1); // 0 0 1 1 2 2
    sortColors(a2);
    print(a2); // 0 1 2
    sortColors(a3);
    print(a3); // 0
    sortColors(a4);
    print(a4); // 1 1 1
    sortColors(a5);
    print(a5); // 0 0 1 2 2
    sortColors(a6);
    print(a6); // 0 1 2   <-- expected
    sortColors(a7);
    print(a7); // 0 1 2   <-- expected
    return 0;
}
