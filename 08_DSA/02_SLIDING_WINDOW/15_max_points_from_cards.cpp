// ============================================================
// MAXIMUM POINTS YOU CAN OBTAIN FROM CARDS      (LC-1423, SW — fresh)
// ============================================================
// cardPoints[] ek row me cards. tumhe EXACTLY k cards lene hain,
// PAR sirf row ke DONO ENDS se (front se ya back se, koi bhi combo).
// li gayi cards ka SUM MAXIMIZE karo -> wahi return.
//
//   [1,2,3,4,5,6,1], k=3   -> 12   (back se 3: 1+6+5)
//   sirf ends se le sakte -- beech ka card seedha nahi utha sakte.
//
// ---- TEST CASES (cardPoints, k -> expected) ----
//   [1,2,3,4,5,6,1],          k=3  -> 12
//   [2,2,2],                  k=2  -> 4
//   [9,7,7,9,7,7,9],          k=7  -> 55    (saare)
//   [1,1000,1],               k=1  -> 1     (beech ka 1000 nahi utha sakte)
//   [1,79,80,1,1,1,200,1],    k=3  -> 202
// ============================================================
//
//
//  ---- 2 TRAP jo discussion me khud pakde + fix kiye ----
//   1. "sirf back se k lo" GALAT -> front bhi le sakte, aur front+back MIX bhi. ([100,1,1,1,1],k=2 -> front best)
//   2. "front se k + back se k = 2k cards" GALAT -> k cards TOTAL (dono end MILAKE), na har end se k.
//      (isse untaken = L-2k nikal raha tha; sahi = L-k.)
//
//  COLD nahi hota (combo-twist) -> library me daala -> revise 1/3/7 -> next time instant.
//
//  ================= DERIVE-JOURNEY (24-Aug, discussion me kaise-kaise samjha — STEP BY STEP) =================
//   Ye "answer" seedha nahi aaya -- STEPS me khula. dobara padho to YE soch-ki-raah yaad karo, sirf code nahi:
//
//   STEP 0 (pehla khyaal): "last se k element ka sum le lo" (sirf BACK se).
//          -> GALAT. counter: [100,1,1,1,1], k=2 -> last-2 = 2, par FRONT-2 = 101. front bhi le sakte + mix bhi.
//
//   STEP 1: "achha, front se bhi + back se bhi." to socha "front se k AUR back se k".
//          -> GALAT. wo 2k cards ho gaye. problem = k cards TOTAL (dono end MILAKE k). ye slip 2 baar hua.
//          (isi se main baar-baar untaken = L-2k nikaal raha tha -> dry-run "4 bacha" se KHUD pakda: L-2k=1 != 4.)
//
//   STEP 2 (ASLI turning point): agar window ko taken-cards pe slide karu -> beech ke elements aa jaate,
//          jo mai le hi nahi sakta (sirf ends se). matlab taken cards CONTIGUOUS nahi -> seedha SW FAIL.
//
//   STEP 3 (FLIP / aha): "jo mai NAHI leta, wo kaise dikhte?" -> beech me, EK SAATH (contiguous!).
//          un "nahi-li" cards ka window size = L - k. (total L, k utha liye -> L-k bacha.)
//
//   STEP 4 (formula): taken MAX karna = untaken (L-k window) MIN karna. Total fix hai.
//          => answer = Total - min(window of size L-k).   -> ab ye plain MAX-SUM-K (min-version) ban gaya!
//
//   STEP 5 (edge): k == L -> window size 0 -> answer = Total (saare cards). crash se bacho.
//
//   SEEKH: "hard" problem FLIP/complement ke baad TRIVIAL ho gaya ("ye to problem tha hi nahi").
//          reframe = superpower: "jo chahiye" ki jagah "jo NAHI chahiye" gino.
//  ============================================================================================================
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int minSumK(vector<int> &nums, int k)
{
    int sum = 0;
    int minSum = INT_MAX;
    int i = 0, j = 0;
    while (j < nums.size())
    {
        sum += nums[j];
        if (j - i + 1 >= k)
        {
            minSum = min(minSum, sum);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return minSum;
}

int maxScore(vector<int> &nums, int k)
{
    int Total = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        Total += nums[i];
    }

    if (nums.size() - k == 0)
        return Total;

    int minSum = minSumK(nums, nums.size() - k);

    return Total - minSum;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {1, 2, 3, 4, 5, 6, 1};
    check(maxScore(a1, 3), 12, "case1");

    vector<int> a2 = {2, 2, 2};
    check(maxScore(a2, 2), 4, "case2");

    vector<int> a3 = {9, 7, 7, 9, 7, 7, 9};
    check(maxScore(a3, 7), 55, "case3");

    vector<int> a4 = {1, 1000, 1};
    check(maxScore(a4, 1), 1, "case4");

    vector<int> a5 = {1, 79, 80, 1, 1, 1, 200, 1};
    check(maxScore(a5, 3), 202, "case5");
    return 0;
}
