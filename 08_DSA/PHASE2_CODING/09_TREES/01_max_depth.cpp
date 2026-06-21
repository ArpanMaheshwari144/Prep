// ============================================================
// MAXIMUM DEPTH OF BINARY TREE   —   Trees (recursion)
// ============================================================
// Binary tree ki MAX depth (root se sabse door leaf tak ke nodes ki ginti).
//        3
//       / \
//      9  20
//        /  \
//       15   7      -> depth 3
//   empty tree -> 0
//
// SIGNAL: "tree ki height/depth/ya har node pe kuch compute" -> RECURSION (DFS).
//   Soch (tera spatial mode -- tree ko dekho):
//     - ek node ki depth = 1 + (uske left aur right mein jo BADA ho)
//     - base: node NULL -> depth 0
//     - left subtree khud solve karega, right khud -> tu bas combine kar (1 + max)
//   No code. Copy pe chhota tree bana ke recursion soch.
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
int maxDepth(TreeNode *root)
{
    if (root == nullptr)
    {
        return 0;
    }

    int left = maxDepth(root->left);
    int right = maxDepth(root->right);

    return 1 + max(left, right);
}

// ---- helper: level-order array se tree banao (-1 = NULL) ----
TreeNode *build(vector<int> v)
{
    if (v.empty() || v[0] == -1)
        return nullptr;
    TreeNode *root = new TreeNode(v[0]);
    queue<TreeNode *> q;
    q.push(root);
    int i = 1;
    while (!q.empty() && i < v.size())
    {
        TreeNode *node = q.front();
        q.pop();
        if (i < v.size() && v[i] != -1)
        {
            node->left = new TreeNode(v[i]);
            q.push(node->left);
        }
        i++;
        if (i < v.size() && v[i] != -1)
        {
            node->right = new TreeNode(v[i]);
            q.push(node->right);
        }
        i++;
    }
    return root;
}

int main()
{
    cout << maxDepth(build({3, 9, 20, -1, -1, 15, 7})) << endl; // expected 3
    cout << maxDepth(build({1, 2, 3, 4, 5})) << endl;           // expected 3
    cout << maxDepth(build({1})) << endl;                       // expected 1
    cout << maxDepth(build({})) << endl;                        // expected 0
    cout << maxDepth(build({1, 2, -1, 3, -1, 4})) << endl;      // expected 4 (left-skewed)
    return 0;
}
