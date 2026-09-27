// ============================================================
// IMPLEMENT QUEUE USING STACKS (LC 232) — stack / design (C++)
// ============================================================
// Sirf STACK (std::stack) use karke ek QUEUE (FIFO) banao.
// Allowed stack operations: push (upar daalo), pop (upar se nikaalo), top (upar dekho),
//                           size, empty. (queue / deque / vector as queue NAHI)
//
// METHODS:
//   void push(int x) -> x ko queue ke PEECHE daalo
//   int  pop()       -> queue ke AAGE wala nikaalo aur lautao
//   int  peek()      -> queue ke AAGE wala lautao (nikaalo nahi)
//   bool empty()     -> queue khaali hai?
//
// Pop / peek sirf tab bulaye jaayenge jab queue khaali NA ho.
//
// Walmart (R2) me poochha gaya: "fully working code".
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class MyQueue
{
public:
    stack<int> input;
    stack<int> output;
    MyQueue()
    {
    }

    void push(int x)
    {
        input.push(x);
    }

    int pop()
    {
        if (output.empty())
        {
            while (!input.empty())
            {
                output.push(input.top());
                input.pop();
            }
        }
        int x = output.top();
        output.pop();
        return x;
    }

    int peek()
    {
        if (output.empty())
        {
            while (!input.empty())
            {
                output.push(input.top());
                input.pop();
            }
        }
        return output.top();
    }

    bool empty()
    {
        if (input.empty() && output.empty())
        {
            return true;
        }
        return false;
    }
};

// ============================ TESTS ============================
int passed = 0, total = 0;
void check(const string &name, bool ok)
{
    total++;
    if (ok)
        passed++;
    cout << (ok ? "PASS  " : "FAIL  ") << name << "\n";
}

int main()
{
    // 1. LC example: push 1, push 2, peek -> 1, pop -> 1, empty -> false
    {
        MyQueue q;
        q.push(1);
        q.push(2);
        check("1 peek = 1", q.peek() == 1);
        check("1 pop = 1", q.pop() == 1);
        check("1 empty = false", q.empty() == false);
    }
    // 2. naya queue khaali
    {
        MyQueue q;
        check("2 new empty = true", q.empty() == true);
    }
    // 3. FIFO kram: 1,2,3 daalo -> 1,2,3 hi niklein
    {
        MyQueue q;
        q.push(1);
        q.push(2);
        q.push(3);
        bool ok = q.pop() == 1 && q.pop() == 2 && q.pop() == 3 && q.empty();
        check("3 order 1,2,3", ok);
    }
    // 4. beech me push + pop mila ke
    {
        MyQueue q;
        q.push(1);
        q.push(2);
        bool a = q.pop() == 1;
        q.push(3);
        q.push(4);
        bool b = q.peek() == 2 && q.pop() == 2 && q.pop() == 3;
        q.push(5);
        bool c = q.pop() == 4 && q.pop() == 5 && q.empty();
        check("4 interleaved", a && b && c);
    }
    // 5. peek se kuch nikalna NAHI chahiye
    {
        MyQueue q;
        q.push(7);
        q.push(8);
        bool ok = q.peek() == 7 && q.peek() == 7 && q.pop() == 7 && q.peek() == 8;
        check("5 peek does not remove", ok);
    }
    // 6. khaali karke phir se use
    {
        MyQueue q;
        q.push(1);
        q.pop();
        bool e = q.empty();
        q.push(9);
        check("6 reuse after empty", e && q.peek() == 9 && q.pop() == 9 && q.empty());
    }
    // 7. bada: 1..1000 daalo, sab kram se niklein
    {
        MyQueue q;
        for (int i = 1; i <= 1000; i++)
            q.push(i);
        bool ok = true;
        for (int i = 1; i <= 1000; i++)
            if (q.pop() != i)
            {
                ok = false;
                break;
            }
        check("7 1..1000 FIFO", ok && q.empty());
    }

    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
