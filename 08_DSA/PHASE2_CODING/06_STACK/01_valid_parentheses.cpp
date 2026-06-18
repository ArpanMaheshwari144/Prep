// ============================================================
// VALID PARENTHESES   —   Stack (LIFO)
// ============================================================
// String mein ( ) { } [ ] brackets. Valid hai? (sahi type + sahi order mein band).
//
// Example:
//   "()"      -> true       "(]"   -> false  (galat type)
//   "()[]{}"  -> true       "([)]" -> false  (galat order / nesting tooti)
//   "{[]}"    -> true
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   LAST khula bracket PEHLE band hona chahiye = LIFO = STACK (plates ki dher).
//     - opening ( { [  ->  stack mein PUSH
//     - closing ) } ]  ->  stack khaali? -> false
//                          warna TOP match karta hai? -> pop : return false
//     - end mein stack KHAALI -> true (sab pairs band), warna false
//   (Match: '(' <-> ')',  '{' <-> '}',  '[' <-> ']')
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isValid(string s)
{
    stack<char> st;
    for (auto &it : s) // ()
    {
        if (it == '[' || it == '{' || it == '(')
        {
            st.push(it);
        }
        else
        {
            if (st.empty())
            {
                return false;
            }

            char topp = st.top();
            st.pop();
            if ((it == ')' && topp == '(') || (it == ']' && topp == '[') || (it == '}' && topp == '{'))
            {
                continue;
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
    cout << boolalpha << isValid("()") << endl;     // true
    cout << boolalpha << isValid("()[]{}") << endl; // true
    cout << boolalpha << isValid("(]") << endl;     // false
    cout << boolalpha << isValid("([)]") << endl;   // false
    cout << boolalpha << isValid("{[]}") << endl;   // true
    return 0;
}
