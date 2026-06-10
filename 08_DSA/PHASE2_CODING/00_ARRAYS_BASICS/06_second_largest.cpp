// ============================================================
// SECOND LARGEST ELEMENT   —   Arrays (single pass)
// ============================================================
// Array `nums` mein doosra sabse bada (second largest) element return karo.
// Koi valid second na ho to -1.
//
// Example:
//   {3, 7, 1, 9, 5, 2}  ->  7    (9 sabse bada, 7 doosra)
//   {3, 2}              ->  2
//
// INTUITION (soch — code TU likhega):
//   Ek hi pass mein DO cheez track karo: max (sabse bada) + second (doosra bada).
//   Dono ko shuru mein INT_MIN se start karo (safe — descending edge se bachata).
//   Har element pe:
//     - agar woh max se BADA hai  -> pehle purana max ko second bana do, PHIR max ko naya karo
//                                    (order: pehle bachao, phir badlo)
//     - warna agar woh second se bada hai (par max ke barabar nahi) -> second update karo
//   End mein second abhi bhi INT_MIN ho -> koi valid second nahi -> -1.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int secondLargest(vector<int> &nums)
{
    int first = INT_MIN;
    int second = INT_MIN;
    for (auto &it : nums)
    {
        if (it > first)
        {
            second = first;
            first = it;
        }
        else if (it > second && first != it)
        {
            second = it;
        }
    }
    return second;
}

int main()
{
    vector<int> a = {3, 7, 1, 9, 5, 2};
    cout << secondLargest(a) << endl; // expected: 7

    vector<int> b = {3, 2};
    cout << secondLargest(b) << endl; // expected: 2
    return 0;
}
