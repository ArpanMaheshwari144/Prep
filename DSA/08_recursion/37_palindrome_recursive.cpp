// Q: String palindrome hai ya nahi — recursion se reverse karke check karo
#include <iostream>
#include <string>
using namespace std;

class Solution {
public:
    // pehle string reverse karo recursively
    string reverseStr(string s) {
        // base case — khaali string
        if(s.size() == 0){
            return "";
        }

        // base case — single character
        if(s.size() == 1){
            return s;
        }

        // baaki reverse karo aur pehla character end mein lagao
        string ans = reverseStr(s.substr(1)) + s[0];
        return ans;
    }

    bool isPalindrome(string s) {
        // reverse karke original se compare karo
        string ans = reverseStr(s);

        // dono same hain toh palindrome hai
        if(s == ans){
            return true;
        }
        return false;

    }
};

int main() {
    Solution sol;

    cout << "Test 1: \"racecar\" → " << (sol.isPalindrome("racecar") ? "true" : "false");
    cout << (sol.isPalindrome("racecar") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: \"hello\" → " << (sol.isPalindrome("hello") ? "true" : "false");
    cout << (sol.isPalindrome("hello") == false ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: \"abba\" → " << (sol.isPalindrome("abba") ? "true" : "false");
    cout << (sol.isPalindrome("abba") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: \"a\" → " << (sol.isPalindrome("a") ? "true" : "false");
    cout << (sol.isPalindrome("a") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: \"\" → " << (sol.isPalindrome("") ? "true" : "false");
    cout << (sol.isPalindrome("") == true ? "  PASS" : "  FAIL") << endl;

    return 0;
}
