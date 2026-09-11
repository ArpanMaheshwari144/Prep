// ============================================================
// COUNT SUBARRAYS WITH MAXIMUM == K  [REDO_1 · blank retrieval]
// ============================================================
// arr[] aur K -> kitne subarrays jinka MAX bilkul K ho -> count.
//   valid = (har element <= K) AND (>=1 element == K).  K se bade = WALL.
//
//   [8,2,1,3,4,5,1,10], K=3  -> 3
//   [3,3,3],           K=3  -> 6
//   [1,2,4],           K=3  -> 0
//   [3],              K=3  -> 1
//   [5,3,5],          K=3  -> 1
//   [3,1,2],          K=3  -> 3
// insight (khud yaad kar): lastWall + lastK track (init -1) -> har i pe count += (lastK - lastWall),
//                          sirf jab lastK > lastWall.  ★ LONG LONG (overflow).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

long long countSubarraysMaxK(vector<int> &arr, int K)
{
    int lastWall = -1;
    int lastK = -1;
    long long count = 0;
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] > K)
        {
            lastWall = i;
        }
        if (arr[i] == K)
        {
            lastK = i;
        }
        if (lastK > lastWall)
            count += lastK - lastWall;
    }
    return count;
}

// ─── TESTS (haath mat lagana) ──────────────────────────
void check(vector<int> in, int K, long long exp, int t)
{
    long long got = countSubarraysMaxK(in, K);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({8, 2, 1, 3, 4, 5, 1, 10}, 3, 3, 1);
    check({3, 3, 3}, 3, 6, 2);
    check({1, 2, 4}, 3, 0, 3);
    check({3}, 3, 1, 4);
    check({5, 3, 5}, 3, 1, 5);
    check({3, 1, 2}, 3, 3, 6);
    return 0;
}
