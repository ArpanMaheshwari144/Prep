// ============================================================
// MAX CONSECUTIVE ONES III — Sliding Window  (SPACED-REDO, blank)
// ============================================================
// binary array nums (0/1) aur int k. tu ZYADA-SE-ZYADA k zeros FLIP (0->1) kar sakta.
// flip karke jo SABSE LAMBI consecutive 1s ki length ban sakti -> wo lautao.
//   nums=[1,1,1,0,0,0,1,1,1,1,0], k=2  -> 6
//
// ---- ARPAN KI APPROACH ----
//  variable window + zeroCount. j pe 0 mila -> zeroCount++. k>0 hua to jab tak
//  zeroCount > k -> left se shrink. update sirf jab (zeroCount == k && k > 0).
//  end me agar total zeros==0 ya zeros<k -> poora nums.size() lautao, warna ans.
//
// Tests (complete — edge cases bhi):
//   [1,1,1,0,0,0,1,1,1,1,0], k=2                       -> 6
//   [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], k=3       -> 10
//   [0,0,0,0], k=0                                      -> 0
//   [1,1,1,1], k=0                                      -> 4
//   [1,0,1,0,1], k=1                                    -> 3
//   [1,1,1,1], k=2                                      -> 4   (edge: k > total zeros)
//   [1,1,0,1], k=5                                      -> 4   (edge: k > total zeros)
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int longestOnes(vector<int> &nums, int k)
{
    // int ans = INT_MIN;
    // int zeroCount = 0;
    // int i = 0, j = 0;
    // while (j < nums.size())
    // {
    //     if (nums[j] == 0)
    //     {
    //         zeroCount++;
    //     }

    //     while (zeroCount > k)
    //     {
    //         if (nums[i] == 0)
    //         {
    //             zeroCount--;
    //         }
    //         i++;
    //     }
    //     ans = max(ans, j - i + 1);
    //     j++;
    // }
    // return ans == INT_MIN ? 0 : ans;

    int ans = INT_MIN;
    int zeroCount = 0;
    int i = 0, j = 0;
    while (j < nums.size())
    {
        if (nums[j] == 0)
        {
            zeroCount++;
        }

        if (k > 0)
        {
            while (zeroCount > k)
            {
                if (nums[i] == 0)
                {
                    zeroCount--;
                }
                i++;
            }
        }

        if (zeroCount == k && k > 0) // vector<int> c = {0, 0, 0, 0}; k = 0
        {
            ans = max(ans, j - i + 1);
        }

        j++;
    }

    if (zeroCount == 0 || zeroCount < k)
    {
        return nums.size();
    }

    return ans == INT_MIN ? 0 : ans;
}

int main()
{
    vector<int> a = {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0};
    vector<int> b = {0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1};
    vector<int> c = {0, 0, 0, 0};
    vector<int> d = {1, 1, 1, 1};
    vector<int> e = {1, 0, 1, 0, 1};
    vector<int> f = {1, 1, 1, 1};
    vector<int> g = {1, 1, 0, 1};

    cout << longestOnes(a, 2) << " (expected 6)\n";
    cout << longestOnes(b, 3) << " (expected 10)\n";
    cout << longestOnes(c, 0) << " (expected 0)\n";
    cout << longestOnes(d, 0) << " (expected 4)\n";
    cout << longestOnes(e, 1) << " (expected 3)\n";
    cout << longestOnes(f, 2) << " (expected 4)\n";
    cout << longestOnes(g, 5) << " (expected 4)\n";
    return 0;
}
