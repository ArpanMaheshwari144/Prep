// Q: Binary tree ka inorder traversal karo — Left, Root, Right
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
    // Inorder traversal: Left, Root, Right
    void inorder(TreeNode* root, vector<int>& result) {
        // base case — NULL hai toh wapas jao
        if(root == nullptr){
            return;
        }

        // pehle left subtree jao
        inorder(root->left, result);
        // phir root ko store karo
        result.push_back(root->val);
        // phir right subtree jao
        inorder(root->right, result);
    }

    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        inorder(root, result);
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

    //     1
    //    / \
    //   2   3
    //  / \
    // 4   5
    TreeNode* root1 = new TreeNode(1);
    root1->left = new TreeNode(2);
    root1->right = new TreeNode(3);
    root1->left->left = new TreeNode(4);
    root1->left->right = new TreeNode(5);

    vector<int> r1 = sol.inorderTraversal(root1);
    cout << "Test 1: "; printArr(r1);
    cout << (r1.size() == 5 && r1[0]==4 && r1[1]==2 && r1[2]==5 && r1[3]==1 && r1[4]==3 ? "  PASS" : "  FAIL") << endl;

    // Empty tree
    vector<int> r2 = sol.inorderTraversal(NULL);
    cout << "Test 2: "; printArr(r2);
    cout << (r2.size() == 0 ? "  PASS" : "  FAIL") << endl;

    // Single node
    TreeNode* root3 = new TreeNode(1);
    vector<int> r3 = sol.inorderTraversal(root3);
    cout << "Test 3: "; printArr(r3);
    cout << (r3.size() == 1 && r3[0] == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
