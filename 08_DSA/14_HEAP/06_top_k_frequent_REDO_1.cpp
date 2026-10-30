// ═══════════════════════════════════════════════════════════════════════
//  PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: ★ EDGE line + ek chhota dushman-example chalao
// ═══════════════════════════════════════════════════════════════════════
//  TOP K FREQUENT ELEMENTS
//  nums[] aur k diya. Wo k numbers lautao jo SABSE ZYADA baar aaye hain. Order matter nahi.
//  Jawab hamesha ek hi hota hai (top k me barabari nahi tootti).
//
//  INPUT-FORM: 1 <= nums.size() <= 1e5, -1e4 <= nums[i] <= 1e4, 1 <= k <= alag numbers ki ginti.
//  TEST-CASES (nums, k -> expected, koi bhi order):
//   1) [1,1,1,2,2,3], k=2                 -> {1,2}
//   2) [1], k=1                           -> {1}
//   3) [4,4,4,6,6,2,2,2], k=2             -> {4,2}      (4 aur 2 teen-teen baar, 6 do baar)
//   4) [5,5,5,5], k=1                     -> {5}
//   5) [7,8,9], k=3                       -> {7,8,9}    (k = saare alag numbers)
//   6) [-1,-1,-2,-2,-2,3], k=1            -> {-2}       (negative)
//   7) [1,2,2,3,3,3,4,4,4,4], k=3         -> {4,3,2}
//   8) [1,1,2,2,3], k=3                   -> {1,2,3}
//   9) [3,0,1,0], k=1                     -> {0}        (0 bhi number hai)
//  10) [6,5,6,5,6,7,7,7,7], k=2           -> {7,6}      (bikhre hue)
//  11) 9 x 1000 baar, 8 x 999 baar, 1, k=2 -> {9,8}     (bada input)
//  12) [0,0,0,1,1,2,2,2,2,3], k=2         -> {2,0}
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

vector<int> topKFrequent(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    for (auto &it : nums)
    {
        mp[it]++;
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;
    for (auto &it : mp)
    {
        minHeap.push({it.second, it.first});
        if (minHeap.size() > k)
        {
            minHeap.pop();
        }
    }

    vector<int> ans;
    while (!minHeap.empty())
    {
        ans.push_back(minHeap.top().second);
        minHeap.pop();
    }
    return ans;
}

// ---- test helper (ise mat chhed) -- order matter nahi ----
void check(vector<int> nums, int k, vector<int> exp, string name)
{
    vector<int> got = topKFrequent(nums, k);
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name << " | got={";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "}\n";
}

int main()
{
    check({1, 1, 1, 2, 2, 3}, 2, {1, 2}, "t1");
    check({1}, 1, {1}, "t2 ek number");
    check({4, 4, 4, 6, 6, 2, 2, 2}, 2, {4, 2}, "t3");
    check({5, 5, 5, 5}, 1, {5}, "t4 sab same");
    check({7, 8, 9}, 3, {7, 8, 9}, "t5 k = sab");
    check({-1, -1, -2, -2, -2, 3}, 1, {-2}, "t6 negative");
    check({1, 2, 2, 3, 3, 3, 4, 4, 4, 4}, 3, {4, 3, 2}, "t7");
    check({1, 1, 2, 2, 3}, 3, {1, 2, 3}, "t8");
    check({3, 0, 1, 0}, 1, {0}, "t9 zero");
    check({6, 5, 6, 5, 6, 7, 7, 7, 7}, 2, {7, 6}, "t10 bikhre");
    vector<int> big;
    for (int i = 0; i < 1000; i++)
        big.push_back(9);
    for (int i = 0; i < 999; i++)
        big.push_back(8);
    big.push_back(1);
    check(big, 2, {9, 8}, "t11 bada");
    check({0, 0, 0, 1, 1, 2, 2, 2, 2, 3}, 2, {2, 0}, "t12");
    return 0;
}
