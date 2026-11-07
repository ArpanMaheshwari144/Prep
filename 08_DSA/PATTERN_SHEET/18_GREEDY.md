## PATTERN 18 — GREEDY

```
 BROAD IDEA: har step pe ABHI-KA-BEST choice pakdo (local best), aage ki poori planning nahi.
   Kaam karta SIRF jab abhi-ka-best = aage-bhi-best (greedy-choice property); warna DP (jo sab explore kare).
   SIGNATURE: aksar pehle SORT (kisi key pe: size / finish-time) -> phir ek LINEAR pass me pick / skip.

 KAB: har step pe abhi ka best chuno aur wo aage bhi best rahe · aksar pehle SORT, phir ek pass
 ★ DHYAAN: intervals me "kam se kam hatao" -> END pe sort
 ★ DHYAAN: gas station: tank minus hua -> agla station naya start
```

### ▸ ASSIGN COOKIES (LC-455)  = greedy intro (sort + 2-pointer)

```
g[] me har bachche ki bhookh (greed) aur s[] me har cookie ka size diya hai. Bachcha khush tab hota hai jab cookie ka size uski bhookh se kam na ho, aur ek bachche ko zyada se zyada ek cookie milti hai.
Zyada se zyada kitne bachche khush kar sakte ho, wo lautao.
  e.g. g=[1,2,3], s=[1,1] -> 1
  kyun: dono cookie size 1 -> sirf greed-1 wala khush

chhoti greed wale ko jitni ZAROORAT utni (chhoti se chhoti jo kaam kare) -> badi cookies bade greed ko
```

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

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
g[] = bachhe ki GREED, s[] = cookie SIZE. cookie j bachhe i ko khush karti: s[j] >= g[i]. max kitne bachhe khush?
GREEDY soch: chhoti-greed wale bachhe ko jitni-ZAROORAT-utni (chhoti-se-chhoti jo kaam kare) cookie do
             -> badi cookies bade-greed walon ke liye bachi rahein.
dono SORT -> 2-pointer (i=bachha, j=cookie): // while(i<n && j<m): koi ek khatam -> ruk
   else                j++;                    // cookie choti -> badi dhoondho
count = ans.  (s khali -> 0 return)
```

### ▸ JUMP GAME (LC-55)  = greedy REACHABILITY (farthest-reach track)

```
nums[i] batata hai ki index i se zyada se zyada kitne kadam aage kood sakte ho. Index 0 se shuru karke kya aakhri index tak pahunch sakte ho? true ya false lautao.
  e.g. [2,3,1,1,4] -> true · [3,2,1,0,4] -> false
  kyun: 0->1->4 pahunche · doosre me har raasta index 3 (value 0) pe atakta
```

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

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
nums[i] = us index se max kitni door kood sakte. index0 se LAST tak pahunch sakte ho? true/false.
GREEDY soch: har index pe track karo "ab tak sabse DOOR kahan tak pahunch sakta hoon" (maxReach).
   agar koi index i KHUD maxReach se aage nikal gaya -> wahan pahunch hi nahi sakte -> FALSE.
   maxReach = max(maxReach, i + nums[i]);       // apni pahunch badhao
★ cookies se FARAK: wahan sort+2-pointer; yahan sort nahi -> ek scan me "farthest reachable" track. greedy ke 2 alag rang.
```

### ▸ NON-OVERLAPPING INTERVALS (LC-435)  = greedy interval-scheduling (keep SMALLER-END)

```
intervals[] diye hain. Kam se kam kitne interval hatane padenge taaki bache hue koi bhi do interval overlap na karein, wo lautao. Sirf chhoona (ek ka end = doosre ka start) overlap nahi maana jaata.
  e.g. [[1,2],[2,3],[3,4],[1,3]] -> 1
  kyun: [1,3] hatao -> [1,2],[2,3],[3,4] sirf chhoote, overlap nahi
```

```
YAAD:

PURANI SHEET KI LINE (git 07e45c2b):
★ <= (< nahi): sirf CHHOONA overlap nahi. [[1,2],[2,3]] pe < se 1 removal (sahi 0).
```

```cpp
int eraseOverlapIntervals(vector<vector<int>> intervals) {   // MIN kitne HATAO ki overlap na bache
    sort(begin(intervals), end(intervals)); // default sort (start pe)
    int start = 0, end = 1, count = 0;      // start = rakha hua, end = candidate
    while (end < intervals.size()) {
        if (intervals[start][1] <= intervals[end][0]) {   // ★ NO overlap (<= : chhoona overlap nahi)
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

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
intervals me se MIN kitne HATAO taaki koi overlap na bache? (= max non-overlapping rakho.)
GREEDY: default SORT. 2-pointer -> start=rakha-hua, end=candidate. // start=0, end=1; while(end < n)
                    end++;   // agla candidate dekho
★ EDGE (dry-run se pakda): overlap pe BLINDLY start rakhna GALAT -> [[1,100],[2,3],[3,4]] pe 2 aata (sahi 1).
  end-compare zaroori. (alt: END pe sort karo -> pehla-waala hamesha chhota-end -> ye if-check gayab.)
PEHLI SHEET (Aug) se bhi, same to same:
intervals me se MIN kitne HATAO taaki koi overlap na bache? (= max non-overlapping rakho.)
GREEDY: default SORT. 2-pointer -> start=rakha-hua, end=candidate.
★ EDGE (dry-run se pakda): overlap pe BLINDLY start rakhna GALAT -> [[1,100],[2,3],[3,4]] pe 2 aata (sahi 1).
  end-compare zaroori. (alt: END pe sort karo -> pehla-waala hamesha chhota-end -> ye if-check gayab.)
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     intervals[]. MIN kitne hatao ki baaki koi overlap na kare (touching chalta).
```

### ▸ GAS STATION (LC-134)  = greedy running-tank + reset-start

```
N petrol pump gol ghere me hain. gas[i] = pump i pe milne wala petrol, cost[i] = pump i se agle pump tak ka kharcha.
Kis pump se shuru karke poora chakkar laga sakte ho, wo index lautao. Na laga sako to -1.
  e.g. gas=[1,2,3,4,5], cost=[3,4,5,1,2] -> 3
  kyun: index 3 se tank 3, 6, 4, 2, 0 -> kabhi minus nahi, chakkar poora

  chalu ki poora ghoom ke wapas aa jau? koi nahi -> -1. har pump ka NET = gas[i] - cost[i]
```

```
TASVEER:

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

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
har pump ka NET = gas[i] - cost[i]. Ek pass, DO accumulator:
   kyaNegativeHua  = poore circle ka total   -> KABHI reset nahi  -> sirf "possible hai ya nahi"
int index = 0, meriGaddiKaTank = 0, kyaNegativeHua = 0;   // 0 se: pehle pump ko start maano, tank khaali
    kyaNegativeHua  += gas[i] - cost[i];   // poore circle ka total, kabhi reset nahi
return kyaNegativeHua < 0 ? -1 : index;  // total kam = petrol hi kam -> -1
★ KEY: total >= 0 ho to LAST reset wala index hi answer. kyun: start s se pump i pe tank dooba
  -> s aur i ke beech ka koi bhi pump start bana ke bhi i paar nahi hota (beech ka tank >= 0 hi tha,
  uske bina aur kam padta) -> seedha i+1 pe kood.
PEHLI SHEET (Aug) se bhi, same to same:
N pump CIRCLE me. gas[i]=milta, cost[i]=agle pump tak kharcha. kaha se shuru karu
ki poora ghoom ke wapas wahin aa jau? koi start possible nahi -> -1.
har pump ka NET = gas[i] - cost[i]. Ek pass, DO accumulator:
   kyaNegativeHua  += gas[i]-cost[i];   // KABHI reset nahi -> ant me <0 -> -1 (petrol hi kam)
   meriGaddiKaTank += gas[i]-cost[i];   // running; jaha doobta wahan se aage nahi ja sakte
return  kyaNegativeHua < 0 ? -1 : index;
★ KEY: kyaNegativeHua>=0 ho to jo LAST reset-index bacha wahi ANSWER (unique). kyun: jis pump pe
  tank dooba, us-se-pehle kisi bhi start se wahan pahucha ja HI nahi sakta -> start hamesha aage hi.
★ cookies/jump/interval se FARAK: yahan sort NAHI. circular + 2-accumulator (feasibility + start-track).
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     N pump gol me. gas[i] = milta, cost[i] = agle tak kharcha. kis index se poora chakkar lagega? na lage -> -1.
```

### ▸ TASK SCHEDULER (LC-621)  = greedy IDLE-SLOTS (most-frequent frame)

```
tasks me kaam diye hain (har akshar ek kaam, har kaam 1 unit time). Ek hi kaam dobara karne se pehle beech me kam se kam n unit ka gap chahiye (khaali baith sakte ho).
Saare kaam khatam karne me kam se kam kitna kul time lagega (khaali time bhi milake), wo lautao.
  e.g. [A,A,A,B,B,B], n=2 -> 8
  kyun: A B idle A B idle A B = 8 unit

SAME task dobara se pehle n gap. minimum total TIME (idle bhi)? signal: "cooldown" + "minimum time"
  -> greedy (most-frequent frame) YA max-heap. sabse ZYADA wala task (maxFreq) DHANCHA banata,
  beech ke gaps doosre tasks se bharo, khaali bache = IDLE
```

```
YAAD:

DRY-RUN: tasks=[A,A,A,B,B,B], n=2
   freq: A=3, B=3 -> maxFreq=3 (A frame banega)
   gadde = 3-1 = 2                 // A _ _ A _ _ A  -> 2 gaps
   idleSlots = n*gadde = 2*2 = 4   // 4 khali slots (_ _ _ _)
   B se bharo: min(3,2)=2 -> idleSlots = 4-2 = 2   // B har gap me 1 -> 2 slot bhare; 3rd B last column me A ke saath
   idleSlots=2 > 0 -> ans = 6 + 2 = 8
   picture:  A B idle A B idle A B    // 2 idle bache = wahi idleSlots
★ length-WINS edge: [A3,B3,C3,D2,E2] n=3 -> idleSlots -2 (<=0) -> ans = 13 (saare gap bhar gaye, idle 0).

★★ GOTCHA — gadde = maxFreq-1 KYUN (maxFreq nahi)?   [ye confusion 12-Sep REDO pe hui]
   TERMS (mix mat kar — yahi confuse hua tha):
      _     = ek SLOT (ek unit time)
      _ _   = ek GADDA (do A ke BEECH ka poora gap) = n slots  (n=2 -> gadda me 2 slot)
      gadde = KITNE gadde/gap = maxFreq-1   (A _ _ A _ _ A  -> 2 gadde)
      total idle slots = gadde x n = 2 x 2 = 4     (gadda alag, slot alag!)
   SAHI :  A _ _ A _ _ A         (3 A -> gadde sirf BEECH me -> 2 = maxFreq-1)
   GALAT:  A _ _ A _ _ A _ _     (aakhri A ke baad gadda NAHI)
   KYUN: cooldown = "AGLA same-task chalane se pehle gap" -> wo AGLE ko rokta hai.
         Aakhri A ke baad koi agla A nahi -> cooldown kisi ko rok nahi raha -> koi idle nahi.
         (dawa: aakhri goli kha li -> bottle khali -> 2 ghante wait nahi, kaam khatam.)
   + problem "FINISH tak minimum time" poochta -> aakhri task pe clock STOP.
   => gaps = (count-1) = maxFreq-1 = gadde.  n*maxFreq lena = zyada idle = GALAT.
```

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

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
tasks (letters) + cooldown n: SAME task dobara chalane se pehle n gap chahiye. minimum total TIME (idle included)?
BADI SOCH: sabse ZYADA baar aane wala task (maxFreq) poori timeline ka DHANCHA banata.
   usko n-gap ke saath rakho -> beech ke gaps doosre tasks/idle se bharo. jo gap khali bache = IDLE.
STEP-BY-STEP (har line ka KYUN):
  2. gadde = maxFreq - 1                 // maxFreq copies rakho to unke BEECH itne gap bante
                                         //   A _ _ A _ _ A  (3 A -> 2 gaps). WHY -1: gaps hamesha (count-1).
        idleSlots -= min(mp[i], gadde)   // WHY min(count, gadde): ek task har gap me SIRF EK copy daal sakta
                                         //   (2 same ek gap me = apna cooldown tootega). gadde gaps hai ->
  5. idleSlots > 0 ? tasks.size()+idleSlots : tasks.size()
                                         // <=0 = saare gap bhar gaye (bahut distinct tasks) -> koi idle nahi -> LENGTH wins
(alt approach: max-heap by freq + cooldown-queue — har cycle sabse frequent pehle. heap = intuitive backup jab "actual schedule" poochein.)
```
