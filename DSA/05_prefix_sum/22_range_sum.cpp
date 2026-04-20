// Q: Range sum query — L se R tak ka sum O(1) mein nikalo prefix sum se
#include <iostream>
#include <vector>
using namespace std;

class NumArray {
public:
    vector<int> prefix;

    // Constructor — prefix sum array bana
    NumArray(vector<int>& nums) {
        int n = nums.size();
        prefix.resize(n, 0);
        // pehla element wahi rahega
        prefix[0] = nums[0];
        // har position pe usse pehle tak ka total sum store karo
        for(int i=1;i<n;i++){
            prefix[i] = prefix[i-1]+nums[i];
        }
    }

    // L se R tak ka sum return kar (inclusive)
    // L=0 toh prefix[R]
    // L>0 toh prefix[R] - prefix[L-1]
    int sumRange(int left, int right) {
        // agar left 0 hai toh seedha prefix[right] answer hai
        if(left == 0){
            return prefix[right];
        }
        // nahi toh total minus left se pehle ka hissa
        return prefix[right] - prefix[left-1];
    }
};

int main() {
    vector<int> nums = {-2, 0, 3, -5, 2, -1};
    NumArray obj(nums);

    cout << "Test 1: sum(0,2) → " << obj.sumRange(0, 2);
    cout << (obj.sumRange(0, 2) == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: sum(2,5) → " << obj.sumRange(2, 5);
    cout << (obj.sumRange(2, 5) == -1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: sum(0,5) → " << obj.sumRange(0, 5);
    cout << (obj.sumRange(0, 5) == -3 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: sum(3,3) → " << obj.sumRange(3, 3);
    cout << (obj.sumRange(3, 3) == -5 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: sum(0,0) → " << obj.sumRange(0, 0);
    cout << (obj.sumRange(0, 0) == -2 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
