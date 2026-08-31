// ============================================================
// MEDIAN OF TWO SORTED ARRAYS — (LeetCode 4, Hard)   [BINARY_SEARCH #12]
// ============================================================
// Do sorted arrays A, B diye. Dono ka COMBINED median nikaalo. O(log(m+n)) chahiye.
// (merge mat karo -> binary-search se ek CUT dhundo: left aadha | right aadha,
//  left ka har element <= right ka har element.)
//
//   median = beech wala. total ODD -> ekdum middle. total EVEN -> do middle ka AVERAGE.
//   return type = double (2.5 jaisa aa sakta).
//
// TEST-CASES (A , B -> expected median):
//   [1,3]        , [2]                 -> 2.0
//   [1,2]        , [3,4]               -> 2.5
//   [1,3,8,9,15] , [7,11,18,19,21,25]  -> 11.0
//   []           , [1]                 -> 1.0
//   [2]          , []                  -> 2.0
//   [0,0]        , [0,0]               -> 0.0
// ============================================================

#include <bits/stdc++.h>
using namespace std;

double findMedianSortedArrays(vector<int> &A, vector<int> &B)
{
    int m = A.size();
    int n = B.size();

    if (A.size() > B.size())
    {
        return findMedianSortedArrays(B, A);
    }

    int low = 0;
    int high = m;

    while (low <= high)
    {
        int Px = low + (high - low) / 2; // mid
        int Py = ((m + n + 1) / 2) - Px;

        int x1 = Px == 0 ? INT_MIN : A[Px - 1];
        int x2 = Py == 0 ? INT_MIN : B[Py - 1];
        int x3 = Px == m ? INT_MAX : A[Px];
        int x4 = Py == n ? INT_MAX : B[Py];

        if (x1 <= x4 && x2 <= x3)
        {
            if ((m + n) % 2 == 0)
            {
                return (max(x1, x2) + min(x3, x4)) / 2.0;
            }
            else
            {
                return max(x1, x2);
            }
        }

        if (x1 > x4)
        {
            high = Px - 1;
        }
        else
        {
            low = Px + 1;
        }
    }
    return -1;
}

void check(vector<int> A, vector<int> B, double exp)
{
    double got = findMedianSortedArrays(A, B);
    cout << (abs(got - exp) < 1e-6 ? "PASS" : "FAIL")
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    check({1, 3}, {2}, 2.0);
    check({1, 2}, {3, 4}, 2.5);
    check({1, 3, 8, 9, 15}, {7, 11, 18, 19, 21, 25}, 11.0);
    check({}, {1}, 1.0);
    check({2}, {}, 2.0);
    check({0, 0}, {0, 0}, 0.0);
    return 0;
}
