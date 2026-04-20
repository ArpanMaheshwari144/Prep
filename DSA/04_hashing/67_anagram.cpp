#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
public:
    bool isAnagram(string s, string t) {
        unordered_map<int, int> mp;
        for(auto &it : s){
            mp[it]++;
        }

        for(auto &it : t){
            mp[it]--;
            if(mp[it] == 0){
                mp.erase(it);
            }
        }
        return mp.empty();
    }
};

int main() {
    Solution sol;

    cout << "Test 1: " << (sol.isAnagram("listen", "silent") ? "true" : "false");
    cout << (sol.isAnagram("listen", "silent") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: " << (sol.isAnagram("hello", "world") ? "true" : "false");
    cout << (sol.isAnagram("hello", "world") == false ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: " << (sol.isAnagram("anagram", "nagaram") ? "true" : "false");
    cout << (sol.isAnagram("anagram", "nagaram") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: " << (sol.isAnagram("ab", "a") ? "true" : "false");
    cout << (sol.isAnagram("ab", "a") == false ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: " << (sol.isAnagram("a", "a") ? "true" : "false");
    cout << (sol.isAnagram("a", "a") == true ? "  PASS" : "  FAIL") << endl;

    return 0;
}
