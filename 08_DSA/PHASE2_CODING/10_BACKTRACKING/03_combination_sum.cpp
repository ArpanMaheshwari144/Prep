// ============================================================
// COMBINATION SUM   —   Backtracking  [rep 3]   (JP ka actual OA Q)
// ============================================================
// candidates (distinct) + target. Woh combinations jinka SUM == target.
// SAME number BAAR-BAAR use kar sakte (reuse allowed).
//
//   [2,3,6,7], target 7 -> [2,2,3], [7]          (2+2+3=7, 7=7)
//   [2,3,5],   target 8 -> [2,2,2,2],[2,3,3],[3,5]
//   [2],       target 1 -> (kuch nahi -- 1 ban hi nahi sakta)
//
// SIGNAL: "sum == target ke saare combos, reuse allowed" -> backtracking + 3 twist.
//   Twist (jo samjha):
//     1. REUSE -> recurse mein index AAGE mat badhao: recurse(target-cand[i], i)  [i NOT i+1]
//     2. TARGET base -> target==0 -> mila (push) ; target<0 -> STOP (prune)
//     3. START index -> loop hamesha `start` se (peeche mat jaao) -> duplicate combos rok
//   Template: base(==0 push / <0 return) -> loop i from start -> push -> recurse(target-cand[i], i) -> pop.
//   No code -- helper bana (candidates, target, start, current&, ans&).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- TERA kaam: yeh function bhar (backtracking + start + reuse) ----
void solve(vector<int> candidates, int target, int idx, vector<int> &temp, vector<vector<int>> &ans)
{
    if (target == 0)
    {
        ans.push_back(temp);
        return;
    }
    if (target < 0)
    {
        return;
    }

    for (int i = idx; i < candidates.size(); i++)
    {
        temp.push_back(candidates[i]);
        solve(candidates, target - candidates[i], i, temp, ans);
        temp.pop_back();
    }
}

vector<vector<int>> combinationSum(vector<int> candidates, int target)
{
    vector<vector<int>> ans;
    vector<int> temp;
    solve(candidates, target, 0, temp, ans);
    return ans;
}

// ---- helper (mat chhu) — count + print ----
void printAll(vector<vector<int>> res)
{
    cout << "count=" << res.size() << " : ";
    for (auto &c : res)
    {
        cout << "[";
        for (int i = 0; i < c.size(); i++)
        {
            cout << c[i];
            if (i + 1 < c.size())
                cout << ",";
        }
        cout << "]";
    }
    cout << endl;
}

int main()
{
    printAll(combinationSum({2, 3, 6, 7}, 7)); // expected count=2 : [2,2,3][7] (order alag ho sakta)
    printAll(combinationSum({2, 3, 5}, 8));    // expected count=3 : [2,2,2,2][2,3,3][3,5]
    printAll(combinationSum({7}, 7));          // expected count=1 : [7]
    printAll(combinationSum({2}, 1));          // expected count=0 :   (1 ban nahi sakta)
    printAll(combinationSum({3, 5, 8}, 11));   // expected count=2 : [3,3,5][3,8]
    return 0;
}
