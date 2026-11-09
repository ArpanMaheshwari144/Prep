## PATTERN 6 — STACK

```
 BROAD IDEA: stack = LIFO. "kholo push, band karo pop-match" · ya MONOTONIC (next-greater/smaller).
 ★ EDGE (monotonic stack): stack me INDEX rakho, value nahi (DOORI chahiye: i - st.top()).
 ★ MILTE-JULTE (histogram vs container / trapping): 00_SHURU me NAAM-SE-CONFUSE A dekh.

 ★ YAHAN NAHI MILEGI:
   generate parentheses   ->  PATTERN 16 (BACKTRACKING)
         valid-parentheses CHECK karta hai (stack) · generate BANATA hai (choose/un-choose)
```

#### ── FAMILY: MONOTONIC stack (INDEX stack, curr vs top compare -> pop + us index ka ans) ──

```
     // ek hi skeleton, sirf 3 cheez badalti:
     // (1) cmp: `<` = GREATER dhoondh rahe, `>` = SMALLER (ulta lagta: top ko jab curr hara de)
     // (2) ans me kya: VALUE (next greater / smaller) · DISTANCE i - top (daily temp) · INDEX (histogram)
     // (3) disha: next = 0..n-1 · prev = n-1..0 (loop ulta) · circular = 2n loop, i % n

 ★★ ASLI CHAABI (Arpan, 9-Nov chai ke baad): is family me yaad sirf LOOP KI DISHA rakhni hai.
      NEXT  (smaller / greater)  ->  loop 0 se n-1
      PREV  (smaller / greater)  ->  loop n-1 se 0
      Arpan (9-Nov raat): "previous ko yaad karne ki zaroorat hi nahi. PREVIOUS = peeche se chalna, to loop ulta (n-1 se 0).
      NEXT = aage ka, to 0 se. Bas ye yaad rakho."
    sign (< ya >) copy-pen pe ek dry run me pakad aata ("chhota aaya to kise pop?").
    disha galat = dry run bhi ulta ghoomta (histogram REDO_2, 38 min yahin).

 KAB: brackets match · "har element ka agla / pichhla bada ya chhota" · span · histogram
```

### ▸ NEXT GREATER ELEMENT  = monotonic stack ka BASE

```
Ek array diya hai. Har number ke liye uske daayi taraf jo pehla usse bada number hai, wo lautao. Koi bada na mile to -1.
  e.g. [2,1,2,4,3] -> [4,2,4,-1,-1]
  kyun: 2 ke aage pehla bada 4 · 1 ke aage 2 · 2 ke aage 4 · 4 aur 3 ke aage koi bada nahi -> -1

har element ke DAAYE pehla BADA, koi nahi -> -1.  [2,1,2,4,3] -> [4,2,4,-1,-1]
```

```
SAAR: MONOTONIC stack (INDEX ka). left->right. jise abhi tak apna "bada" nahi mila wo stack me pade
      (decreasing). curr aaya -> jo bhi top usse CHHOTA -> unka answer = curr -> pop.
DRY-RUN [2,1,2,4,3]:
  i0(2): push            st=[0]
  i1(1): 2<1? no; push   st=[0,1]
  i2(2): 1<2 ans[1]=2 pop; 2<2? no; push  st=[0,2]
  i3(4): 2<4 ans[2]=4; 2<4 ans[0]=4; push st=[3]
  i4(3): 4<3? no; push   st=[3,4]
  => ans=[4,2,4,-1,-1]
FAMILY: monotonic-stack ka BASE. Next-Smaller / Prev-Smaller = isi ka DELTA (neeche).
```

```cpp
vector<int> nextGreater(vector<int> &nums) {
    vector<int> ans(nums.size(), -1);       // -1 init: daaye bada na mile wo -1 hi rahe
    stack<int> st;                          // ★ INDEX rakho: ans[topp] me likhne ko position chahiye
    for (int i = 0; i < nums.size(); i++) { // left->right: daaye wala bada aage aayega
        while (!st.empty() && nums[st.top()] < nums[i]) {   // ★ !empty pehle (warna top() crash)
            int topp = st.top();            // < : curr BADA -> top ko jawab mila. ★ <= nahi: [2,2] pe ans[0]=2, sahi -1
            st.pop();                       // top pehle bachao: pop ke baad top() badal jaata
            ans[topp] = nums[i];
        }
        st.push(i);                         // ★ while ke BAAD push: curr ab apne jawab ka intezaar kare
    }
    return ans;                             // stack me bache = koi bada nahi = -1 (init)
}
```

### ▸ NEXT SMALLER ELEMENT  = NEXT GREATER, sirf condition ULTI (>)

```
Ek array diya hai. Har number ke liye uske daayi taraf jo pehla usse chhota (strictly) number hai, wo lautao. Koi na mile to -1.
  e.g. [4,5,2,10,8] -> [2,2,-1,8,-1]
  kyun: 4 aur 5 ke aage pehla chhota 2 · 2 ke aage sab bade -> -1 · 10 ke aage 8 · 8 aakhri -> -1
```

```cpp
vector<int> nextSmaller(vector<int> &nums) {   // = next greater, sirf condition ULTI
    vector<int> ans(nums.size(), -1);
    stack<int> st;
    for (int i = 0; i < nums.size(); i++) {
        while (!st.empty() && nums[st.top()] > nums[i]) {   // ★ > strict (>= nahi): [2,2,2] -> saare -1
            int topp = st.top();
            st.pop();
            ans[topp] = nums[i];
        }
        st.push(i);
    }
    return ans;
}
```

### ▸ PREVIOUS SMALLER ELEMENT  = NEXT SMALLER, sirf LOOP ULTA (right->left)

```
Ek array diya hai. Har number ke liye uske baayi taraf jo pehla usse chhota (strictly) number hai, wo lautao. Koi na mile to -1.
  e.g. [4,5,2,10,8] -> [-1,4,-1,2,2]
  kyun: 4 ke peeche kuch nahi · 5 ke peeche 4 · 2 ke peeche 4,5 bade -> -1 · 10 aur 8 ke peeche pehla chhota 2
```

```cpp
vector<int> prevSmaller(vector<int> &nums) {   // = next smaller, sirf LOOP ULTA (condition wahi >)
    vector<int> ans(nums.size(), -1);
    stack<int> st;
    for (int i = nums.size() - 1; i >= 0; i--) {   // ★ right->left: right ka next smaller = left ka prev
        while (!st.empty() && nums[st.top()] > nums[i]) {
            int topp = st.top();
            st.pop();
            ans[topp] = nums[i];
        }
        st.push(i);
    }
    return ans;
}
// prev smaller + next smaller = histogram ka building block
```

### ▸ LARGEST RECTANGLE IN HISTOGRAM (LC-84)  = PREV-SMALLER + NEXT-SMALLER (INDEX store + combine)

```
heights[] me bars ki unchai di hai, har bar ki chaudai 1 hai. Lagaatar bars pe jo sabse bada rectangle ban sakta hai, uska area lautao.
  e.g. [2,1,5,6,2,3] -> 10
  kyun: bars 5 aur 6 -> height min(5,6)=5 x width 2 = 10, baaki koi bada nahi banta

har bar pe KHADE ho: left me pehla CHHOTA (pS), right me pehla CHHOTA (nS) = dono STOPPER
[2,1,5,6,2,3] -> 10 (bars 5,6 -> min 5 * width 2). next GREATER se nahi: rectangle chhote pe rukta
```

```
SAAR: HAR bar pe KHADE ho -> left me pehla CHHOTA (PS), right me pehla CHHOTA (NS)
      = dono STOPPER -> beech ki width -> area = height*width -> sabka MAX.
= PREV-SMALLER + NEXT-SMALLER ka WALL, bas DELTA: VALUE nahi INDEX store + combine loop.

3 JAGAH DHYAAN:
  1. INDEX store, value nahi (width index se banti).
  2. nS ka default = n   (n = array ka SIZE = kitne bar)   NA -1

     KYUN: -1 aur n = array ke BAHAR ki nakli deewar

           [2,4] -> size 2 -> n = 2   (index 0, 1 andar; index 2 bahar)

           idx:      -1  |  0   1  |  2 (= n)
                   deewar|  2   4  |deewar

           left  me koi chhota nahi  ->  deewar index 0 se PEHLE  ->  pS = -1
           right me koi chhota nahi  ->  deewar aakhri ke BAAD    ->  nS = n

     MISAAL: [2,4]   (dono bar ko right me koi chhota nahi -> nS dono jagah DEFAULT)

       CASE 1: nS default = -1                      GALAT

           idx   :  0    1
           h     :  2    4
           pS    : -1    0
           nS    : -1   -1      <- default -1

           bar0  : -1 - (-1) - 1 = -1   ->  area 2 * -1 = -2
           bar1  : -1 -  0   - 1 = -2   ->  area 4 * -2 = -8
           ans   = -2                     (sahi 4)
           wajah : right ki deewar LEFT me rakh di -> width minus

       CASE 2: nS default = n = 2                   SAHI

           idx   :  0    1
           h     :  2    4
           pS    : -1    0
           nS    :  2    2      <- default n (size)

           bar0  :  2 - (-1) - 1 = 2    ->  area 2 * 2 = 4   (dono bar, height 2)
           bar1  :  2 -  0   - 1 = 1    ->  area 4 * 1 = 4   (sirf bar1)
           ans   = 4
           wajah : right ki deewar RIGHT me bahar -> width sahi

       CASE 3: nS default = n-1 = 1                 GALAT

           bar0  :  1 - (-1) - 1 = 1    ->  area 2 * 1 = 2   (bar1 chhoot gaya)
           bar1  :  1 -  0   - 1 = 0    ->  area 4 * 0 = 0
           ans   = 2                     (sahi 4)
           wajah : deewar aakhri bar ke UPAR rakh di, uske BAAD nahi

     FAYDA: NS - PS - 1 kinare wale bar pe bhi sahi, alag if-else nahi.
  3. `>` strictly, `>=` NAHI.   [3,3,3,3]: `>=` -> barabar wala bhi stopper -> har width 1 -> 3 (galat)
                                            `>`  -> koi stopper nahi -> width 4 -> 12.

width = NS-PS-1 KYUN: stopper khud andar NAHI -> bars PS+1 se NS-1 tak
      count = (NS-1)-(PS+1)+1 = NS-PS-1   (-1 SIRF EK baar, do nahi -- building6: 4-2-1=1 sahi, 4-2-1-1=0 galat)

DRY-RUN (h=[2,1,5,6,2,3]):
     6 |            ##
       |        +------+
     5 |        |## ##|
     4 |        |## ##|
     3 |        |## ##|
     2 |  ##    |## ##|  ##  ##
     1 |  ## ## |## ##|  ##  ##
       +--------+------+--------
    idx:  0  1    2 3    4   5

    ANSWER (dash-box) = height 5 x width 2 = 10  (sabse bada)
      i3 ka h=6 ka 6th block box ke UPAR (rectangle sirf 5 tall).

    idx  :  0   1   2   3   4   5
    h    :  2   1   5   6   2   3
    PS   : -1  -1   1   2   1   4
    NS   :  1   6   4   4   6   6
    width:  1   6   2   1   4   1
    area :  2   6 [10]  6   8   3   -> MAX 10
    bar5 (idx2): PS=1 (h1), NS=4 (h2) -> beech me c2,c3 -> width 2 -> 5*2 = 10.
                 first = PS+1 = 2, last = NS-1 = 3 -> 3-2+1 = 2. stoppers khud andar nahi.

FAMILY: next-GREATER se NAHI (rectangle bade bar ko andar leta, CHHOTE pe rukta).
        2D bhai = MAXIMAL RECTANGLE (har row = histogram, isi ko call).

★ GALTI (REDO_2, 38 min yahin atka): "2 ka jawab abhi kahan se laaun, stack to khaali hai?"
  STACK = jawab ka INTEZAAR karne walon ki LINE. Jawab abhi nahi chahiye, BAAD me aayega.
    stack khaali = koi baat nahi: 2 line me khada ho jaata, default pehle se bhara (NS = n, PS = -1).
    CHHOTA aaya  = line walon ka jawab mil gaya -> pop karo, unka ans bharo, phir naya bhi line me.

  NS (aage ka chhota)   : loop 0 -> n-1
  PS (peeche ka chhota) : loop n-1 -> 0      wahi chaal, bas disha ulti
  shart (dono me same)  : heights[top] > heights[i]   -> naya CHHOTA aaya tab pop
                          ulta sign (heights[i] > heights[top]) = next GREATER ban jaata (yahi likha tha)

  CHAAR KE CHAAR pe yahi niyam (sirf "pop karke bharo" wale tareeke me):
    next smaller / next greater  -> 0 -> n-1
    prev smaller / prev greater  -> n-1 -> 0
    smaller ya greater           -> sirf while ka sign badalta
  dhyaan: "pehle bekaar nikaalo, phir top hi jawab" wala doosra tareeka ULTI disha chalta. dono mat milao.

  copy-pen [2,1,5,6,2,3], NS:
    i0(2): line khaali -> 2 line me
    i1(1): 1 < 2 -> 2 ka NS = 1, pop; 1 line me
    i2(5), i3(6): bade -> line me
    i4(2): 2 < 6 -> 6 ka NS = 4; 2 < 5 -> 5 ka NS = 4; 2 line me
    i5(3): line me.  bache (1, 2, 3) -> NS = n = 6
    NS = [1, 6, 4, 4, 6, 6]
```

```cpp
vector<int> nextSmaller(vector<int> &heights) {
    int n = heights.size();
    stack<int> st;
    vector<int> ans(n, n);                  // ★ default n (-1 NAHI): right ki bahar deewar
    for (int i = 0; i < n; i++) {
        while (!st.empty() && heights[st.top()] > heights[i]) {   // ★ > strict: [3,3,3,3] >= -> 3, > -> 12
            int tp = st.top();
            st.pop();
            ans[tp] = i;                    // INDEX store, value nahi: width index se banti
        }
        st.push(i);
    }
    return ans;
}

vector<int> prevSmaller(vector<int> &heights) {
    int n = heights.size();
    stack<int> st;
    vector<int> ans(n, -1);                 // default -1: left ki bahar deewar (index 0 se pehle)
    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && heights[st.top()] > heights[i]) {
            int tp = st.top();
            st.pop();
            ans[tp] = i;
        }
        st.push(i);
    }
    return ans;
}

int largestRectangleArea(vector<int> &heights) {
    int n = heights.size();
    vector<int> ns = nextSmaller(heights);
    vector<int> ps = prevSmaller(heights);
    int ans = INT_MIN;                      // INT_MIN: pehla area bhi max me aaye (0 bhi chalega)
    for (int i = 0; i < n; i++) {           // har bar pe khade ho: wahi is rectangle ki height
        int width = ns[i] - ps[i] - 1;      // ★ bars ps+1 .. ns-1; stopper khud andar nahi
        ans = max(ans, heights[i] * width); // heights[i] : range me ye bar sabse chhota
    }
    return ans;
}
// [2,4]: nS default -1 hota to bar0 width = -1-(-1)-1 = -1 (galat); n=2 se width 2 -> area 4 (sahi)
```

### ▸ MAXIMAL RECTANGLE (LC-85, 2D)  = LC-84 ka 2D bhai (har row = histogram)

```
0 aur 1 ki 2D matrix di hai. Sirf 1 se bhara hua sabse bada rectangle kitna bada hai, uska area lautao.
  e.g. [[1,0,1,0,0],[1,0,1,1,1],[1,1,1,1,1],[1,0,0,1,0]] -> 6
  kyun: row 1 aur row 2 me col 2,3,4 sab 1 -> 2 row x 3 col = 6

= LC-84 ka 2D bhai: HAR ROW = histogram -> upar wala largestRectangleArea REUSE -> sab rows ka MAX
★ LeetCode pe matrix vector<vector<char>> aati: wahan signature char karo aur check matrix[i][j] == '1'
  (neeche code teri file 06_STACK/10_maximal_rectangle.cpp jaisa: int matrix + == 1)
```

```cpp
int maximalRectangle(vector<vector<int>> &matrix) {
    int row = matrix.size();
    int col = matrix[0].size();
    vector<int> heights(col, 0);            // COL size (row nahi): har column ki running height
    int maxi = INT_MIN;
    for (int i = 0; i < row; i++) {         // outer i = row
        for (int j = 0; j < col; j++) {     // inner j = col
            if (matrix[i][j] == 1)
                heights[j] += 1;            // upar wale 1s ke saath build-up
            else
                heights[j] = 0;             // ★ column toot gaya -> RESET
        }
        maxi = max(maxi, largestRectangleArea(heights));   // ★ call j-loop ke BAHAR, har row ke baad
    }
    return maxi;
}
```

### ▸ DAILY TEMPERATURES  = NEXT GREATER, ans me VALUE nahi DISTANCE (i - top)

```
temps[] me har din ka taapmaan diya hai. Har din ke liye batao ki kitne din baad pehla usse garam din aata hai. Garam din kabhi na aaye to 0.
  e.g. [73,74,75,71,69,72,76,73] -> [1,1,4,2,1,1,0,0]
  kyun: 75 (din 2) ke baad pehla zyada 76 din 6 pe -> 6-2 = 4 · 76 ke baad koi zyada nahi -> 0
```

```cpp
vector<int> dailyTemperatures(vector<int> &temps) {
    int n = temps.size();
    vector<int> ans(n, 0);                  // 0 init (-1 nahi): garam din na mile to 0
    stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && temps[st.top()] < temps[i]) {   // < strict: barabar = garam nahi
            int tp = st.top();
            st.pop();
            ans[tp] = i - tp;               // ★ VALUE nahi, DISTANCE (kitne din baad)
        }
        st.push(i);
    }
    return ans;
}
```

### ▸ NEXT GREATER ELEM II  = NEXT GREATER + CIRCULAR (2n loop, i % n)

```
Ek gol (circular) array diya hai, matlab aakhri number ke baad phir pehla number aata hai. Har number ka agla bada number lautao, na mile to -1.
  e.g. [1,2,3,4,3] -> [2,3,4,-1,4]
  kyun: aakhri 3 ke aage kuch nahi, ghoom ke shuru me 1,2,3,4 -> pehla bada 4 · 4 se bada kahin nahi -> -1
```

```cpp
vector<int> nextGreaterElements(vector<int> &nums) {   // CIRCULAR: aakhri ka greater shuru me bhi
    if (nums.size() == 0)
        return {};
    int n = nums.size();
    stack<int> st;                          // INDEX (0..n-1) jinka greater abhi nahi mila
    vector<int> ans(n, -1);                 // size n (2n nahi); na mile -> -1
    for (int i = 0; i < 2 * n; i++) {       // ★ 2n: doosre chakkar me aakhri elements ko shuru wale dikhte
        while (!st.empty() && nums[st.top()] < nums[i % n]) {   // i % n = asli index (i 2n tak)
            int tp = st.top();
            st.pop();
            ans[tp] = nums[i % n];          // jawab TOP ka -> ans[tp] (ans[i] nahi)
        }
        st.push(i % n);                     // ★ i % n push, i nahi (i >= n pe index bahar)
    }
    return ans;
}
// [1,2,1]: aakhri 1 (idx 2) pehle chakkar me bacha -> i=4 -> 4%3=1 -> nums[1]=2 -> ans[2]=2. isliye 2n
```

### ▸ ONLINE STOCK SPAN (LC-901)  = monotonic-stack + SUM-span DELTA

```
Share ke daam ek-ek karke aate hain (next(price) call). Har call pe span lautao: aaj ko milake peeche kitne lagaatar din ka daam aaj ke daam se kam ya barabar tha.
  e.g. next: 100,80,60,70,60,75,85 -> 1,1,1,2,1,4,6
  kyun: 75 aaya -> peeche 60,70,60 sab <= 75, phir 80 bada -> ruk -> 75,60,70,60 = 4
```

```cpp
class StockSpanner {
public:
    stack<pair<int, int>> st;               // INDEX nahi, PAIR {price, span}
    StockSpanner() {}

    int next(int price) {
        int span = 1;                       // 1 se: aaj ka din khud bhi span me
        while (!st.empty() && st.top().first <= price) {   // ★ <= : barabar price wala din bhi span me
            span += st.top().second;        // ★ pop wale ka span JODO: wo apne peeche ke din nigal chuka
            st.pop();
        }
        st.push({price, span});             // poora span saath push: aage bada din ise nigle to sab jude
        return span;
    }
};
// 75 aaya, top pe ...60,70,60: 60 -> span 2 · 70 -> span 4 · 80 > 75 ruko -> push {75,4}
```

── (STANDALONE — apni alag trick) ──

### ▸ VALID PARENTHESES

```
Ek string me sirf ( ) [ ] { } hain. true lautao agar har khulne wala bracket sahi type ke bracket se aur sahi kram me band hota hai, warna false.
  e.g. "{[]}" -> true · "([)]" -> false
  kyun: "([)]" me ( khula, [ khula, par ) aaya jabki sabse upar [ khula hai -> match nahi
```

```cpp
bool isValid(string s) {
    stack<int> st;
    for (auto &it : s) {
        if (it == '(' || it == '[' || it == '{')
            st.push(it);                    // opening -> push
        else {
            if (st.empty())
                return false;               // closing aaya, stack khaali -> false
            char ch = st.top();             // top + pop PEHLE, phir match
            st.pop();
            if ((ch == '(' && it == ')') || (ch == '[' && it == ']') || (ch == '{' && it == '}'))
                continue;
            else
                return false;               // match nahi -> turant false
        }
    }
    return st.empty();                      // ★ end me stack KHAALI = valid ("" -> true)
}
```

### ▸ MIN STACK

```
Ek stack banao jisme push(val), pop(), top() aur getMin() chaaron O(1) me chalein. getMin() stack me abhi ka sabse chhota number deta hai.
  e.g. push(-2) push(0) push(-3) getMin() -> -3 · pop() · top() -> 0 · getMin() -> -2
  kyun: -3 pop hone ke baad stack me [-2,0] bacha -> upar 0, sabse chhota -2

class MinStack: push / pop / top / getMin sab O(1)
```

```cpp
class MinStack {                            // push / pop / top / getMin sab O(1)
public:
    stack<pair<int, int>> st;               // ★ {val, yahan tak ka MIN}: min hamesha ready, scan nahi
    MinStack() {}

    void push(int val) {
        int minVal = st.empty() ? val : min(st.top().second, val);   // ★ empty pehle: pehle pe top() nahi
        st.push({val, minVal});
    }
    void pop() { st.pop(); }
    int top() { return st.top().first; }
    int getMin() { return st.top().second; }   // O(1), koi loop nahi
};
// ek single min var kyun NAHI: pop pe purana min WAPAS aata, single var wo history nahi rakhta
// push 5,3,7 -> min 3 · pop 7 -> 3 · pop 3 -> 5 WAPAS (pair me neeche wala min apne aap sahi)
```

### ▸ MIN ADD MAKE VALID  = VALID PARENTHESES ka counter wala (stack nahi)

```
Ek string me sirf '(' aur ')' hain. Kam se kam kitne bracket jodne padenge taaki string valid ho jaaye, wo ginti lautao.
  e.g. "()))((" -> 4
  kyun: pehla () theek · agle do ')' ka koi '(' nahi -> 2 · aakhri do '(' band nahi hue -> 2 · total 4
```

```cpp
int minAddToMakeValid(string s) {           // COUNTER, stack nahi (sirf '(' ')')
    if (s.empty())
        return 0;
    int open_needed = 0;                    // kitne ')' UNMATCHED aaye (inhe pehle '(' chahiye tha)
    int close_needed = 0;                   // kitne '(' abhi UNMATCHED khade (inhe aage ')' chahiye)
    for (auto &it : s) {
        if (it == '(')
            close_needed++;                 // ek open, iska close pending
        else {
            if (close_needed > 0)
                close_needed--;             // pending open se MATCH = 0 insertion ("(())" -> 0)
            else
                open_needed++;              // ★ koi open nahi -> ye ')' extra
        }
    }
    return open_needed + close_needed;      // ★ dono bache = utne insert
}
```

### ▸ QUEUE USING STACKS (LC 232)  — Walmart R2 me aaya ("fully working code")

```
Sirf stack (push, pop, top, empty) ka use karke ek queue (pehle aaya, pehle gaya) banao: push(x) peeche daalta hai, pop() aage wala nikaalta hai, peek() aur empty() bhi chahiye.
  e.g. push(1) push(2) peek() -> 1 · pop() -> 1 · push(3) pop() -> 2 · pop() -> 3 · empty() -> true
  kyun: queue me jo pehle aaya wahi pehle nikalta -> 1, phir 2, phir 3, ab khaali
```

```cpp
class MyQueue {
public:
    stack<int> input;                       // push yahan
    stack<int> output;                      // pop / peek yahan se
    MyQueue() {}

    void push(int x) {
        input.push(x);
    }

    int pop() {
        if (output.empty()) {               // ★ KHAALI ho tabhi palto: har element ek hi baar input -> output
            while (!input.empty()) {        // -> amortized O(1) (har pop pe palatna = O(n))
                output.push(input.top());
                input.pop();
            }
        }
        int x = output.top();               // x bachao, phir pop, phir return
        output.pop();
        return x;
    }

    int peek() {
        if (output.empty()) {               // TRAP: output me kuch bacha ho tab palta -> kram toot jaata
            while (!input.empty()) {
                output.push(input.top());
                input.pop();
            }
        }
        return output.top();
    }

    bool empty() {
        if (input.empty() && output.empty())   // dono khaali
            return true;
        return false;
    }
};

// broad-trick: MONOTONIC stack (next-greater + daily-temp = same, bas ans me kya bharo alag).
```

