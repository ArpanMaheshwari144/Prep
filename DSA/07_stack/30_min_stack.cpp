// Q: Stack design karo jisme push, pop, top ke saath O(1) mein minimum mile
#include <iostream>
#include <stack>
#include <climits>
using namespace std;
class MinStack {
public:
    // push, pop, top — normal stack jaisa
    // getMin — O(1) mein stack ka minimum batao

    // st1 = main stack, st2 = minimum track karne ka stack
    stack<int> st1;
    stack<int> st2;

    void push(int val) {
        // main stack mein daalo
        st1.push(val);
        // agar val current min se bada hai toh purana min repeat karo st2 mein
        if(!st2.empty() && val > st2.top()){
            st2.push(st2.top());
        }
        // nahi toh naya min mil gaya — wo daalo
        else{
            st2.push(val);
        }
    }

    void pop() {
        // dono stacks se pop karo — sync mein rehne chahiye
        if(st1.empty() == false){
            st1.pop();
        }
        if(st2.empty() == false){
            st2.pop();
        }
    }

    int top() {
        // main stack ka top return karo
        if(st1.empty() == false){
            return st1.top();
        }
        return -1;
    }

    int getMin() {
        // min stack ka top hi current minimum hai
        if(st2.empty() == false){
            return st2.top();
        }
        return -1;
    }
};

int main() {
    MinStack ms;

    ms.push(5);
    ms.push(3);
    ms.push(7);
    cout << "Test 1: getMin → " << ms.getMin();
    cout << (ms.getMin() == 3 ? "  PASS" : "  FAIL") << endl;

    ms.pop();
    cout << "Test 2: getMin → " << ms.getMin();
    cout << (ms.getMin() == 3 ? "  PASS" : "  FAIL") << endl;

    ms.pop();
    cout << "Test 3: getMin → " << ms.getMin();
    cout << (ms.getMin() == 5 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: top → " << ms.top();
    cout << (ms.top() == 5 ? "  PASS" : "  FAIL") << endl;

    ms.push(1);
    cout << "Test 5: getMin → " << ms.getMin();
    cout << (ms.getMin() == 1 ? "  PASS" : "  FAIL") << endl;

    ms.push(1);
    ms.pop();
    cout << "Test 6: getMin → " << ms.getMin();
    cout << (ms.getMin() == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
