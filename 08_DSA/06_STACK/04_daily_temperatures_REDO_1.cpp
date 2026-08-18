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
    int n = temps.size();
    vector<int> ans(n, 0);
    stack<int> st;
    for (int i = 0; i < n; i++)
    {
        while (!st.empty() && temps[st.top()] < temps[i])
        {
            int tp = st.top();
            st.pop();
            ans[tp] = i - tp;
        }
        st.push(i);
    }
    return ans;
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
