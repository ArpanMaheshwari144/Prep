// Q: Binary tree ka level order traversal karo — BFS queue se
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:

    vector<vector<int>> levelOrder(TreeNode* root) {
        // khaali tree toh khaali result
        if(root == nullptr){
            return vector<vector<int>>{};
        }
        // queue mein nodes level wise process karo
        queue<TreeNode*> q;
        vector<vector<int>> ans;
        q.push(root);
        while (!q.empty())
        {
            // is level mein kitne nodes hain
            int n = q.size();
            vector<int> temp;
            // saare nodes is level ke process karo
            for(int i=0;i<n;i++){
                TreeNode* curr = q.front();
                temp.push_back(curr->val);
                q.pop();
                // left child hai toh queue mein daalo — agla level
                if(curr->left){
                    q.push(curr->left);
                }
                // right child hai toh queue mein daalo
                if(curr->right){
                    q.push(curr->right);
                }
            }
            // ye level ka result save karo
            ans.push_back(temp);
        }
        return ans;

    }
};

// --- NEECHE TOUCH MAT KAR ---
void printResult(vector<vector<int>>& res) {
    cout << "[";
    for (int i = 0; i < res.size(); i++) {
        cout << "[";
        for (int j = 0; j < res[i].size(); j++) {
            cout << res[i][j];
            if (j < res[i].size() - 1) cout << ",";
        }
        cout << "]";
        if (i < res.size() - 1) cout << ",";
    }
    cout << "]";
}

int main() {
    Solution sol;

    TreeNode* r1 = new TreeNode(1);
    r1->left = new TreeNode(2);
    r1->right = new TreeNode(3);
    r1->left->left = new TreeNode(4);
    r1->left->right = new TreeNode(5);
    r1->right->right = new TreeNode(6);

    vector<vector<int>> res1 = sol.levelOrder(r1);
    cout << "Test 1: "; printResult(res1);
    cout << (res1.size()==3 && res1[0][0]==1 && res1[1][0]==2 && res1[1][1]==3 && res1[2][0]==4 ? "  PASS" : "  FAIL") << endl;

    vector<vector<int>> res2 = sol.levelOrder(NULL);
    cout << "Test 2: "; printResult(res2);
    cout << (res2.size() == 0 ? "  PASS" : "  FAIL") << endl;

    vector<vector<int>> res3 = sol.levelOrder(new TreeNode(1));
    cout << "Test 3: "; printResult(res3);
    cout << (res3.size()==1 && res3[0][0]==1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
