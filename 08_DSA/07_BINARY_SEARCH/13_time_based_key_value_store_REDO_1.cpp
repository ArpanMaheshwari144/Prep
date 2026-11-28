// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// TIME BASED KEY-VALUE STORE   (LC-981 — REDO_1)
// ============================================================
// Ek class TimeMap banao, do kaam:
//   set(key, value, timestamp) : us key ki value us waqt pe rakh do.
//   get(key, timestamp)        : us key ki wo value do jiska waqt <= timestamp ho,
//                                aur aise kai ho to SABSE BADE waqt wali.
//                                Koi na mile (key hi nahi, ya sab waqt bade) to "" lautao.
// set me timestamp hamesha badhta hi jaata hai (har agla set pichhle se bade waqt pe).
//
//   set("foo","bar",1)
//   get("foo",1)  -> "bar"
//   get("foo",3)  -> "bar"     kyun: 3 se pehle sirf waqt 1 wali value thi
//   set("foo","bar2",4)
//   get("foo",4)  -> "bar2"
//   get("foo",5)  -> "bar2"    kyun: 5 se pehle waqt 1 aur 4, sabse bada 4
//
// Tests (har test naya TimeMap):
//   1. upar wala LC example
//   2. khaali map me get -> ""
//   3. set waqt 5 pe, get waqt 4 pe -> ""
//   4. do alag key, ek ki value doosri me na aaye; anjaan key -> ""
//   5. waqt 10, 20, 30: get 20 -> v2 · 25 -> v2 · 9 -> "" · 100 -> v3
//   6. 1000 set (waqt 2, 4, ..., 2000): get 1001 -> v500 · 2000 -> v1000 · 2 -> v1 · 1 -> ""
//   7. bada waqt (10^7): get 10^7 -> last · 9999999 -> first
// ============================================================
#include <bits/stdc++.h>
using namespace std;

class TimeMap
{
public:
    TimeMap()
    {
    }

    void set(string key, string value, int timestamp)
    {
    }

    string get(string key, int timestamp)
    {
        return "";
    }
};

// ---------- checker (ise mat chhed) ----------
int passed = 0, total = 0;
void check(const string &got, const string &expected)
{
    total++;
    bool ok = (got == expected);
    if (ok)
        passed++;
    cout << "check " << total << "  got=\"" << got << "\"  expected=\"" << expected << "\""
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    {
        cout << "-- test 1 (LC example)\n";
        TimeMap t;
        t.set("foo", "bar", 1);
        check(t.get("foo", 1), "bar");
        check(t.get("foo", 3), "bar");
        t.set("foo", "bar2", 4);
        check(t.get("foo", 4), "bar2");
        check(t.get("foo", 5), "bar2");
    }
    {
        cout << "-- test 2 (khaali map)\n";
        TimeMap t;
        check(t.get("a", 1), "");
    }
    {
        cout << "-- test 3 (waqt pehle ka)\n";
        TimeMap t;
        t.set("a", "x", 5);
        check(t.get("a", 4), "");
    }
    {
        cout << "-- test 4 (do key)\n";
        TimeMap t;
        t.set("a", "x", 1);
        t.set("b", "y", 2);
        check(t.get("a", 2), "x");
        check(t.get("b", 1), "");
        check(t.get("c", 5), "");
    }
    {
        cout << "-- test 5 (beech ka waqt)\n";
        TimeMap t;
        t.set("k", "v1", 10);
        t.set("k", "v2", 20);
        t.set("k", "v3", 30);
        check(t.get("k", 20), "v2");
        check(t.get("k", 25), "v2");
        check(t.get("k", 9), "");
        check(t.get("k", 100), "v3");
    }
    {
        cout << "-- test 6 (1000 set)\n";
        TimeMap t;
        for (int i = 1; i <= 1000; i++)
            t.set("k", "v" + to_string(i), 2 * i);
        check(t.get("k", 1001), "v500");
        check(t.get("k", 2000), "v1000");
        check(t.get("k", 2), "v1");
        check(t.get("k", 1), "");
    }
    {
        cout << "-- test 7 (bada waqt)\n";
        TimeMap t;
        t.set("big", "first", 1);
        t.set("big", "last", 10000000);
        check(t.get("big", 10000000), "last");
        check(t.get("big", 9999999), "first");
    }
    cout << "\n"
         << passed << "/" << total << " checks passed\n";
    return 0;
}
