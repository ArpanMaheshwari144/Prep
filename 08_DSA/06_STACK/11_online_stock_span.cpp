// ============================================================
// ONLINE STOCK SPAN — (LeetCode 901, Medium)   [STACK #11 · monotonic stack, STREAMING]
// ============================================================
// STREAMING: prices ek-ek karke aate hain (next(price) call). Har call pe us din ka SPAN lauta.
// SPAN = aaj ko milaakar peeche kitne LAGATAAR din the jinme price aaj ke price se <= thi
//        (jaise hi koi din aaj se BADA mile, wahin ruk jao).
//
//   next(100) -> 1   (peeche kuch nahi, khud = 1)
//   next(80)  -> 1   (80 < 100 -> ruk, sirf khud)
//   next(60)  -> 1   (60 < 80)
//   next(70)  -> 2   (70 >= 60 -> 60 bhi ginlo, phir 80>70 ruk -> 70,60 = 2)
//   next(60)  -> 1   (60 < 70)
//   next(75)  -> 4   (75 >= 60,70,60 sab -> phir 80>75 ruk -> 75,60,70,60 = 4)
//   next(85)  -> 6   (85 >= 75,60,70,60,80 -> phir 100>85 ruk -> 6)
//
// SAAR: har din ka answer = "peeche consecutive <= today ka count". Brute = har call pe peeche loop
//   (O(n) per call). Redundancy: chhote din baar-baar dekhne padte -> STACK me (price, span) rakho,
//   jab tak stack-top ka price <= aaj -> pop karke uska span JODO. Monotonic-decreasing stack.
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
    check({31, 41, 48, 59, 79}, {1, 2, 3, 4, 5}, 2); // sab badhte -> 1,2,3,4,5
    check({80, 70, 60, 50}, {1, 1, 1, 1}, 3);        // sab ghatte -> sab 1
    check({10}, {1}, 4);
    check({5, 5, 5}, {1, 2, 3}, 5); // barabar bhi <= -> ginlo
    return 0;
}
