// Q: Wo saare subsets dhundho jinke sum = target
#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
    void solve(vector<int>& nums, int index, int target, int currentSum, vector<int>& current, vector<vector<int>>& result) {
        // base case — saare elements dekh liye
        if(index == nums.size()){
            // agar sum target ke barabar hai toh valid subset mila
            if(currentSum == target){
                result.push_back(current);
            }
            return;
        }

        // PICK — element lo aur sum mein add karo
        currentSum += nums[index];
        current.push_back(nums[index]);
        solve(nums, index+1, target, currentSum, current, result);

        // BACKTRACK — pick hatao aur sum se minus karo
        current.pop_back();
        currentSum -= nums[index];

        // NOT-PICK — ye element skip karo
        solve(nums, index+1, target, currentSum, current, result);
    }

    vector<vector<int>> subsetSum(vector<int>& nums, int target) {
        vector<vector<int>> result;
        vector<int> current;
        solve(nums, 0, target, 0, current, result);
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
    vector<vector<int>> r1 = sol.subsetSum(t1, 3);
    cout << "Test 1: target=3 → "; printSubsets(r1);
    cout << (r1.size() == 2 ? "  PASS (2 subsets)" : "  FAIL") << endl;

    vector<int> t2 = {1, 2, 3, 4};
    vector<vector<int>> r2 = sol.subsetSum(t2, 5);
    cout << "Test 2: target=5 → "; printSubsets(r2);
    cout << (r2.size() == 2 ? "  PASS (2 subsets)" : "  FAIL") << endl;

    vector<int> t3 = {1, 2, 3};
    vector<vector<int>> r3 = sol.subsetSum(t3, 10);
    cout << "Test 3: target=10 → "; printSubsets(r3);
    cout << (r3.size() == 0 ? "  PASS (0 subsets)" : "  FAIL") << endl;

    return 0;
}
