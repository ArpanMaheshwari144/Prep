// ============================================================
// TWO MAX-SUM NON-OVERLAPPING SUBARRAYS   (khud solve, no peek)
// ============================================================
// ek int array (size N) diya. usme se DO subarrays chuno:
//   - dono CONTIGUOUS (lagataar), dono NON-EMPTY (kam-se-kam 1 element)
//   - dono NON-OVERLAPPING (ek doosre se index share nahi karte)
// aisi 2 subarrays jinke SUMS ka JOD (subarray1_sum + subarray2_sum) MAXIMUM ho.
// wo maximum total sum lautao.
//
//   [2,1,-99,3,3]     -> 9     (subarray1 [2,1]=3 · subarray2 [3,3]=6)
//   [1,2,-7,8,6,-4]   -> 17    ([1,2]=3 · [8,6]=14)
//   [3,3,-100,6,6]    -> 18    ([3,3]=6 · [6,6]=12)
//   [1,2,3,4]         -> 10    ([1,2,3]=6 · [4]=4  -> ya koi bhi split)
//   [5,-2,5]          -> 10    ([5] · [5], beech ka -2 chhoड़a)
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int maxTwoNonOverlap(vector<int> &nums)
{
    // TODO: tu likh
    return 0;
}

int main()
{
    vector<int> a = {2, 1, -99, 3, 3};
    vector<int> b = {1, 2, -7, 8, 6, -4};
    vector<int> c = {3, 3, -100, 6, 6};
    vector<int> d = {1, 2, 3, 4};
    vector<int> e = {5, -2, 5};

    cout << maxTwoNonOverlap(a) << " (expected 9)\n";
    cout << maxTwoNonOverlap(b) << " (expected 17)\n";
    cout << maxTwoNonOverlap(c) << " (expected 18)\n";
    cout << maxTwoNonOverlap(d) << " (expected 10)\n";
    cout << maxTwoNonOverlap(e) << " (expected 10)\n";
    return 0;
}
