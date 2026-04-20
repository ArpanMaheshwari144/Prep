// Q: x ka integer square root nikalo binary search se
#include <iostream>
using namespace std;

class Solution {
public:
    // x ka square root nikaal (integer part)
    // Example: sqrt(8) = 2 (2*2=4 <= 8, 3*3=9 > 8)
    // Example: sqrt(16) = 4
    // Hint: 1 se x tak BS lagao. mid*mid <= x? Right jao. mid*mid > x? Left jao.
    int mySqrt(int x) {
        int low = 1;
        int high = x;
        while(low <= high){
            int mid = (low + (high-low)/2);
            // mid*mid abhi x ke andar hai — aur bada try karo
            if(mid * mid <= x){
                low = mid+1;
            }
            // mid*mid x se bada — chhota karo
            else{
                high = mid-1;
            }
        }
        // high pe last valid sqrt hai jahan mid*mid <= x tha
        return high;
    }
};

int main() {
    Solution sol;

    cout << "Test 1: sqrt(4) → " << sol.mySqrt(4);
    cout << (sol.mySqrt(4) == 2 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: sqrt(8) → " << sol.mySqrt(8);
    cout << (sol.mySqrt(8) == 2 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: sqrt(16) → " << sol.mySqrt(16);
    cout << (sol.mySqrt(16) == 4 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: sqrt(1) → " << sol.mySqrt(1);
    cout << (sol.mySqrt(1) == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: sqrt(0) → " << sol.mySqrt(0);
    cout << (sol.mySqrt(0) == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
