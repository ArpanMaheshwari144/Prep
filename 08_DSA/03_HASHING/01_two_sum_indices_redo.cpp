// ============================================================
// TWO SUM — return INDICES   (pehle BRUTE, phir OPTIMIZE)
// ============================================================
// Array A aur ek target diya. Aise DO index (i, j) return karo
// jahan A[i] + A[j] == target  (i != j).
// Maan lo hamesha exactly ek jodi milegi. Chhota index pehle.
//
// >>> STEP 1: BRUTE likh (do loop). STEP 2: optimize karenge. <<<
//   brute recipe: NOUN = "pair" -> saare pair (i, j>i) -> A[i]+A[j]==target? -> {i, j} return.
//
// ---- TEST CASES ----
//   A=[2,7,11,15], t=9  -> [0,1]     (2+7)
//   A=[3,2,4],     t=6  -> [1,2]     (2+4)
//   A=[3,3],       t=6  -> [0,1]
//   A=[1,5,3],     t=8  -> [1,2]     (5+3)
//   A=[0,4,3,0],   t=0  -> [0,3]     (0+0)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> twoSum(vector<int> &A, int target)
{
    unordered_map<int, int> mp;
    for (int i = 0; i < A.size(); i++)
    {
        if (mp.find(target - A[i]) != mp.end())
        {
            return {mp[target - A[i]], i};
        }
        mp[A[i]] = i;
    }
    return {-1, -1};
}

// ---- test helper (ise mat chhed) ----
void check(vector<int> got, vector<int> exp, const string &label)
{
    bool ok = (got == exp);
    cout << label << " -> got [" << (got.size() == 2 ? to_string(got[0]) + "," + to_string(got[1]) : "?")
         << "] | exp [" << exp[0] << "," << exp[1] << "]"
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {2, 7, 11, 15};
    check(twoSum(a1, 9), {0, 1}, "case1");
    vector<int> a2 = {3, 2, 4};
    check(twoSum(a2, 6), {1, 2}, "case2");
    vector<int> a3 = {3, 3};
    check(twoSum(a3, 6), {0, 1}, "case3");
    vector<int> a4 = {1, 5, 3};
    check(twoSum(a4, 8), {1, 2}, "case4");
    vector<int> a5 = {0, 4, 3, 0};
    check(twoSum(a5, 0), {0, 3}, "case5");
    return 0;
}
