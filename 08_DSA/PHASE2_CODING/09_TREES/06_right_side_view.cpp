// ============================================================
// BINARY TREE RIGHT SIDE VIEW   —   Trees (BFS twist)
// ============================================================
// Tree ke DAANYI (right) taraf khade ho ke dekho -- har level ka jo node sabse
// right pe DIKHTA hai, wahi answer mein (upar se neeche).
//
//        1            -> 1
//       / \
//      2   3          -> 3   (2 peeche chhup gaya)
//       \   \
//        5   4        -> 4   (5 chhup gaya)
//
//   answer: [1, 3, 4]
//
//   single {1}   -> [1]
//   empty {}     -> []
//   left chain {1,2,-1,3} -> [1, 2, 3]   (har level pe akela node hi right-most)
//
// SIGNAL: "har level se EK node (right se dikhne wala)" -> BFS (level-order ka twist).
//   Soch (template wahi -- #34 BFS):
//     - queue + "queue.size() = ek pura level" (same dhaancha)
//     - har level ke andar node ek-ek nikaalte ho -> us level mein se TUJHE
//       sirf wahi chahiye jo right se dikhe -> soch: ek level mein wo kaunsi POSITION pe?
//     - bachche (left phir right) hamesha queue mein daalo (taaki agla level bane),
//       bas answer mein har level se EK push karo.
//   No code. #34 wala template uthao, "har level se ek (right-most)" twist khud nikaalo.
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

// ---- TERA kaam: yeh function bhar (BFS + har level se right-most) ----
vector<int> rightSideView(TreeNode *root)
{
    if (!root)
    {
        return {};
    }

    queue<TreeNode *> q;
    vector<int> ans;
    q.push(root);

    while (!q.empty())
    {
        int size = q.size();
        for (int i = 0; i < size; i++)
        {
            TreeNode *curr = q.front();
            q.pop();
            if (i == size - 1)
                ans.push_back(curr->val);

            if (curr->left != nullptr)
            {
                q.push(curr->left);
            }
            if (curr->right != nullptr)
            {
                q.push(curr->right);
            }
        }
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

void printVec(vector<int> res)
{
    cout << "[";
    for (int i = 0; i < res.size(); i++)
    {
        cout << res[i];
        if (i + 1 < res.size())
            cout << ",";
    }
    cout << "]" << endl;
}

int main()
{
    printVec(rightSideView(build({1, 2, 3, -1, 5, -1, 4}))); // expected [1,3,4]
    printVec(rightSideView(build({1})));                     // expected [1]
    printVec(rightSideView(build({})));                      // expected []
    printVec(rightSideView(build({1, 2, -1, 3})));           // expected [1,2,3]  (left chain)
    printVec(rightSideView(build({1, 2, 3, 4})));            // expected [1,3,4]  (4 is left child of 2 -> level2 rightmost)
    return 0;
}
