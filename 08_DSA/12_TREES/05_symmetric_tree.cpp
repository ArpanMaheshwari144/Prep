// ============================================================
// SYMMETRIC TREE — (LeetCode 101, Easy)   [TREES #5]
// ============================================================
// root diya. tree apne CENTER ke around SYMMETRIC (mirror-of-itself) hai? true/false.
//
//        1                 1
//       / \               / \
//      2   2             2   2
//     / \ / \             \   \
//    3  4 4  3            3   3
//    -> true (mirror)     -> false
//
//   [1]    -> true
//   null   -> true
// ============================================================
// ---- APPROACH ----
//  ★ = SAME-TREE + ek CHANGE (recognize + adapt): tree ko apne MIRROR se compare (fold beech se -> dono half overlap).
//  helper isMirror(a, b): base wahi -> dono null=true · ek null=false · val alag=false.
//    warna CROSS recurse:  isMirror(a->left, b->right) && isMirror(a->right, b->left).   (★ CROSS = mirror)
//  call: isSymmetric(root) = isMirror(root->left, root->right).
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

bool isSameTree(TreeNode *p, TreeNode *q)
{
    // base cases
    if (p == NULL && q == NULL)
    {
        return true;
    }
    if (p == NULL && q != NULL)
    {
        return false;
    }
    if (p != NULL && q == NULL)
    {
        return false;
    }

    // value ALAG -> yahi se false (aage recurse hi nahi karna)
    if (p->val != q->val)
    {
        return false;
    }
    else if (p->val == q->val)
    {
        // value SAME -> left & right recurse karke dekho (DONO side match hone chahiye)
        return isSameTree(p->left, q->right) && isSameTree(p->right, q->left);
    }
    return false;
}

bool isSymmetric(TreeNode *root)
{
    if (!root)
        return true;
    return isSameTree(root->left, root->right);
}

int main()
{
    // symmetric: 1 / (2,2) / (3,4,4,3)
    TreeNode *a = new TreeNode(1);
    a->left = new TreeNode(2);
    a->right = new TreeNode(2);
    a->left->left = new TreeNode(3);
    a->left->right = new TreeNode(4);
    a->right->left = new TreeNode(4);
    a->right->right = new TreeNode(3);

    // NOT symmetric: 1 / (2,2) / (null,3,null,3)
    TreeNode *b = new TreeNode(1);
    b->left = new TreeNode(2);
    b->right = new TreeNode(2);
    b->left->right = new TreeNode(3);
    b->right->right = new TreeNode(3);

    TreeNode *c = new TreeNode(1);

    cout << boolalpha;
    cout << isSymmetric(a) << " (expected true)\n";
    cout << isSymmetric(b) << " (expected false)\n";
    cout << isSymmetric(c) << " (expected true)\n";
    cout << isSymmetric(nullptr) << " (expected true)\n";
    return 0;
}
