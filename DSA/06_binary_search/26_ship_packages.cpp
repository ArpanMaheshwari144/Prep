// Q: Minimum ship capacity dhundho jisse D days mein saare packages ship ho jaayein
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

class Solution {
public:
    // Tu delivery boy hai. Gaadi ki capacity "capacity" hai.
    // Packages order mein uthao. Capacity cross ho → naya din.
    // Kitne din lage? days ke andar hua → true.
    // CONTINUOUS: add karte jao, cross ho → day++, current package se restart
    bool canShip(vector<int>& weights, int capacity, int days) {
        // ek din se start, weight jodte jao
        int day = 1;
        int sum = 0;
        for(int i=0;i<weights.size();i++){
            sum += weights[i];
            // capacity cross ho gayi — naya din shuru, current package se restart
            if(sum > capacity){
                day++;
                sum = weights[i];
            }
        }
        // total days allowed ke andar hua ya nahi
        return day <= days;
    }

    // Minimum capacity dhundh jisse sab packages "days" dinon mein ship ho
    // low = max(weights) — isse kam pe sabse bhari package nahi uthega
    // high = sum(weights) — ek din mein sab le ja
    int shipWithinDays(vector<int>& weights, int days) {
        int ans = 0;
        // binary search range: max single weight se total weight tak
        int low = *max_element(begin(weights), end(weights));
        int high = accumulate(begin(weights), end(weights), 0);
        while(low <= high){
            int mid = (low + (high-low)/2);
            // is capacity se ship ho sakta? aur chhota try karo
            if(canShip(weights, mid, days)){
                ans = mid;
                high = mid-1;
            }
            // nahi ho sakta — capacity badhao
            else{
                low = mid+1;
            }
        }
        return ans;
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "Test 1: days=5 → " << sol.shipWithinDays(t1, 5);
    cout << (sol.shipWithinDays(t1, 5) == 15 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {3, 2, 2, 4, 1, 4};
    cout << "Test 2: days=3 → " << sol.shipWithinDays(t2, 3);
    cout << (sol.shipWithinDays(t2, 3) == 6 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {1, 2, 3, 1, 1};
    cout << "Test 3: days=4 → " << sol.shipWithinDays(t3, 4);
    cout << (sol.shipWithinDays(t3, 4) == 3 ? "  PASS" : "  FAIL") << endl;

    // Edge: ek package
    vector<int> t4 = {10};
    cout << "Test 4: days=1 → " << sol.shipWithinDays(t4, 1);
    cout << (sol.shipWithinDays(t4, 1) == 10 ? "  PASS" : "  FAIL") << endl;

    // Edge: har din ek package
    vector<int> t5 = {1, 1, 1, 1, 1};
    cout << "Test 5: days=5 → " << sol.shipWithinDays(t5, 5);
    cout << (sol.shipWithinDays(t5, 5) == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
