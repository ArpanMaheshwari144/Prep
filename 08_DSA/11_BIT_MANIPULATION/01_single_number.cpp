// ============================================================
// SINGLE NUMBER — (LeetCode 136, Easy)   [bit-manipulation / XOR-cancel]
// ============================================================
// array me har number DO baar aata, sirf EK number ek baar. wahi single lautao.
// constraint: O(n) time + O(1) space (extra map/set NAHI).
//
//   [2,2,1]        -> 1
//   [4,1,2,1,2]    -> 4
//   [1]            -> 1
//   [7,3,5,3,7]    -> 5
//   [-1,-1,-2]     -> -2
//
// ---- ARPAN KI APPROACH ----
//  ★ TRICK: XOR-cancel -> a^a=0 (pair gayab), a^0=a (single bacha). saare ko XOR karo -> jo bacha = answer.
//  ★ 3 line:  XORR = nums[0];   for i=1..n: XORR ^= nums[i];   return XORR.   (O(n)/O(1), koi map nahi)
//  ★ FAMILY (bit-manip/XOR): Missing-Number · Single-Number-II bhi isi tarah.
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int singleNumber(vector<int> &nums)
{
    int XORR = nums[0];
    for (int i = 1; i < nums.size(); i++)
    {
        XORR = XORR ^ nums[i];
    }
    return XORR;
}

int main()
{
    vector<int> a = {2, 2, 1};
    vector<int> b = {4, 1, 2, 1, 2};
    vector<int> c = {1};
    vector<int> d = {7, 3, 5, 3, 7};
    vector<int> e = {-1, -1, -2};

    cout << singleNumber(a) << " (expected 1)\n";
    cout << singleNumber(b) << " (expected 4)\n";
    cout << singleNumber(c) << " (expected 1)\n";
    cout << singleNumber(d) << " (expected 5)\n";
    cout << singleNumber(e) << " (expected -2)\n";
    return 0;
}
