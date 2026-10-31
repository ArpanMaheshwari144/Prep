// ═══════════════════════════════════════════════════════════════════════
//  PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: ★ EDGE line + ek chhota dushman-example chalao
// ═══════════════════════════════════════════════════════════════════════
//  INSERT INTERVAL (LC-57)  -- REDO_2 (khaali page, khud likh)
//  intervals[] pehle se START ke hisaab se SORTED hain aur aapas me overlap NAHI karte.
//  Ek newInterval diya hai. Use sahi jagah daalo; kisi se overlap ho to MERGE kar do.
//  Result bhi sorted + non-overlapping lautao.
//  Note: [1,5] aur [5,7] ko chhoona bhi overlap maana jaata hai (merge -> [1,7]).
//
//  TEST-CASES (intervals, new -> expected):
//   1) [[1,3],[6,9]], [2,5]                        -> [[1,5],[6,9]]
//   2) [[1,2],[3,5],[6,7],[8,10],[12,16]], [4,8]   -> [[1,2],[3,10],[12,16]]
//   3) [], [5,7]                                   -> [[5,7]]          (khaali list)
//   4) [[1,5]], [2,3]                              -> [[1,5]]          (andar hi ghus gaya)
//   5) [[1,5]], [6,8]                              -> [[1,5],[6,8]]    (baad me, bina overlap)
//   6) [[3,5]], [1,2]                              -> [[1,2],[3,5]]    (sabse aage)
//   7) [[1,2],[5,6]], [3,4]                        -> [[1,2],[3,4],[5,6]] (beech me, bina overlap)
//   8) [[1,5]], [5,7]                              -> [[1,7]]          (sirf chhoota)
//   9) [[1,2],[3,4],[5,6]], [0,10]                 -> [[0,10]]         (sabko nigal gaya)
//  10) [[1,5],[10,15]], [5,10]                     -> [[1,15]]         (dono ko jod diya)
//  11) [[2,3],[5,7]], [8,9]                        -> [[2,3],[5,7],[8,9]] (sabse peeche)
//  12) [[1,3],[4,6],[8,10]], [2,9]                 -> [[1,10]]
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> insert(vector<vector<int>> &intervals, vector<int> &newInterval)
{
    // yahan likh
    return {};
}

// ---- test helper (ise mat chhed) ----
void check(vector<vector<int>> in, vector<int> ni, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = insert(in, ni);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=[";
    for (auto &v : got)
        cout << "[" << v[0] << "," << v[1] << "]";
    cout << "]\n";
}

int main()
{
    check({{1, 3}, {6, 9}}, {2, 5}, {{1, 5}, {6, 9}}, 1);
    check({{1, 2}, {3, 5}, {6, 7}, {8, 10}, {12, 16}}, {4, 8}, {{1, 2}, {3, 10}, {12, 16}}, 2);
    check({}, {5, 7}, {{5, 7}}, 3);
    check({{1, 5}}, {2, 3}, {{1, 5}}, 4);
    check({{1, 5}}, {6, 8}, {{1, 5}, {6, 8}}, 5);
    check({{3, 5}}, {1, 2}, {{1, 2}, {3, 5}}, 6);
    check({{1, 2}, {5, 6}}, {3, 4}, {{1, 2}, {3, 4}, {5, 6}}, 7);
    check({{1, 5}}, {5, 7}, {{1, 7}}, 8);
    check({{1, 2}, {3, 4}, {5, 6}}, {0, 10}, {{0, 10}}, 9);
    check({{1, 5}, {10, 15}}, {5, 10}, {{1, 15}}, 10);
    check({{2, 3}, {5, 7}}, {8, 9}, {{2, 3}, {5, 7}, {8, 9}}, 11);
    check({{1, 3}, {4, 6}, {8, 10}}, {2, 9}, {{1, 10}}, 12);
    return 0;
}
