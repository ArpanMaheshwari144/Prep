// ============================================================
// EVALUATE REVERSE POLISH NOTATION (RPN / postfix)   —   Stack
// ============================================================
// Tokens (numbers + operators + - * /) — postfix expression. Evaluate -> int result.
//
// Example:
//   {"2","1","+","3","*"}    -> 9      ((2+1)*3)
//   {"4","13","5","/","+"}   -> 6      (4 + 13/5 = 4+2 = 6)
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   stack<int>.  Token ek-ek dekho:
//     - operator (+ - * /)  ->  b = pop (top, 2nd operand), a = pop (1st operand),
//                               a (op) b karo, result PUSH.  (order: a-b, a/b — dhyaan!)
//     - number string       ->  st.push(stoi(token))   // string->int = stoi
//   End mein stack mein ek number bacha = answer.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int operate(int a, int b, string token)
{
    if (token == "+")
    {
        return a + b;
    }
    else if (token == "-")
    {
        return a - b;
    }
    else if (token == "*")
    {
        return a * b;
    }
    else if (token == "/")
    {
        return a / b;
    }
    return -1;
}

int evalRPN(vector<string> &tokens)
{
    stack<int> st;

    for (auto &it : tokens)
    {
        if (it == "+" || it == "-" || it == "*" || it == "/")
        {
            int b = st.top();
            st.pop();

            int a = st.top();
            st.pop();

            st.push(operate(a, b, it));
        }
        else
        {
            st.push(stoi(it));
        }
    }
    return st.top();
}

int main()
{
    vector<string> t1 = {"2", "1", "+", "3", "*"};
    cout << evalRPN(t1) << endl; // 9

    vector<string> t2 = {"4", "13", "5", "/", "+"};
    cout << evalRPN(t2) << endl; // 6

    vector<string> t3 = {"5", "1", "2", "+", "4", "*", "+", "3", "-"};
    cout << evalRPN(t3) << endl; // 14
    return 0;
}
