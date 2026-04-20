# Pattern 9: Trees

## Ye kya hai?

Bhai dekh — Family tree soch. Dada ji upar, papa-chacha neeche, tu aur bhai sabse neeche.

```
        Dada ji
       /       \
     Papa      Chacha
    /    \       \
  Tu    Bhai    Cousin
```

DSA mein **Binary Tree** — har node ke maximum 2 children: left aur right.

```
        1
       / \
      2   3
     / \   \
    4   5   6
```

- **Root** = sabse upar (1)
- **Children** = neeche wale (2, 3 are 1's children)
- **Leaves** = sabse neeche, koi child nahi (4, 5, 6)
- **NULL** = koi child nahi

## Node kaise banta hai:

```cpp
struct TreeNode {
    int val;           // data
    TreeNode* left;    // left child
    TreeNode* right;   // right child
};
```

Linked List mein `next` ek tha. Tree mein `left` aur `right` — do.

## Kyun use karte hain?

```
Array:  search O(n), insert O(n)
Tree:   search O(log n), insert O(log n)  (agar balanced ho)
```

File system, HTML DOM, database indexing — sab tree hai.

## Tree mein ghoomna — 3 Traversals

```
        1
       / \
      2   3
     / \
    4   5
```

**Inorder (Left, Root, Right):** 4, 2, 5, 1, 3
**Preorder (Root, Left, Right):** 1, 2, 4, 5, 3
**Postorder (Left, Right, Root):** 4, 5, 2, 3, 1

### Kaise yaad rakhein:

```
INorder:   Left  ROOT  Right    ← ROOT IN beech mein
PREorder:  ROOT  Left  Right    ← ROOT PRE (pehle)
POSTorder: Left  Right ROOT     ← ROOT POST (baad mein)
```

### Teeno mein code same — sirf push_back ki jagah badli:

```
Inorder:   left call → push → right call
Preorder:  push → left call → right call
Postorder: left call → right call → push
```

**BASE CASE BHOOLNA MAT:** `if(root == NULL) return;`
Bina iske NULL pe `root->left` → CRASH.

---

## Dry Run — Inorder

```
        1
       / \
      2   3
     / \
    4   5

inorder(1):
  inorder(2):
    inorder(4):
      inorder(NULL) → return
      push 4
      inorder(NULL) → return
    push 2
    inorder(5):
      inorder(NULL) → return
      push 5
      inorder(NULL) → return
  push 1
  inorder(3):
    inorder(NULL) → return
    push 3
    inorder(NULL) → return

Result: [4, 2, 5, 1, 3] ✅
```

---

## Questions Jo Kiye

| # | Question | Type | Key trick |
|---|----------|------|-----------|
| 43 | Inorder Traversal | Traversal | Left → Push → Right |
| 44 | Preorder Traversal | Traversal | Push → Left → Right |
| 45 | Postorder Traversal | Traversal | Left → Right → Push |
| 46 | Max Depth | Recursion | 1 + max(left depth, right depth). Base: NULL → 0 |
| 47 | Balanced Tree | Recursion | Har node pe depth check + children bhi balanced check |

---

## Dry Run — Max Depth (Q46)

```
        1
       / \
      2   3
     / \
    4   5

depth(1):
  left = depth(2):
    left = depth(4): 1+max(0,0) = 1
    right = depth(5): 1+max(0,0) = 1
    return 1+max(1,1) = 2
  right = depth(3): 1+max(0,0) = 1
  return 1+max(2,1) = 3 ✅
```

## Dry Run — Balanced Tree (Q47)

Ye question mein bahut struggle hua — lekin dry run se khud fix kiya.

**3 galtiyan thi:**

**Galti 1: abs() nahi lagaya**
```
leftDepth=0, rightDepth=2. 0-2 = -2. -2 <= 1? Haan → true. GALAT!
abs(0-2) = 2 > 1 → false. SAHI.
```

**Galti 2: sirf root pe check, children pe nahi**
```
       1
      / \
     2   3
    /       \
   4         5
  /           \
 6             7

Root pe: depth(left)=3, depth(right)=3. Fark=0. Balanced lagta hai.
Lekin node 2 pe: left=2, right=0. Fark=2. NOT balanced!

Fix: isBalanced(root->left) && isBalanced(root->right) bhi check karo.
```

**Galti 3: child unbalanced hai toh parent bhi unbalanced**
```
if(isBalanced(left) && isBalanced(right)) — false hua toh skip.
Lekin return true likh diya — GALAT!

Fix: else return false. Child galat → parent bhi galat.
```

**Final sahi code ka logic:**
```
isBalanced(root):
  NULL → true
  dono children balanced nahi → return false
  dono balanced → depth check. Fark > 1 → false. Warna true.
```

---

## Max Depth (Q46) — Recursion se

```
Tree ki depth = 1 + max(left ki depth, right ki depth)
Base case: NULL → 0

depth(1) = 1 + max(depth(2), depth(3))
         = 1 + max(2, 1) = 3 ✅
```

---

## Balanced Tree Check (Q47) — Bahut struggle hua, khud fix kiya

Balanced = har node pe left aur right ki depth ka fark <= 1

**3 galtiyan thi aur teeno dry run se pakdi:**

**1. abs() nahi lagaya** — 0-2 = -2, -2 <= 1 true ho gaya. abs lagao.
**2. Sirf root check kiya** — children pe bhi isBalanced call karo.
**3. Child false hai toh parent bhi false** — else return false likhna mat bhoolo.

**Final logic:**
```
isBalanced(root):
  NULL → true
  dono children balanced nahi → return false
  dono balanced → abs(leftDepth - rightDepth) > 1 → false, warna true
```

---

## Level Order Traversal / BFS (Q48) — Queue use karo

Ye DFS (recursion) se alag hai — **level by level** traverse karo.

**Real Life:** Building ke floors. Pehle ground floor ke sab rooms dekho, phir first floor ke, phir second ke...

```
        1
       / \
      2   3
     / \   \
    4   5   6

Level 0: [1]
Level 1: [2, 3]
Level 2: [4, 5, 6]
```

**Kaise kaam karta hai:**
1. Root queue mein daal
2. Jab tak queue khaali na ho:
   - Queue ka size dekh (ye is level ke elements hain)
   - Size jitne baar nikaal → har ek ke children push kar
   - Jo nikale unke values ek vector mein daal (ek level ka result)

**Dry Run:**
```
Queue: [1]

Level 0: size=1.
  1 nikala. Children 2,3 daale. Queue: [2, 3]
  Result: [1]

Level 1: size=2.
  2 nikala. Children 4,5 daale. Queue: [3, 4, 5]
  3 nikala. Child 6 daala. Queue: [4, 5, 6]
  Result: [2, 3]

Level 2: size=3.
  4 nikala. No children. Queue: [5, 6]
  5 nikala. No children. Queue: [6]
  6 nikala. No children. Queue: []
  Result: [4, 5, 6]

Queue khaali → STOP. Answer: [[1], [2,3], [4,5,6]] ✅
```

**3 galtiyan thi:**
1. **Nikala element wapas push kar diya** — q.push(curr) nahi chahiye, sirf pop karo
2. **q.size() loop mein change ho raha tha** — pehle size save karo variable mein
3. **temp mein val push nahi kiya** — curr->val push karo
4. **NULL base case** — root NULL toh seedha empty return

---

## DFS vs BFS

```
DFS (Depth First):  Recursion. Ek branch mein deep jao, phir wapas.
                    Inorder, Preorder, Postorder — ye sab DFS hain.

BFS (Breadth First): Queue. Level by level jao.
                     Level Order Traversal — ye BFS hai.
```

---

## Questions Jo Kiye

| # | Question | Type | Key trick |
|---|----------|------|-----------|
| 43 | Inorder Traversal | DFS | Left → Push → Right |
| 44 | Preorder Traversal | DFS | Push → Left → Right |
| 45 | Postorder Traversal | DFS | Left → Right → Push |
| 46 | Max Depth | DFS | 1 + max(left, right). Base: NULL→0 |
| 47 | Balanced Tree | DFS | abs(depth fark) <= 1 + children bhi balanced |
| 48 | Level Order | BFS | Queue. Size save, utne nikaal, children push |
| 49 | LCA | DFS | root==p||q → return. Dono side mile → root. Ek side → wohi. |

---

## Silly Mistakes

1. **Base case bhoolna** — NULL pe root->left → CRASH. Hamesha if(root==NULL) return.
2. **Preorder mein inorder function call** — copy paste galti. Function naam check kar.
3. **abs() nahi lagana** — negative difference bhi > 1 ho sakta hai.
4. **Sirf root check, children skip** — recursive call children pe bhi karo.
5. **Child false toh parent true return** — else return false likhna zaroori.
6. **Queue mein nikala element wapas push** — sirf pop karo, push nahi.
7. **q.size() loop mein change** — pehle save karo.
8. **temp mein val push nahi kiya** — result khaali rahega.
9. **Order galat** — inorder = Left ROOT Right, preorder = ROOT Left Right. Confuse mat ho.

---

## Ek Line Mein Yaad Rakh

> DFS = **"Recursion. Deep jao ek branch mein. Base case NULL. 3 lines ka order badlo."**
> BFS = **"Queue. Level by level. Size save, utne nikaal, children push."**

---

## Trees DONE
