// ═══════════════════════════════════════════════════════════════════════
//  PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: ★ EDGE line + ek chhota dushman-example chalao
// ═══════════════════════════════════════════════════════════════════════
//  COMBINATIONS (LC-77)  -- REDO_1 (khaali page, khud likh)
//  n aur k diye hain. 1..n me se k numbers ke SAARE combinations lautao.
//  Order maayne nahi rakhta: [1,2] aur [2,1] ek hi hain (sirf ek baar aaye).
//  Kis kram me lautao, farak nahi padta (test khud sort karke milata hai).
//
//  TEST-CASES (n, k -> expected):
//   1) 4, 2 -> [1,2] [1,3] [1,4] [2,3] [2,4] [3,4]     (6)
//   2) 1, 1 -> [1]                                     (sabse chhota)
//   3) 3, 3 -> [1,2,3]                                 (k = n: sirf ek)
//   4) 3, 1 -> [1] [2] [3]                             (k = 1: har number akela)
//   5) 4, 3 -> [1,2,3] [1,2,4] [1,3,4] [2,3,4]         (4)
//   6) 2, 1 -> [1] [2]
//   7) 5, 2 -> 10 combinations
//   8) 4, 4 -> [1,2,3,4]                               (k = n)
//   9) 5, 3 -> 10 combinations
//  10) 5, 1 -> [1] [2] [3] [4] [5]
//  11) 6, 5 -> 6 combinations
//  12) 3, 2 -> [1,2] [1,3] [2,3]
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

void solve(int index, int n, int k, vector<vector<int>> &ans, vector<int> &temp)
{
    if (temp.size() == k)
    {
        ans.push_back(temp);
        return;
    }

    for (int i = index; i <= n; i++)
    {
        temp.push_back(i);
        solve(i + 1, n, k, ans, temp);
        temp.pop_back();
    }
}

vector<vector<int>> combine(int n, int k)
{
    vector<vector<int>> ans;
    vector<int> temp;
    solve(1, n, k, ans, temp);
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int passed = 0, total = 0;
void check(int n, int k, vector<vector<int>> exp, int t)
{
    total++;
    vector<vector<int>> got = combine(n, k);
    for (auto &v : got)
        sort(v.begin(), v.end());
    for (auto &v : exp)
        sort(v.begin(), v.end());
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << "T" << t << " (n=" << n << ", k=" << k << "): " << (ok ? "PASS" : "FAIL")
         << "  count=" << got.size() << " (exp " << exp.size() << ")\n";
}

int main()
{
    check(4, 2, {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}, 1);
    check(1, 1, {{1}}, 2);
    check(3, 3, {{1, 2, 3}}, 3);
    check(3, 1, {{1}, {2}, {3}}, 4);
    check(4, 3, {{1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}}, 5);
    check(2, 1, {{1}, {2}}, 6);
    check(5, 2, {{1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 4}, {3, 5}, {4, 5}}, 7);
    check(4, 4, {{1, 2, 3, 4}}, 8);
    check(5, 3, {{1, 2, 3}, {1, 2, 4}, {1, 2, 5}, {1, 3, 4}, {1, 3, 5}, {1, 4, 5}, {2, 3, 4}, {2, 3, 5}, {2, 4, 5}, {3, 4, 5}}, 9);
    check(5, 1, {{1}, {2}, {3}, {4}, {5}}, 10);
    check(6, 5, {{1, 2, 3, 4, 5}, {1, 2, 3, 4, 6}, {1, 2, 3, 5, 6}, {1, 2, 4, 5, 6}, {1, 3, 4, 5, 6}, {2, 3, 4, 5, 6}}, 11);
    check(3, 2, {{1, 2}, {1, 3}, {2, 3}}, 12);
    cout << "\n"
         << passed << "/" << total << " PASS\n";
    return 0;
}
