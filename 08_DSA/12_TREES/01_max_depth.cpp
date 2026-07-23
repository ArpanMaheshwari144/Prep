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
// ★ TREES ka CORE (naya mechanic -- ek baar samajh, phir har tree-Q isi pe):
//   -- har tree problem = RECURSION. TreeNode = val + left + right (do bachche).
//   -- BASE CASE: node NULL ho -> kuch return (yahan 0). [recursion yahin rukti]
//   -- har node pe: LEFT recurse + RIGHT recurse -> phir dono ko COMBINE.
//   -- Max-Depth SOLVED: base = 0.  return 1 + max(leftDepth, rightDepth).  (khud=1 + gehri branch)
//   (bas 3 cheez: base | dono taraf recurse | combine. poora tree khel isi pe.)
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
