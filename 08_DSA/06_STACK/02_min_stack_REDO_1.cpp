// ============================================================
// MIN STACK — Stack — REDO_1 (khaali page)
// ============================================================
// Ek stack DESIGN karo jo ye 4 operations kare — SAB O(1) me:
//   push(val) · pop() · top() · getMin()
//   getMin() = stack ke ANDAR ka current MINIMUM (O(1), loop nahi).
//
// Tests (neeche main me, khud PASS/FAIL batayenge):
//   push(-2) push(0) push(-3) -> getMin -3 -> pop -> top 0 -> getMin -2
//   push(5) push(3) push(7)   -> getMin 3 -> pop -> getMin 3 -> pop -> getMin 5
//   push(2) push(2) push(1)   -> getMin 1 -> pop -> getMin 2 -> pop -> getMin 2   (DUPLICATE min)
//   push(INT_MIN) push(INT_MAX) -> getMin INT_MIN -> top INT_MAX
// ============================================================

#include <iostream>
#include <climits>
#include <stack>
using namespace std;

class MinStack
{
public:
    stack<pair<int, int>> st;
    MinStack()
    {
    }

    void push(int val)
    {
        int minVal = st.empty() ? val : min(st.top().second, val);
        st.push({val, minVal});
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

// ---- test harness (mat chhed) ----
int failCount = 0;
void expect(const char *what, int got, int exp)
{
    bool ok = (got == exp);
    if (!ok)
        failCount++;
    cout << (ok ? "PASS" : "FAIL") << "  " << what << " got=" << got << " exp=" << exp << "\n";
}

int main()
{
    MinStack a;
    a.push(-2);
    a.push(0);
    a.push(-3);
    expect("T1 getMin", a.getMin(), -3);
    a.pop();
    expect("T1 top", a.top(), 0);
    expect("T1 getMin", a.getMin(), -2);

    MinStack b;
    b.push(5);
    b.push(3);
    b.push(7);
    expect("T2 getMin", b.getMin(), 3);
    b.pop();
    expect("T2 getMin", b.getMin(), 3);
    b.pop();
    expect("T2 getMin", b.getMin(), 5);

    MinStack c;
    c.push(2);
    c.push(2);
    c.push(1);
    expect("T3 getMin", c.getMin(), 1);
    c.pop();
    expect("T3 getMin", c.getMin(), 2);
    c.pop();
    expect("T3 getMin (duplicate)", c.getMin(), 2);

    MinStack d;
    d.push(INT_MIN);
    d.push(INT_MAX);
    expect("T4 getMin", d.getMin(), INT_MIN);
    expect("T4 top", d.top(), INT_MAX);

    cout << (failCount == 0 ? "ALL PASS" : "kuch FAIL") << "\n";
    return 0;
}
