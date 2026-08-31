// ============================================================
// BINARY TREE MAXIMUM PATH SUM — (LeetCode 124, Hard)   [TREES #16]  -- REDO 1
// ============================================================
// Koi bhi PATH lo (connected: node -> child -> child..., root se guzre YA na guzre).
// Us path ke node-values ka SUM max karo. (path me kam-se-kam 1 node.)
//
//        1
//       / \        best path = 2 + 1 + 3 = 6
//      2   3
//
//      -10
//      /  \        best path = 15 + 20 + 7 = 42  (root -10 NAHI liya)
//     9   20
//         / \
//        15  7
//
// TEST-CASES (input tree -> expected max path sum):
//   [1,2,3]                       -> 6
//   [-10,9,20,null,null,15,7]     -> 42
//   [-3]                          -> -3   (akela node bhi valid path)
//   [2,-1]                        -> 2    (negative child chhodo)
//   [-2,-1]                       -> -1   (best = akela -1)
//
//  yaad: = DIAMETER ka SUM-version (return-EK-arm / record-DONO-arm / post-order / global-max).
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

int solve(TreeNode *root, int &maxi)
{
    if (!root)
        return 0;

    int left = solve(root->left, maxi);
    if(left < 0) left = 0;

    int right = solve(root->right, maxi);
    if(right < 0) right = 0;

    maxi = max(maxi, left + root->val + right);

    return root->val + max(left, right);
}

int maxPathSum(TreeNode *root)
{
    int maxi = INT_MIN;
    solve(root, maxi);
    return maxi;
}

void check(TreeNode *root, int exp)
{
    int got = maxPathSum(root);
    cout << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    //   1
    //  / \
    // 2   3                         -> 6
    TreeNode *a = new TreeNode(1);
    a->left = new TreeNode(2);
    a->right = new TreeNode(3);

    //   -10
    //   /  \
    //  9   20
    //      / \
    //     15  7                     -> 42
    TreeNode *b = new TreeNode(-10);
    b->left = new TreeNode(9);
    b->right = new TreeNode(20);
    b->right->left = new TreeNode(15);
    b->right->right = new TreeNode(7);

    // akela negative node               -> -3
    TreeNode *c = new TreeNode(-3);

    //   2
    //  /
    // -1                            -> 2  (negative arm chhodo)
    TreeNode *d = new TreeNode(2);
    d->left = new TreeNode(-1);

    //   -2
    //   /
    //  -1                           -> -1 (best akela -1)
    TreeNode *e = new TreeNode(-2);
    e->left = new TreeNode(-1);

    check(a, 6);
    check(b, 42);
    check(c, -3);
    check(d, 2);
    check(e, -1);
    return 0;
}
