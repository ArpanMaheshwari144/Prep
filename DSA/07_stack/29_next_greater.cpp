// Q: Har element ka next greater element dhundho — monotonic stack se
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Solution {
public:
    // Har element ke liye uske right mein sabse pehla bada element dhundh
    // Nahi mila toh -1
    // Example: [4,5,2,10,8] → [5,10,10,-1,-1]
    vector<int> nextGreater(vector<int>& nums) {
        int n = nums.size();
        // stack mein indices rakhenge — monotonic decreasing stack
        stack<int> st;
        // default -1 — agar koi bada nahi mila toh -1 rahega
        vector<int> ans(nums.size(), -1);
        for(int i=0;i<n;i++){
            // jab tak stack top se bada element aaye — unka answer ye hai
            while(!st.empty() && nums[st.top()] < nums[i]){
                ans[st.top()] = nums[i];
                st.pop();
            }
            // current index stack mein daalo
            st.push(i);
        }
        return ans;
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

int main() {
    Solution sol;

    vector<int> t1 = {4, 5, 2, 10, 8};
    vector<int> r1 = sol.nextGreater(t1);
    cout << "Test 1: "; printArr(r1);
    cout << (r1[0]==5 && r1[1]==10 && r1[2]==10 && r1[3]==-1 && r1[4]==-1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 2, 3, 4};
    vector<int> r2 = sol.nextGreater(t2);
    cout << "Test 2: "; printArr(r2);
    cout << (r2[0]==2 && r2[1]==3 && r2[2]==4 && r2[3]==-1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {4, 3, 2, 1};
    vector<int> r3 = sol.nextGreater(t3);
    cout << "Test 3: "; printArr(r3);
    cout << (r3[0]==-1 && r3[1]==-1 && r3[2]==-1 && r3[3]==-1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {5};
    vector<int> r4 = sol.nextGreater(t4);
    cout << "Test 4: "; printArr(r4);
    cout << (r4[0]==-1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
