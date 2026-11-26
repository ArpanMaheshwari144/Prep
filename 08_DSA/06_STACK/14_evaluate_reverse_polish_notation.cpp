// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// EVALUATE REVERSE POLISH NOTATION   (LC-150 — NAYA)
// ============================================================
// String ka array tokens. Har token ya to ek number hai (minus wala bhi, jaise "-11"),
// ya ek operator: "+", "-", "*", "/".
// Ye ek hisaab hai Reverse Polish Notation me: operator apne DONO number ke BAAD aata hai.
// Poore hisaab ka jawab (int) lautao.
//   - "/" = integer division, ZERO ki taraf kaato (7/-2 = -3, -7/2 = -3).
//   - Hisaab hamesha sahi hoga, zero se divide kabhi nahi.
//   - Beech ke aur aakhri jawab int me aa jaayenge.
//
//   ["2","1","+","3","*"]  ->  9      kyunki (2 + 1) * 3
//   ["4","13","5","/","+"] ->  6      kyunki 4 + (13 / 5) = 4 + 2
//
// Tests:
//   ["2","1","+","3","*"] -> 9 · ["4","13","5","/","+"] -> 6
//   ["10","6","9","3","+","-11","*","/","*","17","+","5","+"] -> 22
//   ["5"] -> 5 · ["-3"] -> -3 · ["3","4","-"] -> -1 · ["2","8","/"] -> 0
//   ["7","-2","/"] -> -3 · ["-7","2","/"] -> -3 · ["-100","-5","/"] -> 20
//   ["1","2","+","3","4","+","*"] -> 21
//   ["15","7","1","1","+","-","/","3","*","2","1","1","+","+","-"] -> 5
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int solve(int a, int b, string token)
{
    if (token == "+")
    {
        return a + b;
    }
    else if (token == "-")
    {
        return a - b;
    }
    else if (token == "/")
    {
        return a / b;
    }
    else if (token == "*")
    {
        return (long)a * (long)b;
    }
    return -1;
}

int evalRPN(vector<string> &tokens)
{
    stack<int> st;
    for (auto &token : tokens)
    {
        if (token == "+" || token == "-" || token == "/" || token == "*")
        {
            int b = st.top();
            st.pop();

            int a = st.top();
            st.pop();

            int ans = solve(a, b, token);
            st.push(ans);
        }
        else
        {
            st.push(stoi(token));
        }
    }
    return st.top();
}

// ---------- checker (ise mat chhed) ----------
int passed = 0, total = 0;
void check(vector<string> tokens, int expected)
{
    total++;
    int got = evalRPN(tokens);
    bool ok = (got == expected);
    if (ok)
        passed++;
    cout << "test " << total << "  got=" << got << "  expected=" << expected
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check({"2", "1", "+", "3", "*"}, 9);
    check({"4", "13", "5", "/", "+"}, 6);
    check({"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"}, 22);
    check({"5"}, 5);
    check({"-3"}, -3);
    check({"3", "4", "-"}, -1);
    check({"2", "8", "/"}, 0);
    check({"7", "-2", "/"}, -3);
    check({"-7", "2", "/"}, -3);
    check({"-100", "-5", "/"}, 20);
    check({"1", "2", "+", "3", "4", "+", "*"}, 21);
    check({"15", "7", "1", "1", "+", "-", "/", "3", "*", "2", "1", "1", "+", "+", "-"}, 5);
    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
