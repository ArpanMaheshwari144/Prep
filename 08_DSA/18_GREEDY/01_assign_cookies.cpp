// ============================================================
// ASSIGN COOKIES                          (LC-455 — GREEDY intro)
// ============================================================
// g[] = har bachhe ka GREED factor (kitni badi cookie chahiye khush hone ko)
// s[] = available cookies ke SIZE.
// har bachhe ko max EK cookie de sakte ho. cookie j bachhe i ko khush karti
// hai agar s[j] >= g[i]. MAX kitne bachhe khush kar sakte ho? count return karo.
//
//   g=[1,2,3], s=[1,1] -> 1   (do size-1 cookie, sirf greed-1 wala bachha khush)
//
// ---- TEST CASES (g, s -> expected) ----
//   [1,2,3], [1,1]         -> 1
//   [1,2],   [1,2,3]       -> 2
//   [1,2,3], [3]           -> 1     (cookie 3 -> greed-1 wala khush)
//   [10,9,8,7], [5,6,7,8]  -> 2     (7->7, 8->8)
//   [1],     []            -> 0     (koi cookie nahi)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int findContentChildren(vector<int> &g, vector<int> &s)
{
    int n = g.size(), m = s.size(), count = 0, i = 0, j = 0;

    if (s.size() == 0)
        return 0;

    sort(begin(g), end(g));
    sort(begin(s), end(s));

    //   [10,9,8,7], [5,6,7,8]  -> 2     (7->7, 8->8)
    while (i < n && j < m)
    {
        if (s[j] >= g[i])
        {
            i++;
            j++;
            count++;
        }
        else
        {
            j++;
        }
    }
    return count;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> g1 = {1, 2, 3}, s1 = {1, 1};
    check(findContentChildren(g1, s1), 1, "case1");

    vector<int> g2 = {1, 2}, s2 = {1, 2, 3};
    check(findContentChildren(g2, s2), 2, "case2");

    vector<int> g3 = {1, 2, 3}, s3 = {3};
    check(findContentChildren(g3, s3), 1, "case3");

    vector<int> g4 = {10, 9, 8, 7}, s4 = {5, 6, 7, 8};
    check(findContentChildren(g4, s4), 2, "case4");

    vector<int> g5 = {1}, s5 = {};
    check(findContentChildren(g5, s5), 0, "case5");
    return 0;
}
