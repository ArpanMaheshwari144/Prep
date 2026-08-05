// ============================================================
// TWO MAX-SUM NON-OVERLAPPING SUBARRAYS   (Google-tag)
// ============================================================
// 2 subarray: CONTIGUOUS, NON-EMPTY, NON-OVERLAPPING. dono ke sum ka JOD max. wo max lautao.
//
//   [2,1,-99,3,3] -> 9    [1,2,-7,8,6,-4] -> 17    [3,3,-100,6,6] -> 18
//   [1,2,3,4] -> 10    [5,-2,5] -> 10    [-5,10,10] -> 20
// ============================================================
// ---- APPROACH ----   (= 06 (bestLeft) + uska MIRROR (bestRight) -> WALL se jodo)
//  ESSENCE: har wall pe -> [0..i] ka best + [i+1..n-1] ka best -> in sab ka MAX. bas yehi.
//  TRICK: 2 non-overlap tukde -> beech me DEEWAAR. har deewaar pe left-best + right-best -> MAX.
//     bestLeft  = 06 wala (aage chalke: i = 1 -> n-1).
//     bestRight = 06 wala hi ULTA (peeche chalke: i = n-2 -> 0).
//     for(i=0..n-2)  ans = max( ans , bestLeft[i] + bestRight[i+1] );   // i+1 = non-overlap
//  ★ VISUAL [1,2,-7,8,6,-4]:
//     val       : [ 1    2   -7    8    6   -4 ]
//     maxEndAt  : [ 1    3   -4    8   14   10 ]   (05)
//     bestLeft  : [ 1    3    3    8   14   14 ]   (06)
//     bestRight : [ 14   14   14   14   6   -4 ]   (06 ka mirror)
//     i=1: bestLeft[1]=3 + bestRight[2]=14 = 17   (max)
//  ★ i+1 KYUN: bestRight[i] (SAME index) -> index i dono me = OVERLAP -> jhootha 22. i+1 se dono alag.
//  ★ loop i<n-1: aakhri split (right = akela last element) MISS mat karo. [-5,10,10] -> 20.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- STEP 1 ----   test: [1,2,-7,8,6,-4]  ->  [1, 3, -4, 8, 14, 10]
vector<int> maxEndingAtForLeft(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    ans[0] = nums[0];
    for (int i = 1; i < n; i++)
    {
        ans[i] = max(nums[i], ans[i - 1] + nums[i]);
    }
    return ans;
}

// ---- STEP 2 ----   (STEP 1 ka running-max)   test: [1,2,-7,8,6,-4]  ->  [1, 3, 3, 8, 14, 14]
vector<int> bestLeftPrefix(vector<int> &nums)
{
    int n = nums.size();
    vector<int> temp = maxEndingAtForLeft(nums);
    vector<int> ans(n, 0);
    ans[0] = temp[0];
    for (int i = 1; i < n; i++)
    {
        ans[i] = max(ans[i - 1], temp[i]);
    }
    return ans;
}

// ---- STEP 3 ----   (STEP 1 ka MIRROR, right se)   test: [1,2,-7,8,6,-4]  ->  [10, 9, 7, 14, 6, -4]
vector<int> maxEndingAtForRight(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    ans[n - 1] = nums[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        ans[i] = max(nums[i], ans[i + 1] + nums[i]);
    }
    return ans;
}

// ---- STEP 4 ----   (STEP 3 ka running-max, right se)   test: [1,2,-7,8,6,-4]  ->  [14, 14, 14, 14, 6, -4]
vector<int> bestRightPrefix(vector<int> &nums)
{
    int n = nums.size();
    vector<int> temp = maxEndingAtForRight(nums);
    vector<int> ans(n, 0);
    ans[n - 1] = temp[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        ans[i] = max(ans[i + 1], temp[i]);
    }
    return ans;
}

// ---- STEP 5 ----   (WALL: bestLeft + bestRight jodo)   test: [1,2,-7,8,6,-4]  ->  17
int maxTwoNonOverlap(vector<int> &nums)
{
    int n = nums.size();
    vector<int> bestLeft = bestLeftPrefix(nums);
    vector<int> bestRight = bestRightPrefix(nums);
    int ans = INT_MIN;

    for (int i = 0; i < n - 1; i++)
    {
        // i+1 KYUN: right ko i se lete to shared index DONO subarray me -> do baar count -> OVERLAP
        // (e.g. bestLeft[3]=8 + bestRight[3]=14 = 22, index 3 dono me = jhootha). i+1 (ya i-1) se dono ALAG.
        ans = max(ans, bestLeft[i] + bestRight[i + 1]);
    }

    return ans;
}

int main()
{
    vector<int> a = {2, 1, -99, 3, 3};
    vector<int> b = {1, 2, -7, 8, 6, -4};
    vector<int> c = {3, 3, -100, 6, 6};
    vector<int> d = {1, 2, 3, 4};
    vector<int> e = {5, -2, 5};
    vector<int> f = {-5, 10, 10};

    cout << maxTwoNonOverlap(a) << " (expected 9)\n";
    cout << maxTwoNonOverlap(b) << " (expected 17)\n";
    cout << maxTwoNonOverlap(c) << " (expected 18)\n";
    cout << maxTwoNonOverlap(d) << " (expected 10)\n";
    cout << maxTwoNonOverlap(e) << " (expected 10)\n";
    cout << maxTwoNonOverlap(f) << " (expected 20)\n";
    return 0;
}
