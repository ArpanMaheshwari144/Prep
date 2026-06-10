// ============================================================
// MISSING NUMBER (1..n)   —   Arrays (XOR trick)
// ============================================================
// Array mein 1 se n tak ke numbers hone chahiye the, par EK missing hai.
// Woh missing number return karo.
//
// Example:
//   n = 5,  nums = {1, 2, 4, 5}  ->  3
//
// INTUITION (soch — code TU likhega):
//   XOR ke do rule:  a ^ a = 0 (pair gayab),  a ^ 0 = a (akela bachta).
//   Ek result = 0 rakho.
//   1 se n tak ke saare numbers result mein XOR karo.
//   Phir array ke saare elements bhi result mein XOR karo.
//   Jo number dono mein hai (duplicate) -> cancel. MISSING akela bachta = answer.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int missingNumber(vector<int> &nums, int n)
{
    int xorr = 0;
    for (int i = 1; i <= n; i++)
    {
        xorr ^= i;
    }
    for (int i = 0; i < nums.size(); i++)
    {
        xorr ^= nums[i];
    }
    return xorr;
}

int main()
{
    vector<int> a = {1, 2, 4, 5};
    cout << missingNumber(a, 5) << endl; // expected: 3
    return 0;
}
