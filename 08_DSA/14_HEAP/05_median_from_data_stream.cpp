// ============================================================
// FIND MEDIAN FROM DATA STREAM      (LC-295)   [interview mode]
// ============================================================
// Numbers ek-ek karke aate rehte (stream). Kabhi bhi median poocha
// jaa sakta -- ab tak ke saare numbers ka.
//   median = sorted me beech wala.
//            odd count  -> beech wala ek.
//            even count -> beech ke DO ka average.
//
// Do operation banane hain:
//   addNum(x)     -> stream me naya number daalo.
//   findMedian()  -> ab tak ke saare numbers ka median (double) return.
//
// ---- TEST (operations -> expected median) ----
//   add(1)                       -> 1.0
//   add(2)   findMedian          -> 1.5     (1,2)
//   add(3)   findMedian          -> 2.0     (1,2,3)
//   [fresh] add(5) add(15) add(1) add(3) findMedian -> 4.0   (1,3,5,15 -> (3+5)/2)
//   [fresh] add(2) add(2) add(2) findMedian         -> 2.0
// ============================================================
//
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
        // STEP 1 PLACEMENT: num chhota (<=maxHeap.top) -> chhoti team (maxHeap) ; warna badi team (minHeap)
        //         maxHeap khaali ho to seedha maxHeap (top() crash se bacho)
        if (maxHeap.empty() || maxHeap.top() >= num)
        {
            maxHeap.push(num);
        }
        else
        {
            minHeap.push(num);
        }

        // STEP 2 BALANCE: farak max 1, extra hamesha maxHeap me (odd pe maxHeap.top = median)
        //   minHeap bada -> uska top maxHeap me shift
        if (maxHeap.size() < minHeap.size())
        {
            double tp = minHeap.top();
            minHeap.pop();
            maxHeap.push(tp);
        }
        //   maxHeap 1 se zyada bada -> uska top minHeap me shift
        else if (maxHeap.size() > minHeap.size() + 1)
        {
            double tp = maxHeap.top();
            maxHeap.pop();
            minHeap.push(tp);
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
    return 0;
}
