// Q: Linked list ka middle node dhundho — slow-fast pointer se
#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    // Slow-fast se middle dhundh
    // Slow 1 step, fast 2 step. Fast end pe → slow middle pe.
    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        // fast 2x speed se jaata hai — jab fast end pe toh slow middle pe
        while(fast != nullptr && fast->next != nullptr){
            slow = slow->next;
            fast = fast->next->next;
        }
        // slow ab middle pe khada hai
        return slow;
    }
};

// --- NEECHE TOUCH MAT KAR ---
ListNode* makeList(int arr[], int n) {
    if (n == 0) return NULL;
    ListNode* head = new ListNode(arr[0]);
    ListNode* curr = head;
    for (int i = 1; i < n; i++) {
        curr->next = new ListNode(arr[i]);
        curr = curr->next;
    }
    return head;
}

int main() {
    Solution sol;

    int a1[] = {1, 2, 3, 4, 5};
    cout << "Test 1: " << sol.middleNode(makeList(a1, 5))->val;
    cout << (sol.middleNode(makeList(a1, 5))->val == 3 ? "  PASS" : "  FAIL") << endl;

    int a2[] = {1, 2, 3, 4};
    cout << "Test 2: " << sol.middleNode(makeList(a2, 4))->val;
    cout << (sol.middleNode(makeList(a2, 4))->val == 3 ? "  PASS" : "  FAIL") << endl;

    int a3[] = {1};
    cout << "Test 3: " << sol.middleNode(makeList(a3, 1))->val;
    cout << (sol.middleNode(makeList(a3, 1))->val == 1 ? "  PASS" : "  FAIL") << endl;

    int a4[] = {1, 2};
    cout << "Test 4: " << sol.middleNode(makeList(a4, 2))->val;
    cout << (sol.middleNode(makeList(a4, 2))->val == 2 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
