// ============================================================
// LEVEL ORDER TRAVERSAL  (BFS)   —   Trees
// ============================================================
// Tree ko LEVEL BY LEVEL padho (upar se neeche), har level ki values ek alag list.
//
//        1
//       / \
//      2   3
//       \   \
//        5   6     -> [[1],[2,3],[5,6]]
//
//   single node {1}      -> [[1]]
//   empty {}             -> []   (khaali — koi level nahi)
//
// SIGNAL: "level by level / level-order / har level alag" -> BFS -> QUEUE.
//   Soch (template — code tera):
//     - QUEUE = line, FIFO (jo pehle aaya wo pehle nikla)
//     - root ko queue mein daalo
//     - jab tak queue khaali na ho:
//         * ABHI queue mein jitne node hain = POORA ek level  (yahi BFS ka dil)
//         * utne ko ek-ek nikaalo (front), unki value is level ki list mein
//         * har nikle node ke left/right (jo null nahi) ko queue ke PEECHE daalo
//         * level ki list ko answer mein push
//   No code. Copy pe tree + queue chala -- ek level nikaalte hue agla level peeche bharta ja.
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

// ---- TERA kaam: yeh function bhar (BFS + queue) ----
vector<vector<int>> levelOrder(TreeNode *root)
{
    if (!root)
    {
        return {};
    }

    queue<TreeNode *> q;
    vector<vector<int>> ans;
    q.push(root);

    while (!q.empty())
    {
        int size = q.size();
        vector<int> v;
        for (int i = 0; i < size; i++)
        {
            TreeNode *curr = q.front();
            q.pop();
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
    printLevels(levelOrder(build({1, 2, 3, -1, 5, -1, 6}))); // expected [[1],[2,3],[5,6]]
    printLevels(levelOrder(build({1})));                     // expected [[1]]
    printLevels(levelOrder(build({})));                      // expected []
    printLevels(levelOrder(build({1, 2, 3, 4, 5, 6, 7})));   // expected [[1],[2,3],[4,5,6,7]]
    printLevels(levelOrder(build({1, 2, -1, 3, -1, 4})));    // expected [[1],[2],[3],[4]]  (left chain)
    return 0;
}
