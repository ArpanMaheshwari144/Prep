// Q: Array ke saare elements ka sum nikalo
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int arraySum(vector<int>& nums) {
        // sum 0 se start karo
        int sum = 0;
        // har element ko sum mein add karte jao
        for(int i=0;i<nums.size();i++){
            sum += nums[i];
        }
        // total sum return karo
        return sum;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {5, 3, 8, 1, 9, 2};
    cout << "Test 1: " << sol.arraySum(t1);
    cout << (sol.arraySum(t1) == 28 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {-1, -2, -3};
    cout << "Test 2: " << sol.arraySum(t2);
    cout << (sol.arraySum(t2) == -6 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {10};
    cout << "Test 3: " << sol.arraySum(t3);
    cout << (sol.arraySum(t3) == 10 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {0, 0, 0};
    cout << "Test 4: " << sol.arraySum(t4);
    cout << (sol.arraySum(t4) == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
