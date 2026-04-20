#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Solution {
public:
    int maxSumK(vector<int>& nums, int k) {
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

    vector<int> t1 = {3, 1, -2, 5, 4, -7};
    cout << "Test 1: " << sol.maxSumK(t1, 3);
    cout << (sol.maxSumK(t1, 3) == 7 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 4, 2, 10, 23, 3, 1, 0, 20};
    cout << "Test 2: " << sol.maxSumK(t2, 4);
    cout << (sol.maxSumK(t2, 4) == 39 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {-1, -2, -3, -4};
    cout << "Test 3: " << sol.maxSumK(t3, 2);
    cout << (sol.maxSumK(t3, 2) == -3 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {5};
    cout << "Test 4: " << sol.maxSumK(t4, 1);
    cout << (sol.maxSumK(t4, 1) == 5 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
