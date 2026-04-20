// Q: 1 se n tak ka sum nikalo recursion se
#include <iostream>
using namespace std;

class Solution {
public:
    // 1 se n tak ka sum nikaal recursively
    // sum(5) = 15 (1+2+3+4+5)
    // Base case: n=1 → return 1
    int sumN(int n) {
        // base case — agar 1 hai toh 1 return karo
        if(n==1){
            return 1;
        }
        // n + baaki sab ka sum (n-1 tak)
        int sum = n + sumN(n-1);
        return sum;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: sum(5) → " << sol.sumN(5);
    cout << (sol.sumN(5) == 15 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: sum(1) → " << sol.sumN(1);
    cout << (sol.sumN(1) == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: sum(10) → " << sol.sumN(10);
    cout << (sol.sumN(10) == 55 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: sum(3) → " << sol.sumN(3);
    cout << (sol.sumN(3) == 6 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
