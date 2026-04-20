// Q: Do sorted arrays ko merge karo ek sorted array mein — two pointer peeche se
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // nums1 mein extra space hai end mein (0s)
    // m = nums1 ke real elements, n = nums2 ke elements
    // Dono merge karke nums1 mein sorted order mein daalo
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        // k = last position jahan likhna hai, peeche se bharenge
        int k = m+n-1;
        // i = nums1 ka last real element, j = nums2 ka last element
        int i = m-1;
        int j = n-1;

        // dono arrays ke end se compare karo, bada wala peeche daalo
        while(i >= 0 && j >= 0){
            if(nums1[i] <= nums2[j]){
                nums1[k] = nums2[j];
                j--;
                k--;
            }
            else if(nums1[i] >= nums2[j]){
                nums1[k] = nums1[i];
                i--;
                k--;
            }
        }
        // agar nums2 ke elements bache toh daal do
        while(j >= 0){
            nums1[k] = nums2[j];
            j--;
            k--;
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

    vector<int> t1 = {1, 3, 5, 0, 0, 0};
    vector<int> t1b = {2, 4, 6};
    vector<int> e1 = {1, 2, 3, 4, 5, 6};
    sol.merge(t1, 3, t1b, 3);
    cout << "Test 1: "; printArr(t1);
    cout << (checkEqual(t1, e1) ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 2, 3};
    vector<int> t2b = {};
    vector<int> e2 = {1, 2, 3};
    sol.merge(t2, 3, t2b, 0);
    cout << "Test 2: "; printArr(t2);
    cout << (checkEqual(t2, e2) ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {0};
    vector<int> t3b = {1};
    vector<int> e3 = {1};
    sol.merge(t3, 0, t3b, 1);
    cout << "Test 3: "; printArr(t3);
    cout << (checkEqual(t3, e3) ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {4, 5, 6, 0, 0, 0};
    vector<int> t4b = {1, 2, 3};
    vector<int> e4 = {1, 2, 3, 4, 5, 6};
    sol.merge(t4, 3, t4b, 3);
    cout << "Test 4: "; printArr(t4);
    cout << (checkEqual(t4, e4) ? "  PASS" : "  FAIL") << endl;

    return 0;
}
