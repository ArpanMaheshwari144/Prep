// ============================================================
// DIAMETER OF BINARY TREE   —   Trees (depth + track max)
// ============================================================
// Diameter = kisi BHI do node ke beech sabse lambi path ki LENGTH (edges ki ginti).
// Path root se gujre ZAROORI nahi.
//        1
//       / \
//      2   3
//     / \
//    4   5      -> diameter 3   (path: 4-2-1-3, ya 5-2-1-3 -> 3 edges)
//   single node -> 0
//
// SIGNAL: "longest path / diameter" -> depth-recursion + har node pe MAX track.
//   Soch (tera instinct sahi -- max-depth pe banta):
//     - kisi node se gujarne wali sabse lambi path = (left ki depth) + (right ki depth)  [edges]
//     - diameter = SAB nodes mein se is (left+right) ka MAXIMUM
//     - toh: depth recursively nikaalo (jaise max-depth), AUR saath-saath har node pe
//       (left+right) ko ek max-variable mein update karte jao.
//     - dikkat: max sab nodes ka chahiye -> ek BAHAR wala variable (global / reference) soch.
//   No code. Copy pe tree chala -- depth lautao, max update karo.
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

// ---- TERA kaam: yeh function (helper bana sakta ho jo depth lautaye + max update kare) ----
int maxi;
int maxDepth(TreeNode *root)
{
    if (root == nullptr)
    {
        return 0;
    }

    int left = maxDepth(root->left);
    int right = maxDepth(root->right);

    maxi = max(maxi, left + right);

    return 1 + max(left, right);
}

int diameterOfBinaryTree(TreeNode *root)
{
    maxi = 0;
    maxDepth(root);
    return maxi;
}

// ---- helper (mat chhu) ----
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

int main()
{
    cout << diameterOfBinaryTree(build({1, 2, 3, 4, 5})) << endl; // expected 3
    cout << diameterOfBinaryTree(build({1, 2})) << endl;          // expected 1
    cout << diameterOfBinaryTree(build({1})) << endl;             // expected 0
    cout << diameterOfBinaryTree(build({})) << endl;              // expected 0
    cout << diameterOfBinaryTree(build({1, 2, -1, 3, -1, 4})) << endl; // expected 3 (left-skewed chain 4 nodes -> 3 edges)
    return 0;
}
