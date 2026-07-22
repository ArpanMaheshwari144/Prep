// ============================================================
// MAXIMUM SUBARRAY — (LeetCode 53, Easy/Medium)   [NAYA METHOD: struggle-first, NO peek]
// ============================================================
// ek int array diya. aisa CONTIGUOUS subarray dhoondho jiska SUM sabse BADA ho.
// wo max sum lautao. (kam-se-kam ek element)
//
//   [-2,1,-3,4,-1,2,1,-5,4]  -> 6    ([4,-1,2,1])
//   [1]                      -> 1
//   [5,4,-1,7,8]             -> 23
//   [-1]                     -> -1
//   [-2,-1]                  -> -1
//
// ★★ NAYA METHOD (aaj se) — ye padh:
//   1. PEEK MAT KAR. net/editorial abhi mat khol.
//   2. Chhota example haath se TRACE kar (jaise [-2,1,-3,4,-1]) -> running sum chala,
//      dekh sum negative hone pe kya karna smart hoga. 15-20 min ASLI struggle.
//   3. Behavior mil jaye -> code likh.
//   4. SIRF sachchi struggle ke BAAD atke -> tab editorial (last resort, no guilt).
//   5. Kuch din baad COLD REDO -> tab pata chalega tera hua ya nahi.
// ============================================================
// ---- ARPAN KI APPROACH ----
//  ★ TRICK: pichhla running sum NEGATIVE = aage sirf bojh -> PHENK do (sum=0), fresh shuru. bas yahi.
//  ★ 3 line:
//     sum += nums[i]
//     max_sum = max(max_sum, sum)     // update RESET se PEHLE (max_sum=INT_MIN start -> all-negative [-1] safe)
//     if (sum < 0) sum = 0
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int maxSubArray(vector<int> &nums)
{
    int max_sum = INT_MIN; // [-2,1,-3,4,-1,2,1,-5,4]  -> 6    ([4,-1,2,1])
    int sum = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i];
        max_sum = max(max_sum, sum);
        if (sum < 0)
        {
            sum = 0;
        }
    }
    return max_sum;
}

int main()
{
    vector<int> a = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    vector<int> b = {1};
    vector<int> c = {5, 4, -1, 7, 8};
    vector<int> d = {-1};
    vector<int> e = {-2, -1};

    cout << maxSubArray(a) << " (expected 6)\n";
    cout << maxSubArray(b) << " (expected 1)\n";
    cout << maxSubArray(c) << " (expected 23)\n";
    cout << maxSubArray(d) << " (expected -1)\n";
    cout << maxSubArray(e) << " (expected -1)\n";
    return 0;
}
