// ============================================================
// KTH LARGEST ELEMENT IN AN ARRAY — (LeetCode 215, Medium)   [HEAP #2 · min-heap of size k]
// ============================================================
// nums[] me se K-th SABSE BADA element lautao.
//
//   [3,2,1,5,6,4], k=2                 -> 5   (2nd largest)
//   [3,2,3,1,2,4,5,5,6], k=4           -> 4
//   [1], k=1                           -> 1
//   [7,6,5,4,3,2,1], k=5               -> 3
//
//   ★ TRICK: MIN-HEAP of size k. k sabse bade elements rakho -> unme se SABSE CHHOTA (heap top) = k-th largest.
//     har element push; size > k -> top (chhota) pop. end: top() = answer.
//   ★ C++ min-heap: priority_queue<int, vector<int>, greater<int>> pq;
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (MIN-HEAP of size k)
//  ★ TRICK: MIN-heap me sirf k SABSE BADE elements rakho -> unme se sabse chhota (heap TOP) = k-th largest.
//  1. min-heap (greater<> = chhota top pe).
//  2. har num push -> agar size > k -> top pop (sabse chhota nikaal do). isse k-bade bache rehte.
//  3. loop ke baad: pq.top() = k-th largest.
//  ★ MIN-heap kyun (max nahi): size > k pe SABSE CHHOTA evict karna hai -> min-heap top wahi deta.
//  ★ sort (O(n log n)) se behtar jab sirf k-th chahiye: har op O(log k) -> total O(n log k).
// ============================================================
int findKthLargest(vector<int> &nums, int k)
{
    // MIN-heap: greater<> -> sabse chhota top pe
    priority_queue<int, vector<int>, greater<int>> pq;
    for (auto &it : nums)
    {
        pq.push(it);          // element daalo
        if (pq.size() > k)    // k se zyada ho gaye ->
        {
            pq.pop();         // sabse chhota (top) nikaal do -> sirf k SABSE BADE bache
        }
    }
    return pq.top();          // k bade me se sabse chhota = k-th largest
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int main()
{
    vector<int> a = {3, 2, 1, 5, 6, 4};
    cout << "T1: " << findKthLargest(a, 2) << " (exp 5)\n";
    vector<int> b = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    cout << "T2: " << findKthLargest(b, 4) << " (exp 4)\n";
    vector<int> c = {1};
    cout << "T3: " << findKthLargest(c, 1) << " (exp 1)\n";
    vector<int> d = {7, 6, 5, 4, 3, 2, 1};
    cout << "T4: " << findKthLargest(d, 5) << " (exp 3)\n";
    return 0;
}
