// ============================================================
// MAXIMUM POINTS YOU CAN OBTAIN FROM CARDS   (LC-1423, SW)  -- REDO_1 (blank)
// ============================================================
// cardPoints[] ek row me cards. tumhe EXACTLY k cards lene hain,
// PAR sirf row ke DONO ENDS se (front se ya back se, koi bhi combo).
// li gayi cards ka SUM MAXIMIZE karo -> wahi return.
//
//   [1,2,3,4,5,6,1], k=3  -> 12   (back se 3: 1+6+5)
//   sirf ends se le sakte -- beech ka card seedha nahi utha sakte.
//
// >>> HINT NAHI. khud yaad kar: ends se k lena = beech me kya bachta? <<<
//
// ---- TEST CASES (cardPoints, k -> expected) ----
//   [1,2,3,4,5,6,1],          k=3  -> 12
//   [2,2,2],                  k=2  -> 4
//   [9,7,7,9,7,7,9],          k=7  -> 55     (saare)
//   [1,1000,1],               k=1  -> 1      (beech ka 1000 nahi utha sakte)
//   [1,79,80,1,1,1,200,1],    k=3  -> 202
//   [100,40,17,9,73,75],      k=3  -> 248    (front 100 + back 75+73)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int minSumK(vector<int> &nums, int k)
{
    int sum = 0;
    int minSum = INT_MAX;
    int i = 0, j = 0;
    while (j < nums.size())
    {
        sum += nums[j];
        if (j - i + 1 >= k)
        {
            minSum = min(minSum, sum);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return minSum;
}

int maxScore(vector<int> &nums, int k)
{
    int Total = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        Total += nums[i];
    }

    if (nums.size() - k == 0)
    {
        return Total;
    }

    int minSum = minSumK(nums, nums.size() - k);

    return Total - minSum;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {1, 2, 3, 4, 5, 6, 1};
    check(maxScore(a1, 3), 12, "case1");

    vector<int> a2 = {2, 2, 2};
    check(maxScore(a2, 2), 4, "case2");

    vector<int> a3 = {9, 7, 7, 9, 7, 7, 9};
    check(maxScore(a3, 7), 55, "case3");

    vector<int> a4 = {1, 1000, 1};
    check(maxScore(a4, 1), 1, "case4");

    vector<int> a5 = {1, 79, 80, 1, 1, 1, 200, 1};
    check(maxScore(a5, 3), 202, "case5");

    vector<int> a6 = {100, 40, 17, 9, 73, 75};
    check(maxScore(a6, 3), 248, "case6");
    return 0;
}
