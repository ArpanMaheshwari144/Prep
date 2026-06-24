// ============================================================
// KTH LARGEST ELEMENT IN AN ARRAY   —   Heap  [pehla heap problem]
// ============================================================
// nums diya, k diya. K-th sabse BADA element nikaalo.
//
//   [3,2,1,5,6,4], k=2 -> 5   (bada-se-chhota: 6,5,4,3,2,1 -> 2nd = 5)
//   [3,2,3,1,2,4,5,5,6], k=4 -> 4
//
// SIGNAL: "Kth largest / top-K" -> HEAP (priority_queue).
//   Trick (jo samjha): size-K MIN-HEAP.
//     - har element min-heap me push.
//     - size K se ZYADA ho jaaye -> top (sabse chhota) POP kar do.
//     - ant me heap me K SABSE BADE bachte -> top() = Kth largest.
//   (ya simple: max-heap me sab push -> k-1 baar pop -> top = Kth largest. dono chalega.)
//   C++: priority_queue<int, vector<int>, greater<int>> minHeap;   // min-heap
//   No code -- tu likh.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- TERA kaam: yeh function bhar (heap) ----
int findKthLargest(vector<int> nums, int k)
{
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (auto &it : nums)
    {
        minHeap.push(it);
        if (minHeap.size() > k)
        {
            minHeap.pop();
        }
    }
    return minHeap.top();
}

int main()
{
    cout << findKthLargest({3, 2, 1, 5, 6, 4}, 2) << endl;          // expected 5
    cout << findKthLargest({3, 2, 3, 1, 2, 4, 5, 5, 6}, 4) << endl; // expected 4
    cout << findKthLargest({1}, 1) << endl;                         // expected 1
    cout << findKthLargest({7, 6, 5, 4, 3, 2, 1}, 5) << endl;       // expected 3
    cout << findKthLargest({2, 1}, 2) << endl;                      // expected 1
    return 0;
}
