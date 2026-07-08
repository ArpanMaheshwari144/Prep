// ============================================================
// FIRST & LAST POSITION of target — Binary Search block #3
// ============================================================
// SORTED array (DUPLICATES ho sakte) + target diya.
//   target ka PEHLA index aur AAKHRI index return karo -> {first, last}.
//   nahi mile -> {-1, -1}.  O(log n) chahiye (linear nahi).
//
// (hint: ek normal BS se "mil gaya" pe RUKO mat — us direction me aur DHUNDO.
//  do baar BS: ek LEFT-most ke liye, ek RIGHT-most ke liye. approach khud socho.)
//
// Tests (// expected):
//   [5,7,7,8,8,10], target=8  -> 3 4
//   [5,7,7,8,8,10], target=6  -> -1 -1
//   [],             target=0  -> -1 -1
//   [1],            target=1  -> 0 0
//   [2,2],          target=2  -> 0 1
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> searchRange(vector<int> &nums, int target)
{
    int low = 0, high = nums.size() - 1;
    // ★ VECTOR rule:
    //   size PATA hai (fixed)   -> vector<int> ans(2,-1);  phir ans[0]=.., ans[1]=..  (slots pehle se bane, SET karo).
    //   size NAHI pata (badhta) -> vector<int> ans;        phir ans.push_back(..)      (naya slot END me jodta).
    //   ★ dono MIX mat karo -> pichhli galti: ans(2,-1) + push_back = [-1,-1,mid...] (extra append, ans[0]/[1] -1 reh gaye).
    //   yahan size FIXED (2: first+last) -> isliye ans(2,-1) + index-set.
    vector<int> ans(2, -1);
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target)
        {
            ans[0] = mid;
            high = mid - 1;
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

    low = 0, high = nums.size() - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target)
        {
            ans[1] = mid;
            low = mid + 1;
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
    return ans;
}

int main()
{
    vector<int> a1 = {5, 7, 7, 8, 8, 10};
    vector<int> a2 = {};
    vector<int> a3 = {1};
    vector<int> a4 = {2, 2};

    auto p = [](vector<int> r)
    { cout << r[0] << " " << r[1] << "\n"; };
    p(searchRange(a1, 8)); // 3 4
    p(searchRange(a1, 6)); // -1 -1
    p(searchRange(a2, 0)); // -1 -1
    p(searchRange(a3, 1)); // 0 0
    p(searchRange(a4, 2)); // 0 1
    return 0;
}
