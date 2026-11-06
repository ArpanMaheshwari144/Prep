// ============================================================
// CSV SE KUL MARKS (OA tools)  — REDO_1, 6-Nov (spaced din 1)
// ============================================================
// PEHLE YE (30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// Har line "naam,subject,marks". Har student ke kul marks nikaalo.
// Output: naam ke alphabet kram me, "naam kul".
//
// Tests (// expected):
//   ["ravi,maths,40","priya,maths,55","ravi,sci,35","priya,sci,30"]  -> priya 85, ravi 75
//   ["amit,eng,90"]                                                   -> amit 90
//   ["a,x,0","a,y,0"]                                                 -> a 0
//   ["zoya,maths,100","ali,maths,7","zoya,sci,1"]                     -> ali 7, zoya 101
// ============================================================

#include <bits/stdc++.h>
using namespace std;

map<string, int> totalMarks(vector<string> &lines)
{
    map<string, int> mp;
    for (auto &s : lines)
    {
        stringstream ss(s);
        string name, sub, marks;

        getline(ss, name, ',');
        getline(ss, sub, ',');
        getline(ss, marks, ',');

        int t = stoi(marks);

        mp[name] += t;
    }
    return mp;
}

// ---------- printer (boilerplate, chhoo mat) ----------
void check(vector<string> lines)
{
    map<string, int> r = totalMarks(lines);
    for (auto &it : r)
        cout << it.first << " " << it.second << ", ";
    cout << "\n";
}

int main()
{
    check({"ravi,maths,40", "priya,maths,55", "ravi,sci,35", "priya,sci,30"}); // priya 85, ravi 75
    check({"amit,eng,90"});                                                    // amit 90
    check({"a,x,0", "a,y,0"});                                                 // a 0
    check({"zoya,maths,100", "ali,maths,7", "zoya,sci,1"});                    // ali 7, zoya 101
    return 0;
}
