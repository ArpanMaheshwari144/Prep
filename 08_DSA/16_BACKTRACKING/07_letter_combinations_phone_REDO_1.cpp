// ============================================================
// LETTER COMBINATIONS OF A PHONE NUMBER   (LeetCode 17, Medium)   REDO_1 (khaali page se)
// ============================================================
// digits = "2" se "9" tak ke ank ki string (0 se 4 ank).
// Purane phone ki tarah har ank pe kuch akshar hain:
//
//     2 = abc    3 = def    4 = ghi    5 = jkl
//     6 = mno    7 = pqrs   8 = tuv    9 = wxyz
//
// In ank se jitne bhi akshar-shabd ban sakte hain, SAARE lautao (koi bhi kram).
// digits khaali ho to khaali list lautao.
//
//   "23" -> ["ad","ae","af","bd","be","bf","cd","ce","cf"]
//   ""   -> []
//   "2"  -> ["a","b","c"]
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

void solve(int index, string &temp, unordered_map<char, string> &mp, vector<string> &ans, string &digits)
{
    if (index >= digits.size())
    {
        ans.push_back(temp);
        return;
    }

    string str = mp[digits[index]];
    for (int i = 0; i < str.size(); i++)
    {
        temp.push_back(str[i]);
        solve(index + 1, temp, mp, ans, digits);
        temp.pop_back();
    }
}

vector<string> letterCombinations(string digits)
{
    if (digits.size() == 0)
    {
        return {};
    }

    unordered_map<char, string> mp;
    mp['2'] = "abc";
    mp['3'] = "def";
    mp['4'] = "ghi";
    mp['5'] = "jkl";
    mp['6'] = "mno";
    mp['7'] = "pqrs";
    mp['8'] = "tuv";
    mp['9'] = "wxyz";

    vector<string> ans;
    string temp = "";
    solve(0, temp, mp, ans, digits);
    return ans;
}

// ------------------------------------------------------------
int main()
{
    struct T
    {
        string digits;
        vector<string> expected;
        string kyun;
    };

    // 234 ke 27 aur 99 ke 16 — generate karke bharte hain
    auto all = [](string d)
    {
        map<char, string> m = {{'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5', "jkl"}, {'6', "mno"}, {'7', "pqrs"}, {'8', "tuv"}, {'9', "wxyz"}};
        vector<string> r = {""};
        for (char c : d)
        {
            vector<string> nr;
            for (auto &s : r)
                for (char x : m[c])
                    nr.push_back(s + x);
            r = nr;
        }
        return r;
    };

    vector<T> tests = {
        {"23", {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"}, "classic"},
        {"", {}, "khaali digits"},
        {"2", {"a", "b", "c"}, "ek ank"},
        {"7", {"p", "q", "r", "s"}, "7 pe CHAAR akshar"},
        {"79", all("79"), "dono chaar-akshar wale -> 16"},
        {"234", all("234"), "teen ank -> 27"},
        {"99", all("99"), "same ank do baar -> 16"},
        {"2345", all("2345"), "chaar ank (max) -> 81"},
    };

    int pass = 0;
    for (int i = 0; i < (int)tests.size(); i++)
    {
        vector<string> got = letterCombinations(tests[i].digits);
        vector<string> exp = tests[i].expected;
        sort(got.begin(), got.end());
        sort(exp.begin(), exp.end());
        bool ok = (got == exp);
        pass += ok;
        cout << (ok ? "PASS  " : "FAIL  ")
             << "digits=\"" << tests[i].digits << "\""
             << "   expected " << exp.size()
             << ", mile " << got.size()
             << "   [" << tests[i].kyun << "]\n";
    }
    cout << "\n"
         << pass << " / " << tests.size() << "\n";
    return 0;
}
