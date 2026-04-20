// Q: Graph ka BFS traversal karo — Breadth First Search queue se
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
class Solution {
public:
    // Graph ka BFS traversal — Queue use kar
    // visited track karo
    vector<int> bfsTraversal(int n, vector<vector<int>>& adj) {
        vector<int> ans;
        vector<int> vis(n, false);
        queue<int> q;
        // node 0 se start — visit mark karo aur queue mein daalo
        vis[0] = true;
        q.push(0);

        while(!q.empty()){
            // is level ke saare nodes process karo
            int n = q.size();
            for(int i=0;i<n;i++){
                // queue se nikalo aur result mein daalo
                int node = q.front();
                q.pop();
                ans.push_back(node);
                // saare unvisited neighbours queue mein daalo
                for(auto &it : adj[node]){
                    if(!vis[it]){
                        vis[it] = true;
                        q.push(it);
                    }
                }
            }
        }
        return ans;
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
    vector<int> r1 = sol.bfsTraversal(4, adj1);
    cout << "Test 1: "; printArr(r1);
    cout << (r1.size() == 4 ? "  PASS (4 nodes)" : "  FAIL") << endl;

    // Disconnected
    vector<vector<int>> adj2 = {{1}, {0}, {3}, {2}};
    vector<int> r2 = sol.bfsTraversal(4, adj2);
    cout << "Test 2: "; printArr(r2);
    cout << (r2.size() == 2 ? "  PASS (2 nodes from 0)" : "  FAIL") << endl;

    // Single
    vector<vector<int>> adj3 = {{}};
    vector<int> r3 = sol.bfsTraversal(1, adj3);
    cout << "Test 3: "; printArr(r3);
    cout << (r3.size() == 1 && r3[0] == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
