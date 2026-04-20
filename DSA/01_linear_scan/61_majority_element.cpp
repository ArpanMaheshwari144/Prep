#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int, int> mp;
        for(int i=0;i<nums.size();i++){
            mp[nums[i]]++;
        }

        for(auto & it : mp)
        if(it.second > nums.size()/2){
            return it.first;
        }
        return -1;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {2, 2, 1, 1, 1, 2, 2};
    cout << "Test 1: " << sol.majorityElement(t1);
    cout << (sol.majorityElement(t1) == 2 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {3, 3, 4};
    cout << "Test 2: " << sol.majorityElement(t2);
    cout << (sol.majorityElement(t2) == 3 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {1};
    cout << "Test 3: " << sol.majorityElement(t3);
    cout << (sol.majorityElement(t3) == 1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {6, 5, 5};
    cout << "Test 4: " << sol.majorityElement(t4);
    cout << (sol.majorityElement(t4) == 5 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
