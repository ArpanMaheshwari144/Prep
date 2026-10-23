// ============================================================
// CAPACITY TO SHIP PACKAGES WITHIN D DAYS — (LeetCode 1011, Medium)   [BINARY SEARCH #08]  — REDO_1
// ============================================================
// weights[] = packages, ORDER me ship karne hain (aage-peeche nahi kar sakte).
// D din diye. har din kuch packages ship (order me), par ek din ka total weight
// ship-capacity se ZYADA nahi. MINIMUM capacity lauta jisse saare packages D din me ship ho jaayein.
//   [1,2,3,4,5,6,7,8,9,10], D=5  -> 15
//   [3,2,2,4,1,4], D=3           -> 6
//   [1,2,3,1,1], D=4             -> 3
// ============================================================
#include <bits/stdc++.h>
using namespace std;

bool solve(vector<int> &weights, int D, int mid)
{
    int days = 1;
    int sum = 0;
    for (auto &w : weights)
    {
        if (sum + w > mid)
        {
            days++;
            sum = 0;
        }
        sum += w;
    }
    return days <= D;
}

int shipWithinDays(vector<int> &weights, int days)
{
    int low = *max_element(begin(weights), end(weights));
    int high = accumulate(begin(weights), end(weights), 0);
    int ans = -1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (solve(weights, days, mid))
        {
            ans = mid;
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int passed = 0, total = 0;
void check(vector<int> w, int d, int expected, int t)
{
    total++;
    int got = shipWithinDays(w, d);
    bool ok = (got == expected);
    if (ok)
        passed++;
    cout << "Test " << t << ": " << (ok ? "PASS" : "FAIL") << "  got " << got << "  expected " << expected << "\n";
}

int main()
{
    check({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5, 15, 1);
    check({3, 2, 2, 4, 1, 4}, 3, 6, 2);
    check({1, 2, 3, 1, 1}, 4, 3, 3);
    check({1, 2, 3, 4, 5}, 5, 5, 4);
    check({10, 10, 10}, 1, 30, 5);
    check({7}, 1, 7, 6);
    check({7}, 3, 7, 7);
    check({1, 1, 1, 1, 1, 1}, 2, 3, 8);
    check({5, 1, 1, 1, 1, 1}, 2, 5, 9);
    check({1, 2, 3, 4, 5}, 1, 15, 10);
    check({9, 8, 1, 1, 1}, 4, 9, 11);
    check({500, 1, 1, 1, 499}, 2, 501, 12);
    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
