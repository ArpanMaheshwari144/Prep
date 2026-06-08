// ============================================================
// REVERSE ARRAY  —  Arrays basics
// ============================================================
// Array `nums` ko IN-PLACE reverse karo.
//
// Example:
//   {3, 7, 2, 9, 4, 1}  ->  {1, 4, 9, 2, 7, 3}
//
// INTUITION (soch — code TU likhega):
//   Do haath — ek shuruaat pe, ek aakhir pe.
//   Dono ko aapas mein BADAL do (swap), phir dono ek kadam ANDAR aao.
//   Aise beech tak milte jao — array ulta ho jaata.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void reverseArray(vector<int> &nums)
{
    int low = 0;
    int high = nums.size() - 1;
    while (low < high)
    {
        swap(nums[low], nums[high]);
        low++;
        high--;
    }
}

int main()
{
    vector<int> nums = {3, 7, 2, 9, 4, 1};
    reverseArray(nums);
    for (int x : nums)
        cout << x << " "; // expected: 1 4 9 2 7 3
    cout << endl;
    return 0;
}
