// ============================================================
// SUBARRAYS WITH K DIFFERENT INTEGERS   (LC-992, SW — exactly-K trick)
// ============================================================
// nums[] diya. Ginno kitne SUBARRAYS (contiguous) hain jinme
// EXACTLY k DIFFERENT (distinct) numbers ho.
//
//   nums = [1,2,1,2,3], k=2  -> 7
//     ([1,2],[2,1],[1,2],[1,2,1],[2,1,2],[1,2,1,2] ... exactly-2-distinct wale)
//
// >>> TRICK (jo seekha): exactly(k) = atMost(k) - atMost(k-1).
//     atMost(K) = normal variable-SW: distinct <= K rakho, har right pe += (right-left+1).
//     baaki tu likh. <<<
//
// ---- TEST CASES (nums, k -> expected) ----
//   [1,2,1,2,3],     k=2  -> 7
//   [1,2,1,3,4],     k=3  -> 3
//   [1,1,1,1],       k=1  -> 10     (n*(n+1)/2 = 4*5/2)
//   [2,1,2,1,2],     k=2  -> 10
//   [1,2,3,4,5],     k=1  -> 5      (har single element)
//   [1],             k=1  -> 1
// ============================================================
//
// ---- ARPAN KI APPROACH (exactly-K = atMost trick) ----
//  "exactly K distinct" ko seedha SW se count karna ganda (boundary saaf nahi).
//  TRICK: exactly(k) = atMost(k) - atMost(k-1).
//     atMost(K) = normal variable-SW: window me distinct <= K rakho,
//                 har right pe ans += (right - left + 1)  [is right pe khatam hone wale valid subarray].
//     distinct > K -> left aage badhao (count-- , 0 hua to map se erase) jab tak <= K.
//  KYUN: atMost(k) = "0..k distinct wale sab" ; atMost(k-1) = "0..k-1 wale sab".
//        ghata do -> sirf EXACTLY k bache. (jaise "<=5 - <=1 = 2..5")
//
//  ★ k=1 pe koi special-case NAHI: formula uniform. atMost(0) naturally 0 return karta
//    (0-distinct wala koi subarray nahi) -> subtraction rehne do.
//
//  ---- GALTI jo hui + fix (careful-note) ----
//   atMost helper max-window (totalFruit) se copy kiya -> ans = INT_MIN reh gaya.
//   par yahaan COUNT karna tha -> ans = 0 hona chahiye. (INT_MIN k=2 pe cancel ho gaya,
//   par atMost(0)=0 hone se k=1 pe cancel nahi hua -> garbage.) FIX: ans = 0.
//   SEEKH: template copy karo to "max -> count" shift pe init check karo.
//
//  = FRUIT-INTO-BASKETS (atMost-K distinct window) ka COUSIN, bas 2 baar chala ke ghata do.
//  COLD nahi hota (subtraction-trick) -> library me daala -> revise 1/3/7.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int totalFruit(vector<int> &fruits, int k)
{
    int i = 0, j = 0;
    unordered_map<int, int> mp;
    int ans = 0;
    while (j < fruits.size())
    {
        mp[fruits[j]]++;
        while (mp.size() > k)
        {
            mp[fruits[i]]--;
            if (mp[fruits[i]] == 0)
            {
                mp.erase(fruits[i]);
            }
            i++;
        }
        ans += j - i + 1;
        j++;
    }
    return ans;
}

int subarraysWithKDistinct(vector<int> &nums, int k)
{
    return totalFruit(nums, k) - totalFruit(nums, k - 1);
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {1, 2, 1, 2, 3};
    check(subarraysWithKDistinct(a1, 2), 7, "case1");

    vector<int> a2 = {1, 2, 1, 3, 4};
    check(subarraysWithKDistinct(a2, 3), 3, "case2");

    vector<int> a3 = {1, 1, 1, 1};
    check(subarraysWithKDistinct(a3, 1), 10, "case3");

    vector<int> a4 = {2, 1, 2, 1, 2};
    check(subarraysWithKDistinct(a4, 2), 10, "case4");

    vector<int> a5 = {1, 2, 3, 4, 5};
    check(subarraysWithKDistinct(a5, 1), 5, "case5");

    vector<int> a6 = {1};
    check(subarraysWithKDistinct(a6, 1), 1, "case6");
    return 0;
}
