// ============================================================
// TWO MAX-SUM NON-OVERLAPPING SUBARRAYS
// ============================================================
// ek int array (size N). usme se DO subarrays chuno:
//   - dono CONTIGUOUS, dono NON-EMPTY, dono NON-OVERLAPPING (index share nahi).
// aisi 2 subarrays jinke SUMS ka JOD max ho. wo max total sum lautao.
//
//   [2,1,-99,3,3]     -> 9     ([2,1]=3 · [3,3]=6)
//   [1,2,-7,8,6,-4]   -> 17    ([1,2]=3 · [8,6]=14)
//   [3,3,-100,6,6]    -> 18    ([3,3]=6 · [6,6]=12)
//   [1,2,3,4]         -> 10
//   [5,-2,5]          -> 10    ([5] · [5])
// ============================================================
// ---- APPROACH ----  (bestLeft + bestRight -> WALL. sab Kadane ka per-index STORE.)
//  SOCH: 2 non-overlapping tukde -> beech me ek DEEWAAR (wall) socho. 1 subarray poori LEFT,
//        doosra poori RIGHT -> overlap KABHI nahi. har wall pe: left-ka-best + right-ka-best -> MAX.
//
//  4 building block (sab Kadane, upar-neeche STORE):
//    1. maxEndingAtForLeft  = har index i pe "yahin-KHATAM best"  (Kadane, L->R).
//    2. bestLeftPrefix[i]   = 0..i me best   = #1 ka RUNNING-MAX ("ab tak ka max", kabhi neeche nahi).
//    3. maxEndingAtForRight = har index pe "yahin-SHURU best"     (Kadane, R->L).
//    4. bestRightPrefix[i]  = i..n-1 me best = #3 ka running-max R->L.   (2 aur 4 = MIRROR)
//
//  WALL (jodna):  for i=0..n-2 ->  ans = max(ans, bestLeft[i] + bestRight[i+1]).
//    ★ i+1 (non-overlap ki JAAN): left ne index i TAK le liya -> right ko i+1 SE -> dono kabhi nahi milte.
//    ★ loop i=0..n-2 (i<n-1): aakhri split (right = akela last element) MISS mat karo. [-5,10,10] -> 20.
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> maxEndingAtForLeft(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    ans[0] = nums[0];
    for (int i = 1; i < n; i++)
    {
        ans[i] = max(nums[i], ans[i - 1] + nums[i]);
    }
    return ans;
}

vector<int> bestLeftPrefix(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    vector<int> temp = maxEndingAtForLeft(nums);
    ans[0] = temp[0];
    for (int i = 1; i < temp.size(); i++)
    {
        ans[i] = max(ans[i - 1], temp[i]);
    }
    return ans;
}

vector<int> maxEndingAtForRight(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    ans[n - 1] = nums[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        ans[i] = max(nums[i], ans[i + 1] + nums[i]);
    }
    return ans;
}

vector<int> bestRightPrefix(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    vector<int> temp = maxEndingAtForRight(nums);
    ans[n - 1] = temp[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        ans[i] = max(ans[i + 1], temp[i]);
    }
    return ans;
}

int maxTwoNonOverlap(vector<int> &nums)
{
    int ans = INT_MIN;
    vector<int> bestLeft = bestLeftPrefix(nums);
    vector<int> bestRight = bestRightPrefix(nums);

    for (int i = 0; i < nums.size() - 1; i++)
    {
        ans = max(ans, bestLeft[i] + bestRight[i + 1]);
    }
    return ans;
}

int main()
{
    vector<int> a = {2, 1, -99, 3, 3};
    vector<int> b = {1, 2, -7, 8, 6, -4};
    vector<int> c = {3, 3, -100, 6, 6};
    vector<int> d = {1, 2, 3, 4};
    vector<int> e = {5, -2, 5};

    cout << maxTwoNonOverlap(a) << " (expected 9)\n";
    cout << maxTwoNonOverlap(b) << " (expected 17)\n";
    cout << maxTwoNonOverlap(c) << " (expected 18)\n";
    cout << maxTwoNonOverlap(d) << " (expected 10)\n";
    cout << maxTwoNonOverlap(e) << " (expected 10)\n";
    vector<int> f = {-5, 10, 10};
    cout << maxTwoNonOverlap(f) << " (expected 20)\n";   // <-- ye wala dhyaan se (dono 10 alag subarray)
    return 0;
}
