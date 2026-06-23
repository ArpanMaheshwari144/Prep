// ============================================================
// KTH SMALLEST ELEMENT IN A BST     —   Trees (inorder = sorted)
// ============================================================
// BST diya, k diya. K-th sabse CHHOTA element nikaalo.
//
//        5
//       / \
//      3   8
//     / \   \
//    1   4   9
//
//   inorder (left->node->right) = 1, 3, 4, 5, 8, 9   <- SORTED!
//   k=1 -> 1   k=3 -> 4   k=6 -> 9
//
// SIGNAL: "BST + k-th smallest" -> inorder (left->node->right) BST pe SORTED deta -> k-th nikaalo.
//   Tera plan (sahi): inorder ko ek vector me bharo -> v[k-1] return (0-based, isi liye k-1).
//   base: node null -> return.
//   No code -- tu helper bana (inorder fill kare), phir v[k-1].
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

// ---- TERA kaam: yeh function bhar (inorder -> v[k-1]) ----
void inOrder(TreeNode *root, vector<int> &ans)
{
    if (!root)
    {
        return;
    }

    inOrder(root->left, ans);
    ans.push_back(root->val);
    inOrder(root->right, ans);
}
int kthSmallest(TreeNode *root, int k)
{
    vector<int> ans;
    inOrder(root, ans);
    return ans[k - 1];
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
    TreeNode *r = build({5, 3, 8, 1, 4, -1, 9});
    cout << kthSmallest(r, 1) << endl; // expected 1
    cout << kthSmallest(r, 3) << endl; // expected 4
    cout << kthSmallest(r, 4) << endl; // expected 5
    cout << kthSmallest(r, 6) << endl; // expected 9

    cout << kthSmallest(build({2, 1, 3}), 2) << endl;             // expected 2
    cout << kthSmallest(build({1}), 1) << endl;                   // expected 1
    cout << kthSmallest(build({4, 2, 6, 1, 3, 5, 7}), 5) << endl; // expected 5 (inorder 1,2,3,4,5,6,7)
    return 0;
}
