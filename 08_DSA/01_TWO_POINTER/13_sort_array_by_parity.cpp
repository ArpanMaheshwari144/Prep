// ============================================================
// SORT ARRAY BY PARITY — Two Pointer  (fresh)
// ============================================================
// int array nums. rearrange karo taaki SAARE EVEN numbers pehle, phir saare ODD.
// even-even ya odd-odd ka aapas ka order koi bhi ho (koi bhi valid answer chalega).
//   [3,1,2,4]  -> [2,4,3,1]  (ya [4,2,1,3] etc -- evens pehle, odds baad)
//   [0]        -> [0]
//
// Tests niche khud check kar raha hoon (evens-first, phir odds -- order koi bhi):
//   [3,1,2,4]     -> evens {2,4} pehle, odds {1,3} baad
//   [0,1,2]       -> evens {0,2} pehle, odd {1}
//   [1,3,5]       -> all odd -> same
//   [2,4,6]       -> all even -> same
//   []            -> []
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

// tera code yahan:
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
    // valid? -> koi even kisi odd ke BAAD nahi hona chahiye
    bool seenOdd = false, ok = true;
    for (int x : r)
    {
        if (x % 2 != 0)
            seenOdd = true;
        else if (seenOdd)
            ok = false; // even mila odd ke baad -> galat
    }
    cout << "[ ";
    for (int x : r)
        cout << x << " ";
    cout << "]  -> " << (ok ? "VALID" : "INVALID") << "\n";
}

int main()
{
    check({3, 1, 2, 4});
    check({3, 0, 2, 4});
    check({3, 1, 3, 4});
    check({0, 1, 2});
    check({1, 3, 5});
    check({2, 4, 6});
    check({});
    // ★ ye chhupe case abhi INVALID aayenge (left even && right odd wala galat branch):
    check({2, 3});
    check({2, 1});
    check({4, 2, 6, 1});
    return 0;
}
