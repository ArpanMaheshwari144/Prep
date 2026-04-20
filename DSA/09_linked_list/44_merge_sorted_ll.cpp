// Q: Do sorted linked lists ko merge karke ek sorted list banao
#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    // Do sorted linked lists merge karo — ek sorted list banao
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        // agar ek list khaali toh dusri return karo
        if(l1 == nullptr){
            return l2;
        }
        else if(l2 == nullptr){
            return l1;
        }
        // l1 ka pehla node new head banega
        ListNode* newhead = l1;
        l1 = l1->next;
        // dummy pointer — merged list build karne ke liye
        ListNode* dummy = newhead;
        // jab tak dono lists mein elements hain — chhota wala aage jodo
        while(l1 != nullptr && l2 != nullptr){
            if(l1->val < l2->val){
                dummy->next = l1;
                l1 = l1->next;
                dummy = dummy->next;
            }
            else{
                dummy->next = l2;
                l2 = l2->next;
                dummy = dummy->next;
            }
        }
        // l1 bachi toh baaki jod do
        while(l1 != nullptr){
            dummy->next = l1;
            l1 = l1->next;
            dummy = dummy->next;
        }
        // l2 bachi toh baaki jod do
        while(l2 != nullptr){
            dummy->next = l2;
            l2 = l2->next;
            dummy = dummy->next;
        }
        return newhead;
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

    int a1[] = {1, 3, 5};
    int a2[] = {2, 4, 6};
    ListNode* r1 = sol.mergeTwoLists(makeList(a1, 3), makeList(a2, 3));
    cout << "Test 1: "; printList(r1);
    int e1[] = {1, 2, 3, 4, 5, 6};
    cout << (checkList(r1, e1, 6) ? "  PASS" : "  FAIL") << endl;

    // One empty
    int a3[] = {1, 2, 3};
    ListNode* r2 = sol.mergeTwoLists(makeList(a3, 3), NULL);
    cout << "Test 2: "; printList(r2);
    int e2[] = {1, 2, 3};
    cout << (checkList(r2, e2, 3) ? "  PASS" : "  FAIL") << endl;

    // Both empty
    ListNode* r3 = sol.mergeTwoLists(NULL, NULL);
    cout << "Test 3: "; printList(r3);
    cout << (r3 == NULL ? "  PASS" : "  FAIL") << endl;

    // Different sizes
    int a4[] = {1};
    int a5[] = {2, 4, 6, 8};
    ListNode* r4 = sol.mergeTwoLists(makeList(a4, 1), makeList(a5, 4));
    cout << "Test 4: "; printList(r4);
    int e4[] = {1, 2, 4, 6, 8};
    cout << (checkList(r4, e4, 5) ? "  PASS" : "  FAIL") << endl;

    return 0;
}
