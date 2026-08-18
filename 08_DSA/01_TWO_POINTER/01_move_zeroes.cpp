// ============================================================
// MOVE ZEROES
// ============================================================
// Ek array diya. SAARE 0 ko END me le jao, non-zero elements ka ORDER same rahe.
// IN-PLACE karo (naya array nahi).
//
// Tests (// expected):
//   [0,1,0,3,12]  -> 1 3 12 0 0
//   [0]           -> 0
//   [1,2,3]       -> 1 2 3
//   [0,0,1]       -> 1 0 0
//   [4,0,5,0,0,6] -> 4 5 6 0 0 0
// ============================================================
// ---- ARPAN KI APPROACH ----
//  2 pointer slow/fast, dono 0 se start. fast poore array pe chalta.
//  jab nums[fast] != 0 -> swap(slow, fast) -> slow++. fast har baar ++.
//  non-zero aage swap hote jaate, 0 apne aap peeche reh jaate. order bana rehta.
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

void moveZeroes(vector<int> &nums)
{
    int slow = 0;
    int fast = 0;
    while (fast < nums.size()) // [0,1,0,3,12]  
    {
        if (nums[fast] != 0)
        {
            swap(nums[slow], nums[fast]);
            slow++;
        }
        fast++;
    }
}

void print(vector<int> &v)
{
    for (int x : v)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    vector<int> a1 = {0, 1, 0, 3, 12};
    vector<int> a2 = {0};
    vector<int> a3 = {1, 2, 3};
    vector<int> a4 = {0, 0, 1};
    vector<int> a5 = {4, 0, 5, 0, 0, 6};

    moveZeroes(a1);
    print(a1); // 1 3 12 0 0
    moveZeroes(a2);
    print(a2); // 0
    moveZeroes(a3);
    print(a3); // 1 2 3
    moveZeroes(a4);
    print(a4); // 1 0 0
    moveZeroes(a5);
    print(a5); // 4 5 6 0 0 0
    return 0;
}
