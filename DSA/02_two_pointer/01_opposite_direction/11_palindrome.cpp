// Q: String palindrome hai ya nahi check karo — two pointer se
#include <iostream>
#include <string>
using namespace std;

class Solution {
public:
    bool isPalindrome(string s) {
        int n = s.size();
        // start aur end se compare karo
        int low = 0;
        int high = n-1;

        while(low <= high){
            // agar dono side ke characters match nahi — palindrome nahi
            if(s[low] != s[high]){
                return false;
            }
            // dono pointers andar badho
            low++;
            high--;
        }
        // sab match ho gaye — palindrome hai
        return true;
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

    return 0;
}
