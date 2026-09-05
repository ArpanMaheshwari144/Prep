// ============================================================
// SLIDING WINDOW MAXIMUM — (LeetCode 239, Hard)   [SLIDING WINDOW #14]
// ============================================================
// nums[] aur k diya. har window (size k) ka MAXIMUM nikaalo, left->right slide karte.
// return: har window-position ka max ek vector me.
//
// ---- TEST CASES ----
//   nums=[1,3,-1,-3,5,3,6,7], k=3  -> [3,3,5,5,6,7]
//   nums=[1],                 k=1  -> [1]
//   nums=[1,-1],              k=1  -> [1,-1]
//   nums=[9,11],              k=2  -> [11]
//   nums=[4,-2],              k=2  -> [4]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ★ ORDER MATTERS: (1) front-expiry + (2) monotonic-clean DONO (3) answer-record se PEHLE.
vector<int> maxSlidingWindow(vector<int> &nums, int k)
{
    deque<int> dq;  // INDICES rakhta (values nahi); front = current window ka MAX
    vector<int> ans;

    for (int i = 0; i < nums.size(); i++)
    {
        // 1) FRONT-EXPIRY: front-index window se bahar (== i-k) -> hata.
        // YEHI wajah DEQUE lene ki -- stack sirf ek end se pop karta; window ke liye front-removal chahiye.
        if (!dq.empty() && dq.front() == i - k)
        {
            dq.pop_front();
        }

        // 2) MONOTONIC (back-pop): nums[i] se CHHOTE back-wale bekaar -> pop.
        // ye UNIVERSAL hai (stack me bhi yahi; deque ka special NAHI).
        while (!dq.empty() && nums[dq.back()] <= nums[i])
        {
            dq.pop_back();
        }
        dq.push_back(i);  // current index daal do

        // 3) RECORD: window poora bana (i >= k-1) -> front = max -> ans me daal.
        if (i >= k - 1)
        {
            ans.push_back(nums[dq.front()]);
        }
    }
    return ans;
}

// ---- test helper (ise mat chhed) ----
void check(vector<int> got, vector<int> exp, const string &label)
{
    cout << label << " -> " << (got == exp ? "PASS" : "*** FAIL ***") << "\n";
    if (got != exp)
    {
        cout << "   got: ";
        for (int x : got)
            cout << x << " ";
        cout << "\n   exp: ";
        for (int x : exp)
            cout << x << " ";
        cout << "\n";
    }
}

int main()
{
    vector<int> a1 = {1, 3, -1, -3, 5, 3, 6, 7};
    check(maxSlidingWindow(a1, 3), {3, 3, 5, 5, 6, 7}, "case1");
    vector<int> a2 = {1};
    check(maxSlidingWindow(a2, 1), {1}, "case2");
    vector<int> a3 = {1, -1};
    check(maxSlidingWindow(a3, 1), {1, -1}, "case3");
    vector<int> a4 = {9, 11};
    check(maxSlidingWindow(a4, 2), {11}, "case4");
    vector<int> a5 = {4, -2};
    check(maxSlidingWindow(a5, 2), {4}, "case5");
    return 0;
}
