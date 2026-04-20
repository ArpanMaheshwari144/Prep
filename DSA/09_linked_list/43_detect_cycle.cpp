// Q: Linked list mein cycle hai ya nahi detect karo — Floyd's algo
#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    // Cycle hai ya nahi? true/false
    // Slow-fast — mile toh cycle, fast NULL pe toh no cycle
    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        // fast 2 step, slow 1 step — agar cycle hai toh zaroor milenge
        while(fast != nullptr && fast->next != nullptr){
            slow = slow->next;
            fast = fast->next->next;

            // dono mil gaye — cycle confirm
            if(slow == fast){
                return true;
            }
        }
        // fast NULL pe pahunch gaya — cycle nahi hai
        return false;
    }
};

int main() {
    // Test 1: Cycle hai
    ListNode* h1 = new ListNode(1);
    h1->next = new ListNode(2);
    h1->next->next = new ListNode(3);
    h1->next->next->next = new ListNode(4);
    h1->next->next->next->next = h1->next; // 4 → 2 cycle

    Solution sol;
    cout << "Test 1: " << (sol.hasCycle(h1) ? "true" : "false");
    cout << (sol.hasCycle(h1) == true ? "  PASS" : "  FAIL") << endl;

    // Test 2: No cycle
    ListNode* h2 = new ListNode(1);
    h2->next = new ListNode(2);
    h2->next->next = new ListNode(3);
    cout << "Test 2: " << (sol.hasCycle(h2) ? "true" : "false");
    cout << (sol.hasCycle(h2) == false ? "  PASS" : "  FAIL") << endl;

    // Test 3: Single node, no cycle
    ListNode* h3 = new ListNode(1);
    cout << "Test 3: " << (sol.hasCycle(h3) ? "true" : "false");
    cout << (sol.hasCycle(h3) == false ? "  PASS" : "  FAIL") << endl;

    // Test 4: Empty
    cout << "Test 4: " << (sol.hasCycle(NULL) ? "true" : "false");
    cout << (sol.hasCycle(NULL) == false ? "  PASS" : "  FAIL") << endl;

    // Test 5: Self loop
    ListNode* h5 = new ListNode(1);
    h5->next = h5; // 1 → 1 self loop
    cout << "Test 5: " << (sol.hasCycle(h5) ? "true" : "false");
    cout << (sol.hasCycle(h5) == true ? "  PASS" : "  FAIL") << endl;

    return 0;
}
