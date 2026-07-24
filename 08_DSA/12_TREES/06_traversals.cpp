// ============================================================
// TREE TRAVERSALS — inorder / preorder / postorder   [TREES #6 · DFS fundamentals]
// ============================================================
// ---- APPROACH ----
// teeno = SAME recursion (base: if(!root) return;  ·  left recurse  ·  right recurse).
// SIRF ek line -- VISIT = res.push_back(root->val) -- ki JAGAH badalti, aur wahi ORDER decide karti:
//   PREORDER   : VISIT -> left -> right      (root PEHLE)
//   INORDER    : left -> VISIT -> right      (root BEECH me)
//   POSTORDER  : left -> right -> VISIT      (root BAAD me)
// ★ recurse dono HAMESHA same -- sirf VISIT-position badalti. (yehi "order matters", jaise diameter.)
//
//        1
//       / \
//      2   3
//     / \
//    4   5
//   preorder:   1 2 4 5 3
//   inorder:    4 2 5 1 3
//   postorder:  4 5 2 3 1
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

void preorder(TreeNode *root, vector<int> &res)
{
    if (!root)
        return;

    res.push_back(root->val);
    preorder(root->left, res);
    preorder(root->right, res);
}

void inorder(TreeNode *root, vector<int> &res)
{
    if (!root)
        return;

    inorder(root->left, res);
    res.push_back(root->val);
    inorder(root->right, res);
}

void postorder(TreeNode *root, vector<int> &res)
{
    if (!root)
        return;

    postorder(root->left, res);
    postorder(root->right, res);
    res.push_back(root->val);
}

// ---- test helper (print vector) -- mat chhed ----
void printVec(vector<int> &v)
{
    for (int x : v)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    //     1
    //    / \
    //   2   3
    //  / \
    // 4   5
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    vector<int> pre, in, post;
    preorder(root, pre);
    inorder(root, in);
    postorder(root, post);

    cout << "preorder:   ";
    printVec(pre);
    cout << "  (expected 1 2 4 5 3)\n";
    cout << "inorder:    ";
    printVec(in);
    cout << "  (expected 4 2 5 1 3)\n";
    cout << "postorder:  ";
    printVec(post);
    cout << "  (expected 4 5 2 3 1)\n";
    return 0;
}
