// ============================================================
// DAILY TEMPERATURES — Stack (monotonic stack; next-greater ka cousin)
// ============================================================
// temperatures[] = har din ka temp. Har din ke liye lautao:
//   kitne DIN BAAD pehla WARMER (zyada) din aata hai? aage koi warmer nahi -> 0.
//   temps = [73,74,75,71,69,72,76,73]  ->  [1,1,4,2,1,1,0,0]
//     73 -> agla warmer 74 (1 din baad) -> 1
//     75 -> agla warmer 76 (4 din baad) -> 4
//     76 -> aage koi warmer nahi -> 0
//
// (approach LIKHA NAHI. ye next-greater JAISA (monotonic stack) hai --
//  bas socho: is baar answer me kya bharna hai? warmer ki VALUE... ya kuch aur?
//  copy-pen pe trace karke twist khud pakad.)
//
// Tests (// expected):
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
    vector<int> ans(temps.size(), 0);
    stack<int> st;

    for (int i = 0; i < temps.size(); i++)
    {
        while (!st.empty() && temps[st.top()] < temps[i])
        {
            int topp = st.top();
            st.pop();
            ans[topp] = i - topp;
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
