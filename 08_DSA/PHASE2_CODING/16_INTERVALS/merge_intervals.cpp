// ============================================================
// MERGE INTERVALS   (Medium)
// ============================================================
// intervals di (har ek [start, end]). OVERLAP karne wale merge kar do.
//
//   [[1,3],[2,6],[8,10],[15,18]] -> [[1,6],[8,10],[15,18]]   (1-3 & 2-6 overlap -> 1-6)
//   [[1,4],[4,5]]                -> [[1,5]]                   (4 pe touch -> merge)
//   [[1,4],[2,3]]                -> [[1,4]]                   (2-3 andar hai)
//   [[1,4]]                      -> [[1,4]]
//
// (no signal, no hint — tu khud soch ke solve kar. fasa to "madad" bol.)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ---- TERA kaam: yeh function bhar ----
vector<vector<int> > merge(vector<vector<int> > intervals)
{
    sort(begin(intervals), end(intervals));
    vector<vector<int> > ans;
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

// ---- helper (mat chhu) — print ----
void printIv(vector<vector<int> > res)
{
    cout << "[";
    for (int i = 0; i < res.size(); i++)
    {
        cout << "[" << res[i][0] << "," << res[i][1] << "]";
        if (i + 1 < res.size())
            cout << ",";
    }
    cout << "]" << endl;
}

int main()
{
    printIv(merge({{1, 3}, {2, 6}, {8, 10}, {15, 18}})); // expected [[1,6],[8,10],[15,18]]
    printIv(merge({{1, 4}, {4, 5}}));                    // expected [[1,5]]
    printIv(merge({{1, 4}, {2, 3}}));                    // expected [[1,4]]
    printIv(merge({{1, 4}}));                            // expected [[1,4]]
    printIv(merge({{1, 4}, {0, 4}}));                    // expected [[0,4]]
    printIv(merge({{1, 4}, {5, 6}}));                    // expected [[1,4],[5,6]]  (no overlap)
    return 0;
}
