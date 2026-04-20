// Q: Graph ka DFS traversal karo — Depth First Search
#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
    // Graph ka DFS traversal
    // visited track karo — cycle se bachne ke liye
    void dfs(int node, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& result) {
        // current node visit kar liya — mark karo
        visited[node] = true;
        result.push_back(node);

        // saare neighbours pe jao — jo visit nahi hua
        for(auto &it : adj[node]){
            if(!visited[it]){
                dfs(it, adj, visited, result);
            }
        }
    }

    vector<int> dfsTraversal(int n, vector<vector<int>>& adj) {
        vector<bool> visited(n, false);
        vector<int> result;
        // node 0 se DFS start karo
        dfs(0, adj, visited, result);
        return result;
    }
};

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

    // 0 --- 1
    // |     |
    // 3 --- 2
    vector<vector<int>> adj1 = {{1,3}, {0,2}, {1,3}, {0,2}};
    vector<int> r1 = sol.dfsTraversal(4, adj1);
    cout << "Test 1: "; printArr(r1);
    cout << (r1.size() == 4 ? "  PASS (4 nodes visited)" : "  FAIL") << endl;

    // 0 --- 1    2 --- 3
    // (disconnected — sirf 0 se connected wale aayenge)
    vector<vector<int>> adj2 = {{1}, {0}, {3}, {2}};
    vector<int> r2 = sol.dfsTraversal(4, adj2);
    cout << "Test 2: "; printArr(r2);
    cout << (r2.size() == 2 ? "  PASS (2 nodes from 0)" : "  FAIL") << endl;

    // Single node
    vector<vector<int>> adj3 = {{}};
    vector<int> r3 = sol.dfsTraversal(1, adj3);
    cout << "Test 3: "; printArr(r3);
    cout << (r3.size() == 1 && r3[0] == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
