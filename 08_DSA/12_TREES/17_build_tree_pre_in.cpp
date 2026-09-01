// ============================================================
// BUILD TREE from PREORDER + INORDER — (LeetCode 105)   [TREES #17]
// ============================================================
// preorder (Root,Left,Right) aur inorder (Left,Root,Right) diye. Original binary tree BANAO.
//
//   - preorder[0] = ROOT (hamesha).
//   - inorder me us root ko dhundo -> uske BAAYE = left-subtree, DAAYE = right-subtree.
//   - recurse. (2 KEY trick: preorder ka MOVING index jo har root pe aage; inorder value->index HashMap O(1) lookup.)
//
// TEST-CASES (preorder , inorder -> tree; check pre+in traversal se verify karta):
//   [3,9,20,15,7] , [9,3,15,20,7]   -> valid tree
//   [1,2]         , [2,1]           -> 2 left-child of 1
//   [1,2]         , [1,2]           -> 2 right-child of 1
//   [1]           , [1]             -> single node
//   []            , []              -> empty (null)
//   [1,2,3,4]     , [4,3,2,1]       -> left-skewed
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

TreeNode *solve(int &rootIndex, vector<int> &preorder, vector<int> &inorder, int l, int r)
{
    if (l > r)
    {
        return NULL;
    }

    int pivot = l;
    while (inorder[pivot] != preorder[rootIndex])
    {
        pivot++;
    }
    rootIndex++;

    TreeNode *newNode = new TreeNode(inorder[pivot]);
    newNode->left = solve(rootIndex, preorder, inorder, l, pivot - 1);
    newNode->right = solve(rootIndex, preorder, inorder, pivot + 1, r);
    return newNode;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
{
    int rootIndex = 0;
    return solve(rootIndex, preorder, inorder, 0, inorder.size() - 1);
}

// ---- verify helpers (built tree ka pre+in traversal nikaalte) ----
void pre(TreeNode *r, vector<int> &out)
{
    if (!r)
        return;
    out.push_back(r->val);
    pre(r->left, out);
    pre(r->right, out);
}
void in(TreeNode *r, vector<int> &out)
{
    if (!r)
        return;
    in(r->left, out);
    out.push_back(r->val);
    in(r->right, out);
}

void check(vector<int> preorder, vector<int> inorder)
{
    TreeNode *root = buildTree(preorder, inorder);
    vector<int> gotPre, gotIn;
    pre(root, gotPre);
    in(root, gotIn);
    bool ok = (gotPre == preorder && gotIn == inorder);
    cout << (ok ? "PASS" : "FAIL")
         << "  built-pre=";
    for (int x : gotPre)
        cout << x << " ";
    cout << " built-in=";
    for (int x : gotIn)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    check({3, 9, 20, 15, 7}, {9, 3, 15, 20, 7});
    check({1, 2}, {2, 1});
    check({1, 2}, {1, 2});
    check({1}, {1});
    check({}, {});
    check({1, 2, 3, 4}, {4, 3, 2, 1});
    return 0;
}
