// ============================================================
// TRAPPING RAIN WATER — (LeetCode 42)   (fresh, classic)
// ============================================================
// n non-negative integers ka array height[] diya -- har bar ki WIDTH 1 hai.
// ye ek elevation-map (buildings ki heights) hai. baarish ke baad kitna PAANI
// in bars ke beech TRAP (fasa) ho sakta hai -- total units nikaalo.
//
//   height = [0,1,0,2,1,0,1,3,2,1,2,1]   -> 6
//        (visual: gaddho me paani bharta hai; kul 6 unit paani ruk jaata)
//   height = [4,2,0,3,2,5]               -> 9
//   height = [1,2,3,4,5]                 -> 0    (sirf chadhaai -> koi gaddha nahi)
//   height = [5,4,3,2,1]                 -> 0    (sirf dhalaan -> koi gaddha nahi)
//   height = [3,0,3]                     -> 3    (beech ka gaddha 3 bharta)
//   height = []                          -> 0
//   height = [2,0,2]                     -> 2
//
// (soch: kisi ek bar ke UPAR kitna paani ruk sakta -- wo kis cheez pe depend karta?)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int trap(vector<int> &height)
{
    int n = height.size();
    int left = 0, right = n - 1;
    int leftMax = 0, rightMax = 0, ans = 0;
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
            if (height[right] < height[left])
            {
                if (rightMax > height[right])
                {
                    ans += rightMax - height[right];
                }
                else
                {
                    rightMax = height[right];
                }
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

    cout << trap(a) << " (expected 6)\n";
    cout << trap(b) << " (expected 9)\n";
    cout << trap(c) << " (expected 0)\n";
    cout << trap(d) << " (expected 0)\n";
    cout << trap(e) << " (expected 3)\n";
    cout << trap(f) << " (expected 0)\n";
    cout << trap(g) << " (expected 2)\n";
    return 0;
}
