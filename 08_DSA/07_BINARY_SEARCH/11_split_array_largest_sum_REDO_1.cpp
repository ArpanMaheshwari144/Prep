// ============================================================
// SPLIT ARRAY LARGEST SUM   (LC-410 — REDO_1, khaali page)
// ============================================================
// nums[] (sab >= 0) aur k diya. Array ko k LAGAATAAR (contiguous), non-empty tukdon me baanto.
// Har tukde ka sum nikaalo; us split ki "cost" = sabse BADA tukda-sum.
// Saare possible splits me se sabse CHHOTI cost lautao.
//
//   [7,2,5,10,8], k=2  ->  18     ([7,2,5]=14 | [10,8]=18)
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   [7,2,5,10,8], k=2     -> 18
//   [1,2,3,4,5],  k=2     -> 9
//   [1,4,4],      k=3     -> 4
//   [10,20,30,40], k=2    -> 60     (grill wala: [10,20,30] | [40])
//   [5],          k=1     -> 5
//   [1,2,3,4,5],  k=1     -> 15     (poora array ek tukda)
//   [1,2,3,4,5],  k=5     -> 5      (har element akela)
//   [0,0,0,0],    k=2     -> 0
//   [2,3,1,2,4,3], k=3    -> 6
//   [100,1,1,1],  k=2     -> 100    (bada element akela bhi 100)
//   [1,1,1,1,1,1,1,1,1,10], k=2 -> 10
//   [3,3,3,3,3,3], k=4    -> 6
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool solve(vector<int> &nums, int k, int mid)
{
    int sum = 0, count = 1;
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

// ---- test helper (ise mat chhed) ----
void check(vector<int> nums, int k, int exp, const string &label)
{
    int got = splitArray(nums, k);
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check({7, 2, 5, 10, 8}, 2, 18, "case1");
    check({1, 2, 3, 4, 5}, 2, 9, "case2");
    check({1, 4, 4}, 3, 4, "case3");
    check({10, 20, 30, 40}, 2, 60, "case4");
    check({5}, 1, 5, "case5");
    check({1, 2, 3, 4, 5}, 1, 15, "case6");
    check({1, 2, 3, 4, 5}, 5, 5, "case7");
    check({0, 0, 0, 0}, 2, 0, "case8");
    check({2, 3, 1, 2, 4, 3}, 3, 6, "case9");
    check({100, 1, 1, 1}, 2, 100, "case10");
    check({1, 1, 1, 1, 1, 1, 1, 1, 1, 10}, 2, 10, "case11");
    check({3, 3, 3, 3, 3, 3}, 4, 6, "case12");
    return 0;
}
