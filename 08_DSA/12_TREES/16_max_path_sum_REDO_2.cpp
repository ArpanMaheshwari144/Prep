// ============================================================
// BINARY TREE MAXIMUM PATH SUM (LC 124) — REDO_2   [TREES #16]
// khud se, bina dekhe — REDO_1 / sheet mat kholo; atko tabhi peek.
// ============================================================
//
// Koi bhi PATH lo — connected chain: node -> child -> child ...
// Root se guzarna ZAROORI NAHI. Path me kam-se-kam 1 node.
// Us path ke node-values ka SUM maximum karo.
//
//        1
//       / \        best = 2 + 1 + 3 = 6
//      2   3
//
//      -10
//      /  \        best = 15 + 20 + 7 = 42   (root -10 liya hi nahi)
//     9   20
//         / \
//        15  7
//
// TEST-CASES:
//   [1,2,3]                      -> 6
//   [-10,9,20,null,null,15,7]    -> 42
//   [-3]                         -> -3    (akela node bhi valid path hai)
//   [2,-1]                       -> 2     (negative arm chhod sakte ho)
//   [-2,-1]                      -> -1    (best = akela -1)
//   [5,4,8,11,null,13,4,7,2,null,null,null,1]  -> 48
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int maxDepth(TreeNode *root, int &maxSum)
{
    if (!root)
    {
        return 0;
    }

    int left = max(0, maxDepth(root->left, maxSum));
    int right = max(0, maxDepth(root->right, maxSum));

    maxSum = max(maxSum, root->val + left + right);

    return root->val + max(left, right);
}

int maxPathSum(TreeNode *root)
{
    int maxSum = INT_MIN;
    maxDepth(root, maxSum);
    return maxSum;
}

// ------------------------------------------------------------
// TEST HARNESS
// ------------------------------------------------------------
int pass_count = 0, total = 0;

void check(TreeNode *root, int exp, const char *label)
{
    int got = maxPathSum(root);
    bool ok = (got == exp);
    pass_count += ok;
    total++;
    cout << (ok ? "PASS" : "FAIL")
         << "  " << label
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    //   1
    //  / \
    // 2   3                                    -> 6
    TreeNode *a = new TreeNode(1);
    a->left = new TreeNode(2);
    a->right = new TreeNode(3);

    //   -10
    //   /  \
    //  9   20
    //      / \
    //     15  7                                -> 42
    TreeNode *b = new TreeNode(-10);
    b->left = new TreeNode(9);
    b->right = new TreeNode(20);
    b->right->left = new TreeNode(15);
    b->right->right = new TreeNode(7);

    // akela negative node                      -> -3
    TreeNode *c = new TreeNode(-3);

    //   2
    //  /
    // -1                                       -> 2
    TreeNode *d = new TreeNode(2);
    d->left = new TreeNode(-1);

    //   -2
    //   /
    //  -1                                      -> -1
    TreeNode *e = new TreeNode(-2);
    e->left = new TreeNode(-1);

    //        5
    //       / \
    //      4   8
    //     /   / \
    //   11  13   4
    //   / \       \
    //  7   2       1                           -> 48
    TreeNode *f = new TreeNode(5);
    f->left = new TreeNode(4);
    f->right = new TreeNode(8);
    f->left->left = new TreeNode(11);
    f->left->left->left = new TreeNode(7);
    f->left->left->right = new TreeNode(2);
    f->right->left = new TreeNode(13);
    f->right->right = new TreeNode(4);
    f->right->right->right = new TreeNode(1);

    check(a, 6, "[1,2,3]                ");
    check(b, 42, "[-10,9,20,_,_,15,7]    ");
    check(c, -3, "[-3]                   ");
    check(d, 2, "[2,-1]                 ");
    check(e, -1, "[-2,-1]                ");
    check(f, 48, "[5,4,8,11,_,13,4,7,2..]");

    cout << "\n"
         << pass_count << "/" << total << " passed\n";
    return 0;
}
