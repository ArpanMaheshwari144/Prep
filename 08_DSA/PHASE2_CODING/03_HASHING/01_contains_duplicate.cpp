// ============================================================
// CONTAINS DUPLICATE   —   Hashing (HashSet — membership)
// ============================================================
// Array `nums` mein koi value kam se kam DO baar aati hai? true / false.
//
// Example:
//   {1, 2, 3, 1}  ->  true   (1 do baar)
//   {1, 2, 3, 4}  ->  false  (sab alag)
//
// INTUITION (soch — code TU likhega):
//   Ek "note" rakho (jisme likhte jao kaunse numbers DEKH chuke).
//   - Har number pe poochho: "kya yeh PEHLE dekha hai?" (note mein hai?)
//        haan -> duplicate -> true
//        nahi -> note mein daal do, aage badho
//   - Poora ghoom liya, koi repeat nahi -> false.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool containsDuplicate(vector<int> &nums)
{
    unordered_set<int> st;
    for (int i = 0; i < nums.size(); i++)
    {
        if (st.count(nums[i]))
        {
            return true;
        }
        st.insert(nums[i]);
    }
    return false;
}

int main()
{
    vector<int> a = {1, 2, 3, 1};
    cout << boolalpha << containsDuplicate(a) << endl; // expected: true

    vector<int> b = {1, 2, 3, 4};
    cout << boolalpha << containsDuplicate(b) << endl; // expected: false
    return 0;
}
