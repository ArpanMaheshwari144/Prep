// ============================================================
// PEHLE YE (har problem pe, 30 sec)
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree? intervals? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE -> "baar-baar SAME kya nikal raha?" -> STORE -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" -> X me jo THI HI NAHI, wo line kya? (edge wahin)
//   6. Code ke BAAD: pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// LONGEST SUBARRAY WITH SUM K  (LC-325 jaisa)   [naya, 9-Nov grill twist se, Arpan ne maanga]
// ============================================================
// nums[] (negative bhi ho sakte) aur k diya. Aisa SABSE LAMBA lagaatar tukda
// (subarray) dhoondo jiska total THEEK k ho. Uski LAMBAI lautao. Koi na mile -> 0.
// (subarray_sum_k me GINTI thi, yahan LAMBAI.)
//
//   [3,-1,4,-2,2,1], k=4   -> 5    ([-1,4,-2,2,1])
//   [3,-1,4,-2,2,1], k=5   -> 4    ([4,-2,2,1])
//   [1,-1,5,-2,3],   k=3   -> 4    ([1,-1,5,-2])
//   [-2,-1,2,1],     k=1   -> 2    ([-1,2])
//   [1,2,3],         k=3   -> 2    ([1,2])
//   [1,2,3],         k=7   -> 0    (koi nahi)
//   [5],             k=5   -> 1
//   [0,0,0],         k=0   -> 3    (poora)
//   [1,-1,1,-1],     k=0   -> 4    (poora)
//   [1,1,1],         k=2   -> 2
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int longestSubarraySumK(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    mp[0] = -1;
    int sum = 0;
    int ans = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i];
        if (mp.count(sum - k))
        {
            ans = max(ans, i - mp[sum - k]);
        }
        if (!mp.count(sum)) // override se bachao
            mp[sum] = i;
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> nums, int k, int exp, int t)
{
    int got = longestSubarraySumK(nums, k);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({3, -1, 4, -2, 2, 1}, 4, 5, 1);
    check({3, -1, 4, -2, 2, 1}, 5, 4, 2);
    check({1, -1, 5, -2, 3}, 3, 4, 3);
    check({-2, -1, 2, 1}, 1, 2, 4);
    check({1, 2, 3}, 3, 2, 5);
    check({1, 2, 3}, 7, 0, 6);
    check({5}, 5, 1, 7);
    check({0, 0, 0}, 0, 3, 8);
    check({1, -1, 1, -1}, 0, 4, 9);
    check({1, 1, 1}, 2, 2, 10);
    return 0;
}
