// ============================================================
// BUILD TREE from PREORDER + INORDER — (LeetCode 105)   [TREES #17]
// ============================================================
// preorder (Root,Left,Right) aur inorder (Left,Root,Right) diye. Original binary tree BANAO.
//
//   - preorder[0] = ROOT (hamesha).
//   - inorder me us root ko dhundo -> uske BAAYE = left-subtree, DAAYE = right-subtree.
//   - recurse. (2 KEY trick: preorder ka MOVING index jo har root pe aage; inorder value->index HashMap O(1) lookup.)
//
// TEST-CASES (preorder , inorder -> tree; check pre+in traversal se verify karta):
//   [3,9,20,15,7] , [9,3,15,20,7]   -> valid tree
//   [1,2]         , [2,1]           -> 2 left-child of 1
//   [1,2]         , [1,2]           -> 2 right-child of 1
//   [1]           , [1]             -> single node
//   []            , []              -> empty (null)
//   [1,2,3,4]     , [4,3,2,1]       -> left-skewed
// ============================================================
//
// ---- APPROACH ----  (BF pehle -> phir OPTIMIZE, recursion trace ke saath)
//  preorder = ROOT,Left,Right  |  inorder = Left,ROOT,Right
//  soch: preorder ka pehla = ROOT. inorder me us root pe todo -> LEFT-part | root | RIGHT-part.
//        LEFT-part pe recurse, RIGHT-part pe recurse. har baar preorder ka AGLA = agla root.
//
//  ---- BRUTE FORCE (pehle likha) ----
//  solve(rootIndex&, l, r):        // l, r = inorder ki current range
//     1. base : l > r -> return NULL                 (range khaali -> koi node nahi)
//     2. pivot: inorder me preorder[rootIndex] kahan -> WHILE-scan
//            int pivot = l;  while (inorder[pivot] != preorder[rootIndex]) pivot++;
//     3. rootIndex++                                 (root use ho gaya -> pointer agle root pe)
//     4. node = new TreeNode(inorder[pivot])
//     5. node->left  = solve(l, pivot-1)             (inorder ka LEFT-part -> left-subtree)
//     6. node->right = solve(pivot+1, r)             (inorder ka RIGHT-part -> right-subtree)
//     7. return node
//  buildTree: rootIndex = 0 se  solve(0, n-1)  call.
//
//  ---- RECURSION KAISE CHALTI  (trace: preorder[3,9,20,15,7] , inorder[9,3,15,20,7]) ----
//   solve(0,4): root=3,  pivot=1, rootIndex->1 | left=solve(0,0)   right=solve(2,4)
//     solve(0,0): root=9,  pivot=0, rootIndex->2 | solve(0,-1)=NULL, solve(1,0)=NULL   (leaf 9)
//     solve(2,4): root=20, pivot=3, rootIndex->3 | left=solve(2,2)   right=solve(4,4)
//       solve(2,2): root=15, pivot=2, rootIndex->4 | NULL, NULL     (leaf 15)
//       solve(4,4): root=7,  pivot=4, rootIndex->5 | NULL, NULL     (leaf 7)
//   -> LEFT poora ban-ne ke BAAD hi RIGHT chalti (rootIndex tab tak aage) = Root,Left,Right order.
//
//  * rootIndex = REFERENCE (&): ek hi pointer sab calls me shared -> tabhi left-subtree poora
//    consume hone ke baad right ko sahi agla-root milta. value se pass -> copy -> galat tree.
//
//  ---- OPTIMIZE (map) ----  BF ka step-2 while-scan slow (O(n) har call -> O(n^2)).
//   inorder ka {value -> index} ek baar unordered_map me -> pivot = mp[preorder[rootIndex]]  O(1).
//   -> total O(n).  baaki poora same.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// map-REDO: pivot linear-scan HATA ke unordered_map (inorder value->index) se O(1) karna.

TreeNode *solve(int &rootIndex, vector<int> &preorder, vector<int> &inorder, int l, int r, unordered_map<int, int> &mp)
{
    if (l > r)
    {
        return NULL;
    }

    int pivot = mp[preorder[rootIndex]];
    rootIndex++;
    
    TreeNode *newNode = new TreeNode(inorder[pivot]);
    newNode->left = solve(rootIndex, preorder, inorder, l, pivot - 1, mp);
    newNode->right = solve(rootIndex, preorder, inorder, pivot + 1, r, mp);
    return newNode;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
{
    int rootIndex = 0;
    unordered_map<int, int> mp;
    for (int i = 0; i < inorder.size(); i++)
    {
        mp[inorder[i]] = i;
    }
    return solve(rootIndex, preorder, inorder, 0, inorder.size() - 1, mp);
}

// ---- verify helpers (built tree ka pre+in traversal nikaalte) ----
void pre(TreeNode *r, vector<int> &out)
{
    if (!r)
        return;
    out.push_back(r->val);
    pre(r->left, out);
    pre(r->right, out);
}
void in(TreeNode *r, vector<int> &out)
{
    if (!r)
        return;
    in(r->left, out);
    out.push_back(r->val);
    in(r->right, out);
}

void check(vector<int> preorder, vector<int> inorder)
{
    TreeNode *root = buildTree(preorder, inorder);
    vector<int> gotPre, gotIn;
    pre(root, gotPre);
    in(root, gotIn);
    bool ok = (gotPre == preorder && gotIn == inorder);
    cout << (ok ? "PASS" : "FAIL")
         << "  built-pre=";
    for (int x : gotPre)
        cout << x << " ";
    cout << " built-in=";
    for (int x : gotIn)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    check({3, 9, 20, 15, 7}, {9, 3, 15, 20, 7});
    check({1, 2}, {2, 1});
    check({1, 2}, {1, 2});
    check({1}, {1});
    check({}, {});
    check({1, 2, 3, 4}, {4, 3, 2, 1});
    return 0;
}
