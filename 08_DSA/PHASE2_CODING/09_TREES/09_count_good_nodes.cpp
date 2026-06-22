// ============================================================
// COUNT GOOD NODES IN BINARY TREE        [ COLD TEST -- pattern KHUD pehchaan ]
// ============================================================
// Ek node "GOOD" hai agar root se us node tak ke raaste mein koi node uss se BADA nahi.
// (root hamesha good.) Kitne good nodes hain -> count.
//
//        3
//       / \
//      1   4
//     /   / \
//    3   1   5
//
//   good = 4   (3-root, 3-under-1, 4, 5)
//
//   single {1}   -> 1
//   empty {}     -> 0
//
// >> COLD TEST: koi signal/approach NAHI diya. Pattern khud pehchaan, khud derive.
//    Soch: har node tak pahunchte waqt tujhe kya "yaad" chahiye us node ko judge karne ko?
//    (bas itna ishaara -- baaki tera.) No peeking, no leetcode.
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

// ---- TERA kaam: yeh function bhar (helper bana sakta ho) ----
int goodNodes(TreeNode *root)
{
    if (!root)
    {
        return 0;
    }

    if (!root)
    {
        return {};
    }

    queue<pair<TreeNode *, int>> q;
    int count = 0;
    q.push({root, root->val});

    while (!q.empty())
    {
        TreeNode *curr = q.front().first;
        int maxi = q.front().second;
        q.pop();

        if(curr->val >= maxi){
            count++;
        }

        if (curr->left != nullptr)
        {
            q.push({curr->left, curr->val});
        }
        if (curr->right != nullptr)
        {
            q.push({curr->right, curr->val});
        }
    }
    return count;
}

// ---- helpers (mat chhu) ----
TreeNode *build(vector<int> v)
{
    if (v.empty() || v[0] == -1)
        return nullptr;
    TreeNode *root = new TreeNode(v[0]);
    queue<TreeNode *> bq;
    bq.push(root);
    int i = 1;
    while (!bq.empty() && i < v.size())
    {
        TreeNode *node = bq.front();
        bq.pop();
        if (i < v.size() && v[i] != -1)
        {
            node->left = new TreeNode(v[i]);
            bq.push(node->left);
        }
        i++;
        if (i < v.size() && v[i] != -1)
        {
            node->right = new TreeNode(v[i]);
            bq.push(node->right);
        }
        i++;
    }
    return root;
}

int main()
{
    cout << goodNodes(build({3, 1, 4, 3, -1, 1, 5})) << endl; // expected 4
    cout << goodNodes(build({3, 3, -1, 4, 2})) << endl;       // expected 3
    cout << goodNodes(build({1})) << endl;                    // expected 1
    cout << goodNodes(build({})) << endl;                     // expected 0
    cout << goodNodes(build({1, 2, 3})) << endl;              // expected 3 (1 root, 2>=1, 3>=1)
    cout << goodNodes(build({2, -1, 1})) << endl;             // expected 1 (root only; 1 < 2)
    return 0;
}
