// Q: Binary tree mein do nodes ka Lowest Common Ancestor (LCA) dhundho
#include <iostream>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:

    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // base case — NULL toh NULL return
        if(root == nullptr){
            return nullptr;
        }

        // agar current node p ya q hai toh ye khud ancestor hai
        if(root == p || root == q){
            return root;
        }

        // left aur right subtree mein dhundho
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        // dono side se mila — current node hi LCA hai
        if(left != nullptr && right != nullptr){
            if(left->val == p->val && right->val == q->val){
                return root;
            }
        }

        // sirf right side mein mila
        if(left == nullptr){
            return right;
        }
        // sirf left side mein mila
        if(right == nullptr){
            return left;
        }
        return root;
    }
};

int main() {
    //        3
    //       / \
    //      5   1
    //     / \ / \
    //    6  2 0  8
    //      / \
    //     7   4
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(5);
    root->right = new TreeNode(1);
    root->left->left = new TreeNode(6);
    root->left->right = new TreeNode(2);
    root->right->left = new TreeNode(0);
    root->right->right = new TreeNode(8);
    root->left->right->left = new TreeNode(7);
    root->left->right->right = new TreeNode(4);

    Solution sol;

    TreeNode* r1 = sol.lowestCommonAncestor(root, root->left, root->right);
    cout << "Test 1: LCA(5,1) → " << r1->val;
    cout << (r1->val == 3 ? "  PASS" : "  FAIL") << endl;

    TreeNode* r2 = sol.lowestCommonAncestor(root, root->left, root->left->right->right);
    cout << "Test 2: LCA(5,4) → " << r2->val;
    cout << (r2->val == 5 ? "  PASS" : "  FAIL") << endl;

    TreeNode* r3 = sol.lowestCommonAncestor(root, root->left->left, root->left->right->right);
    cout << "Test 3: LCA(6,4) → " << r3->val;
    cout << (r3->val == 5 ? "  PASS" : "  FAIL") << endl;

    TreeNode* r4 = sol.lowestCommonAncestor(root, root->left->right->left, root->right->right);
    cout << "Test 4: LCA(7,8) → " << r4->val;
    cout << (r4->val == 3 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
