// ============================================================
// FIND PIVOT INDEX — Prefix Sum block #1
// ============================================================
// nums[] diya. wo INDEX dhoondo jahan LEFT ke sabhi elements ka sum ==
// RIGHT ke sabhi elements ka sum. (index khud kisi taraf count nahi hota.)
// aisa PEHLA index return karo; koi na ho -> -1.
//
// (hint: pura TOTAL nikaal lo. phir left-sum chalate raho ->
//  right-sum = total - left-sum - nums[i]. khud socho.)
//
// Tests (// expected):
//   [1,7,3,6,5,6]        -> 3    (left 1+7+3=11, right 5+6=11)
//   [1,2,3]              -> -1
//   [2,1,-1]             -> 0    (left=0, right=1+(-1)=0)
//   [0]                  -> 0
//   [-1,-1,-1,-1,-1,0]   -> 2
// ============================================================
// ---- ARPAN KI APPROACH ----
//  pehle poora totalSum nikaal lo. phir left-se-right chalo, leftSum running rakho.
//  har index i pe rightSum = totalSum - leftSum - nums[i]. agar leftSum == rightSum -> pivot mila, return i.
//  warna leftSum += nums[i] karke aage badho. koi na mile -> -1.
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int pivotIndex(vector<int> &nums)
{
    int totalSum = 0;
    int leftSum = 0;
    for (auto &it : nums)
    {
        totalSum += it;
    }

    for (int i = 0; i < nums.size(); i++) //   [1,7,3,6,5,6]  -> 3    (left 1+7+3=11, right 5+6=11)
    {
        int rightSum = totalSum - leftSum - nums[i];
        if (leftSum == rightSum)
        {
            return i;
        }
        leftSum += nums[i];
    }
    return -1;
}

int main()
{
    vector<int> a1 = {1, 7, 3, 6, 5, 6};
    vector<int> a2 = {1, 2, 3};
    vector<int> a3 = {2, 1, -1};
    vector<int> a4 = {0};
    vector<int> a5 = {-1, -1, -1, -1, -1, 0};

    cout << pivotIndex(a1) << " (expected 3)\n";
    cout << pivotIndex(a2) << " (expected -1)\n";
    cout << pivotIndex(a3) << " (expected 0)\n";
    cout << pivotIndex(a4) << " (expected 0)\n";
    cout << pivotIndex(a5) << " (expected 2)\n";
    return 0;
}
