// Q: String ko reverse karo recursion se
#include <iostream>
#include <string>
using namespace std;

class Solution {
public:
    // String reverse kar recursively
    // Base case: string khaali ya 1 character → return
    string reverseStr(string s) {
        // base case — khaali string toh khaali return
        if(s.size() == 0){
            return "";
        }

        // base case — ek character toh wahi return
        if(s.size() == 1){
            return s;
        }

        // pehla character hata ke baaki reverse karo, phir pehla end mein lagao
        string ans = reverseStr(s.substr(1)) + s[0];
        return ans;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: " << sol.reverseStr("hello");
    cout << (sol.reverseStr("hello") == "olleh" ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: " << sol.reverseStr("a");
    cout << (sol.reverseStr("a") == "a" ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: " << sol.reverseStr("ab");
    cout << (sol.reverseStr("ab") == "ba" ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: " << sol.reverseStr("");
    cout << (sol.reverseStr("") == "" ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: " << sol.reverseStr("racecar");
    cout << (sol.reverseStr("racecar") == "racecar" ? "  PASS" : "  FAIL") << endl;

    return 0;
}
