// Q: Array mein sabse bada element dhundho
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // Array ka sabse bada number return kar
    int findMax(vector<int>& nums) {
        int n = nums.size();
        // sabse chhota value se start karo taaki koi bhi element bada ho
        int maxNum = INT_MIN;
        // ye loop har element dekh raha
        for(int i=0;i<n;i++){
            // agar current element ab tak ke max se bada hai toh update karo
            if(nums[i] > maxNum){
                maxNum = nums[i];
            }
        }
        // final max return karo
        return maxNum;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {5, 3, 8, 1};
    cout << "Test 1: " << sol.findMax(t1);
    cout << (sol.findMax(t1) == 8 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {-3, -1, -7};
    cout << "Test 2: " << sol.findMax(t2);
    cout << (sol.findMax(t2) == -1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {42};
    cout << "Test 3: " << sol.findMax(t3);
    cout << (sol.findMax(t3) == 42 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
