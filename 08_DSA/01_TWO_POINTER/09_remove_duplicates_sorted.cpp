// ============================================================
// REMOVE DUPLICATES FROM SORTED ARRAY — Two Pointer  (fresh, in-place)
// ============================================================
// SORTED array diya. duplicates IN-PLACE hatao taaki har element sirf EK baar aaye.
// naya LENGTH k lauta (pehle k elements unique + sorted hone chahiye). baaki se matlab nahi.
//   nums=[1,1,2]        -> k=2, nums=[1,2,...]
//   nums=[0,0,1,1,1,2,2,3,3,4] -> k=5, nums=[0,1,2,3,4,...]
//

// ---- ARPAN KI APPROACH ----
//  MOVE-ZEROES ka cousin: slow = last-unique ka index, fast = scan.
//  wahan 0 dekhte the, yahan DUPLICATE: agar nums[slow] != nums[fast] (naya unique mila) ->
//  pehle slow++ (slow ab ek DUPLICATE pe jaata, pichle run ka dupe pada hota),
//  phir nums[slow] = nums[fast] (us dupe ko naye unique se OVERWRITE kar do).
//  fast har baar ++ (aage badhta rehta). end me unique count = slow + 1 (0-based isliye +1).


// Tests (nums -> expected k, aur pehle k elements):
//   [1,1,2]                    -> 2  | 1 2
//   [0,0,1,1,1,2,2,3,3,4]      -> 5  | 0 1 2 3 4
//   [1,2,3]                    -> 3  | 1 2 3
//   [1,1,1,1]                  -> 1  | 1
//   [5]                        -> 1  | 5
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int removeDuplicates(vector<int> &nums)
{
    int slow = 0;
    int fast = 0;
    while (fast < nums.size())
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
    vector<vector<int>> tests = {
        {1, 1, 2},
        {0, 0, 1, 1, 1, 2, 2, 3, 3, 4},
        {1, 2, 3},
        {1, 1, 1, 1},
        {5}};

    for (auto &t : tests)
    {
        int k = removeDuplicates(t);
        cout << "k=" << k << " | ";
        for (int i = 0; i < k; i++)
            cout << t[i] << " ";
        cout << "\n";
    }
    // expected:
    // k=2 | 1 2
    // k=5 | 0 1 2 3 4
    // k=3 | 1 2 3
    // k=1 | 1
    // k=1 | 5
    return 0;
}
