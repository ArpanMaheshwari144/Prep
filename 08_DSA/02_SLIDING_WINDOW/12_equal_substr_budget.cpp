// ============================================================
// GET EQUAL SUBSTRINGS WITHIN BUDGET — (LeetCode 1208, Medium)   (fresh)
// ============================================================
// do string s aur t (same length) aur ek int maxCost diya.
// s[i] ko t[i] me badalne ka COST = |s[i] - t[i]| (ASCII farak).
// aisa SABSE LAMBA substring (window) ki LENGTH lautao jise t ke us hisse me
// badalne ka TOTAL cost <= maxCost ho.
//
//   s="abcd", t="bcdf", maxCost=3  -> 3   (|a-b|+|b-c|+|c-d| = 1+1+1 = 3, length 3)
//   s="abcd", t="cdef", maxCost=3  -> 1   (har char cost 2; 2 char = 4 > 3, to sirf 1)
//   s="abcd", t="acde", maxCost=0  -> 1   (sirf a->a cost 0; baaki > 0)
//   s="krrgw", t="zjxss", maxCost=19 -> 2
//   s="abcd", t="abcd", maxCost=0  -> 4   (sab same, cost 0)
//
// ============================================================
/*
---- ARPAN KI APPROACH (khud ke words) ----
Approach dekh -- ye same VARIABLE WINDOW jaisa hi hai, bas problem thoda alag laga.
same i, j, cost, ans.

  while (j < size):
      cost += abs(s[j] - t[j])                  // window me char add
      ab dekh -> while (cost > maxCost) to bhai ab INVALID -> shrink kar, i++
          lekin hatao KYA? -> cost -= abs(s[i] - t[i])   (jo dala tha wahi nikaal do, bas)
      baaki answer update to hota hi hai -> ans = max(ans, j - i + 1)
      j++

  return ans
*/


#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int equalSubstring(string s, string t, int maxCost)
{
    int i = 0, j = 0, cost = 0, ans = INT_MIN;
    while (j < t.size()) //   s="abcd", t="cdef", maxCost=3  -> 1   (har char cost 2; 2 char = 4 > 3, to sirf 1)
    {
        cost += abs(s[j] - t[j]);
        while (cost > maxCost)
        {
            cost -= abs(s[i] - t[i]);
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans;
}

int main()
{
    cout << equalSubstring("abcd", "bcdf", 3) << " (expected 3)\n";
    cout << equalSubstring("abcd", "cdef", 3) << " (expected 1)\n";
    cout << equalSubstring("abcd", "acde", 0) << " (expected 1)\n";
    cout << equalSubstring("krrgw", "zjxss", 19) << " (expected 2)\n";
    cout << equalSubstring("abcd", "abcd", 0) << " (expected 4)\n";
    return 0;
}
