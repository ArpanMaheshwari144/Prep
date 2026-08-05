// ============================================================
// TWO MAX-SUM NON-OVERLAPPING SUBARRAYS   (Google-tag)   [REDO_1 — khud bharo]
// ============================================================
// 2 subarray: CONTIGUOUS, NON-EMPTY, NON-OVERLAPPING. dono ke sum ka JOD max. wo max lautao.
//
//   [2,1,-99,3,3] -> 9    [1,2,-7,8,6,-4] -> 17    [3,3,-100,6,6] -> 18
//   [1,2,3,4] -> 10    [5,-2,5] -> 10    [-5,10,10] -> 20
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// STEP 1 — maxEndingAtForLeft  (kadane per-index)
// STEP 2 — bestLeftPrefix       (STEP1 ka running-max, aage)
// STEP 3 — maxEndingAtForRight  (STEP1 ka mirror, peeche)
// STEP 4 — bestRightPrefix      (STEP3 ka running-max, peeche)
// STEP 5 — maxTwoNonOverlap     (WALL: bestLeft[i] + bestRight[i+1], i=0..n-2)

int maxTwoNonOverlap(vector<int> &nums)
{
    // TODO: khud likho (5 step)
    return 0;
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
