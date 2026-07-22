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
//  ★★ MENTAL MODEL (ek line me pura): HAR height pe KHADE ho -> dono taraf ka NEAREST-SMALLER (prev + next) dhoondo
//      -> width = next - prev - 1 -> area = height * width -> sab me se MAX. bas.
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
//
//  ========== WIDTH FORMULA "NS - PS - 1" KYUN AATA (derivation, 19-Jul) ==========
//   1. PS (prev-smaller) aur NS (next-smaller) dono CHHOTE bar hain -> ye rectangle me NAHI aate (inpe ruk gaya).
//      -> rectangle ka PEHLA bar = PS + 1   ·   rectangle ka AAKHRI bar = NS - 1.
//      ★ DIRECTION: dono boundary se ANDAR ki taraf ek kadam -> PS+1 (left boundary se right), NS-1 (right boundary se left).
//        (ns+1 / ps-1 NAHI -- wo to bahar chala jaata; hamesha andar.)  e.g. PS=1,NS=4 -> bar 2 aur 3 = buildings 5,6.
//   2. kisi range me kitne bar? -> BASIC count formula:  count = last - first + 1.
//      (★ +1 kyunki DONO ends count hote. e.g. 2 se 5 tak = 2,3,4,5 = 4 = 5-2+1.)
//   3. yahan lagao:  count = (NS - 1) - (PS + 1) + 1
//                          = NS - 1 - PS - 1 + 1
//                          = NS - PS - 1        (★ constants: -1 -1 +1 = -1 -> ek hi -1 bacha)
//   4. example building 5 (PS=1, NS=4): first=PS+1=2, last=NS-1=3 -> count = 3-2+1 = 2 = 4-1-1 -> 5*2 = 10.
//   ★ NOTE: -1 SIRF EK baar (NS-PS-1). "dono boundary hatane ko do -1" GALAT ->
//           building 6 (NS=4, PS=2): 4-2-1 = 1 sahi (6*1=6); 4-2-1-1 = 0 galat.
//  ================================================================================
//
//  ========== VISUAL (aankhon se — 20-Jul, jo click karaata) ==========
//   heights = [2,1,5,6,2,3]
//        c0   c1   c2   c3   c4   c5
//   h6                   #
//   h5              #    #
//   h4              #    #
//   h3              #    #         #
//   h2    #         #    #    #    #
//   h1    #    #    #    #    #    #
//
//   index :  0    1    2    3    4    5
//   height:  2    1    5    6    2    3
//   PS    : -1   -1    1    2    1    4     (baayein pehla CHHOTA bar ka index; koi nahi -> -1)
//   NS    :  1    6    4    4    6    6     (dayein pehla CHHOTA bar ka index; koi nahi -> n=6)
//   width :  1    6    2    1    4    1     (NS - PS - 1)
//   area  :  2    6   10    6    8    3     -> MAX = 10
//
//   bar 5 (idx2) pe dekh -- PS=1 (h=1), NS=4 (h=2). rectangle DONO stoppers ke BEECH baithta:
//        PS=1 |  c2    c3  | NS=4        -> sirf c2,c3 (dono >=5) -> 2 chauda
//        (1)  | (5)  (6)  | (2)
//   first = PS+1 = 2,  last = NS-1 = 3  -> count = 3-2+1 = 2 = NS-PS-1. (stoppers KHUD rectangle me nahi aate.)
//  ====================================================================
//
//  ---- 19-Jul brainstorm se 2 aur samajh (yaad rakhne wali) ----
//  ★ HAR bar apna OWN rectangle banata -> sabka area nikaal ke MAX lete. (bar 5 -> 5*2=10 · bar 6 -> 6*1=6 · max=10.)
//     ek rectangle "us bar ka" hai jiski height us range me sabse chhoti (jo define karti). taller bars uske upar se guzarte.
//  ★ "-1" ka RULE (kab lagta): boundary bar rectangle me SHAAMIL hai ya nahi?
//       boundary = SMALLER element (next/prev-smaller) -> wo bar rectangle me NAHI -> width = ns-ps-1 (-1 LAGTA, beech ka count).
//       agar koi boundary INCLUDE hoti -> us taraf -1 NAHI (e.g. seedha ns-ps type).
//  ★ NEXT-GREATER se kyun NAHI hoga (ye socha tha): rectangle taller bar ko INCLUDE karta, SHORTER pe RUKTA.
//     next-greater bar 5 ko 6 (taller) pe rok deta -> width sirf 1 -> 5*1=5 GALAT (10 chahiye). isliye SMALLER boundary hi.
//     (next-greater tab kaam aata jab problem me BADA element boundary ho, chhota nahi.)
//
//  ★ next-smaller me "n" sentinel ka FAYDA kyun ([2,4] se samjho):
//     [2,4] me kisi bar ka next-smaller HAI HI NAHI (right me kuch chhota nahi). plain "next-smaller" problem me
//     iska matlab "-1 = koi jawab nahi." PAR histogram me next-smaller = RIGHT BOUNDARY -> "right me koi chhota nahi"
//     ka matlab hai bar array ke RIGHT-EDGE tak failta -> wo edge index = n (last ke ek aage).
//     n=2 (sahi): bar2(idx0) pS=-1,nS=2 -> width 2-(-1)-1=2 -> 2*2=4 · bar4(idx1) pS=0,nS=2 -> 2-0-1=1 -> 4*1=4 -> max 4 ✓
//     -1 (galat): nS=-1 -> width -1-(-1)-1=-1 (negative) -> area -2/-8 -> max -2 ✗.
//     => n = "bar right-edge tak failta" -> width positive banti. (-1 plain-problem ke liye theek, boundary ke liye n.)

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
