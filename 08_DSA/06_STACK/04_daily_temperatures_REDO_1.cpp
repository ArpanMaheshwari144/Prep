// ============================================================
// DAILY TEMPERATURES — Stack (monotonic)   [REDO_1 — khud bharo]
// ============================================================
// temperatures[] = har din ka temp. Har din ke liye: kitne DIN BAAD pehla WARMER din?
//   aage koi warmer nahi -> 0.
//   twist: answer = kitne DIN baad (index-diff), value nahi.
//
//   [73,74,75,71,69,72,76,73]  -> 1 1 4 2 1 1 0 0
//   [30,40,50,60]              -> 1 1 1 0
//   [30,60,90]                 -> 1 1 0
//   [90,80,70,60]              -> 0 0 0 0
//   [50]                       -> 0
// ============================================================

#include <iostream>
#include <vector>
#include <stack>
using namespace std;

vector<int> dailyTemperatures(vector<int> &temps)
{
    // TODO: khud likho (monotonic stack, INDEX rakho; pop pe ans = i - topp)
    return vector<int>(temps.size(), 0);
}

int main()
{
    vector<vector<int>> tests = {
        {73, 74, 75, 71, 69, 72, 76, 73},
        {30, 40, 50, 60},
        {30, 60, 90},
        {90, 80, 70, 60},
        {50}};

    auto p = [](vector<int> r)
    { for (int x : r) cout << x << " "; cout << "\n"; };

    for (auto &t : tests)
        p(dailyTemperatures(t));
    // expected:
    // 1 1 4 2 1 1 0 0
    // 1 1 1 0
    // 1 1 0
    // 0 0 0 0
    // 0
    return 0;
}
