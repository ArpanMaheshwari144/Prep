// ============================================================
// LOWEST COMMON ANCESTOR (Binary Tree)   —   Trees (DFS bubble-up)
// ============================================================
// p aur q diye. LCA = sabse NEECHE wala node jiske neeche DONO aate hain.
// (node khud ka bhi ancestor maana jaata.)
//
//        3
//       / \
//      5   1
//     / \  / \
//    6  2 0  8
//
//   LCA(6,2) = 5    LCA(5,1) = 3    LCA(5,6) = 5 (5 khud apna ancestor)
//
// SIGNAL: "do node ka milne ka point / common ancestor" -> DFS + jawab UPAR bubble karo.
//   Tera derive (sahi tha):
//     - base: node null -> null ;  node == p ya q -> wahi node (mil gaya)
//     - left = recurse(left), right = recurse(right)
//     - left null  -> right bhejo
//     - right null -> left bhejo
//     - dono non-null -> root bhejo (p ek taraf, q doosri -> yahi LCA)
//   No code likha tujhe -- tu bhar. p/q yahan node pointers diye (main mein dhoondh ke pass kiye).
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

// ---- TERA kaam: yeh function bhar (DFS bubble-up) ----
TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
{
    if (!root)
    {
        return nullptr;
    }

    if (root == p || root == q)
    {
        return root;
    }

    TreeNode *left = lowestCommonAncestor(root->left, p, q);
    TreeNode *right = lowestCommonAncestor(root->right, p, q);

    if (left == NULL)
    {
        return right;
    }
    else if (right == NULL)
    {
        return left;
    }
    else
    {
        return root;
    }
    return nullptr;
}

// ---- helpers (mat chhu) ----
TreeNode *build(vector<int> v)
{
    if (v.empty() || v[0] == -1)
        return nullptr;
    TreeNode *root = new TreeNode(v[0]);
    queue<TreeNode *> bq;
    bq.push(root);
    int i = 1;
    while (!bq.empty() && i < v.size())
    {
        TreeNode *node = bq.front();
        bq.pop();
        if (i < v.size() && v[i] != -1)
        {
            node->left = new TreeNode(v[i]);
            bq.push(node->left);
        }
        i++;
        if (i < v.size() && v[i] != -1)
        {
            node->right = new TreeNode(v[i]);
            bq.push(node->right);
        }
        i++;
    }
    return root;
}

// value se node dhoondho (test setup ke liye)
TreeNode *find(TreeNode *root, int target)
{
    if (!root)
        return nullptr;
    if (root->val == target)
        return root;
    TreeNode *l = find(root->left, target);
    if (l)
        return l;
    return find(root->right, target);
}

int main()
{
    TreeNode *r = build({3, 5, 1, 6, 2, 0, 8});
    cout << lowestCommonAncestor(r, find(r, 6), find(r, 2))->val << endl; // expected 5
    cout << lowestCommonAncestor(r, find(r, 5), find(r, 1))->val << endl; // expected 3
    cout << lowestCommonAncestor(r, find(r, 5), find(r, 6))->val << endl; // expected 5 (ancestor of itself)
    cout << lowestCommonAncestor(r, find(r, 6), find(r, 8))->val << endl; // expected 3
    cout << lowestCommonAncestor(r, find(r, 0), find(r, 8))->val << endl; // expected 1

    TreeNode *r2 = build({1, 2});
    cout << lowestCommonAncestor(r2, find(r2, 1), find(r2, 2))->val << endl; // expected 1
    return 0;
}
