// ============================================================
// MAXIMUM ABSOLUTE SUM OF ANY SUBARRAY — (LeetCode 1749, Medium)   [library-transfer test]
// ============================================================
// int array diya. kisi bhi CONTIGUOUS subarray ka sum lo -> uske ABSOLUTE value
// (|sum|) me se sabse BADA return karo.
//
//   [1,-3,2,3,-4]        -> 5    ([2,3] = 5)
//   [2,-5,1,-4,3,-2]     -> 8    ([-5,1,-4] = -8, |−8| = 8)
//   [1,2,3]              -> 6
//   [-1,-2,-3]           -> 6    (|-6|)
//   [5]                  -> 5
//
// ---- ARPAN KI APPROACH + KADANE FAMILY (22-Jul, derive-first se seekha) ----
//  ★ KADANE = "running value aage carry + har step best update; ya RESTART." har position pe
//     best-ending-HERE nikalo, aur global best track. 3 sagే (transfer-distance chhota):
//  1) MAX SUBARRAY (LC-53, SUM):  sum += nums[i]; ans=max(ans,sum); if(sum<0) sum=0.  (ek running value)
//     [sum me negative-flip NAHI -> ek value kaafi]
//  2) MAX PRODUCT (LC-152):  ★ negative-flip hota (num<0 -> min×num = BADA max) -> MAX aur MIN DONO track.
//     har step 3 CANDIDATE se: {num(restart), num×oldMax, num×oldMin}. newMax=max(3), newMin=min(3).
//     ★ old values FREEZE (temp) -> warna ek naya doosre me ghus jaata. ans=max(ans,maxProd).
//  3) MAX ABS SUM (LC-1749, ye):  ★ #2 ka near-transfer, bas SUM pe. 3 candidate: {num, num+oldMax, num+oldMin}.
//     maxSum/minSum dono track (+temp). ans = max(ans, |maxSum|, |minSum|).  [answer minSum se bhi aa sakta -- e.g. [-1,-2,-3]->6]
//  ★ WHY 3 candidate (na single running prod/sum): single wala RESTART nahi kar sakta + MIN-flip use nahi kar sakta.
//  ★★ SEEKH: ye "max+min track + 3-candidate + freeze" ek MECHANIC hai -> ek baar seekho (dry-run se zero-invent nahi
//     hota) -> phir library me, aur naye problem (jaise ye 1749) me TRANSFER ho jaata. [struggle-first > peek-first]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxAbsoluteSum(vector<int> &nums)
{
    if (nums.size() == 1)
    {
        return nums[0];
    }

    int maxSum = nums[0]; // vector<int> c = {1, 2, 3};
    int minSum = nums[0];
    int ans = 0;
    for (int i = 1; i < nums.size(); i++)
    {
        int temp = max({nums[i], nums[i] + maxSum, nums[i] + minSum});
        minSum = min({nums[i], nums[i] + maxSum, nums[i] + minSum});
        maxSum = temp;
        ans = max({ans, abs(maxSum), abs(minSum)});
    }
    return ans;
}

int main()
{
    vector<int> a = {1, -3, 2, 3, -4};
    vector<int> b = {2, -5, 1, -4, 3, -2};
    vector<int> c = {1, 2, 3};
    vector<int> d = {-1, -2, -3};
    vector<int> e = {5};

    cout << maxAbsoluteSum(a) << " (expected 5)\n";
    cout << maxAbsoluteSum(b) << " (expected 8)\n";
    cout << maxAbsoluteSum(c) << " (expected 6)\n";
    cout << maxAbsoluteSum(d) << " (expected 6)\n";
    cout << maxAbsoluteSum(e) << " (expected 5)\n";
    return 0;
}
