// ============================================================
// BINARY TREE RIGHT SIDE VIEW — (LeetCode 199, Medium)   [TREES #10]
// ============================================================
// tree ke DAAYE (right) side khade ho -> upar se neeche jo node DIKHTE hain wo lautao.
// (yaani har LEVEL ka SABSE RIGHT wala node.)
//
//        1            <-  1 dikhta
//       / \
//      2   3          <-  3 dikhta (2 peeche chhup gaya)
//       \   \
//        5   4        <-  4 dikhta
//
//   [1,2,3,null,5,null,4]  ->  [1, 3, 4]
//   [1,null,3]             ->  [1, 3]
//   [1,2,3,4]              ->  [1, 3, 4]     (4 level-2 me akela right)
//   []  (null root)        ->  []
// ============================================================
// ---- APPROACH ----  (= LEVEL-ORDER BFS ka NEAR-TRANSFER; sirf "level me kya karna" badla)
//  same BFS skeleton (queue + sz-SNAPSHOT), par har level se sirf RIGHT-MOST node chahiye.
//  1. base : root null -> empty {}.
//  2. queue me root push. while(!q.empty):
//       sz = q.size()  [snapshot -- is level ke node].
//       for i = 0..sz-1: front pop -> ★ if(i == sz-1) ans me daalo -> bachche(L,R) push.
//  ★ WHY i==sz-1: FIFO me level LEFT-to-RIGHT dequeue hota -> aakhri (i=sz-1) = sabse right = daaye se dikhne wala.
//  ★ 0-based: sz node -> index 0..sz-1 -> last = sz-1.  (left-side-view chahiye to i==0 le lena.)
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

vector<int> rightSideView(TreeNode *root)
{
    if (!root)
        return {};
    
    queue<TreeNode *> q;
    q.push(root); // level-0 ka node (root) queue me daala -- yahin se BFS shuru
    vector<int> ans;
    while (!q.empty())
    {
        int sz = q.size(); // ★ SNAPSHOT: abhi queue me jitne node = is level ke saare (for-loop se pehle pakda)
        for (int i = 0; i < sz; i++)
        {
            TreeNode *curr = q.front(); // front node nikaalo (FIFO -> jo pehle aaya)
            q.pop();

            // ★ har level ka LAST (right-most) node chahiye. for-loop i: 0 -> sz-1 me nodes LEFT-to-RIGHT nikalte
            //   -> aakhri wala (i == sz-1) = is level ka sabse RIGHT = daaye se jo dikhta.
            //   (0-based: sz node -> index 0..sz-1 -> last index = sz-1)
            if (i == sz - 1)
            {
                ans.push_back(curr->val);
            }

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
    }
    return ans;
}

// ---- test helper (print vector) -- mat chhed ----
void printVec(vector<int> &v)
{
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++)
    {
        cout << v[i];
        if (i + 1 < (int)v.size())
            cout << ", ";
    }
    cout << "]\n";
}

int main()
{
    //        1
    //       / \
    //      2   3
    //       \   \
    //        5   4
    TreeNode *a = new TreeNode(1);
    a->left = new TreeNode(2);
    a->right = new TreeNode(3);
    a->left->right = new TreeNode(5);
    a->right->right = new TreeNode(4);

    //   1
    //    \
    //     3
    TreeNode *b = new TreeNode(1);
    b->right = new TreeNode(3);

    //        1
    //       / \
    //      2   3
    //     /
    //    4
    TreeNode *c = new TreeNode(1);
    c->left = new TreeNode(2);
    c->right = new TreeNode(3);
    c->left->left = new TreeNode(4);

    TreeNode *d = nullptr;

    vector<int> r1 = rightSideView(a);
    vector<int> r2 = rightSideView(b);
    vector<int> r3 = rightSideView(c);
    vector<int> r4 = rightSideView(d);

    printVec(r1); // expected [1, 3, 4]
    printVec(r2); // expected [1, 3]
    printVec(r3); // expected [1, 3, 4]
    printVec(r4); // expected []
    return 0;
}
