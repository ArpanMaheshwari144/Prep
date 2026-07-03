// ============================================================
// CONTAINER WITH MOST WATER
// ============================================================
// height[] = har value ek vertical line ki height. 2 lines + x-axis se container.
// SABSE ZYADA paani (max AREA) nikaalo.
//   area(i,j) = min(height[i], height[j]) * (j - i)
//
// Tests (// expected):
//   [1,8,6,2,5,4,8,3,7] -> 49
//   [1,1]               -> 1
//   [4,3,2,1,4]         -> 16
//   [1,2,1]             -> 2
//   [2,3,4,5,18,17,6]   -> 17
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxArea(vector<int> &height)
{
    int left = 0;
    int right = height.size() - 1;
    int maxArea = INT_MIN;
    while (left < right)
    {
        int area = (right - left) * min(height[left], height[right]);
        maxArea = max(maxArea, area);

        if (height[left] < height[right])
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    return maxArea;
}

int main()
{
    vector<int> a1 = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    vector<int> a2 = {1, 1};
    vector<int> a3 = {4, 3, 2, 1, 4};
    vector<int> a4 = {1, 2, 1};
    vector<int> a5 = {2, 3, 4, 5, 18, 17, 6};

    cout << maxArea(a1) << " (expected 49)\n";
    cout << maxArea(a2) << " (expected 1)\n";
    cout << maxArea(a3) << " (expected 16)\n";
    cout << maxArea(a4) << " (expected 2)\n";
    cout << maxArea(a5) << " (expected 17)\n";
    return 0;
}
