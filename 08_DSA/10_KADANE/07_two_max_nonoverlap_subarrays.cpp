// ============================================================
// TWO MAX-SUM NON-OVERLAPPING SUBARRAYS   (Google-tag)
// ============================================================
// 2 subarray: CONTIGUOUS, NON-EMPTY, NON-OVERLAPPING. dono ke sum ka JOD max. wo max lautao.
//
//   [2,1,-99,3,3] -> 9    [1,2,-7,8,6,-4] -> 17    [3,3,-100,6,6] -> 18
//   [1,2,3,4] -> 10    [5,-2,5] -> 10    [-5,10,10] -> 20
// ============================================================
// ---- APPROACH ----  (bestLeft + bestRight -> WALL. 5 step, sab Kadane per-index STORE.)
//  SOCH: 2 non-overlap tukde -> beech me DEEWAAR. 1 poori LEFT, 1 poori RIGHT -> overlap kabhi nahi.
//        har deewaar pe: left-ka-best + right-ka-best -> MAX.
//  STEP 1 maxEndingAtForLeft  = har index tak "yahin-KHATAM best"  (Kadane L->R).
//  STEP 2 bestLeftPrefix      = #1 ka RUNNING-MAX (0..i me best).
//  STEP 3 maxEndingAtForRight = #1 ka MIRROR, right se ("yahin-SHURU best").
//  STEP 4 bestRightPrefix     = #3 ka running-max R->L (i..n-1 me best).
//  STEP 5 WALL                = for i=0..n-2: ans = max(ans, bestLeft[i] + bestRight[i+1]).
//
//  ★ i+1 (non-overlap ki JAAN): left ne index i TAK le liya -> right ko i+1 SE lena.
//     agar bestLeft[i] + bestRight[i] (same index) karo -> index i DONO me aata = OVERLAP -> jhoothа bada (22).
//  ★ loop i=0..n-2 (i<n-1): aakhri split (right = akela last element) MISS mat karo. [-5,10,10] -> 20.
//  DRY-RUN [1,2,-7,8,6,-4]:
//     bestLeft  : [ 1   3   3   8  14  14 ]
//     bestRight : [ 14  14  14  14  6  -4 ]
//     i=1: bestLeft[1]=3 + bestRight[2]=14 = 17   (max).   overlap-galti: bestLeft[3]+bestRight[3]=8+14=22 (index 3 dono me)
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

    for (int i = 0; i < n-1; i++)
    {
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
