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
//  2. left  = invertTree(root->left)     -> left subtree invert
//  3. right = invertTree(root->right)    -> right subtree invert
//  4. swap(root->left, root->right)      -> children POINTERS swap
//  5. return root
//
//  ★★ POINTER swap (root->left, root->right) -- NA value:
//     swap(left->val, right->val) galat -> (a) leaf pe left=null -> null->val = CRASH (segfault)
//                                          (b) crash na ho tab bhi: value badalta, poora SUBTREE side nahi jaata.
//     invert = SUBTREE ko side badalna -> isliye pointers swap, values nahi.
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
