// ============================================================
// MINIMUM DEPTH OF BINARY TREE — (LeetCode 111, Easy)   [TREES #15 · BFS]
// ============================================================
// root se sabse PAAS wale LEAF tak ki depth (nodes ki ginti).
// LEAF = jiske DONO bachche NULL. (root=1.)
//
//        3
//       / \
//      9  20          -> 2   (root 3 -> 9, aur 9 LEAF hai)
//         / \
//        15  7
//
//   ★ DFS-TRAP (isliye BFS saaf): neeche wale tree me --
//        2
//         \
//          3
//           \
//            4        -> 3   (sirf ek path 2->3->4 = 3 node, 4 leaf)
//   ★ yahan "1 + min(leftDepth, rightDepth)" GALAT: 2 ka left NULL (depth 0) -> min=0 -> galat 1 deta.
//     LEAF tak jaana hai, NULL-child ko "depth 0" mat maano. BFS me ye dikkat hi nahi (first leaf pe ruk jao).
//
//   [3,9,20,null,null,15,7]  -> 2
//   [2,null,3,null,4]        -> 4
//   [1]                      -> 1
//   []  (null root)          -> 0
// ============================================================
// ---- APPROACH ----  (BFS -- level-order skeleton + EARLY-STOP; naya BFS flavor)
//  ★ idea: BFS level-by-level neeche jaati -> jo PEHLA leaf mila = sabse PAAS = min depth. first-leaf pe RUK jao.
//     (BFS ki taakat: "sabse pehle jo mila = shortest" -- poora tree dekhne ki zaroorat nahi.)
//  1. base : root null -> 0.
//  2. queue me root push. depth = 0 (ab tak poore hue level).
//  3. while (!q.empty()) -- har ghumaav = ek LEVEL:
//       sz = q.size() [snapshot] -> sz baar: front pop ->
//           ★ agar LEAF (!curr->left && !curr->right) -> return depth + 1 (current level ki depth).
//           warna bachche (L,R) queue me push.
//       level poora -> depth++.
//  ★ depth+1 kyun: depth = ABHI TAK poore hue level ki ginti; jab leaf is level me milta, wo current level = depth+1.
//  ★★ DFS-TRAP (isliye BFS chuna): "1 + min(leftDepth, rightDepth)" GALAT -- NULL-child ko depth-0 maan ke chhota answer deta
//     (skew tree 2->3->4 pe). BFS me ye dikkat hi nahi (first leaf = sahi min).
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

int minDepth(TreeNode *root)
{
    if (!root)
        return 0;

    queue<TreeNode *> q;
    q.push(root);
    int depth = 0; // ab tak kitne level POORE ho chuke (current level = depth+1)
    while (!q.empty())
    {
        int sz = q.size(); // is level ke node (level-order jaisa snapshot)
        for (int i = 0; i < sz; i++)
        {
            TreeNode *curr = q.front(); // front node nikaalo (FIFO -> jo pehle aaya)
            q.pop();

            // ★ EARLY-STOP: level-order se neeche jaate, jo PEHLA leaf mila = sabse PAAS = min depth. wahi return, aage mat dekho.
            if (!curr->left && !curr->right)
            {
                return depth + 1; // depth = poore ho chuke level; ye node current level me = depth+1
            }

            // bachche queue me daalo -> AGLE level me process honge (abhi nahi, sz snapshot tha)
            if (curr->left != NULL)
            {
                q.push(curr->left);
            }
            if (curr->right != NULL)
            {
                q.push(curr->right);
            }
        }
        depth++; // poora level ho gaya -> agle level ke liye depth badhao
    }
    return -1; // (yahan pahunchte nahi -- root hai to leaf zaroor milega)
}

int main()
{
    //     3
    //    / \
    //   9  20
    //      / \
    //     15  7
    TreeNode *a = new TreeNode(3);
    a->left = new TreeNode(9);
    a->right = new TreeNode(20);
    a->right->left = new TreeNode(15);
    a->right->right = new TreeNode(7);

    //   2
    //    \
    //     3
    //      \
    //       4
    TreeNode *b = new TreeNode(2);
    b->right = new TreeNode(3);
    b->right->right = new TreeNode(4);

    TreeNode *c = new TreeNode(1);

    cout << minDepth(a) << " (expected 2)\n";
    cout << minDepth(b) << " (expected 3)  <- DFS-trap wala\n";
    cout << minDepth(c) << " (expected 1)\n";
    cout << minDepth(nullptr) << " (expected 0)\n";
    return 0;
}
