// ============================================================
// MEDIAN OF TWO SORTED ARRAYS  (LC-4, Hard)  — REDO_2 (blank redo)
// ------------------------------------------------------------
// KYA KARNA: 2 sorted arrays a[], b[]. Dono ko MERGE kiye BINA overall
//   median nikaalo, O(log(min(m,n))).
// FAMILY (nudge, baaki khud): Binary Search on PARTITION/CUT — chhote
//   array pe cut lo, doosre ka cut auto. valid-cut ka feel dry-run se.
//
// TESTS (input -> expected):
//   a=[1,3]      b=[2]        -> 2.0
//   a=[1,2]      b=[3,4]      -> 2.5
//   a=[]         b=[1]        -> 1.0
//   a=[2]        b=[]         -> 2.0
//   a=[1,2,3,4]  b=[5,6,7,8]  -> 4.5
//   a=[1,3,5]    b=[2,4]      -> 3.0
// ============================================================
#include <bits/stdc++.h>
using namespace std;

double findMedianSortedArrays(vector<int> &A, vector<int> &B)
{
    int m = A.size();
    int n = B.size();

    if (m > n)
    {
        return findMedianSortedArrays(B, A);
    }

    int low = 0, high = m;
    while (low <= high)
    {
        int Px = low + (high - low) / 2;
        int Py = ((m + n + 1) / 2) - Px;

        int x1 = (Px == 0) ? INT_MIN : A[Px - 1];
        int x2 = (Py == 0) ? INT_MIN : B[Py - 1];
        int x3 = (Px == m) ? INT_MAX : A[Px];
        int x4 = (Py == n) ? INT_MAX : B[Py];

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

    return 0.0;
}

void check(vector<int> a, vector<int> b, double exp)
{
    double got = findMedianSortedArrays(a, b);
    cout << (fabs(got - exp) < 1e-6 ? "PASS" : "FAIL")
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    check({1, 3}, {2}, 2.0);
    check({1, 2}, {3, 4}, 2.5);
    check({}, {1}, 1.0);
    check({2}, {}, 2.0);
    check({1, 2, 3, 4}, {5, 6, 7, 8}, 4.5);
    check({1, 3, 5}, {2, 4}, 3.0);
    return 0;
}
