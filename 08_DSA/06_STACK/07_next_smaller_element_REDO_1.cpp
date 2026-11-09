// ============================================================
// PEHLE YE (har problem pe, 30 sec)
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree? intervals? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE -> "baar-baar SAME kya nikal raha?" -> STORE -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" -> X me jo THI HI NAHI, wo line kya? (edge wahin)
//   6. Code ke BAAD: pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// NEXT SMALLER + PREV GREATER   [REDO_1, histogram ke baad usi din]
// ============================================================
// Do function likhne hain (dono VALUE lautayein, na mile to -1, strictly):
//
// (A) nextSmaller: har element ke RIGHT me pehla CHHOTA.
//   [4,5,2,10,8]     -> [2, 2, -1, 8, -1]
//   [1,2,3,4]        -> [-1,-1,-1,-1]
//   [4,3,2,1]        -> [3, 2, 1, -1]
//   [2,2,2]          -> [-1,-1,-1]
//   [5]              -> [-1]
//   [2,1,5,6,2,3]    -> [1, -1, 2, 2, -1, -1]
//
// (B) prevGreater: har element ke LEFT me pehla BADA.
//   [4,5,2,10,8]     -> [-1,-1, 5, -1, 10]
//   [1,2,3,4]        -> [-1,-1,-1,-1]
//   [4,3,2,1]        -> [-1, 4, 3, 2]
//   [2,2,2]          -> [-1,-1,-1]
//   [5]              -> [-1]
//   [2,1,5,6,2,3]    -> [-1, 2, -1, -1, 6, 6]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> nextSmaller(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, -1);
    stack<int> st;
    for (int i = 0; i < n; i++)
    {
        while (!st.empty() && nums[i] < nums[st.top()])
        {
            int tp = st.top();
            st.pop();
            ans[tp] = nums[i];
        }
        st.push(i);
    }
    return ans;
}

vector<int> prevGreater(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, -1);
    stack<int> st;
    for (int i = n - 1; i >= 0; i--)
    {
        while (!st.empty() && nums[i] > nums[st.top()])
        {
            int tp = st.top();
            st.pop();
            ans[tp] = nums[i];
        }
        st.push(i);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void show(const vector<int> &v)
{
    for (int x : v)
        cout << x << " ";
}

void checkNS(vector<int> nums, vector<int> exp, int t)
{
    vector<int> got = nextSmaller(nums);
    cout << "NS" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=";
    show(got);
    cout << "\n";
}

void checkPG(vector<int> nums, vector<int> exp, int t)
{
    vector<int> got = prevGreater(nums);
    cout << "PG" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=";
    show(got);
    cout << "\n";
}

int main()
{
    checkNS({4, 5, 2, 10, 8}, {2, 2, -1, 8, -1}, 1);
    checkNS({1, 2, 3, 4}, {-1, -1, -1, -1}, 2);
    checkNS({4, 3, 2, 1}, {3, 2, 1, -1}, 3);
    checkNS({2, 2, 2}, {-1, -1, -1}, 4);
    checkNS({5}, {-1}, 5);
    checkNS({2, 1, 5, 6, 2, 3}, {1, -1, 2, 2, -1, -1}, 6);

    checkPG({4, 5, 2, 10, 8}, {-1, -1, 5, -1, 10}, 1);
    checkPG({1, 2, 3, 4}, {-1, -1, -1, -1}, 2);
    checkPG({4, 3, 2, 1}, {-1, 4, 3, 2}, 3);
    checkPG({2, 2, 2}, {-1, -1, -1}, 4);
    checkPG({5}, {-1}, 5);
    checkPG({2, 1, 5, 6, 2, 3}, {-1, 2, -1, -1, 6, 6}, 6);
    return 0;
}
