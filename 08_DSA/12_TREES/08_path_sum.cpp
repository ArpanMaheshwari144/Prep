// ============================================================
// PATH SUM — (LeetCode 112, Easy)   [TREES #8]
// ============================================================
// root aur ek targetSum diya. Check karo koi ROOT-se-LEAF path hai jiske
// saare nodes ka SUM = targetSum? true/false.
//
//        5
//       / \
//      4   8
//     /   / \
//    11  13  4
//   root->leaf paths ka sum:  5+4+11=20 · 5+8+13=26 · 5+8+4=17
//
//   target=20  -> true   (5->4->11)
//   target=17  -> true   (5->8->4)
//   target=100 -> false
//   [1] target=1 -> true   (root khud leaf, 1==1)
//   null target=0 -> false
// ============================================================
// ---- APPROACH ----
//  ★ NAYA angle: value UPAR return nahi -- value NEECHE carry karo (target me se ghatate jao).
//  1. base: root NULL -> false.
//  2. ★ LEAF (!root->left && !root->right) pe -> return (targetSum == root->val).  [bacha target leaf-value se match?]
//  3. warna -> hasPathSum(left, target - val) || hasPathSum(right, target - val).   COMBINE = OR (koi ek path bane).
//  ★★ TRAP (jo tune pakda): LEAF ka check = !root->left && !root->right (ACTUAL bachche), NA ki !left && !right
//     (recursion-result). warna non-leaf pe bhi target==val match ho jaata -> GALAT. (5->3, target5 = false, na true.)
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

bool hasPathSum(TreeNode *root, int targetSum)
{
    if (!root)
    {
        return false;
    }

    // ★ LEAF check: left & right DONO nahi -> mei jahan khada hu WAHI leaf hai (ACTUAL bachche dekho).
    // neeche aate hue target me se har node-value ghatate aaye -> leaf pe: bacha target == leaf-value? -> path poora.
    if (!root->left && !root->right)
        return root->val == targetSum;

    return hasPathSum(root->left, targetSum - root->val) || hasPathSum(root->right, targetSum - root->val);
}

int main()
{
    //     5
    //    / \
    //   4   8
    //  /   / \
    // 11  13  4
    TreeNode *root = new TreeNode(5);
    root->left = new TreeNode(4);
    root->right = new TreeNode(8);
    root->left->left = new TreeNode(11);
    root->right->left = new TreeNode(13);
    root->right->right = new TreeNode(4);

    TreeNode *single = new TreeNode(1);

    cout << boolalpha;
    cout << hasPathSum(root, 20) << " (expected true)\n";
    cout << hasPathSum(root, 17) << " (expected true)\n";
    cout << hasPathSum(root, 100) << " (expected false)\n";
    cout << hasPathSum(single, 1) << " (expected true)\n";
    cout << hasPathSum(nullptr, 0) << " (expected false)\n";
    return 0;
}
