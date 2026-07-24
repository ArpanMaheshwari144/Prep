// ============================================================
// TREE TRAVERSALS — inorder / preorder / postorder   [TREES #6 · DFS fundamentals]
// ============================================================
// teeno DFS hain. FARAK sirf ek: node ko VISIT (res.push_back(val)) KAB karte ho —
//   PREORDER   : ROOT -> Left -> Right      (visit PEHLE, phir recurse)
//   INORDER    : Left -> ROOT -> Right      (left recurse, visit BEECH me, right recurse)
//   POSTORDER  : Left -> Right -> ROOT      (dono recurse, visit BAAD me)
// -> yehi "ORDER MATTERS" (jaise diameter) — bas visit-line ki JAGAH badalti, recurse wahi.
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
