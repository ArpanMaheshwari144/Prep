// ============================================================
// INVERT BINARY TREE   —   Trees (recursion)
// ============================================================
// Tree ka MIRROR banao — har node ke left aur right swap. Naya root return.
//        4                 4
//       / \               / \
//      2   7     ->       7   2
//     / \ / \           / \  / \
//    1  3 6  9         9  6 3  1
//
// SIGNAL: "tree transform / har node pe kuch karna" -> RECURSION (DFS).
//   Soch (max-depth wala SAME template):
//     - base: node NULL -> return (kuch nahi)
//     - node pe KAAM: left aur right child SWAP karo
//     - phir left subtree invert, right subtree invert (recursion)
//   No code. Copy pe chhota tree palat ke dekh.
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

// ---- TERA kaam: yeh function ----
TreeNode *invertTree(TreeNode *root)
{
    if(root == nullptr){
        return nullptr;
    }

    swap(root->left, root->right);

    TreeNode* l = invertTree(root->left);
    TreeNode* r = invertTree(root->right);

    return root;

}

// ---- helpers (mat chhu) ----
TreeNode *build(vector<int> v)
{
    if (v.empty() || v[0] == -1) return nullptr;
    TreeNode *root = new TreeNode(v[0]);
    queue<TreeNode *> q; q.push(root);
    int i = 1;
    while (!q.empty() && i < v.size())
    {
        TreeNode *node = q.front(); q.pop();
        if (i < v.size() && v[i] != -1) { node->left = new TreeNode(v[i]); q.push(node->left); } i++;
        if (i < v.size() && v[i] != -1) { node->right = new TreeNode(v[i]); q.push(node->right); } i++;
    }
    return root;
}
// level-order print (NULL = -1) -- output yahi se compare kar
void printLevel(TreeNode *root)
{
    if (!root) { cout << "(empty)" << endl; return; }
    queue<TreeNode *> q; q.push(root);
    while (!q.empty())
    {
        TreeNode *n = q.front(); q.pop();
        if (n) { cout << n->val << " "; q.push(n->left); q.push(n->right); }
        else cout << "x ";
    }
    cout << endl;
}

int main()
{
    printLevel(invertTree(build({4, 2, 7, 1, 3, 6, 9}))); // expected: 4 7 2 9 6 3 1 x x x x x x x x
    printLevel(invertTree(build({1, 2})));                 // expected: 1 x 2 x x
    printLevel(invertTree(build({})));                     // expected: (empty)
    printLevel(invertTree(build({1})));                    // expected: 1 x x
    return 0;
}
