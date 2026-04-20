// Q: Array mein target kitni baar aaya hai count karo
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int countTarget(vector<int>& nums, int target) {
        int count = 0;
        // har element pe jao
        for(int i=0;i<nums.size();i++){
            // agar target se match kiya toh count badhao
            if(nums[i] == target){
                count++;
            }
        }
        // total count return karo
        return count;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {3, 7, 1, 9, 4, 9, 2};
    cout << "Test 1: " << sol.countTarget(t1, 9);
    cout << (sol.countTarget(t1, 9) == 2 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 1, 1, 1};
    cout << "Test 2: " << sol.countTarget(t2, 1);
    cout << (sol.countTarget(t2, 1) == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {5, 3, 8};
    cout << "Test 3: " << sol.countTarget(t3, 7);
    cout << (sol.countTarget(t3, 7) == 0 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {2};
    cout << "Test 4: " << sol.countTarget(t4, 2);
    cout << (sol.countTarget(t4, 2) == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
