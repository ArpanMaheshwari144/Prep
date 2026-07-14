// ============================================================
// 3 SUM
// ============================================================
// Array diya. SAARE UNIQUE triplets [a,b,c] dhoondho jinka sum = 0.
// (duplicate triplets nahi chahiye.)
//
// ---- ARPAN KI APPROACH ----
//  pehle sort. phir i pe har element FIX karo (i dupe ho -> continue skip).
//  baaki pe two-pointer low=i+1, high=end, target sum = -nums[i].
//  nums[low]+nums[high] == sum -> triplet push -> dono taraf dupes skip -> low++,high--.
//  sum chhota -> low++ , sum bada -> high--.
//
// Tests (// expected — triplets sorted, order alag ho sakta):
//   [-1,0,1,2,-1,-4]  -> [-1,-1,2] [-1,0,1]
//   [0,1,1]           -> (none)
//   [0,0,0]           -> [0,0,0]
//   [-2,0,1,1,2]      -> [-2,0,2] [-2,1,1]
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> threeSum(vector<int> &nums)
{
    // TODO: tera code (COLD REDO)
    //   sort -> fix i -> low/high two-pointer -> sum==0 -> record + skip-dupes
    //   (yaad jaha atka tha: triplet mile -> skip-dupes + low++/high-- advance; bounds!)
    vector<vector<int>> ans;
    sort(begin(nums), end(nums));

    for (int i = 0; i < nums.size(); i++)
    {
        if (i > 0 && nums[i] == nums[i - 1])
        {
            continue;
        }
        int low = i + 1, high = nums.size() - 1, sum = -nums[i];
        while (low < high)
        {
            if (nums[low] + nums[high] == sum)
            {
                vector<int> temp;
                temp.push_back(nums[i]);
                temp.push_back(nums[low]);
                temp.push_back(nums[high]);
                ans.push_back(temp);

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

void print(vector<vector<int>> &res)
{
    for (auto &t : res)
        cout << "[" << t[0] << "," << t[1] << "," << t[2] << "] ";
    cout << "\n";
}

int main()
{
    vector<int> a1 = {-1, 0, 1, 2, -1, -4};
    vector<int> a2 = {0, 1, 1};
    vector<int> a3 = {0, 0, 0};
    vector<int> a4 = {-2, 0, 1, 1, 2};

    auto r1 = threeSum(a1);
    print(r1); // [-1,-1,2] [-1,0,1]
    auto r2 = threeSum(a2);
    print(r2); // (none)
    auto r3 = threeSum(a3);
    print(r3); // [0,0,0]
    auto r4 = threeSum(a4);
    print(r4); // [-2,0,2] [-2,1,1]
    return 0;
}
