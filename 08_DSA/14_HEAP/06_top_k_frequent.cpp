// ============================================================
// TOP K FREQUENT ELEMENTS     (LC-347, HEAP)
// ============================================================
// nums[] + k diya. Wo k elements return karo jo SABSE ZYADA baar aate hain.
//   (order matter nahi -- koi bhi order chalega)
//
//   [1,1,1,2,2,3], k=2  -> [1,2]   (1 aaya 3 baar, 2 aaya 2 baar -> top 2)
//
// ---- TEST (nums, k -> expected, order-independent) ----
//   [1,1,1,2,2,3],   k=2  -> {1,2}
//   [1],             k=1  -> {1}
//   [4,4,4,6,6,2,2,2],k=2  -> {4,2}      (4->3, 2->3, 6->2)
//   [5,5,5,5],       k=1  -> {5}
//   [7,8,9],         k=3  -> {7,8,9}     (sab freq 1)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> topKFrequent(vector<int> &nums, int k)
{
    // STEP 1: har number ki frequency gino
    unordered_map<int, int> mp;
    for (int i = 0; i < nums.size(); i++)
    {
        mp[nums[i]]++;
    }

    // STEP 2: min-heap {freq, num} -- size k se bada hote hi chhoti-freq pop
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;
    for (auto &it : mp)
    {
        minHeap.push({it.second, it.first}); // {freq, num} -> heap freq pe sort
        if (minHeap.size() > k)              // sirf top-k freq wale rakhne hain
        {
            minHeap.pop(); // sabse chhoti freq nikal do
        }
    }

    // STEP 3: heap me bache hue = top-k freq -> answer
    vector<int> ans;
    while (!minHeap.empty())
    {
        int tp = minHeap.top().second; // .second = actual number
        minHeap.pop();
        ans.push_back(tp);
    }
    return ans;
}

// ---- test helper (ise mat chhed) -- order-independent compare ----
void check(vector<int> got, vector<int> exp, const string &label)
{
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    bool ok = (got == exp);
    cout << label << " -> got {";
    for (int x : got)
        cout << x << " ";
    cout << "} | exp {";
    for (int x : exp)
        cout << x << " ";
    cout << "}" << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {1, 1, 1, 2, 2, 3};
    check(topKFrequent(a1, 2), {1, 2}, "case1");
    vector<int> a2 = {1};
    check(topKFrequent(a2, 1), {1}, "case2");
    vector<int> a3 = {4, 4, 4, 6, 6, 2, 2, 2};
    check(topKFrequent(a3, 2), {4, 2}, "case3");
    vector<int> a4 = {5, 5, 5, 5};
    check(topKFrequent(a4, 1), {5}, "case4");
    vector<int> a5 = {7, 8, 9};
    check(topKFrequent(a5, 3), {7, 8, 9}, "case5");
    return 0;
}
