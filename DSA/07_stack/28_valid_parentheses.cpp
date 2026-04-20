// Q: Brackets valid hain ya nahi check karo — stack se
#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Solution {
public:
    // String mein brackets hain: (), {}, []
    // Check kar — sab properly open aur close ho rahe hain ya nahi
    // "()" → true
    // "()[]{}" → true
    // "(]" → false
    // "({[]})" → true
    // "(((" → false
    bool isValid(string s) {
        // stack mein opening brackets store karo
        stack<char> st;
        int n = s.size();
        // har character pe jao
        for(char &ch : s){
            // opening bracket hai toh stack mein daal do
            if(ch == '(' || ch == '{' || ch == '['){
                st.push(ch);
            }
            else{
                // closing bracket aaya — check karo ki stack ka top matching opening hai
                if(st.empty() == false && st.top() == '(' && ch == ')'){
                    st.pop();
                }
                else if(st.empty() == false && st.top() == '{' && ch == '}'){
                    st.pop();
                }
                else if(st.empty() == false && st.top() == '[' && ch == ']'){
                    st.pop();
                }
            }
        }
        // stack khaali hai toh sab brackets match ho gaye
        return st.empty() ? true : false;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: \"()\" → " << (sol.isValid("()") ? "true" : "false");
    cout << (sol.isValid("()") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: \"()[]{}\" → " << (sol.isValid("()[]{}") ? "true" : "false");
    cout << (sol.isValid("()[]{}") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: \"(]\" → " << (sol.isValid("(]") ? "true" : "false");
    cout << (sol.isValid("(]") == false ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: \"({[]})\" → " << (sol.isValid("({[]})") ? "true" : "false");
    cout << (sol.isValid("({[]})") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: \"(((\" → " << (sol.isValid("(((") ? "true" : "false");
    cout << (sol.isValid("(((") == false ? "  PASS" : "  FAIL") << endl;

    // Edge: empty string
    cout << "Test 6: \"\" → " << (sol.isValid("") ? "true" : "false");
    cout << (sol.isValid("") == true ? "  PASS" : "  FAIL") << endl;

    cout << "Test 7: \"}{\" → " << (sol.isValid("}{") ? "true" : "false");
    cout << (sol.isValid("}{") == false ? "  PASS" : "  FAIL") << endl;

    return 0;
}
