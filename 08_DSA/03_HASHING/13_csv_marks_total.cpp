// ============================================================
// CSV SE KUL MARKS (OA tools: stringstream + getline ',' + map)  — 5-Nov grill se
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
    for (auto &it : lines)
    {
        stringstream ss(it);
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
