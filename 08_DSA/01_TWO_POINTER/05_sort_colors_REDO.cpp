// ============================================================
// SORT COLORS (Dutch National Flag) — BLANK REDO
// ============================================================
// array me sirf 0, 1, 2 hain. IN-PLACE sort karo (0s -> 1s -> 2s).
// ★ ek PASS, O(1) extra space (counting-sort/library-sort nahi).
//
// ---- ARPAN KI APPROACH ----
//  Dutch flag 3 pointer: low=0, mid=0, high=end. mid se scan (mid<=high).
//  nums[mid]==0 -> swap(low,mid), low++,mid++.  ==1 -> sirf mid++.
//  ==2 -> swap(mid,high), high-- (mid NAHI badhao, high se aaya element abhi check hona baaki).
//
// Tests (// expected — sorted array):
//   [2,0,2,1,1,0]  -> 0 0 1 1 2 2
//   [2,0,1]        -> 0 1 2
//   [0]            -> 0
//   [1,2,0]        -> 0 1 2
//   [2,2,1,1,0,0]  -> 0 0 1 1 2 2
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

void sortColors(vector<int> &nums)
{
    //   [2,0,2,1,1,0]  -> 0 0 1 1 2 2
    int low = 0;
    int mid = 0;
    int high = nums.size() - 1;

    while (mid <= high)
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
        else if (nums[mid] == 2)
        {
            swap(nums[mid], nums[high]);
            high--;
        }
    }
}

int main()
{
    vector<vector<int>> tests = {
        {2, 0, 2, 1, 1, 0},
        {2, 0, 1},
        {0},
        {1, 2, 0},
        {2, 2, 1, 1, 0, 0}};

    for (auto t : tests)
    {
        sortColors(t);
        for (int x : t)
            cout << x << " ";
        cout << "\n";
    }
    return 0;
}
