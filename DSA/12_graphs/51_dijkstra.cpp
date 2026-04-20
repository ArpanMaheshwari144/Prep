// Q: Dijkstra's Algorithm — weighted graph mein source se sabke shortest path nikalo
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;
class Solution
{
public:
    // Dijkstra — shortest path from source to all nodes
    // adj[i] = {{neighbor, weight}, ...}
    // Return dist array
    vector<int> dijkstra(int n, vector<vector<pair<int, int>>> &adj, int src)
    {
        // sabki distance INF se start — source ki 0
        vector<int> dist(n, INT_MAX);

        // min-heap — sabse chhoti distance wala pehle niklega
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty())
        {
            // sabse chhoti distance wala node nikalo
            auto [d, node] = pq.top();
            pq.pop();

            // purana entry hai — skip karo
            if (d > dist[node])
            {
                continue;
            }

            // saare neighbours check karo — chhota path mila toh update karo
            for (auto &it : adj[node])
            {
                if (d + it.second < dist[it.first])
                {
                    dist[it.first] = d + it.second;
                    pq.push({d + it.second, it.first});
                }
            }
        }
        return dist;
    }
};

void printArr(vector<int> &v)
{
    cout << "[";
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i];
        if (i < v.size() - 1)
            cout << ",";
    }
    cout << "]";
}

int main()
{
    Solution sol;

    //     1
    // 0 ----- 1
    // |        |
    // 4|       |2
    // |    3   |
    // 2 ----- 3
    vector<vector<pair<int, int>>> adj1(4);
    adj1[0] = {{1, 1}, {2, 4}};
    adj1[1] = {{0, 1}, {3, 2}};
    adj1[2] = {{0, 4}, {3, 3}};
    adj1[3] = {{1, 2}, {2, 3}};

    vector<int> r1 = sol.dijkstra(4, adj1, 0);
    cout << "Test 1: ";
    printArr(r1);
    cout << (r1[0] == 0 && r1[1] == 1 && r1[2] == 4 && r1[3] == 3 ? "  PASS" : "  FAIL") << endl;

    // Single node
    vector<vector<pair<int, int>>> adj2(1);
    vector<int> r2 = sol.dijkstra(1, adj2, 0);
    cout << "Test 2: ";
    printArr(r2);
    cout << (r2[0] == 0 ? "  PASS" : "  FAIL") << endl;

    //     10
    // 0 ------ 1
    // |        |
    // 5|       |3
    // |    2   |
    // 2 ----- 3
    vector<vector<pair<int, int>>> adj3(4);
    adj3[0] = {{1, 10}, {2, 5}};
    adj3[1] = {{0, 10}, {3, 3}};
    adj3[2] = {{0, 5}, {3, 2}};
    adj3[3] = {{1, 3}, {2, 2}};

    vector<int> r3 = sol.dijkstra(4, adj3, 0);
    cout << "Test 3: ";
    printArr(r3);
    cout << (r3[0] == 0 && r3[1] == 10 && r3[2] == 5 && r3[3] == 7 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
