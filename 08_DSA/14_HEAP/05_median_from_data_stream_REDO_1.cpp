// ============================================================
// FIND MEDIAN FROM DATA STREAM   (LC-295)   -- REDO_1 (blank)
// ============================================================
// Numbers ek-ek karke aate rehte (stream). Kabhi bhi median poocha
// jaa sakta -- ab tak ke saare numbers ka.
//   median = sorted me beech wala.  odd -> beech ek ; even -> beech ke DO ka average.
//
//   addNum(x)    -> stream me naya number daalo.
//   findMedian() -> ab tak ke saare numbers ka median (double).
//
// ---- TEST (operations -> expected median) ----
//   add(1)                                          -> 1.0
//   add(2)  findMedian                              -> 1.5   (1,2)
//   add(3)  findMedian                              -> 2.0   (1,2,3)
//   [fresh] add(5) add(15) add(1) add(3) findMedian -> 4.0   (1,3,5,15 -> (3+5)/2)
//   [fresh] add(2) add(2) add(2) findMedian         -> 2.0
//   [fresh] add(1)add(3)add(5)add(8)add(15) findMed -> 5.0   (odd, beech = 5)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class MedianFinder
{
public:
    priority_queue<double> maxHeap;
    priority_queue<double, vector<double>, greater<double>> minHeap;

    MedianFinder()
    {
    }

    void addNum(int num)
    {
        if (maxHeap.empty() || maxHeap.top() >= num)
        {
            maxHeap.push(num);
        }
        else
        {
            minHeap.push(num);
        }

        if (maxHeap.size() < minHeap.size())
        {
            double it = minHeap.top();
            minHeap.pop();
            maxHeap.push(it);
        }
        else if (maxHeap.size() > minHeap.size() + 1)
        {
            double it = maxHeap.top();
            maxHeap.pop();
            minHeap.push(it);
        }
    }

    double findMedian()
    {
        if (maxHeap.size() == minHeap.size())
        {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
        return maxHeap.top();
    }
};

// ---- test helper (ise mat chhed) ----
void check(double got, double exp, const string &label)
{
    bool ok = fabs(got - exp) < 1e-6;
    cout << label << " -> got " << got << " | exp " << exp
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    MedianFinder m1;
    m1.addNum(1);
    check(m1.findMedian(), 1.0, "case1");
    m1.addNum(2);
    check(m1.findMedian(), 1.5, "case2");
    m1.addNum(3);
    check(m1.findMedian(), 2.0, "case3");

    MedianFinder m2;
    m2.addNum(5);
    m2.addNum(15);
    m2.addNum(1);
    m2.addNum(3);
    check(m2.findMedian(), 4.0, "case4");

    MedianFinder m3;
    m3.addNum(2);
    m3.addNum(2);
    m3.addNum(2);
    check(m3.findMedian(), 2.0, "case5");

    MedianFinder m4;
    m4.addNum(1);
    m4.addNum(3);
    m4.addNum(5);
    m4.addNum(8);
    m4.addNum(15);
    check(m4.findMedian(), 5.0, "case6");
    return 0;
}
