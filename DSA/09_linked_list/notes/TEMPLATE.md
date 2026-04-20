# Template: Linked List

## Reverse Linked List
```
3 pointers: prev, curr, next. Har node ka next ulta kar.

ListNode* prev = NULL;
ListNode* curr = head;
while(curr != NULL){
    ListNode* next = curr->next;    // agla yaad rakh
    curr->next = prev;              // pointer ulta kar
    prev = curr;                    // prev aage
    curr = next;                    // curr aage
}
return prev;                        // prev naya head hai
```

**IMPORTANT:** return prev, curr nahi — curr NULL hai.

## Middle of LL (kal karega)
```
Slow-fast. Slow 1 step, fast 2 step. Fast end pe → slow middle pe.
```

## Traverse
```
ListNode* temp = head;
while(temp != NULL){
    // temp->val dekho
    temp = temp->next;
}
```
