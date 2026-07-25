// ============================================================
// BALANCED BINARY TREE — (LeetCode 110, Easy)   [TREES #12]
// ============================================================
// tree HEIGHT-BALANCED hai? -> HAR node pe: left aur right subtree ki
// HEIGHT ka farak <= 1 (abs(leftH - rightH) <= 1). true/false.
//
//        3
//       / \
//      9  20          -> true (har node balanced)
//         / \
//        15  7
//
//        1
//       / \
//      2   2
//     / \
//    3   3
//   / \
//  4   4              -> false (left bahut gehra, farak > 1)
//
//   []  (null)  -> true
//   [1]         -> true
// ============================================================
// ---- APPROACH ----  (= MAX-DEPTH code + thoda change; -1 = "imbalance mila" ka SIGNAL)
//  ★ idea: alag "balanced?" pass NAHI. height nikaalte-nikaalte HI balance check kar lo -> O(n) ek pass.
//     helper maxDepth(root) ab 2 kaam karta: (a) height return, YA (b) -1 agar kahin imbalance.
//  1. base : null -> 0.
//  2. left  = maxDepth(root->left);   left == -1 -> turant -1 return (neeche imbalance mil gaya, aage mat dekho).
//  3. right = maxDepth(root->right);  right == -1 -> turant -1 return.
//  4. IS node pe check: abs(left - right) > 1 -> -1 return (yahan imbalance).
//  5. warna balanced -> asli HEIGHT return: 1 + max(left, right).
//  isBalanced: maxDepth(root) != -1 -> true.  (kahin -1 aaya = imbalance = false)
//  ★★ -1 = SENTINEL/flag: "imbalance neeche mila" -> UPAR propagate -> ek node bhi galat -> poora false.
//     O(n) single-pass (O(n^2) se behtar: har node pe alag-alag height nikaalne ki zaroorat nahi).
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

// ye max-depth hi hai + balance-check. return = HEIGHT, YA -1 (matlab: neeche kahin imbalance mila)
int maxDepth(TreeNode *root)
{
    if (!root)
    {
        return 0; // null ki height 0
    }

    // --- DEBUG-PRINT (recursion samajhne ko; final solution me HATA dena) ---
    // ★ enter-print null-check ke BAAD -- warna null node pe root->val = CRASH (segfault).
    cout << "enter : " << root->val << endl;

    int left = maxDepth(root->left);
    if (left == -1)
    {
        return -1; // left-subtree imbalanced -> -1 UPAR bhejo, aage mat dekho (short-circuit)
    }
    int right = maxDepth(root->right);
    if (right == -1)
    {
        return -1; // right-subtree imbalanced -> -1 propagate
    }

    if (abs(left - right) > 1)
    {
        return -1; // ★ IS node pe imbalance -> -1 (yehi wo "har node" check tha)
    }

    int result = 1 + max(left, right); // balanced -> asli HEIGHT (max-depth wala)

    // --- DEBUG-PRINT: left/right = neeche se aayi VALUES, result = HEIGHT jo UPAR ja rahi ---
    cout << "node " << root->val << " : left=" << left << ", right=" << right
         << " -> HEIGHT return " << result << endl;

    return result;
}

bool isBalanced(TreeNode *root)
{
    if (!root)
        return true; // (redundant -- maxDepth(null)=0 bhi != -1 -> true; par saaf padhne ko rakha)

    return maxDepth(root) != -1 ? true : false; // kahin -1 aaya = imbalance = false
}

int main()
{
    //        3
    //       / \
    //      9  20
    //         / \
    //        15  7
    TreeNode *a = new TreeNode(3);
    a->left = new TreeNode(9);
    a->right = new TreeNode(20);
    a->right->left = new TreeNode(15);
    a->right->right = new TreeNode(7);

    //        1
    //       / \
    //      2   2
    //     / \
    //    3   3
    //   / \
    //  4   4
    // TreeNode *b = new TreeNode(1);
    // b->left = new TreeNode(2);
    // b->right = new TreeNode(2);
    // b->left->left = new TreeNode(3);
    // b->left->right = new TreeNode(3);
    // b->left->left->left = new TreeNode(4);
    // b->left->left->right = new TreeNode(4);

    // TreeNode *c = new TreeNode(1);

    // ★ TRAP TEST (d): ROOT balanced, par ANDAR ka node imbalanced -> answer FALSE.
    //        1
    //       / \
    //      2   2
    //     /     \
    //    3       3
    //   /         \
    //  4           4
    //  root: leftH=3, rightH=3 -> balanced. PAR node-2(left): leftH=2, rightH=0 -> imbalanced!
    // TreeNode *d = new TreeNode(1);
    // d->left = new TreeNode(2);
    // d->right = new TreeNode(2);
    // d->left->left = new TreeNode(3);
    // d->left->left->left = new TreeNode(4);
    // d->right->right = new TreeNode(3);
    // d->right->right->right = new TreeNode(4);

    //   1
    //  /
    // 2       -> left-skew, root pe hi imbalanced (leftH=2, rightH=0)
    ///
    // TreeNode *e = new TreeNode(1);
    // e->left = new TreeNode(2);
    // e->left->left = new TreeNode(3);

    cout << boolalpha;
    cout << isBalanced(a) << " (expected true)\n";
    // cout << isBalanced(b) << " (expected false)\n";
    // cout << isBalanced(c) << " (expected true)\n";
    // cout << isBalanced(nullptr) << " (expected true)\n";
    // cout << isBalanced(d) << " (expected false)  <- ★ TRAP: root balanced par andar imbalanced\n";
    // cout << isBalanced(e) << " (expected false)\n";
    return 0;
}
