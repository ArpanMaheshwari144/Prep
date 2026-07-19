// ============================================================
// LARGEST RECTANGLE IN HISTOGRAM — (LeetCode 84)   (fresh, classic)
// ============================================================
// heights[] diya -- har bar ki WIDTH 1 hai, height = heights[i].
// in bars se banne wale histogram me SABSE BADA rectangle ka AREA nikaalo.
// (rectangle lagatar bars pe banega; uski height = un bars me se SABSE CHHOTI.)
//
//   heights = [2,1,5,6,2,3]   -> 10   (bars 5 aur 6 -> min-height 5, width 2 -> 5*2=10)
//   heights = [2,4]           -> 4    (bar 4 akela: 4*1=4 ; ya dono: min 2 * width 2 = 4)
//   heights = [1,1]           -> 2    (min 1 * width 2)
//   heights = [6]             -> 6
//   heights = [0]             -> 0
//   heights = [2,1,2]         -> 3    (poora: min 1 * width 3 = 3)
//   heights = [4,2,0,3,2,5]   -> 6    (bars 3,2,5 nahi... 2*3? -> actually 6: bar 5 se? trace karo)
//
// ============================================================

// ---- ARPAN KI APPROACH ----
//  ★ ye question = PREV-SMALLER + NEXT-SMALLER ka code reuse (dono building block pehle bana liye). "hard" lagta
//     par medium hai -- bas 2-3 trick ek saath.
//  ★ IDEA: har bar `i` ko uski OWN height pe rectangle bana ke failao -- left me prev-smaller tak, right me next-smaller tak
//     (dono usse chhote, wahi rok dete). us rectangle ki height = heights[i], width = beech ke bars ki ginti.
//     example [2,1,5,6,2,3]: bar 5 (idx 2) -> left prev-smaller idx 1, right next-smaller idx 4 -> beech me idx 2,3 -> width 2 -> 5*2 = 10.
//  ★ TRICK 1: is baar prev/next-smaller me VALUE nahi, INDEX store karo (width ke liye index chahiye).
//  ★ TRICK 2 (yahi atka): next-smaller ke "koi chhota nahi" case me sentinel = n (array ke aage), NA ki -1.
//     (-1 rakha to width = ns-ps-1 NEGATIVE aa jaati -> [2,4] jaise fail. prev-smaller ka "none" = -1 sahi hai, left boundary.)
//  ★ FORMULA: har i pe -> width = nS[i] - pS[i] - 1;  area = heights[i] * width;  ans = max(ans, area).
//     (-1 isliye: dono boundary bar khud rectangle me nahi aate, unke beech wale aate.)

#include <bits/stdc++.h>
using namespace std;

vector<int> prevSmaller(vector<int> &nums)
{
    vector<int> ans(nums.size(), -1);
    stack<int> st;

    for (int i = nums.size() - 1; i >= 0; i--)
    {
        while (!st.empty() && nums[st.top()] > nums[i])
        {
            int topp = st.top();
            st.pop();
            ans[topp] = i;
        }
        st.push(i);
    }
    return ans;
}

vector<int> nextSmaller(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(nums.size(), n);
    stack<int> st;

    for (int i = 0; i < nums.size(); i++)
    {
        while (!st.empty() && nums[st.top()] > nums[i])
        {
            int topp = st.top();
            st.pop();
            ans[topp] = i;
        }
        st.push(i);
    }
    return ans;
}

int largestRectangleArea(vector<int> &heights)
{
    vector<int> pS = prevSmaller(heights);
    vector<int> nS = nextSmaller(heights);

    // for (int i = 0; i < pS.size(); i++)
    // {
    //     cout << pS[i] << " ";
    // }
    // cout << endl;

    // for (int i = 0; i < nS.size(); i++)
    // {
    //     cout << nS[i] << " ";
    // }
    // cout << endl;

    int ans = INT_MIN;
    for (int i = 0; i < heights.size(); i++)
    {

        int width = (nS[i] - pS[i] - 1);
        // cout << heights[i] << " " << width << endl;
        ans = max(ans, (heights[i] * width));
    }
    return ans;
}

int main()
{
    vector<int> a = {2, 1, 5, 6, 2, 3};
    vector<int> b = {2, 4};
    vector<int> c = {1, 1};
    vector<int> d = {6};
    vector<int> e = {0};
    vector<int> f = {2, 1, 2};
    vector<int> g = {4, 2, 0, 3, 2, 5};

    cout << largestRectangleArea(a) << " (expected 10)\n";
    cout << largestRectangleArea(b) << " (expected 4)\n";
    cout << largestRectangleArea(c) << " (expected 2)\n";
    cout << largestRectangleArea(d) << " (expected 6)\n";
    cout << largestRectangleArea(e) << " (expected 0)\n";
    cout << largestRectangleArea(f) << " (expected 3)\n";
    cout << largestRectangleArea(g) << " (expected 6)\n";
    return 0;
}
