// ============================================================
// VALIDATE BINARY SEARCH TREE        —   Trees (range window)
// ============================================================
// BST = har node ke liye: left subtree SAARE chhote, right subtree SAARE bade.
// Check karo poora tree yeh maanta hai ya nahi.
//
//        5                     5
//       / \                   / \
//      3   8   -> VALID      3   8   -> INVALID
//     / \                       / \
//    1   4                     2   9    <- 2 root 5 ke RIGHT side mein, par 2<5 -> galat
//
// TRAP: node ko sirf apne PARENT se compare karna GALAT (2, parent 8 se chhota lagta,
//       par poore tree ke hisaab se galat jagah).
//
// SIGNAL: "valid BST?" -> har node ek allowed WINDOW (min,max) ke andar hona chahiye.
//   Soch (jo samjha):
//     - root window = (-inf, +inf)
//     - har node pe check: min < node->val < max ? nahi -> false
//     - LEFT recurse  -> window (min, node->val)   [max ko node se replace -- sab chhote]
//     - RIGHT recurse -> window (node->val, max)   [min ko node se replace -- sab bade]
//     - base: node null -> true
//   C++ tip: inf ke liye `long long` (LLONG_MIN/LLONG_MAX) -- node INT_MIN/MAX bhi ho sakti.
//   No code -- tu helper bana (root, min, max).
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

// ---- TERA kaam: yeh function bhar (helper with min,max use kar sakta ho) ----
bool solve(TreeNode *root, long long minVal, long long maxVal)
{
    if (!root)
    {
        return true;
    }

    if (root->val > minVal && root->val < maxVal)
    {
        return solve(root->left, minVal, root->val) && solve(root->right, root->val, maxVal);
    }
    return false;
}
bool isValidBST(TreeNode *root)
{
    return solve(root, LLONG_MIN, LLONG_MAX);
}

// ---- helpers (mat chhu) ----
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
    cout << isValidBST(build({5, 3, 8, 1, 4})) << endl;            // expected 1 (valid)
    cout << isValidBST(build({5, 3, 8, -1, -1, 2, 9})) << endl;    // expected 0 (2 < 5 but on right side)
    cout << isValidBST(build({2, 1, 3})) << endl;                  // expected 1
    cout << isValidBST(build({1})) << endl;                        // expected 1
    cout << isValidBST(build({})) << endl;                         // expected 1 (empty = valid)
    cout << isValidBST(build({5, 4, 6, -1, -1, 3, 7})) << endl;    // expected 0 (3 < 5 on right subtree)
    cout << isValidBST(build({10, 5, 15, -1, -1, 6, 20})) << endl; // expected 0 (6 < 10 on right subtree)
    return 0;
}
