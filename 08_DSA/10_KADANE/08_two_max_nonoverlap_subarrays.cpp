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

    for (int i = 0; i < nums.size() - 2; i++)
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
    return 0;
}
