// Q: Array se given element (val) ke saare occurrences hatao in-place
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int n = nums.size();
        // slow — aage likho wali position
        int slow = 0;
        int fast = 0;
        while(fast < n){
            // agar ye element val nahi hai toh rakh lo
            if(nums[fast] != val){
                nums[slow] = nums[fast];
                slow++;
            }
            // val hai toh skip — fast aage badh jaayega
            fast++;
        }
        // slow = kitne elements bache
        return slow;

    }
};

int main() {
    Solution sol;

    vector<int> t1 = {3, 2, 2, 3};
    int k1 = sol.removeElement(t1, 3);
    cout << "Test 1: k=" << k1;
    cout << (k1 == 2 && t1[0]==2 && t1[1]==2 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {0, 1, 2, 2, 3, 0, 4, 2};
    int k2 = sol.removeElement(t2, 2);
    cout << "Test 2: k=" << k2;
    cout << (k2 == 5 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {1, 2, 3};
    int k3 = sol.removeElement(t3, 5);
    cout << "Test 3: k=" << k3;
    cout << (k3 == 3 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {4, 4, 4};
    int k4 = sol.removeElement(t4, 4);
    cout << "Test 4: k=" << k4;
    cout << (k4 == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
