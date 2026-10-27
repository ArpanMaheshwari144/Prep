// ═══════════════════════════════════════════════════════════════════════
//  SERVER ERROR STREAK  (Wise OA, 27-Oct jaisa)
//  logs = vector<string>, har entry "<server> <code> <message>"  (jaise "s1 500 error", "s2 200 success").
//  message "success" = success, message "error" = error  (Wise test jaisa; code kuch bhi ho sakta, uspe mat jao).
//  Kisi server ke LAGAATAAR 3 error aayein -> alert++ aur us server ki ginti 0 se phir shuru.
//  Us server ka success aaye -> uski ginti 0. Doosre server ke log beech me aa sakte hain (wo ginti nahi todte).
//  Kul alerts lautao.
//
//  INPUT-FORM: 0 <= logs.size() <= 1e5. server naam koi bhi string (space nahi), message ek shabd.
//  TEST-CASES (input -> expected):
//   1) s1 err x3                                           -> 1
//   2) s1 err, s2 success, s1 err, s3 err, s1 err          -> 1   (beech me doosre server)
//   3) s1 err, s1 err, s1 200, s1 err                      -> 0   (success ne toda)
//   4) s1 err x6                                           -> 2
//   5) s1 err x5                                           -> 1
//   6) []                                                  -> 0
//   7) s1 200, s2 200                                      -> 0
//   8) s1 err, s2 err, s1 err, s2 err, s1 err, s2 err      -> 2   (dono server)
//   9) s1 500 error, s1 404 error, s1 503 error            -> 1   (code alag, message error)
//  10) s1 err, s1 err, s2 200, s1 err                      -> 1   (s2 ka 200 s1 ko nahi todta)
//  11) "web-01 500 error" x3                               -> 1   (lamba server naam)
//  12) s1 err, s1 err, s1 200, s1 err, s1 err, s1 err      -> 1
//  13) s1 err, s1 err, s1 success, s1 err, s1 err          -> 0   (success = ginti 0, ghatao nahi)
//  14) s1 401 error x3                                     -> 1   (koi bhi code, message error)
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int countAlerts(vector<string> &logs)
{
    unordered_map<string, int> mp;
    int count = 0;
    for (auto &s : logs)
    {
        stringstream ss(s);
        string server, code, msg;
        ss >> server >> code >> msg;

        if (msg == "error")
        {
            mp[server]++;
            if (mp[server] >= 3)
            {
                count++;
                mp[server] = 0;
            }
        }
        else if (msg == "success")
        {
            mp[server] = 0;
        }
    }
    return count;
}

// ---- test helper ----
void check(vector<string> logs, int exp, string name)
{
    int got = countAlerts(logs);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({"s1 500 error", "s1 500 error", "s1 500 error"}, 1, "t1");
    check({"s1 500 error", "s2 200 success", "s1 500 error", "s3 404 error", "s1 500 error"}, 1, "t2 beech me doosre");
    check({"s1 500 error", "s1 500 error", "s1 200 success", "s1 500 error"}, 0, "t3 success ne toda");
    check({"s1 500 error", "s1 500 error", "s1 500 error", "s1 500 error", "s1 500 error", "s1 500 error"}, 2, "t4 x6");
    check({"s1 500 error", "s1 500 error", "s1 500 error", "s1 500 error", "s1 500 error"}, 1, "t5 x5");
    check({}, 0, "t6 khaali");
    check({"s1 200 success", "s2 200 success"}, 0, "t7 sab success");
    check({"s1 500 error", "s2 500 error", "s1 500 error", "s2 500 error", "s1 500 error", "s2 500 error"}, 2, "t8 dono server");
    check({"s1 500 error", "s1 404 error", "s1 503 error"}, 1, "t9 alag code");
    check({"s1 500 error", "s1 500 error", "s2 200 success", "s1 500 error"}, 1, "t10 s2 ka 200");
    check({"web-01 500 error", "web-01 500 error", "web-01 500 error"}, 1, "t11 lamba naam");
    check({"s1 500 error", "s1 500 error", "s1 200 success", "s1 500 error", "s1 500 error", "s1 500 error"}, 1, "t12");
    check({"s1 500 error", "s1 500 error", "s1 200 success", "s1 500 error", "s1 500 error"}, 0, "t13 success = 0, ghatao nahi");
    check({"s1 401 error", "s1 401 error", "s1 401 error"}, 1, "t14 koi bhi code");
    return 0;
}
