// ============================================================
// DIAMETER OF BINARY TREE — (LeetCode 543, Easy)   [TREES #3]
// ============================================================
// diameter = kisi bhi DO node ke beech sabse LAMBA path ki length (EDGES me).
// path root se guzre YA na guzre. (length = edges ki ginti, nodes-1.)
//
//        1
//       / \
//      2   3        diameter = 3  (path 4-2-1-3 = 3 edges)
//     / \
//    4   5
//
//   [1,2,3,4,5]  -> 3
//   [1,2]        -> 1
//   [1]          -> 0
// ============================================================
//
// ---- APPROACH ----
//  ★ MAX-DEPTH pe build (height). har node pe DO alag sawaal:
//     Q1 "main kitna OONCHA?"          = HEIGHT = 1 + max(leftH, rightH)   -> PARENT ko chahiye -> ye RETURN.
//     Q2 "mujh se guzarne wala path?"  = leftH + rightH                    -> ANSWER-candidate -> global MAX track.
//
//  ★★ RETURN height kyun, diameter kyun NAHI (yehi asli samajh):
//     parent ko child ki HEIGHT chahiye (apni leftH/rightH banane ko). agar diameter return karte to
//     height KHO jaati -> parent apna kaam nahi kar paata. HEIGHT = recursion ka "eendhan" (upar flow karta);
//     DIAMETER byproduct hai -> return-slot me nahi ja sakta -> SIDE me (global/reference) collect.
//     (diameter clean-compose nahi hoti -- wo heights se banti, isliye heights ko upar bhejna PADTA.)
//
//  ★ ORDER: recurse-PEHLE (post-order) -- leftH/rightH chahiye TABHI Q2 nikle. yehi "order matters" ka live case.
//  ★ steps: base null->0 · leftH,rightH recurse · maxDia = max(maxDia, leftH+rightH) · return 1+max(leftH,rightH).
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

int height(TreeNode *root, int &maxDia)
{
    if (!root)
    {
        return 0;
    }

    // recursion pehle ekdum neeche (leaf) tak jaati -- beech me rukti nahi; kaam PEECHE aate hue hota (post-order)
    int left = height(root->left, maxDia);
    int right = height(root->right, maxDia);

    maxDia = max(maxDia, left + right); // is node se guzarne wali diameter = leftH + rightH (dono taraf ka jod) -> global max

    return 1 + max(left, right); // return HEIGHT (isko depth bhi keh sakte) -- PARENT ke liye
}

int diameterOfBinaryTree(TreeNode *root)
{
    int maxDia = 0;
    height(root, maxDia);
    return maxDia;
}

int main()
{
    //      1
    //     / \
    //    2   3
    //   / \
    //  4   5
    TreeNode *a = new TreeNode(1);
    a->left = new TreeNode(2);
    a->right = new TreeNode(3);
    a->left->left = new TreeNode(4);
    a->left->right = new TreeNode(5);

    TreeNode *b = new TreeNode(1);
    b->left = new TreeNode(2);

    TreeNode *c = new TreeNode(1);

    // d: diameter SUBTREE me (root se NAHI guzarta) -> ye overwrite-bug ko pakadta
    //      1
    //       \
    //        2
    //       / \
    //      3   4
    //     /     \
    //    5       6      diameter = 4 (5-3-2-4-6). bug hota to 3 deta.
    TreeNode *d = new TreeNode(1);
    d->right = new TreeNode(2);
    d->right->left = new TreeNode(3);
    d->right->right = new TreeNode(4);
    d->right->left->left = new TreeNode(5);
    d->right->right->right = new TreeNode(6);

    cout << diameterOfBinaryTree(a) << " (expected 3)\n";
    cout << diameterOfBinaryTree(b) << " (expected 1)\n";
    cout << diameterOfBinaryTree(c) << " (expected 0)\n";
    cout << diameterOfBinaryTree(d) << " (expected 4)  <- ye fix PROVE karta\n";
    return 0;
}
