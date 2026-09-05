// ============================================================
// SORT A NEARLY-SORTED (k-sorted) ARRAY       (REDO_1 — blank, spaced)
// ============================================================
// array me har element apni SAHI (sorted) position se max k DOOR hai.
// isi property ka faayda utha ke sort karo -> O(n log k).
//   arr = [6,5,3,2,8,10,9],  k=3  -> [2,3,5,6,8,9,10]
//
// ---- TEST CASES (arr, k -> expected sorted) ----
//   [6,5,3,2,8,10,9],          k=3  -> [2,3,5,6,8,9,10]
//   [10,9,8,7,4,70,60,50],     k=4  -> [4,7,8,9,10,50,60,70]
//   [1,4,5,2,3,7,8,6,10,9],    k=2  -> [1,2,3,4,5,6,7,8,9,10]
//   [2,1],                     k=1  -> [1,2]
//   [1],                       k=0  -> [1]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> sortKSorted(vector<int> &arr, int k)
{
    vector<int> ans;
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < arr.size(); i++)
    {
        pq.push(arr[i]);
        if (pq.size() > k + 1)
        {
            int tp = pq.top();
            pq.pop();
            ans.push_back(tp);
        }
    }
    while (!pq.empty())
    {
        int tp = pq.top();
        pq.pop();
        ans.push_back(tp);
    }
    return ans;
}

// ---- test helper (ise mat chhed) ----
void check(vector<int> got, vector<int> exp, const string &label)
{
    bool ok = (got == exp);
    cout << label << " -> " << (ok ? "PASS" : "*** FAIL ***") << "  got: ";
    for (int x : got)
        cout << x << " ";
    if (!ok)
    {
        cout << "| exp: ";
        for (int x : exp)
            cout << x << " ";
    }
    cout << "\n";
}

int main()
{
    vector<int> a1 = {6, 5, 3, 2, 8, 10, 9};
    check(sortKSorted(a1, 3), {2, 3, 5, 6, 8, 9, 10}, "case1");

    vector<int> a2 = {10, 9, 8, 7, 4, 70, 60, 50};
    check(sortKSorted(a2, 4), {4, 7, 8, 9, 10, 50, 60, 70}, "case2");

    vector<int> a3 = {1, 4, 5, 2, 3, 7, 8, 6, 10, 9};
    check(sortKSorted(a3, 2), {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, "case3");

    vector<int> a4 = {2, 1};
    check(sortKSorted(a4, 1), {1, 2}, "case4");

    vector<int> a5 = {1};
    check(sortKSorted(a5, 0), {1}, "case5");
    return 0;
}
