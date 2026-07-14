// ============================================================
// SORT ARRAY BY PARITY — Two Pointer  (REDO — paper pehle, code baad me)
// ============================================================
// int array nums. SAARE EVEN pehle, phir saare ODD (aapas ka order koi bhi).
//   [3,1,2,4]  -> evens {2,4} pehle, odds {1,3} baad

// ---- ARPAN KI APPROACH ----
//  even pehle, odd baad. left/right pointer.
//  condition: left ODD && right EVEN (dono galat jagah) -> turant swap + left++ right--.
//  left EVEN (sahi jagah) -> left++.  warna -> right--.

//
// Tests (checker "VALID/INVALID" bata dega):
//   [3,1,2,4] · [2,3] · [2,1] · [0,1,2] · [1,3,5] · [2,4,6] · []
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> sortArrayByParity(vector<int> nums)
{
    int left = 0;
    int right = nums.size() - 1;
    while (left < right)
    {
        if (nums[left] % 2 != 0 && nums[right] % 2 == 0)
        {
            swap(nums[left], nums[right]);
            left++;
            right--;
        }
        else if (nums[left] % 2 == 0)
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    return nums;
}

// ---------- checker (boilerplate, chhoo mat) ----------
void check(vector<int> nums)
{
    vector<int> r = sortArrayByParity(nums);
    bool seenOdd = false, ok = true;
    for (int x : r)
    {
        if (x % 2 != 0)
            seenOdd = true;
        else if (seenOdd)
            ok = false;
    }
    cout << "[ ";
    for (int x : r)
        cout << x << " ";
    cout << "]  -> " << (ok ? "VALID" : "INVALID") << "\n";
}

int main()
{
    check({3, 1, 2, 4});
    check({2, 3});
    check({2, 1});
    check({0, 1, 2});
    check({1, 3, 5});
    check({2, 4, 6});
    check({});
    return 0;
}
