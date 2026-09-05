// ============================================================
// SUBSETS — (LeetCode 78, Medium)   [BACKTRACKING #1 · choose/explore/un-choose]
// ============================================================
// nums[] diya (saare UNIQUE). uske SAARE subsets (power set) lautao — koi bhi order.
//
//   [1,2,3]  ->  [], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]   (total 2^3 = 8)
//   [0]      ->  [], [0]
//   [1,2]    ->  [], [1], [2], [1,2]
//
//   backtracking: har element pe 2 CHOICE -> include karo ya skip.
//     CHOOSE (element daalo path me) -> EXPLORE (aage recurse) -> UN-CHOOSE (nikaalo, agla try).
//     har "leaf"/har node pe current path = ek subset -> ans me daalo.
//   ek 'start' index rakhna (kahan tak pahunche) -> repeat/duplicate na aaye.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp)
{
    // base case: agar index nums.size() ke aage/barabar -> ek poora subset ban gaya
    if (index >= nums.size())
    {
        ans.push_back(temp);
        return;
    }

    // INCLUDE: element le liya
    temp.push_back(nums[index]);
    solve(nums, ans, index + 1, temp); // aage badho
    temp.pop_back();                   // aur usko hata diya (un-choose / backtrack)

    // EXCLUDE: element nahi liya
    solve(nums, ans, index + 1, temp);
}

vector<vector<int>> subsets(vector<int> &nums)
{
    vector<vector<int>> ans;
    vector<int> temp; // ye BAHAR hi banega -- agar helper me banate to har call naya banta (backtrack toot jaata)
    solve(nums, ans, 0, temp);
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
// (order maayne nahi rakhta -> set-of-sets compare)
void check(vector<int> in, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = subsets(in);
    for (auto &v : got)
        sort(v.begin(), v.end());
    for (auto &v : exp)
        sort(v.begin(), v.end());
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  count=" << got.size() << " (exp " << exp.size() << ")\n";
}

int main()
{
    check({1, 2, 3}, {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}}, 1);
    check({0}, {{}, {0}}, 2);
    check({1, 2}, {{}, {1}, {2}, {1, 2}}, 3);
    return 0;
}
