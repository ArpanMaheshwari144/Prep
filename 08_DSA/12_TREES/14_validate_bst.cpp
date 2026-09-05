// ============================================================
// VALIDATE BINARY SEARCH TREE — (LeetCode 98, Medium)   [TREES #14 · BST family]
// ============================================================
// diya tree VALID BST hai? true/false.
//
// BST rule (STRICT): HAR node pe -> POORE left-subtree ke saare value < node->val
//   < POORE right-subtree ke saare value.  (sirf turant child se compare = KAAFI NAHI!)
//
//        5                       5
//       / \                     / \
//      3   7        VALID      4   6      INVALID (dekh neeche kyun)
//     / \ / \                     / \
//    2  4 6  8                   3   7
//
//   TRAP: doosra tree -> local check sab PASS (4<5, 6>5, 3<6, 7>6) PAR 3 galat hai:
//     3, node-6 ke left me hai (6<6 nahi, thik) -- PAR 6 to 5 ke RIGHT subtree me -> 5 ke right ke SAARE value > 5 hone chahiye.
//     3 < 5 -> RULE toota -> INVALID. (turant-parent check ye MISS karta.)
//
//   [2,1,3]              -> true
//   [5,4,6,null,null,3,7]-> false  (3 < 5 par 5 ke right-subtree me)
//   [1]                  -> true
//   []                   -> true
//   [2,2,2]              -> false  (equal -> strict < toota)
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

bool solve(TreeNode *root, int mini, int maxi)
{
    if (!root)
    {
        return true; // khaali subtree valid
    }

    // node apni WINDOW (mini, maxi) ke andar hona chahiye. bahar -> invalid. (<=,>= = strict, equal bhi galat)
    if (root->val <= mini || root->val >= maxi)
    {
        return false;
    }

    bool left = solve(root->left, mini, root->val);   // LEFT: node se CHHOTA hona -> naya maxi = root->val
    bool right = solve(root->right, root->val, maxi);  // RIGHT: node se BADA hona -> naya mini = root->val

    // --- DEBUG-PRINT (recursion dekhne ko; final me hata do): har node pe left/right/val/mini/maxi ---
    // cout << "left " << left << " right " << right << " rootval " << root->val << " mini " << mini << " maxi " << maxi << endl;

    return left && right; // dono side valid ho TABHI valid BST
}

bool isValidBST(TreeNode *root)
{
    if (!root)
        return true;
    return solve(root, INT_MIN, INT_MAX);
}

int main()
{
    //     5
    //    / \
    //   3   7
    //  / \ / \
    // 2  4 6  8
    TreeNode *a = new TreeNode(5);
    a->left = new TreeNode(3);
    a->right = new TreeNode(7);
    a->left->left = new TreeNode(2);
    a->left->right = new TreeNode(4);
    a->right->left = new TreeNode(6);
    a->right->right = new TreeNode(8);

    //     5
    //    / \
    //   4   6      <- INVALID trap: 3 (6 ke left) < 5, par 6 to 5 ke right subtree me
    //      / \
    //     3   7
    TreeNode *b = new TreeNode(5);
    b->left = new TreeNode(4);
    b->right = new TreeNode(6);
    b->right->left = new TreeNode(3);
    b->right->right = new TreeNode(7);

    TreeNode *c = new TreeNode(1);

    //   2
    //  / \
    // 2   2      <- equal -> strict < toota
    TreeNode *d = new TreeNode(2);
    d->left = new TreeNode(2);
    d->right = new TreeNode(2);

    cout << boolalpha;
    cout << isValidBST(a) << " (expected true)\n";
    cout << isValidBST(b) << " (expected false)  <- TRAP: local pass par 3<5 galat\n";
    cout << isValidBST(c) << " (expected true)\n";
    cout << isValidBST(d) << " (expected false)\n";
    cout << isValidBST(nullptr) << " (expected true)\n";
    return 0;
}
