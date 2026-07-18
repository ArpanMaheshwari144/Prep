// ============================================================
// SPLIT ARRAY LARGEST SUM — (LeetCode 410)   (fresh)
// ============================================================
// int array nums (non-negative) aur ek int k diya.
// array ko K CONTIGUOUS (lagatar) non-empty subarrays me baanto.
// har split ka ek "sum" hota (us subarray ke elements ka jod).
// K subarrays ke jitne bhi sum bane, unme se SABSE BADA sum -> minimize karo.
// yaani: aise baanto ki "largest subarray-sum" JITNA CHHOTA ho sake, wahi lauta.

// ---- ARPAN KI APPROACH ----
//  ★ RECOGNITION: ye bhai koko-bananas aur ship-within-days wale se SAME-TO-SAME pattern pe based hai
//     -> BS-ON-ANSWER. question ko samjho: array ko k parts me todna hai, har part ka sum, aur unme se
//     SABSE BADA sum -> JITNA CHHOTA ho sake wahi lauta.
//
//  ★ RANGE (kis cheez pe BS): guess-value = ek candidate "largest sum".
//     low  = max of array  (kyunki sabse bada single element ko todna allowed nahi -> wo ek tukde me akela
//            aayega hi -> largest sum kabhi max-element se chhota nahi ho sakta).
//     high = sum of array  (k=1 wala case -> pura array ek hi tukda -> pura sum).
//
//  ★ BS shape: while (low <= high) -- yahan low<high NAHI, kyunki hum high = mid-1 / low = mid+1 kar rahe
//     (mid ko reject/accept karke aage badhte, candidate hold ans me karte). rule yaad: mid-1 use kiya -> low<=high.
//
//  ★ mid = cap. maan lo [7,2,5,10,8] pe mid=21. iska matlab "koi tukda 21 se bada nahi hona chahiye".
//     ab array ko is cap se tod ke GINN kitne tukde bante:
//        - 21 pe 2 tukde bane (<= k) -> mid FEASIBLE -> ans = mid, aur CHHOTA try karo -> high = mid-1
//        - agar tukde ZYADA (> k) -> cap chhota pad gaya -> low = mid+1
//
//  ★ solve() = ship/koko wala feasibility-check: sum jodte jaao; sum > mid ho jaye -> count++ aur
//     sum = nums[i] (naye tukde me current element carry). ant me count <= k -> feasible.
//     ★ count = 1 se START (0 se NAHI) -- warna aakhri tukda (jo kabhi cap cross nahi karta) ginti me
//        chhut jayega. pehla tukda already chalu hai -> isliye 1.
//
//  ★ ans = mid store karke aakhir me return ans (jab bhi feasible mila, best-so-far pakad ke chhota dhoondte gaye).


//
//   nums = [7,2,5,10,8], k = 2  -> 18
//        (best split: [7,2,5] | [10,8] -> sums 14 aur 18 -> largest = 18. isse behtar nahi.)
//   nums = [1,2,3,4,5], k = 2  -> 9
//        ([1,2,3] | [4,5] -> 6 aur 9 -> 9)
//   nums = [1,4,4], k = 3      -> 4
//        (har element apna subarray -> 1|4|4 -> largest = 4)
//   nums = [1,2,3,4,5], k = 1  -> 15   (ek hi subarray -> pura sum)
//   nums = [10], k = 1         -> 10
//
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool solve(vector<int> &nums, int k, int mid)
{
    int sum = 0;
    int count = 1;
    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i];
        if (sum > mid)
        {
            count++;
            sum = nums[i];
        }
    }
    return count <= k;
}

int splitArray(vector<int> &nums, int k)
{
    int low = *max_element(begin(nums), end(nums));
    int high = accumulate(begin(nums), end(nums), 0);
    int ans = -1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (solve(nums, k, mid))
        {
            ans = mid;
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return ans;
}

int main()
{
    vector<int> a = {7, 2, 5, 10, 8};
    vector<int> b = {1, 2, 3, 4, 5};
    vector<int> c = {1, 4, 4};
    vector<int> d = {1, 2, 3, 4, 5};
    vector<int> e = {10};

    cout << splitArray(a, 2) << " (expected 18)\n";
    cout << splitArray(b, 2) << " (expected 9)\n";
    cout << splitArray(c, 3) << " (expected 4)\n";
    cout << splitArray(d, 1) << " (expected 15)\n";
    cout << splitArray(e, 1) << " (expected 10)\n";
    return 0;
}
