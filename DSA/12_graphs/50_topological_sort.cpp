// Q: Directed Acyclic Graph (DAG) ka topological sort karo — Kahn's BFS algo
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
class Solution {
public:
    vector<int> topoSort(int n, vector<vector<int>>& adj) {
        vector<int> ans;
        vector<int> vis(n, false);
        queue<int> q;

        // har node ka indegree calculate karo — kitne edges aa rahe hain
        vector<int> indegree(n, 0);
        for(int i=0;i<n;i++){
            for(auto &it : adj[i]){
                indegree[it]++;
            }
        }

        // jinke indegree 0 hai unhe queue mein daalo — ye pehle aayenge
        for(int i=0;i<n;i++){
            if(indegree[i] == 0){
                q.push(i);
                vis[i] = true;
            }
        }

        while(!q.empty()){
            int n = q.size();
            for(int i=0;i<n;i++){
                // queue se nikalo aur result mein daalo
                int node = q.front();
                q.pop();
                ans.push_back(node);
                // iske neighbours ka indegree kam karo
                for(auto &it : adj[node]){
                    if(!vis[it]){
                        indegree[it]--;
                        // indegree 0 ho gaya — ab ye bhi ready hai
                        if(indegree[it] == 0){
                            vis[it] = true;
                            q.push(it);
                        }
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

    // 0 → 1 → 3
    // 0 → 2 → 3
    vector<vector<int>> adj1 = {{1,2}, {3}, {3}, {}};
    vector<int> r1 = sol.topoSort(4, adj1);
    cout << "Test 1: "; printArr(r1);
    cout << (r1.size() == 4 && r1[0] == 0 ? "  PASS" : "  FAIL") << endl;

    // 5 → 0, 5 → 2, 4 → 0, 4 → 1, 2 → 3, 3 → 1
    vector<vector<int>> adj2(6);
    adj2[5] = {0, 2};
    adj2[4] = {0, 1};
    adj2[2] = {3};
    adj2[3] = {1};
    vector<int> r2 = sol.topoSort(6, adj2);
    cout << "Test 2: "; printArr(r2);
    cout << (r2.size() == 6 ? "  PASS (6 nodes)" : "  FAIL") << endl;

    // Single node
    vector<vector<int>> adj3 = {{}};
    vector<int> r3 = sol.topoSort(1, adj3);
    cout << "Test 3: "; printArr(r3);
    cout << (r3.size() == 1 && r3[0] == 0 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
