// Q: k consecutive elements ka maximum sum dhundho — fixed size sliding window
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Solution {
public:
    // k consecutive elements ka maximum sum
    int maxSumSubarray(vector<int>& nums, int k) {
        int maxSum = INT_MIN;
        int i =0, j=0;
        int sum = 0;
        while(j < nums.size()){
            // pehle add — naya element window mein daalo
            sum += nums[j];
            // window size k se badi ho gayi toh shrink — left se nikalo
            if(j-i+1 > k){
                sum -= nums[i];
                i++;
            }
            // window exactly k size ki hai toh answer update karo
            if(j-i+1 == k){
                maxSum = max(maxSum, sum);
            }
            j++;
        }
        return maxSum;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {2, 1, 5, 1, 3, 2};
    cout << "Test 1: " << sol.maxSumSubarray(t1, 3);
    cout << (sol.maxSumSubarray(t1, 3) == 9 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 2, 3, 4, 5};
    cout << "Test 2: " << sol.maxSumSubarray(t2, 2);
    cout << (sol.maxSumSubarray(t2, 2) == 9 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {5, 5, 5, 5};
    cout << "Test 3: " << sol.maxSumSubarray(t3, 2);
    cout << (sol.maxSumSubarray(t3, 2) == 10 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1, 2, 3};
    cout << "Test 4: " << sol.maxSumSubarray(t4, 3);
    cout << (sol.maxSumSubarray(t4, 3) == 6 ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {-1, 4, -2, 5, -3};
    cout << "Test 5: " << sol.maxSumSubarray(t5, 3);
    cout << (sol.maxSumSubarray(t5, 3) == 7 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
