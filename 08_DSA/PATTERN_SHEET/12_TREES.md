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

 KAB: TreeNode diya · depth / height / path / same / mirror · level-wise · BST
 ★ DHYAAN: BST ka inorder = sorted
```

### ▸ 3 DFS TRAVERSALS (LC-144 / 94 / 145)

```
Binary tree ka root diya hai. Tree ke nodes ki values teen kram me lautao: preorder (node, left, right), inorder (left, node, right) aur postorder (left, right, node).
  e.g. [1,2,3,4,5] -> pre 1 2 4 5 3 · in 4 2 5 1 3 · post 4 5 2 3 1
  kyun: pre = root pehle (1, phir 2 ka subtree 2 4 5, phir 3); in = left-root-right; post = root sabse baad

preorder: PRE = VISIT -> left -> right (root pehle)
```

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

#### ── FAMILY: recurse + COMBINE (answer = return-value; jo combine karo wahi return) ──

### ▸ MAX DEPTH (LC-104)

```
Binary tree ka root diya hai. Root se sabse door wale leaf tak raste me kitne node aate hain, wo lautao. Khaali tree pe 0.
  e.g. [3,9,20,null,null,15,7] -> 3
  kyun: sabse lamba rasta 3 -> 20 -> 15 (ya 7) = 3 node
```

```
PURANI SHEET SE (same to same):

base : null -> 0.
comb : return 1 + max(leftDepth, rightDepth).   (khud=1 + gehri branch)
```

```cpp
int maxDepth(TreeNode *root) {
    if (!root)
        return 0;                           // base: null -> 0
    int left = maxDepth(root->left);
    int right = maxDepth(root->right);
    return 1 + max(left, right);            // ★ khud = 1 + gehri branch
}
```

### ▸ INVERT TREE (LC-226)

```
Binary tree ka root diya hai. Har node ke left aur right bachche aapas me badal do (tree ka aaina), aur root lautao.
  e.g. [4,2,7,1,3,6,9] -> [4,7,2,9,6,3,1]
  kyun: 4 ke bachche 2,7 -> 7,2; 2 ke 1,3 -> 3,1; 7 ke 6,9 -> 9,6
```

```
PURANI SHEET SE (same to same):

base : null -> null.
comb : recurse dono (left, right) -> phir children SWAP -> return root.
TRAP : swap POINTERS (root->left <-> root->right), NA values.
       pointer-swap = poori BRANCH side badalti · value-swap = sirf 2 number + leaf pe null->val CRASH. order FREE.
```

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

### ▸ SAME TREE (LC-100)

```
Do binary tree p aur q diye hain. Kya dono ka dhaancha aur har node ki value bilkul ek jaisi hai? true ya false lautao.
  e.g. p=[1,2], q=[1,null,2] -> false
  kyun: values same (1,2) par p me 2 LEFT child, q me RIGHT child; structure alag
```

```
PURANI SHEET SE (same to same):

base : dono null->true · ek null->false · val alag->false.   // null checks PEHLE: warna null->val crash
comb : isSame(p->left, q->left) && isSame(p->right, q->right).
COMBINE = && (dono side match ho tabhi true).   (clean base: if(!p||!q) return p==q;)
```

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

### ▸ SYMMETRIC TREE (LC-101)  = SAME-TREE + CROSS

```
Binary tree ka root diya hai. Kya tree beech se aaine jaisa hai, matlab left hissa right hisse ka ulta roop hai? true ya false lautao.
  e.g. [1,2,2,3,4,4,3] -> true
  kyun: left 2 ke bachche 3,4 aur right 2 ke 4,3; bahar-bahar 3=3, andar-andar 4=4

isSameTree: same tree ka hi base (dono null / ek null / val alag)
```

```
PURANI SHEET SE (same to same):

base : dono null->true · ek null->false · val alag->false.   // null checks PEHLE: warna null->val crash
comb : CROSS -> compare(p->left, q->right) && compare(p->right, q->left).   CROSS = mirror (yehi ek change).
call : isSymmetric(root) = root null?true : compare(root->left, root->right).   // root khud nahi, uske dono bachche aapas me mirror hone chahiye
```

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

#### ── FAMILY: return-ONE + track-ANOTHER (parent ko HEIGHT return, asli ANSWER global / ref me; post-order) ──

### ▸ DIAMETER (LC-543)  = MAX-DEPTH + 1 line

```
Binary tree diya hai. Kinhi bhi do node ke beech sabse lamba rasta kitna hai, wo edges ki ginti me lautao. Rasta root se guzre, ye zaroori nahi.
  e.g. [1,2,3,4,5] -> 3
  kyun: path 4 -> 2 -> 1 -> 3 = 3 edge; isse lamba koi rasta nahi

height: max-depth ka height(), maxDia by-REFERENCE
```

```
PURANI SHEET SE (same to same):

= max-depth ka height() helper (maxDia by-REFERENCE), BAS 1 line EXTRA:
   int height(root, int& maxDia):
       left = height(root->left, maxDia);   right = height(root->right, maxDia);
       maxDia = max(maxDia, left + right);   // EXTRA line: is node se guzarne wali diameter = leftH + rightH
       return 1 + max(left, right);          // HEIGHT return (parent ko yeh chahiye)
diameterOfBinaryTree: maxDia=0; height(root, maxDia); return maxDia.   // 0 se theek: diameter edge-ginti hai, kabhi minus nahi
RETURN = height (parent ko chahiye); diameter = BYPRODUCT -> global maxDia (return NAHI).
maxDia = MAX (overwrite NAHI -- test-pass != code-sahi trap).
```

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

### ▸ MAX PATH SUM (LC-124, Hard)  = DIAMETER ka SUM-version

```
Binary tree diya hai (values minus bhi). Koi bhi juda hua rasta chuno (kam se kam ek node, root se guzarna zaroori nahi). Raste ke nodes ka sabse bada jod lautao.
  e.g. [-10,9,20,null,null,15,7] -> 42
  kyun: 15 + 20 + 7 = 42; root -10 jodne se sum ghatta, isliye chhoda

skeleton HUBAHU diameter (return EK arm / record DONO arm / post-order / global max). SIRF 3 badle:
  (1) arm = height -> GAIN (sum): return me 1 ki jagah root->val
  (2) record me + root->val: junction pe node bhi ginta
  (3) max(0, ..): minus arm SKIP (diameter me sab positive the)
```

```
PURANI SHEET SE (same to same):

CONNECTION -- diameter ki HAR line ke saamne, kya badla (side-by-side):

  DIAMETER (upar)                      MAX PATH SUM
  -------------------------            -------------------------------------
  left  = height(L, maxDia)            left  = max(0, solve(L, maxi))   <- neg arm -> 0 (minus arm lene se sum ghatta) // base: null -> 0
  right = height(R, maxDia)            right = max(0, solve(R, maxi))
  maxDia = max(maxDia, left+right)     maxi  = max(maxi, left + node->val + right)  <- +node
  return 1 + max(left,right)           return node->val + max(left,right)           <- val, 1 nahi

=> skeleton HUBAHU: return-EK-arm / record-DONO-arm / post-order / global-max.
   SIRF 3 jagah badla:  (1) arm = height -> GAIN(sum)   [1 ki jagah node->val]
                        (2) record me + node->val        [junction pe node bhi ginta]
                        (3) max(0,..) neg-arm SKIP        [diameter me sab positive the]
   + maxi = INT_MIN (0 NAHI!): akela -3 bhi valid path (khaali/0 se galat).

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

### ▸ BUILD TREE from PREORDER + INORDER (LC-105)

```
Ek binary tree ka preorder aur inorder kram diya hai (saari values alag). Inse wapas wahi tree banao aur root lautao.
  e.g. pre=[3,9,20,15,7], in=[9,3,15,20,7] -> [3,9,20,null,null,15,7]
  kyun: pre[0]=3 root; inorder me 3 ke baaye [9] = left, daaye [15,20,7] = right; right ka root 20 (agla pre)

preorder[rootIndex] = ROOT · inorder me us root pe todo -> LEFT-part | root | RIGHT-part, recurse
```

```
PURANI SHEET SE (same to same):

preorder[0] = ROOT · inorder me us root pe todo -> LEFT-part | root | RIGHT-part. recurse.

TEMPLATE  (MAP wala, O(n))

    buildTree:  mp[inorder[i]] = i  (ek baar) ;  rootIndex = 0 ;  return solve(0, n-1)   // rootIndex 0: preorder ka pehla = root; 0..n-1 = poora inorder

    solve(int &rootIndex, l, r):
        pivot = mp[preorder[rootIndex]];   rootIndex++;      // inorder me root kahan; ++ turant: agla preorder elem agla root
        node->left  = solve(l, pivot-1);                     // LEFT pehle (preorder me root ke baad LEFT wale node aate)
        node->right = solve(pivot+1, r);   // RIGHT baad me: preorder me left ke saare node pehle aate
        return node;

★ rootIndex by-REFERENCE (&): ek moving pointer SAB calls me shared -> LEFT poora consume hone ke
   BAAD hi RIGHT ko sahi agla-root milta (= preorder ka Root,Left,Right order). value-pass -> copy -> galat tree.

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
```

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

### ▸ BALANCED TREE (LC-110)  = MAX-DEPTH + (-1 SENTINEL)

```
Binary tree diya hai. Kya har node pe uske left aur right subtree ki unchai ka farak 1 ya usse kam hai? true ya false lautao.
  e.g. [1,2,2,3,3,null,null,4,4] -> false
  kyun: root pe left height 3 (2 -> 3 -> 4), right height 1; farak 2 > 1

HAR node pe abs(leftH - rightH) <= 1? alag "balanced?" pass nahi -> height nikaalte-nikaalte HI check
maxDepth: return = HEIGHT, ya -1 (neeche kahin imbalance)
```

```
PURANI SHEET SE (same to same):

trick: alag "balanced?" pass NAHI -> height nikaalte-nikaalte HI check -> O(n) single-pass. maxDepth return = HEIGHT ya -1 (imbalance-flag).
   int maxDepth(root):
       left  = maxDepth(root->left);    if (left == -1)  return -1;   // neeche imbalance -> UPAR propagate
       right = maxDepth(root->right);   if (right == -1) return -1;   // right me imbalance -> turant -1 upar bhejo
       return 1 + max(left, right);                                   // balanced -> asli HEIGHT
   isBalanced = (maxDepth(root) != -1).   // -1 nahi aaya = kahin bhi imbalance nahi
-1 = SENTINEL: imbalance neeche mila -> UPAR propagate -> ek node bhi galat = poora false.
TRAP: check HAR node pe (sirf root nahi) -- root balanced par ANDAR imbalanced ho sakta [test-pass != code-sahi].
```

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

#### ── FAMILY: search + BUBBLE-UP (jise dhundh rahe wo mila -> UPAR return; DONO taraf se mila = answer) ──

### ▸ LCA -- Lowest Common Ancestor (LC-236)

```
Binary tree ka root aur do node p, q diye hain. Wo sabse neeche wala node lautao jiske neeche p aur q dono aate hain. Node khud bhi p ya q ho sakta hai.
  e.g. [3,5,1,6,2,0,8,null,null,7,4], p=5, q=4 -> 5
  kyun: 4 to 5 ke subtree me hai (5 -> 2 -> 4); 5 khud p hai, to 5 hi sabse gehra common
```

```
PURANI SHEET SE (same to same):

idea : wo node jahan p, q ALAG side split ho jaate (ek left, ek right).
   TreeNode* lca(root, p, q):
       if (!root) return NULL;   // khaali subtree -> p/q yahan nahi mile
       left  = lca(root->left, p, q);
       right = lca(root->right, p, q);
       if (!left)  return right;                    // ek side khali -> doosri return
       if (!right) return left;   // right khali -> dono left me, left wala hi jawab
       return root;                                 // DONO non-null -> yehi split-point = LCA
mechanic: found-node UPAR bubble; jahan DONO taraf se kuch mila = LCA. (if(!left) return right dono case sambhaalta: left-khali+right-mila, aur dono-khali->NULL.)
```

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

#### ── FAMILY: carry-value DOWN + root-to-LEAF (target ghatao, LEAF pe check, combine = OR) ──

### ▸ PATH SUM (LC-112)

```
Binary tree ka root aur ek targetSum diya hai. Kya root se kisi leaf tak aisa rasta hai jiske nodes ka jod targetSum ke barabar ho? true ya false lautao.
  e.g. [5,4,8,11,null,13,4], target=20 -> true
  kyun: 5 + 4 + 11 = 20 aur 11 leaf hai; baaki paths 26 (5,8,13) aur 17 (5,8,4)
```

```
PURANI SHEET SE (same to same):

idea : root-se-LEAF sum == target? value NEECHE carry (target me se ghatao).
base : null -> false.
leaf : (!root->left && !root->right) -> return target == root->val.   // leaf pe hi check: beech ke node pe sum mile to path adhoora
comb : hasPathSum(left, t-val) || hasPathSum(right, t-val).   // || : koi ek path target bana de to kaafi
TRAP: LEAF = !root->left && !root->right (ACTUAL bachche), NA "!left && !right" (recursion-result) -> warna non-leaf pe galat match. [test-pass != code-sahi]
```

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

#### ── FAMILY: BFS / LEVEL-ORDER (QUEUE FIFO, recursion nahi; while ka ek chakkar = ek LEVEL) ──

### ▸ LEVEL ORDER (LC-102)

```
Binary tree ka root diya hai. Har level ke nodes ko left se right, alag-alag list me lautao (list of lists).
  e.g. [3,9,20,null,null,15,7] -> [[3],[9,20],[15,7]]
  kyun: level 0 = 3, level 1 = 9,20, level 2 = 15,7
```

```
PURANI SHEET SE (same to same):

idea : har level ki alag list -> list-of-lists. queue = FIFO (pehle-aaya-pehle-process) -> level-order + L-to-R apne aap.
SKELETON (saare BFS-level Q bas isi pe):
        int sz = q.size();               // SNAPSHOT -- is level ke node (for se PEHLE pakdo)
        vector<int> level;
        for (int i = 0; i < sz; i++) {   // sirf sz baar: isi level ke node, naye push wale nahi
            node = q.front();  q.pop();
            level.push_back(node->val);
            if (node->left)  q.push(node->left);    // bachche -> AGLE level me jaayenge
            if (node->right) q.push(node->right);
        ans.push_back(level);   // level poora bhar gaya tab push (for ke bahar)
TRAP: sz ko for se PEHLE pakdo. loop me bachche push -> q.size() badalta -> warna agle level ke node is level me ghus jaate.
TEMPLATE: right-side-view (level ka LAST) · level-average · zigzag -- sab isi skeleton pe, bas "level me kya karna" badalta.
```

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

### ▸ RIGHT SIDE VIEW (LC-199)  = LEVEL-ORDER + 1 line

```
Binary tree ke daayi taraf khade ho ke upar se neeche dekho. Jo nodes dikhte hain wo lautao, matlab har level ka sabse daayi wala node.
  e.g. [1,2,3,null,5,null,4] -> [1,3,4]
  kyun: level 0 = 1; level 1 = 2,3 me right 3; level 2 = 5,4 me right 4
```

```
PURANI SHEET SE (same to same):

idea : daaye khade -> har level ka SIRF right-most node dikhta (baaki peeche chhup jaate).
LEVEL-ORDER template bilkul SAME (while + sz-snapshot + for + push L/R). FARAK sirf level ke andar 1 line:
    level-order :  har node ka kaam / sab collect
    right-view  :  if(i == sz-1) ans.push_back(curr->val);     // level ka LAST hi
KYUN i==sz-1: nodes L-to-R (FIFO) nikalte -> level ka AAKHRI = sabse RIGHT.  (left-side-view -> i==0 le lo.)
```

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

### ▸ ZIGZAG LEVEL ORDER (LC-103)  = LEVEL-ORDER + reverse

```
Level order jaisa hi, par disha har level pe badalti hai: level 0 left se right, level 1 right se left, level 2 phir left se right, aise hi.
  e.g. [3,9,20,null,null,15,7] -> [[3],[20,9],[15,7]]
  kyun: level 1 ka [9,20] ulta = [20,9]; level 2 seedha [15,7]
```

```
PURANI SHEET SE (same to same):

idea : level-order BFS jaisa, par direction ALTERNATE (0:L->R · 1:R->L · 2:L->R...).
= LEVEL-ORDER ka SAME skeleton (queue + sz-SNAPSHOT + inner-loop -> level) + ek levelCounter.
DELTA (naya = bas ye, inner-loop ke BAAD -- level HAMESHA normal L->R banao):
    if (levelCounter % 2 != 0) reverse(level);   // ODD level -> ULTA (R->L) · levelCounter=0 se: root level seedha
    levelCounter++;  ans.push_back(level);   // reverse check ke BAAD ++: warna root level ulta ho jaata
even (0,2..) = seedha · odd (1,3..) = reversed.  (alt: deque/front-insert, par reverse simplest.)
```

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

### ▸ MINIMUM DEPTH (LC-111)   = LEVEL-ORDER + EARLY-STOP

```
Binary tree ka root diya hai. Root se sabse paas wale leaf tak raste me kitne node hain, wo lautao. Leaf = jiske dono bachche null. Khaali tree pe 0.
  e.g. [3,9,20,null,null,15,7] -> 2
  kyun: 3 -> 9, aur 9 leaf hai = 2 node; 20 wali taraf 3 node lagte
```

```
PURANI SHEET SE (same to same):

idea : root se sabse PAAS wale LEAF tak depth. BFS neeche -> PEHLA leaf mila = min depth -> RUK jao (first = shortest).
TEMPLATE (level-order skeleton + EARLY-STOP):
    if(!root) return 0;   // khaali tree ki depth 0 (warna NULL push hota)
    queue<TreeNode*> q; q.push(root);  int depth=0;   // depth = ab tak poore hue level, shuru me koi nahi
        int sz=q.size();   // for se PEHLE pakdo: loop me push se size badalta
        for(int i=0;i<sz;i++){   // sirf sz baar: isi level ke node
            auto curr=q.front(); q.pop();
            if(!curr->left && !curr->right) return depth+1;  // PEHLA leaf -> turant return (aage mat dekho)
            if(curr->left)  q.push(curr->left);
            if(curr->right) q.push(curr->right);
        depth++;                                             // level POORA -> depth++ (for-loop ke BAAHAR)
depth = ab tak POORE hue level; leaf is level me mila -> return depth+1.
DFS-TRAP (isliye BFS): "1 + min(leftD, rightD)" GALAT -- NULL-child ko depth-0 maan ke skew-tree (2->3->4) pe galat deta. BFS me ye dikkat nahi.
```

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

#### ── FAMILY: BST (left < node < right; INORDER = SORTED -> "sorted / kth chahiye? -> INORDER") ──

### ▸ KTH SMALLEST IN BST (LC-230)

```
Ek BST aur ek number k diya hai. Tree ki values me k-va sabse chhota number lautao (k 1 se gina jaata hai).
  e.g. [5,3,6,2,4,null,null,1], k=3 -> 3
  kyun: inorder (sorted) = 1,2,3,4,5,6; teesra = 3

inorder: BST pe inorder = SORTED (ascending)
```

```
PURANI SHEET SE (same to same):

idea : BST inorder = SORTED -> kth smallest = inorder ka k-va element.
steps: inorder(root) -> sorted vector 'ans' -> return ans[k-1].
k-1 TRAP: k 1-based (1st smallest = k=1), vector 0-based -> index = k-1.
optimize: k-va pe EARLY-STOP (counter, k-va node pe ruko) -- collect-all bhi gate-theek.
```

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

### ▸ VALIDATE BST (LC-98)  = RANGE carry-DOWN

```
Binary tree diya hai. Kya ye sahi BST hai? Har node pe uske poore left subtree ki saari values usse chhoti aur poore right subtree ki saari values usse badi honi chahiye (barabar nahi chalega).
  e.g. [5,4,6,null,null,3,7] -> false
  kyun: 3 node 6 ke left me (3 < 6 theek), par 6 to 5 ke right me, to 3 > 5 hona chahiye tha

node ko SAARE ancestors ka rule maanna -> har node ki WINDOW (mini, maxi), neeche jaate tight hoti
═════════════════════════ ★ EDGE ═════════════════════════════════════
sirf turant child check KAAFI NAHI ([5,4,6,null,null,3,7]: 3 < 6 theek, par 3 < 5 -> galat)
```

```
PURANI SHEET SE (same to same):

TRAP : local check (sirf turant child) KAAFI NAHI -- node ko SAARE ancestors ka rule maanna.
IDEA : har node ki ek WINDOW (mini, maxi) -> node us range me ho. neeche jaate window TIGHT hoti.
TEMPLATE:
    solve(root, mini, maxi):
       null -> true   // khaali subtree valid BST hai
       (val <= mini || val >= maxi) -> false   // <= aur >=: barabar value bhi galat (BST me duplicate nahi)
       left  = solve(L, mini, val)      // LEFT  jao -> maxi = val (chhota hona)
       right = solve(R, val, maxi)      // RIGHT jao -> mini = val (bada hona)
       return left && right   // && : dono subtree valid tabhi poora valid
CALL : solve(root, LLONG_MIN, LLONG_MAX);   mini/maxi = long long.
★ EDGE: INT_MIN/INT_MAX se shuru kiya to node ki value KHUD INT_MIN ya INT_MAX ho
   (LC-98 allow karta) -> strict check (<=, >=) use galat reject kar deta. [2147483647] pe false aata, sahi true.
(alt: inorder traversal = strictly increasing? -> BST.)
(2nd tarika: inorder = strictly increasing? -> kth-smallest ka near-transfer.)

PEHLI SHEET (Aug) se bhi, same to same:

TRAP : local check (sirf turant child) KAAFI NAHI -- node ko SAARE ancestors ka rule maanna.
IDEA : har node ki ek WINDOW (mini, maxi) -> node us range me ho. neeche jaate window TIGHT hoti.
TEMPLATE:
    solve(root, mini, maxi):
       null -> true
       (val <= mini || val >= maxi) -> false
       left  = solve(L, mini, val)      // LEFT  jao -> maxi = val (chhota hona)
       right = solve(R, val, maxi)      // RIGHT jao -> mini = val (bada hona)
       return left && right
CALL : solve(root, INT_MIN, INT_MAX).
(alt: inorder traversal = strictly increasing? -> BST.)
(2nd tarika: inorder = strictly increasing? -> kth-smallest ka near-transfer.)
```

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

