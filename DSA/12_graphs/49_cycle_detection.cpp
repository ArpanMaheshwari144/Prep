// Q: Undirected graph mein cycle hai ya nahi — DFS se detect karo
#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
    // DFS se cycle detect kar
    // Visited hai aur parent nahi → cycle!
    bool dfs(int node, int parent, vector<vector<int>>& adj, vector<bool>& visited) {
        // current node visit karo
        visited[node] = true;
        bool isCycle = false;

        // saare neighbours check karo
        for(auto &it : adj[node]){
            // unvisited hai aur parent nahi — aage jao DFS se
            if(!visited[it] and it != parent){
                if(dfs(it, node, adj, visited) == true){
                    isCycle = true;
                }
            }
            // visited hai par parent nahi — matlab cycle mil gayi!
            else if(visited[it] == true and it != parent){
                isCycle = true;
            }
        }
        return isCycle;
    }

    bool hasCycle(int n, vector<vector<int>>& adj) {
        vector<bool> visited(n, false);
        // Disconnected graph ke liye har node se check karo
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                if (dfs(i, -1, adj, visited)) return true;
            }
        }
        // koi cycle nahi mili
        return false;
    }
};

int main() {
    Solution sol;

    // Cycle: 0-1-2-3-0
    vector<vector<int>> adj1 = {{1,3}, {0,2}, {1,3}, {0,2}};
    cout << "Test 1: " << (sol.hasCycle(4, adj1) ? "true" : "false");
    cout << (sol.hasCycle(4, adj1) == true ? "  PASS" : "  FAIL") << endl;

    // No cycle: 0-1-2
    vector<vector<int>> adj2 = {{1}, {0,2}, {1}};
    cout << "Test 2: " << (sol.hasCycle(3, adj2) ? "true" : "false");
    cout << (sol.hasCycle(3, adj2) == false ? "  PASS" : "  FAIL") << endl;

    // Single node
    vector<vector<int>> adj3 = {{}};
    cout << "Test 3: " << (sol.hasCycle(1, adj3) ? "true" : "false");
    cout << (sol.hasCycle(1, adj3) == false ? "  PASS" : "  FAIL") << endl;

    // Disconnected, one component has cycle
    // 0-1-2-0 (cycle)  3-4 (no cycle)
    vector<vector<int>> adj4 = {{1,2}, {0,2}, {0,1}, {4}, {3}};
    cout << "Test 4: " << (sol.hasCycle(5, adj4) ? "true" : "false");
    cout << (sol.hasCycle(5, adj4) == true ? "  PASS" : "  FAIL") << endl;

    return 0;
}
