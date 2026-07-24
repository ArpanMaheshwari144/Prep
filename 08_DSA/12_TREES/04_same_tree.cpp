// ============================================================
// SAME TREE — (LeetCode 100, Easy)   [TREES #4]
// ============================================================
// do binary trees p, q diye. dono BILKUL SAME hain? (same structure + same values). true/false.
//
//   p=[1,2,3]  q=[1,2,3]      -> true
//   p=[1,2]    q=[1,null,2]   -> false   (structure alag)
//   p=[1,2,1]  q=[1,1,2]      -> false   (values alag)
//   p=null     q=null         -> true
//   p=[1]      q=null         -> false
// ============================================================
// ---- APPROACH ----
//  ★ compare-2-trees: BASE -> dono null = true · ek null = false · val alag = false.
//    warna COMBINE = isSame(p->left, q->left) && isSame(p->right, q->right).  (★ && = DONO side match zaroori)
//  (clean base: if(!p || !q) return p==q;  -> dono null=true, ek null=false ek line me.)
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

bool isSameTree(TreeNode *p, TreeNode *q)
{
    // base cases
    if (p == NULL && q == NULL)
    {
        return true;
    }
    if (p == NULL && q != NULL)
    {
        return false;
    }
    if (p != NULL && q == NULL)
    {
        return false;
    }

    // agra value alga to yehi se falue recsre nahi
    if (p->val != q->val)
    {
        return false;
    }
    else if (p->val == q->val)
    {
        // agar value same left and right rceise karke dekho
        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
    return false;
}

int main()
{
    // p=[1,2,3]  q=[1,2,3]  -> true
    TreeNode *p1 = new TreeNode(1);
    p1->left = new TreeNode(2);
    p1->right = new TreeNode(3);
    TreeNode *q1 = new TreeNode(1);
    q1->left = new TreeNode(2);
    q1->right = new TreeNode(3);

    // p=[1,2]  q=[1,null,2]  -> false (structure)
    TreeNode *p2 = new TreeNode(1);
    p2->left = new TreeNode(2);
    TreeNode *q2 = new TreeNode(1);
    q2->right = new TreeNode(2);

    // p=[1,2,1]  q=[1,1,2]  -> false (values)
    TreeNode *p3 = new TreeNode(1);
    p3->left = new TreeNode(2);
    p3->right = new TreeNode(1);
    TreeNode *q3 = new TreeNode(1);
    q3->left = new TreeNode(1);
    q3->right = new TreeNode(2);

    TreeNode *p5 = new TreeNode(1);

    cout << boolalpha;
    cout << isSameTree(p1, q1) << " (expected true)\n";
    cout << isSameTree(p2, q2) << " (expected false)\n";
    cout << isSameTree(p3, q3) << " (expected false)\n";
    cout << isSameTree(nullptr, nullptr) << " (expected true)\n";
    cout << isSameTree(p5, nullptr) << " (expected false)\n";
    return 0;
}
