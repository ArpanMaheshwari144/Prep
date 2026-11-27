## PATTERN 18 — GREEDY


> [!IMPORTANT]
> **KAB:** har step pe abhi ka best chuno aur wo aage bhi best rahe · aksar pehle SORT, phir ek pass

> [!IMPORTANT]
> **BROAD IDEA:** har step pe ABHI-KA-BEST choice pakdo (local best), aage ki poori planning nahi.
> Kaam karta SIRF jab abhi-ka-best = aage-bhi-best (greedy-choice property); warna DP (jo sab explore kare).
>
> **SIGNATURE:** aksar pehle SORT (kisi key pe: size / finish-time) -> phir ek LINEAR pass me pick / skip.

> [!WARNING]
> ★ DHYAAN: intervals me "kam se kam hatao" -> tera code: START pe sort + overlap pe CHHOTE-END wala rakho (LC-435). Ya seedha END pe sort, tab wo if hi nahi lagta. Dono sahi.
>
> ★ DHYAAN: gas station: tank minus hua -> agla station naya start



████████████████████████████████████████████████

### ▸ ASSIGN COOKIES (LC-455)  = greedy intro (sort + 2-pointer)

**SAWAAL** g[] me har bachche ki bhookh (greed) aur s[] me har cookie ka size diya hai. Bachcha khush tab hota hai jab cookie ka size uski bhookh se kam na ho, aur ek bachche ko zyada se zyada ek cookie milti hai. Zyada se zyada kitne bachche khush kar sakte ho, wo lautao.

- **e.g.** `g=[1,2,3], s=[1,1] -> 1`
- **kyun:** dono cookie size 1 -> sirf greed-1 wala khush

```
bachche (bhookh):  1    2    3
cookie (size)   :  1    1
cookie 1 -> bhookh 1 khush · doosri cookie 1 < 2 aur 1 < 3 -> kisi kaam ki nahi -> 1
```

> [!IMPORTANT]
> **CHAABI:** dono SORT, 2 pointer: `s[j] >= g[i]` to khush (`i++ j++ count++`), warna sirf `j++` (badi cookie dhoondho). koi ek khatam -> ruk.

> [!WARNING]
> **Galti:** cookie chhoti pade to `i++` bhi  ->  **Sahi:** sirf `j++`, `i` wahi
> **Kyun:** bachcha wait kare, uske liye agli badi cookie dhoondhni hai.
>
> **Galti:** `s[j] > g[i]`  ->  **Sahi:** `s[j] >= g[i]`
> **Kyun:** barabar size bhi bachche ko khush karti.

**CODE**
```cpp
int findContentChildren(vector<int> &g, vector<int> &s) {
    int n = g.size(), m = s.size(), count = 0, i = 0, j = 0;   // i = bachha, j = cookie
    if (s.size() == 0)
        return 0;                           // cookie hi nahi -> 0
    sort(begin(g), end(g));                 // ★ dono SORT
    sort(begin(s), end(s));
    while (i < n && j < m) {                // koi ek khatam -> ruk
        if (s[j] >= g[i]) {                 // >= : barabar size bhi khush karti
            i++;                            // khush -> dono aage
            j++;
            count++;
        }
        else
            j++;                            // ★ cookie chhoti -> badi dhoondho
    }
    return count;                           // [10,9,8,7], [5,6,7,8] -> 2 (7->7, 8->8)
}
```



████████████████████████████████████████████████

### ▸ JUMP GAME (LC-55)  = greedy REACHABILITY (farthest-reach track)

**SAWAAL** nums[i] batata hai ki index i se zyada se zyada kitne kadam aage kood sakte ho. Index 0 se shuru karke kya aakhri index tak pahunch sakte ho? true ya false lautao.

- **e.g.** `[2,3,1,1,4] -> true · [3,2,1,0,4] -> false`
- **kyun:** 0->1->4 pahunche · doosre me har raasta index 3 (value 0) pe atakta

```
[2,3,1,1,4]   index:  0  1  2  3  4
              nums :  2  3  1  1  4
              i+nums: 2  4  3  4      -> index 1 se 4 tak pahunch -> true
[3,2,1,0,4]   index:  0  1  2  3  4
              nums :  3  2  1  0  4
              i+nums: 3  3  3  3      -> sab 3 pe rukte, 4 tak koi nahi -> false
```

> [!IMPORTANT]
> **CHAABI:** ek scan, `maxReach = max(maxReach, i + nums[i])`; `i > maxReach` hua to yahan pahunch hi nahi -> false; end `maxReach >= n - 1`.

> [!WARNING]
> **Galti:** `max(maxReach, nums[i])`  ->  **Sahi:** `max(maxReach, i + nums[i])`
> **Kyun:** `nums[i]` = kitne kadam (relative), maxReach = index (absolute), isliye `i` jodo.
>
> **Galti:** `i > maxReach` check update ke baad  ->  **Sahi:** update se PEHLE
> **Kyun:** jis index tak pahunche hi nahi, uska jump bhi gin liya jaata.

**CODE**
```cpp
bool canJump(vector<int> &nums) {  // nums[i] = us index se max kitna kood sakte. last tak pahunche?
    int maxReach = 0;                       // ab tak sabse DOOR kahan tak pahunch sakte (0 se)
    for (int i = 0; i < nums.size(); i++) {
        if (i > maxReach)
            return false;                   // ★ index i KHUD maxReach se aage = yahan pahunch hi nahi
        maxReach = max(maxReach, i + nums[i]);   // ★ i + nums[i]: nums[i] = kitne STEP (relative), i jodo
                                                 //   ->
    }                                       //   ABSOLUTE index ([2,3,1,1,4]: index 1 -> 1+3 = 4)
    return maxReach >= nums.size() - 1;     // O(n), koi DP nahi. [2,3,1,1,4] -> true
}
// cookies se FARAK: wahan sort + 2-pointer; yahan sort nahi, ek scan me farthest-reach (greedy ke 2
//   rang)
```



████████████████████████████████████████████████

### ▸ NON-OVERLAPPING INTERVALS (LC-435)  = greedy interval-scheduling (keep SMALLER-END) · MERGE INTERVALS ka bhai (sort + overlap check)

**SAWAAL** intervals[] diye hain. Kam se kam kitne interval hatane padenge taaki bache hue koi bhi do interval overlap na karein, wo lautao. Sirf chhoona (ek ka end = doosre ka start) overlap nahi maana jaata.

- **e.g.** `[[1,2],[2,3],[3,4],[1,3]] -> 1`
- **kyun:** [1,3] hatao -> [1,2],[2,3],[3,4] sirf chhoote, overlap nahi

```
time:    1   2   3   4
[1,2]    |---|
[2,3]        |---|
[3,4]            |---|
[1,3]    |-------|        <- [1,2] aur [2,3] dono se takraata, ise hatao -> 1
```

> [!IMPORTANT]
> **CHAABI:** start pe sort; overlap pe `count++` aur chhote-END wala rakho.

> [!WARNING]
> **Galti:** LINE SWEEP (MEETING ROOMS II) ka "ek saath MAX" = jawab  ->  **Sahi:** greedy: sort, overlap pe `count++`
> **Kyun:** sawaal "kitne HATAO" hai, "ek saath kitne" nahi: `[[1,2],[2,3],[3,4],[1,3]]` pe ek saath max 2, par hatana sirf 1.
>
> **Galti:** no-overlap check `<`  ->  **Sahi:** `intervals[start][1] <= intervals[end][0]`
> **Kyun:** sirf chhoona overlap nahi; `<` se `[[1,2],[2,3]]` pe 1 aata, sahi 0.
>
> **Galti:** overlap pe blindly `start` wala rakha  ->  **Sahi:** end compare, chhote END wala rakho
> **Kyun:** `[[1,100],[2,3],[3,4]]` pe 2 aata, sahi 1.

**CODE**
```cpp
int eraseOverlapIntervals(vector<vector<int>> intervals) {   // MIN kitne HATAO ki overlap na bache
    sort(begin(intervals), end(intervals)); // default sort (start pe)
    int start = 0, end = 1, count = 0;      // start = rakha hua, end = candidate
    while (end < intervals.size()) {
        // ★ <= (< nahi): sirf CHHOONA overlap nahi. [[1,2],[2,3]] pe < se 1 removal (sahi 0)
        if (intervals[start][1] <= intervals[end][0]) {   // NO overlap
            start = end;                    // candidate safe, rakho
            end++;
        }
        else {                              // OVERLAP
            count++;                        // ek to hatana hi
            if (intervals[start][1] > intervals[end][1])
                start = end;                // ★ chhota-END wala rakho: aage zyada jagah -> zyada fit
            end++;                          // agla candidate
        }
    }
    return count;                           // removals. O(n log n)
}
// EDGE: overlap pe BLINDLY start rakhna GALAT -> [[1,100],[2,3],[3,4]] pe 2 aata (sahi 1). end-compare
//   zaroori.
//   (alt: END pe sort -> pehla wala hamesha chhota-end -> ye if gayab)
```



████████████████████████████████████████████████

### ▸ GAS STATION (LC-134)  = greedy running-tank + reset-start

**SAWAAL** N petrol pump gol ghere me hain. gas[i] = pump i pe milne wala petrol, cost[i] = pump i se agle pump tak ka kharcha. Kis pump se shuru karke poora chakkar laga sakte ho, wo index lautao. Na laga sako to -1.

- **e.g.** `gas=[1,2,3,4,5], cost=[3,4,5,1,2] -> 3`
- **kyun:** index 3 se tank 3, 6, 4, 2, 0 -> kabhi minus nahi, chakkar poora

**DRY-RUN**
```
DRY-RUN  gas=[1,2,3,4,5]  cost=[3,4,5,1,2]
   idx:   p0  p1  p2  p3  p4
   net:   -2  -2  -2  +3  +3
   p0: tank -2 <0 -> tank=0, index=1     total -2
   p1: tank -2 <0 -> tank=0, index=2     total -4
   p2: tank -2 <0 -> tank=0, index=3     total -6
   p3: tank +3                           total -3
   p4: tank +6                           total  0
   total 0 >= 0 -> return index = 3
```

> [!IMPORTANT]
> **CHAABI:** 2 accumulator: `tank` (abhi wale start se; minus hua to `0` + `index = i + 1`) aur `total` (poore circle ka, kabhi reset nahi). `total < 0` -> -1, warna `index`.

> [!WARNING]
> **Galti:** reset pe `index = i`  ->  **Sahi:** `index = i + 1`
> **Kyun:** s..i ke beech kahin se bhi shuru karo, i paar nahi hota.
>
> **Galti:** `total` bhi reset  ->  **Sahi:** `total` kabhi reset nahi
> **Kyun:** wahi bataata poora chakkar possible hai ya nahi.

**CODE**
```cpp
int canCompleteCircuit(vector<int> &gas, vector<int> &cost) {
    int n = gas.size();
    int index = 0, meriGaddiKaTank = 0, kyeNegativeHua = 0;  // 0: pehle pump ko start maano, tank khaali
    for (int i = 0; i < n; i++) {
        meriGaddiKaTank += gas[i] - cost[i];   // abhi wale start se tank -> doobe to reset ("start
                                               //   kahan")
        kyeNegativeHua += gas[i] - cost[i];  // poore circle ka total, KABHI reset nahi ("possible hai?")
        if (meriGaddiKaTank < 0) {          // is start se pump i+1 tak nahi pahunche
            meriGaddiKaTank = 0;            // naya start, khaali tank
            index = i + 1;                  // ★ AGLE pump pe khiska do: s..i ke beech se shuru karke bhi
        }  //   i paar nahi hota (beech ka tank >= 0 tha, uske bina aur kam)
    }
    return kyeNegativeHua < 0 ? -1 : index;  // ★ total kam -> -1; warna LAST reset index
}
// gas=[1,2,3,4,5] cost=[3,4,5,1,2]: net -2 -2 -2 +3 +3 -> p0,p1,p2 pe reset, total 0 -> index 3
// cookies / jump / interval se FARAK: sort NAHI. circular + 2 accumulator (possible? + start kahan)
```



████████████████████████████████████████████████

### ▸ TASK SCHEDULER (LC-621)  = greedy IDLE-SLOTS (most-frequent frame)

**SAWAAL** tasks me kaam diye hain (har akshar ek kaam, har kaam 1 unit time). Ek hi kaam dobara karne se pehle beech me kam se kam n unit ka gap chahiye (khaali baith sakte ho). Saare kaam khatam karne me kam se kam kitna kul time lagega (khaali time bhi milake), wo lautao.

- **e.g.** `[A,A,A,B,B,B], n=2 -> 8`
- **kyun:** A B idle A B idle A B = 8 unit

### DRY-RUN

```
DRY-RUN: tasks=[A,A,A,B,B,B], n=2
   freq: A=3, B=3 -> maxFreq=3 (A frame banega)
   gadde = 3-1 = 2                 // A _ _ A _ _ A  -> 2 gaps
   idleSlots = n*gadde = 2*2 = 4   // 4 khali slots (_ _ _ _)
   B se bharo: min(3,2)=2 -> idleSlots = 4-2 = 2   // B har gap me 1 -> 2 slot bhare; 3rd B last column me A ke saath
   idleSlots=2 > 0 -> ans = 6 + 2 = 8
   picture:  A B idle A B idle A B    // 2 idle bache = wahi idleSlots
```

### TERMS: slot vs gadda (SAHI / GALAT tasveer)

```
   TERMS (mix mat kar — yahi confuse hua tha):
      _     = ek SLOT (ek unit time)
      _ _   = ek GADDA (do A ke BEECH ka poora gap) = n slots  (n=2 -> gadda me 2 slot)
      gadde = KITNE gadde/gap = maxFreq-1   (A _ _ A _ _ A  -> 2 gadde)
      total idle slots = gadde x n = 2 x 2 = 4     (gadda alag, slot alag!)
   SAHI :  A _ _ A _ _ A         (3 A -> gadde sirf BEECH me -> 2 = maxFreq-1)
   GALAT:  A _ _ A _ _ A _ _     (aakhri A ke baad gadda NAHI)
```

> [!IMPORTANT]
> **CHAABI:** sabse zyada wala task dhaancha: `gadde = maxFreq - 1`, `idle = n * gadde`, baaki har task se `min(mp[i], gadde)` ghatao; `idle > 0` -> `size + idle`, warna `size`.

> [!WARNING]
> **Galti:** `gadde = maxFreq` (`n * maxFreq`)  ->  **Sahi:** `gadde = maxFreq - 1`
> **Kyun:** gap sirf BEECH me (A _ _ A _ _ A); aakhri A ke baad koi agla A nahi, cooldown kisi ko nahi rokta, to idle bhi nahi.
>
> **Galti:** idle negative pe bhi `size + idle`  ->  **Sahi:** `idle > 0` tabhi `size + idle`, warna sirf `size`
> **Kyun:** `[A3,B3,C3,D2,E2]` n=3 pe idle -2 aata = saare gap bhar gaye, jawab 13 (length).

**CODE**
```cpp
int leastInterval(vector<char> &tasks, int n) {
    vector<int> mp(26);
    for (int i = 0; i < tasks.size(); i++)
        mp[tasks[i] - 'A']++;               // freq count (26-size)
    sort(begin(mp), end(mp));               // ascending: sabse badi freq aakhri index 25 pe
    int maxFreq = mp[25];
    int gadde = maxFreq - 1;  // ★ gaps sirf BEECH me: A _ _ A _ _ A (3 A -> 2). aakhri A ke baad
                                            // koi agla A nahi -> cooldown kisi ko nahi rokta (maxFreq
                                            //   NAHI)
    int idleSlotes = n * gadde;             // har gadde me n slot. gadda (gap) alag, slot (1 unit) alag
    for (int i = 24; i >= 0; i--)           // 24 se: 25 = maxFreq khud, use chhodo
        idleSlotes -= min(mp[i], gadde);  // ★ ek task har gadde me SIRF EK copy (2 = apna cooldown toota)
                                            //   -> max gadde slots bhar sakta; kam copies -> utne hi
    if (idleSlotes > 0)
        return tasks.size() + idleSlotes;   // kuch gap sach me KHAALI -> length + idle
    return tasks.size();                    // ★ <= 0: saare gap bhar gaye -> LENGTH wins
}
// [A,A,A,B,B,B] n=2: gadde 2, idle 4, B bhare min(3,2)=2 -> idle 2 -> 6+2 = 8  (A B idle A B idle A B)
// length-wins: [A3,B3,C3,D2,E2] n=3 -> idle -2 -> 13
// alt: max-heap by freq + cooldown-queue (jab "actual schedule" poochein)
```



████████████████████████████████████████████████

### ▸ PARTITION LABELS (LC 763)

**SAWAAL** String s (a-z). Jitne zyada tukde ho sakein utne kaato, shart: har akshar sirf EK tukde me aaye. Har tukde ki lambai lautao, kram me.

- **e.g.** `"ababcbacadefegdehijhklij" -> [9,7,8] · "eccbbbbdec" -> [10] · "abac" -> [3,1]`
- **kyun:** "ababcbaca" | "defegde" | "hijhklij" (a, b, c sirf pehle me) · e shuru me bhi aakhir me bhi -> poori ek

> [!IMPORTANT]
> **CHAABI:** pehle se har akshar ka AAKHRI index map me rakh lo, phir chalte-chalte `end = max(end, mp[akshar])`; `j` end pe pahunche to tukda khatam. Ye library me honi chahiye: on the spot ye trick 5-6 hint ke bina nahi banti (Arpan).

**CODE**
```cpp
vector<int> partitionLabels(string s) {
    int n = s.size();
    // sabki last occurrence store karlo, bas; har char ka last dekh ke end update karo, size nikal lo
    vector<int> mp(26, -1);
    for (int i = 0; i < n; i++) {
        int index = s[i] - 'a';
        mp[index] = i;
    }
    vector<int> ans;
    int i = 0;
    while (i < n) {
        int end = mp[s[i] - 'a'];
        int j = i;
        while (j < end) {
            end = max(end, mp[s[j] - 'a']);   // beech ka har akshar apna last laata, end aage khisakta
            j++;
        }
        ans.push_back(j - i + 1);
        i = j + 1;
    }
    return ans;
}
```

**DRY RUN: "ababcc"**
```
index:   0 1 2 3 4 5
s:       a b a b c c

mp banana (har akshar ka AAKHRI index, aage wala purane ko overwrite karta):
  i=0 a -> mp[a]=0 · i=1 b -> mp[b]=1 · i=2 a -> mp[a]=2 · i=3 b -> mp[b]=3 · i=4 c -> mp[c]=4 · i=5 c -> mp[c]=5
  ant me:  a=2  b=3  c=5

tukda 1: i=0, end = mp[a] = 2
  j=0 (a):  0 < 2 -> end = max(2, mp[a]=2) = 2
  j=1 (b):  1 < 2 -> end = max(2, mp[b]=3) = 3     <- b beech me aaya, uska aakhri 3 pe, end khiska
  j=2 (a):  2 < 3 -> end = max(3, mp[a]=2) = 3
  j=3:      3 < 3 nahi -> ruko
  lambai = j - i + 1 = 3 - 0 + 1 = 4   ("abab"),  i = 4

tukda 2: i=4, end = mp[c] = 5
  j=4 (c):  4 < 5 -> end = max(5, 5) = 5
  j=5:      5 < 5 nahi -> ruko
  lambai = 5 - 4 + 1 = 2   ("cc"),  i = 6 -> khatam

ans = [4, 2]
```
