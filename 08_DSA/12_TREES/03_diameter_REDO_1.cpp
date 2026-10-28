// ============================================================
// DIAMETER OF BINARY TREE  (LC-543)  — REDO_1 (blank redo)
// ------------------------------------------------------------
// PEHLE YE (30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ------------------------------------------------------------
// KYA KARNA: binary tree diya. kisi bhi DO node ke beech sabse LAMBA path ki length lautao,
//            EDGES me ginti. path root se guzre ya na guzre.
//
// INPUT: level-order vector, N = khaali jagah (build helper neeche, mat chhED).
//   [1,2,3,4,5]                -> 3
//   [1,2]                      -> 1
//   [1]                        -> 0
//   []                         -> 0
//   [1,N,2,3,4,5,N,N,6]        -> 4
//   [1,2,N,3,N,4]              -> 3
//   [1,2,3,4,N,N,5,6,N,N,7]    -> 6
// ============================================================
#include <bits/stdc++.h>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int diameterOfBinaryTree(TreeNode *root)
{
    // tera code yahan
    return 0;
}

// --- build helper (mat chhED — tera kaam sirf diameterOfBinaryTree) ---
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

void check(vector<long long> v, int exp, int t)
{
    int got = diameterOfBinaryTree(build(v));
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, 2, 3, 4, 5}, 3, 1);
    check({1, 2}, 1, 2);
    check({1}, 0, 3);
    check({}, 0, 4);
    check({1, N, 2, 3, 4, 5, N, N, 6}, 4, 5);
    check({1, 2, N, 3, N, 4}, 3, 6);
    check({1, 2, 3, 4, N, N, 5, 6, N, N, 7}, 6, 7);
    return 0;
}
