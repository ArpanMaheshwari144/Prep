// ============================================================
// VALIDATE BINARY SEARCH TREE  (LC-98)  — REDO_1 (blank redo)
// ------------------------------------------------------------
// KYA KARNA: diya tree VALID BST hai? true / false.
//   BST (STRICT): HAR node ke liye -> uske POORE left-subtree ke saare value < node->val
//                 aur POORE right-subtree ke saare value > node->val. Equal = INVALID.
//
// INPUT: level-order vector, N = khaali jagah (build helper neeche, mat chhED).
//   [2,1,3]                 -> true
//   [5,1,4,N,N,3,6]         -> false
//   [5,4,6,N,N,3,7]         -> false
//   [1]                     -> true
//   []                      -> true
//   [2,2,2]                 -> false
//   [1,1]                   -> false
//   [10,5,15,N,N,6,20]      -> false
//   [3,1,5,0,2,4,6]         -> true
//   [INT_MIN]               -> true
//   [INT_MAX]               -> true
//   [INT_MIN,N,INT_MAX]     -> true
//   [INT_MAX,INT_MAX]       -> false
// ============================================================
#include <bits/stdc++.h>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

bool solve(TreeNode *root, long long mini, long long maxi)
{
    if (root == NULL)
    {
        return true;
    }

    if (root->val <= mini || root->val >= maxi)
    {
        return false;
    }

    bool left = solve(root->left, mini, root->val);
    bool right = solve(root->right, root->val, maxi);

    return left && right;
}

bool isValidBST(TreeNode *root)
{
    return solve(root, LLONG_MIN, LLONG_MAX);
}

// --- build helper (mat chhED — tera kaam sirf isValidBST) ---
const long long N = LLONG_MIN; // khaali jagah
TreeNode *build(vector<long long> v)
{
    if (v.empty() || v[0] == N)
        return nullptr;
    TreeNode *root = new TreeNode((int)v[0]);
    queue<TreeNode *> q;
    q.push(root);
    int i = 1;
    while (!q.empty() && i < (int)v.size())
    {
        TreeNode *cur = q.front();
        q.pop();
        if (i < (int)v.size() && v[i] != N)
        {
            cur->left = new TreeNode((int)v[i]);
            q.push(cur->left);
        }
        i++;
        if (i < (int)v.size() && v[i] != N)
        {
            cur->right = new TreeNode((int)v[i]);
            q.push(cur->right);
        }
        i++;
    }
    return root;
}

void check(vector<long long> v, bool exp, int t)
{
    bool got = isValidBST(build(v));
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << boolalpha << got << " exp=" << exp << "\n";
}

int main()
{
    check({2, 1, 3}, true, 1);
    check({5, 1, 4, N, N, 3, 6}, false, 2);
    check({5, 4, 6, N, N, 3, 7}, false, 3);
    check({1}, true, 4);
    check({}, true, 5);
    check({2, 2, 2}, false, 6);
    check({1, 1}, false, 7);
    check({10, 5, 15, N, N, 6, 20}, false, 8);
    check({3, 1, 5, 0, 2, 4, 6}, true, 9);
    check({INT_MIN}, true, 10);
    check({INT_MAX}, true, 11);
    check({INT_MIN, N, INT_MAX}, true, 12);
    check({INT_MAX, INT_MAX}, false, 13);
    return 0;
}
