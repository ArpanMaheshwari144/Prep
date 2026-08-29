// ============================================================
// SUBARRAYS WITH K DIFFERENT INTEGERS   (LC-992)  -- REDO_1 (blank)
// ============================================================
// nums[] + k. Ginno kitne SUBARRAYS (contiguous) me EXACTLY k DIFFERENT numbers ho.
//   [1,2,1,2,3], k=2  -> 7
//
// >>> HINT NAHI. khud yaad: exactly(k) = atMost(k) - atMost(k-1).
//     atMost(K) = variable-SW, distinct<=K, har right pe += (right-left+1), ans=0 (COUNT). <<<
//
// ---- TEST (nums, k -> expected) ----
//   [1,2,1,2,3], k=2  -> 7
//   [1,2,1,3,4], k=3  -> 3
//   [1,1,1,1],   k=1  -> 10
//   [2,1,2,1,2], k=2  -> 10
//   [1,2,3,4,5], k=1  -> 5
//   [1],         k=1  -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int atMostK(vector<int> &fruits, int k)
{
    int i = 0, j = 0, count = 0;
    unordered_map<int, int> mp;
    while (j < fruits.size())
    {
        mp[fruits[j]]++;
        while (mp.size() > k)
        {
            mp[fruits[i]]--;
            if (mp[fruits[i]] == 0)
            {
                mp.erase(fruits[i]);
            }
            i++;
        }
        count += j - i + 1;
        j++;
    }
    return count;
}

int subarraysWithKDistinct(vector<int> &nums, int k)
{
    return atMostK(nums, k) - atMostK(nums, k-1);
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {1, 2, 1, 2, 3};
    check(subarraysWithKDistinct(a1, 2), 7, "case1");
    vector<int> a2 = {1, 2, 1, 3, 4};
    check(subarraysWithKDistinct(a2, 3), 3, "case2");
    vector<int> a3 = {1, 1, 1, 1};
    check(subarraysWithKDistinct(a3, 1), 10, "case3");
    vector<int> a4 = {2, 1, 2, 1, 2};
    check(subarraysWithKDistinct(a4, 2), 10, "case4");
    vector<int> a5 = {1, 2, 3, 4, 5};
    check(subarraysWithKDistinct(a5, 1), 5, "case5");
    vector<int> a6 = {1};
    check(subarraysWithKDistinct(a6, 1), 1, "case6");
    return 0;
}
