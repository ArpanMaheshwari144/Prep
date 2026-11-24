// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// SUBARRAY SUM EQUALS K   (LC-560 — REDO_3, khaali page)
// ============================================================
// Array nums + ek number k diya. KITNE contiguous subarrays (lagaatar hissa) hain
// jinka sum EXACTLY k hai -> wo COUNT lautao. nums me minus aur 0 bhi ho sakte hain.
//
//   nums = [1,2,3], k = 3 -> 2   ([1,2] aur [3])
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   [1,1,1],               k=2   -> 2
//   [1,2,3],               k=3   -> 2
//   [1,-1,0],              k=0   -> 3
//   [3,4,7,2,-3,1,4,2],    k=7   -> 4
//   [1],                   k=0   -> 0
//   [1],                   k=1   -> 1
//   [0,0,0],               k=0   -> 6
//   [-1,-1,1],             k=0   -> 1
//   [1,2,1,2,1],           k=3   -> 4
//   [5],                   k=-5  -> 0
//   [2,-2,2,-2],           k=0   -> 4
//   [10,2,-2,-20,10],      k=-10 -> 3
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int subarraySum(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    int count = 0, sum = 0;
    mp[0] = 1;
    for (auto &it : nums)
    {
        sum += it;
        count += mp[sum - k];
        mp[sum]++;
    }
    return count;
}

// ---- test helper (ise mat chhed) ----
int passed = 0, total = 0;
void check(vector<int> nums, int k, int exp)
{
    total++;
    string arr = "[";
    for (int i = 0; i < (int)nums.size(); i++)
        arr += (i ? "," : "") + to_string(nums[i]);
    arr += "]";
    int got = subarraySum(nums, k);
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << arr << " k=" << k << " -> got " << got << " | exp " << exp
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check({1, 1, 1}, 2, 2);
    check({1, 2, 3}, 3, 2);
    check({1, -1, 0}, 0, 3);
    check({3, 4, 7, 2, -3, 1, 4, 2}, 7, 4);
    check({1}, 0, 0);
    check({1}, 1, 1);
    check({0, 0, 0}, 0, 6);
    check({-1, -1, 1}, 0, 1);
    check({1, 2, 1, 2, 1}, 3, 4);
    check({5}, -5, 0);
    check({2, -2, 2, -2}, 0, 4);
    check({10, 2, -2, -20, 10}, -10, 3);
    cout << "\n"
         << passed << "/" << total << " PASS\n";
    return 0;
}
