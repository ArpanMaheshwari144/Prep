// ============================================================
// DIAMETER OF BINARY TREE   —   [C] COLD RE-CODE (revision)
// ============================================================
// Diameter = kisi BHI 2 node ke beech sabse lambi path ki LENGTH (edges).
// Path root se gujre ZAROORI nahi.
//          1
//        2   3
//      4   5        -> diameter 3   (4-2-1-3 = 3 edges)
//   single node -> 0
//
// SIGNAL: "longest path / diameter" -> depth-recursion + har node pe MAX track.
//   Soch (bina purana dekhe -- COLD):
//     - kisi node se gujarne wali sabse lambi path = left-depth + right-depth (edges)
//     - diameter = SAB nodes mein se is (left+right) ka MAXIMUM
//     - depth recursively nikaalo (max-depth jaisा) AUR saath-saath ek BAHAR-wala max update karo
//   3 gotchas yaad rakh (yahi pichli baar phasа tha):
//     (1) helper DEPTH lautata, diameter = SIDE-effect (global max)
//     (2) diameter-func max return kare, depth nahi
//     (3) global max har call pe RESET
//   No code -- khud likh, cold.
// ============================================================

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ---- TERA kaam: yeh bhar (helper depth + global max) ----
int maxD;
int maxDepth(TreeNode *root)
{
    if (root == nullptr)
    {
        return 0;
    }

    int left = maxDepth(root->left);
    int right = maxDepth(root->right);

    maxD = max(maxD, left + right);

    return 1 + max(left, right);
}

int diameterOfBinaryTree(TreeNode *root)
{
    maxD = 0;
    maxDepth(root);
    return maxD;
}

// ---- helper (mat chhu) ----
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
    cout << diameterOfBinaryTree(build({1, 2, 3, 4, 5})) << endl;      // expected 3
    cout << diameterOfBinaryTree(build({1, 2})) << endl;               // expected 1
    cout << diameterOfBinaryTree(build({1})) << endl;                  // expected 0
    cout << diameterOfBinaryTree(build({})) << endl;                   // expected 0
    cout << diameterOfBinaryTree(build({1, 2, -1, 3, -1, 4})) << endl; // expected 3 (left chain, 4 nodes -> 3 edges)
    return 0;
}
