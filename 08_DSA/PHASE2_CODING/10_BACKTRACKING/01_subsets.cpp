// ============================================================
// SUBSETS  (power set)   —   Backtracking  [pehla backtracking problem]
// ============================================================
// nums diya. SAARE possible subsets nikaalo (har element: LO ya CHHODO).
//
//   [1,2,3] -> [], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]   (2^3 = 8 subsets)
//   [1]     -> [], [1]
//   []      -> [[]]   (sirf khaali subset)
//
// SIGNAL: "saare subsets/combinations" -> BACKTRACKING (choices ka ped).
//   Template (jo samjha):
//     CHOOSE   -> current element LO (current list me push)
//     EXPLORE  -> aage recurse (agle index pe)
//     UN-CHOOSE-> wapas aake us element ko HATAO (pop_back) -> "chhodo" wala raasta
//   har element pe 2 raaste: lo / chhodo. index end pe pahunche -> current ko answer me daalo.
//   No code -- helper bana (index, current-list, ans). aaram se.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- TERA kaam: yeh function bhar (helper backtracking) ----
void solve(int idx, vector<int> nums, vector<vector<int>> &ans, vector<int> &temp)
{
    if (idx == nums.size())
    {
        ans.push_back(temp);
        return;
    }

    // liya
    temp.push_back(nums[idx]);
    solve(idx + 1, nums, ans, temp);

    // backtrcak
    temp.pop_back();

    // nahi liya
    solve(idx + 1, nums, ans, temp);
}
vector<vector<int>> subsets(vector<int> nums)
{
    vector<vector<int>> ans;
    vector<int> temp;
    solve(0, nums, ans, temp);
    return ans;
}

// ---- helper (mat chhu) — count + print ----
void printAll(vector<vector<int>> res)
{
    cout << "count=" << res.size() << " : ";
    for (auto &sub : res)
    {
        cout << "[";
        for (int i = 0; i < sub.size(); i++)
        {
            cout << sub[i];
            if (i + 1 < sub.size())
                cout << ",";
        }
        cout << "]";
    }
    cout << endl;
}

int main()
{
    printAll(subsets({1, 2, 3})); // expected count=8 : [],[1],[2],[3],[1,2],[1,3],[2,3],[1,2,3] (order alag ho sakta)
    printAll(subsets({1, 2}));    // expected count=4 : [],[1],[2],[1,2]
    printAll(subsets({1}));       // expected count=2 : [],[1]
    printAll(subsets({}));        // expected count=1 : []   (sirf khaali subset)
    return 0;
}
