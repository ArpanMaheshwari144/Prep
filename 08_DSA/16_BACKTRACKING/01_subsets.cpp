// ============================================================
// SUBSETS — (LeetCode 78, Medium)   [BACKTRACKING #1 · choose/explore/un-choose]
// ============================================================
// nums[] diya (saare UNIQUE). uske SAARE subsets (power set) lautao — koi bhi order.
//
//   [1,2,3]  ->  [], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]   (total 2^3 = 8)
//   [0]      ->  [], [0]
//   [1,2]    ->  [], [1], [2], [1,2]
//
//   ★ backtracking: har element pe 2 CHOICE -> include karo ya skip.
//     CHOOSE (element daalo path me) -> EXPLORE (aage recurse) -> UN-CHOOSE (nikaalo, agla try).
//     har "leaf"/har node pe current path = ek subset -> ans me daalo.
//   ★ ek 'start' index rakhna (kahan tak pahunche) -> repeat/duplicate na aaye.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (INCLUDE / EXCLUDE — har element pe 2 choice, index-based)
//  ★ har element ke saamne 2 raaste: LE lo ya CHHOD do -> 2 recursive call.
//  1. base: index >= size -> ek poora subset ban gaya (temp) -> ans me daalo -> RETURN.
//  2. INCLUDE : temp.push(nums[i]) -> solve(i+1) -> temp.pop() (UN-CHOOSE, backtrack).
//  3. EXCLUDE : solve(i+1) bina push kiye (element chhod diya).
//  ★ temp by-REFERENCE (bahar bana) -> ek hi SHARED path; push/pop usi pe -> backtrack kaam kare.
//     (helper ke andar banate to har call naya temp -> backtrack toot jaata.)
//  ★ 2^n subsets (har element in/out) -> yahi power-set.
//  ★ ALT form (start-loop): har NODE pe temp daalo + for(i=start..n) choose/explore/un-choose. same cheez.
// ============================================================
void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp)
{
    // base case sahi agra indes ums size ke aage ya wah take ek mil gaya
    if (index >= nums.size())
    {
        ans.push_back(temp);
        return;
    }

    // le liye
    temp.push_back(nums[index]);
    solve(nums, ans, index + 1, temp); // age badho
    temp.pop_back(); // and ukos hata diya

    solve(nums, ans, index + 1, temp); // nahi liya
}

vector<vector<int>> subsets(vector<int> &nums)
{
    vector<vector<int>> ans;
    vector<int> temp; // ye baahar hi benga kyu ki agar helper mei bane to har baar new banage jo ki galat hai
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
