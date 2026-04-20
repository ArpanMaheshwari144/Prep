// Q: Unsorted array mein 2 elements dhundho jinke sum = target — hashing se
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    // Unsorted array mein 2 elements dhundh jinke sum = target
    // Return indices
    // Hint: complement = target - nums[i]. Map mein dhundh.
    vector<int> twoSum(vector<int>& nums, int target) {
        // map mein element -> index store karo
        unordered_map<int, int> mp;
        vector<int> ans(2, -1);
        for(int i=0;i<nums.size();i++){
            // complement (target - current) map mein hai? toh pair mil gaya
            if(mp.count(target - nums[i])){
                ans[0] = mp[target- nums[i]];
                ans[1] = i;
            }
            // current element map mein daalo — future ke liye
            mp[nums[i]] = i;
        }
        return ans;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {2, 7, 11, 15};
    vector<int> r1 = sol.twoSum(t1, 9);
    cout << "Test 1: [" << r1[0] << "," << r1[1] << "]";
    cout << (r1[0] == 0 && r1[1] == 1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {3, 2, 4};
    vector<int> r2 = sol.twoSum(t2, 6);
    cout << "Test 2: [" << r2[0] << "," << r2[1] << "]";
    cout << (r2[0] == 1 && r2[1] == 2 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {3, 3};
    vector<int> r3 = sol.twoSum(t3, 6);
    cout << "Test 3: [" << r3[0] << "," << r3[1] << "]";
    cout << (r3[0] == 0 && r3[1] == 1 ? "  PASS" : "  FAIL") << endl;

    // Edge: no solution
    vector<int> t4 = {1, 2, 3};
    vector<int> r4 = sol.twoSum(t4, 10);
    cout << "Test 4: [" << r4[0] << "," << r4[1] << "]";
    cout << (r4[0] == -1 && r4[1] == -1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
