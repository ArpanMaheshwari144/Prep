// Q: Binary tree ka preorder traversal karo — Root, Left, Right
#include <iostream>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:
    // Preorder: Root, Left, Right
    void preorder(TreeNode* root, vector<int>& result) {
        // base case — NULL hai toh wapas
        if(root == nullptr){
            return;
        }

        // pehle root store karo
        result.push_back(root->val);
        // phir left subtree
        preorder(root->left, result);
        // phir right subtree
        preorder(root->right, result);
    }

    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        preorder(root, result);
        return result;
    }
};

// --- NEECHE TOUCH MAT KAR ---
void printArr(vector<int>& v) {
    cout << "[";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i < v.size() - 1) cout << ",";
    }
    cout << "]";
}

int main() {
    Solution sol;

    TreeNode* root1 = new TreeNode(1);
    root1->left = new TreeNode(2);
    root1->right = new TreeNode(3);
    root1->left->left = new TreeNode(4);
    root1->left->right = new TreeNode(5);

    vector<int> r1 = sol.preorderTraversal(root1);
    cout << "Test 1: "; printArr(r1);
    cout << (r1.size()==5 && r1[0]==1 && r1[1]==2 && r1[2]==4 && r1[3]==5 && r1[4]==3 ? "  PASS" : "  FAIL") << endl;

    vector<int> r2 = sol.preorderTraversal(NULL);
    cout << "Test 2: "; printArr(r2);
    cout << (r2.size() == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
