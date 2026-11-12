// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// TWO MAX-SUM NON-OVERLAPPING SUBARRAYS   (Google-tag)   (REDO_2 — blank, 13-Nov)
// ============================================================
// 2 subarray: CONTIGUOUS, NON-EMPTY, NON-OVERLAPPING. dono ke sum ka JOD max. wo max lautao.
//
// Tests (neeche main me, sab PASS aane chahiye):
//   [2,1,-99,3,3]     -> 9
//   [1,2,-7,8,6,-4]   -> 17
//   [3,3,-100,6,6]    -> 18
//   [1,2,3,4]         -> 10
//   [5,-2,5]          -> 10
//   [-5,10,10]        -> 20
//   [-1,-2]           -> -3     (sab negative, phir bhi dono non-empty)
//   [-3,-1,-2]        -> -3
// ============================================================

#include <bits/stdc++.h>
using namespace std;

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

int maxTwoNonOverlap(vector<int> &nums)
{
    int ans = INT_MIN;
    vector<int> bestForleft = bestLeftPrefix(nums);
    vector<int> bestForRight = bestRightPrefix(nums);
    for (int i = 0; i < nums.size() - 1; i++)
    {
        ans = max(ans, bestForleft[i] + bestForRight[i + 1]);
    }
    return ans;
}

int main()
{
    vector<pair<vector<int>, int>> tests = {
        {{2, 1, -99, 3, 3}, 9},
        {{1, 2, -7, 8, 6, -4}, 17},
        {{3, 3, -100, 6, 6}, 18},
        {{1, 2, 3, 4}, 10},
        {{5, -2, 5}, 10},
        {{-5, 10, 10}, 20},
        {{-1, -2}, -3},
        {{-3, -1, -2}, -3},
    };
    int pass = 0;
    for (auto &t : tests)
    {
        int got = maxTwoNonOverlap(t.first);
        bool ok = (got == t.second);
        if (ok)
            pass++;
        cout << "got " << got << " | exp " << t.second << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
    }
    cout << "\n"
         << pass << "/" << tests.size() << " PASS\n";
    return 0;
}
