// ============================================================
// MAX SUM SUBARRAY (size k)   —   Sliding Window (FIXED)
// ============================================================
// Array `nums`, k size ke LAGATAR (consecutive) subarray ka MAXIMUM sum return karo.
//
// Example:
//   {2, 1, 5, 1, 3, 2}, k = 3  ->  9   (5 + 1 + 3)
//
// INTUITION (soch — code TU likhega):
//   Ek KHIDKI (window) jo k lagatar elements dekhti hai.
//   Pehle k elements ka sum nikaalo (pehli khidki).
//   Phir khidki ek kadam AAGE sarkao:
//       naya element andar aaya   -> sum mein JODO   (+ entering)
//       sabse purana bahar gaya   -> sum mein se GHATAO (- leaving)
//   Har poori khidki pe sum dekho, sabse bada yaad rakho.
//   Trick: poora dobara mat joro — bas ek add, ek minus (O(n)).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxSumK(vector<int> &nums, int k)
{
    int i = 0, j = 0, n = nums.size(), currSum = 0, maxSum = 0;
    while (j < n)
    {
        currSum += nums[j];
        if (j - i + 1 > k)
        {
            currSum -= nums[i];
            i++;
        }
        if (j - i + 1 == k)
        {
            maxSum = max(maxSum, currSum);
        }
        j++;
    }
    return maxSum;
}

int main()
{
    vector<int> nums = {2, 1, 5, 1, 3, 2};
    cout << maxSumK(nums, 3) << endl; // expected: 9
    return 0;
}
