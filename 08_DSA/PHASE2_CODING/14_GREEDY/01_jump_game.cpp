// ============================================================
// JUMP GAME   (Medium | Greedy)
// ============================================================
// nums[] -- nums[i] = i se max jump length. Kya LAST index tak pahunch sakte? (true/false)
//
//   [2,3,1,1,4] -> true   (0->1->4 ya 0->2...)
//   [3,2,1,0,4] -> false  (index 3 pe 0 -> aage atak)
//   [0]         -> true   (already last pe)
//   [1,0,1,0]   -> false
//
// GREEDY (jo samjha): "farthest" track -> i > farthest? FALSE. farthest=max(farthest, i+nums[i]). farthest>=last? TRUE.
// (tu code kar. fasa to "madad".)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ---- TERA kaam: yeh function bhar (greedy) ----
bool canJump(vector<int> nums)
{
    int n = nums.size();
    int farthest = 0;
    for (int i = 0; i < n; i++)
    {
        if (i <= farthest)
            farthest = max(farthest, i + nums[i]);
        else
            return false;
    }
    return (farthest >= n - 1) ? true : false;
}

int main()
{
    cout << canJump({2, 3, 1, 1, 4}) << endl; // expected 1 (true)
    cout << canJump({3, 2, 1, 0, 4}) << endl; // expected 0 (false)
    cout << canJump({0}) << endl;             // expected 1
    cout << canJump({1, 0, 1, 0}) << endl;    // expected 0
    cout << canJump({2, 0, 0}) << endl;       // expected 1 (0->jump2->last)
    return 0;
}
