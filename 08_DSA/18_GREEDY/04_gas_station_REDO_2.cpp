// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// 7. Pattern yaad aaya -> PEHLE COPY-PEN pe chhota example haath se chalao, PHIR code.
// ============================================================
// GAS STATION  -- REDO_2  (LC-134)  (blank, 21-Nov)
// ============================================================
// N gas stations CIRCLE me. gas[i] = station i pe milne wala petrol.
// cost[i] = station i se AGLE station (i+1) tak jaane ka kharcha.
// tank khali se shuru. aisa STARTING index return karo jahaan se poora circle
// (wapas wahin) complete ho jaaye. agar koi start possible nahi -> -1.
// (answer unique hota hai agar exist kare.)
//
// ---- TEST CASES (gas, cost -> expected start index) ----
//   [1,2,3,4,5], [3,4,5,1,2]   -> 3
//   [2,3,4],     [3,4,3]       -> -1
//   [5,1,2,3,4], [4,4,1,5,1]   -> 4
//   [2],         [2]           -> 0
//   [3,3,4],     [3,4,4]       -> -1
//   [1],         [2]           -> -1   (ek station, petrol kam)
//   [5],         [3]           -> 0
//   [0,0,5],     [1,1,3]       -> 2    (total exactly 0, start aakhri pe)
//   [4,0,0,0],   [1,1,1,1]     -> 0
//   [1,1,1,1],   [1,1,1,1]     -> 0    (har jagah tank 0, phir bhi chalega)
//   [3,1,1],     [1,2,2]       -> 0
//   [1,2,3],     [2,2,2]       -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int canCompleteCircuit(vector<int> &gas, vector<int> &cost)
{
    int index = 0, meriGaddiKaTank = 0, kyeNegativeHua = 0;
    for (int i = 0; i < gas.size(); i++)
    {
        meriGaddiKaTank += gas[i] - cost[i];
        kyeNegativeHua += gas[i] - cost[i];

        if (meriGaddiKaTank < 0)
        {
            meriGaddiKaTank = 0;
            index = i + 1;
        }
    }
    return kyeNegativeHua < 0 ? -1 : index;
}

int main()
{
    vector<tuple<vector<int>, vector<int>, int>> tests = {
        {{1, 2, 3, 4, 5}, {3, 4, 5, 1, 2}, 3},
        {{2, 3, 4}, {3, 4, 3}, -1},
        {{5, 1, 2, 3, 4}, {4, 4, 1, 5, 1}, 4},
        {{2}, {2}, 0},
        {{3, 3, 4}, {3, 4, 4}, -1},
        {{1}, {2}, -1},
        {{5}, {3}, 0},
        {{0, 0, 5}, {1, 1, 3}, 2},
        {{4, 0, 0, 0}, {1, 1, 1, 1}, 0},
        {{1, 1, 1, 1}, {1, 1, 1, 1}, 0},
        {{3, 1, 1}, {1, 2, 2}, 0},
        {{1, 2, 3}, {2, 2, 2}, 1},
    };
    int pass = 0;
    for (auto &[g, c, exp] : tests)
    {
        int got = canCompleteCircuit(g, c);
        bool ok = (got == exp);
        if (ok)
            pass++;
        cout << "got " << got << " | exp " << exp << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
    }
    cout << "\n"
         << pass << "/" << tests.size() << " PASS\n";
    return 0;
}
