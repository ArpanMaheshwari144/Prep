// Q: Binary tree balanced hai ya nahi check karo — har node pe depth diff <= 1
#include <iostream>
#include <cmath>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:
    // Tree balanced hai ya nahi?
    // Har node pe left aur right depth ka fark <= 1 hona chahiye
    // Hint: maxDepth wala code reuse kar

    int maxDepth(TreeNode* root) {
        if(!root){
            return 0;
        }
        int left = maxDepth(root->left);
        int right = maxDepth(root->right);
        return 1 + max(left, right);
    }

    bool isBalanced(TreeNode* root) {
        // base case — khaali tree balanced hai
        if(root == nullptr){
            return true;
        }

        int leftDepth = 0;
        int rightDepth = 0;

        // pehle check karo dono subtrees balanced hain
        if(isBalanced(root->left) && isBalanced(root->right)){
            // phir dono ki depth nikalo
            leftDepth = maxDepth(root->left);
            rightDepth = maxDepth(root->right);

            // fark 1 se zyada — balanced nahi
            if(abs(leftDepth - rightDepth) > 1){
                return false;
            }
        }
        else{
            // koi subtree balanced nahi toh ye bhi nahi
            return false;
        }
        // sab theek — balanced hai
        return true;
    }
};

int main() {
    Solution sol;

    //     1
    //    / \
    //   2   3
    //  / \
    // 4   5
    TreeNode* r1 = new TreeNode(1);
    r1->left = new TreeNode(2);
    r1->right = new TreeNode(3);
    r1->left->left = new TreeNode(4);
    r1->left->right = new TreeNode(5);
    cout << "Test 1: " << (sol.isBalanced(r1) ? "true" : "false");
    cout << (sol.isBalanced(r1) == true ? "  PASS" : "  FAIL") << endl;

    // Not balanced
    //   1
    //    \
    //     2
    //      \
    //       3
    TreeNode* r2 = new TreeNode(1);
    r2->right = new TreeNode(2);
    r2->right->right = new TreeNode(3);
    cout << "Test 2: " << (sol.isBalanced(r2) ? "true" : "false");
    cout << (sol.isBalanced(r2) == false ? "  PASS" : "  FAIL") << endl;

    // Empty
    cout << "Test 3: " << (sol.isBalanced(NULL) ? "true" : "false");
    cout << (sol.isBalanced(NULL) == true ? "  PASS" : "  FAIL") << endl;

    // Single
    cout << "Test 4: " << (sol.isBalanced(new TreeNode(1)) ? "true" : "false");
    cout << (sol.isBalanced(new TreeNode(1)) == true ? "  PASS" : "  FAIL") << endl;

    // Not balanced deep
    //       1
    //      /
    //     2
    //    /
    //   3
    //  /
    // 4
    TreeNode* r5 = new TreeNode(1);
    r5->left = new TreeNode(2);
    r5->left->left = new TreeNode(3);
    r5->left->left->left = new TreeNode(4);
    cout << "Test 5: " << (sol.isBalanced(r5) ? "true" : "false");
    cout << (sol.isBalanced(r5) == false ? "  PASS" : "  FAIL") << endl;

    return 0;
}
