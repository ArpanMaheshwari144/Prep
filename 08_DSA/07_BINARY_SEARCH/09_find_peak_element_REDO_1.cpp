// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// FIND PEAK ELEMENT   (LC-162 — REDO_1, khaali page)
// ============================================================
// int array nums. Ek PEAK ka INDEX lautao -- peak = jo apne DONO padosiyon se
// STRICTLY BADA ho. Kai peak ho to KOI BHI ek chalega.
// Maano nums[-1] = nums[n] = -infinity (array ke bahar sabse neecha).
// Do padosi kabhi barabar nahi (nums[i] != nums[i+1]).
// O(log n) chahiye.
//
//   [1,3,2,4,6,5] -> 1 ya 4
//
// Tests (checker khud dekhega ki lautaya index sach me peak hai):
//   [1,2,3,1] · [1,2,1,3,5,6,4] · [1] · [1,2] · [2,1] · [5,4,3,2,1] · [1,2,3,4,5]
//   [1,3,2,4,6,5] · [3,1,2] · [1,3,2,1]
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int findPeakElement(vector<int> &nums)
{
}

// ---------- checker (ise mat chhed) ----------
int passed = 0, total = 0;
void check(vector<int> nums)
{
    total++;
    int n = nums.size();
    int i = findPeakElement(nums);
    bool ok = false;
    if (i >= 0 && i < n)
    {
        long long left = (i - 1 >= 0) ? nums[i - 1] : LLONG_MIN;
        long long right = (i + 1 < n) ? nums[i + 1] : LLONG_MIN;
        ok = (nums[i] > left && nums[i] > right);
    }
    if (ok)
        passed++;
    cout << "test " << total << "  idx=" << i << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check({1, 2, 3, 1});
    check({1, 2, 1, 3, 5, 6, 4});
    check({1});
    check({1, 2});
    check({2, 1});
    check({5, 4, 3, 2, 1});
    check({1, 2, 3, 4, 5});
    check({1, 3, 2, 4, 6, 5});
    check({3, 1, 2});
    check({1, 3, 2, 1});
    cout << "\n" << passed << "/" << total << " passed\n";
    return 0;
}
