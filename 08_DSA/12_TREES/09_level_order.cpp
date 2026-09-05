// ============================================================
// BINARY TREE LEVEL ORDER TRAVERSAL — (LeetCode 102, Medium)   [TREES #9]
// ============================================================
// root diya. har LEVEL ke nodes, LEFT-to-RIGHT, alag-alag list me lautao.
// (yaani level 0 ke sab node ek list, level 1 ke ek list, ... => list of lists)
//
//        3
//       / \
//      9   20
//          / \
//         15  7
//
//   [3,9,20,null,null,15,7]  ->  [[3], [9,20], [15,7]]
//   [1]                      ->  [[1]]
//   []  (null root)          ->  []
// ============================================================
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

vector<vector<int>> levelOrder(TreeNode *root)
{
    vector<vector<int>> ans;

    if (!root)
        return ans;
    
    queue<TreeNode *> q;
    q.push(root); // level-0 ka node (root) queue me daala -- yahin se BFS shuru
    while (!q.empty())
    {
        int sz = q.size(); // SNAPSHOT: abhi queue me jitne node = is level ke saare (for-loop se pehle pakda)
        vector<int> temp;  // is level ka apna answer collect karne ko
        for (int i = 0; i < sz; i++)
        {
            TreeNode *curr = q.front(); // front node nikaalo (FIFO -> jo pehle aaya)
            q.pop();
            temp.push_back(curr->val);  // is level ki list me daalo
            // bachche queue me daalo -> ye AGLE level me process honge (abhi nahi, kyunki sz snapshot tha)
            if (curr->left != NULL)
            {
                q.push(curr->left);
            }
            if (curr->right != NULL)
            {
                q.push(curr->right);
            }
        }
        ans.push_back(temp);
    }
    return ans;
}

// ---- test helper (print list-of-lists) -- mat chhed ----
void printLL(vector<vector<int>> &res)
{
    cout << "[";
    for (int i = 0; i < (int)res.size(); i++)
    {
        cout << "[";
        for (int j = 0; j < (int)res[i].size(); j++)
        {
            cout << res[i][j];
            if (j + 1 < (int)res[i].size())
                cout << ",";
        }
        cout << "]";
        if (i + 1 < (int)res.size())
            cout << ", ";
    }
    cout << "]\n";
}

int main()
{
    //        3
    //       / \
    //      9   20
    //          / \
    //         15  7
    TreeNode *a = new TreeNode(3);
    a->left = new TreeNode(9);
    a->right = new TreeNode(20);
    a->right->left = new TreeNode(15);
    a->right->right = new TreeNode(7);

    TreeNode *b = new TreeNode(1);

    TreeNode *c = nullptr;

    vector<vector<int>> r1 = levelOrder(a);
    vector<vector<int>> r2 = levelOrder(b);
    vector<vector<int>> r3 = levelOrder(c);

    printLL(r1); // expected [[3], [9,20], [15,7]]
    printLL(r2); // expected [[1]]
    printLL(r3); // expected []
    return 0;
}
