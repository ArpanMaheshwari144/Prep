// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// CAR FLEET   (LC-853 — REDO_1)
// ============================================================
// Ek seedhi ek-lane sadak, manzil = mile `target` pe. n gaadiyan: gaadi i mile position[i] pe khadi,
// speed[i] mile/ghanta se manzil ki taraf chalti. Overtake NAHI ho sakta: tez gaadi aage wali dheemi
// gaadi ko pakad le to usi ke peeche usi speed se chalti, dono ab ek FLEET.
// Manzil pe hi pakda (dono ek saath pahunche) to bhi ek fleet. Akeli gaadi bhi ek fleet.
// Kitne fleet manzil pe pahunchenge, lautao.
//
//   target=12, position=[10,8,0,5,3], speed=[2,4,1,1,3] -> 3
//     kyun: 10 aur 8 dono 1 ghante me pahunchte -> ek fleet · 0 akela (12 ghante) · 5 aur 3 dono 7 ghante -> ek fleet
//   target=10, position=[6,8], speed=[3,2] -> 2
//     kyun: 8 wali 1 ghante me, 6 wali 4/3 ghante me (der se) -> peeche wali pakad hi nahi paayi
//
// Tests:
//   (12,[10,8,0,5,3],[2,4,1,1,3]) -> 3 · (10,[3],[3]) -> 1 · (100,[0,2,4],[4,2,1]) -> 1
//   (10,[6,8],[3,2]) -> 2 · (10,[0,4,2],[2,1,3]) -> 1 · (20,[6,2,17],[3,9,2]) -> 2
//   (10,[0,1,2,3],[1,1,1,1]) -> 4 · (10,[5,0],[1,2]) -> 1 · (13,[10,2,5,7,4,6,11],[7,5,10,5,9,4,1]) -> 2
//   (10,[8,3,7,4,6,5],[4,4,4,4,4,4]) -> 6 · (10,[7,4],[2,5]) -> 1
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int carFleet(int target, vector<int> &position, vector<int> &speed)
{
    return 0;
}

// ---------- checker (ise mat chhed) ----------
int passed = 0, total = 0;
void check(int target, vector<int> position, vector<int> speed, int expected)
{
    total++;
    int got = carFleet(target, position, speed);
    bool ok = (got == expected);
    if (ok)
        passed++;
    cout << "test " << total << "  got=" << got << "  expected=" << expected
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check(12, {10, 8, 0, 5, 3}, {2, 4, 1, 1, 3}, 3);
    check(10, {3}, {3}, 1);
    check(100, {0, 2, 4}, {4, 2, 1}, 1);
    check(10, {6, 8}, {3, 2}, 2);
    check(10, {0, 4, 2}, {2, 1, 3}, 1);
    check(20, {6, 2, 17}, {3, 9, 2}, 2);
    check(10, {0, 1, 2, 3}, {1, 1, 1, 1}, 4);
    check(10, {5, 0}, {1, 2}, 1);
    check(13, {10, 2, 5, 7, 4, 6, 11}, {7, 5, 10, 5, 9, 4, 1}, 2);
    check(10, {8, 3, 7, 4, 6, 5}, {4, 4, 4, 4, 4, 4}, 6);
    check(10, {7, 4}, {2, 5}, 1);
    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
