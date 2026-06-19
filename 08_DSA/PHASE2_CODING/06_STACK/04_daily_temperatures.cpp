// ============================================================
// DAILY TEMPERATURES   —   Stack (monotonic)
// ============================================================
// Har din: kitne din baad WARMER temperature? Agar aage warmer nahi -> 0.
//   [73,74,75,71,69,72,76,73]  ->  [1,1,4,2,1,1,0,0]
//
// SIGNAL: "har element ke liye agla BADA (kitni door)" -> monotonic stack.
//   (Structure TU banayega — apni thinking se, blank se. No pseudo-code.
//    Soch: stack mein kya rakhoge? kab pop? distance kaise? "warmer" = STRICTLY bada.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> dailyTemperatures(vector<int> &temperatures)
{
    int n = temperatures.size();
    stack<int> st;
    vector<int> ans(n);

    for (int i = 0; i < temperatures.size(); i++)
    {
        while (!st.empty() && temperatures[i] > temperatures[st.top()])
        {
            int idx = st.top();
            st.pop();
            ans[idx] = i - idx;
        }
        st.push(i);
    }
    return ans;
}

int main()
{
    vector<int> t1 = {73, 74, 75, 71, 69, 72, 76, 73};
    for (int x : dailyTemperatures(t1))
        cout << x << " "; // 1 1 4 2 1 1 0 0
    cout << endl;

    vector<int> t2 = {73, 73, 74};
    for (int x : dailyTemperatures(t2))
        cout << x << " "; // 2 1 0  (equal != warmer)
    cout << endl;

    vector<int> t3 = {30, 60, 90};
    for (int x : dailyTemperatures(t3))
        cout << x << " "; // 1 1 0
    cout << endl;
    return 0;
}
