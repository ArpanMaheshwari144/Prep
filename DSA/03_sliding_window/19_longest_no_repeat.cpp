// Q: Longest substring bina kisi repeating character ke — variable size sliding window
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
public:
    // Longest substring jisme koi character repeat na ho
    int lengthOfLongestSubstring(string s) {
        int ans = INT_MIN;
        int i = 0;
        int j = 0;
        // map mein har character ki frequency track karo
        unordered_map<int, int> mp;
        while(j < s.size()){
            // Step 1: add — naya character window mein daalo
            mp[s[j]]++;

            // Step 2: shrink — jab tak repeat hai, left se nikaalte jao
            // i pe same character ho ya alag — farak nahi padta
            // bas nikaalte jao jab tak repeat khatam na ho
            // while loop khud handle karega — beech ke sab elements nikal jayenge
            while (mp[s[j]] > 1)
            {
                mp[s[i]]--;
                // frequency 0 ho gayi toh map se hatao
                if(mp[s[i]] == 0){
                    mp.erase(s[i]);
                }
                i++;
            }

            // Step 3: update — shrink ke BAAD answer update karo
            ans = max(ans, j-i+1);
            j++;
        }
        // agar string khaali thi toh 0 return karo
        return ans == INT_MIN ? 0 : ans;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: \"abcabcbb\" → " << sol.lengthOfLongestSubstring("abcabcbb");
    cout << (sol.lengthOfLongestSubstring("abcabcbb") == 3 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: \"bbbbb\" → " << sol.lengthOfLongestSubstring("bbbbb");
    cout << (sol.lengthOfLongestSubstring("bbbbb") == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: \"pwwkew\" → " << sol.lengthOfLongestSubstring("pwwkew");
    cout << (sol.lengthOfLongestSubstring("pwwkew") == 3 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: \"\" → " << sol.lengthOfLongestSubstring("");
    cout << (sol.lengthOfLongestSubstring("") == 0 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: \"abcdef\" → " << sol.lengthOfLongestSubstring("abcdef");
    cout << (sol.lengthOfLongestSubstring("abcdef") == 6 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
