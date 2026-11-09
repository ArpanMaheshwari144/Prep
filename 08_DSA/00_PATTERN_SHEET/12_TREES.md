## PATTERN 12 — TREES (recursion)

```
 BROAD IDEA: har tree-Q = RECURSION on TreeNode(val, left, right).
             3 cheez -> BASE (null pe return) · dono taraf RECURSE · COMBINE.
 ★ EDGE (max path sum): left = max(0, left) · right = max(0, right) -> "0 = wo arm li hi nahi".
         2 ke neeche -1: arm lo to 1, chhodo to 2 -> negative arm LENA nuksaan.

 ORDER: kaam recursion-RESULT pe depend kare -> recurse-PEHLE (post-order); independent -> free.

 CORE (har tree-Q bas yehi 4 line; sirf BASE + COMBINE badalte):
     solve(node) {
         if (node == NULL) return BASE;      // (1) base -> null pe ruko
         left  = solve(node->left);          // (2) node->left = NEECHE bhejo (call)
         right = solve(node->right);         // (3) left / right = jo VALUE wapas aayi (child ka jawab)
         return COMBINE(left, right);        // (4) apna jawab banao -> parent ko do (jawab neeche se upar)
     }
     // Q1 root->left (asli bachcha: recurse · LEAF check · swap) vs left (return value: COMBINE me)
     // Q1b return type: bool -> seedha call ||/&& (variable nahi) · node -> LCA
     //     · int dobara chahiye -> variable
     // Q2 COMBINE: gehra/bada -> max · dono side match -> && · koi ek path -> || · node dhundo -> node return
     //    special condition kuch Q me: path-sum -> LEAF pe check · LCA -> root == p || q
     // Q3 answer seedha bane -> RETURN · parent ko height chahiye -> height RETURN + answer GLOBAL (diameter)
     // Q4 subtree se bane (height / sum) -> UPAR return · target path pe -> value NEECHE carry (path-sum)
     // DEBUG: poora trace mat karo. (1) maano left/right mil gaya, bas "is node pe kya" socho
     //   (2) base + 2-3 node ka chhota tree haath se check (3) print: enter-print null-check ke BAAD
     //   (warna null->val crash), result variable me lo -> print + return dono

 TASVEER (FLOW -- calls NEECHE jaati, JAWAB upar BUBBLE hota):
         [A]
        /   \
      [B]   [C]
      /  \
    [D]  [E]

    solve(A)
      line2:  left  = solve(B) ──► neeche gaya
                   solve(B):  left=solve(D), right=solve(E) ─► DONO ka COMBINE ═► B ka jawab ─► A ki 'left'
      line3:  right = solve(C) ──► neeche gaya
                   solve(C):  ... ─► COMBINE ═► C ka jawab ─► A ki 'right'
      line4:  return COMBINE(left, right) ═════════════════► FINAL ANSWER

    -> har node SIRF apne 2 bachchon ka jawab leta -> apna banata -> upar deta. poora tree ek saath NAHI.
    -> null pe BASE -> phir jawab neeche se upar chadhta -> root pe final.

 KAB: TreeNode diya · depth / height / path / same / mirror · level-wise · BST
 ★ DHYAAN: BST ka inorder = sorted
```


---

### ▸ 3 DFS TRAVERSALS (LC-144 / 94 / 145)

**SAWAAL**
```
Binary tree ka root diya hai. Tree ke nodes ki values teen kram me lautao: preorder (node, left, right), inorder (left, node, right) aur postorder (left, right, node).
  e.g. [1,2,3,4,5] -> pre 1 2 4 5 3 · in 4 2 5 1 3 · post 4 5 2 3 1
  kyun: pre = root pehle (1, phir 2 ka subtree 2 4 5, phir 3); in = left-root-right; post = root sabse baad
```

> [!IMPORTANT]
> **CHAABI:** teeno ka recursion ek hi hai (`if (!root) return;` + left + right), sirf `res.push_back(root->val)` (VISIT) ki JAGAH badalti: pre me sabse pehle, in me beech, post me sabse baad.
>
> preorder: PRE = VISIT -> left -> right (root pehle)

**CODE**
```cpp
void preorder(TreeNode *root, vector<int> &res) {    // PRE = VISIT -> left -> right (root pehle)
    if (!root)
        return;
    res.push_back(root->val);               // VISIT
    preorder(root->left, res);
    preorder(root->right, res);
}

void inorder(TreeNode *root, vector<int> &res) {     // IN = left -> VISIT -> right (beech)
    if (!root)
        return;
    inorder(root->left, res);
    res.push_back(root->val);               // VISIT
    inorder(root->right, res);
}

void postorder(TreeNode *root, vector<int> &res) {   // POST = left -> right -> VISIT (baad)
    if (!root)
        return;
    postorder(root->left, res);
    postorder(root->right, res);
    res.push_back(root->val);               // VISIT
}                                           // ★ teeno SAME recursion, sirf VISIT ki JAGAH badalti
```


---

#### ── FAMILY: recurse + COMBINE (answer = return-value; jo combine karo wahi return) ──

### ▸ MAX DEPTH (LC-104)  = recurse + COMBINE ka BASE (1 + max)

**SAWAAL**
```
Binary tree ka root diya hai. Root se sabse door wale leaf tak raste me kitne node aate hain, wo lautao. Khaali tree pe 0.
  e.g. [3,9,20,null,null,15,7] -> 3
  kyun: sabse lamba rasta 3 -> 20 -> 15 (ya 7) = 3 node
```

> [!IMPORTANT]
> **CHAABI:** null pe `0`, phir khud = `1 + max(left, right)`. Ye hi recurse + COMBINE ka base saancha hai.

**CODE**
```cpp
int maxDepth(TreeNode *root) {
    if (!root)
        return 0;                           // base: null -> 0
    int left = maxDepth(root->left);
    int right = maxDepth(root->right);
    return 1 + max(left, right);            // ★ khud = 1 + gehri branch
}
```


---

### ▸ INVERT TREE (LC-226)  = recurse + swap(left, right)

**SAWAAL**
```
Binary tree ka root diya hai. Har node ke left aur right bachche aapas me badal do (tree ka aaina), aur root lautao.
  e.g. [4,2,7,1,3,6,9] -> [4,7,2,9,6,3,1]
  kyun: 4 ke bachche 2,7 -> 7,2; 2 ke 1,3 -> 3,1; 7 ke 6,9 -> 9,6
```

> [!IMPORTANT]
> **CHAABI:** dono taraf recurse (order free), phir `swap(root->left, root->right)`, aur `root` lautao.

> [!WARNING]
> POINTERS swap karo, values nahi. Value-swap me sirf 2 number badalte hain (poori branch nahi) aur leaf pe `null->val` crash.

**CODE**
```cpp
TreeNode *invertTree(TreeNode *root) {
    if (!root)
        return nullptr;                     // base: null -> null
    invertTree(root->left);                 // recurse dono (order FREE)
    invertTree(root->right);
    swap(root->left, root->right);          // ★ TRAP: POINTERS swap (poori branch badalti), values NAHI
    return root;                            //   value-swap = sirf 2 number + leaf pe null->val CRASH
}
```


---

### ▸ SAME TREE (LC-100)  = dono tree saath recurse, COMBINE = &&

**SAWAAL**
```
Do binary tree p aur q diye hain. Kya dono ka dhaancha aur har node ki value bilkul ek jaisi hai? true ya false lautao.
  e.g. p=[1,2], q=[1,null,2] -> false
  kyun: values same (1,2) par p me 2 LEFT child, q me RIGHT child; structure alag
```

> [!IMPORTANT]
> **CHAABI:** dono tree saath chalo: dono null -> `true`, ek null -> `false`, `val` alag -> `false`, warna `isSameTree(left,left) && isSameTree(right,right)`.

> [!WARNING]
> null checks PEHLE, `p->val` se pehle. Warna ek null pe `null->val` crash.

**CODE**
```cpp
bool isSameTree(TreeNode *p, TreeNode *q) {
    if (p == NULL && q == NULL)
        return true;                        // dono null -> true
    if (p == NULL && q != NULL)             // ★ null checks PEHLE: warna null->val crash
        return false;                       // ek null -> false
    if (p != NULL && q == NULL)             //   (clean base: if (!p || !q) return p == q;)
        return false;
    if (p->val != q->val)
        return false;                       // val alag -> yahin false, recurse nahi
    else if (p->val == q->val)
        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);   // && : DONO side match
    return false;
}
```


---

### ▸ SYMMETRIC TREE (LC-101)  = SAME-TREE + CROSS

**SAWAAL**
```
Binary tree ka root diya hai. Kya tree beech se aaine jaisa hai, matlab left hissa right hisse ka ulta roop hai? true ya false lautao.
  e.g. [1,2,2,3,4,4,3] -> true
  kyun: left 2 ke bachche 3,4 aur right 2 ke 4,3; bahar-bahar 3=3, andar-andar 4=4
```

> [!IMPORTANT]
> **CHAABI:** same tree ka hi code, bas CROSS: `isSameTree(p->left, q->right) && isSameTree(p->right, q->left)`. Root pe call `isSameTree(root->left, root->right)`, root khud nahi.
>
> isSameTree: same tree ka hi base (dono null / ek null / val alag)

> [!WARNING]
> null checks yahan bhi PEHLE (dono null / ek null), warna `null->val` crash.

**CODE**
```cpp
bool isSameTree(TreeNode *p, TreeNode *q) { // same tree ka hi base (dono null / ek null / val alag)
    if (p == NULL && q == NULL)
        return true;
    if (p == NULL && q != NULL)             // null checks PEHLE: warna null->val crash
        return false;
    if (p != NULL && q == NULL)
        return false;
    if (p->val != q->val)
        return false;
    else if (p->val == q->val)
        return isSameTree(p->left, q->right) && isSameTree(p->right, q->left);   // ★ CROSS = mirror
    return false;
}

bool isSymmetric(TreeNode *root) {
    if (!root)
        return true;
    return isSameTree(root->left, root->right);   // root khud nahi, uske dono bachche aapas me mirror
}
```


---

#### ── FAMILY: return-ONE + track-ANOTHER (parent ko HEIGHT return, asli ANSWER global / ref me; post-order) ──

### ▸ DIAMETER (LC-543)  = MAX-DEPTH + 1 line

**SAWAAL**
```
Binary tree diya hai. Kinhi bhi do node ke beech sabse lamba rasta kitna hai, wo edges ki ginti me lautao. Rasta root se guzre, ye zaroori nahi.
  e.g. [1,2,3,4,5] -> 3
  kyun: path 4 -> 2 -> 1 -> 3 = 3 edge; isse lamba koi rasta nahi
```

> [!IMPORTANT]
> **CHAABI:** max-depth ka height() hi, recurse ke BAAD ek EXTRA line `maxDia = max(maxDia, left + right)`. Return HEIGHT (`1 + max`), diameter by-reference me.
>
> height: max-depth ka height(), maxDia by-REFERENCE

> [!WARNING]
> `max` lo, overwrite nahi: `maxDia = left + right` likha to test pass ho sakta par galat. Aur function HEIGHT return karta hai, diameter nahi.

**CODE**
```cpp
int height(TreeNode *root, int &maxDia) {   // max-depth ka height(), maxDia by-REFERENCE
    if (!root)
        return 0;                           // null ki height 0: uske neeche koi edge nahi
    int left = height(root->left, maxDia);  // recurse PEHLE: kaam peeche aate hue (post-order)
    int right = height(root->right, maxDia);
    maxDia = max(maxDia, left + right);     // ★ EXTRA line: is node se guzarti diameter = leftH + rightH
                                            //   MAX (overwrite NAHI: test pass != code sahi)
    return 1 + max(left, right);            // ★ HEIGHT return (parent ko yahi chahiye)
}

int diameterOfBinaryTree(TreeNode *root) {
    int maxDia = 0;                         // 0 se theek: diameter edge-ginti hai, kabhi minus nahi
    height(root, maxDia);
    return maxDia;                          // diameter = BYPRODUCT -> global maxDia (return nahi)
}
```


---

### ▸ MAX PATH SUM (LC-124, Hard)  = DIAMETER ka SUM-version

**SAWAAL**
```
Binary tree diya hai (values minus bhi). Koi bhi juda hua rasta chuno (kam se kam ek node, root se guzarna zaroori nahi). Raste ke nodes ka sabse bada jod lautao.
  e.g. [-10,9,20,null,null,15,7] -> 42
  kyun: 15 + 20 + 7 = 42; root -10 jodne se sum ghatta, isliye chhoda
```

**SAAR**
```
skeleton HUBAHU diameter (return EK arm / record DONO arm / post-order / global max). SIRF 3 badle:
  (1) arm = height -> GAIN (sum): return me 1 ki jagah root->val
  (2) record me + root->val: junction pe node bhi ginta
  (3) max(0, ..): minus arm SKIP (diameter me sab positive the)
```

```
DIAMETER (upar)                      MAX PATH SUM
-------------------------            -------------------------------------
left  = height(L, maxDia)            left  = max(0, solve(L, maxi))   <- neg arm -> 0 (minus arm lene se sum ghatta) // base: null -> 0
right = height(R, maxDia)            right = max(0, solve(R, maxi))
maxDia = max(maxDia, left+right)     maxi  = max(maxi, left + node->val + right)  <- +node
return 1 + max(left,right)           return node->val + max(left,right)           <- val, 1 nahi

VISUAL (junction):
         node
        /    \
    Lgain    Rgain      parent ko : node + max(L,R)   [EK sadak hi upar ja sakti]
                        answer me : L + node + R      [junction pe DONO sadak, upar nahi]

DRY-RUN:
           -10
          /   \
         9     20
              /  \
            15    7
    @15 -> return 15   |   @7 -> return 7
    @20 : maxi = 15 + 20 + 7 = 42 ;   return 20 + max(15,7) = 35
    @9  -> return 9
    @-10: maxi = max(42, 9 + (-10) + 35) = 42 ;   return -10 + max(9,35) = 25
```

> [!IMPORTANT]
> **CHAABI:** diameter ka skeleton, 3 badlaav: arm `max(0, child)` (minus arm = 0), record `root->val + left + right`, return `root->val + max(left, right)`.

> [!WARNING]
> `maxSum = INT_MIN` se shuru, `0` nahi: akela `-3` bhi valid path hai. Aur `max(0, ..)` bhoola to minus arm lene se sum ghat jaata.

**CODE**
```cpp
int maxDepth(TreeNode *root, int &maxSum) {
    if (!root)
        return 0;                           // base: null -> 0
    int left = max(0, maxDepth(root->left, maxSum));     // ★ (3) minus arm -> 0 (lene se sum ghatta)
    int right = max(0, maxDepth(root->right, maxSum));
    maxSum = max(maxSum, root->val + left + right);      // ★ (2) answer: junction pe DONO sadak + node
    return root->val + max(left, right);    // ★ (1) parent ko: node + EK sadak (upar ek hi ja sakti)
}

int maxPathSum(TreeNode *root) {
    int maxSum = INT_MIN;                   // ★ INT_MIN (0 NAHI): akela -3 bhi valid path
    maxDepth(root, maxSum);
    return maxSum;                          // [-10,9,20,_,_,15,7]: @20 -> 15+20+7 = 42 (answer), upar 35
}
```


---

### ▸ BUILD TREE from PREORDER + INORDER (LC-105)  = preorder se ROOT, inorder me todo (map), recurse

**SAWAAL**
```
Ek binary tree ka preorder aur inorder kram diya hai (saari values alag). Inse wapas wahi tree banao aur root lautao.
  e.g. pre=[3,9,20,15,7], in=[9,3,15,20,7] -> [3,9,20,null,null,15,7]
  kyun: pre[0]=3 root; inorder me 3 ke baaye [9] = left, daaye [15,20,7] = right; right ka root 20 (agla pre)
```

**SAAR**
```
VISUAL:   preorder  [3] 9 20 15 7        inorder  9 | 3 | 15 20 7
                   ^root                           L  root   R
        -> tree:       3
                      / \
                     9   20
                        /  \
                      15    7
DRY-RUN (rootIndex aage badhta, pivot = inorder me root):
  solve(0,4): root=3,  pivot=1 | L=solve(0,0)   R=solve(2,4)
    solve(0,0): root=9,  pivot=0 | NULL, NULL   (leaf 9)
    solve(2,4): root=20, pivot=3 | L=solve(2,2)=15   R=solve(4,4)=7

BF (linear scan)  O(n^2)                    OPTIMIZE (map)  O(n) -- sirf jaha badla
-----------------------------------------   -----------------------------------------
solve(int& rootIndex, l, r):
  int pivot = l;                            int pivot = mp[preorder[rootIndex]];
  while (inorder[pivot]                       // while-scan HATA -> map O(1)
        != preorder[rootIndex]) pivot++;
buildTree: rootIndex=0; solve(0,n-1);       buildTree: + mp{inorder-val->idx} ek baar
```

> [!IMPORTANT]
> **CHAABI:** `preorder[rootIndex]` = root, map se inorder me pivot, `rootIndex++`, phir LEFT pehle (`l..pivot-1`) aur RIGHT (`pivot+1..r`) recurse.
>
> preorder[rootIndex] = ROOT · inorder me us root pe todo -> LEFT-part | root | RIGHT-part, recurse

> [!WARNING]
> `rootIndex` by-REFERENCE (`int &`): value-pass me copy jaati, RIGHT ko galat root milta. Base `l > r` (`>=` nahi: `l == r` pe ek node banana hai).

**CODE**
```cpp
TreeNode *solve(int &rootIndex, vector<int> &preorder, vector<int> &inorder, int l, int r,
                unordered_map<int, int> &mp) {   // &rootIndex: ek pointer SAB calls me shared (neeche)
    if (l > r)
        return NULL;                        // > (>= nahi): l==r pe ek node bacha, wo banana hai
    int pivot = mp[preorder[rootIndex]];    // inorder me root kahan (map = O(1))
    rootIndex++;                            // ★ ++ turant: agla preorder element = agla root
    TreeNode *newNode = new TreeNode(inorder[pivot]);
    newNode->left = solve(rootIndex, preorder, inorder, l, pivot - 1, mp);    // ★ LEFT pehle: preorder
    newNode->right = solve(rootIndex, preorder, inorder, pivot + 1, r, mp);   //   = Root, Left, Right
    return newNode;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
    int rootIndex = 0;                      // 0: preorder ka pehla = root
    unordered_map<int, int> mp;
    for (int i = 0; i < inorder.size(); i++)
        mp[inorder[i]] = i;                 // value -> inorder index (ek baar)
    return solve(rootIndex, preorder, inorder, 0, inorder.size() - 1, mp);   // 0..n-1 = poora inorder
}
// rootIndex by-REFERENCE (&): LEFT poora consume hone ke BAAD hi RIGHT ko sahi agla root milta.
//   value-pass -> copy -> galat tree. pre [3,9,20,15,7] in [9,3,15,20,7] -> 3 (9) (20 (15) (7))
```


---

### ▸ BALANCED TREE (LC-110)  = MAX-DEPTH + (-1 SENTINEL)

**SAWAAL**
```
Binary tree diya hai. Kya har node pe uske left aur right subtree ki unchai ka farak 1 ya usse kam hai? true ya false lautao.
  e.g. [1,2,2,3,3,null,null,4,4] -> false
  kyun: root pe left height 3 (2 -> 3 -> 4), right height 1; farak 2 > 1
```

> [!IMPORTANT]
> **CHAABI:** maxDepth hi, par return = HEIGHT ya `-1` (sentinel). `abs(left - right) > 1` hua to `-1`, warna `1 + max(left, right)`.
>
> HAR node pe abs(leftH - rightH) <= 1? alag "balanced?" pass nahi -> height nikaalte-nikaalte HI check
> maxDepth: return = HEIGHT, ya -1 (neeche kahin imbalance)

> [!WARNING]
> left se `-1` aaya to TURANT `-1` upar bhejo, right mat dekho. Check HAR node pe (sirf root nahi: root balanced par andar imbalanced ho sakta).

**CODE**
```cpp
int maxDepth(TreeNode *root) {              // return = HEIGHT, ya -1 (neeche kahin imbalance)
    if (!root)
        return 0;                           // null ki height 0 (balanced hai, -1 nahi)
    int left = maxDepth(root->left);
    if (left == -1)
        return -1;                          // ★ left me imbalance -> -1 UPAR bhejo, aage mat dekho
    int right = maxDepth(root->right);
    if (right == -1)
        return -1;                          // right me imbalance -> turant -1 upar
    if (abs(left - right) > 1)
        return -1;                          // IS node pe imbalance (har node pe check, sirf root nahi:
                                            //   root balanced par andar imbalanced ho sakta)
    int result = 1 + max(left, right);      // balanced -> asli HEIGHT
    return result;
}

bool isBalanced(TreeNode *root) {
    if (!root)
        return true;                        // redundant (maxDepth(null)=0 != -1), saaf padhne ko
    return maxDepth(root) != -1 ? true : false;   // -1 nahi aaya = kahin imbalance nahi. O(n) ek pass
}
```


---

#### ── FAMILY: search + BUBBLE-UP (jise dhundh rahe wo mila -> UPAR return; DONO taraf se mila = answer) ──

### ▸ LCA -- Lowest Common Ancestor (LC-236)  = search + BUBBLE-UP (dono taraf mila = answer)

**SAWAAL**
```
Binary tree ka root aur do node p, q diye hain. Wo sabse neeche wala node lautao jiske neeche p aur q dono aate hain. Node khud bhi p ya q ho sakta hai.
  e.g. [3,5,1,6,2,0,8,null,null,7,4], p=5, q=4 -> 5
  kyun: 4 to 5 ke subtree me hai (5 -> 2 -> 4); 5 khud p hai, to 5 hi sabse gehra common
```

> [!IMPORTANT]
> **CHAABI:** `root == p || root == q` -> root UPAR bhejo; dono taraf recurse; ek side `NULL` -> doosri return; dono non-null -> yahi root = LCA.

> [!WARNING]
> `if (left == NULL) return right;` me `&& right != NULL` mat jodo. Dono khali pe root return hota = GALAT; `right` return se dono-khali pe `NULL` apne aap sahi.

**CODE**
```cpp
TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {   // p, q ALAG side split
    if (!root)
        return NULL;                        // khaali subtree -> p/q yahan nahi
    if (root == p || root == q)
        return root;                        // ★ mil gaya -> UPAR bhejo
    TreeNode *left = lowestCommonAncestor(root->left, p, q);
    TreeNode *right = lowestCommonAncestor(root->right, p, q);
    if (left == NULL)                       // ek side khali -> doosri return. DO case ek saath:
        return right;                       //   left khali + right mila · dono khali -> NULL (sahi)
                                            //   "&& right != NULL" jodo to dono-khali pe root = GALAT
    else if (right == NULL)
        return left;                        // right khali -> dono left me, left wala hi jawab
    else if (left == p && right == q)
        return root;
    else if (left == q && right == p)
        return root;
    return root;                            // ★ DONO non-null -> yahi split point = LCA
}
```


---

#### ── FAMILY: carry-value DOWN + root-to-LEAF (target ghatao, LEAF pe check, combine = OR) ──

### ▸ PATH SUM (LC-112)  = target NEECHE carry, LEAF pe check, COMBINE = ||

**SAWAAL**
```
Binary tree ka root aur ek targetSum diya hai. Kya root se kisi leaf tak aisa rasta hai jiske nodes ka jod targetSum ke barabar ho? true ya false lautao.
  e.g. [5,4,8,11,null,13,4], target=20 -> true
  kyun: 5 + 4 + 11 = 20 aur 11 leaf hai; baaki paths 26 (5,8,13) aur 17 (5,8,4)
```

> [!IMPORTANT]
> **CHAABI:** target NEECHE carry: `targetSum - root->val` bhejo. LEAF pe `root->val == targetSum`, combine `||`.

> [!WARNING]
> LEAF = ACTUAL bachche (`!root->left && !root->right`), recursion-result nahi. Beech ke node pe match = path adhoora, wahan check mat karo.

**CODE**
```cpp
bool hasPathSum(TreeNode *root, int targetSum) {   // root se LEAF tak sum == target?
    if (!root)
        return false;                       // base: null -> false
    if (!root->left && !root->right)        // ★ LEAF = ACTUAL bachche (root->left), recursion-result nahi
        return root->val == targetSum;      // leaf pe hi check: beech me mile to path adhoora
    return hasPathSum(root->left, targetSum - root->val) ||    // value NEECHE carry (target ghatao)
           hasPathSum(root->right, targetSum - root->val);     // || : koi ek path bana de to kaafi
}
```


---

#### ── FAMILY: BFS / LEVEL-ORDER (QUEUE FIFO, recursion nahi; while ka ek chakkar = ek LEVEL) ──

### ▸ LEVEL ORDER (LC-102)  = BFS ka BASE (queue, ek chakkar = ek level)

**SAWAAL**
```
Binary tree ka root diya hai. Har level ke nodes ko left se right, alag-alag list me lautao (list of lists).
  e.g. [3,9,20,null,null,15,7] -> [[3],[9,20],[15,7]]
  kyun: level 0 = 3, level 1 = 9,20, level 2 = 15,7
```

> [!IMPORTANT]
> **CHAABI:** queue me root, `while (!q.empty())`: `sz = q.size()` snapshot, `sz` baar pop + `temp` me daalo + bachche push, for ke BAAD `ans.push_back(temp)`.

> [!WARNING]
> `sz` for se PEHLE pakdo: loop me push se size badalta, agle level ke node is level me ghus jaate. Aur `if (!root) return ans`, warna `NULL` push -> `->val` crash.

**CODE**
```cpp
vector<vector<int>> levelOrder(TreeNode *root) {   // har level ki alag list. FIFO -> L-to-R apne aap
    vector<vector<int>> ans;
    if (!root)
        return ans;                         // warna NULL push hota, phir node->val crash
    queue<TreeNode *> q;
    q.push(root);                           // level-0 (root) se BFS shuru
    while (!q.empty()) {
        int sz = q.size();                  // ★ SNAPSHOT for se PEHLE: loop me push se size badalta,
        vector<int> temp;                   //   warna agle level ke node is level me ghus jaate
        for (int i = 0; i < sz; i++) {      // sirf sz baar: isi level ke node
            TreeNode *curr = q.front();
            q.pop();
            temp.push_back(curr->val);
            if (curr->left != NULL)
                q.push(curr->left);         // bachche -> AGLE level me
            if (curr->right != NULL)
                q.push(curr->right);
        }
        ans.push_back(temp);                // level poora tab push (for ke bahar)
    }
    return ans;
}
// TEMPLATE: right view · level average · zigzag · min depth -> isi skeleton pe, bas "level me kya"
```


---

### ▸ RIGHT SIDE VIEW (LC-199)  = LEVEL-ORDER + 1 line

**SAWAAL**
```
Binary tree ke daayi taraf khade ho ke upar se neeche dekho. Jo nodes dikhte hain wo lautao, matlab har level ka sabse daayi wala node.
  e.g. [1,2,3,null,5,null,4] -> [1,3,4]
  kyun: level 0 = 1; level 1 = 2,3 me right 3; level 2 = 5,4 me right 4
```

> [!IMPORTANT]
> **CHAABI:** level order hi, bas ek line: `if (i == sz - 1) ans.push_back(curr->val)` (level ka aakhri = sabse RIGHT).

> [!WARNING]
> `i == sz - 1` (0-based), `sz` nahi. Left side view hota to `i == 0`.

**CODE**
```cpp
vector<int> rightSideView(TreeNode *root) { // daaye se har level ka SIRF right-most dikhta
    if (!root)
        return {};
    queue<TreeNode *> q;
    q.push(root);
    vector<int> ans;
    while (!q.empty()) {
        int sz = q.size();                  // snapshot (level-order jaisa)
        for (int i = 0; i < sz; i++) {
            TreeNode *curr = q.front();
            q.pop();
            if (i == sz - 1)                // ★ level ka LAST: FIFO me L-to-R -> aakhri = sabse RIGHT
                ans.push_back(curr->val);   //   (1-based "position sz" = 0-based i == sz-1)
                                            //   left side view: i == 0
            if (curr->left != NULL)
                q.push(curr->left);
            if (curr->right != NULL)
                q.push(curr->right);
        }
    }
    return ans;
}
```


---

### ▸ ZIGZAG LEVEL ORDER (LC-103)  = LEVEL-ORDER + reverse

**SAWAAL**
```
Level order jaisa hi, par disha har level pe badalti hai: level 0 left se right, level 1 right se left, level 2 phir left se right, aise hi.
  e.g. [3,9,20,null,null,15,7] -> [[3],[20,9],[15,7]]
  kyun: level 1 ka [9,20] ulta = [20,9]; level 2 seedha [15,7]
```

> [!IMPORTANT]
> **CHAABI:** level HAMESHA normal L->R banao, for ke BAAD `levelCounter % 2 != 0` to `reverse(temp)`.

> [!WARNING]
> reverse check ke BAAD `levelCounter++`, pehle kiya to root level ulta ho jaata. `levelCounter = 0` se shuru.

**CODE**
```cpp
vector<vector<int>> zigzagLevelOrder(TreeNode *root) {   // 0: L->R · 1: R->L · 2: L->R ...
    vector<vector<int>> ans;
    if (!root)
        return ans;
    queue<TreeNode *> q;
    q.push(root);
    int levelCounter = 0;                   // 0 se: root level seedha
    while (!q.empty()) {
        int sz = q.size();
        vector<int> temp;                   // level HAMESHA normal L->R banao
        for (int i = 0; i < sz; i++) {
            TreeNode *curr = q.front();
            q.pop();
            temp.push_back(curr->val);
            if (curr->left != NULL)
                q.push(curr->left);
            if (curr->right != NULL)
                q.push(curr->right);
        }
        if (levelCounter % 2 != 0)          // ★ DELTA (inner loop ke BAAD): ODD level -> ulta
            reverse(begin(temp), end(temp));    // (alt: deque / front insert, reverse simplest)
        levelCounter++;                     // reverse check ke BAAD ++: warna root level ulta
        ans.push_back(temp);
    }
    return ans;
}
```


---

### ▸ MINIMUM DEPTH (LC-111)   = LEVEL-ORDER + EARLY-STOP

**SAWAAL**
```
Binary tree ka root diya hai. Root se sabse paas wale leaf tak raste me kitne node hain, wo lautao. Leaf = jiske dono bachche null. Khaali tree pe 0.
  e.g. [3,9,20,null,null,15,7] -> 2
  kyun: 3 -> 9, aur 9 leaf hai = 2 node; 20 wali taraf 3 node lagte
```

> [!IMPORTANT]
> **CHAABI:** level order + EARLY-STOP: pehla leaf (`!curr->left && !curr->right`) mile to `return depth + 1`. `depth++` for ke BAHAR.

> [!WARNING]
> DFS `1 + min(leftD, rightD)` GALAT: NULL child ko depth 0 maan leta, skew tree (2->3->4) pe galat. Isliye BFS.

**CODE**
```cpp
int minDepth(TreeNode *root) {              // root se sabse PAAS wale LEAF tak
    if (!root)
        return 0;                           // khaali tree -> 0 (warna NULL push)
    queue<TreeNode *> q;
    q.push(root);
    int depth = 0;                          // ab tak POORE hue level (current = depth+1)
    while (!q.empty()) {
        int sz = q.size();                  // for se PEHLE pakdo
        for (int i = 0; i < sz; i++) {
            TreeNode *curr = q.front();
            q.pop();
            if (!curr->left && !curr->right)
                return depth + 1;           // ★ PEHLA leaf = sabse paas -> turant return
            if (curr->left != NULL)
                q.push(curr->left);
            if (curr->right != NULL)
                q.push(curr->right);
        }
        depth++;                            // level POORA -> depth++ (for ke BAHAR)
    }
    return -1;                              // yahan pahunchte nahi (root hai to leaf milega)
}
// DFS TRAP (isliye BFS): "1 + min(leftD, rightD)" GALAT -> NULL child ko depth 0 maan ke
//   skew tree (2->3->4) pe galat deta
```


---

#### ── FAMILY: BST (left < node < right; INORDER = SORTED -> "sorted / kth chahiye? -> INORDER") ──

### ▸ KTH SMALLEST IN BST (LC-230)  = BST ka INORDER = sorted list, uska k-1 index

**SAWAAL**
```
Ek BST aur ek number k diya hai. Tree ki values me k-va sabse chhota number lautao (k 1 se gina jaata hai).
  e.g. [5,3,6,2,4,null,null,1], k=3 -> 3
  kyun: inorder (sorted) = 1,2,3,4,5,6; teesra = 3
```

> [!IMPORTANT]
> **CHAABI:** BST ka inorder = SORTED vector, jawab `ans[k - 1]`.
>
> inorder: BST pe inorder = SORTED (ascending)

> [!WARNING]
> `k - 1`: k 1-based hai, vector 0-based.

**CODE**
```cpp
void inorder(TreeNode *root, vector<int> &res) {   // BST pe inorder = SORTED (ascending)
    if (!root)
        return;
    inorder(root->left, res);               // pehle poora LEFT (chhote)
    res.push_back(root->val);               // phir NODE
    inorder(root->right, res);              // phir RIGHT (bade)
}

int kthSmallest(TreeNode *root, int k) {
    vector<int> ans;
    inorder(root, ans);                     // saari values SORTED
    return ans[k - 1];                      // ★ k-1 TRAP: k 1-based, vector 0-based
}                                           // optimize: counter, k-va node pe EARLY-STOP
```


---

### ▸ VALIDATE BST (LC-98)  = RANGE carry-DOWN

**SAWAAL**
```
Binary tree diya hai. Kya ye sahi BST hai? Har node pe uske poore left subtree ki saari values usse chhoti aur poore right subtree ki saari values usse badi honi chahiye (barabar nahi chalega).
  e.g. [5,4,6,null,null,3,7] -> false
  kyun: 3 node 6 ke left me (3 < 6 theek), par 6 to 5 ke right me, to 3 > 5 hona chahiye tha
```

**SAAR**
```
node ko SAARE ancestors ka rule maanna -> har node ki WINDOW (mini, maxi), neeche jaate tight hoti
═════════════════════════ ★ EDGE ═════════════════════════════════════
sirf turant child check KAAFI NAHI ([5,4,6,null,null,3,7]: 3 < 6 theek, par 3 < 5 -> galat)
```

```
★ PURANI GALTI (Aug sheet): CALL solve(root, INT_MIN, INT_MAX) tha -> [2147483647] pe false aata, sahi true. Isliye ab long long (LLONG_MIN, LLONG_MAX).
```

> [!IMPORTANT]
> **CHAABI:** har node ki window `(mini, maxi)` neeche carry: LEFT jao -> `maxi = root->val`, RIGHT jao -> `mini = root->val`. `<= mini || >= maxi` pe false, combine `&&`.

> [!WARNING]
> `long long` + `LLONG_MIN, LLONG_MAX` se shuru. `INT_MIN / INT_MAX` liya to node khud `INT_MAX` ho (`[2147483647]`) -> strict check galat false deta.

**CODE**
```cpp
bool solve(TreeNode *root, long long mini, long long maxi) {
    if (root == NULL)
        return true;                        // khaali subtree valid
    if (root->val <= mini || root->val >= maxi)
        return false;                       // <= aur >= : barabar bhi galat (BST me duplicate nahi)
    bool left = solve(root->left, mini, root->val);    // ★ LEFT jao -> maxi = val (chhota hona)
    bool right = solve(root->right, root->val, maxi);  // RIGHT jao -> mini = val (bada hona)
    return left && right;                   // && : dono subtree valid tabhi valid
}

bool isValidBST(TreeNode *root) {
    return solve(root, LLONG_MIN, LLONG_MAX);   // ★ long long: INT_MIN / INT_MAX se shuru kiya to node
}                                           //   khud INT_MAX ho ([2147483647]) -> strict = galat false
// alt: inorder strictly increasing? -> BST (kth smallest ka near-transfer)
```


