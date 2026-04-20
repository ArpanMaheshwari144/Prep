#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int firstUniqChar(string s) {
        unordered_map<int, int> mp;
        for(auto &it : s){
            mp[it]++;
        }

        for(int i=0;i<s.size();i++){
            if(mp[s[i]] == 1){
                return i;
            }
        }
        return -1;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: " << sol.firstUniqChar("hello");
    cout << (sol.firstUniqChar("hello") == 0 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: " << sol.firstUniqChar("aabb");
    cout << (sol.firstUniqChar("aabb") == -1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: " << sol.firstUniqChar("leetcode");
    cout << (sol.firstUniqChar("leetcode") == 0 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: " << sol.firstUniqChar("aadadaad");
    cout << (sol.firstUniqChar("aadadaad") == -1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: " << sol.firstUniqChar("z");
    cout << (sol.firstUniqChar("z") == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
