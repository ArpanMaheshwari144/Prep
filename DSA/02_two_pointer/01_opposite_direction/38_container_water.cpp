// Q: Container With Most Water — do walls ke beech max paani kitna bhar sakta hai
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // Do walls ke beech maximum paani kitna bhar sakta hai
    // area = min(height[left], height[right]) * (right - left)
    // Chhoti wall wala pointer move kar
    int maxArea(vector<int>& height) {
        int n = height.size();
        // do pointer — ek left ek right se start
        int low = 0;
        int high = n-1;
        int max_area = INT_MIN;
        int ans = 0;

        while(low <= high){
            // area nikaalo — chhoti wall * distance
            int area = min(height[low], height[high]) * (high-low);
            // best area update karo
            max_area = max(max_area, area);
            // chhoti wall wala pointer hatao — badi wall se chance zyada
            if(height[low] < height[high]){
                low++;
            }
            else{
                high--;
            }
        }
        return max_area;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << "Test 1: " << sol.maxArea(t1);
    cout << (sol.maxArea(t1) == 49 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {1, 1};
    cout << "Test 2: " << sol.maxArea(t2);
    cout << (sol.maxArea(t2) == 1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {4, 3, 2, 1, 4};
    cout << "Test 3: " << sol.maxArea(t3);
    cout << (sol.maxArea(t3) == 16 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1, 2, 1};
    cout << "Test 4: " << sol.maxArea(t4);
    cout << (sol.maxArea(t4) == 2 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
