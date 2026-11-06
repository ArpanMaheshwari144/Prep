## PATTERN 8 — LINKED LIST

```
> **YAHAN NAHI MILEGI:**
> ```
> merge K sorted lists   ->  PATTERN 14 (HEAP)
>       node/list dikhta hai, par asli kaam MIN-HEAP of current heads ka hai
>       (jodne ka engine — dummy + tail — wahi hai jo merge-TWO me hai)
> ```


 BROAD IDEA: pointer manipulation. 3 broad-trick: FAST/SLOW · REVERSE (3-pointer) · DUMMY node.
 ★ EDGE (fast-slow): loop ka SHURU chahiye -> milne ke baad slow ko HEAD pe, dono 1-1 step.
         sirf "loop hai ya nahi" poocha to ye step nahi chahiye.

 LOOP-CONDITION (while me kya likhun) -- ek sawaal: aakhri node ka bhi kaam hai, ya sirf uske NEXT ko dekhna?
     while (curr != NULL)        // har node ka apna kaam (get / remove / count / print) -> ZYADATAR yahi
     while (curr->next != NULL)  // curr ka NEXT dekhna, ya aakhri node PE rukna (tail pakad ke aage jodna)
     // A->B->NULL: curr!=NULL B tak jaake B ka kaam karta; curr->next!=NULL B se pehle ruk jaata

```

#### ── FAMILY: FAST/SLOW (slow 1 kadam, fast 2 kadam ya gap; middle / cycle / palindrome / nth-from-end) ──

```
 KAB: node / next diya · middle · cycle · ulta karo · nth from end · do list jodo
 ★ DHYAAN: jodna / hatana -> DUMMY node (head bhi badal sakta)
```

### ▸ MIDDLE OF LIST

```
Linked list ka head diya hai. Beech wala node lautao. Agar list ki lambai even hai aur do beech wale node hain, to doosra wala lautao.
  e.g. 1->2->3->4->5->6 -> 4
  kyun: 6 node me beech ke do 3 aur 4 -> doosra = 4 (odd 1->2->3->4->5 hota to 3)
```

```
TASVEER (purani sheet se, same to same):

FAST/SLOW: slow 1 step, fast 2 step; fast end pe -> slow MIDDLE pe.
   Node *slow=head, *fast=head;   // dono head se: even list me 2nd middle milta
   while (fast && fast->next) { slow=slow->next; fast=fast->next->next; }   // fast pehle check (NULL->next crash), fast->next se next->next safe
LOOP-COND `fast && fast->next` dono zaroori: even [1,2,3,4] -> slow=3 (2nd middle); odd [1,2,3] -> slow=2.
   (fast->next check hataya -> fast->next->next NULL pe crash.)
```

```cpp
Node *middleNode(Node *head) {              // fast end pe -> slow MIDDLE pe
    Node *slow = head;                      // dono head se: even list me 2nd middle milta
    Node *fast = head;
    while (fast != NULL && fast->next != NULL) {   // ★ fast pehle check (NULL->next crash);
        slow = slow->next;                  //   fast->next check hataya -> fast->next->next crash
        fast = fast->next->next;
    }
    return slow;                            // [1,2,3,4] -> 3 (2nd middle) · [1,2,3] -> 2
}
```

### ▸ DETECT CYCLE

```
Linked list ka head diya hai. Agar list me cycle hai (koi node wapas kisi pichhle node pe point karta hai) to true, warna false.
  e.g. 3->2->0->-4, aur -4 ka next = 2 wala node -> true
  kyun: -4 ke baad phir 2 pe aa gaye -> 2,0,-4,2,... chakkar kabhi NULL nahi aata
```

```
TASVEER (purani sheet se, same to same):

FAST/SLOW (Floyd): cycle ho to tez-dheema MILENGE (slow==fast) -> true.
   Node *slow=head, *fast=head;   // dono head se: cycle ho to fast ghoom ke slow ko pakdega
   while (fast && fast->next) { slow=slow->next; fast=fast->next->next; if (slow==fast) return true; }   // check move ke BAAD: dono head se shuru, pehle check karte to turant true
   return false;   // fast NULL tak pahuncha = list ka end hai, cycle nahi
no-cycle -> fast NULL pe pahunch ke loop khatam -> false.
```

```cpp
bool hasCycle(Node *head) {                 // Floyd: cycle ho to tez-dheema MILENGE
    Node *slow = head;                      // dono head se: cycle ho to fast ghoom ke slow ko pakdega
    Node *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)                   // ★ check move ke BAAD: dono head se shuru,
            return true;                    //   pehle check karte to turant true
    }
    return false;                           // fast NULL tak pahuncha = list ka end, cycle nahi
}
```

### ▸ FIND DUPLICATE NUMBER (LC-287)   [= DETECT CYCLE ka ARRAY-application]

```
Array me n+1 number hain aur har value 1 se n ke beech hai, isliye ek number zaroor dohraya gaya hai. Wo number lautao.
Extra jagah O(1) aur array ko badalna mana hai.
  e.g. [1,3,4,2,2] -> 2
  kyun: n=4, values 1..4 me 2 do baar aaya

n+1 numbers, values 1..n, ek repeat. O(1) space + array READ-ONLY
PEHCHAN: "array + duplicate + O(1) space + no-modify" -> value = POINTER (i -> nums[i])
values 1..n + n+1 slot -> koi value 2 baar point -> loop; cycle ki ENTRY = duplicate
[3,1,3,4,2]: 0->3->4->2->3->4->2... (loop, entry 3 = dup)
```

```
TASVEER (purani sheet se, same to same):

SAAR: n+1 numbers, values 1..n, ek repeat. dhoondo. O(1) space + array READ-ONLY.
PEHCHAN: "array + duplicate + O(1)-space + no-modify" -> value ko POINTER samjho (i -> nums[i]).
= DETECT CYCLE, bas DELTA: (a) array-as-linked-list  (b) detect nahi, cycle-ENTRY chahiye -> PHASE-2.
KYUN CYCLE: values 1..n + n+1 slots -> koi value 2 baar point -> loop. entry-node = duplicate.
VISUAL: nums=[3,1,3,4,2]:  0->3->4->2->3->4->2...  (loop; entry=3=dup)
TEMPLATE:
   int slow=0, fast=0;   // 0 se: value 1..n, koi 0 pe point nahi karta -> 0 cycle ke bahar ka start
   slow=nums[slow]; fast=nums[nums[fast]];   // ★ PEHLE ek kadam chalao — dono 0 pe hi rahe to slow==fast, PHASE-1 loop chalega hi nahi
   while(slow!=fast){ slow=nums[slow]; fast=nums[nums[fast]]; }   // PHASE-1: cycle me tez-dheema milne tak chalao
   while(slow!=fast){ slow=nums[slow]; fast=nums[fast]; }          // PHASE-2 entry (ab fast bhi 1 kadam, 2 nahi)
   return slow;   // phase-2 me jahan mile wahi cycle entry = duplicate
crux: phase-2 = meet aur 0 se entry-tak distance BARABAR (Floyd) -> 0-reset + 1-1 kadam.
FAMILY: FAST/SLOW (detect_cycle cousin) — wahi Floyd, array pe.
```

```cpp
int findDuplicate(vector<int> &nums) {
    int slow = 0, fast = 0;                 // 0 se: value 1..n, koi 0 pe point nahi -> 0 cycle ke bahar
    slow = nums[slow], fast = nums[nums[fast]];   // ★ PEHLE ek kadam: 0 pe hi rahe to loop chalega nahi
    while (slow != fast) {                  // PHASE-1: cycle me tez-dheema milne tak
        slow = nums[slow];
        fast = nums[nums[fast]];
    }
    slow = 0;                               // ★ sirf slow wapas 0 pe, fast meet point pe hi rahe
    while (slow != fast) {                  // PHASE-2 entry: ab fast bhi 1 kadam, 2 nahi
        slow = nums[slow];                  //   (meet se aur 0 se entry tak doori BARABAR = Floyd)
        fast = nums[fast];
    }
    return slow;                            // jahan mile wahi cycle entry = duplicate
}
```

### ▸ LINKED LIST CYCLE II (LC-142)   [= DETECT CYCLE + phase-2 ENTRY]

```
Linked list ka head diya hai. Agar cycle hai to wo node lautao jahan se cycle shuru hoti hai, warna NULL.
  e.g. 3->2->0->-4, aur -4 ka next = 2 wala node -> node 2
  kyun: chakkar 2,0,-4,2,... hai, bahar se andar pehla kadam 2 pe padta -> entry 2

cycle ho to uske START node ka pointer, warna NULL. = find-duplicate ka Node* version (->next)
```

```
TASVEER (purani sheet se, same to same):

SAAR: cycle ho to uske START (entry) node ka pointer return; warna NULL.
= DETECT CYCLE (03), bas DELTA: true/false nahi, ENTRY chahiye -> PHASE-2.
= FIND-DUPLICATE (12) ka Node* version: nums[i] ki jagah ->next. SAME phase-2.
TEMPLATE:
   slow=head, fast=head;  bool cycle=false;   // flag: 1 node pe bina cycle bhi slow==fast hota
   while(fast && fast->next){ slow=slow->next; fast=fast->next->next; if(slow==fast){ cycle=true; break; } }   // break: fast meet point pe hi ruke
   if(!cycle) return NULL;              // no cycle (flag se decide, slow!=fast se nahi: 1 node pe dono head pe barabar)
   slow=head;   // sirf slow head pe, fast meet point pe hi rahe
   while(slow!=fast){ slow=slow->next; fast=fast->next; }   // PHASE-2 entry (dono 1-1 kadam)
crux: meet aur head se entry-tak distance BARABAR (Floyd) -> head-reset + 1-1 kadam.
GOTCHA (24-Sep theek kiya): pehle `if(slow!=fast) return NULL` likha tha -- GALAT.
   1 node, bina cycle: loop chalta hi nahi -> slow==fast==head reh jaate -> head return ho jaata (sahi NULL).
   -> alag `cycle` flag rakho, sirf slow==fast MILNE pe true.
FAMILY: FAST/SLOW TRIPLET — detect_cycle(03) + find-duplicate(12) + cycle-II(13), teeno wahi Floyd.
```

```cpp
ListNode *detectCycle(ListNode *head) {
    if (head == NULL)
        return NULL;
    ListNode *slow = head;   // ★
    ListNode *fast = head;
    bool cycle = false;                     // ★ flag: 1 node pe bina cycle bhi slow==fast hota
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            cycle = true;
            break;                          // break: fast meet point pe hi ruke
        }
    }
    if (!cycle)                             // flag se decide, `slow != fast` se NAHI:
        return NULL;                        //   1 node pe loop chalta hi nahi, dono head pe = galat head
    slow = head;                            // sirf slow head pe, fast meet point pe
    while (slow != fast) {                  // PHASE-2: dono 1-1 kadam (meet aur head se entry barabar)
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}                                           // detect cycle + find dup + cycle II = teeno wahi Floyd
```

### ▸ PALINDROME LL (LC-234)

```
Linked list ka head diya hai. Agar list aage se aur peeche se ek jaisi padhi jaati hai to true, warna false.
  e.g. 1->2->2->1 -> true · 1->2->3 -> false
  kyun: 1,2,2,1 ulta bhi 1,2,2,1 · 1,2,3 ulta 3,2,1 alag

COMBO: middle -> 2nd half REVERSE -> head (aage se) & rev (peeche se) saath compare
middleNode = upar wala MIDDLE OF LIST · reverseList = neeche wala REVERSE LINKED LIST
```

```
TASVEER (purani sheet se, same to same):

COMBO (3 tool jodo): middle -> 2nd half REVERSE -> head(front) & rev(back) saath chala ke compare.
   while (head && rev) { if (head->val != rev->val) return false; head=head->next; rev=rev->next; }   // && : even me rev wala half chhota, jo pehle NULL ho wahin ruko
   return true;   (khali list -> true)
list TOOT-ti NAHI: pehle-half node ka ->next kabhi nahi badla -> odd me middle node SHARED (head-se aur rev-se dono beech pe aa ke NULL milte -> while(head&&rev) bina cut ke odd+even dono handle).
```

```cpp
bool isPalindrome(Node *head) {
    Node *mid = middleNode(head);           // slow / fast
    Node *rev = reverseList(mid);           // ★ 2nd half ulta (rev = naya head)
    while (head != NULL && rev != NULL) {   // && : even me rev half chhota, jo pehle NULL wahin ruko
        if (head->val == rev->val) {
            head = head->next;
            rev = rev->next;
        }
        else
            return false;
    }
    return true;                            // khaali list -> true
}   // list TOOTI nahi: pehle half ka ->next nahi badla, odd me middle SHARED -> bina cut odd + even dono
```

### ▸ REMOVE NTH FROM END

```
Linked list ka head aur ek number n diya hai. List ke end se gina hua n-va node hatao aur naya head lautao.
  e.g. 1->2->3->4->5, n=2 -> 1->2->3->5
  kyun: end se gino 5 (1st), 4 (2nd) -> 4 hata
```

```
TASVEER (purani sheet se, same to same):

FAST/SLOW GAP: pehle fast ko n kadam aage, phir dono saath.
Node *slow = head, *fast = head;                     // dummy nahi, dono head se
    slow = slow->next; fast = fast->next;            // dono saath, gap wahi

PEHLI SHEET (Aug) se bhi, same to same:

FAST/SLOW GAP: fast ko PEHLE aage bhejo (loop i=1; i<=n). ab fast & slow me n ka gap.
agar fast==NULL -> head hi hatana hai -> return head->next.
warna dono SAATH chalao (gap constant) jab tak fast->next NULL -> slow "hatane wale ke PEHLE" baith jaata.
slow->next us node ko point kar raha -> slow->next = slow->next->next. done.
```

```cpp
Node *removeNthFromEnd(Node *head, int n) { // FAST/SLOW GAP: pehle fast n kadam aage, phir dono saath
    Node *slow = head;                      // dummy nahi, dono head se
    Node *fast = head;
    for (int i = 1; i <= n; i++)
        fast = fast->next;                  // fast n kadam aage -> dono me n ka gap
    if (fast == NULL)
        return head->next;                  // ★ fast list se bahar = head hi hatana hai
    while (fast->next != NULL) {            // ★ fast AAKHRI node pe ruke -> slow hatane wale ke PEHLE
        slow = slow->next;                  // dono saath, gap wahi
        fast = fast->next;
    }
    slow->next = slow->next->next;          // while ke BAHAR: loop khatam, ek hi baar hatao
    return head;                            // head nahi hata
}
```

### ▸ REMOVE ELEMENTS (delete by val, LC-203)

```
Linked list ka head aur ek number val diya hai. Jitne bhi node ki value val hai, sab hatao (head bhi ho sakta hai). Naya head lautao.
  e.g. 1->2->6->3->4->5->6, val=6 -> 1->2->3->4->5
  kyun: dono 6 wale node (beech wala aur aakhri) nikal gaye, baaki kram waisa hi
```

```
TASVEER (purani sheet se, same to same):

val wale SAARE node hatao.  if (!head) return null;   prev = null, curr = head;   // prev=null: head ke peeche koi node nahi
while (curr != null && head != null):   // head null = sab node hat gaye, aage kuch nahi
   curr->val==val && prev==null   -> head = head->next          (HEAD / ya sab-node-same)
   curr->val==val && prev!=null   -> prev->next = curr->next     (beech)
   else (no-match)                -> prev = curr   // sirf no-match pe: hata hua node prev nahi ban sakta
   curr = curr->next              (har case ke end me)
TRICK: prev SIRF no-match pe aage (removal pe prev=curr NAHI -- curr hata) -> warna consecutive [1,6,6,2] fail.
```

```cpp
Node *removeElements(Node *head, int val) { // val wale SAARE node hatao
    if (!head)
        return NULL;
    Node *prev = NULL;                      // prev=NULL: head ke peeche koi node nahi
    Node *curr = head;
    while (curr != NULL && head != NULL) {  // head NULL = sab node hat gaye, aage kuch nahi
        if (curr->val == val && prev == NULL)
            head = head->next;              // HEAD hata (ya saare node same val)
        else if (curr->val == val && prev != NULL)
            prev->next = curr->next;        // ★ beech ka hata; prev = curr NAHI (curr hata hua node)
        else
            prev = curr;                    // ★ prev SIRF no-match pe aage, warna [1,6,6,2] val=6 fail
        curr = curr->next;                  // har case ke end me
    }
    return head;
}
```

#### ── FAMILY: DUMMY node (nakli head + tail; naye node jodte jao; ans = dummy->next; head ka case apne aap) ──

### ▸ MERGE TWO SORTED (LC-21)

```
Do SORTED linked list di hain. Dono ko jod ke ek sorted list banao aur uska head lautao.
  e.g. a: 1->2->4, b: 1->3->4 -> 1->1->2->3->4->4
  kyun: har baar dono ke aage wale me se chhota uthao: 1,1,2,3,4,4
```

```
TASVEER (purani sheet se, same to same):

DUMMY node + tail (dummy = head ka special-case avoid).   // dummy=new Node(0); tail=dummy -> tail dummy se shuru
   while (a && b): (a->val <= b->val) ? { tail->next=a; a=a->next; } : { tail->next=b; b=b->next; }   tail=tail->next;   // && : ek list khatam hote hi ruko, baaki neeche ke loop jodte
   while (a) { tail->next=a; a=a->next; tail=tail->next; }    // baaki a jod do (already sorted)
   while (b) { tail->next=b; b=b->next; tail=tail->next; }    // baaki b jod do
   return dummy->next;   (dummy skip -> asli head)
ek list khatam -> doosri ki BAAKI seedha jod do (dono already sorted).
```

```cpp
Node *mergeTwoLists(Node *a, Node *b) {
    Node *dummy = new Node(0);              // ★ dummy: head ka special case nahi
    Node *tail = dummy;                     // tail dummy se shuru
    while (a != NULL && b != NULL) {        // && : ek list khatam hote hi ruko, baaki neeche jodte
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
            tail = tail->next;
        }
        else if (b->val < a->val) {
            tail->next = b;
            b = b->next;
            tail = tail->next;
        }
    }
    while (a != NULL) {                     // baaki a jod do (already sorted)
        tail->next = a;
        a = a->next;
        tail = tail->next;
    }
    while (b != NULL) {                     // baaki b jod do
        tail->next = b;
        b = b->next;
        tail = tail->next;
    }
    return dummy->next;                     // dummy skip -> asli head
}
```

### ▸ ADD TWO NUMBERS

```
Do linked list di hain, har node me ek digit hai aur number ulta rakha hai (ikai wala digit pehle). Dono numbers jodo aur jawab bhi isi ulte tareeke se list me lautao.
  e.g. 2->4->3 + 5->6->4 -> 7->0->8
  kyun: 342 + 465 = 807 · 2+5=7, 4+6=10 (0, carry 1), 3+4+1=8

number list me ULTA (units digit HEAD pe) -> head se head jodo = school wala right-to-left add
(2->4->3) + (5->6->4) = 342 + 465 = 807 -> (7->0->8)
```

```
TASVEER (purani sheet se, same to same):

SAAR: 2 number linked-list me ULTA pade (units-digit HEAD pe). Jodo -> naya list.
      ULTA hai isiliye HEAD-se-HEAD jodo = school-wala right-to-left add (carry aage jaata).
e.g. (2->4->3) + (5->6->4) = 342 + 465 = 807 -> answer (7->0->8).

dummy + tail (MERGE jaisa) -> naye node peeche jodte jao.  + ek CARRY.
while (a || b || carry):          <- jab tak koi list bachi HAI, ya carry bacha
   sum = carry                     (pichhle step ka carry se shuru)
   if (a) { sum += a->val;  a = a->next; }     (a hai to jodo, a aage)
   if (b) { sum += b->val;  b = b->next; }     (b hai to jodo, b aage)
   tail->next = new Node(sum % 10);  tail = tail->next;   (digit jodo)
   carry = sum / 10;                                      (agle ke liye)
return dummy->next   // dummy nakli hai, asli head uske next pe

'|| carry' KYUN: 999 + 1 -> dono list khatam, par carry=1 bacha -> ek aur node (leading 1) chahiye.

PEHLI SHEET (Aug) se bhi, same to same:

dummy + tail (MERGE jaisa) + CARRY. digits ULTA store (units-digit pehle) -> seedha aage-aage jodo.
while(a || b || carry):              <- koi list bachi HAI ya carry bacha -> chalte raho
   sum = carry;                        (pichle ka carry se shuru)
   a hai to sum += a->val (a aage);  b hai to sum += b->val (b aage).
   naya node = sum % 10 -> tail se jodo, tail aage.   carry = sum / 10 (agle ke liye).
return dummy->next.
'|| carry' zaroori: 999 + 1 = 1000 -> dono list khatam par carry bacha -> ek aur node chahiye (leading '1').
```

```cpp
Node *addTwoNumbers(Node *a, Node *b) {
    Node *dummy = new Node(0);              // dummy + tail (MERGE jaisa) + ek CARRY
    Node *tail = dummy;
    int carry = 0;
    while (a != NULL || b != NULL || carry > 0) {   // ★ || carry: 999 + 1 -> list khatam, carry 1 bacha
        int sum = 0;                                 //   -> ek aur node (leading 1)
        if (a != nullptr) {                 // a hai to jodo, a aage
            sum += a->val;
            a = a->next;
        }
        if (b != nullptr) {                 // b hai to jodo, b aage
            sum += b->val;
            b = b->next;
        }
        sum += carry;                       // pichhle step ka carry
        tail->next = new Node(sum % 10);    // digit jodo
        tail = tail->next;
        carry = sum / 10;                   // agle ke liye
    }
    return dummy->next;                     // dummy nakli, asli head uske next pe
}
```

### ▸ SWAP NODES IN PAIRS  = DUMMY + 3-player rewire (order + temp-save)

```
Linked list di hai. Har do paas-paas wale node ki jodi aapas me badal do. Value nahi badalni, node ki jagah badalni hai. Naya head lautao.
  e.g. 1->2->3->4->5 -> 2->1->4->3->5
  kyun: (1,2) -> 2,1 · (3,4) -> 4,3 · 5 akela, waise hi
```

```
TASVEER (purani sheet se, same to same):

SAAR : har do adjacent NODE ki jodi swap (values nahi, NODES = pointer rewire). DUMMY-node trick.
TEMPLATE:
    Node* dummy=new Node(0); dummy->next=head;   // dummy: head bhi badlega, isliye usse pehle ek node chahiye
    Node* prev=dummy, *first=head;   // prev=dummy: pehli jodi se pehle bhi jodne wala node chahiye
    while(prev->next && first->next){        // jodi (2 node) bache tab tak (pehle prev->next, warna NULL->next)
        Node* temp=second->next;             // BACHA lo (warna next line me gum)
        prev=first;  first=temp;             // ADVANCE (prev=purana first, first=agli jodi ka pehla)
    return dummy->next;   // naya head = pehli jodi ka second, wo dummy->next pe
dummy kyu: jodi reconnect karne ko "jodi se pehle wala" node chahiye; pehli jodi ke pehle kuch nahi -> dummy.
rewire ORDER + temp-save = crux. har iteration = SIRF EK jodi (aage ki jodi ka kaam mat karo).
```

```cpp
Node *swapPairs(Node *head) {               // har do adjacent NODE ki jodi swap (values nahi, pointer)
    if (!head)
        return nullptr;                     // khaali list: swap ko kuch nahi
    Node *dummy = new Node(0);              // dummy: head bhi badlega, usse pehle ek node chahiye
    Node *prev = dummy;                     // prev = jodi se PEHLE wala (pehli jodi ke pehle = dummy)
    prev->next = head;
    Node *first = head;
    while (prev->next != nullptr && first->next != nullptr) {   // jodi (2 node) bache tab tak
        Node *second = first->next;         //   (pehle prev->next, warna NULL->next)
        Node *temp = second->next;          // ★ BACHA lo, warna agli line me gum
        second->next = first;               // 2 -> 1
        first->next = temp;                 // 1 -> baaki list
        prev->next = second;                // prev -> 2 (= is jodi ka naya head)
        prev = first;                       // ★ ADVANCE: prev = purana first
        first = temp;                       //   first = agli jodi ka pehla
    }                                       // har iteration SIRF ek jodi; rewire ORDER + temp = crux
    return dummy->next;                     // naya head = pehli jodi ka second
}
```

#### ── FAMILY: REVERSE (3-pointer: prev / curr / next se link ulta; reverse khud + palindrome ka 2nd half) ──

### ▸ REVERSE LINKED LIST

```
Linked list ka head diya hai. Poori list ulti karo aur naya head lautao.
  e.g. 1->2->3->4->5 -> 5->4->3->2->1
  kyun: har node ka teer ulta, purana aakhri (5) naya head
```

```
TASVEER (purani sheet se, same to same):

3-pointer: prev=NULL, curr=head;   // prev=NULL: purana head naya tail banega, uska next NULL
   while (curr) { Node *nextt=curr->next; curr->next=prev; prev=curr; curr=nextt; }   // nextt pehle bachao: link ulta karte hi aage ka rasta gum
   return prev;   // prev: curr NULL pe ruka, prev aakhri node = naya head
har step: nextt BACHA -> curr ka link ULTA (->prev) -> dono aage. end: prev = naya head.
```

```cpp
Node *reverseList(Node *head) {
    Node *prev = NULL;                      // prev=NULL: purana head naya tail, uska next NULL
    Node *curr = head;
    Node *nextt = head;
    while (curr != NULL) {
        nextt = curr->next;                 // ★ nextt pehle BACHAO: link ulta karte hi aage ka raasta gum
        curr->next = prev;                  // curr ka link ULTA (-> prev)
        prev = curr;                        // dono aage
        curr = nextt;
    }
    return prev;                            // ★ curr NULL pe ruka, prev = aakhri node = naya head
}
```

### ▸ REVERSE NODES IN K-GROUP (LC-25)  = REVERSE-LL (3-ptr) + RECURSION

```
Linked list aur ek number k diya hai. Har k node ke group ko ulta karo (node badalne hain, value nahi).
Aakhir me k se kam node bachein to unhe waise hi chhod do.
  e.g. 1->2->3->4->5, k=3 -> 3->2->1->4->5
  kyun: pehla group 1,2,3 -> 3,2,1 · bache 4,5 sirf 2 (< 3) -> waise hi

har k-node group ulta (NODES), k se KAM bache -> waise hi. 1->2->3->4->5, k=2 -> 2->1->4->3->5
PEHCHAN: "reverse in groups of k" -> 3-ptr + count + recurse (copy-random se ALAG: ulatna, naya nahi)
```

```
TASVEER (purani sheet se, same to same):

SAAR: har k-node group ko reverse (NODES). k se KAM bache -> waise hi.

2 TUKDE (dono pehle se aate):
   REVERSE-LL  = ek group ko 3-pointer se ulta        [engine reuse]
   RECURSION   = group reverse -> BAAKI list pe khud ko call -> stitch

VISUAL ( 1 -> 2 -> 3 -> 4 -> 5 ,  k=2 ):
   GRP1 [1,2]:  1 -> 2 -> (3 4 5)
        rev ->  2 -> 1          (prev=2 = NAYA HEAD ,  1 = TAIL)
        1(TAIL) -> reverseKGroup(3 4 5)     [RECURSE baaki pe]
   GRP2 [3,4]:  3 -> 4 -> (5)
        rev ->  4 -> 3          (3 = TAIL)
        3(TAIL) -> reverseKGroup(5)
   GRP3 [5]:    count=1 < 2 -> return 5      (leftover, waise hi)
   JODO:  2 -> 1 -> 4 -> 3 -> 5

DELTA (reverse-LL ke upar):
   (1) count-FIRST: k node GINO -> < k? return head (leftover guard).   // while(chk && cnt<k) gino; cnt!=k -> return head · head==NULL -> NULL
   (2) 3-pointer se k reverse (count<k).
   (3) head ab TAIL -> head->next = reverseKGroup(next,k) ; return prev.   // next = agle group ka head; next!=NULL tabhi recurse

crux: count-FIRST + head-BANTA-TAIL + RECURSE-stitch.

PEHCHAN: "reverse in groups of k" sunte hi -> 3-ptr + count + recurse.
         (copy-random se ALAG: yahan nodes ULATNA, naye banana NAHI.)
```

```cpp
Node *reverseKGroup(Node *head, int k) {
    if (head == NULL)
        return NULL;
    int countNode = 0;
    Node *currNode = head;
    while (currNode && countNode < k) {     // ★ (1) count-FIRST: pehle k node GINO
        currNode = currNode->next;
        countNode++;
    }
    if (countNode != k)
        return head;                        // k se kam bache (leftover) -> waise hi
    int count = 0;
    Node *prev = NULL;
    Node *curr = head;
    Node *nextt = NULL;
    while (curr && count < k) {             // (2) 3-pointer se sirf k node ulte
        nextt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextt;
        count++;
    }
    if (nextt != NULL)                      // (3) head ab is group ka TAIL -> baaki pe RECURSE + stitch
        head->next = reverseKGroup(nextt, k);   // ★ nextt = agle group ka head
    return prev;                            // prev = is group ka naya head
}
```

#### ── FAMILY: HASHMAP-AS-GLUE (map<old*, new*> = purane node ko naye se jodne wala gond; LRU me bhi) ──

### ▸ COPY LIST w/ RANDOM POINTER (LC-138) = HASHMAP old->new + 2 PASS

```
Linked list ke har node me val, next aur ek random pointer hai (kisi bhi node pe ya NULL). Iski poori nayi copy (deep copy) banao.
Copy ke random pointer copy wale node pe hi hone chahiye, purani list pe nahi.
  e.g. vals=[7,13,11,10,1], random index=[-1,0,4,2,0] -> naye node, wahi structure
  kyun: copy ke 13 ka random copy wale 7 pe (original 7 pe nahi) · 7 ka random NULL hi

2 PASS kyun: random aage bhi ho sakta, 1 pass me target abhi bana hi na ho -> pehle sab bana lo (+map)
PEHCHAN: "random pointer / deep copy" -> MAP (old -> new). T = O(n), S = O(n)
```

```
TASVEER (purani sheet se, same to same):

SAAR: har node me val+next+RANDOM (kahin bhi ya NULL). DEEP copy banao,
      copy ke random COPY-list ke andar point karein (original pe NAHI).
KYUN 2 PASS: random aage/peeche kahin bhi -> 1-pass me target abhi bana
      hi na ho. pehle SAARE new node bana lo (+map), PHIR random set.

PEHCHAN: "random pointer / deep copy" sunte hi -> MAP (old->new).
         (k-group se ALAG: yahan naye node BANANA, ulatna NAHI.)

TEMPLATE:
  curr=head; prev=NULL; newHead=NULL;   // pehle head==NULL -> return NULL · mp = unordered_map<Node*,Node*>
     Node* temp=new Node(curr->val); mp[curr]=temp;   // mp old->new: pass-2 me random ka naya node yahin se milega
     if(newHead==NULL){ newHead=temp; prev=temp; }   // pehla clone = naya head, peeche jodne ko koi nahi
     else { prev->next=temp; prev=temp; }   // pehle prev->next pe jodo, phir prev aage badhao
  // PASS 2: random set
  curr=head; currRand=newHead;   // dono list shuru se: same position pe saath chalengi
     currRand->random = (curr->random==NULL) ? NULL : mp[curr->random];   // random NULL to NULL; warna original target ka naya copy
     curr=curr->next; currRand=currRand->next;   // dono saath aage: copy aur original same position pe rahein

VISUAL ( [x]=node · -> next · ~> random ):
  ORIG:  [7] -> [13] -> [11] -> [10] -> [1]
  rand:   7~>NULL   13~>7   11~>1   10~>11   1~>7      (random kahin bhi)
  mp (clone ka pata):  7=>7'   13=>13'   11=>11'   10=>10'   1=>1'

  PASS 1 (har orig ka clone + next jodo; ~> abhi KHAALI):
  COPY:  [7']-> [13']-> [11']-> [10']-> [1']

  PASS 2 (orig se rand padho -> UPAR mp me dekho -> naya utha ke copy pe lagao):
    13' ~>  mp[ 13.rand=7 ]  = 7'         (orig-7 ka naya = 7')
    11' ~>  mp[ 11.rand=1 ]  = 1'
    10' ~>  mp[ 10.rand=11]  = 11'
     1' ~>  mp[ 1.rand=7  ]  = 7'
     7' ~>  NULL
  => saare copy-random COPY ke andar (original pe nahi) = DEEP

KEY: currRand->random = mp[curr->random] = old-random ka NAYA-copy -> DEEP.
★ SAMAJH: "13 ka random 7 pe hai" -> ye ORIGINAL batata (curr->random padho).
  mp sirf old->new BADALTA (clone-map). Source hamesha curr (original);
  currRand (copy) padhte to random abhi NULL hota -- isliye curr se lete.
T=O(n) S=O(n).  [O(1)-space alt = interleave A->A'->B->B' clone, optional.]
```

```cpp
Node *copyRandomList(Node *head) {
    if (head == NULL)
        return NULL;
    unordered_map<Node *, Node *> mp;       // ★ old -> new: pass-2 me random ka naya node yahin se
    Node *curr = head;
    Node *prev = NULL;
    Node *newHead = NULL;
    while (curr) {                          // PASS 1: clone + next + map
        Node *temp = new Node(curr->val);
        mp[curr] = temp;                    // 7 => 7'
        if (newHead == NULL) {              // pehla clone = naya head, peeche jodne ko koi nahi
            newHead = temp;
            prev = newHead;
        }
        else {
            prev->next = temp;              // pehle prev->next pe jodo, phir prev aage
            prev = temp;
        }
        curr = curr->next;
    }
    curr = head;                            // PASS 2: random set; dono list shuru se, same position
    Node *currRand = newHead;
    while (curr) {
        if (curr->random == NULL)
            currRand->random = NULL;        // random NULL to NULL
        else
            currRand->random = mp[curr->random];   // ★ ORIGINAL ka random padho (curr), uska NAYA copy
        curr = curr->next;                  //   copy (currRand) se padhte to abhi NULL hota
        currRand = currRand->next;          // dono saath aage
    }
    return newHead;
}   // O(1) space alt = interleave A->A'->B->B' clone (optional, code nahi)
```

