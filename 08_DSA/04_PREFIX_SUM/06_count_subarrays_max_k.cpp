// ============================================================
// COUNT SUBARRAYS WITH MAXIMUM == K   [boundary-index counting · count-subarrays-ending-at-i]
// ============================================================
// arr[] diya (n tak 1e5), aur ek K. Kitne subarrays aise hain jinka MAXIMUM element
// bilkul K ho (na zyada na kam) -> COUNT return.
//   valid subarray = (har element <= K) AND (kam-se-kam ek element == K)
//   -> K se BADE elements = WALL (array ko todte).
//
//   [8,2,1,3,4,5,1,10], K=3  -> 3     ([2,1,3],[1,3],[3])
//   [3,3,3],            K=3  -> 6     (n*(n+1)/2, sab ke max=3)
//   [1,2,4],            K=3  -> 0     (koi element ==3 nahi; 4>3)
//   [3],               K=3  -> 1
//   [5,3,5],           K=3  -> 1     ([3] only; 5 walls)
//   [3,1,2],           K=3  -> 3     ([3],[3,1],[3,1,2])
//
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
long long countSubarraysMaxK(vector<int> &arr, int K)
{
    long long lastWall = -1;
    long long lastK = -1;
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

// ─── TESTS (isko haath mat lagana) ──────────────────────────
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
