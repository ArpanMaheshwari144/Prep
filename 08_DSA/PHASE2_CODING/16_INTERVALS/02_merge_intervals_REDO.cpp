// ============================================================
// MERGE INTERVALS   —   COLD REDO (kal [C] tha — aaj khud, blank se)
// ============================================================
// Diya: intervals ki list (har interval = [start, end]).
// OVERLAP karne wale intervals ko MERGE karo. merged list return karo.
//
// Example:
//   [[1,3],[2,6],[8,10],[15,18]] -> [[1,6],[8,10],[15,18]]   (1-3 aur 2-6 overlap -> 1-6)
//   [[1,4],[4,5]]                -> [[1,5]]                    (chhute hue bhi merge)
//
// SOCH khud (purana file mat dekh):
//   - overlap check karna easy ho, isliye pehle kuch ORDER me lana padega...
//   - phir ek-ek pe chal, "current" merged interval track kar.
//
// Tests (// expected):
//   [[1,3],[2,6],[8,10],[15,18]] -> [1,6] [8,10] [15,18]
//   [[1,4],[4,5]]                -> [1,5]
//   [[1,4],[2,3]]                -> [1,4]      (ek dusre ke andar)
//   [[1,4]]                      -> [1,4]
//   [[1,4],[5,6]]                -> [1,4] [5,6]  (no overlap)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> merge(vector<vector<int>> &intervals)
{
    sort(begin(intervals), end(intervals));
    vector<vector<int>> ans;
    vector<int> current = intervals[0];
    for (int i = 1; i < intervals.size(); i++)
    {
        if (intervals[i][0] <= current[1])
        {
            current[1] = max(current[1], intervals[i][1]);
        }
        else
        {
            ans.push_back(current);
            current = intervals[i];
        }
    }
    ans.push_back(current);
    return ans;
}

void print(vector<vector<int>> &res)
{
    for (auto &iv : res)
        cout << "[" << iv[0] << "," << iv[1] << "] ";
    cout << "\n";
}

int main()
{
    vector<vector<int>> a1 = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
    vector<vector<int>> a2 = {{1, 4}, {4, 5}};
    vector<vector<int>> a3 = {{1, 4}, {2, 3}};
    vector<vector<int>> a4 = {{1, 4}};
    vector<vector<int>> a5 = {{1, 4}, {5, 6}};

    auto r1 = merge(a1);
    print(r1); // expected: [1,6] [8,10] [15,18]
    auto r2 = merge(a2);
    print(r2); // expected: [1,5]
    auto r3 = merge(a3);
    print(r3); // expected: [1,4]
    auto r4 = merge(a4);
    print(r4); // expected: [1,4]
    auto r5 = merge(a5);
    print(r5); // expected: [1,4] [5,6]
    return 0;
}
