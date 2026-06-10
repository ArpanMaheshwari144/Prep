// ============================================================
// REMOVE DUPLICATES FROM SORTED ARRAY   —   Two Pointer (slow / fast)
// ============================================================
// SORTED array se duplicates HATAO (in-place) — har value sirf EK baar rahe.
// Unique elements aage rakho, aur unique COUNT return karo.
//
// Example:
//   {1, 1, 2, 2, 2, 3, 4, 4}  ->  return 4,  array aage: [1, 2, 3, 4, ...]
//
// INTUITION (soch — code TU likhega):
//   Array SORTED -> duplicates PAAS-PAAS (adjacent). Naya unique check karne ke liye
//   bas PICHLE (last unique) se compare kaafi — poore array se nahi.
//   slow = last unique ki jagah.  fast = aage scan karta (hamesha ++).
//   jab nums[fast] != nums[slow] (naya unique mila) -> slow++; nums[slow] = nums[fast].
//   end mein unique count = slow + 1.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int removeDuplicates(vector<int> &nums)
{
    int slow = 0;
    int fast = 0;
    int n = nums.size();
    while (fast < n)
    {
        if (nums[slow] != nums[fast])
        {
            slow++;
            nums[slow] = nums[fast];
        }
        fast++;
    }
    return slow + 1;
}

int main()
{
    vector<int> a = {1, 1, 2, 2, 2, 3, 4, 4};
    int k = removeDuplicates(a);

    cout << k << endl; // expected: 4
    for (int i = 0; i < k; i++)
        cout << a[i] << " ";
    cout << endl; // expected: 1 2 3 4
    return 0;
}
