// Q: Binary tree ka postorder traversal karo — Left, Right, Root
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
    // Postorder: Left, Right, Root
    void postorder(TreeNode* root, vector<int>& result) {
        // base case — NULL hai toh wapas
        if(root == nullptr){
            return;
        }

        // pehle left subtree
        postorder(root->left, result);
        // phir right subtree
        postorder(root->right, result);
        // sabse last mein root store karo
        result.push_back(root->val);
    }

    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        postorder(root, result);
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

    vector<int> r1 = sol.postorderTraversal(root1);
    cout << "Test 1: "; printArr(r1);
    cout << (r1.size()==5 && r1[0]==4 && r1[1]==5 && r1[2]==2 && r1[3]==3 && r1[4]==1 ? "  PASS" : "  FAIL") << endl;

    vector<int> r2 = sol.postorderTraversal(NULL);
    cout << "Test 2: "; printArr(r2);
    cout << (r2.size() == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
