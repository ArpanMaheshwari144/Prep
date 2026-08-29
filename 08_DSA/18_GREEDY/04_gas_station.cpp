// ============================================================
// GAS STATION                             (LC-134 — GREEDY running-tank + reset-start)
// ============================================================
// N gas stations CIRCLE me. gas[i] = station i pe milne wala petrol.
// cost[i] = station i se AGLE station (i+1) tak jaane ka kharcha.
// tank khali se shuru. aisa STARTING index return karo jahaan se poora circle
// (wapas wahin) complete ho jaaye. agar koi start possible nahi -> -1.
// (answer unique hota hai agar exist kare.)
//
//   gas=[1,2,3,4,5], cost=[3,4,5,1,2] -> 3   (index 3 se shuru -> circle complete)
//
// >>> GREEDY: total(gas)<total(cost) -> -1. warna running-tank; negative pe start=i+1, tank=0. <<<
//
// ---- TEST CASES (gas, cost -> expected start index) ----
//   [1,2,3,4,5], [3,4,5,1,2]   -> 3
//   [2,3,4],     [3,4,3]       -> -1
//   [5,1,2,3,4], [4,4,1,5,1]   -> 4
//   [2],         [2]           -> 0
//   [3,3,4],     [3,4,4]       -> -1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// >>> APPROACH (Arpan — greedy: 2 accumulator, 1 pass, 5/5) <<<
//   har station ka NET = gas[i] - cost[i]. Do jagah jodo:
//     1. kyaNegativeHua (KABHI reset nahi) = poore circle ka total net.
//          agar ant me total < 0 -> jitna petrol hi nahi jitna chahiye -> koi start possible NAHI -> -1.
//     2. meriGaddiKaTank (running tank) = candidate start se ab tak ka bacha petrol.
//          jaise hi negative hua -> is start se ye station tak nahi pahucha ja sakta ->
//          tank = 0 reset, index = i+1 (agle station se naya try).
//   ★ KEY insight: agar total >= 0, to jo LAST reset-point bacha wahi ANSWER hai (unique).
//     kyun: jis station pe tank doobta hai, uske aur pehle-ke kisi bhi start se pahucha ja HI nahi sakta ->
//     start hamesha uske AAGE hi ho sakta -> index i+1 pe khiskta rehta. O(n), single pass.
int canCompleteCircuit(vector<int> &gas, vector<int> &cost)
{
    int index = 0;
    int meriGaddiKaTank = 0;
    int kyaNegativeHua = 0;
    for (int i = 0; i < gas.size(); i++)
    {
        meriGaddiKaTank += gas[i] - cost[i];
        kyaNegativeHua += gas[i] - cost[i];
        if (meriGaddiKaTank < 0)
        {
            meriGaddiKaTank = 0;
            index = i + 1;
        }
    }
    return kyaNegativeHua < 0 ? -1 : index;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
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
    return 0;
}
