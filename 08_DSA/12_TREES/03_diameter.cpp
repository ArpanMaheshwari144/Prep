// ============================================================
// DIAMETER OF BINARY TREE — (LeetCode 543, Easy)   [TREES #3]
// ============================================================
// diameter = kisi bhi DO node ke beech sabse LAMBा path ki length (EDGES me).
// path root se guzre YA na guzre. (length = edges ki ginti, nodes-1.)
//
//        1
//       / \
//      2   3        diameter = 3  (path 4-2-1-3 = 3 edges)
//     / \
//    4   5
//
//   [1,2,3,4,5]  -> 3
//   [1,2]        -> 1
//   [1]          -> 0
// ============================================================
//
// ★ MAX-DEPTH pe build (height), NAYA twist -> return HEIGHT, par global DIAMETER track:
//   -- height(node) = 1 + max(leftH, rightH);  base null -> 0.   (bilkul max-depth)
//   -- har node pe: is-node-se-guzarne-wali diameter = leftH + rightH  -> global MAX update.
//   -- helper HEIGHT return karta (parent ke liye), par ANSWER = global diameter.  [2 ALAG cheez]
//   ★ ORDER: recurse-PEHLE (post-order) -- leftH/rightH chahiye TABHI diameter nikle. (yehi "order matters".)
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

int height(TreeNode *root, int &maxDia)
{
    if (!root)
    {
        return 0;
    }

    int left = height(root->left, maxDia);
    int right = height(root->right, maxDia);

    maxDia = max(maxDia, left + right);

    return 1 + max(left, right);
}

int diameterOfBinaryTree(TreeNode *root)
{
    int maxDia = 0;
    height(root, maxDia);
    return maxDia;
}

int main()
{
    //      1
    //     / \
    //    2   3
    //   / \
    //  4   5
    TreeNode *a = new TreeNode(1);
    a->left = new TreeNode(2);
    a->right = new TreeNode(3);
    a->left->left = new TreeNode(4);
    a->left->right = new TreeNode(5);

    TreeNode *b = new TreeNode(1);
    b->left = new TreeNode(2);

    TreeNode *c = new TreeNode(1);

    // d: diameter SUBTREE me (root se NAHI guzarta) -> ye overwrite-bug ko pakadta
    //      1
    //       \
    //        2
    //       / \
    //      3   4
    //     /     \
    //    5       6      diameter = 4 (5-3-2-4-6). bug hota to 3 deta.
    TreeNode *d = new TreeNode(1);
    d->right = new TreeNode(2);
    d->right->left = new TreeNode(3);
    d->right->right = new TreeNode(4);
    d->right->left->left = new TreeNode(5);
    d->right->right->right = new TreeNode(6);

    cout << diameterOfBinaryTree(a) << " (expected 3)\n";
    cout << diameterOfBinaryTree(b) << " (expected 1)\n";
    cout << diameterOfBinaryTree(c) << " (expected 0)\n";
    cout << diameterOfBinaryTree(d) << " (expected 4)  <- ye fix PROVE karta\n";
    return 0;
}
