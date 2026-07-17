// ============================================================
// FIND PEAK ELEMENT — Binary Search  (fresh, NAYA pattern)
// ============================================================
// int array nums. ek PEAK element ka INDEX lauta -- peak = jo apne DONO neighbours
// se STRICTLY BADA ho. Multiple peak ho sakte -> KOI BHI ek peak ka index chalega.
// maano nums[-1] = nums[n] = -infinity (edges ke bahar sabse chhota).
// O(log n) chahiye (binary search).
//   [1,2,3,1]        -> index 2 (element 3, peak)
//   [1,2,1,3,5,6,4]  -> index 5 (element 6)  [ya index 1 bhi valid]
//   [1]              -> index 0
//   [1,2]            -> index 1
//   [2,1]            -> index 0
//

// ---- ARPAN KI APPROACH ----
//  ★ NAYA insight: array SORTED nahi, phir bhi BS lagta -- kyunki ek comparison se
//     tu PAKKA keh sakta peak kis half me hoga (BS ko full-sorted nahi, "half-discard property" chahiye).
//  ★ mid ko DONO neighbours se compare NAHI -- sirf mid+1 se (ek comparison kaafi):
//     nums[mid] < nums[mid+1]  -> chadhaai (right ooncha) -> peak RIGHT me guaranteed -> low = mid+1
//     else (nums[mid] > nums[mid+1]) -> dhalaan -> peak mid KHUD ya LEFT me -> high = mid
//  ★★ GOTCHA (yeh naya seekha): dusre case me high = mid-1 NAHI, high = mid.
//     kyunki mid KHUD peak ho sakta hai (right se bada hai already) -> use discard mat karo.
//     (rule: mid REJECT kar diya? -> mid-1 (+ while low<=high).  mid abhi CANDIDATE? -> mid (+ while low<high).)
//  while(low < high); ant me low == high pe aake ruk jaata -> wahi peak -> return low.
//  (edges auto-handle: kinaare ke bahar -infinity, to kinaara khud peak ban jaata -- sentinel add karne ki zaroorat nahi.)
//
//  ---- 18-Jul, DEBUG + REFINE se nikla (2 valid version, dono SAME core) ----
//  V1 (SHORT, interview-BEST -- neeche active): sirf mid vs mid+1; nums[mid]<nums[mid+1] -> low=mid+1; else high=mid; return low.
//  V2 (explicit peak-check, khud debug kiya): pehle "mid peak hai?" (dono neighbour) -> return mid; else ascending->low=mid+1; else high=mid-1.
//     ★ V2 me high=mid-1 SAHI hai kyunki mid ko EXPLICITLY reject (peak-check se) kar chuke -> discard safe.
//  ★★ DONO SAME algorithm hain -- answer HAMESHA `return low` (convergence) se aa hi jaata; V2 ka explicit-check bas EXTRA early-return hai (optional).
//  ★ INTERVIEW = V1 (short): koi explicit-check nahi, koi edge-sentinel nahi (while low<high -> mid<high -> mid+1 hamesha in-bounds).
//  ★ agar sentinel dena ho (nums[-1] wala): -infinity = INT_MIN daalo, `nums[0]` NAHI (nums[0]<nums[0]=false -> check mid=0 pe kaam nahi karta; phir bhi return-low bacha leta).
//  ★ META: surface pe alag dikhne wale 2 code -> peeche SAME pattern pehchanna = recognition skill.

// Tests (checker verify karega ki lautaya index sach me peak hai):
//   [1,2,3,1] · [1,2,1,3,5,6,4] · [1] · [1,2] · [2,1] · [5,4,3,2,1] · [1,2,3,4,5]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int findPeakElement(vector<int> &nums)
{
    int n = nums.size();
    int low = 0, high = n - 1;
    while (low < high)
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] < nums[mid + 1])
        {
            low = mid + 1;
        }
        else if (nums[mid] > nums[mid + 1])
        {
            high = mid;
        }
    }
    return low;
}

// ---------- checker (boilerplate, chhoo mat) ----------
void check(vector<int> nums)
{
    int i = findPeakElement(nums);
    int n = nums.size();
    long long left = (i - 1 >= 0) ? nums[i - 1] : LLONG_MIN;
    long long right = (i + 1 < n) ? nums[i + 1] : LLONG_MIN;
    bool ok = (nums[i] > left && nums[i] > right);
    cout << "idx=" << i << " (val=" << nums[i] << ")  -> " << (ok ? "PEAK ✓" : "NOT PEAK ✗") << "\n";
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
    return 0;
}
