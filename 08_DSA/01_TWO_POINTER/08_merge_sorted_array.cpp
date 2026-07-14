// ============================================================
// MERGE SORTED ARRAY — Two Pointer  (fresh, in-place from back)
// ============================================================
// nums1[] size (m+n) — pehle m elements valid + sorted, aakhir me n ZEROS (jagah).
// nums2[] size n — sorted. nums2 ko nums1 me MERGE karo -> nums1 poora sorted ho jaaye.
// (IN-PLACE, nums1 me hi. return void.)
//   nums1=[1,2,3,0,0,0] m=3, nums2=[2,5,6] n=3  ->  nums1=[1,2,2,3,5,6]
//

// ---- ARPAN KI APPROACH ----
//  PEECHE se bharo (squares-sorted jaisa). 3 pointer: i=m-1 (nums1 last valid), j=n-1 (nums2 last),
//  k=m+n-1 (bharne ki jagah, end). ★ galti bachi: k = m+n-1 hai, m*n-1 NAHI.
//  while(i>=0 && j>=0): dono value compare -> BADA nums1[k] me daalo -> us pointer + k ghatao.
//  ★ && lagao (|| NAHI): jab dono compare kar rahe to DONO valid hone chahiye. || lagate to ek khatam
//    hone par doosra kis se compare karta? -> galat. isliye &&; aur ek khatam wala case NEECHE ke
//    do while-loops me handle (jo bacha use seedha bhar do). (ye pattern kai merge-Q me: main-loop && + tail-loops)


//
// Tests (nums1, m, nums2, n -> expected nums1):
//   [1,2,3,0,0,0], m=3, [2,5,6], n=3  -> 1 2 2 3 5 6
//   [1],           m=1, [],      n=0  -> 1
//   [0],           m=0, [1],     n=1  -> 1
//   [4,5,6,0,0,0], m=3, [1,2,3], n=3  -> 1 2 3 4 5 6
//   [1,2,4,5,6,0], m=5, [3],     n=1  -> 1 2 3 4 5 6
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int> &nums1, int m, vector<int> &nums2, int n)
{
    int i = m - 1;
    int j = n - 1;
    int k = m + n - 1;

    while (i >= 0 && j >= 0)
    {
        if (nums1[i] < nums2[j])
        {
            nums1[k] = nums2[j];
            j--;
            k--;
        }
        else
        {
            nums1[k] = nums1[i];
            i--;
            k--;
        }
    }

    while (i >= 0)
    {
        nums1[k] = nums1[i];
        i--;
        k--;
    }

    while (j >= 0)
    {
        nums1[k] = nums2[j];
        j--;
        k--;
    }
}

int main()
{
    vector<int> a1 = {1, 2, 3, 0, 0, 0}, a2 = {2, 5, 6};
    vector<int> b1 = {1}, b2 = {};
    vector<int> c1 = {0}, c2 = {1};
    vector<int> d1 = {4, 5, 6, 0, 0, 0}, d2 = {1, 2, 3};
    vector<int> e1 = {1, 2, 4, 5, 6, 0}, e2 = {3};

    auto p = [](vector<int> v)
    { for (int x : v) cout << x << " "; cout << "\n"; };

    merge(a1, 3, a2, 3);
    p(a1); // 1 2 2 3 5 6
    merge(b1, 1, b2, 0);
    p(b1); // 1
    merge(c1, 0, c2, 1);
    p(c1); // 1
    merge(d1, 3, d2, 3);
    p(d1); // 1 2 3 4 5 6
    merge(e1, 5, e2, 1);
    p(e1); // 1 2 3 4 5 6
    return 0;
}
