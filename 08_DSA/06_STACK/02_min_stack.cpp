// ============================================================
// MIN STACK — Stack (pattern #2)
// ============================================================
// Ek stack DESIGN karo jo ye 4 operations kare — SAB O(1) me:
//   push(val) · pop() · top() · getMin()
//   getMin() = stack ke ANDAR ka current MINIMUM element lautaye (O(1), loop nahi).
//   yahi trick hai -> min ko har waqt kaise "ready" rakhein bina scan kiye. socho.
//
// ---- ARPAN KI APPROACH ----
//  stack me pair<int,int> rakho: {value, uss point tak ka min}.
//  push pe naya min = min(val, purana top ka min) nikaal ke saath store karo.
//  getMin() -> bas top ka second (min) lauta do, koi loop nahi -> O(1).
//
// Tests (// expected):
//   push(-2) push(0) push(-3)
//   getMin() -> -3
//   pop()
//   top()    -> 0
//   getMin() -> -2
//
//   (dusra scenario)
//   push(5) push(3) push(7)
//   getMin() -> 3
//   pop()            (7 gaya)
//   getMin() -> 3
//   pop()            (3 gaya)
//   getMin() -> 5
// ============================================================

#include <iostream>
#include <stack>
using namespace std;

class MinStack
{
    // members tere -- min ko O(1) me paane ke liye kya-kya store karoge? socho.

public:
    stack<pair<int, int>> st;
    MinStack()
    {
    }

    void push(int val)
    {
        int x = st.empty() ? val : min(val, st.top().second);
        st.push({val, x});
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
    MinStack st;
    st.push(-2);
    st.push(0);
    st.push(-3);
    cout << st.getMin() << " (expected -3)\n";
    st.pop();
    cout << st.top() << " (expected 0)\n";
    cout << st.getMin() << " (expected -2)\n";

    cout << "---\n";

    MinStack st2;
    st2.push(5);
    st2.push(3);
    st2.push(7);
    cout << st2.getMin() << " (expected 3)\n";
    st2.pop();
    cout << st2.getMin() << " (expected 3)\n";
    st2.pop();
    cout << st2.getMin() << " (expected 5)\n";

    return 0;
}
