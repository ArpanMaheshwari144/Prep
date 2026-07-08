// ============================================================
// VALID PARENTHESES — Stack (pattern #1)
// ============================================================
// string s me sirf brackets: ( ) [ ] { }
// TRUE agar sab brackets sahi se MATCH + NESTED hain, warna FALSE.
//   "()[]{}" -> true   ·   "([)]" -> false
//
// (approach yahan LIKHA NAHI -- poora tera. copy-pen pe ek example trace pehle.)
//
// Tests (// expected):
//   "()"        -> 1   (true)
//   "()[]{}"    -> 1
//   "(]"        -> 0   (false)
//   "([)]"      -> 0
//   "{[]}"      -> 1
//   "("         -> 0
//   ""          -> 1   (empty = valid)
// ============================================================

#include <iostream>
#include <string>
#include <stack>
using namespace std;

bool isValid(string s)
{
    stack<char> st;
    for (auto &it : s)
    {
        if (it == '(' || it == '[' || it == '{')
        {
            st.push(it);
        }
        else
        {
            if (!st.empty())
            {
                char topp = st.top();
                st.pop();
                if ((topp == '(' && it == ')') || (topp == '[' && it == ']') || (topp == '{' && it == '}'))
                {
                    continue;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return st.empty();
}

int main()
{
    cout << isValid("()") << " (expected 1)\n";
    cout << isValid("()[]{}") << " (expected 1)\n";
    cout << isValid("(]") << " (expected 0)\n";
    cout << isValid("([)]") << " (expected 0)\n";
    cout << isValid("{[]}") << " (expected 1)\n";
    cout << isValid("(") << " (expected 0)\n";
    cout << isValid("") << " (expected 1)\n";
    cout << isValid(")") << " (expected 0)\n"; // closing bracket, kuch match karne ko nahi
    cout << isValid("]") << " (expected 0)\n";

    return 0;
}
