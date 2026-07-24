// ============================================================
// LOWEST COMMON ANCESTOR (Binary Tree) — (LeetCode 236, Medium)   [TREES #7]
// ============================================================
// root aur do node p, q diye. unka LCA lautao = wo SABSE DEEPEST node
// jiske neeche p aur q DONO aate hain (khud bhi ancestor ho sakta).
//
//            3
//          /   \
//         5     1
//        / \   / \
//       6   2 0   8
//          / \
//         7   4
//
//   LCA(5, 1) -> 3
//   LCA(5, 4) -> 5   (5 khud 4 ka ancestor)
//   LCA(6, 4) -> 5
//   LCA(7, 8) -> 3
// ============================================================
// ---- APPROACH ----
//  ★ INTUITION: LCA = wo node jahan p aur q ALAG side split ho jaate (ek left, ek right).
//  1. base: root NULL -> null.   root == p ya q -> return root (mil gaya, UPAR bhejo).
//  2. left = recurse(left) ;  right = recurse(right).
//  3. COMBINE:
//     - ek side NULL, doosri non-null -> non-null wali return (dono ek hi side me).
//     - DONO non-null -> ★ YE root hi LCA (p,q alag subtrees me split).
//  ★ mechanic = "return FOUND-node UPAR": jise mila wo upar chadhta; jahan DONO milte = LCA.
//  (clean combine: if(!left) return right;  if(!right) return left;  return root;  -- p/q-specific checks redundant.)
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

TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
{
    // base: null -> null.  root KHUD p ya q -> return root (ek mil gaya, upar bhejo)
    if (!root)
        return NULL;

    if (root == p || root == q)
    {
        return root;
    }

    // dono side dhundo
    TreeNode *left = lowestCommonAncestor(root->left, p, q);
    TreeNode *right = lowestCommonAncestor(root->right, p, q);

    // COMBINE: ek side NULL -> doosri return (dono ek hi side me). DONO non-null -> ★ root = LCA (p,q alag side split)
    if (left == NULL)
    {
        return right;
    }
    else if (right == NULL)
    {
        return left;
    }
    else if (left == p && right == q)
    {
        return root;
    }
    else if (left == q && right == p)
    {
        return root;
    }
    return root;
}

int main()
{
    //          3
    //        /   \
    //       5     1
    //      / \   / \
    //     6   2 0   8
    //        / \
    //       7   4
    TreeNode *n3 = new TreeNode(3);
    TreeNode *n5 = new TreeNode(5);
    TreeNode *n1 = new TreeNode(1);
    TreeNode *n6 = new TreeNode(6);
    TreeNode *n2 = new TreeNode(2);
    TreeNode *n0 = new TreeNode(0);
    TreeNode *n8 = new TreeNode(8);
    TreeNode *n7 = new TreeNode(7);
    TreeNode *n4 = new TreeNode(4);
    n3->left = n5;
    n3->right = n1;
    n5->left = n6;
    n5->right = n2;
    n1->left = n0;
    n1->right = n8;
    n2->left = n7;
    n2->right = n4;

    cout << lowestCommonAncestor(n3, n5, n1)->val << " (expected 3)\n";
    cout << lowestCommonAncestor(n3, n5, n4)->val << " (expected 5)\n";
    cout << lowestCommonAncestor(n3, n6, n4)->val << " (expected 5)\n";
    cout << lowestCommonAncestor(n3, n7, n8)->val << " (expected 3)\n";
    return 0;
}
