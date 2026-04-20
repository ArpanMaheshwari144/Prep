// Q: Binary tree ki maximum depth (height) nikalo
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
    // Tree ki maximum depth nikaal
    // Base case: NULL → 0
    // Recursive: 1 + max(left depth, right depth)
    int maxDepth(TreeNode* root) {
        // base case — khaali node ki depth 0
        if(!root){
            return 0;
        }

        // left subtree ki depth nikalo
        int left = maxDepth(root->left);
        // right subtree ki depth nikalo
        int right = maxDepth(root->right);

        // current node ka 1 + dono mein se badi depth
        return 1 + max(left, right);
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
    cout << "Test 1: " << sol.maxDepth(r1);
    cout << (sol.maxDepth(r1) == 3 ? "  PASS" : "  FAIL") << endl;

    // Empty
    cout << "Test 2: " << sol.maxDepth(NULL);
    cout << (sol.maxDepth(NULL) == 0 ? "  PASS" : "  FAIL") << endl;

    // Single
    cout << "Test 3: " << sol.maxDepth(new TreeNode(1));
    cout << (sol.maxDepth(new TreeNode(1)) == 1 ? "  PASS" : "  FAIL") << endl;

    //   1
    //    \
    //     2
    //      \
    //       3
    TreeNode* r4 = new TreeNode(1);
    r4->right = new TreeNode(2);
    r4->right->right = new TreeNode(3);
    cout << "Test 4: " << sol.maxDepth(r4);
    cout << (sol.maxDepth(r4) == 3 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
