// ═══════════════════════════════════════════════════════════════════════
//  FRUIT INTO BASKETS  (LC 904)  — REDO
//  fruits[] = ek line me ped, har number ek fruit-TYPE. tere paas 2 basket,
//  har basket me SIRF EK type. kisi ped se shuru karke lagataar (continuous)
//  aage ke ped se ek-ek fruit uthata. SABSE ZYADA kitne fruit utha sakta? -> LENGTH.
//
//  INPUT-FORM: 1 <= fruits.size(), fruits[i] >= 0.
//  TEST-CASES (fruits -> expected):
//   1) [1,2,1]                        -> 3
//   2) [0,1,2,2]                      -> 3
//   3) [1,2,3,2,2]                    -> 4
//   4) [3,3,3,1,2,1,1,2,3,3,4]        -> 5
//   5) [1]                            -> 1
//   6) [1,1,1,1]                      -> 4
//   7) [1,2]                          -> 2
//   8) [1,2,3]                        -> 2
//   9) [1,2,3,4,5]                    -> 2
//  10) [0,0,1,1]                      -> 4
//  11) [1,0,1,4,1,4,1,2,3]            -> 5
//  12) [4,1,1,1,3,1,7,5]              -> 5
//  13) [2,1,2,1,2,1,3,3,3,3]          -> 6
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int totalFruit(vector<int> &fruits)
{
    unordered_map<int, int> mp;
    int i = 0, j = 0, len = INT_MIN;
    while (j < fruits.size())
    {
        mp[fruits[j]]++;
        // while : window hamesha sahi (2 se zyada type kabhi nahi).
        // if bhi chalta (sirf LONGEST me): ek aaya ek gaya -> size wahi, window bas khiskti, len galat nahi badhta.
        //   [1,2,1,3,4]: j=3 -> [2,1,3] (3 type, galat) par size 3 = best 3 -> jawab phir bhi 3.
        // COUNT (count += j-i+1) ya SABSE CHHOTI wale me sirf while -> wahan har step window sahi chahiye.
        while (mp.size() > 2)
        {
            mp[fruits[i]]--;
            if (mp[fruits[i]] == 0)
            {
                mp.erase(fruits[i]);
            }
            i++;
        }
        len = max(len, j - i + 1);
        j++;
    }
    return len;
}

// ---- test helper ----
void check(vector<int> fruits, int exp, string name)
{
    int got = totalFruit(fruits);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, 2, 1}, 3, "t1");
    check({0, 1, 2, 2}, 3, "t2");
    check({1, 2, 3, 2, 2}, 4, "t3");
    check({3, 3, 3, 1, 2, 1, 1, 2, 3, 3, 4}, 5, "t4 lamba");
    check({1}, 1, "t5 single");
    check({1, 1, 1, 1}, 4, "t6 ek hi type");
    check({1, 2}, 2, "t7");
    check({1, 2, 3}, 2, "t8");
    check({1, 2, 3, 4, 5}, 2, "t9 sab alag");
    check({0, 0, 1, 1}, 4, "t10 poora array");
    check({1, 0, 1, 4, 1, 4, 1, 2, 3}, 5, "t11");
    check({4, 1, 1, 1, 3, 1, 7, 5}, 5, "t12");
    check({2, 1, 2, 1, 2, 1, 3, 3, 3, 3}, 6, "t13");
    return 0;
}
