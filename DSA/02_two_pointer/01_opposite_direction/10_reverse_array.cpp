// Q: Array ko reverse karo — two pointer opposite direction se
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    void reverseArray(vector<int>& nums) {
        int n = nums.size();
        // ek pointer start pe, ek end pe
        int low = 0;
        int high = n-1;
        // jab tak pointers cross na karein, swap karte raho
        while(low < high){
            // dono ends ke elements swap karo
            swap(nums[low], nums[high]);
            // andar ki taraf badho
            low++;
            high--;
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

    vector<int> t1 = {1, 2, 3, 4, 5};
    vector<int> e1 = {5, 4, 3, 2, 1};
    sol.reverseArray(t1);
    cout << "Test 1: "; printArr(t1);
    cout << (checkEqual(t1, e1) ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 2};
    vector<int> e2 = {2, 1};
    sol.reverseArray(t2);
    cout << "Test 2: "; printArr(t2);
    cout << (checkEqual(t2, e2) ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {7};
    vector<int> e3 = {7};
    sol.reverseArray(t3);
    cout << "Test 3: "; printArr(t3);
    cout << (checkEqual(t3, e3) ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1, 2, 3, 4};
    vector<int> e4 = {4, 3, 2, 1};
    sol.reverseArray(t4);
    cout << "Test 4: "; printArr(t4);
    cout << (checkEqual(t4, e4) ? "  PASS" : "  FAIL") << endl;

    return 0;
}
