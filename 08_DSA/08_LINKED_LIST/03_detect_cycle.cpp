// ============================================================
// LINKED LIST CYCLE (DETECT) — Linked List (pattern #3, fast/slow)
// ============================================================
// head diya. TRUE agar list me CYCLE hai (koi node ka next kisi PICHLE node pe
// wapas point karta) -> warna FALSE.
//   [3,2,0,-4] with tail->node(index1)  -> true
//   [1,2,3] no cycle                     -> false
//
// (approach LIKHA NAHI -- tu ne khud pehchana: MIDDLE wala same concept = fast/slow.
//  socho: agar cycle hai to tez aur dheema kabhi MILENGE? ya nahi? copy-pen pe socho.)
//
// Tests (values, cyclePos):  cyclePos = -1 matlab NO cycle; warna us index pe tail jud jaata.
//   [3,2,0,-4], pos=1   -> true
//   [1,2],      pos=0   -> true
//   [1],        pos=0   -> true   (khud pe point)
//   [1,2,3],    pos=-1  -> false
//   [1],        pos=-1  -> false
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

struct Node
{
    int val;
    Node *next;
    Node(int v) : val(v), next(nullptr) {}
};

// ---- tera kaam: yeh function ----
bool hasCycle(Node *head)
{
    Node *slow = head;
    Node *fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            return true;
        }
    }
    return false;
}

// ---------- helpers (boilerplate, chhoo mat) ----------
// list banata; agar pos>=0 -> last node ka next = node at index pos (cycle).
Node *buildListWithCycle(vector<int> v, int pos)
{
    if (v.empty())
        return nullptr;
    vector<Node *> nodes;
    for (int x : v)
        nodes.push_back(new Node(x));
    for (int i = 0; i + 1 < (int)nodes.size(); i++)
        nodes[i]->next = nodes[i + 1];
    if (pos >= 0)
        nodes.back()->next = nodes[pos]; // cycle jodo
    return nodes[0];
}

int main()
{
    cout << hasCycle(buildListWithCycle({3, 2, 0, -4}, 1)) << " (expected 1)\n";
    cout << hasCycle(buildListWithCycle({1, 2}, 0)) << " (expected 1)\n";
    cout << hasCycle(buildListWithCycle({1}, 0)) << " (expected 1)\n";
    cout << hasCycle(buildListWithCycle({1, 2, 3}, -1)) << " (expected 0)\n";
    cout << hasCycle(buildListWithCycle({1}, -1)) << " (expected 0)\n";
    return 0;
}
