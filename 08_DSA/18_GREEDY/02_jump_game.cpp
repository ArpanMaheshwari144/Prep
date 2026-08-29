// ============================================================
// JUMP GAME                               (LC-55 — GREEDY reachability)
// ============================================================
// nums[] diya hai. har nums[i] = us index se MAX kitni door kood sakte ho
// (0..nums[i] steps aage). tum index 0 pe khade ho. kya LAST index tak
// pahunch sakte ho? true/false return karo.
//
//   [2,3,1,1,4] -> true    (0->1->4, ya 0->2->3->4)
//
// >>> GREEDY: ek var "maxReach" (ab tak sabse door). i > maxReach -> false. <<<
//
// ---- TEST CASES (nums -> expected) ----
//   [2,3,1,1,4]   -> true
//   [3,2,1,0,4]   -> false   (index 3 pe value 0 -> aage nahi, 4 tak nahi pahunch sakte)
//   [0]           -> true    (pehle se hi last index pe)
//   [2,0,0]       -> true    (index0 se 2 kood -> last)
//   [1,0,1,0]     -> false   (index1 pe 0 -> atak gaye)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// >>> APPROACH (greedy: farthest-reach) <<<
//   maxReach = ab tak sabse DOOR pahunch-yogya index. loop i:
//     agar i > maxReach -> yahan tak pahunch hi nahi sakte -> return false.
//     warna maxReach = max(maxReach, i + nums[i]).
//   end: maxReach >= last-index -> true. O(n) single pass, no DP zaroorat.
bool canJump(vector<int> &nums)
{
    //   [2,3,1,1,4]   -> true
    int maxReach = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        if (i > maxReach)
            return false;

        // i + nums[i] KYUN jodte hain:
        //   nums[i] = "kitne STEP aage kood sakte" (relative distance), i = abhi ka index.
        //   dono JODO -> ABSOLUTE index milta jahan tak pahunch sakte.
        //   eg [2,3,1,1,4]: index1 pe value 3 -> 1 + nums[1] = 1+3 = 4 -> index 4 tak pahunch gaye.
        maxReach = max(maxReach, i + nums[i]);
    }
    return maxReach >= nums.size() - 1;
}

// ---- test helper (ise mat chhed) ----
void check(bool got, bool exp, const string &label)
{
    cout << label << " -> got " << (got ? "true" : "false")
         << " | exp " << (exp ? "true" : "false")
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {2, 3, 1, 1, 4};
    check(canJump(a1), true, "case1");

    vector<int> a2 = {3, 2, 1, 0, 4};
    check(canJump(a2), false, "case2");

    vector<int> a3 = {0};
    check(canJump(a3), true, "case3");

    vector<int> a4 = {2, 0, 0};
    check(canJump(a4), true, "case4");

    vector<int> a5 = {1, 0, 1, 0};
    check(canJump(a5), false, "case5");
    return 0;
}
