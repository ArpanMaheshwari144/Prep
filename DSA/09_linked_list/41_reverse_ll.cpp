// Q: Linked list ko reverse karo — 3 pointer technique
#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    // Linked list reverse karo
    // 3 pointers: prev, curr, next
    ListNode* reverseList(ListNode* head) {
        // prev = ulta list ka naya head banega
        ListNode* prev = NULL;
        ListNode* curr = head;
        // jab tak list khatam na ho
        while(curr != nullptr){
            // next save karo — link tootne se pehle
            ListNode* next = curr->next;
            // current ka link ulta karo — peeche point karo
            curr->next = prev;
            // prev aur curr ek step aage badhao
            prev = curr;
            curr = next;
        }
        // prev ab naya head hai
        return prev;
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

void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " → ";
        head = head->next;
    }
    cout << " → NULL";
}

bool checkList(ListNode* head, int arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (!head || head->val != arr[i]) return false;
        head = head->next;
    }
    return head == NULL;
}

int main() {
    Solution sol;

    int a1[] = {1, 2, 3, 4, 5};
    ListNode* h1 = sol.reverseList(makeList(a1, 5));
    cout << "Test 1: "; printList(h1);
    int e1[] = {5, 4, 3, 2, 1};
    cout << (checkList(h1, e1, 5) ? "  PASS" : "  FAIL") << endl;

    int a2[] = {1, 2};
    ListNode* h2 = sol.reverseList(makeList(a2, 2));
    cout << "Test 2: "; printList(h2);
    int e2[] = {2, 1};
    cout << (checkList(h2, e2, 2) ? "  PASS" : "  FAIL") << endl;

    ListNode* h3 = sol.reverseList(NULL);
    cout << "Test 3: "; printList(h3);
    cout << (h3 == NULL ? "  PASS" : "  FAIL") << endl;

    int a4[] = {7};
    ListNode* h4 = sol.reverseList(makeList(a4, 1));
    cout << "Test 4: "; printList(h4);
    int e4[] = {7};
    cout << (checkList(h4, e4, 1) ? "  PASS" : "  FAIL") << endl;

    return 0;
}
