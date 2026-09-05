// ============================================================
// KTH SMALLEST ELEMENT IN A BST — (LeetCode 230, Medium)   [TREES #13 · BST family]
// ============================================================
// BST diya + integer k. k-va SABSE CHHOTA element lautao (1-indexed).
//
// BST (Binary Search Tree) PROPERTY (ye YAAD rakh -- BST family ka dil):
//     har node pe -> LEFT subtree ke SAARE value < node->val < RIGHT subtree ke SAARE value.
//     (yani left me chhote, right me bade. har node pe.)
//
//        3
//       / \
//      1   4
//       \
//        2
//   k=1 -> 1  (sabse chhota)
//
//        5
//       / \
//      3   6
//     / \
//    2   4
//   /
//  1
//   k=3 -> 3
//
//   [3,1,4,null,2]  k=1 -> 1
//   [1]             k=1 -> 1
// ============================================================
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

// inorder = left -> node -> right. BST pe ye values SORTED (ascending) me deta.
void inorder(TreeNode *root, vector<int> &res)
{
    if (!root)
        return;

    inorder(root->left, res);  // pehle poora LEFT (chhote values)
    res.push_back(root->val);  // phir NODE
    inorder(root->right, res); // phir RIGHT (bade values)
}

int kthSmallest(TreeNode *root, int k)
{
    vector<int> ans;
    inorder(root, ans);    // ans = BST ki saari values, SORTED
    return ans[k - 1];     // k 1-based, vector 0-based -> index = k-1
}

int main()
{
    //     3
    //    / \
    //   1   4
    //    \
    //     2
    TreeNode *a = new TreeNode(3);
    a->left = new TreeNode(1);
    a->right = new TreeNode(4);
    a->left->right = new TreeNode(2);

    //       5
    //      / \
    //     3   6
    //    / \
    //   2   4
    //  /
    // 1
    TreeNode *b = new TreeNode(5);
    b->left = new TreeNode(3);
    b->right = new TreeNode(6);
    b->left->left = new TreeNode(2);
    b->left->right = new TreeNode(4);
    b->left->left->left = new TreeNode(1);

    TreeNode *c = new TreeNode(1);

    cout << kthSmallest(a, 1) << " (expected 1)\n";
    cout << kthSmallest(a, 3) << " (expected 3)\n";
    cout << kthSmallest(b, 3) << " (expected 3)\n";
    cout << kthSmallest(b, 1) << " (expected 1)\n";
    cout << kthSmallest(c, 1) << " (expected 1)\n";
    return 0;
}
