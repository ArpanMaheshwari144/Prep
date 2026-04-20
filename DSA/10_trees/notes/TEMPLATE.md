# Template: Trees

## Traversals (teeno same — sirf order badal)

```
void traverse(TreeNode* root, vector<int>& result) {
    if(root == NULL) return;           // BASE CASE — hamesha!

    // INORDER:   Left, Root, Right
    traverse(root->left, result);
    result.push_back(root->val);
    traverse(root->right, result);

    // PREORDER:  Root, Left, Right
    result.push_back(root->val);
    traverse(root->left, result);
    traverse(root->right, result);

    // POSTORDER: Left, Right, Root
    traverse(root->left, result);
    traverse(root->right, result);
    result.push_back(root->val);
}
```

**3 lines ka order badlo — bas itna hi fark hai teeno mein.**
**BASE CASE bhoolna = CRASH.**

## Max Depth
```
int maxDepth(TreeNode* root) {
    if(!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
```

## Balanced Tree
```
isBalanced(root):
    if NULL → return true
    if !isBalanced(left) || !isBalanced(right) → return false
    abs(maxDepth(left) - maxDepth(right)) > 1 → return false
    return true
```

## Level Order (BFS) — Queue
```
Queue mein root daal.
while queue khaali nahi:
    int size = q.size()        ← PEHLE size save!
    for(i = 0 to size):
        node nikaal (front + pop)
        val temp mein daal
        left hai toh push
        right hai toh push
    result mein temp daal

Base case: root NULL → return empty
```
