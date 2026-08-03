// ============================================================
// CONNECTED COMPONENTS via DSU (Union-Find)   [GRAPHS #10 · DSU]
// ============================================================
// n nodes (0..n-1), undirected edges. Kitne ALAG groups (components)?
//   (yehi #3 tha -- tab BFS se kiya. ab UNION-FIND se. jawab same aana chahiye.)
//
//   DSU (Disjoint Set Union / Union-Find) = "kaun kiske GROUP me" ka FAST manager.
//     real-life: DOST-CIRCLE. sab apne-apne circle me; do dost bante -> circles MERGE.
//                poochо "A-B same circle?" -> turant jawab.
//
//   SAAR: parent[] -> har node ka leader. har edge pe do node UNITE karo.
//           end me kitne alag LEADER (root, parent[i]==i) bache = utne components.
//
//   ---- STRUCTURE ----
//     parent[] : har node apne parent ko point. ROOT = jo khud ko point kare (parent[x]==x) = us group ka LEADER.
//     rnk[]    : tree ki approx height (union-by-rank ke liye).
//
//   ---- 2 OPERATIONS ----
//     find(x)     = x ka ROOT/leader. parent follow karte jao till parent[x]==x.
//     unite(x,y)  = dono ke root nikalo (find). alag ho to ek ko dusre ke neeche laga do (MERGE).
//     same group? = find(x)==find(y)
//
//   ---- 2 OPTIMIZATION (isse ~O(1) fast) ----
//     1. PATH COMPRESSION (find me): root milte hi parent[x]=root seedha -> tree flat -> agli find fast.
//          return parent[x] = find(parent[x]);
//     2. UNION BY RANK (unite me): chhota (kam rank) tree bade ke neeche -> tree flat rahe.
//          rank barabar ho to koi bhi neeche + uska rank++.
//
//   ---- KAB USE (DSU ke signal) ----
//     "groups / connected / same-set?" + edges ek-ek aate + MERGE/query.
//     e.g.: connected components, UNDIRECTED cycle-detect, number of provinces,
//           redundant connection, accounts merge, MST-Kruskal.
//     DSU vs BFS/DFS: BFS/DFS pura graph traverse karta; DSU INCREMENTAL (edge aate-aate merge) + query fast.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ─── DSU CLASS (find/unite tu bhar) ─────────────────────────
class DSU
{
public:
    vector<int> parent, rnk;
    // parent[x] = x ka parent (root = khud ka parent = LEADER)
    // rnk[x]    = us tree ki approx HEIGHT (kitna gehra). union-by-rank isi se "chhota-bada" naap ke
    //             chhota ped bade ke neeche lagata -> tree FLAT rehta -> find fast.
    //             (path-compression ke baad ye actual-height nahi, bas heuristic reh jaata.)

    DSU(int n)
    {
        parent.resize(n);
        rnk.assign(n, 0);
        for (int i = 0; i < n; i++)
            parent[i] = i; // shuru: har node apna hi leader (root)
    }

    int find(int x)
    {
        if (x == parent[x]) // khud ka parent = ROOT (leader) mil gaya
        {
            return x;
        }
        // PATH COMPRESSION: root laao AUR parent[x] ko seedha root pe set (agli baar 1 step)
        return parent[x] = find(parent[x]);
    }

    void unite(int x, int y)
    {
        int u_x = find(x); // x ka root
        int u_y = find(y); // y ka root

        if (u_x == u_y) // pehle se same group -> kuch nahi
            return;

        // UNION BY RANK: chhota (kam rank) tree bade ke neeche -> tree flat rahe
        if (rnk[u_x] < rnk[u_y])
        {
            parent[u_x] = u_y; // u_x chhota -> u_y ke neeche
        }
        else if (rnk[u_y] < rnk[u_x])
        {
            parent[u_y] = u_x; // u_y chhota -> u_x ke neeche
        }
        else
        {
            parent[u_y] = u_x; // barabar -> koi bhi neeche
            rnk[u_x]++;        // naye root ka rank badha
        }
    }
};

int countComponents(int n, vector<vector<int>> &edges)
{
    DSU dsu(n);
    for (auto &e : edges)
        dsu.unite(e[0], e[1]); // har edge -> do node merge

    int count = 0;
    for (int i = 0; i < n; i++)
        if (dsu.find(i) == i)
            count++; // distinct root = ek group
    return count;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
// ── VISUAL DRY-RUN (T1: n=5, edges={{0,1},{1,2},{3,4}}) ──────────────
//   Shuru: parent=[0,1,2,3,4]  rnk=[0,0,0,0,0]     0  1  2  3  4  (sab apna leader)
//
//   unite(0,1): rnk barabar -> parent[1]=0, rnk[0]++
//      parent=[0,0,2,3,4]  rnk=[1,0,0,0,0]      0(r1)     2   3   4
//                                                |
//                                                1
//   unite(1,2): find(1)=0; rnk[0]=1 > rnk[2]=0 -> parent[2]=0  (chhota bade ke neeche)
//      parent=[0,0,0,3,4]  rnk=[1,0,0,0,0]       0(r1)        3   4
//                                               /  \
//                                              1    2
//   unite(3,4): rnk barabar -> parent[4]=3, rnk[3]++
//      parent=[0,0,0,3,3]  rnk=[1,0,0,1,0]       0(r1)        3(r1)
//                                               /  \           |
//                                              1    2          4
//   count: find(i)==i wale -> node 0 aur node 3 -> 2 ROOT = 2 components
//   (2 alag ped: {0,1,2} leader 0, {3,4} leader 3)
// ─────────────────────────────────────────────────────────────────────
int main()
{
    vector<vector<int>> e1 = {{0, 1}, {1, 2}, {3, 4}};
    cout << "T1: " << countComponents(5, e1) << " (exp 2)\n";

    vector<vector<int>> e2 = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    cout << "T2: " << countComponents(5, e2) << " (exp 1)\n";

    vector<vector<int>> e3 = {};
    cout << "T3: " << countComponents(4, e3) << " (exp 4)\n";

    vector<vector<int>> e4 = {};
    cout << "T4: " << countComponents(1, e4) << " (exp 1)\n";
    return 0;
}
