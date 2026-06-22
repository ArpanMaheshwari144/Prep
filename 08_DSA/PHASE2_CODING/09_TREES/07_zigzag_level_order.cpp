// ============================================================
// ZIGZAG LEVEL ORDER TRAVERSAL   —   Trees (BFS twist)
// ============================================================
// Level-order jaise hi, PAR alternate level ULTA:
//   level 0 -> L se R, level 1 -> R se L, level 2 -> L se R, ... (zigzag)
//
//        1
//       / \
//      2   3
//     / \   \
//    4   5   6
//
//   Level 0 (L->R):  [1]
//   Level 1 (R->L):  [3, 2]     <- ulta
//   Level 2 (L->R):  [4, 5, 6]
//   answer: [[1],[3,2],[4,5,6]]
//
//   single {1}   -> [[1]]
//   empty {}     -> []
//
// SIGNAL: "level by level + alternate ULTA / zigzag" -> BFS (#34 template) + alternate-reverse twist.
//   Soch (template wahi):
//     - queue se HAMESHA normal order (L->R) mein node nikaalo -- queue ka order MAT chhedo
//     - ek flag/counter rakho: "yeh level ulta hai kya?" -> har level pe flip
//     - sirf us LEVEL ki list ko reverse karo (ya odd level pe ulta bharo), queue ko nahi
//     - even level seedha, odd level ulta
//   No code. #34 ka template uthao, ek "leftToRight" flag add karo jo har level toggle ho.
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

// ---- TERA kaam: yeh function bhar (BFS + alternate reverse) ----
vector<vector<int>> zigzagLevelOrder(TreeNode *root)
{
    if (!root)
    {
        return {};
    }

    queue<TreeNode *> q;
    vector<vector<int>> ans;
    q.push(root);
    bool flag = true;
    while (!q.empty())
    {
        int size = q.size();
        vector<int> v;
        for (int i = 0; i < size; i++)
        {
            TreeNode *curr = q.front();
            q.pop();

            // int index = flag ? i : size - i - 1;
            // v[index] = curr->val;

            v.push_back(curr->val);

            if (curr->left != nullptr)
            {
                q.push(curr->left);
            }
            if (curr->right != nullptr)
            {
                q.push(curr->right);
            }
        }
        if (flag == false)
        {
            reverse(begin(v), end(v));
        }
        flag = !flag;
        ans.push_back(v);
    }
    return ans;
}

// ---- helpers (mat chhu) ----
TreeNode *build(vector<int> v)
{
    if (v.empty() || v[0] == -1)
        return nullptr;
    TreeNode *root = new TreeNode(v[0]);
    queue<TreeNode *> q;
    q.push(root);
    int i = 1;
    while (!q.empty() && i < v.size())
    {
        TreeNode *node = q.front();
        q.pop();
        if (i < v.size() && v[i] != -1)
        {
            node->left = new TreeNode(v[i]);
            q.push(node->left);
        }
        i++;
        if (i < v.size() && v[i] != -1)
        {
            node->right = new TreeNode(v[i]);
            q.push(node->right);
        }
        i++;
    }
    return root;
}

void printLevels(vector<vector<int>> res)
{
    cout << "[";
    for (int i = 0; i < res.size(); i++)
    {
        cout << "[";
        for (int j = 0; j < res[i].size(); j++)
        {
            cout << res[i][j];
            if (j + 1 < res[i].size())
                cout << ",";
        }
        cout << "]";
        if (i + 1 < res.size())
            cout << ",";
    }
    cout << "]" << endl;
}

int main()
{
    printLevels(zigzagLevelOrder(build({1, 2, 3, 4, 5, -1, 6}))); // expected [[1],[3,2],[4,5,6]]
    printLevels(zigzagLevelOrder(build({1})));                    // expected [[1]]
    printLevels(zigzagLevelOrder(build({})));                     // expected []
    printLevels(zigzagLevelOrder(build({1, 2, 3, 4, 5, 6, 7})));  // expected [[1],[3,2],[4,5,6,7]]
    printLevels(zigzagLevelOrder(build({1, 2, -1, 3, -1, 4})));   // expected [[1],[2],[3],[4]] (single-node levels)
    return 0;
}
