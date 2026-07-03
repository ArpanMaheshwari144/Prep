// ============================================================
// FROG JUMP  (DP)
// ============================================================
// Frog index 0 pe hai, index n-1 tak pahunchna hai.
// har jagah se 1 ya 2 step aage jump kar sakta.
// ek jump ka COST = |height ka farak| (start aur landing).
// MINIMUM total cost nikaalo top tak pahunchne ka.
//
// (soch: fib jaisा structure... par ab tu ADD nahi kar raha — ab CHOICE hai + cost.)
//
// Tests (// expected):
//   [10,30,40,20]        -> 30
//   [10,20,30,10]        -> 20
//   [30,10,60,10,60,50]  -> 40
//   [10]                 -> 0
//   [10,50]              -> 40
// ============================================================

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int frogJump(vector<int>& heights) {
    // TODO: tera code (memo helper bana sakta)
    return 0;
}

int main() {
    vector<int> a1 = {10,30,40,20};
    vector<int> a2 = {10,20,30,10};
    vector<int> a3 = {30,10,60,10,60,50};
    vector<int> a4 = {10};
    vector<int> a5 = {10,50};

    cout << frogJump(a1) << " (expected 30)\n";
    cout << frogJump(a2) << " (expected 20)\n";
    cout << frogJump(a3) << " (expected 40)\n";
    cout << frogJump(a4) << " (expected 0)\n";
    cout << frogJump(a5) << " (expected 40)\n";
    return 0;
}
