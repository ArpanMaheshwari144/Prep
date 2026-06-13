// ============================================================
// 3SUM   —   Two Pointer (build on Two Sum II sorted)
// ============================================================
// Integer array. Saare UNIQUE triplets [a,b,c] jinka sum = 0. Duplicate triplets nahi.
//
// Example:
//   [-1, 0, 1, 2, -1, -4]   ->   [[-1, -1, 2], [-1, 0, 1]]
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   1. Array SORT karo (warna low/high two-pointer logic kaam nahi karega).
//   2. for i (har element FIX karo):
//        target = -nums[i]
//        low = i+1, high = n-1   ->   Two Sum II:
//           nums[low] + nums[high] < target  -> low++
//           nums[low] + nums[high] > target  -> high--
//           ==                                -> triplet record (nums[i], nums[low], nums[high])
//   3. DUPLICATE-skip twist: abhi chhod. Pehle basic chalao, duplicate baad mein.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> threeSum(vector<int> &nums)
{
    sort(begin(nums), end(nums));

    vector<vector<int>> ans;

    for (int i = 0; i < nums.size(); i++)
    {
        if (i > 0 && nums[i] == nums[i - 1])
            continue;
        
        int low = i + 1, high = nums.size() - 1, sum = 0 - nums[i];
        while (low < high)
        {
            if (nums[low] + nums[high] == sum)
            {
                vector<int> temp;
                temp.push_back(nums[i]);
                temp.push_back(nums[low]);
                temp.push_back(nums[high]);
                ans.push_back(temp);

                // low aage chalta rahega isliye low+1
                while (low < high && nums[low] == nums[low + 1])
                {
                    low++;
                }
                while (low < high && nums[high] == nums[high - 1])
                {
                    high--;
                }
                low++;
                high--;
            }
            else if (nums[low] + nums[high] < sum)
            {
                low++;
            }
            else
            {
                high--;
            }
        }
    }
    return ans;
}

int main()
{
    vector<int> nums = {-1, 0, 1, 2, -1, -4};

    vector<vector<int>> ans = threeSum(nums);

    for (auto &t : ans)
    {
        cout << "[ ";
        for (int x : t)
            cout << x << " ";
        cout << "]" << endl;
    }
    // expected (kisi order mein): [-1 -1 2]  aur  [-1 0 1]
    return 0;
}
