// ============================================================
// SQUARES OF A SORTED ARRAY — Two Pointer  (fresh)
// ============================================================
// sorted array (negatives bhi ho sakte) diya. har element ka SQUARE karke
// SORTED order me return karo. O(n) me (dobara sort nahi).
//   [-4,-1,0,3,10] -> [0,1,9,16,100]
//   (kyun tricky: -4 ka square 16 bada hai -> negatives ka square order ULTA ho jaata)
//
// ---- ARPAN KI APPROACH ----
//  ans array size n, PEECHE se bharo (k = n-1). 2 pointer left=0, right=end.
//  sabse bada square ya left-chhor (bada negative) ya right-chhor pe hota.
//  left^2 <= right^2 -> ans[k]=right^2, right--; warna ans[k]=left^2, left++. har baar k--.
//
// Tests (// expected):
//   [-4,-1,0,3,10]     -> 0 1 9 16 100
//   [-7,-3,2,3,11]     -> 4 9 9 49 121
//   [1,2,3]            -> 1 4 9
//   [-5,-3,-1]         -> 1 9 25
//   [0]                -> 0
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> sortedSquares(vector<int> &nums)
{
    vector<int> ans(nums.size(), -1); // //   [-4,-1,0,3,10]     -> 0 1 9 16 100
    int left = 0, right = nums.size() - 1, k = nums.size() - 1;
    while (left <= right)
    {
        if (nums[left] * nums[left] <= nums[right] * nums[right])
        {
            ans[k] = nums[right] * nums[right];
            right--;
            k--;
        }
        else
        {
            ans[k] = nums[left] * nums[left];
            left++;
            k--;
        }
    }
    return ans;
}

int main()
{
    vector<int> a = {-4, -1, 0, 3, 10};
    vector<int> b = {-7, -3, 2, 3, 11};
    vector<int> c = {1, 2, 3};
    vector<int> d = {-5, -3, -1};
    vector<int> e = {0};

    auto p = [](vector<int> r)
    { for (int x : r) cout << x << " "; cout << "\n"; };

    p(sortedSquares(a)); // 0 1 9 16 100
    p(sortedSquares(b)); // 4 9 9 49 121
    p(sortedSquares(c)); // 1 4 9
    p(sortedSquares(d)); // 1 9 25
    p(sortedSquares(e)); // 0
    return 0;
}
