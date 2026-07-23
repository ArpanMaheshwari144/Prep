// ============================================================
// MAXIMUM DEPTH OF BINARY TREE — (LeetCode 104, Easy)   [TREES ka PEHLA]
// ============================================================
// binary tree ka root diya. MAX DEPTH = root se sabse door leaf tak
// nodes ki ginti. khali tree (null) -> 0.
//
//        3
//       / \
//      9   20        -> depth 3
//         /  \
//        15   7
//
//   diagram wala   -> 3
//   [1,null,2]     -> 2   (root->right->2)
//   []  (null)     -> 0
//   [1]            -> 1
// ============================================================
//
// ---- APPROACH ----
//  ★ tree = RECURSION (har node khud ko call karta).
//  1. BASE: root null -> return 0            (recursion yahin rukti)
//  2. left  = maxDepth(root->left)           -> left subtree ki depth
//  3. right = maxDepth(root->right)          -> right subtree ki depth
//  4. COMBINE: return 1 + max(left, right)   (1 = khud, + gehri branch)
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

int maxDepth(TreeNode *root)
{
    if (!root)
    {
        return 0;
    }

    int left = maxDepth(root->left);
    int right = maxDepth(root->right);

    return 1 + max(left, right);
}

int main()
{
    //      3
    //     / \
    //    9   20
    //       /  \
    //      15   7
    TreeNode *a = new TreeNode(3);
    a->left = new TreeNode(9);
    a->right = new TreeNode(20);
    a->right->left = new TreeNode(15);
    a->right->right = new TreeNode(7);

    TreeNode *b = new TreeNode(1);
    b->right = new TreeNode(2);

    TreeNode *d = new TreeNode(1);

    cout << maxDepth(a) << " (expected 3)\n";
    cout << maxDepth(b) << " (expected 2)\n";
    cout << maxDepth(nullptr) << " (expected 0)\n";
    cout << maxDepth(d) << " (expected 1)\n";
    return 0;
}
