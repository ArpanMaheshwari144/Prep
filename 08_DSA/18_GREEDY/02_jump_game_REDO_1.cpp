// ═══════════════════════════════════════════════════════════════════════
//  PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: ★ EDGE line + ek chhota dushman-example chalao
// ═══════════════════════════════════════════════════════════════════════
//  JUMP GAME (LC-55)  -- REDO_1 (khaali page, khud likh)
//  nums[] diya. nums[i] = us index se MAX kitne step aage kood sakte ho (0..nums[i]).
//  Tum index 0 pe khade ho. LAST index tak pahunch sakte ho? true / false lautao.
//
//  TEST-CASES (nums -> expected):
//   1) [2,3,1,1,4]     -> true   (0->1->4)
//   2) [3,2,1,0,4]     -> false  (index 3 pe 0, aage nahi)
//   3) [0]             -> true   (pehle se last pe)
//   4) [2,0,0]         -> true
//   5) [1,0,1,0]       -> false
//   6) [0,1]           -> false  (pehle hi kadam pe 0)
//   7) [1,1,1,1]       -> true
//   8) [2,5,0,0]       -> true   (0->1, wahan se door tak)
//   9) [1,2,0,1]       -> true
//  10) [5,0,0,0,0,0]   -> true   (ek hi kood me last)
//  11) [4,0,0,0,0,1]   -> false  (4 tak hi, last 5 hai)
//  12) [2,0,1,0,1]     -> false
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

bool canJump(vector<int> &nums)
{
    int maxReach = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        if (i > maxReach)
        {
            return false;
        }
        maxReach = max(maxReach, i + nums[i]);
    }
    return maxReach >= nums.size() - 1;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int passed = 0, total = 0;
void check(vector<int> in, bool exp, int t)
{
    total++;
    bool got = canJump(in);
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << "T" << t << ": " << (ok ? "PASS" : "FAIL") << "  got=" << (got ? "true" : "false")
         << " exp=" << (exp ? "true" : "false") << "\n";
}

int main()
{
    check({2, 3, 1, 1, 4}, true, 1);
    check({3, 2, 1, 0, 4}, false, 2);
    check({0}, true, 3);
    check({2, 0, 0}, true, 4);
    check({1, 0, 1, 0}, false, 5);
    check({0, 1}, false, 6);
    check({1, 1, 1, 1}, true, 7);
    check({2, 5, 0, 0}, true, 8);
    check({1, 2, 0, 1}, true, 9);
    check({5, 0, 0, 0, 0, 0}, true, 10);
    check({4, 0, 0, 0, 0, 1}, false, 11);
    check({2, 0, 1, 0, 1}, false, 12);
    cout << "\n"
         << passed << "/" << total << " PASS\n";
    return 0;
}
