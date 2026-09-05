// ============================================================
// SPLIT ARRAY LARGEST SUM — (LeetCode 410)   (fresh)
// ============================================================
// int array nums (non-negative) aur ek int k diya.
// array ko K CONTIGUOUS (lagatar) non-empty subarrays me baanto.
//
// WORDING SAAF (10-din-baad-confusion-proof):
//   ek split me K parts bante -> har part ka apna sum. us split ka jo SABSE BADA part-sum ho,
//   wahi us split ki "COST" hai (chhote part-sum se matlab nahi -- bada hi cost decide karta).
//   ab SAARE possible splits me se jis split ki COST (bada-sum) SABSE CHHOTI ho -> wahi answer lauta.
//   (yaani: bada-sum ko "across all splits" minimize karo. ek split ke andar uska bada-sum majboori hai.)
//
//   EXAMPLE se pakka samjho -- [7,2,5,10,8], k=2. saare 2-splits + har ek ka BADA-sum:
//        [7]        | [2,5,10,8]   -> 7,  25   -> cost = 25
//        [7,2]      | [5,10,8]     -> 9,  23   -> cost = 23
//        [7,2,5]    | [10,8]       -> 14, 18   -> cost = 18   <-- inme sabse CHHOTI cost
//        [7,2,5,10] | [8]          -> 24, 8    -> cost = 24
//      har split ki cost: 25, 23, 18, 24 -> in sab me se MINIMUM = 18 -> ANSWER = 18.
//      14 kyun NAHI: 14 to [7,2,5] wale split ka CHHOTA part hai; ussi split me [10,8]=18 bhi hai
//        jise ignore nahi kar sakte. "dono part <= 14" karna chaho -> [10,8]=18 > 14 -> fit hi nahi.
//        isliye 18 se neeche jaa hi nahi sakte.



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
