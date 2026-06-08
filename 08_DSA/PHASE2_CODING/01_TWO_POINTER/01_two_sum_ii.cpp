// ============================================================
// TWO SUM II - Input Array Is Sorted        (Pattern: Two Pointer)
// ============================================================
// Ek 1-indexed SORTED array `numbers` aur ek `target` diya hai.
// Do numbers ke 1-based indices [i, j] return karo jinka sum = target.
// Exactly ek solution hai; har element ek hi baar use.
//
// Example:
//   numbers = [2, 7, 11, 15], target = 9   ->  [1, 2]   (2 + 7 = 9)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int n = numbers.size();
        int i = 0;
        int j = n-1;

        while(i < j){
            if(numbers[i] + numbers[j] == target){
                return vector<int>{i+1, j+1};
            }
            else if(numbers[i] + numbers[j] < target){
                i++;
            }
            else if(numbers[i] + numbers[j] > target){
                j--;
            }
        }
        return vector<int>{-1, -1};
    }
};

int main() {
    Solution s;
    vector<int> numbers = {2, 7, 11, 15};
    int target = 9;
    vector<int> ans = s.twoSum(numbers, target);

    cout << "[";
    for (int i = 0; i < (int)ans.size(); i++)
        cout << ans[i] << (i + 1 < (int)ans.size() ? ", " : "");
    cout << "]" << endl;   // expected: [1, 2]
    return 0;
}
