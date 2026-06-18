// ============================================================
// MIN STACK   —   Stack design (getMin in O(1))
// ============================================================
// Ek stack jo push, pop, top, getMin — SAB O(1) mein de.
// getMin() = stack ka sabse chhota element (scan nahi).
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   1 stack of PAIRS: {value, ab-tak-ka-min}. Har element apna min-so-far leke chalta.
//     push(x):  m = stack khaali ? x : min(x, top.second);  push {x, m}
//     pop():    stack pop
//     top():    return top.first
//     getMin(): return top.second   // pair ka min hamesha current-min
//   Pop pe pichla pair apne aap pichla-min de deta — alag min-stack nahi chahiye.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class MinStack
{
    stack<pair<int, int>> st; // {value, minSoFar}

public:
    void push(int x)
    {
        int m = st.empty() ? x : min(x, st.top().second);
        st.push({x, m});
    }

    void pop()
    {
        st.pop();
    }

    int top()
    {
        return st.top().first;
    }

    int getMin()
    {
        return st.top().second;
    }
};

int main()
{
    MinStack ms;
    ms.push(5);
    ms.push(3);
    ms.push(7);
    ms.push(2);
    cout << ms.getMin() << endl; // 2
    ms.pop();                    // 2 hata
    cout << ms.getMin() << endl; // 3
    cout << ms.top() << endl;    // 7
    ms.pop();                    // 7 hata
    ms.pop();                    // 3 hata
    cout << ms.getMin() << endl; // 5
    return 0;
}
