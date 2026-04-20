// Q: nth Fibonacci number nikalo recursion se
#include <iostream>
using namespace std;

class Solution {
public:
    int fib(int n) {
        // base case — fib(0) = 0
        if(n==0){
            return 0;
        }
        // base case — fib(1) = 1
        if(n==1){
            return 1;
        }

        // fib(n) = fib(n-1) + fib(n-2) — do pichle ka sum
        int ans = fib(n-1) + fib(n-2);
        return ans;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: fib(0) → " << sol.fib(0);
    cout << (sol.fib(0) == 0 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: fib(1) → " << sol.fib(1);
    cout << (sol.fib(1) == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: fib(5) → " << sol.fib(5);
    cout << (sol.fib(5) == 5 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: fib(10) → " << sol.fib(10);
    cout << (sol.fib(10) == 55 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
