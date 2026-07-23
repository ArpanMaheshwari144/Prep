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
// ★ SAME tree mechanic (base + recurse + kaam) -- bas is baar KAAM = swap:
//   -- BASE CASE: node NULL -> return NULL (kuch swap nahi).
//   -- har node pe: left/right SWAP karo, phir dono bachcho pe recurse.
//   -- return root.  (kaam pehle ya recurse pehle -- dono chalta, soch kyun.)
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

    // swap(root->left, root->right); // swap + recurse INDEPENDENT -> order matter NAHI (swap-first ya recurse-first, dono same)

    TreeNode *left = invertTree(root->left);
    TreeNode *right = invertTree(root->right);

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
