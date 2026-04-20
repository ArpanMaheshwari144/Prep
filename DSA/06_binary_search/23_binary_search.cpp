// Q: Sorted array mein target dhundho — basic binary search
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // Sorted array mein target dhundh. Index return kar. Nahi mila toh -1.
    int search(vector<int>& nums, int target) {
        int n = nums.size();
        int low = 0;
        int high = n-1;
        // <= lagao tab jab dhudhna ho kahi sab element dekhne hai
        while(low <= high){
            // mid nikaalo — overflow safe tarike se
            int mid = (low + (high-low)/2);
            // mil gaya toh return karo
            if(nums[mid] == target){
                return mid;
            }
            // mid chhota hai toh right half mein dhundho
            else if(nums[mid] < target){
                low = mid+1;
            }
            // mid bada hai toh left half mein dhundho
            else{
                high = mid-1;
            }
        }
        // nahi mila toh -1
        return -1;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {-1, 0, 3, 5, 9, 12};
    cout << "Test 1: target=9 → " << sol.search(t1, 9);
    cout << (sol.search(t1, 9) == 4 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: target=2 → " << sol.search(t1, 2);
    cout << (sol.search(t1, 2) == -1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {5};
    cout << "Test 3: target=5 → " << sol.search(t3, 5);
    cout << (sol.search(t3, 5) == 0 ? "  PASS" : "  FAIL") << endl;

    // Edge: empty array
    vector<int> t4 = {};
    cout << "Test 4: empty → " << sol.search(t4, 1);
    cout << (sol.search(t4, 1) == -1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "Test 5: target=1 → " << sol.search(t5, 1);
    cout << (sol.search(t5, 1) == 0 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 6: target=10 → " << sol.search(t5, 10);
    cout << (sol.search(t5, 10) == 9 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
