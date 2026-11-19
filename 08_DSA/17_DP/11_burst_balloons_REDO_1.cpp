// ==============================================================
// BURST BALLOONS  (LC-312, Hard)  -- REDO_1 (blank, khud likh)
// --------------------------------------------------------------
// PEHLE YE (30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
//   7. Pattern yaad aaya -> PEHLE COPY-PEN pe chhota example haath se chalao, PHIR code.
// --------------------------------------------------------------
// KYA KARNA:
//   nums[] = balloons ki row. Ek-ek karke saare phodo.
//   Balloon i phodne pe coins = (left padosi) * nums[i] * (right padosi),
//   padosi = US WAQT ke zinda balloons (phoote hue gaayab ho jaate). Kinaare pe padosi na ho to 1 maano.
//   MAX total coins lautao.
//
// TEST CASES (input -> expected):
//   [3,1,5,8]          -> 167
//   [1,5]              -> 10
//   [2,4,6]            -> 66
//   [7]                -> 7
//   [1,1,1]            -> 3
//   [5,0,5]            -> 30
//   [9,76,64,21]       -> 116718
//   [8,2,6,8,9,8,1]    -> 1712
// ==============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(int i, int j, vector<int> &p, vector<vector<int>> &dp)
{
    if (i > j)
        return 0;

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int ans = INT_MIN;
    for (int k = i; k <= j; k++)
    {
        ans = max(ans, solve(i, k - 1, p, dp) + solve(k + 1, j, p, dp) + p[i - 1] * p[k] * p[j + 1]);
    }
    return dp[i][j] = ans;
}

int matrixChain(vector<int> &p, int n, vector<vector<int>> &dp)
{
    return solve(1, n, p, dp);
}

int maxCoins(vector<int> &nums)
{
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    nums.insert(nums.begin(), 1);
    nums.push_back(1);
    return matrixChain(nums, n, dp);
}

int main()
{
    vector<pair<vector<int>, int>> tests = {
        {{3, 1, 5, 8}, 167},
        {{1, 5}, 10},
        {{2, 4, 6}, 66},
        {{7}, 7},
        {{1, 1, 1}, 3},
        {{5, 0, 5}, 30},
        {{9, 76, 64, 21}, 116718},
        {{8, 2, 6, 8, 9, 8, 1}, 1712},
    };

    int pass = 0;
    for (int t = 0; t < (int)tests.size(); t++)
    {
        vector<int> in = tests[t].first;
        int got = maxCoins(in);
        bool ok = (got == tests[t].second);
        if (ok)
            pass++;
        cout << "Test " << t + 1 << ": got " << got << ", expected " << tests[t].second
             << (ok ? "  PASS" : "  FAIL") << "\n";
    }
    cout << "\n"
         << pass << "/" << tests.size() << " pass\n";
    return 0;
}
