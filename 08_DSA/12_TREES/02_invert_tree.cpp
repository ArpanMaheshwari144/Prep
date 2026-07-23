// ============================================================
// INVERT BINARY TREE — (LeetCode 226, Easy)   [TREES #2]
// ============================================================
// har node ke LEFT aur RIGHT child ko SWAP karo (poora tree mirror ho jaye).
// inverted root return karo.
//
//        4                    4
//       / \                  / \
//      2   7     -->        7   2
//     / \ / \              / \ / \
//    1  3 6  9            9  6 3  1
//
//   level-order pehle:  4 2 7 1 3 6 9
//   invert ke baad:     4 7 2 9 6 3 1
// ============================================================
//
// ---- APPROACH ----
//  ★ tree = RECURSION. kaam = children SWAP.
//  1. BASE: root null -> return null
//  2. invertTree(root->left)             -> left subtree invert
//  3. invertTree(root->right)            -> right subtree invert
//  4. swap(root->left, root->right)      -> children POINTERS swap
//  5. return root
//
//  ★★ SWAP kise? -> POINTER (root->left, root->right), NA value (->val). ye samajh:
//     recurse ke baad -> root->left = left-subtree ka root · root->right = right-subtree ka root.
//     -- POINTER swap: dono pointers palat do -> poora LEFT-subtree RIGHT me chala jaata, RIGHT-subtree LEFT me.
//        = poori BRANCH side badli. SAHI (invert = subtree mirror).
//     -- VALUE swap (left->val, right->val): sirf 2 top NUMBER badalte, subtree apni jagah pada rehta. GALAT.
//        + LEAF pe CRASH kyun: leaf ke DONO bachche NULL hote. value-swap ko bachcho ka
//          ->val chahiye -> par NULL ka ->val nahi hota -> null pointer access -> SEGFAULT.
//     yaad: invert = poori BRANCH ghumao (pointer), sirf number ghumana nahi (value).
//  ★ ORDER free: swap-first ya recurse-first -- dono same (swap + recurse INDEPENDENT,
//    swap ko recursion-result nahi chahiye).
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

TreeNode *invertTree(TreeNode *root)
{
    if (!root)
    {
        return nullptr;
    }
    
    invertTree(root->left);
    invertTree(root->right);

    swap(root->left, root->right);

    return root;
}

// ---- test helper (level-order print) -- ise mat chhed, sirf check ke liye ----
void printLevelOrder(TreeNode *root)
{
    if (!root)
    {
        cout << "(empty)";
        return;
    }
    queue<TreeNode *> q;
    q.push(root);
    while (!q.empty())
    {
        TreeNode *n = q.front();
        q.pop();
        cout << n->val << " ";
        if (n->left)
            q.push(n->left);
        if (n->right)
            q.push(n->right);
    }
}

int main()
{
    //        4
    //       / \
    //      2   7
    //     / \ / \
    //    1  3 6  9
    TreeNode *root = new TreeNode(4);
    root->left = new TreeNode(2);
    root->right = new TreeNode(7);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(3);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(9);

    cout << "before: ";
    printLevelOrder(root);
    cout << "\n";
    invertTree(root);
    cout << "after:  ";
    printLevelOrder(root);
    cout << "  (expected 4 7 2 9 6 3 1)\n";
    return 0;
}
