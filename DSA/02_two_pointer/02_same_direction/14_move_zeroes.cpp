// Q: Saare zeros end mein le jao — baaki elements ka order mat bigado
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int n = nums.size();
        // slow — next non-zero element yahan aayega
        int slow = 0;
        int fast = 0;
        while(fast < n){
            // non-zero mila toh slow ki jagah rakh do aur swap karo
            if(nums[fast] != 0){
                swap(nums[slow], nums[fast]);
                slow++;
            }
            // fast hamesha aage
            fast++;
        }
    }
};

void printArr(vector<int>& v) {
    cout << "[";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i < v.size() - 1) cout << ",";
    }
    cout << "]";
}

bool checkEqual(vector<int>& a, vector<int>& b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < a.size(); i++) if (a[i] != b[i]) return false;
    return true;
}

int main() {
    Solution sol;

    vector<int> t1 = {0, 1, 0, 3, 12};
    vector<int> e1 = {1, 3, 12, 0, 0};
    sol.moveZeroes(t1);
    cout << "Test 1: "; printArr(t1);
    cout << (checkEqual(t1, e1) ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {0, 0, 0, 1};
    vector<int> e2 = {1, 0, 0, 0};
    sol.moveZeroes(t2);
    cout << "Test 2: "; printArr(t2);
    cout << (checkEqual(t2, e2) ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {1, 2, 3};
    vector<int> e3 = {1, 2, 3};
    sol.moveZeroes(t3);
    cout << "Test 3: "; printArr(t3);
    cout << (checkEqual(t3, e3) ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {0, 0, 0};
    vector<int> e4 = {0, 0, 0};
    sol.moveZeroes(t4);
    cout << "Test 4: "; printArr(t4);
    cout << (checkEqual(t4, e4) ? "  PASS" : "  FAIL") << endl;

    return 0;
}
