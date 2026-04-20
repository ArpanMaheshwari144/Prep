// Q: k consecutive elements ka maximum average dhundho — fixed size window
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Solution {
public:
    // k consecutive elements ka maximum average
    double maxAvgSubarray(vector<int>& nums, int k) {
        double maxAvg = -1e9;
        double avg = 0;
        int sum = 0, i=0, j=0;
        while(j < nums.size()){
            // pehle add — naya element window mein
            sum += nums[j];
            // window badi ho gayi toh shrink karo
            if(j-i+1 > k){
                sum -= nums[i];
                i++;
            }
            // window size k ho gayi toh average nikaal ke update karo
            if(j-i+1 == k){
                avg = (double)sum/(j-i+1);
                maxAvg = max(maxAvg, avg);
            }
            j++;
        }
        return maxAvg;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {1, 12, -5, -6, 50, 3};
    cout << "Test 1: " << sol.maxAvgSubarray(t1, 4);
    cout << (sol.maxAvgSubarray(t1, 4) == 12.75 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {5};
    cout << "Test 2: " << sol.maxAvgSubarray(t2, 1);
    cout << (sol.maxAvgSubarray(t2, 1) == 5.0 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {0, 4, 0, 3, 2};
    cout << "Test 3: " << sol.maxAvgSubarray(t3, 1);
    cout << (sol.maxAvgSubarray(t3, 1) == 4.0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
