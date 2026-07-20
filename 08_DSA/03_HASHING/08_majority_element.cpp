// ============================================================
// MAJORITY ELEMENT — (LeetCode 169)   (fresh)
// ============================================================
// ek array nums diya jisme ek element "majority" hai -- yaani wo n/2 se
// ZYADA baar aata hai (n = array size). us majority element ko return karo.
// (maan lo majority hamesha exist karta hai.)
//
//   [3,2,3]           -> 3
//   [2,2,1,1,1,2,2]   -> 2
//   [1]               -> 1
//   [5,5,5,2,2]       -> 5
//   [4,4,4,4,3,3,2]   -> 4
//
// ============================================================
// ---- ARPAN KI APPROACH (do tarike) ----
//
//  ★ APPROACH 1 — HASHMAP (count karo)   [O(n) time, O(n) space]
//     har element ki ginti map me rakho (mp[x]++). phir map ghoomo --
//     jiska count > n/2 wahi majority -> return.
//     simple/seedha. space lagta (map). (comment me pada hai neeche.)
//
//  ★ APPROACH 2 — BOYER-MOORE VOTING   [O(n) time, O(1) space]  <- ye laga hai
//     ★ INSIGHT: majority n/2 se ZYADA hai -> baaki SAB milkar bhi usse kam.
//        to har DO ALAG element ko aapas me CANCEL karte jao -> aakhir me
//        majority hi bachega (uske paas cancel karne ko extra bache honge).
//     ★ 2 cheezein: candidate (abhi kaun aage) + count (kitne se aage).
//        - naya == candidate  -> count++   (support badha)
//        - naya != candidate  -> count--   (ek cancel hua)
//        - count == 0         -> purana poora cancel ho gaya -> naya candidate = nums[i],
//                                 ★★ count = 1 (naya khud EK baar aaya -> uska vote 1 se shuru,
//                                    0 chhoda to agli mismatch pe -1 chala jaata, reset kabhi nahi hota -> BUG).
//     ★ end me jo candidate bacha = majority. (majority guaranteed hai, isliye verify-pass ki zaroorat nahi.)
//        trace [3,2,3]: cand=3,c=1 -> i1: 2!=3 c=0 -> cand=2,c=1 -> i2: 3!=2 c=0 -> cand=3,c=1 -> ans 3. ✓
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int majorityElement(vector<int> &nums)
{
    // int n = nums.size();
    // unordered_map<int, int> mp;
    // for (auto &it : nums)
    // {
    //     mp[it]++;
    // }

    // for (auto &it : mp)
    // {
    //     if (it.second > n / 2)
    //     {
    //         return it.first;
    //     }
    // }
    // return 0;

    int count = 1;
    int candidate = nums[0];
    for (int i = 1; i < nums.size(); i++)
    {
        if (candidate == nums[i])
        {
            count++;
        }
        else if (candidate != nums[i])
        {
            count--;

            if (count == 0)
            {
                candidate = nums[i];
                count = 1;
            }
        }
    }
    return candidate;
}

int main()
{
    vector<int> a = {3, 2, 3};
    vector<int> b = {2, 2, 1, 1, 1, 2, 2};
    vector<int> c = {1};
    vector<int> d = {5, 5, 5, 2, 2};
    vector<int> e = {4, 4, 4, 4, 3, 3, 2};

    cout << majorityElement(a) << " (expected 3)\n";
    cout << majorityElement(b) << " (expected 2)\n";
    cout << majorityElement(c) << " (expected 1)\n";
    cout << majorityElement(d) << " (expected 5)\n";
    cout << majorityElement(e) << " (expected 4)\n";
    return 0;
}
