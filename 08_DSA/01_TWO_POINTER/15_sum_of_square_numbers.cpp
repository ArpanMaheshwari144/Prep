// ============================================================
// SUM OF SQUARE NUMBERS — (LeetCode 633, Google-asked)  (fresh)
// ============================================================
// non-negative integer c diya. TRUE agar do integers a aur b aise hon ki
// a*a + b*b == c.  warna FALSE.
//   c = 5  -> true   (1*1 + 2*2 = 5)
//   c = 3  -> false
//   c = 0  -> true   (0 + 0)
//   c = 4  -> true   (0 + 4)
//   c = 2  -> true   (1 + 1)
//


//
// Tests (c -> expected):
//   5   -> 1
//   3   -> 0
//   0   -> 1
//   4   -> 1
//   2   -> 1
//   1   -> 1     (0*0 + 1*1)
//   8   -> 1     (2*2 + 2*2)
//   7   -> 0
//   100 -> 1     (6*6 + 8*8 = 36+64)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool judgeSquareSum(long long c)
{
    int low = 0, high = sqrt(c);
    while (low <= high) // cout << judgeSquareSum(0) << " (expected 1)\n";
    {
        long long ans = (long long)low * low + (long long)high * high;
        if (ans == c)
        {
            return true;
        }
        else if (ans < c)
        {
            low++;
        }
        else
        {
            high--;
        }
    }
    return false;
}

int main()
{
    cout << judgeSquareSum(5) << " (expected 1)\n";
    cout << judgeSquareSum(3) << " (expected 0)\n";
    cout << judgeSquareSum(0) << " (expected 1)\n";
    cout << judgeSquareSum(4) << " (expected 1)\n";
    cout << judgeSquareSum(2) << " (expected 1)\n";
    cout << judgeSquareSum(1) << " (expected 1)\n";
    cout << judgeSquareSum(8) << " (expected 1)\n";
    cout << judgeSquareSum(7) << " (expected 0)\n";
    cout << judgeSquareSum(100) << " (expected 1)\n";
    return 0;
}
