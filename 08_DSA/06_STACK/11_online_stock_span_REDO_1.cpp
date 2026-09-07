// ============================================================
// ONLINE STOCK SPAN — (LeetCode 901, Medium)   [STACK #11 · monotonic stack, STREAMING]  — REDO_1
// ============================================================
// STREAMING: prices ek-ek karke aate (next(price) call). Har call pe us din ka SPAN lauta.
// SPAN = aaj ko milaakar peeche kitne LAGATAAR din jinme price aaj ke price se <= thi
//        (jaise hi koi din aaj se BADA mile, wahin ruk).
//
//   next(100) -> 1 · next(80) -> 1 · next(60) -> 1 · next(70) -> 2 · next(60) -> 1
//   next(75)  -> 4 · next(85) -> 6
//
// INPUT-format (tests): prices[] = jis order me next() call honge; expected[] = har call ka span.
// ============================================================
#include <bits/stdc++.h>
using namespace std;

class StockSpanner
{
public:
    stack<pair<int, int>> st; // {price, span}
    StockSpanner() {}

    int next(int price)
    {
        int span = 1;
        while (!st.empty() && st.top().first <= price)
        {
            span += st.top().second;
            st.pop();
        }
        st.push({price, span});
        return span;
    }
};

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> prices, vector<int> expected, int t)
{
    StockSpanner sp;
    vector<int> got;
    for (int p : prices)
        got.push_back(sp.next(p));
    cout << "T" << t << ": " << (got == expected ? "PASS" : "FAIL") << "  got: ";
    for (int x : got)
        cout << x << " ";
    cout << " (exp: ";
    for (int x : expected)
        cout << x << " ";
    cout << ")\n";
}

int main()
{
    check({100, 80, 60, 70, 60, 75, 85}, {1, 1, 1, 2, 1, 4, 6}, 1);
    check({31, 41, 48, 59, 79}, {1, 2, 3, 4, 5}, 2);
    check({80, 70, 60, 50}, {1, 1, 1, 1}, 3);
    check({10}, {1}, 4);
    check({5, 5, 5}, {1, 2, 3}, 5);
    return 0;
}
