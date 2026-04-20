// Q: Koko ki minimum eating speed dhundho jisse h hours mein saare bananas kha le
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

class Solution {
public:
    // Koko bandar hai. Har pile INDEPENDENT hai.
    // Speed "speed" hai toh har pile mein ceil(pile/speed) hours lagenge.
    // Total hours count kar. h ke andar hua → true.
    // INDEPENDENT: har pile ka ceil nikaal ke add karo
    // ceil ke liye (double) cast karo — int/int mein decimal katega
    bool canEat(vector<int>& piles, int speed, int h) {
        int hour = 0;
        // har pile ke liye ceil(pile/speed) hours lagenge
        for(int i=0;i<piles.size();i++){
            hour += ceil((double)piles[i]/speed);
        }
        // total hours h ke andar hai ya nahi
        return hour <= h;
    }

    // Minimum speed dhundh jisse sab bananas h hours mein kha le
    // low = 1 (minimum speed, 0 nahi — divide by zero)
    // high = max(piles) (sabse badi pile ek hour mein kha le)
    int minEatingSpeed(vector<int>& piles, int h) {
        int ans = 0;
       int low = 1;
       int high = *max_element(begin(piles), end(piles));
       while(low <= high){
        int mid = (low + (high-low)/2);
        // is speed se kha sakta — aur slow try karo
        if(canEat(piles, mid, h)){
            ans = mid;
            high = mid-1;
        }
        // nahi kha sakta — speed badhao
        else{
            low = mid+1;
        }
       }
       return ans;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {3, 6, 7, 11};
    cout << "Test 1: h=8 → " << sol.minEatingSpeed(t1, 8);
    cout << (sol.minEatingSpeed(t1, 8) == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {30, 11, 23, 4, 20};
    cout << "Test 2: h=5 → " << sol.minEatingSpeed(t2, 5);
    cout << (sol.minEatingSpeed(t2, 5) == 30 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {30, 11, 23, 4, 20};
    cout << "Test 3: h=6 → " << sol.minEatingSpeed(t3, 6);
    cout << (sol.minEatingSpeed(t3, 6) == 23 ? "  PASS" : "  FAIL") << endl;

    // Edge: sab 1
    vector<int> t4 = {1, 1, 1, 1};
    cout << "Test 4: h=4 → " << sol.minEatingSpeed(t4, 4);
    cout << (sol.minEatingSpeed(t4, 4) == 1 ? "  PASS" : "  FAIL") << endl;

    // Edge: ek pile
    vector<int> t5 = {10};
    cout << "Test 5: h=5 → " << sol.minEatingSpeed(t5, 5);
    cout << (sol.minEatingSpeed(t5, 5) == 2 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
