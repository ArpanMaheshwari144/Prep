// ============================================================
// SUBARRAY SUMS DIVISIBLE BY K   (REDO_1 — blank se, kal scaffold-se hua tha)
// ============================================================
// Array A (positive/negative) aur number k. GINO: kitne CONTIGUOUS
// subarrays hain jinka sum % k == 0.
//
//   - prefix-sum + hashmap family. "divisible" -> exact-sum nahi.
//   - kaagaz pe chhota example trace kar: running-sum ka REMAINDER dekh
//     (negative sambhaal: ((sum%k)+k)%k). same remainder dobara aaye to beech ka tukda %k==0.
//   - hashmap<remainder, count>. seed? (khaali-prefix). count += mp[rem]; phir mp[rem]++.
//   -- code jaldi mat likh, pehle ek dry-run se image; phir code.
//
// ---- TEST CASES ----
//   A=[4,5,0,-2,-3,1], k=5  -> 7
//   A=[5],             k=5  -> 1
//   A=[1,2,3],         k=3  -> 3
//   A=[1,2,3],         k=6  -> 1
//   A=[-1,2,9],        k=2  -> 2
// ============================================================

#include <bits/stdc++.h>
using namespace std;

//  = SAME family: SUBARRAY-SUM-K / DIVISIBLE-PAIRS (prefix + hashmap-count).
//    surface "divisible" -> un-disguise = REMAINDER-transform (dry-run se khula).

int subarraysDivByK(vector<int> &A, int k)
{
    unordered_map<int, int> mp;
    int sum = 0, count = 0;
    mp[0] = 1; // SEED: khaali-prefix -> jo subarray khud shuru se %k==0 ho, use bhi gino
    for (int i = 0; i < A.size(); i++)
    {
        sum += A[i];                    // running prefix-sum (question sum pe hai)
        int rem = ((sum % k) + k) % k;  // remainder -- brute jaisa sum%k, par negative-SAFE (0..k-1)
        count += mp[rem];               // ye rem pehle jitni baar aaya = utne subarrays (beech ka tukda %k==0)
        mp[rem]++;                      // is rem ko aage ke liye gino
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
    vector<int> a1 = {4, 5, 0, -2, -3, 1};
    check(subarraysDivByK(a1, 5), 7, "case1");
    vector<int> a2 = {5};
    check(subarraysDivByK(a2, 5), 1, "case2");
    vector<int> a3 = {1, 2, 3};
    check(subarraysDivByK(a3, 3), 3, "case3");
    vector<int> a4 = {1, 2, 3};
    check(subarraysDivByK(a4, 6), 1, "case4");
    vector<int> a5 = {-1, 2, 9};
    check(subarraysDivByK(a5, 2), 2, "case5");
    return 0;
}
