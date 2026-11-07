// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// SHIFT KITNE MINUTE KI (OA tools, HH:MM) — REDO_1 (khaali page, C++, bina dekhe)
// ============================================================
// Guard ki shift ka start aur end "HH:MM" (24 ghante wali ghadi) me diya hai.
// Shift raat 12 ke paar bhi ja sakti hai. Shift kitne minute ki hai, lautao.
// start == end matlab shift 0 minute ki.
//
// Tests (// expected):
//   "09:00", "17:30"  -> 510
//   "22:30", "01:15"  -> 165
//   "00:00", "00:00"  -> 0
//   "23:59", "00:01"  -> 2
//   "06:45", "20:15"  -> 810
//   "20:00", "08:30"  -> 750
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int shiftMinutes(string start, string end)
{
    // yahan likh
    return 0;
}

// ---------- printer (boilerplate, chhoo mat) ----------
int main()
{
    cout << shiftMinutes("09:00", "17:30") << "\n"; // 510
    cout << shiftMinutes("22:30", "01:15") << "\n"; // 165
    cout << shiftMinutes("00:00", "00:00") << "\n"; // 0
    cout << shiftMinutes("23:59", "00:01") << "\n"; // 2
    cout << shiftMinutes("06:45", "20:15") << "\n"; // 810
    cout << shiftMinutes("20:00", "08:30") << "\n"; // 750
    return 0;
}
