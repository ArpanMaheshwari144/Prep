// Q: Kadane's Algorithm — maximum subarray sum dhundho
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Solution {
public:
    int maxSubarraySum(vector<int>& nums) {
       // maxSum track karega best answer ab tak
       int maxSum = INT_MIN;
       // sum current subarray ka running sum hai
       int sum = 0;
       for(int i=0;i<nums.size();i++){
        // current element add karo
        sum += nums[i];
        // ab tak ka best sum update karo
        maxSum = max(maxSum, sum);
        // agar sum negative ho gaya toh naya subarray start karo
        if(sum < 0){
            sum = 0;
        }
       }
       return maxSum;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    cout << "Test 1: " << sol.maxSubarraySum(t1);
    cout << (sol.maxSubarraySum(t1) == 6 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {-3, -1, -4, -2};
    cout << "Test 2: " << sol.maxSubarraySum(t2);
    cout << (sol.maxSubarraySum(t2) == -1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {1, 2, 3, 4};
    cout << "Test 3: " << sol.maxSubarraySum(t3);
    cout << (sol.maxSubarraySum(t3) == 10 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {5};
    cout << "Test 4: " << sol.maxSubarraySum(t4);
    cout << (sol.maxSubarraySum(t4) == 5 ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {-1, -2, 5, 3};
    cout << "Test 5: " << sol.maxSubarraySum(t5);
    cout << (sol.maxSubarraySum(t5) == 8 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
