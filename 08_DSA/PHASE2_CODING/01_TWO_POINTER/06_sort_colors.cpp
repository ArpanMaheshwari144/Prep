// ============================================================
// SORT COLORS  (Dutch National Flag)
// ============================================================
// Array me sirf 0, 1, 2. IN-PLACE sort karo (0s -> 1s -> 2s).
// Library sort() NAHI. nums ko hi modify karo.
//
// SOCH (seed):
//   2-pass: count 0/1/2 -> array dobara bhar do.  (chalega)
//   1-pass: low(0-zone boundary), high(2-zone boundary), mid(scanner) -> swap+move.
//
// Tests (// expected):
//   [2,0,2,1,1,0]   -> 0 0 1 1 2 2
//   [2,0,1]         -> 0 1 2
//   [0]             -> 0
//   [1,1,1]         -> 1 1 1
//   [2,2,1,0,0]     -> 0 0 1 2 2
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

void sortColors(vector<int>& nums) {
    int n = nums.size();
    int low = 0;
    int mid = 0;
    int high = nums.size()-1;

    while(mid <= high){
        if(nums[mid] == 1){
            mid++;
        }
        else if(nums[mid] == 0){
            swap(nums[low], nums[mid]);
            low++;
            mid++;
        }
        else{
            swap(nums[mid], nums[high]);
            high--;
        }
    }
}

void print(vector<int>& v) {
    for (int x : v) cout << x << " ";
    cout << "\n";
}

int main() {
    vector<int> a1 = {2,0,2,1,1,0};
    vector<int> a2 = {2,0,1};
    vector<int> a3 = {0};
    vector<int> a4 = {1,1,1};
    vector<int> a5 = {2,2,1,0,0};

    sortColors(a1); print(a1);  // expected: 0 0 1 1 2 2
    sortColors(a2); print(a2);  // expected: 0 1 2
    sortColors(a3); print(a3);  // expected: 0
    sortColors(a4); print(a4);  // expected: 1 1 1
    sortColors(a5); print(a5);  // expected: 0 0 1 2 2
    return 0;
}
