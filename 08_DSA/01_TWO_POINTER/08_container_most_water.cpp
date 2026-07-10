// ============================================================
// CONTAINER WITH MOST WATER — Two Pointer  (random self-test)
// ============================================================
// height[] diya — har element ek vertical line ki height. do lines chuno jo
// x-axis ke saath SABSE ZYADA paani ka container banayein. MAX area lautao.
//   area = min(height[i], height[j]) * (j - i)   [width * chhoti wali height]
//   height=[1,8,6,2,5,4,8,3,7] -> 49
//
// (approach LIKHA NAHI -- socho: 2 pointer (i=start, j=end).
//  area nikaalo -> phir CHHOTI height wale pointer ko andar khisko (kyun? chhoti height bottleneck hai).
//  max area track karo. copy-pen pe trace pehle.)
//
// Tests (// expected):
//   [1,8,6,2,5,4,8,3,7]     -> 49
//   [1,1]                    -> 1
//   [4,3,2,1,4]              -> 16
//   [1,2,1]                  -> 2
//   [2,3,10,5,7,8,9]         -> 36
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int maxArea(vector<int> &height)
{
    int low = 0;
    int high = height.size() - 1;
    int max_area = INT_MIN;
    while (low < high)
    {
        int width = high - low;
        int area = width * min(height[low], height[high]);
        max_area = max(max_area, area);

        if (height[low] < height[high])
        {
            low++;
        }
        else
        {
            high--;
        }
    }
    return max_area;
}

int main()
{
    vector<int> a = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    vector<int> b = {1, 1};
    vector<int> c = {4, 3, 2, 1, 4};
    vector<int> d = {1, 2, 1};
    vector<int> e = {2, 3, 10, 5, 7, 8, 9};

    cout << maxArea(a) << " (expected 49)\n";
    cout << maxArea(b) << " (expected 1)\n";
    cout << maxArea(c) << " (expected 16)\n";
    cout << maxArea(d) << " (expected 2)\n";
    cout << maxArea(e) << " (expected 36)\n";
    return 0;
}
