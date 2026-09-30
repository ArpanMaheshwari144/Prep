// ============================================================
// THROTTLING — MIN UTHAO, PADOSI HATAO (Amazon OA jaisa)
// ============================================================
// responseTimes[] diya (har request ka response time).
// Har step me:
//   1. jo requests abhi BACHI hain, unme SABSE CHHOTA response time uthao
//      (barabar ho to jiska INDEX chhota ho).
//   2. uska value total me jodo.
//   3. use aur uske turant PADOSI (original index i-1 aur i+1) hata do.
//      (padosi pehle hi hat chuka ho to kuch nahi)
// Jab tak sab hat na jaayein, chalte raho.
// Total lautao (long long — values 1e9 tak ho sakti hain).
//
// Example: [4, 2, 9, 1, 7, 3]
//   1 uthaya (idx 3) -> 9 aur 7 hate
//   2 uthaya (idx 1) -> 4 hata (9 pehle hi gaya)
//   3 uthaya (idx 5)
//   total = 1 + 2 + 3 = 6
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   [4,2,9,1,7,3]                   -> 6
//   [5]                             -> 5
//   [1,2]                           -> 1
//   [2,1]                           -> 1
//   [3,3,3,3]                       -> 6
//   [1,1,1,1,1]                     -> 3
//   [5,4,3,2,1]                     -> 9
//   [10,1,10,1,10]                  -> 2
//   [1000000000,1000000000,1000000000] -> 2000000000
//   [2,1,3,1,2]                     -> 2
// ============================================================

#include <bits/stdc++.h>
using namespace std;

long long totalResponseTime(vector<int> &responseTimes)
{
    int n = responseTimes.size();
    vector<bool> vis(n, false);
    long long ans = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    for (int i = 0; i < n; i++)
    {
        pq.push({responseTimes[i], i});
    }

    while (!pq.empty())
    {
        int index = pq.top().second;
        pq.pop();
        if (vis[index] == false)
        {
            ans += responseTimes[index];
            vis[index] = true;
            if (index - 1 >= 0)
            {

                vis[index - 1] = true;
            }
            if (index + 1 < n)
            {
                vis[index + 1] = true;
            }
        }
    }
    return ans;
}

// ============================ TESTS ============================
int passed = 0, total = 0;

void check(vector<int> in, long long exp)
{
    total++;
    long long got = totalResponseTime(in);
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << (ok ? "PASS  " : "FAIL  ") << "test " << total << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    check({4, 2, 9, 1, 7, 3}, 6);
    check({5}, 5);
    check({1, 2}, 1);
    check({2, 1}, 1);
    check({3, 3, 3, 3}, 6);
    check({1, 1, 1, 1, 1}, 3);
    check({5, 4, 3, 2, 1}, 9);
    check({10, 1, 10, 1, 10}, 2);
    check({1000000000, 1000000000, 1000000000}, 2000000000LL);
    check({2, 1, 3, 1, 2}, 2);

    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
