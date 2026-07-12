// ============================================================
// TWO SUM   (Hashing — map se "seen it?" O(1))
// ============================================================
// Array nums + ek target diya. DO indices [i, j] return karo jinke
// nums[i] + nums[j] == target. (exactly ek solution, same element 2 baar nahi.)
//
// ---- ARPAN KI APPROACH ----
//  map me {value:index} rakho. har num pe check: (target-num) map me pehle se hai?
//  -> haan -> {us index, current index} return. nahi -> num daal do map me, aage badho.
//  ONE pass, O(n).
//
// Tests (// expected — indices; pair sahi hona chahiye):
//   [2,7,11,15], t=9  -> [0,1]   (2+7)
//   [3,2,4],     t=6  -> [1,2]   (2+4)
//   [3,3],       t=6  -> [0,1]
//   [1,5,3,7],   t=8  -> [1,2]   (5+3)
// ============================================================

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

vector<int> twoSum(vector<int> &nums, int target)
{
    unordered_map<int, int> mp;
    for (int i = 0; i < nums.size(); i++)
    {
        if (mp.find(target - nums[i]) != mp.end())
        {
            return vector<int>{mp[target - nums[i]], i};
        }
        mp[nums[i]] = i;
    }
    return vector<int>{-1, -1};
}

void print(vector<int> &v)
{
    cout << "[";
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << (i + 1 < v.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    vector<int> a1 = {2, 7, 11, 15};
    vector<int> a2 = {3, 2, 4};
    vector<int> a3 = {3, 3};
    vector<int> a4 = {1, 5, 3, 7};

    auto r1 = twoSum(a1, 9);
    print(r1); // [0,1]
    auto r2 = twoSum(a2, 6);
    print(r2); // [1,2]
    auto r3 = twoSum(a3, 6);
    print(r3); // [0,1]
    auto r4 = twoSum(a4, 8);
    print(r4); // [1,2]
    return 0;
}
