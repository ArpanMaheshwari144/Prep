// ═══════════════════════════════════════════════════════════════════════
//  PEHLE YE — har problem pe (30 sec)
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ═══════════════════════════════════════════════════════════════════════
//  THROTTLING — MIN UTHAO, PADOSI HATAO   (REDO_1)
//  responseTimes[] diya (har request ka response time). Har step me:
//    1. jo requests abhi BACHI hain, unme SABSE CHHOTA response time uthao
//       (barabar ho to jiska INDEX chhota ho).
//    2. uska value total me jodo.
//    3. use aur uske turant PADOSI (original index i-1 aur i+1) hata do.
//       (padosi pehle hi hat chuka ho to kuch nahi)
//  Jab tak sab hat na jaayein, chalte raho. RETURN: total (long long, values 1e9 tak).
//
//  Example: [4, 2, 9, 1, 7, 3]
//    1 uthaya (idx 3) -> 9 aur 7 hate
//    2 uthaya (idx 1) -> 4 hata (9 pehle hi gaya)
//    3 uthaya (idx 5)
//    total = 1 + 2 + 3 = 6
//
//  TEST-CASES (input -> expected):
//   1) [4,2,9,1,7,3]                       -> 6
//   2) [5]                                 -> 5
//   3) [1,2]                               -> 1
//   4) [2,1]                               -> 1
//   5) [3,3,3,3]                           -> 6
//   6) [1,1,1,1,1]                         -> 3
//   7) [5,4,3,2,1]                         -> 9
//   8) [10,1,10,1,10]                      -> 2
//   9) [1000000000,1000000000,1000000000]  -> 2000000000
//  10) [2,1,3,1,2]                         -> 2
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

long long totalResponseTime(vector<int> &responseTimes)
{
    int n = responseTimes.size();
    long long ans = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<bool> vis(n, false);
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
