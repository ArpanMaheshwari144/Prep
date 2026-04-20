#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int mostFrequent(vector<int>& nums) {
        unordered_map<int, int> mp;
        for(auto &it : nums){
            mp[it]++;
        }

        int ans = -1;
        int maxi = INT_MIN;
        for(auto &it : mp){
            if(it.second > maxi){
                maxi = it.second;
                ans = it.first;
            }
            else if(it.second == maxi){
                ans = min(ans, it.first);
            }
        }
        return ans;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    cout << "Test 1: " << sol.mostFrequent(t1);
    cout << (sol.mostFrequent(t1) == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {5, 5, 5, 2, 2, 2};
    cout << "Test 2: " << sol.mostFrequent(t2);
    cout << (sol.mostFrequent(t2) == 2 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {7};
    cout << "Test 3: " << sol.mostFrequent(t3);
    cout << (sol.mostFrequent(t3) == 7 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1, 1, 2, 2, 3};
    cout << "Test 4: " << sol.mostFrequent(t4);
    cout << (sol.mostFrequent(t4) == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
