// ============================================================
// 3 SUM   (LC-15 — REDO_1, khaali page)
// ============================================================
// Array diya. SAARE UNIQUE triplets [a,b,c] dhoondho jinka sum = 0.
// Ek index ek triplet me ek hi baar. Duplicate triplets nahi chahiye.
// Triplets ka order aur triplet ke andar ka order koi bhi chalega.
//
//   [-1,0,1,2,-1,-4] -> [-1,-1,2] [-1,0,1]
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   [-1,0,1,2,-1,-4]                    -> [-1,-1,2] [-1,0,1]
//   [0,1,1]                             -> (none)
//   [0,0,0]                             -> [0,0,0]
//   [-2,0,1,1,2]                        -> [-2,0,2] [-2,1,1]
//   [0,0,0,0]                           -> [0,0,0]          (ek hi baar)
//   []                                  -> (none)
//   [1,2]                               -> (none)
//   [1,1,1]                             -> (none)
//   [-1,-1,-1,2,2]                      -> [-1,-1,2]        (ek hi baar)
//   [3,0,-2,-1,1,2]                     -> [-2,-1,3] [-2,0,2] [-1,0,1]
//   [-5,5,0,10,-10]                     -> [-10,0,10] [-5,0,5]
//   [-4,-2,-2,-2,0,1,2,2,2,3,3,4,4,6,6] -> [-4,-2,6] [-4,0,4] [-4,1,3] [-4,2,2] [-2,-2,4] [-2,0,2]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> threeSum(vector<int> &nums)
{
    if (nums.size() == 0)
    {
        return {};
    }

    int n = nums.size();
    sort(begin(nums), end(nums));
    vector<vector<int>> ans;
    for (int i = 0; i < n; i++)
    {
        if (i > 0 && nums[i] == nums[i - 1])
            continue;

        int low = i + 1, high = n - 1, sum = -nums[i];
        while (low < high)
        {
            if (nums[low] + nums[high] == sum)
            {
                ans.push_back({nums[low], nums[high], nums[i]});

                while (low < high && nums[low] == nums[low + 1])
                {
                    low++;
                }
                while (low < high && nums[high] == nums[high - 1])
                {
                    high--;
                }
                low++;
                high--;
            }
            else if (nums[low] + nums[high] < sum)
            {
                low++;
            }
            else
            {
                high--;
            }
        }
    }
    return ans;
}

// ---- test helper (ise mat chhed) — order-free compare ----
string show(vector<vector<int>> v)
{
    if (v.empty())
        return "(none)";
    string s;
    for (auto &t : v)
        s += "[" + to_string(t[0]) + "," + to_string(t[1]) + "," + to_string(t[2]) + "] ";
    return s;
}
void check(vector<int> nums, vector<vector<int>> exp, const string &label)
{
    vector<vector<int>> got = threeSum(nums);
    for (auto &t : got)
        sort(t.begin(), t.end());
    for (auto &t : exp)
        sort(t.begin(), t.end());
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    bool ok = (got == exp);
    cout << label << " -> got " << show(got) << "| exp " << show(exp)
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check({-1, 0, 1, 2, -1, -4}, {{-1, -1, 2}, {-1, 0, 1}}, "case1");
    check({0, 1, 1}, {}, "case2");
    check({0, 0, 0}, {{0, 0, 0}}, "case3");
    check({-2, 0, 1, 1, 2}, {{-2, 0, 2}, {-2, 1, 1}}, "case4");
    check({0, 0, 0, 0}, {{0, 0, 0}}, "case5");
    check({}, {}, "case6");
    check({1, 2}, {}, "case7");
    check({1, 1, 1}, {}, "case8");
    check({-1, -1, -1, 2, 2}, {{-1, -1, 2}}, "case9");
    check({3, 0, -2, -1, 1, 2}, {{-2, -1, 3}, {-2, 0, 2}, {-1, 0, 1}}, "case10");
    check({-5, 5, 0, 10, -10}, {{-10, 0, 10}, {-5, 0, 5}}, "case11");
    check({-4, -2, -2, -2, 0, 1, 2, 2, 2, 3, 3, 4, 4, 6, 6},
          {{-4, -2, 6}, {-4, 0, 4}, {-4, 1, 3}, {-4, 2, 2}, {-2, -2, 4}, {-2, 0, 2}}, "case12");
    return 0;
}
