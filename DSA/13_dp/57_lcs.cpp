#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Solution
{
public:
    int solve(string &s1, string &s2, int i, int j, vector<vector<int>> &dp)
    {
        if (i < 0 || j < 0)
        {
            return 0;
        }

        if(dp[i][j] != -1){
            return dp[i][j];
        }

        int pick = INT_MIN;
        int not_pick = INT_MIN;
        if (s1[i] == s2[j])  // ya to pick hoga
        {
            pick = 1 + solve(s1, s2, i - 1, j - 1, dp);
        }
        else{ // ya pick nahi hoga
            not_pick = max(solve(s1, s2, i-1, j, dp), solve(s1, s2, i, j-1, dp));
        }
        return dp[i][j] = max(pick, not_pick);
    }

    int longestCommonSubsequence(string s1, string s2)
    {
        int m = s1.size(), n = s2.size();
        vector<vector<int>> dp(m, vector<int>(n, -1));
        return solve(s1, s2, m - 1, n - 1, dp);
    }
};

int main()
{
    Solution sol;

    cout << "Test 1: " << sol.longestCommonSubsequence("abcde", "ace");
    cout << (sol.longestCommonSubsequence("abcde", "ace") == 3 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: " << sol.longestCommonSubsequence("abc", "abc");
    cout << (sol.longestCommonSubsequence("abc", "abc") == 3 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: " << sol.longestCommonSubsequence("abc", "def");
    cout << (sol.longestCommonSubsequence("abc", "def") == 0 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: " << sol.longestCommonSubsequence("abcba", "abcbcba");
    cout << (sol.longestCommonSubsequence("abcba", "abcbcba") == 5 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: " << sol.longestCommonSubsequence("a", "a");
    cout << (sol.longestCommonSubsequence("a", "a") == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
