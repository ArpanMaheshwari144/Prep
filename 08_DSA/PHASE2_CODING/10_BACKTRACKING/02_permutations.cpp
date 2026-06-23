// ============================================================
// PERMUTATIONS   —   Backtracking  [rep 2]
// ============================================================
// nums diya. SAARE elements use karke har possible ORDER nikaalo.
//
//   [1,2,3] -> [1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]   (3! = 6)
//   [1,2]   -> [1,2],[2,1]   (2)
//   [1]     -> [1]           (1)
//
// SIGNAL: "saare arrangements / order" -> BACKTRACKING + used[] track.
//   Twist (subsets se alag): saare element use, alag order -> "kaunse use ho chuke" track karo.
//   Template (jo samjha):
//     base: current.size() == nums.size() -> current ko ans me daalo (poora permutation)
//     loop i over saare elements:
//        used[i] ? -> skip
//        CHOOSE   -> used[i]=true, current.push(nums[i])
//        EXPLORE  -> recurse
//        UN-CHOOSE-> current.pop(), used[i]=false   (DONO undo)
//   No code -- helper bana (nums, current&, used&, ans&). pop order yaad rakh.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- TERA kaam: yeh function bhar (backtracking + used[]) ----
void solve(vector<int> nums, vector<bool> &used, vector<int> &temp, vector<vector<int>> &ans)
{
    if (temp.size() == nums.size())
    {
        ans.push_back(temp);
        return;
    }

    for (int i = 0; i < nums.size(); i++)
    {
        if (used[i] == false)
        {
            used[i] = true;
            temp.push_back(nums[i]);

            solve(nums, used, temp, ans);

            used[i] = false;
            temp.pop_back();
        }
    }
}
vector<vector<int>> permute(vector<int> nums)
{
    vector<vector<int>> ans;
    vector<int> temp;
    vector<bool> used(nums.size(), false);
    solve(nums, used, temp, ans);
    return ans;
}

// ---- helper (mat chhu) — count + print ----
void printAll(vector<vector<int>> res)
{
    cout << "count=" << res.size() << " : ";
    for (auto &p : res)
    {
        cout << "[";
        for (int i = 0; i < p.size(); i++)
        {
            cout << p[i];
            if (i + 1 < p.size())
                cout << ",";
        }
        cout << "]";
    }
    cout << endl;
}

int main()
{
    printAll(permute({1, 2, 3})); // expected count=6 : [1,2,3][1,3,2][2,1,3][2,3,1][3,1,2][3,2,1] (order alag ho sakta)
    printAll(permute({1, 2}));    // expected count=2 : [1,2][2,1]
    printAll(permute({1}));       // expected count=1 : [1]
    return 0;
}
