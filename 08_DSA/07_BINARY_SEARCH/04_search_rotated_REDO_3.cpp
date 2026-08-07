// ============================================================
// SEARCH IN ROTATED SORTED ARRAY — Binary Search   [REDO_3 — khud bharo]
// ============================================================
// sorted array kisi pivot pe ROTATE (distinct values). target ka INDEX return, nahi mile -> -1.  O(log n).
//   [0,1,2,4,5,6,7] -> rotate -> [4,5,6,7,0,1,2]
//
// HINT (khud derive): mid==target? warna dekho kaunsa HALF sorted hai (nums[low]<=nums[mid]?) ->
//        target us sorted-half ki range me? -> wahi taraf jao, warna doosri taraf.
//
//   [4,5,6,7,0,1,2], target=0  -> 4
//   [4,5,6,7,0,1,2], target=6  -> 2
//   [4,5,6,7,0,1,2], target=3  -> -1
//   [1],             target=0  -> -1
//   [1],             target=1  -> 0
//   [5,1,3],         target=5  -> 0
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int search(vector<int> &nums, int target)
{
    // TODO: khud likho (low<=high loop, mid, which-half-sorted -> range check)
    return -1;
}

int main()
{
    vector<int> a1 = {4, 5, 6, 7, 0, 1, 2};
    vector<int> a2 = {1};
    vector<int> a3 = {5, 1, 3};

    cout << search(a1, 0) << " (expected 4)\n";
    cout << search(a1, 6) << " (expected 2)\n";
    cout << search(a1, 3) << " (expected -1)\n";
    cout << search(a2, 0) << " (expected -1)\n";
    cout << search(a2, 1) << " (expected 0)\n";
    cout << search(a3, 5) << " (expected 0)\n";
    return 0;
}
