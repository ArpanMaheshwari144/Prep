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

// ---- ARPAN KI APPROACH ----
//  ★ ye question DEKHNE/bolne me hard lagta, par soch nikaalne ke baad kaafi easy. (honest: is variation ko
//     interview me PEHLI baar dekh ke khud reach karna mushkil -- isliye ISE pehle se pata hona = edge.)
//  ★ CORE insight (copy-pen se nikla): kisi EK bar `i` ke upar kitna paani rukega?
//     = min(uske LEFT ka sabse ooncha, uske RIGHT ka sabse ooncha) - height[i].
//     (kyun min: chhoti wali side se paani BAH jaata -- {3,0,2} me beech pe 2 hi rukega, 3 nahi.)
//  ★ har bar ke liye left-max/right-max baar-baar scan = O(n²). isliye PEHLE SE nikaal ke rakho (prefix jaisa):
//     leftMax[i]  = 0..i tak ka max -> ek pass AAGE se.   leftMax[0]=height[0] (shuru me set, warna dikkat).
//     rightMax[i] = i..n-1 tak ka max -> ek pass PEECHE se. rightMax[n-1]=height[n-1] (shuru me set).
//  ★ final: ans += min(leftMax[i], rightMax[i]) - height[i]  (har i pe).  -> O(n) time, O(n) space.
//  ★ EDGE: empty array -> return 0.
//  example {2,0,3}: leftMax=2,2,3 · rightMax=3,3,3 -> (2-2)+(2-0)+(3-3) = 2. sahi.

// ---- ARPAN KI APPROACH (2nd = TWO-POINTER, O(1) space) ----
//  ★ upar wale O(n)-space (leftMax[]/rightMax[] arrays) ka OPTIMIZED version -- arrays ki zaroorat NAHI,
//     bas 2 variable leftMax/rightMax track karo. (ye version thoda tricky -- proof samajhna zaroori.)
//  setup: left=0, right=n-1. leftMax=height[0], rightMax=height[n-1] (shuru me set -- important).
//  loop while(left<right):
//     if (leftMax <= rightMax):   -- left side ki max chhoti/barabar
//         ★ KYUN SAFE: right me rightMax (>= leftMax) already khada = wall. to left bar ka paani sirf
//            leftMax se limit hoga (right se nahi) -> abhi PAKKA commit kar sakte, right pura dekhe bina.
//         leftMax = max(leftMax, height[left]);   (★ REFRESH PEHLE -- naya bar bada ho to wall ban jaye)
//         ans += leftMax - height[left];          (refresh ke baad add -> agar naya bar sabse ooncha to 0 add, negative nahi)
//         left++;
//     else:  (right side chhoti) -> SAME ulta: rightMax=max(..,height[right]); ans += rightMax-height[right]; right--;
//  ★ ORDER TRAP (yahi atka tha): REFRESH pehle, ADD baad me. warna naya bar leftMax se bada hone pe
//     "add-first" NEGATIVE de deta (ans += 4-6). max() se bada wala bacha rehta -> negative nahi aata.
//  ★ ek comparison (leftMax<=rightMax) se decide "konsi side process" -> har bar ek hi pointer chalta -> O(n) time, O(1) space.

#include <bits/stdc++.h>
using namespace std;

int trap(vector<int> &height)
{
    // Prefix Sum wala approach
    // vector<int> b = {4, 2, 0, 3, 2, 5};
    // if (height.size() == 0)
    //     return 0;

    // int ans = 0;
    // int n = height.size();

    // int leftMax[n];
    // int rightMax[n];

    // leftMax[0] = height[0];
    // for (int i = 1; i < n; i++)
    // {
    //     leftMax[i] = max(leftMax[i - 1], height[i]);
    // }

    // rightMax[n - 1] = height[n - 1];
    // for (int i = n - 2; i >= 0; i--)
    // {
    //     rightMax[i] = max(rightMax[i + 1], height[i]);
    // }

    // for (int i = 0; i < n; i++)
    // {
    //     ans += min(leftMax[i], rightMax[i]) - height[i];
    // }
    // return ans;

    // two pointer
    if (height.size() == 0)
        return 0;

    int ans = 0;
    int n = height.size();

    int left = 0, right = n - 1;
    int leftMax = height[0], rightMax = height[n - 1];
    while (left < right) // eg {4, 2, 0, 3, 2, 5}
    {
        // jo max CHHOTA hai (min wali side) wahi tak paani rukega -> wahi side process
        if (leftMax <= rightMax)
        {
            leftMax = max(leftMax, height[left]); // refresh PEHLE (naya bar bada ho to wall)
            ans += leftMax - height[left];        // phir is bar ka paani jodo
            left++;
        }
        else if (rightMax <= leftMax)
        {
            rightMax = max(rightMax, height[right]);
            ans += rightMax - height[right];
            right--;
        }
        // cout << ans << endl;                        // debug (band)
        // cout << leftMax << " " << rightMax << endl; // debug (band)
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
