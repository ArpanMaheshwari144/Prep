// Q: Longest substring jisme exactly k unique characters hon — variable sliding window
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
public:
    // Longest substring jisme exactly k unique characters hon
    // Agar k unique possible nahi toh -1 return kar
    int longestKUnique(string s, int k) {
        int maxAns = INT_MIN;
        int i=0, j=0, ans = 0;
        // map se unique characters aur frequency track karo
        unordered_map<int, int> mp;
        while (j < s.size())
        {
            // pehle add — naya character window mein daalo
            mp[s[j]]++;
            // shrink — jab unique characters k se zyada ho jaayein
            while(mp.size() > k){
                mp[s[i]]--;
                if(mp[s[i]] == 0){
                    mp.erase(s[i]);
                }
                i++;
            }
            // exactly k unique hain toh answer update karo
            if(mp.size() == k){
                ans = max(ans, j-i+1);
                maxAns = max(maxAns, ans);
            }
            j++;
        }
        // k unique kabhi possible nahi hua toh -1
        return maxAns == INT_MIN ? -1 : ans;

    }
};

int main() {
    Solution sol;

    cout << "Test 1: \"aabacbebebe\" k=3 → " << sol.longestKUnique("aabacbebebe", 3);
    cout << (sol.longestKUnique("aabacbebebe", 3) == 7 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: \"aaa\" k=1 → " << sol.longestKUnique("aaa", 1);
    cout << (sol.longestKUnique("aaa", 1) == 3 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: \"aaa\" k=2 → " << sol.longestKUnique("aaa", 2);
    cout << (sol.longestKUnique("aaa", 2) == -1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: \"abcba\" k=2 → " << sol.longestKUnique("abcba", 2);
    cout << (sol.longestKUnique("abcba", 2) == 3 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
