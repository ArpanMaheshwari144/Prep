// Q: Linked list ke end se nth node hatao
#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    // End se nth node hatao
    // Slow-fast: fast ko n step aage bhej, phir dono saath chalo
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* slow = head;
        ListNode* fast = head;
        ListNode* dummy = head;

        // pehle total count nikalo
        int count = 0;
        while(dummy != nullptr){
            dummy = dummy->next;
            count++;
        }

        dummy = head;
        // agar pehla hi node hatana hai (n == total nodes)
        if(count == n){
            dummy = dummy->next;
            return dummy;
        }

        // fast ko n step aage bhej do
        for(int i=0;i<n;i++){
            fast = fast->next;
        }

        // ab dono saath chalo — jab fast end pe toh slow target ke peeche hai
        while(fast != nullptr && fast->next != nullptr){
            slow = slow->next;
            fast = fast->next;
        }
        // slow ka next skip karo — nth node hat gaya
        slow->next = slow->next->next;

        return head;
    }
};

// --- NEECHE TOUCH MAT KAR ---
ListNode* makeList(int arr[], int size) {
    if (size == 0) return NULL;
    ListNode* head = new ListNode(arr[0]);
    ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = new ListNode(arr[i]);
        curr = curr->next;
    }
    return head;
}

void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " → ";
        head = head->next;
    }
    cout << " → NULL";
}

bool checkList(ListNode* head, int arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (!head || head->val != arr[i]) return false;
        head = head->next;
    }
    return head == NULL;
}

int main() {
    Solution sol;

    int a1[] = {1, 2, 3, 4, 5};
    ListNode* r1 = sol.removeNthFromEnd(makeList(a1, 5), 2);
    cout << "Test 1: "; printList(r1);
    int e1[] = {1, 2, 3, 5};
    cout << (checkList(r1, e1, 4) ? "  PASS" : "  FAIL") << endl;

    // Remove last
    int a2[] = {1, 2, 3};
    ListNode* r2 = sol.removeNthFromEnd(makeList(a2, 3), 1);
    cout << "Test 2: "; printList(r2);
    int e2[] = {1, 2};
    cout << (checkList(r2, e2, 2) ? "  PASS" : "  FAIL") << endl;

    // Remove first (n = size)
    int a3[] = {1, 2, 3};
    ListNode* r3 = sol.removeNthFromEnd(makeList(a3, 3), 3);
    cout << "Test 3: "; printList(r3);
    int e3[] = {2, 3};
    cout << (checkList(r3, e3, 2) ? "  PASS" : "  FAIL") << endl;

    // Single element
    int a4[] = {1};
    ListNode* r4 = sol.removeNthFromEnd(makeList(a4, 1), 1);
    cout << "Test 4: "; printList(r4);
    cout << (r4 == NULL ? "  PASS" : "  FAIL") << endl;

    return 0;
}
