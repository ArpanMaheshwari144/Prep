// Q: Sorted array mein target ki pehli aur aakhri position dhundho — 2 binary search
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // Sorted array mein target ki PEHLI aur AAKHRI position dhundh
    // Nahi mila toh {-1, -1}
    // Example: [5,7,7,8,8,10] target=8 → {3,4}
    // Hint: 2 binary search — pehla left dhundhe, dusra right dhundhe
    //       Target milne pe RETURN MAT KARO — aur dhundhte raho
    //       Pehli position: target mila → ans save, high=mid-1 (aur left jao)
    //       Aakhri position: target mila → ans save, low=mid+1 (aur right jao)
    vector<int> searchRange(vector<int>& nums, int target) {
        vector<int> ans(2, -1);
        int n = nums.size();

        // --- Pehli position dhundho (leftmost) ---
        int low = 0;
        int high = n-1;
        while(low <= high){
            int mid = (low + (high-low)/2);
            // target mila — save karo par aur left jao (pehli position chahiye)
            if(nums[mid] == target){
                ans[0] = mid;
                high = mid-1;
            }
            else if(nums[mid] < target){
                low = mid+1;
            }
            else{
                high = mid-1;
            }
        }

        // --- Aakhri position dhundho (rightmost) ---
        low = 0;
        high = n-1;
        while(low <= high){
            int mid = (low + (high-low)/2);
            // target mila — save karo par aur right jao (aakhri position chahiye)
            if(nums[mid] == target){
                ans[1] = mid;
                low = mid+1;
            }
            else if(nums[mid] < target){
                low = mid+1;
            }
            else{
                high = mid-1;
            }
        }
        return ans;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {5, 7, 7, 8, 8, 10};
    vector<int> r1 = sol.searchRange(t1, 8);
    cout << "Test 1: [" << r1[0] << "," << r1[1] << "]";
    cout << (r1[0] == 3 && r1[1] == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> r2 = sol.searchRange(t1, 6);
    cout << "Test 2: [" << r2[0] << "," << r2[1] << "]";
    cout << (r2[0] == -1 && r2[1] == -1 ? "  PASS" : "  FAIL") << endl;

    // Edge: empty
    vector<int> t3 = {};
    vector<int> r3 = sol.searchRange(t3, 0);
    cout << "Test 3: [" << r3[0] << "," << r3[1] << "]";
    cout << (r3[0] == -1 && r3[1] == -1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1, 1, 1, 1, 1};
    vector<int> r4 = sol.searchRange(t4, 1);
    cout << "Test 4: [" << r4[0] << "," << r4[1] << "]";
    cout << (r4[0] == 0 && r4[1] == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {1, 3, 5, 7, 9};
    vector<int> r5 = sol.searchRange(t5, 1);
    cout << "Test 5: [" << r5[0] << "," << r5[1] << "]";
    cout << (r5[0] == 0 && r5[1] == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
