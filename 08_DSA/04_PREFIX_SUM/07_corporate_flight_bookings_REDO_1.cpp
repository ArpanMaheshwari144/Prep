// ============================================================
// CORPORATE FLIGHT BOOKINGS — (LeetCode 1109)  [REDO_1]
// ============================================================
// `n` flights hain, number 1 se n tak.
//
// `bookings` di hai. Har booking = [first, last, seats] ka matlab:
//      flight `first` se flight `last` TAK (dono shaamil) ki HAR flight me
//      `seats` seat book ho gayi.
//
// Ek array lautao jiska i-th element = flight (i+1) pe kul kitni seat book hui.
//
//   n = 5,  bookings = [[1,2,10],[2,3,20],[2,5,25]]
//        -> [10, 55, 45, 25, 25]
//
//   n = 2,  bookings = [[1,2,10],[2,2,15]]
//        -> [10, 25]
//
// ------------------------------------------------------------
// bookings.length  aur  n  dono 20,000 tak ja sakte hain.
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

vector<int> corpFlightBookings(vector<vector<int>> &bookings, int n)
{
    vector<int> diff(n + 2);
    for (auto &it : bookings)
    {
        int start = it[0];
        int end = it[1];
        int count = it[2];

        diff[start] += count;
        diff[end + 1] -= count;
    }

    vector<int> ans;
    int cummSum = 0;
    for (int i = 1; i <= n; i++)
    {
        cummSum += diff[i];
        ans.push_back(cummSum);
    }
    return ans;
}

// ─── TESTS (haath mat lagana) ──────────────────────────
int pass_count = 0, total = 0;

void check(vector<vector<int>> bookings, int n, vector<int> exp, int t)
{
    vector<int> got = corpFlightBookings(bookings, n);
    bool ok = (got == exp);
    pass_count += ok;
    total++;

    cout << "T" << t << ": " << (ok ? "PASS" : "FAIL") << "  got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "]  exp=[";
    for (int i = 0; i < (int)exp.size(); i++)
        cout << exp[i] << (i + 1 < (int)exp.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    check({{1, 2, 10}, {2, 3, 20}, {2, 5, 25}}, 5, {10, 55, 45, 25, 25}, 1);
    check({{1, 2, 10}, {2, 2, 15}}, 2, {10, 25}, 2);
    check({{1, 1, 100}}, 1, {100}, 3);
    check({{1, 3, 5}}, 3, {5, 5, 5}, 4);
    check({{3, 3, 7}}, 5, {0, 0, 7, 0, 0}, 5);
    check({{1, 5, 1}, {1, 5, 2}, {1, 5, 3}}, 5, {6, 6, 6, 6, 6}, 6);
    check({{1, 2, 10}, {4, 5, 20}}, 5, {10, 10, 0, 20, 20}, 7);
    check({}, 3, {0, 0, 0}, 8);
    check({{2, 4, 5}, {1, 1, 9}, {4, 4, 1}}, 4, {9, 5, 5, 6}, 9);
    check({{1, 4, 3}, {2, 3, 7}}, 4, {3, 10, 10, 3}, 10);

    cout << "\n"
         << pass_count << "/" << total << " passed\n";
    return 0;
}
