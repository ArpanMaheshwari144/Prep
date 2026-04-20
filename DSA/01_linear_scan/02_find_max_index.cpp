// Q: Array mein sabse bade element ka INDEX dhundho
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int findMaxIndex(vector<int>& nums) {
        int maxNum = INT_MIN;
        // index -1 se start — agar array empty toh -1 return hoga
        int index = -1;
        // har element pe jao aur max track karo
        for(int i=0;i<nums.size();i++){
            // naya max mila toh value aur index dono update karo
            if(nums[i] > maxNum){
                maxNum = nums[i];
                index = i;
            }
        }
        // max element ka index return karo
        return index;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {5, 3, 8, 1, 9, 2};
    cout << "Test 1: " << sol.findMaxIndex(t1);
    cout << (sol.findMaxIndex(t1) == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {10, 5, 3};
    cout << "Test 2: " << sol.findMaxIndex(t2);
    cout << (sol.findMaxIndex(t2) == 0 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {1, 2, 3, 4};
    cout << "Test 3: " << sol.findMaxIndex(t3);
    cout << (sol.findMaxIndex(t3) == 3 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {7};
    cout << "Test 4: " << sol.findMaxIndex(t4);
    cout << (sol.findMaxIndex(t4) == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
