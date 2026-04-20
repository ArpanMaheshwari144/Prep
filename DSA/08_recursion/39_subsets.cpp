// Q: Array ke saare subsets nikalo — pick/not-pick recursion
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // Array ke saare subsets nikaal
    // Har element ke liye: pick ya not-pick
    // Base case: index == n → current subset store kar
    void solve(vector<int>& nums, int index, vector<int>& current, vector<vector<int>>& result) {
        // base case — saare elements dekh liye, current subset save karo
        if(index == nums.size()){
            result.push_back(current);
            return;
        }

        // PICK — current element ko subset mein daalo
        current.push_back(nums[index]);
        solve(nums, index+1, current, result);
        // BACKTRACK — hatao taaki not-pick case sahi chale
        current.pop_back();

        // NOT-PICK — current element skip karo
        solve(nums, index+1, current, result);
    }

    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        // 0th index se start karo
        solve(nums, 0, current, result);
        return result;
    }
};

void printSubsets(vector<vector<int>>& res) {
    cout << "[";
    for (int i = 0; i < res.size(); i++) {
        cout << "[";
        for (int j = 0; j < res[i].size(); j++) {
            cout << res[i][j];
            if (j < res[i].size() - 1) cout << ",";
        }
        cout << "]";
        if (i < res.size() - 1) cout << ",";
    }
    cout << "]";
}

int main() {
    Solution sol;

    vector<int> t1 = {1, 2, 3};
    vector<vector<int>> r1 = sol.subsets(t1);
    cout << "Test 1: "; printSubsets(r1);
    cout << (r1.size() == 8 ? "  PASS (8 subsets)" : "  FAIL") << endl;

    vector<int> t2 = {0};
    vector<vector<int>> r2 = sol.subsets(t2);
    cout << "Test 2: "; printSubsets(r2);
    cout << (r2.size() == 2 ? "  PASS (2 subsets)" : "  FAIL") << endl;

    vector<int> t3 = {1, 2};
    vector<vector<int>> r3 = sol.subsets(t3);
    cout << "Test 3: "; printSubsets(r3);
    cout << (r3.size() == 4 ? "  PASS (4 subsets)" : "  FAIL") << endl;

    return 0;
}
