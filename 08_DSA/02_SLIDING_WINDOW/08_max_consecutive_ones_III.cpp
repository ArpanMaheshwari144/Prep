// ============================================================
// MAX CONSECUTIVE ONES III — Sliding Window  (random self-test)
// ============================================================
// binary array nums (0/1) aur int k diya. tu ZYADA-SE-ZYADA k zeros ko FLIP (0->1) kar sakta.
// flip karke jo SABSE LAMBI consecutive 1s ki length ban sakti -> wo lautao.
//   nums=[1,1,1,0,0,0,1,1,1,1,0], k=2  -> 6
//     (index 5..10 window me 2 zeros flip -> 0,1,1,1,1,0 me... best window length 6)
//
// (approach LIKHA NAHI -- socho: window [left..right] chalao, window me kitne ZEROS hain count karo.
//  zeros > k ho jaaye -> left ko aage khisko jab tak zeros <= k. max window-length track karo.)
//
// Tests (// expected):
//   [1,1,1,0,0,0,1,1,1,1,0], k=2                       -> 6
//   [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], k=3       -> 10
//   [0,0,0,0], k=0                                      -> 0
//   [1,1,1,1], k=0                                      -> 4
//   [1,0,1,0,1], k=1                                    -> 3
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int longestOnes(vector<int> &nums, int k)
{
    int ans = INT_MIN;
    int zeroCount = 0;
    int i = 0, j = 0;
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

        // ---- ★ PATTERN / LEARNING (khud debug karke nikaala) ----
        // yahan if(zeroCount == k) MAT lagao -> wo FALTU hai. kyun?
        //   upar wale  while(zeroCount > k)  shrink ke BAAD window [i..j] me zeroCount HAMESHA <= k
        //   -> window yahan pahunchte hi HAMESHA VALID hoti -> invalid ho hi nahi sakti.
        //   -> to ans bina kisi condition ke (UNCONDITIONAL) update karo -> answer apne aap sahi.
        // (galti thi: pehle if(zeroCount==k) laga diya tha -> sirf "exactly k" window ginta ->
        //  k=0 / k>zeros wale case toot gaye. condition hatao -> saara mess gayab -> 7/7 clean.)
        //
        // ★ BROAD PATTERN (variable sliding window):
        //     expand -> shrink-till-valid -> ans UNCONDITIONAL (record, no if).
        ans = max(ans, j - i + 1);
        j++;
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
