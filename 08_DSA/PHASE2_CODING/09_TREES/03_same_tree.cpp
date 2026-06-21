// ============================================================
// SAME TREE   —   Trees (recursion on TWO trees)
// ============================================================
// Do binary trees p aur q. Bilkul EK JAISE hain? (same structure + same values) -> true/false
//   p: 1        q: 1         -> true
//     / \         / \
//    2   3       2   3
//
//   p: 1        q: 1         -> false  (structure alag)
//     /             \
//    2               2
//
// SIGNAL: "do tree compare / same hai?" -> RECURSION dono pe SAATH.
//   Soch (template wahi -- base + node-kaam + recurse, par AB DO tree saath):
//     - base: dono NULL -> same (true). ek NULL ek nahi -> alag (false).
//     - node-kaam: dono ki value same? nahi -> false.
//     - recurse: p->left vs q->left  AUR  p->right vs q->right (dono true ho tabhi same)
//   No code. Copy pe do chhote tree saath chala ke dekh.
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
bool isSameTree(TreeNode *p, TreeNode *q)
{
    if (p == nullptr && q == nullptr)
    {
        return true;
    }
    if (p == nullptr && q != nullptr)
    {
        return false;
    }
    if (p != nullptr && q == nullptr)
    {
        return false;
    }

    if (p->val != q->val)
    {
        return false;
    }
    return isSameTree(p->left, q->left) || isSameTree(p->right, q->right);
}

// ---- helper (mat chhu) ----
TreeNode *build(vector<int> v)
{
    if (v.empty() || v[0] == -1)
        return nullptr;
    TreeNode *root = new TreeNode(v[0]);
    queue<TreeNode *> qq;
    qq.push(root);
    int i = 1;
    while (!qq.empty() && i < v.size())
    {
        TreeNode *node = qq.front();
        qq.pop();
        if (i < v.size() && v[i] != -1)
        {
            node->left = new TreeNode(v[i]);
            qq.push(node->left);
        }
        i++;
        if (i < v.size() && v[i] != -1)
        {
            node->right = new TreeNode(v[i]);
            qq.push(node->right);
        }
        i++;
    }
    return root;
}

int main()
{
    cout << isSameTree(build({1, 2, 3}), build({1, 2, 3})) << endl;             // expected 1 (same)
    cout << isSameTree(build({1, 2}), build({1, -1, 2})) << endl;               // expected 0 (structure alag)
    cout << isSameTree(build({1, 2, 1}), build({1, 1, 2})) << endl;             // expected 0 (values alag)
    cout << isSameTree(build({}), build({})) << endl;                           // expected 1 (dono empty)
    cout << isSameTree(build({1}), build({})) << endl;                          // expected 0
    cout << isSameTree(build({1, 2, 3, 4, 5}), build({1, 2, 3, 4, 5})) << endl; // expected 1
    return 0;
}
