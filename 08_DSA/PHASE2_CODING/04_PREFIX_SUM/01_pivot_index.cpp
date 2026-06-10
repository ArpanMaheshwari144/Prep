// ============================================================
// FIND PIVOT INDEX   —   Prefix Sum
// ============================================================
// Woh INDEX return karo jahan left ke sab elements ka sum == right ke sab ka sum
// (us index ka apna element kisi side mein nahi). Leftmost; koi na ho to -1.
//
// Example:
//   {1, 7, 3, 6, 5, 6}  ->  3   (left 1+7+3=11 == right 5+6=11)
//   {1, 2, 3}           -> -1
//   {2, 1, -1}          ->  0   (left 0 == right 1+(-1)=0)
//
// INTUITION (soch — code TU likhega):
//   Pehle poore array ka TOTAL sum nikaalo.
//   Phir "leftSum" chalao (0 se). Har index pe:
//     rightSum = total - leftSum - current
//     leftSum == rightSum ? -> yeh index return
//     warna -> current ko leftSum mein jodo, aage badho
//   Koi na mile -> -1.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int pivotIndex(vector<int> &nums)
{
    int totalSum = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        totalSum += nums[i];
    }

    int leftSum = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        int rightSum = totalSum - leftSum - nums[i];
        if (leftSum == rightSum)
        {
            return i;
        }
        leftSum += nums[i];
    }
    return -1;
}

int main()
{
    vector<int> a = {1, 7, 3, 6, 5, 6};
    cout << pivotIndex(a) << endl; // expected: 3

    vector<int> b = {1, 2, 3};
    cout << pivotIndex(b) << endl; // expected: -1
    return 0;
}
