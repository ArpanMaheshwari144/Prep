// Q: n ka factorial nikalo recursion se
#include <iostream>
using namespace std;

class Solution {
public:
    int factorial(int n) {
       // base case — 0! = 1! = 1
       if(n == 1 || n==0){
        return 1;
       }
       // n * (n-1)! — recursive call
       int fact = n * factorial(n-1);
       return fact;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: fact(5) → " << sol.factorial(5);
    cout << (sol.factorial(5) == 120 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: fact(1) → " << sol.factorial(1);
    cout << (sol.factorial(1) == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: fact(0) → " << sol.factorial(0);
    cout << (sol.factorial(0) == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: fact(3) → " << sol.factorial(3);
    cout << (sol.factorial(3) == 6 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
