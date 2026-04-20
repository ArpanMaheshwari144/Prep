// Q: 2D grid mein kitne islands hain — DFS se connected 1s count karo
#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
    // charon directions — neeche, right, upar, left
    vector<vector<int>> dirs{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    // check karo — ye cell visit karne layak hai ya nahi
    bool canVis(int i, int j, vector<vector<char>>& grid, vector<vector<int>> &vis, int m, int n){
        if(i >= 0 && j >= 0 && i < m && j < n && grid[i][j] == '1' && vis[i][j] == 0){
            return true;
        }
        return false;
    }

    // DFS — ek island ke saare connected 1s visit karo
    void dfs(vector<vector<char>>& grid, vector<vector<int>> &vis, int i, int j, int m, int n) {
       // current cell visit mark karo
       vis[i][j] = 1;

       // charon directions mein jao
       for(auto& it : dirs){
        int i_ = i + it[0];
        int j_ = j + it[1];
        if(canVis(i_, j_, grid, vis, m, n)){
            dfs(grid, vis, i_, j_, m, n);
        }
       }
    }

    // Grid mein kitne islands hain?
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> vis(m,vector<int>(n, 0));
        int count = 0;
        // har cell pe jao — naya unvisited '1' mila toh naya island
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(grid[i][j] == '1' && vis[i][j] == 0){
                    // naya island mila — count badhao aur DFS se poora island visit karo
                    count++;
                    dfs(grid, vis, i, j, m, n);
                }
            }
        }
        return count;
    }
};

int main() {
    Solution sol;

    vector<vector<char>> g1 = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };
    cout << "Test 1: " << sol.numIslands(g1);
    cout << (sol.numIslands(g1) == 3 ? "  PASS" : "  FAIL") << endl;

    vector<vector<char>> g2 = {
        {'1','1','1'},
        {'0','1','0'},
        {'1','1','1'}
    };
    cout << "Test 2: " << sol.numIslands(g2);
    cout << (sol.numIslands(g2) == 1 ? "  PASS" : "  FAIL") << endl;

    vector<vector<char>> g3 = {
        {'0','0','0'},
        {'0','0','0'}
    };
    cout << "Test 3: " << sol.numIslands(g3);
    cout << (sol.numIslands(g3) == 0 ? "  PASS" : "  FAIL") << endl;

    vector<vector<char>> g4 = {
        {'1','0','1'},
        {'0','0','0'},
        {'1','0','1'}
    };
    cout << "Test 4: " << sol.numIslands(g4);
    cout << (sol.numIslands(g4) == 4 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
