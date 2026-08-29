// ============================================================
// SLIDING WINDOW MAXIMUM   (LC-239 — REDO_1, monotonic DEQUE)
// ============================================================
// nums[] + window-size k. har size-k window ka MAXIMUM nikaalo -> answer array.
// window ek-ek step slide karti (left->right). n-k+1 windows banti.
//
//   nums=[1,3,-1,-3,5,3,6,7], k=3 -> [3,3,5,5,6,7]
//     [1 3 -1]      -> 3
//       [3 -1 -3]   -> 3
//         [-1 -3 5] -> 5   ... aise aage.
//
// >>> METHOD (yaad kar, blank se — kal daily-temps monotonic-STACK kiya, ye monotonic DEQUE): <<<
//   - deque me INDEX rakho (value nahi). deque front = current window ka MAX-index.
//   - 3 taar: (1) FRONT-expiry: agar front index window se bahar (i-k) -> pop_front.
//             (2) MONOTONIC clean: jab tak deque-back ka value <= nums[i] -> pop_back (chhote bekaar).
//             (3) push_back(i); phir window poori (i>=k-1) -> ans me nums[front] daalo.
//   - ★ ORDER MATTERS: front-expiry -> clean+push -> record. record hamesha LAST.
//   -- dekhna nahi, khud likh. atko + khud poocho -> tab nudge.
//
// ---- TEST CASES (nums, k -> expected) ----
//   [1,3,-1,-3,5,3,6,7], 3   -> [3,3,5,5,6,7]
//   [1],                 1   -> [1]
//   [1,-1],              1   -> [1,-1]
//   [9,11],              2   -> [11]
//   [4,-2],              2   -> [4]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> maxSlidingWindow(vector<int> &nums, int k)
{
    vector<int> ans;
    deque<int> dq;
    for (int i = 0; i < nums.size(); i++)
    {
        if (!dq.empty() && dq.front() == i - k)
        {
            dq.pop_front();
        }
        while (!dq.empty() && nums[dq.back()] <= nums[i])
        {
            dq.pop_back();
        }
        dq.push_back(i);
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
    bool ok = (got == exp);
    cout << label << " -> got [";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "] | exp [";
    for (int i = 0; i < (int)exp.size(); i++)
        cout << exp[i] << (i + 1 < (int)exp.size() ? "," : "");
    cout << "]" << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
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

    vector<int> a6 = {5, 2, 3, 1, 1};
    check(maxSlidingWindow(a6, 3), {5, 3, 3}, "case6");
    return 0;
}
