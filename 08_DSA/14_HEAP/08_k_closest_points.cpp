// ============================================================
// K CLOSEST POINTS TO ORIGIN — (LeetCode 973, Medium)   [HEAP #8 · max-heap of size k]
// ============================================================
// points[] me har element ek point [x,y]. Origin O(0,0) ke sabse
// PAAS wale K points lautao. "paas" = seedhi doori = x*x + y*y
// (√ ki zaroorat nahi — sirf compare karna hai). Order koi bhi.
//
//   [[1,3],[-2,2]] , k=1              -> [[-2,2]]
//   [[3,3],[5,-1],[-2,4]] , k=2       -> [[3,3],[-2,4]]
//   [[0,1],[1,0]] , k=2               -> [[0,1],[1,0]]   (dono)
//   [[1,1],[2,2],[3,3]] , k=1         -> [[1,1]]
//   [[5,5]] , k=1                     -> [[5,5]]
//   [[-5,4],[1,1],[0,3]] , k=2        -> [[1,1],[0,3]]
//
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
vector<vector<int>> kClosest(vector<vector<int>> &points, int k)
{
    priority_queue<pair<int, vector<int>>> maxHeap;
    for (int i = 0; i < points.size(); i++)
    {
        maxHeap.push({{points[i][0] * points[i][0] + points[i][1] * points[i][1]}, {points[i]}});
        if (maxHeap.size() > k)
        {
            maxHeap.pop();
        }
    }

    vector<vector<int>> ans;
    while (maxHeap.size() > 0)
    {
        auto it = maxHeap.top();
        maxHeap.pop();
        ans.push_back(it.second);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
static long long d2(const vector<int> &p) { return 1LL * p[0] * p[0] + 1LL * p[1] * p[1]; }

static bool sameSet(vector<vector<int>> a, vector<vector<int>> b)
{
    if (a.size() != b.size())
        return false;
    auto cmp = [](const vector<int> &x, const vector<int> &y)
    {
        if (d2(x) != d2(y))
            return d2(x) < d2(y);
        if (x[0] != y[0])
            return x[0] < y[0];
        return x[1] < y[1];
    };
    sort(a.begin(), a.end(), cmp);
    sort(b.begin(), b.end(), cmp);
    return a == b;
}

static void check(int t, vector<vector<int>> pts, int k, vector<vector<int>> exp)
{
    vector<vector<int>> got = kClosest(pts, k);
    cout << "T" << t << ": " << (sameSet(got, exp) ? "PASS" : "FAIL") << "\n";
}

int main()
{
    check(1, {{1, 3}, {-2, 2}}, 1, {{-2, 2}});
    check(2, {{3, 3}, {5, -1}, {-2, 4}}, 2, {{3, 3}, {-2, 4}});
    check(3, {{0, 1}, {1, 0}}, 2, {{0, 1}, {1, 0}});
    check(4, {{1, 1}, {2, 2}, {3, 3}}, 1, {{1, 1}});
    check(5, {{5, 5}}, 1, {{5, 5}});
    check(6, {{-5, 4}, {1, 1}, {0, 3}}, 2, {{1, 1}, {0, 3}});
    return 0;
}
