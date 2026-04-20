// Q: Sorted array se duplicates hatao in-place — same direction two pointer
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int n = nums.size();
        // slow — unique elements ka last index
        int slow = 0;
        // fast — aage jaake naye elements dhundhta hai
        int fast = 0;
        while(fast < n){
            // naya unique element mila — slow ke aage rakh do
            if(nums[slow] != nums[fast]){
                slow++;
                nums[slow] = nums[fast];
            }
            // fast hamesha aage badhta hai
            fast++;
        }
        // slow+1 = total unique elements ki count
        return slow+1;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {1, 1, 2, 2, 3, 4, 4, 5};
    int k1 = sol.removeDuplicates(t1);
    cout << "Test 1: k=" << k1;
    cout << (k1 == 5 && t1[0]==1 && t1[1]==2 && t1[2]==3 && t1[3]==4 && t1[4]==5 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 1, 1};
    int k2 = sol.removeDuplicates(t2);
    cout << "Test 2: k=" << k2;
    cout << (k2 == 1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {1, 2, 3};
    int k3 = sol.removeDuplicates(t3);
    cout << "Test 3: k=" << k3;
    cout << (k3 == 3 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {-3, -1, -1, 0, 0, 2};
    int k4 = sol.removeDuplicates(t4);
    cout << "Test 4: k=" << k4;
    cout << (k4 == 4 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
