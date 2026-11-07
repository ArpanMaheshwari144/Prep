// ============================================================
// PEHLE YE (har problem pe, 30 sec)
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree? intervals? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE -> "baar-baar SAME kya nikal raha?" -> STORE -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" -> X me jo THI HI NAHI, wo line kya? (edge wahin)
//   6. Code ke BAAD: pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// COUNT SUBARRAYS WITH MAXIMUM == K   [REDO_2]
// ============================================================
// arr[] aur ek number K diya hai. Kitne lagaatar subarray hain jinka sabse bada number
// theek K ho (na zyada, na kam)? Wo ginti lautao. n 1e5 tak ho sakta hai.
//
//   [8,2,1,3,4,5,1,10], K=3     -> 3
//   [3,3,3],            K=3     -> 6
//   [1,2,4],            K=3     -> 0
//   [3],                K=3     -> 1
//   [5,3,5],            K=3     -> 1
//   [3,1,2],            K=3     -> 3
//   [1,3,1,3],          K=3     -> 8
//   [2,2],              K=3     -> 0
//   [3,5,3,1],          K=3     -> 3
//   [1,2,3,4,3,2,1],    K=3     -> 6
//   1e5 baar 3,         K=3     -> 5000050000
// ============================================================

#include <bits/stdc++.h>
using namespace std;

long long countSubarraysMaxK(vector<int> &arr, int K)
{
    int prevK = -1, lastK = -1;
    long long count = 0;
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] > K)
        {
            prevK = i;
        }
        if (arr[i] == K)
        {
            lastK = i;
        }
        if (lastK > prevK)
            count += lastK - prevK;
    }
    return count;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, int K, long long exp, int t)
{
    long long got = countSubarraysMaxK(in, K);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    check({8, 2, 1, 3, 4, 5, 1, 10}, 3, 3, 1);
    check({3, 3, 3}, 3, 6, 2);
    check({1, 2, 4}, 3, 0, 3);
    check({3}, 3, 1, 4);
    check({5, 3, 5}, 3, 1, 5);
    check({3, 1, 2}, 3, 3, 6);
    check({1, 3, 1, 3}, 3, 8, 7);
    check({2, 2}, 3, 0, 8);
    check({3, 5, 3, 1}, 3, 3, 9);
    check({1, 2, 3, 4, 3, 2, 1}, 3, 6, 10);
    check(vector<int>(100000, 3), 3, 5000050000LL, 11);
    return 0;
}
