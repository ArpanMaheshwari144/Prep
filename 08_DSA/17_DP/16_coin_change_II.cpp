// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// COIN CHANGE II   (LC-518 — NAYA)
// ============================================================
// coins[] me sikkon ki keemat (har sikka jitni baar chaho), aur ek amount.
// Kitne alag TAREEKON se theek amount bana sakte ho, wo ginti lautao. Na ban sake to 0.
// Tareeke = sikkon ka MEL; kram se farak nahi padta (1+2 aur 2+1 = EK hi tareeka).
// Jawab int me aa jaayega.
//
//   amount=5, coins=[1,2,5] -> 4
//     kyun: 5 · 2+2+1 · 2+1+1+1 · 1+1+1+1+1
//   amount=3, coins=[2] -> 0      (sirf 2 se 3 nahi banta)
//   amount=0, coins=[1,2] -> 1    (kuch na lo = ek tareeka)
//
// Tests:
//   (5,[1,2,5]) -> 4 · (3,[2]) -> 0 · (10,[10]) -> 1 · (0,[1,2]) -> 1 · (3,[1,2]) -> 2
//   (4,[1,2,3]) -> 4 · (7,[2,4]) -> 0 · (11,[1,2,5]) -> 11 · (100,[99,1]) -> 2
//   (500,[3,5,7,8,9,10,11]) -> 35502874
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int change(int amount, vector<int> &coins)
{
    return 0;
}

// ---------- checker (ise mat chhed) ----------
int passed = 0, total = 0;
void check(int amount, vector<int> coins, int expected)
{
    total++;
    int got = change(amount, coins);
    bool ok = (got == expected);
    if (ok)
        passed++;
    cout << "test " << total << "  got=" << got << "  expected=" << expected
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check(5, {1, 2, 5}, 4);
    check(3, {2}, 0);
    check(10, {10}, 1);
    check(0, {1, 2}, 1);
    check(3, {1, 2}, 2);
    check(4, {1, 2, 3}, 4);
    check(7, {2, 4}, 0);
    check(11, {1, 2, 5}, 11);
    check(100, {99, 1}, 2);
    check(500, {3, 5, 7, 8, 9, 10, 11}, 35502874);
    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
