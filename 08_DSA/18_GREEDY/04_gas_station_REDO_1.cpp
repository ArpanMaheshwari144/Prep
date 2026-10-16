// ============================================================
// GAS STATION  -- REDO_1                  (LC-134)
// ============================================================
// N gas stations CIRCLE me. gas[i] = station i pe milne wala petrol.
// cost[i] = station i se AGLE station (i+1) tak jaane ka kharcha.
// tank khali se shuru. aisa STARTING index return karo jahaan se poora circle
// (wapas wahin) complete ho jaaye. agar koi start possible nahi -> -1.
// (answer unique hota hai agar exist kare.)
//
//   gas=[1,2,3,4,5], cost=[3,4,5,1,2] -> 3   (index 3 se shuru -> circle complete)
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
    int n = gas.size();
    int index = 0, meriGaddiKaTank = 0, kyeNegativeHua = 0;
    for (int i = 0; i < n; i++)
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

int passed = 0, total = 0;
void check(int got, int exp, const string &label)
{
    total++;
    if (got == exp)
    {
        passed++;
        cout << "PASS " << label << "\n";
    }
    else
        cout << "FAIL " << label << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    vector<int> g1 = {1, 2, 3, 4, 5}, c1 = {3, 4, 5, 1, 2};
    check(canCompleteCircuit(g1, c1), 3, "case1");
    vector<int> g2 = {2, 3, 4}, c2 = {3, 4, 3};
    check(canCompleteCircuit(g2, c2), -1, "case2");
    vector<int> g3 = {5, 1, 2, 3, 4}, c3 = {4, 4, 1, 5, 1};
    check(canCompleteCircuit(g3, c3), 4, "case3");
    vector<int> g4 = {2}, c4 = {2};
    check(canCompleteCircuit(g4, c4), 0, "case4");
    vector<int> g5 = {3, 3, 4}, c5 = {3, 4, 4};
    check(canCompleteCircuit(g5, c5), -1, "case5");
    vector<int> g6 = {1}, c6 = {2};
    check(canCompleteCircuit(g6, c6), -1, "case6 ek station kam petrol");
    vector<int> g7 = {5}, c7 = {3};
    check(canCompleteCircuit(g7, c7), 0, "case7");
    vector<int> g8 = {0, 0, 5}, c8 = {1, 1, 3};
    check(canCompleteCircuit(g8, c8), 2, "case8 total 0, start aakhri");
    vector<int> g9 = {4, 0, 0, 0}, c9 = {1, 1, 1, 1};
    check(canCompleteCircuit(g9, c9), 0, "case9");
    vector<int> g10 = {1, 1, 1, 1}, c10 = {1, 1, 1, 1};
    check(canCompleteCircuit(g10, c10), 0, "case10 tank har jagah 0");
    vector<int> g11 = {3, 1, 1}, c11 = {1, 2, 2};
    check(canCompleteCircuit(g11, c11), 0, "case11");
    vector<int> g12 = {1, 2, 3}, c12 = {2, 2, 2};
    check(canCompleteCircuit(g12, c12), 1, "case12");

    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
