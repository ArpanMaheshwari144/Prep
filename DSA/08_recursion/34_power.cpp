// Q: base^exp nikalo recursion se
#include <iostream>
using namespace std;

class Solution {
public:
    int power(int base, int exp) {
       // base case — koi bhi number ka 0th power 1 hota hai
       if(exp == 0){
        return 1;
       }

       // base * base^(exp-1) — ek baar multiply aur baaki recursion pe chhod do
       int ans = base * power(base, exp-1);
       return ans;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: pow(2,5) → " << sol.power(2, 5);
    cout << (sol.power(2, 5) == 32 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: pow(3,3) → " << sol.power(3, 3);
    cout << (sol.power(3, 3) == 27 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: pow(5,0) → " << sol.power(5, 0);
    cout << (sol.power(5, 0) == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: pow(2,10) → " << sol.power(2, 10);
    cout << (sol.power(2, 10) == 1024 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
