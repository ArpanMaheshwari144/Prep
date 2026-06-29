// ============================================================
// CONTAINER WITH MOST WATER   (Medium)
// ============================================================
// height[] di -- har value ek vertical line (height). 2 lines + x-axis se container banta.
// SABSE ZYADA paani kitna aa sakta (max AREA) nikaalo.
//   area between i,j = min(height[i], height[j]) * (j - i)
//
//   [1,8,6,2,5,4,8,3,7] -> 49   (index 1 aur 8: min(8,7)*7 = 49)
//   [1,1]               -> 1
//   [4,3,2,1,4]         -> 16   (index 0,4: min(4,4)*4)
//   [1,2,1]             -> 2
//
// (no signal, no hint — tu khud soch. fasa to "madad".)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ---- TERA kaam: yeh function bhar ----
int maxArea(vector<int> height)
{
    // yahan likh
}

int main()
{
    cout << maxArea({1,8,6,2,5,4,8,3,7}) << endl; // expected 49
    cout << maxArea({1,1}) << endl;               // expected 1
    cout << maxArea({4,3,2,1,4}) << endl;         // expected 16
    cout << maxArea({1,2,1}) << endl;             // expected 2
    cout << maxArea({2,3,4,5,18,17,6}) << endl;   // expected 17 (index 4,5: min(18,17)*1)
    return 0;
}
