// ============================================================
// MAXIMUM SUBARRAY — (LeetCode 53, Easy/Medium)   [NAYA METHOD: struggle-first, NO peek]
// ============================================================
// ek int array diya. aisा CONTIGUOUS subarray dhoondho jiska SUM sabse BADA ho.
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
// ---- ARPAN KI APPROACH (basic KADANE -- poore family ka BAAP) ----
//  ★ AHA: har element pe ek SAWAAL -- "pichhla sum saath le jaaun, ya YAHIN se naya shuru karun?"
//     -> pichhla running sum NEGATIVE hai? wo aage sirf BOJH hai (ghatata hai) -> PHENK do (sum=0), fresh shuru.
//     -> positive hai? saath le chalo (faayda dega).  isi ek choice se poora array ek baar me nikal jaata.
//  ★ code:  sum += nums[i];   max_sum = max(max_sum, sum);   if(sum < 0) sum = 0;      (O(n) / O(1))
//     ★★ TRAP: max_sum ka update RESET se PEHLE -- warna all-negative [-1] me answer miss hota
//        (isiliye max_sum = INT_MIN se start, 0 se nahi).
//  ★ SUM me negative-FLIP nahi hota -> ek running value kaafi.
//     (Product me flip hota [neg×neg=+] -> wahan MAX + MIN dono track, 3-candidate -- 02_max_product.cpp)
//  ★ FAMILY: 53 (sum, 1 value) · 152 (product, 3-cand max+min) · 1749 (abs-sum, 3-cand). poora why = 04_max_abs_sum.cpp
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
