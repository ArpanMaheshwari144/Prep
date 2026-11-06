## PATTERN 15 — INTERVALS

```
> ★ **YAHAN NAHI MILEGI:**
> ```
> non-overlapping intervals  ->  PATTERN 18 (GREEDY)
>       intervals hain, par sawaal "kam se kam kitne HATAO" = greedy choice
>       (sort by END, aur takraane wala chhodo)
> ``` (sort-by-start + overlap)

 BROAD IDEA: interval = [start,end]. dikha -> PEHLA reflex = SORT. phir left-to-right handle.
   REAL: calendar/meeting-scheduling, booking, resource-alloc.

 ★ DECISION — input same [start,end], SAWAAL (verb) decide karta kaunsa tool:
     "MERGE overlapping / combine"           -> sort by START + merge
     "MAX non-overlapping pick / min remove" -> GREEDY: sort by END, free ho to pick
     "kitne ek-SAATH / MIN rooms chahiye"    -> LINE-SWEEP (start +1, end -1, running-count ka MAX)
   greedy = wahi sort+loop, bas KEY = END (jaldi-khatam wali max room chhodti -> zyada fit).
      A[1,10] B[2,3] C[4,5] -> START-sort = 1 · END-sort = 2. END jeeta.

```

#### ── FAMILY: sort-by-start + overlap check (bas overlap pe ACTION aur boundary < / <= badalta) ──

```
 KAB: [start, end] diye · merge · meeting · kitne room · insert
```

### ▸ MERGE INTERVALS (LC-56)  = sort-by-start + merge

```
intervals[] me [start, end] diye hain. Jo intervals aapas me overlap karte hain (ya bas chhoote hain), unhe jod ke ek kar do. Jude hue intervals ki list lautao.
  e.g. [[1,3],[2,6],[8,10],[15,18]] -> [[1,6],[8,10],[15,18]]
  kyun: 2 <= 3 -> [1,3] aur [2,6] overlap -> [1,6]; baaki alag
```

```
PURANI SHEET SE (same to same):

SAAR : sort by start -> ans me first daalo -> har next OVERLAP? merge : push.
TEMPLATE:
    sort(intervals.begin(), intervals.end());     // default FIRST-element (start) se sort (pair jaisa)
    for(i=1..n){  // 1 se: intervals[0] pehle hi ans me
        if(intervals[i][0] <= ans.back()[1])       // OVERLAP: curr.start <= last.END (<= : [1,3],[3,5] chhoote bhi merge hote)
            ans.back()[1] = max(ans.back()[1], intervals[i][1]);   // merge -> bada end (max zaroori: [1,10],[2,3] me end 10 hi rahe)
DRY-RUN  [[1,3],[2,6],[8,10],[15,18]]  (sorted):
    ans=[[1,3]]
    [2,6]  : 2 <= 3  overlap -> end=max(3,6)=6 -> ans=[[1,6]]
    [8,10] : 8 <= 6? NO      -> push          -> ans=[[1,6],[8,10]]
    [15,18]: 15 <=10? NO     -> push          -> ans=[[1,6],[8,10],[15,18]]
ans.back() = last interval; [0]=start [1]=END.  overlap = curr.start last.END ko chhoo/cross kare. sort-by-start HAMESHA pehla step.
```

```cpp
vector<vector<int>> merge(vector<vector<int>> &intervals) {
    sort(begin(intervals), end(intervals)); // ★ default FIRST element (start) se. HAMESHA pehla step
    vector<vector<int>> ans;
    ans.push_back(intervals[0]);            // pehla baseline
    for (int i = 1; i < intervals.size(); i++) {   // 1 se: [0] pehle hi ans me
        if (ans.back()[1] >= intervals[i][0])   // ★ OVERLAP: curr.start <= last.END
                                            //   (<= : [1,3],[3,5] chhoote bhi merge)
            ans.back()[1] = max(ans.back()[1], intervals[i][1]);   // ★ max zaroori: [1,10],[2,3] -> 10
        else
            ans.push_back(intervals[i]);    // no overlap -> naya
    }
    return ans;                             // ans.back() = last; [0]=start [1]=END
}   // [[1,3],[2,6],[8,10],[15,18]] -> [[1,6],[8,10],[15,18]]
```

### ▸ MEETING ROOMS (LC-252, Easy)  = Merge ka SAME template, bas action alag

```
meetings[] me [start, end] diye hain. Kya ek insaan saari meetings attend kar sakta hai, matlab koi do meeting takraati nahi? true ya false lautao.
  e.g. [[0,30],[5,10],[15,20]] -> false
  kyun: 5-10 shuru hui jab 0-30 abhi chal rahi (5 < 30) -> overlap

MERGE: overlap -> end max, return list · MEETING: overlap -> return false, end me true
```

```
PURANI SHEET SE (same to same):

SAAR : saare meeting attend ho sakte? = koi 2 OVERLAP na karein. (Merge ka SAME skeleton reuse.)
SAME as MERGE (sort + ans.push(first) + loop overlap-check) -- bas overlap pe KYA karo:
     MERGE:   overlap -> ans.back()[1]=max(...)  (merge)  |  end: return ANS (list)
     MEETING: overlap -> return FALSE           (ruko)    |  end: return TRUE
TEMPLATE (merge-jaisa hi):
    if (intervals.empty()) return true;   // ★ EDGE: warna intervals[0] pe crash
    sort(intervals);  ans.push_back(intervals[0]);   // start se sort, pehla interval baseline
    for(i=1..n){   // 1 se: intervals[0] pehle hi ans me
        if(intervals[i][0] < ans.back()[1]) return false;   // overlap -> attend nahi (< kyunki ek khatam usi pal doosri shuru = chalta)
        else ans.push_back(intervals[i]);   // push: agla overlap isi ke end se check hoga
    return true;   // koi overlap nahi mila -> saari attend
boundary FARAK (crux): Merge touching-MERGE -> `<=`  ·  Meeting touching-OK -> `<`.
ek pattern (sort+overlap) -> bas (a) overlap pe action (merge vs stop) + (b) boundary (< vs <=) badalta. PROBLEM SAMJHO -> decide.
```

```cpp
bool canAttendMeetings(vector<vector<int>> &intervals) {
    if (intervals.empty())
        return true;                        // EDGE: warna intervals[0] pe crash
    sort(intervals.begin(), intervals.end());   // start se sort
    vector<vector<int>> ans;
    ans.push_back(intervals[0]);            // pehla baseline
    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] < ans.back()[1])   // ★ < (<= NAHI): ek khatam usi pal doosri shuru = chalta
            return false;                   // 2 meeting takraye -> saare attend nahi
        else
            ans.push_back(intervals[i]);    // agla overlap isi ke end se check
    }
    return true;                            // koi overlap nahi
}   // boundary farak: Merge touching = MERGE (<=) · Meeting touching = OK (<)
```

### ▸ INSERT INTERVAL (LC-57)  = already-SORTED -> 3 phase (no sort)

```
intervals[] pehle se sorted hain aur aapas me overlap nahi karte. Ek newInterval daalna hai (overlap ho to jod do). Nayi sorted list lautao.
  e.g. [[1,3],[6,9]], new=[2,5] -> [[1,5],[6,9]]
  kyun: [2,5] aur [1,3] overlap -> [1,5]; [6,9] 5 ke baad -> alag

pehle se sorted + non-overlap -> SORT nahi, ek pass. newInterval = "GROWING BOX"
```

```
PURANI SHEET SE (same to same):

SAAR : intervals pehle se sorted+non-overlap. new insert karo (overlap->merge). SORT nahi -> ek pass, 3 phase.
KEY : new = "GROWING BOX" -- har overlap use STRETCH karta; sab overlap khatam pe ek baar push.
TEMPLATE:
    vector<vector<int>> ans;  int count=0;   // count=0: kitne process hue, doosra loop wahin se chalega
    for(i=0..n){
        if(intervals[i][1] < new[0])       ans.push_back(intervals[i]);   // 1. new se PEHLE khatam -> push (sirf < : chhoota hua merge hoga)
        else if(intervals[i][0] > new[1])  break;                          // 3. new ke AAGE -> BREAK (sorted, aage sab door)
        else { new[0]=min(new[0],intervals[i][0]);                         // 2. OVERLAP -> new ko GROW
               new[1]=max(new[1],intervals[i][1]); }   // end bhi max se: dono me jo aage tak jaaye
        count++;                                                           // break wala yahan nahi aata -> count se doosra loop use daalega
    ans.push_back(new);                                                   // 4. bada-hua new daalo
    for(i=count..n) ans.push_back(intervals[i]);                          //    phir count se aage baaki
break-optimization: interval.start > new.end -> aage sab aur door (sorted) -> ruk. 'count'=kahan ruke -> doosra loop wahin se.
Merge/Meeting me SORT + ans.push(first);  yahan sorted-diya -> SORT nahi, bas 3-phase (before / grow / after).

PEHLI SHEET (Aug) se bhi, same to same:

SAAR : intervals pehle se sorted+non-overlap. new insert karo (overlap->merge). SORT nahi -> ek pass, 3 phase.
KEY : new = "GROWING BOX" -- har overlap use STRETCH karta; sab overlap khatam pe ek baar push.
TEMPLATE:
    vector<vector<int>> ans;  int count=0;
    for(i=0..n){
        if(intervals[i][1] < new[0])       ans.push_back(intervals[i]);   // 1. new se PEHLE khatam -> push
        else if(intervals[i][0] > new[1])  break;                          // 3. new ke AAGE -> BREAK (sorted, aage sab door)
        else { new[0]=min(new[0],intervals[i][0]);                         // 2. OVERLAP -> new ko GROW
               new[1]=max(new[1],intervals[i][1]); }
        count++;                                                           // kitne process hue (break se pehle ruk)
    }
    ans.push_back(new);                                                   // 4. bada-hua new daalo
    for(i=count..n) ans.push_back(intervals[i]);                          //    phir count se aage baaki
    return ans;
break-optimization: interval.start > new.end -> aage sab aur door (sorted) -> ruk. 'count'=kahan ruke -> doosra loop wahin se.
Merge/Meeting me SORT + ans.push(first);  yahan sorted-diya -> SORT nahi, bas 3-phase (before / grow / after).
```

```cpp
vector<vector<int>> insert(vector<vector<int>> &intervals, vector<int> &newInterval) {
    vector<vector<int>> ans;
    int count = 0;                          // kitne process hue, doosra loop wahin se
    for (int i = 0; i < intervals.size(); i++) {
        if (intervals[i][1] < newInterval[0])   // ★ 1. new se PEHLE khatam -> push
            ans.push_back(intervals[i]);        //    (sirf < : chhoota hua merge hoga)
        else if (intervals[i][0] > newInterval[1])
            break;                          // ★ 3. new ke AAGE -> BREAK (sorted, aage sab door)
        else {                              // 2. OVERLAP -> new ko GROW
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);   // ★ end bhi max
        }
        count++;                            // break wala yahan nahi aata -> count se doosra loop
    }
    ans.push_back(newInterval);             // 4. bada hua new daalo
    for (int i = count; i < intervals.size(); i++)
        ans.push_back(intervals[i]);        //    phir count se aage baaki
    return ans;
}
```

#### ── FAMILY: SWEEP LINE (events + running count: kitna overlap, sirf "hai?" nahi) ──

### ▸ MEETING ROOMS II (LC-253)  = SWEEP LINE / event-count

```
start[] aur end[] me meetings ke shuru aur khatam hone ka time diya hai. Kam se kam kitne room chahiye, wo lautao.
Ek meeting ka end aur doosri ka start same time ho, to wahi room chal jaata hai.
  e.g. start=[2,9,6], end=[4,12,10] -> 2
  kyun: t=9 pe [9,12] aur [6,10] dono chal rahi -> 2 room

min ROOMS = kisi ek pal pe MAX kitni meeting saath. rooms object mat gino -> +1 / -1 GINTI
```

```
PURANI SHEET SE (same to same):

PURANI SHEET SE (same to same):

NAYA-SOCH (merge/meeting-I se ALAG): rooms ko OBJECT mat gino -> +1/-1 GINTI karo.
    meeting shuru = +1 (room bhara) · khatam = -1 (room khaali). running-sum ka MAX = answer.
TOOL = ORDERED MAP (map<int,int> = time->delta; keys KHUD sorted -> manual sort NAHI):
    map<int,int> mp;
    for(i=0..n){ mp[start[i]]++;  mp[end[i]]--; }        // shuru +1, khatam -1 (map: same time pe +1 -1 kat ke room reuse)
    int overlap=0, maxOverlap=0;   // 0 se: shuru me koi meeting chal nahi rahi
    for(auto &it : mp){                                   // keys sorted -> order me
        overlap += it.second;   // +1/-1 jodo = is pal kitni meeting chal rahi
        if(maxOverlap < overlap) maxOverlap = overlap;   // peak yaad rakho = rooms
    }
    return maxOverlap;
DRY-RUN  start=[2,9,6] end=[4,12,10]:
    mp (key-sorted): {2:+1, 4:-1, 6:+1, 9:+1, 10:-1, 12:-1}
    t=2(+1)->ovl=1 mx=1 | t=4(-1)->0 | t=6(+1)->1 | t=9(+1)->2 mx=2 | t=10->1 | t=12->0
    maxOverlap = 2  -> 2 rooms
SWEEP VISUAL (vertical line jhaadu t=9 pe = 2 bars overlap = PEAK):
         0    5    10
                  ┃            <- jhaadu (sweep line) t=9 pe
  [2,4]    ██     ┃            t=2 shuru(+1) · t=4 khatam(-1)  (line se pehle nikal gayi)
  [6,10]       ███┃            t=6 shuru(+1) · t=10 khatam(-1) (line ANDAR se guzri)
  [9,12]          ┃██          t=9 shuru(+1) · t=12 khatam(-1) (line ke pe shuru)
                  ┃
                  ▲
               t=9 pe [6,10]+[9,12] dono ANDAR = 2 bars = PEAK = 2 rooms
TIE (start==prev end): same key pe +1 aur -1 -> net 0 -> room REUSE free (jhoothi peak nahi).
FARAK merge se: merge/meeting-I = "kya overlap?" (join/bool). MR-II = "KITNA overlap?" (peak count) -> sweep.
FAMILY: sweep-line / "events + running count" -- interval-overlap, max-concurrent, CPU-load, planes-in-sky.
★ YAHAN NAHI MILEGA: iska SAGAA BHAI = DIFFERENCE ARRAY (LC-1109) -> PATTERN 4 (PREFIX SUM) me hai.
   wahi machine, bas: map ki jagah ARRAY · end ki jagah END+1 (inclusive) · max ki jagah POORA array.

SAAR : min ROOMS = kisi bhi ek PAL pe MAX kitni meeting ek-saath (peak overlap).
         0    5    10
                  ┃
                  ▲
```

```cpp
int minMeetingRooms(vector<int> &start, vector<int> &end) {
    map<int, int> mp;                       // ★ ORDERED map: time -> delta, keys khud sorted
    int n = start.size();
    for (int i = 0; i < n; i++) {
        mp[start[i]]++;                     // shuru = +1 (room bhara)
        mp[end[i]]--;                       // ★ khatam = -1. same time pe +1 -1 kat -> room REUSE
    }
    int overlap = 0, maxOverlap = 0;        // 0 se: shuru me koi meeting nahi
    for (auto &it : mp) {                   // keys sorted -> time ke order me
        overlap += it.second;               // is pal kitni meeting chal rahi
        if (overlap > maxOverlap)
            maxOverlap = overlap;           // peak = rooms
    }
    return maxOverlap;
}   // start=[2,9,6] end=[4,12,10]: t=9 pe [6,10]+[9,12] = 2 -> 2 rooms
// FAMILY: max-concurrent, CPU-load, planes-in-sky. SAGAA BHAI = DIFFERENCE ARRAY (LC-1109)
//   -> PATTERN 4: map ki jagah ARRAY · end ki jagah END+1 (inclusive) · max ki jagah POORA array
```

