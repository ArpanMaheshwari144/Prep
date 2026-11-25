// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// CORPORATE FLIGHT BOOKINGS   (LC-1109 — REDO_2, khaali page)
// ============================================================
// n flights hain, number 1 se n tak.
// bookings di hai. Har booking = [first, last, seats]:
//   flight `first` se flight `last` TAK (dono shaamil) ki HAR flight me `seats` seat book hui.
// Ek array lautao jiska i-th element = flight (i+1) pe kul kitni seat book hui.
//
//   n = 5, bookings = [[1,2,10],[2,3,20],[2,5,25]]  ->  [10, 55, 45, 25, 25]
//
// bookings.length aur n dono 20,000 tak ja sakte hain.
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   n=5  [[1,2,10],[2,3,20],[2,5,25]]   -> [10,55,45,25,25]
//   n=2  [[1,2,10],[2,2,15]]            -> [10,25]
//   n=1  [[1,1,100]]                    -> [100]
//   n=3  [[1,3,5]]                      -> [5,5,5]
//   n=5  [[3,3,7]]                      -> [0,0,7,0,0]
//   n=5  [[1,5,1],[1,5,2],[1,5,3]]      -> [6,6,6,6,6]
//   n=5  [[1,2,10],[4,5,20]]            -> [10,10,0,20,20]
//   n=3  []                             -> [0,0,0]
//   n=4  [[4,4,9],[1,1,2]]              -> [2,0,0,9]
//   n=6  [[2,5,3],[5,6,4],[1,6,1]]      -> [1,4,4,4,8,5]
// ============================================================
#include <bits/stdc++.h>
using namespace std;

vector<int> corpFlightBookings(vector<vector<int>> &bookings, int n)
{
    vector<int> diff(n + 2);
    for (auto &it : bookings)
    {
        int start = it[0];
        int end = it[1];
        int cost = it[2];
        diff[start] += cost;
        diff[end + 1] -= cost;
    }

    vector<int> ans;
    int cummSum = 0;
    // i = 1 se kyun: flight number 1 se shuru hote hain, to booking ka start (diff[start]) bhi 1 se hi likha gaya,
    // diff[0] me kabhi kuch nahi jaata. Isliye jodna bhi 1 se.
    // i <= n kyun: n flights hain, flight n tak ka jawab chahiye (diff[n+1] me sirf aakhri "-" hai, wo jawab nahi).
    for (int i = 1; i <= n; i++)
    {
        cummSum += diff[i];
        ans.push_back(cummSum);
    }
    return ans;
}

// ---- test helper (ise mat chhed) ----
int passed = 0, total = 0;
string show(const vector<int> &v)
{
    string s = "[";
    for (int i = 0; i < (int)v.size(); i++)
        s += (i ? "," : "") + to_string(v[i]);
    return s + "]";
}
void check(vector<vector<int>> bookings, int n, vector<int> exp)
{
    total++;
    vector<int> got = corpFlightBookings(bookings, n);
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << "n=" << n << " -> got " << show(got) << " | exp " << show(exp)
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check({{1, 2, 10}, {2, 3, 20}, {2, 5, 25}}, 5, {10, 55, 45, 25, 25});
    check({{1, 2, 10}, {2, 2, 15}}, 2, {10, 25});
    check({{1, 1, 100}}, 1, {100});
    check({{1, 3, 5}}, 3, {5, 5, 5});
    check({{3, 3, 7}}, 5, {0, 0, 7, 0, 0});
    check({{1, 5, 1}, {1, 5, 2}, {1, 5, 3}}, 5, {6, 6, 6, 6, 6});
    check({{1, 2, 10}, {4, 5, 20}}, 5, {10, 10, 0, 20, 20});
    check({}, 3, {0, 0, 0});
    check({{4, 4, 9}, {1, 1, 2}}, 4, {2, 0, 0, 9});
    check({{2, 5, 3}, {5, 6, 4}, {1, 6, 1}}, 6, {1, 4, 4, 4, 8, 5});
    cout << "\n"
         << passed << "/" << total << " PASS\n";
    return 0;
}
