// ============================================================
// TRAPPING RAIN WATER — (LeetCode 42)   REDO_2 (khaali page se)
// ============================================================
// PEHLE YE (30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// n non-negative integers ka array height[] diya -- har bar ki WIDTH 1 hai.
// ye ek elevation-map (buildings ki heights) hai. baarish ke baad kitna PAANI
// in bars ke beech TRAP (fasa) ho sakta hai -- total units nikaalo.
//
// Tests (height -> expected):
//   [0,1,0,2,1,0,1,3,2,1,2,1]   -> 6
//   [4,2,0,3,2,5]               -> 9
//   [1,2,3,4,5]                 -> 0
//   [5,4,3,2,1]                 -> 0
//   [3,0,3]                     -> 3
//   []                          -> 0
//   [2,0,2]                     -> 2
//   [5]                         -> 0
//   [5,5]                       -> 0
//   [4,2,3]                     -> 1
//   [2,1,0,1,3]                 -> 4
//   [5,0,0,0,5]                 -> 15
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int trap(vector<int> &height)
{
    int n = height.size();
    int left = 0, right = n - 1, leftMax = 0, rightMax = 0;
    int ans = 0;
    while (left < right)
    {
        if (height[left] <= height[right])
        {
            if (leftMax > height[left])
            {
                ans += leftMax - height[left];
            }
            else
            {
                leftMax = height[left];
            }
            left++;
        }
        else
        {
            if (rightMax > height[right])
            {
                ans += rightMax - height[right];
            }
            else
            {
                rightMax = height[right];
            }
            right--;
        }
    }
    return ans;
}

int main()
{
    vector<int> a = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    vector<int> b = {4, 2, 0, 3, 2, 5};
    vector<int> c = {1, 2, 3, 4, 5};
    vector<int> d = {5, 4, 3, 2, 1};
    vector<int> e = {3, 0, 3};
    vector<int> f = {};
    vector<int> g = {2, 0, 2};
    vector<int> h = {5};
    vector<int> i = {5, 5};
    vector<int> j = {4, 2, 3};
    vector<int> k = {2, 1, 0, 1, 3};
    vector<int> l = {5, 0, 0, 0, 5};

    cout << trap(a) << " (expected 6)\n";
    cout << trap(b) << " (expected 9)\n";
    cout << trap(c) << " (expected 0)\n";
    cout << trap(d) << " (expected 0)\n";
    cout << trap(e) << " (expected 3)\n";
    cout << trap(f) << " (expected 0)\n";
    cout << trap(g) << " (expected 2)\n";
    cout << trap(h) << " (expected 0)\n";
    cout << trap(i) << " (expected 0)\n";
    cout << trap(j) << " (expected 1)\n";
    cout << trap(k) << " (expected 4)\n";
    cout << trap(l) << " (expected 15)\n";
    return 0;
}
