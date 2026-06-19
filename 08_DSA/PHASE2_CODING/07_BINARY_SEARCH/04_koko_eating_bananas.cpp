// ============================================================
// KOKO EATING BANANAS   —   Binary Search ON THE ANSWER
// ============================================================
// piles[] = kele ke dher. h ghante. Koko ek speed k (kele/ghanta) chunti.
// Har ghanta EK dher se khaati (kam bache -> finish + baaki ghanta wait).
// Nikalo MINIMUM speed k jisse saare kele h ghante mein khatam ho jaayein.
//   piles=[3,6,7,11], h=8   -> 4
//   piles=[30,11,23,4,20], h=5 -> 30
//   piles=[30,11,23,4,20], h=6 -> 23
//
// SIGNAL: "min/max value jo ek CONDITION satisfy kare + monotonic N..N Y..Y"
//         -> Binary Search ON THE ANSWER (speed pe, array pe nahi).
//   Soch (structure TERA — no pseudo-code):
//     - search-space: speed lo=1 .. hi=max(pile).
//     - kisi speed k pe FEASIBLE? -> total ghante = sum( ceil(pile/k) ) <= h ?
//       (ceil bina <cmath>: (pile + k - 1) / k  ya  pile/k + (pile%k!=0))
//     - PASS -> "aur dheere ho sakti?" left; FAIL -> "tez karo" right.
//     - boundary (pehli PASS) = answer.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isFeasible(vector<int> &piles, int h, int mid)
{
    int sum = 0;
    for (int i = 0; i < piles.size(); i++)
    {
        // ek dher ke ghante = ceil(pile / speed). Do tareeke, DONO same kaam:
        //
        // (A) DOUBLE cast (yahan use kiya):  ceil((double)piles[i] / mid)
        //     ZAROORI: ek operand double banao. SIRF ceil(piles[i] / mid) GALAT —
        //     int/int pehle floor kar deta (7/4 -> 1), phir ceil(1)=1 (decimal pehle hi udd gaya).
        //     double se asli division: 7.0/4 = 1.75 -> ceil = 2. ceil double pe hi chalta hai.
        //
        // (B) PURE-INT formula (double nahi chahiye to):  (piles[i] + mid - 1) / mid
        //     int hi mein ceiling. +(mid-1) = remainder ho to agle multiple pe dhakka,
        //     exact divide ho to na badhe. No floating-point, no precision issue (CP-favourite).
        sum += ceil((double)piles[i] / mid);
        // sum += ((piles[i] + mid - 1) / mid);   // (B) ka alternative
    }
    return sum <= h;
}

int minEatingSpeed(vector<int> &piles, int h)
{
    int low = 1;
    int high = *max_element(begin(piles), end(piles));
    int ans = -1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2; // speed
        if (isFeasible(piles, h, mid))
        {
            ans = mid;
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return ans;
}

int main()
{
    vector<int> a = {3, 6, 7, 11};
    cout << minEatingSpeed(a, 8) << endl; // 4

    vector<int> b = {30, 11, 23, 4, 20};
    cout << minEatingSpeed(b, 5) << endl; // 30

    vector<int> c = {30, 11, 23, 4, 20};
    cout << minEatingSpeed(c, 6) << endl; // 23

    vector<int> d = {312884470};
    cout << minEatingSpeed(d, 968709470) << endl; // 1  (bahut time -> dheeमी speed)

    return 0;
}
