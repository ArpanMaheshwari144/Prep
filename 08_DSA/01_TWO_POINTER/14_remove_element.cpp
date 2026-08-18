// ============================================================
// REMOVE ELEMENT — Two Pointer  (fresh)
// ============================================================
// int array nums aur int val. array me se saare val ko HATA do (in-place),
// naya length k lautao. pehle k element me koi val nahi hona chahiye
// (baaki jagah kuch bhi ho, order koi bhi).
//   nums=[3,2,2,3], val=3  -> k=2, nums[0..1] = {2,2}
//   nums=[0,1,2,2,3,0,4,2], val=2 -> k=5, {0,1,3,0,4}
//
// Tests (checker k + pehle-k-element check karega):
//   [3,2,2,3], 3            -> k=2
//   [0,1,2,2,3,0,4,2], 2    -> k=5
//   [1], 1                  -> k=0
//   [4,5], 6                -> k=2  (koi val nahi -> sab bache)
//   [], 0                   -> k=0
//
// ---- ARPAN KI APPROACH ----
//  MOVE-ZEROES cousin. slow/fast pointer. fast poora array scan kare.
//  nums[fast] != val (rakhne wala element) -> nums[slow] = nums[fast], slow++.
//  nums[fast] == val -> kuch nahi, sirf fast++ (skip).
//  end me slow = kitne non-val bache = k. (bache hue shuru ke k me aa jaate.)
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int removeElement(vector<int> &nums, int val)
{
    int slow = 0, fast = 0;  // check({4, 5}, 6);
    while (fast < nums.size())
    {
        if (nums[fast] != val)
        {
            nums[slow] = nums[fast];
            slow++;
        }
        fast++;
    }
    return slow;
}

// ---------- checker (boilerplate, chhoo mat) ----------
void check(vector<int> nums, int val)
{
    vector<int> copy = nums;
    int k = removeElement(nums, val);
    bool ok = true;
    for (int i = 0; i < k; i++)
        if (nums[i] == val)
            ok = false; // pehle k me val nahi hona chahiye
    cout << "k=" << k << "  [ ";
    for (int i = 0; i < k; i++)
        cout << nums[i] << " ";
    cout << "]  -> " << (ok ? "VALID" : "INVALID") << "\n";
}

int main()
{
    check({3, 2, 2, 3}, 3);
    check({0, 1, 2, 2, 3, 0, 4, 2}, 2);
    check({1}, 1);
    check({4, 5}, 6);
    check({}, 0);
    return 0;
}
