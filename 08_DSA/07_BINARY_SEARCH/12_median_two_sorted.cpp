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
//
// ---- APPROACH (BS on PARTITION, is code ke hisaab se) ----
// merge O(m+n) hota; O(log) ke liye: CUT dhundo, na ki merge.
// chhote array (A) pe binary search -> Px = A se kitne element LEFT me.
//   Py auto = (m+n+1)/2 - Px   [left side me total ka aadha hona chahiye]
//
//   A:  1  3  8  9 | 15            Px = 4 (A-left me 4)
//   B:  7 11       | 18 19 21 25   Py = 6-4 = 2
//         x1=9  x3=15
//         x2=11 x4=18
//   |___ LEFT = 6 (half) ___|___ RIGHT = 5 ___|
//
// 4 KINAARE (baaki se matlab NAHI -- sorted isliye apne-aap sahi):
//   x1 = A[Px-1] = Aleft   (edge Px==0 -> INT_MIN)
//   x2 = B[Py-1] = Bleft   (edge Py==0 -> INT_MIN)
//   x3 = A[Px]   = Aright  (edge Px==m -> INT_MAX)
//   x4 = B[Py]   = Bright  (edge Py==n -> INT_MAX)
//
// SAHI cut:  x1<=x4 && x2<=x3   (left-max <= right-min, dono taraf cross-check)
//   galat -> x1>x4 (A ne left me ZYADA liya) -> high=Px-1
//            warna    (A ne KAM liya)        -> low =Px+1
// MEDIAN:  odd -> max(x1,x2)            [left ka sabse bada]
//          even-> (max(x1,x2)+min(x3,x4))/2.0
//
// KYUN sirf 4 number: array sorted -> Px se pehle sab <=A[Px-1], aage sab >=A[Px].
// isliye har step O(1), aur Px pe BS = O(log(min(m,n))).
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
