// ============================================================
// BUILD BINARY TREE from PREORDER + INORDER  (LC-105)  — REDO_1 (blank redo)
// ------------------------------------------------------------
// KYA KARNA: preorder[] + inorder[] (unique values) diye. Tree bana ke
//   ROOT lauta.
// FAMILY (nudge, baaki khud): preorder[0] = root; inorder me pivot pe
//   LEFT | root | RIGHT split. (recursion + preorder pointer ka dhyaan.)
//
// TESTS: buildTree ke baad tree ki preorder+inorder DObara input se
//   match honi chahiye (verify helpers neeche, mat chhED).
//   pre=[3,9,20,15,7]  in=[9,3,15,20,7]
//   pre=[1]            in=[1]              (single)
//   pre=[1,2]          in=[2,1]            (left-child)
//   pre=[1,2]          in=[1,2]            (right-child)
//   pre=[1,2,3,4]      in=[4,3,2,1]        (left-skew)
//   pre=[1,2,3,4]      in=[1,2,3,4]        (right-skew)
// ============================================================
#include <bits/stdc++.h>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode *solve(int &rootIndex, vector<int> &preorder, vector<int> &inorder, int l, int r, unordered_map<int, int> &mp)
{
    if (l > r)
        return NULL;

    int pivot = mp[preorder[rootIndex]];
    rootIndex++;

    TreeNode *newNode = new TreeNode(inorder[pivot]);
    newNode->left = solve(rootIndex, preorder, inorder, l, pivot - 1, mp);
    newNode->right = solve(rootIndex, preorder, inorder, pivot + 1, r, mp);
    return newNode;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
{
    int rootIndex = 0;
    unordered_map<int, int> mp;
    for (int i = 0; i < inorder.size(); i++)
    {
        mp[inorder[i]] = i;
    }

    return solve(rootIndex, preorder, inorder, 0, inorder.size() - 1, mp);
}

// --- verify helpers (mat chhED — tera kaam sirf buildTree) ---
void preT(TreeNode *r, vector<int> &out)
{
    if (!r)
        return;
    out.push_back(r->val);
    preT(r->left, out);
    preT(r->right, out);
}
void inT(TreeNode *r, vector<int> &out)
{
    if (!r)
        return;
    inT(r->left, out);
    out.push_back(r->val);
    inT(r->right, out);
}

void check(vector<int> preIn, vector<int> inIn)
{
    TreeNode *root = buildTree(preIn, inIn);
    vector<int> gotPre, gotIn;
    preT(root, gotPre);
    inT(root, gotIn);
    bool ok = (gotPre == preIn && gotIn == inIn);
    cout << (ok ? "PASS" : "FAIL") << "  pre=";
    for (int x : gotPre)
        cout << x << " ";
    cout << " in=";
    for (int x : gotIn)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    check({3, 9, 20, 15, 7}, {9, 3, 15, 20, 7});
    check({1}, {1});
    check({1, 2}, {2, 1});
    check({1, 2}, {1, 2});
    check({1, 2, 3, 4}, {4, 3, 2, 1});
    check({1, 2, 3, 4}, {1, 2, 3, 4});
    return 0;
}
