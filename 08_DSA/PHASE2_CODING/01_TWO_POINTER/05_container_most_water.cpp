// ============================================================
// CONTAINER WITH MOST WATER   —   Two Pointer (opposite ends)
// ============================================================
// heights[] = deewaron ki height. Do deewar chuno — beech paani bharega.
// area = (right - left) * min(height[left], height[right]). SABSE ZYADA area return karo.
//
// Example:
//   {1, 8, 6, 2, 5, 4, 8, 3, 7}  ->  49   (index 1 height 8, index 8 height 7 -> 7 * 7)
//
// INTUITION (TU ne derive kiya — code TU likhega):
//   Two pointer, dono OPPOSITE end pe (left = 0, right = last).
//   Area = (right - left) * min(height[left], height[right]).  Max area yaad rakho.
//   CHOTI deewar ko andar lao: height[left] < height[right] -> left++, warna right--.
//   (choti deewar hata ke hi bada milne ka chance; taller hata ke kabhi bada nahi.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxArea(vector<int> &height)
{
    int left = 0;
    int right = height.size() - 1;
    int maxi = INT_MIN;
    while (left < right)
    {
        int width = right - left;
        int area = min(height[left], height[right]) * width;
        maxi = max(maxi, area);
        if (height[left] < height[right])
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    return maxi;
}

int main()
{
    vector<int> a = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << maxArea(a) << endl; // expected: 49
    return 0;
}
