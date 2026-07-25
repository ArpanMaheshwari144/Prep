// ============================================================
// BINARY TREE ZIGZAG LEVEL ORDER TRAVERSAL — (LeetCode 103, Medium)   [TREES #11]
// ============================================================
// level-order jaisa -- par direction ZIGZAG (alternate):
//   level 0 -> LEFT-to-RIGHT
//   level 1 -> RIGHT-to-LEFT
//   level 2 -> LEFT-to-RIGHT ... (aise alternate)
//
//        3
//       / \
//      9   20
//          / \
//         15  7
//
//   level 0: [3]           (L->R)
//   level 1: [9,20] ULTA = [20,9]   (R->L)
//   level 2: [15,7]        (L->R)
//   =>  [[3], [20,9], [15,7]]
//
//   [3,9,20,null,null,15,7]  ->  [[3], [20,9], [15,7]]
//   [1]                      ->  [[1]]
//   []  (null root)          ->  []
// ============================================================
// ---- APPROACH ----  (= LEVEL-ORDER BFS + level-counter; sirf ODD level ULTA)
//  same BFS skeleton (queue + sz-SNAPSHOT). extra = ek levelCounter.
//  1. base : root null -> empty.
//  2. queue me root push. levelCounter = 0. while(!q.empty):
//       sz snapshot -> for sz baar { front pop -> val temp me -> bachche(L,R) push }.  (level HAMESHA L-to-R banao)
//       ★ if (levelCounter % 2 != 0) reverse(temp);   (ODD level -> ULTA = R-to-L)
//       levelCounter++.  temp -> ans.
//  ★ TRICK: level normal (L-to-R) banao har baar; bas ODD levels pe REVERSE -> zigzag mil jaata.
//     even (0,2,4..) = L-to-R seedha · odd (1,3..) = R-to-L (reversed).
//  (alt: reverse ke bajaye deque/front-insert bhi -- par reverse simplest + saaf.)
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
    q.push(root); // level-0 ka node (root) queue me daala -- yahin se BFS shuru
    int levelCounter = 0;
    while (!q.empty())
    {
        int sz = q.size(); // ★ SNAPSHOT: abhi queue me jitne node = is level ke saare (for-loop se pehle pakda)
        vector<int> temp;  // is level ka apna answer collect karne ko
        for (int i = 0; i < sz; i++)
        {
            TreeNode *curr = q.front(); // front node nikaalo (FIFO -> jo pehle aaya)
            q.pop();
            temp.push_back(curr->val); // is level ki list me daalo
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
        if (levelCounter % 2 != 0)
        {
            reverse(begin(temp), end(temp));
        }
        levelCounter++;
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

    vector<vector<int>> r1 = zigzagLevelOrder(a);
    vector<vector<int>> r2 = zigzagLevelOrder(b);
    vector<vector<int>> r3 = zigzagLevelOrder(c);

    printLL(r1); // expected [[3], [20,9], [15,7]]
    printLL(r2); // expected [[1]]
    printLL(r3); // expected []
    return 0;
}
