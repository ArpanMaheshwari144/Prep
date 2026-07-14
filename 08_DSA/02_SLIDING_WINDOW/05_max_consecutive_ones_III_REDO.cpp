// ============================================================
// MAX CONSECUTIVE ONES III — Sliding Window  (SPACED-REDO, blank, CLEAN)
// ============================================================
// binary array nums (0/1) aur int k. tu ZYADA-SE-ZYADA k zeros FLIP (0->1) kar sakta.
// flip karke jo SABSE LAMBI consecutive 1s ki length ban sakti -> wo lautao.
//   nums=[1,1,1,0,0,0,1,1,1,1,0], k=2  -> 6
//
// ---- ARPAN KI APPROACH ----
//  variable sliding window + zeroCount. window me zeros ki ginti rakho.
//  j aage badhao: nums[j]==0 -> zeroCount++.
//  INVALID kab? -> zeroCount > k (allowed se zyada flip). tab tak left(i) se shrink:
//     nums[i]==0 hai to zeroCount--, i++ (jab tak window firse valid).
//  ★ ans store karte waqt KOI if nahi -- kyunki jab hum upar se yahan tak aaye,
//     to upar ki (shrink) conditions se window APNE AAP valid ho chuki hai.
//     isliye seedha ans = max(ans, j-i+1) UNCONDITIONAL.
//  koi k>0 guard/flag nahi -- k=0 apne aap handle (zeroCount>0 pe shrink ho jaata).
//
// Tests (edge + bug-catch bhi):
//   [1,1,1,0,0,0,1,1,1,1,0], k=2                       -> 6
//   [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], k=3       -> 10
//   [0,0,0,0], k=0                                      -> 0
//   [1,1,1,1], k=0                                      -> 4
//   [1,0,1,0,1], k=1                                    -> 3
//   [1,1,1,1], k=2                                      -> 4   (k > total zeros)
//   [1,1,0,1], k=5                                      -> 4   (k > total zeros)
//   [1,1,0,1,1], k=0                                    -> 2   (★ mixed + k=0)
//   [0,1,1], k=0                                        -> 2   (★ mixed + k=0)
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

// tera code yahan:

int longestOnes(vector<int> &nums, int k)
{
    int i = 0, j = 0;
    int zeroCount = 0;
    int ans = INT_MIN;

    while (j < nums.size())
    {
        if (nums[j] == 0)
        {
            zeroCount++;
        }
        while (zeroCount > k)
        {
            if (nums[i] == 0)
            {
                zeroCount--;
            }
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans;
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
    vector<int> h = {1, 1, 0, 1, 1};
    vector<int> p = {0, 1, 1};

    cout << longestOnes(a, 2) << " (expected 6)\n";
    cout << longestOnes(b, 3) << " (expected 10)\n";
    cout << longestOnes(c, 0) << " (expected 0)\n";
    cout << longestOnes(d, 0) << " (expected 4)\n";
    cout << longestOnes(e, 1) << " (expected 3)\n";
    cout << longestOnes(f, 2) << " (expected 4)\n";
    cout << longestOnes(g, 5) << " (expected 4)\n";
    cout << longestOnes(h, 0) << " (expected 2)\n";
    cout << longestOnes(p, 0) << " (expected 2)\n";
    return 0;
}
