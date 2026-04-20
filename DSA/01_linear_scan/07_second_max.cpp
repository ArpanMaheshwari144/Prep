// Q: Array mein second largest element dhundho
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Solution {
public:
    int secondMax(vector<int>& nums) {
        // dono max aur secondMax sabse chhote se start
        int maxNum = INT_MIN;
        int secondMaxNum = INT_MIN;
        for(int i=0;i<nums.size();i++){
            // naya max mila — purana max ab secondMax ban gaya
            if(nums[i] > maxNum){
                secondMaxNum = maxNum;
                maxNum = nums[i];
            }
            // max nahi par secondMax se bada hai aur max ke equal bhi nahi
            else if(nums[i] > secondMaxNum && maxNum != nums[i]){
                secondMaxNum = nums[i];
            }
        }
        // second sabse bada return karo
        return secondMaxNum;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {5, 3, 8, 1, 9, 2};
    cout << "Test 1: " << sol.secondMax(t1);
    cout << (sol.secondMax(t1) == 8 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {-3, -1, -7, -2};
    cout << "Test 2: " << sol.secondMax(t2);
    cout << (sol.secondMax(t2) == -2 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {9, 5, 9, 3};
    cout << "Test 3: " << sol.secondMax(t3);
    cout << (sol.secondMax(t3) == 5 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {5, 5, 5};
    cout << "Test 4: " << sol.secondMax(t4);
    cout << (sol.secondMax(t4) == INT_MIN ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {1, 2};
    cout << "Test 5: " << sol.secondMax(t5);
    cout << (sol.secondMax(t5) == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
