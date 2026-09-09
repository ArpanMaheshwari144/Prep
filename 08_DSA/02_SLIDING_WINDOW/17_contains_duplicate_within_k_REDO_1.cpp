// ============================================================
// CONTAINS DUPLICATE II  — REDO_1 (blank retrieval, 9-Sep)
// ============================================================
// Array nums + ek number k diya. Batao kya aise DO index i, j hain jaha
//   nums[i] == nums[j]   AND   |i - j| <= k     -> true/false
//
// (kal solve kiya tha — ab BAND karke, khud se dobara likh. peek NAHI.)
//
// Tests (// expected):
//   [1,2,3,1],       k=3   -> true
//   [1,0,1,1],       k=1   -> true
//   [1,2,3,1,2,3],   k=2   -> false
//   [1],             k=1   -> false
//   [99,99],         k=2   -> true
// ============================================================

#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

bool containsNearbyDuplicate(vector<int> &nums, int k)
{
    int i = 0, j = 0;
    unordered_set<int> st;
    while (j < nums.size())
    {
        if (st.count(nums[j]) > 0)
        {
            return true;
        }
        if (j - i + 1 > k)
        {
            st.erase(nums[i]);
            i++;
        }
        st.insert(nums[j]);
        j++;
    }
    return false;
}

int main()
{
    vector<int> a1 = {1, 2, 3, 1};
    vector<int> a2 = {1, 0, 1, 1};
    vector<int> a3 = {1, 2, 3, 1, 2, 3};
    vector<int> a4 = {1};
    vector<int> a5 = {99, 99};

    cout << containsNearbyDuplicate(a1, 3) << " (exp 1)\n";
    cout << containsNearbyDuplicate(a2, 1) << " (exp 1)\n";
    cout << containsNearbyDuplicate(a3, 2) << " (exp 0)\n";
    cout << containsNearbyDuplicate(a4, 1) << " (exp 0)\n";
    cout << containsNearbyDuplicate(a5, 2) << " (exp 1)\n";
    return 0;
}
