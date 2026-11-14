// ============================================================
// PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// BINARY TREE ZIGZAG LEVEL ORDER TRAVERSAL — (LeetCode 103, Medium)   [TREES #11]  -- REDO 1
// ============================================================
// Tree ko level-by-level padho, par har level ki disha badalti:
// level 0 left -> right, level 1 right -> left, level 2 phir left -> right ...
// Har level ki values ek alag list me lautao.
//
//        3
//       / \          ->  [[3], [20, 9], [15, 7]]
//      9   20
//          / \
//         15  7
//
// TEST-CASES (input tree -> expected):
//   [3,9,20,null,null,15,7]       -> [[3],[20,9],[15,7]]
//   [1]                           -> [[1]]
//   []                            -> []
//   [1,2,3,4,null,null,5]         -> [[1],[3,2],[4,5]]
//   [1,2,3,4,5,6,7,8]             -> [[1],[3,2],[4,5,6,7],[8]]
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

vector<vector<int>> zigzagLevelOrder(TreeNode *root)
{
    vector<vector<int>> ans;
    if (!root)
        return ans;
    queue<TreeNode *> q;
    q.push(root);
    int levelCount = 0;
    while (!q.empty())
    {
        int sz = q.size();
        vector<int> temp;
        for (int i = 0; i < sz; i++)
        {
            TreeNode *curr = q.front();
            q.pop();
            temp.push_back(curr->val);
            if (curr->left != NULL)
            {
                q.push(curr->left);
            }
            if (curr->right != NULL)
            {
                q.push(curr->right);
            }
        }
        if (levelCount % 2 != 0)
        {
            reverse(begin(temp), end(temp));
        }
        levelCount++;
        ans.push_back(temp);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(TreeNode *root, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = zigzagLevelOrder(root);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=[";
    for (auto &lvl : got)
    {
        cout << "[";
        for (int x : lvl)
            cout << x << " ";
        cout << "]";
    }
    cout << "]\n";
}

int main()
{
    //      3
    //     / \
    //    9   20
    //        / \
    //       15  7                    -> [[3],[20,9],[15,7]]
    TreeNode *a = new TreeNode(3);
    a->left = new TreeNode(9);
    a->right = new TreeNode(20);
    a->right->left = new TreeNode(15);
    a->right->right = new TreeNode(7);

    // akela node                     -> [[1]]
    TreeNode *b = new TreeNode(1);

    // khaali tree                    -> []
    TreeNode *c = nullptr;

    //      1
    //     / \
    //    2   3
    //   /     \
    //  4       5                     -> [[1],[3,2],[4,5]]
    TreeNode *d = new TreeNode(1);
    d->left = new TreeNode(2);
    d->right = new TreeNode(3);
    d->left->left = new TreeNode(4);
    d->right->right = new TreeNode(5);

    //          1
    //        /   \
    //       2     3
    //      / \   / \
    //     4   5 6   7
    //    /
    //   8                            -> [[1],[3,2],[4,5,6,7],[8]]
    TreeNode *e = new TreeNode(1);
    e->left = new TreeNode(2);
    e->right = new TreeNode(3);
    e->left->left = new TreeNode(4);
    e->left->right = new TreeNode(5);
    e->right->left = new TreeNode(6);
    e->right->right = new TreeNode(7);
    e->left->left->left = new TreeNode(8);

    check(a, {{3}, {20, 9}, {15, 7}}, 1);
    check(b, {{1}}, 2);
    check(c, {}, 3);
    check(d, {{1}, {3, 2}, {4, 5}}, 4);
    check(e, {{1}, {3, 2}, {4, 5, 6, 7}, {8}}, 5);
    return 0;
}
