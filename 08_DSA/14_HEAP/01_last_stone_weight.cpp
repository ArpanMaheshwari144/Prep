// ============================================================
// LAST STONE WEIGHT — (LeetCode 1046, Easy)   [HEAP #1 · max-heap]
// ============================================================
// stones[] = pathro ke weights. har turn me DO SABSE BHAARI pathar utha ke SMASH:
//   dono barabar (x==y)      -> dono khatam
//   alag (x != y, y bada)    -> chhota khatam, bada ban jaata (y - x)
// jab tak <= 1 pathar bache. AAKHRI pathar ka weight lautao (0 agar koi na bache).
//
//   [2,7,4,1,8,1] -> 1     (8,7->1 | 4,2->2 | 2,1->1 | 1,1->0 | bacha 1)
//   [1]           -> 1
//   [1,1]         -> 0
//   [3,7,2]       -> 2     (7,3->4 | 4,2->2)
//   []            -> 0
//
//   HEAP idea: hamesha "2 sabse bhaari" chahiye -> MAX-HEAP. pop 2, smash, diff wapas push.
//   C++ max-heap: priority_queue<int> pq;   pq.push(x); pq.top(); pq.pop();
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (MAX-HEAP; hamesha 2 sabse bhaari chahiye)
//  idea: har turn "2 SABSE BHAARI" pathar chahiye -> MAX-HEAP (bada top pe). pop 2, smash, diff wapas push.
//  heap kyun (sort nahi): har smash ke baad pathar BADALTE -> baar-baar "2 max" chahiye. sort har-baar
//     mahenga; max-heap: pop=O(log n), top=O(1) -> perfect "changing set + extreme chahiye".
int lastStoneWeight(vector<int> &stones)
{
    // STEP 1: saare stones MAX-HEAP me (bada top pe)
    priority_queue<int> pq; // max heap
    for (int i = 0; i < stones.size(); i++)
    {
        pq.push(stones[i]);
    }

    // STEP 2: 2 sabse bhaari smash karo jab tak 1 se zyada bache
    while (pq.size() > 1)
    {
        int x = pq.top(); // sabse bada
        pq.pop();

        int y = pq.top(); // doosra sabse bada
        pq.pop();

        int diff = abs(y - x); // bacha hua weight (barabar -> 0 = dono khatam)
        if (diff > 0)
        {
            pq.push(diff); // bacha pathar wapas heap me
        }
    }

    // STEP 3: koi pathar nahi -> 0, warna aakhri ka weight
    return pq.empty() ? 0 : pq.top();
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int main()
{
    vector<int> s1 = {2, 7, 4, 1, 8, 1};
    cout << "T1: " << lastStoneWeight(s1) << " (exp 1)\n";
    vector<int> s2 = {1};
    cout << "T2: " << lastStoneWeight(s2) << " (exp 1)\n";
    vector<int> s3 = {1, 1};
    cout << "T3: " << lastStoneWeight(s3) << " (exp 0)\n";
    vector<int> s4 = {3, 7, 2};
    cout << "T4: " << lastStoneWeight(s4) << " (exp 2)\n";
    vector<int> s5 = {};
    cout << "T5: " << lastStoneWeight(s5) << " (exp 0)\n";
    return 0;
}
