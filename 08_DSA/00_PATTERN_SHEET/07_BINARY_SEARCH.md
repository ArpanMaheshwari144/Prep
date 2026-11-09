## PATTERN 7 — BINARY SEARCH

```
 BROAD IDEA: sorted space -> mid -> aadha kaato -> O(log n). (space array ho ya ANSWER-range).
 BS ko poori sorted array nahi chahiye: bas ek comparison se "kaunsa half phenko" tay ho (find-peak bhi).
 BOUNDARY RULE: jis boundary pe element KHUD answer ho sakta -> equality INCLUDE (>= / <=).
    jahan mid==target pehle hi return ho gaya -> < ya <= farak nahi. galat >/< = off-by-one -> chhota dry-run.
 ★ EDGE (BS on answer): lo = max(arr) (isse kam me bhaari item uthta hi nahi), hi = sum(arr).
         ship-in-D-days [3,10], D=2: lo=0 rakha -> 5 aata, sahi 10.

 high=mid  vs  high=mid-1 (EK SAWAAL: "mid KHUD answer ho sakta abhi bhi?"):
    TARGET-REJECT (basic / rotated search: mid != target pakka)   -> high = mid-1  + while (low <= high)
    KHUD-ANSWER (find-peak / find-min-rotated, koi target nahi)  -> high = mid    + while (low < high)
    high=mid + while(low<=high) = INFINITE LOOP: mid neeche round, low==high==mid pe range shrink nahi
       ([2,1] peak-logic: mid=0 -> high=0 -> low=high=0 kabhi khatam nahi)
 3 broad trick: BS-on-ANSWER (koko) · sorted-half-check (rotated) · 2D-index-map (matrix).
```

#### ── FAMILY: BASIC / BOUNDARY (target ko mid se compare, low<=high, mid±1; mila to ruko mat = boundary) ──

```
 KAB: SORTED + ek jagah / threshold dhoondhna · "MIN / MAX value jisme condition sach" (answer pe BS)
```

---

### ▸ BINARY SEARCH (BASIC)

**SAWAAL**
```
Ek SORTED array aur ek target diya hai. target kis index pe hai wo lautao, na mile to -1. O(log n) me karna hai.
  e.g. [-1,0,3,5,9,12], target=9 -> 4
  kyun: 9 index 4 pe baitha hai (-1,0,3,5 ke baad)
```

> [!IMPORTANT]
> **CHAABI:** `while (low <= high)`, `mid = low + (high - low) / 2`. `nums[mid] == target` -> return mid; chhota -> `low = mid + 1`; bada -> `high = mid - 1`. Bahar -> -1.

> [!WARNING]
> `<=` (`<` nahi): `[5], 5` pe `<` se loop chalta hi nahi -> -1, sahi 0. `(low + high) / 2` nahi: bade low+high pe overflow.

**CODE**
```cpp
int search(vector<int> &nums, int target) {
    int low = 0, high = nums.size() - 1;
    // ★ <= (< nahi): low==high wala aakhri ek element bhi check. [5], 5 -> < se loop chalta hi nahi -> -1 (sahi 0)
    while (low <= high) {
        int mid = low + (high - low) / 2;   // ★ (low+high)/2 nahi: bade low+high pe overflow
        if (nums[mid] == target)
            return mid;
        else if (nums[mid] < target)
            low = mid + 1;                  // target right me; mid khud nahi -> +1
        else
            high = mid - 1;                 // target left me; mid khud nahi -> -1
    }
    return -1;                              // loop ke bahar: nahi mila
}
```


---

### ▸ SEARCH INSERT POSITION  = BASIC BS, na mile to low (daalne ki jagah)

**SAWAAL**
```
Ek SORTED array (sab alag number) aur ek target diya hai. target mile to uska index lautao.
Na mile to wo index lautao jahan target daalne se array sorted bana rahe.
  e.g. [1,3,5,6], target=2 -> 1
  kyun: 2 na mila, 1 aur 3 ke beech jaayega -> index 1
```

> [!IMPORTANT]
> **CHAABI:** BASIC BS hubahu; na mile to `return low` (low = pehla `nums >= target` = daalne ki jagah).

> [!WARNING]
> Na mile to -1 nahi, `low` lautao: `[1,3,5,6], 2` -> loop ke baad low = 1.

**CODE**
```cpp
int searchInsert(vector<int> &nums, int target) {   // [1,3,5,6], 2 -> 1 (1 aur 3 ke beech)
    int low = 0, high = nums.size() - 1;
    while (low <= high) {                   // basic BS hubahu
        int mid = low + (high - low) / 2;
        if (nums[mid] == target)
            return mid;
        else if (nums[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low;                             // ★ na mila: low = pehla nums >= target = insert jagah
}
```


---

### ▸ FIRST & LAST POSITION (LC-34)  = BASIC BS x2, mila to ruko mat (ek taraf aur chalo)

**SAWAAL**
```
Ek SORTED array (number dohraye ho sakte hain) aur ek target diya hai. target jahan pehli baar aur aakhri baar aata hai, wo dono index lautao.
Na mile to {-1, -1}. O(log n) me karna hai.
  e.g. [5,7,7,8,8,10], target=8 -> {3,4}
  kyun: 8 index 3 aur 4 pe hai -> pehla 3, aakhri 4

[5,7,7,8,8,10], 8 -> {3,4}. normal BS, bas MILNE pe RUKO MAT -> ans likho, ek taraf aur chalo. 2 BS
```

**SAAR**
```
DRY-RUN ([5,7,7,8,8,10], target 8):
  idx :  0  1  2  3  4  5
  val :  5  7  7  8  8 10
  PASS 1: l0 h5 mid2 (7<8)  -> l=3
          l3 h5 mid4 (8==8) -> ans[0]=4, h=3
          l3 h3 mid3 (8==8) -> ans[0]=3, h=2   -> ruk -> FIRST 3
  PASS 2: l0 h5 mid2 (7<8)  -> l=3
          l3 h5 mid4 (8==8) -> ans[1]=4, l=5
          l5 h5 mid5 (10>8) -> h=4             -> ruk -> LAST 4
  -> {3,4}

DHYAAN:
  1. dono pass ke beech low/high RESET.
  2. mila to sirf EK pointer hilao (FIRST: high=mid-1 · LAST: low=mid+1).
  3. ans(2,-1) + index set karo, push_back mat milao.
```

> [!IMPORTANT]
> **CHAABI:** Do BS. Mila to RUKO MAT: PASS 1 (FIRST) `ans[0] = mid, high = mid - 1` · PASS 2 (LAST) `ans[1] = mid, low = mid + 1`. Baaki basic BS.

> [!WARNING]
> Dono pass ke beech `low = 0, high = n-1` RESET (warna PASS 2 galat jagah se shuru). Mila to sirf EK pointer hilao. `ans(2,-1)` + index set, `push_back` nahi.

**CODE**
```cpp
vector<int> searchRange(vector<int> &nums, int target) {
    vector<int> ans(2, -1);                 // size fixed 2 -> ans(2,-1) + index set, push_back NAHI
    int low = 0, high = nums.size() - 1;    // n-1: aakhri valid index, dono sire inclusive
    while (low <= high) {                   // PASS 1 = FIRST
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            ans[0] = mid;
            high = mid - 1;                 // ★ mila -> aur LEFT (mid save, usse pehle wala dhoondo)
        }
        else if (nums[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    low = 0, high = nums.size() - 1;        // ★ RESET, warna PASS 2 galat jagah se shuru
    while (low <= high) {                   // PASS 2 = LAST
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            ans[1] = mid;
            low = mid + 1;                  // mila -> aur RIGHT (sirf EK pointer hilao)
        }
        else if (nums[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return ans;                             // PASS 1: mid 4 -> 3 = FIRST 3 · PASS 2: mid 4 -> LAST 4
}
```


#### ── FAMILY: ROTATED (ek comparison se kaunsa half sorted / pivot kidhar, wahi half rakho) ──

### ▸ SEARCH IN ROTATED (LC-33)  = rotated sorted array me target, O(log n)

**SAWAAL**
```
Ek sorted array (sab alag number) ko kisi jagah se ghuma diya gaya hai, jaise [4,5,6,7,0,1,2]. Usme target ka index lautao, na mile to -1. O(log n) me.
  e.g. [4,5,6,7,0,1,2], target=0 -> 4
  kyun: [0,1,2,4,5,6,7] ghum ke [4,5,6,7,0,1,2] bana, 0 ab index 4 pe hai
```

> [!IMPORTANT]
> **CHAABI:** `nums[mid] == target` -> return. `nums[low] <= nums[mid]` -> LEFT sorted: target `>= nums[low] && <= nums[mid]` to `high = mid - 1`, warna `low = mid + 1`. Nahi to RIGHT sorted, wahi ulta (`>= nums[mid] && <= nums[high]`).
>
> BOUNDARY RULE (dry-run se pakda): jahaan boundary KHUD valid answer ho sakti (nums[low]/nums[high]) -> >= / <= ZARURI (equality include).
> jahaan mid hai (already returned) -> < ya <= farak NAHI.

> [!WARNING]
> `nums[low] <= nums[mid]` me `<=` (`<` nahi): `[3,1], 1` pe `<` se -1, sahi 1. Range me `>= nums[low]` ZAROORI (target low pe khud ho sakta): `[5,1,3], 5` miss.

**CODE**
```cpp
int search(vector<int> &nums, int target) { // [4,5,6,7,0,1,2] me target ka index, nahi -> -1
    int n = nums.size();
    int low = 0;
    int high = n - 1;
    while (low <= high) {                   // <= : akela element bacha tab bhi check
        int mid = low + (high - low) / 2;
        if (nums[mid] == target)
            return mid;
        // ★ <= (< nahi): 2 bache to low==mid, left half = sirf mid, wo sorted hi hai. [3,1], 1 -> < se -1 (sahi 1)
        if (nums[low] <= nums[mid]) {       // LEFT half sorted
            if (target >= nums[low] && target <= nums[mid])   // ★ >= nums[low] ZARURI: target low pe
                high = mid - 1;             //   khud ho sakta, warna miss ([5,1,3], 5)
            else
                low = mid + 1;
        }
        else {                              // warna RIGHT half sorted
            if (target >= nums[mid] && target <= nums[high])  // <= nums[high]: target high pe ho sakta
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return -1;                              // har baar aadha kaata -> O(log n)
}
```


---

### ▸ FIND MIN IN ROTATED  = ROTATED ka KHUD-ANSWER wala (mid vs HIGH, high = mid, low < high)

**SAWAAL**
```
Ek sorted array (sab alag number) ko kisi jagah se ghuma diya gaya hai. Usme sabse chhota number lautao. O(log n) me.
  e.g. [3,4,5,1,2] -> 1
  kyun: 3,4,5 badhte gaye, 1 pe gira -> wahi ghumne ki jagah, wahi min
```

> [!IMPORTANT]
> **CHAABI:** Target nahi, khud MIN -> mid ko HIGH se compare: `nums[mid] > nums[high]` to `low = mid + 1`, warna `high = mid`. `while (low < high)`, return `nums[low]`.

> [!WARNING]
> `high = mid` (mid-1 NAHI: mid khud min ho sakta, `[3,1,2]` pe mid-1 se galat 3). `high = mid` ke saath `low < high` hi; `<=` = infinite loop.

**CODE**
```cpp
int findMin(vector<int> &nums) {            // target nahi, khud MIN -> mid ko HIGH se compare
    int low = 0, high = nums.size() - 1;
    while (low < high) {                    // ★ low<high: high=mid ke saath <= = infinite loop
        int mid = low + (high - low) / 2;
        if (nums[mid] > nums[high])
            low = mid + 1;                  // mid bada -> min right me, mid khud nahi
        else
            high = mid;                     // ★ high=MID (mid-1 NAHI): mid khud min ho sakta
    }                                       //   ([3,1,2] pe mid-1 se galat 3 aata)
    return nums[low];
}
```


#### ── FAMILY: BS-on-ANSWER (koko · ship · split: SHELL same, sirf RANGE + solve(mid) alag) ──

```
     // array nahi, ANSWER-range pe BS. solve(mid) feasible? -> chhota try (high=mid-1), warna low=mid+1
     // SHARED SHELL (teeno bilkul same):
     //    ans = -1                                   // koi mid feasible na nikle to -1
     //    while (low <= high)                         // <= : low==high wala aakhri candidate bhi test
     //        mid = low + (high - low) / 2
     //        solve(mid) ? (ans = mid, high = mid - 1) : low = mid + 1   // feasible nahi: mid bekaar
     // FARAK sirf 2: (a) RANGE (low, high)  (b) solve(mid) ka formula (greedy count <= limit?)
```

---

### ▸ KOKO BANANAS (LC-875) -- min eating-speed

**SAWAAL**
```
piles[] me kelon ke dher hain aur h ghante hain. Koko har ghante ek dher se k kele khaati hai (dher me kam bache hon to bas wahi).
Saare kele h ghante me khatam karne ke liye sabse kam speed k kitni chahiye, wo lautao.
  e.g. piles=[3,6,7,11], h=8 -> 4
  kyun: k=4 -> 1+2+2+3 = 8 ghante, chal gaya · k=3 -> 1+2+3+4 = 10 > 8, nahi chalega
```

> [!IMPORTANT]
> **CHAABI:** Answer-range pe BS: `low = 1`, `high = max(piles)`. `solve(mid)`: `sum ceil(piles[i] / mid) <= h`? haan -> `ans = mid, high = mid - 1` (chhota try), nahi -> `low = mid + 1`.

> [!WARNING]
> `low = 1` (0 pe `piles[i] / mid` divide by zero). `(double)` cast PEHLE, int/int pehle hi floor (`7/3 = 2`); ya integer `(piles[i] + mid - 1) / mid`. hours `long long`.

**CODE**
```cpp
bool solve(vector<int> &piles, int h, int mid) {
    long long hours = 0;                    // long long: bade piles pe sum overflow
    for (int i = 0; i < piles.size(); i++)
        hours += ceil((double)piles[i] / mid);   // ★ double cast PEHLE: int/int pehle hi floor (7/3=2)
                                            //   ya integer: (piles[i] + mid - 1) / mid (safe, prefer)
    return hours <= h;                      // hours = sum ceil(piles[i] / mid) <= h ?
}

int minEatingSpeed(vector<int> &piles, int h) {
    int low = 1;                            // ★ low=1: 0 pe piles[i]/mid = divide by zero
    int high = *max_element(begin(piles), end(piles));   // high = max: ek ghante me sabse badi dheri
    int ans = -1;                           // [3,6,7,11], h=8 -> 4
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (solve(piles, h, mid)) {
            ans = mid;
            high = mid - 1;                 // feasible -> chhota try
        }
        else
            low = mid + 1;
    }
    return ans;
}
```


---

### ▸ SHIP WITHIN D DAYS (LC-1011) -- min capacity  = KOKO wala shell, sirf range + solve alag

**SAWAAL**
```
weights[] me packet ka wazan diya hai. Inhe isi kram me D din me bhejna hai, aur ek din ka kul wazan jahaaz ki capacity se zyada nahi ho sakta.
Sabse kam capacity kitni chahiye, wo lautao.
  e.g. weights=[3,2,2,4,1,4], D=3 -> 6
  kyun: 6 se [3,2] [2,4] [1,4] = 3 din · 5 se [3,2] [2] [4,1] [4] = 4 din, zyada
```

> [!IMPORTANT]
> **CHAABI:** KOKO ka shell, sirf range + solve alag: `low = max(weights)`, `high = sum(weights)`. `solve`: `sum + w > mid` to `days++, sum = 0`; `days <= D`?

> [!WARNING]
> `days = 1` se shuru (aakhri din ka load bhi gine). `low = max` (sabse bhaari packet akela chadhe): `[3,10], D=2` me lo=0 rakha -> 5 aata, sahi 10.

**CODE**
```cpp
bool solve(vector<int> &weights, int D, int mid) {
    int days = 1;                           // ★ day=1: aakhri din ka load bhi gino
    int sum = 0;
    for (auto &w : weights) {
        if (sum + w > mid) {                // aaj w chadhaya to capacity tooti -> agla din
            days++;
            sum = 0;
        }
        sum += w;
    }
    return days <= D;
}

int shipWithinDays(vector<int> &weights, int days) {
    int low = *max_element(begin(weights), end(weights));   // ★ low=max: sabse bhaari packet akela chadhe
    int high = accumulate(begin(weights), end(weights), 0); // high=sum: ek din me sab
    int ans = -1;
    while (low <= high) {                   // SHELL wahi
        int mid = low + (high - low) / 2;
        if (solve(weights, days, mid)) {
            ans = mid;
            high = mid - 1;
        }
        else
            low = mid + 1;
    }
    return ans;
}
```


---

### ▸ SPLIT ARRAY LARGEST SUM (LC-410) -- min largest part-sum  = SHIP WITHIN D DAYS hi (D din = k hisse)

**SAWAAL**
```
Ek array ko k lagaatar hisson me todna hai. Har tareeke me sabse bade hisse ka jod dekho.
Aisa todo ki ye sabse bada jod jitna ho sake chhota ho, aur wo jod lautao.
  e.g. [7,2,5,10,8], k=2 -> 18
  kyun: splits ke bade-sum 25,23,18,24 -> sabse chhota 18 ([7,2,5]=14 | [10,8]=18)
```

> [!IMPORTANT]
> **CHAABI:** SHIP WITHIN D DAYS hi: k hisse = D din. `solve`: `sum > mid` to `count++, sum = nums[i]`; `count <= k`? Range `low = max`, `high = sum`.

> [!WARNING]
> `count = 1` se START (warna aakhri tukda chhoot jaata). Limit tootne pe `sum = nums[i]` (0 nahi): wahi element naye tukde ka pehla.

**CODE**
```cpp
bool solve(vector<int> &nums, int k, int mid) {
    int sum = 0, count = 1;                 // ★ count=1 se START: warna aakhri tukda chhoot jaata
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
        if (sum > mid) {
            count++;
            sum = nums[i];                  // ★ sum = x (0 nahi): limit todne wala x naye tukde ka pehla
        }
    }
    return count <= k;
}

int splitArray(vector<int> &nums, int k) {
    int low = *max_element(begin(nums), end(nums));   // low=max: bada element akela
    int high = accumulate(begin(nums), end(nums), 0); // high=sum: k=1 poora
    int ans = -1;
    while (low <= high) {                   // SHELL wahi
        int mid = low + (high - low) / 2;
        if (solve(nums, k, mid)) {
            ans = mid;
            high = mid - 1;
        }
        else
            low = mid + 1;
    }
    return ans;
}
```


#### ── FAMILY: HALF-DISCARD (array pura sorted nahi, phir bhi ek comparison se aadha safe discard) ──

### ▸ FIND PEAK ELEMENT (LC-162)

**SAWAAL**
```
Ek array diya hai. Peak wo number hai jo apne dono padosiyon se bada ho (array ke bahar ko minus infinity maano). Kisi bhi ek peak ka index lautao. O(log n) me.
  SHART (LC-162): do padosi kabhi barabar nahi (nums[i] != nums[i+1]). Isi se teesra case (barabar) aata hi nahi; [1,1] pe loop atak jaata
  e.g. [1,2,1,3,5,6,4] -> 5 (ya 1)
  kyun: index 5 pe 6 -> padosi 5 aur 4 dono chhote · index 1 pe 2 bhi (1 aur 1 se bada)
```

> [!IMPORTANT]
> **CHAABI:** mid ko `mid + 1` se compare: CHADHAAI (`nums[mid] < nums[mid+1]`) -> `low = mid + 1`; DHALAAN -> `high = mid`. `while (low < high)`, return `low`.

> [!WARNING]
> `high = mid` (mid-1 NAHI: mid khud peak ho sakta) ke saath `low < high` hi (atke nahi, `mid + 1` hamesha andar). SHART: padosi barabar nahi; `[1,1]` pe loop atak jaata.

**CODE**
```cpp
int findPeakElement(vector<int> &nums) {    // peak = dono padosi se bada; edges ke bahar -inf
    int n = nums.size();
    int low = 0, high = n - 1;
    while (low < high) {                    // low<high: high=mid se atke nahi, aur mid+1 hamesha andar
        int mid = low + (high - low) / 2;
        if (nums[mid] < nums[mid + 1])
            low = mid + 1;                  // CHADHAAI -> peak RIGHT me (mid peak nahi, right bada)
        else if (nums[mid] > nums[mid + 1])
            high = mid;                     // ★ DHALAAN -> mid KHUD ya left: high = mid (mid-1 NAHI)
    }
    return low;                             // low==high pe converge = peak, edges apne aap
}
```


---

### ▸ SINGLE ELEMENT (SORTED, LC-540)  = INDEX-PARITY binary search

**SAWAAL**
```
Ek SORTED array me har number theek do baar aata hai, sirf ek number akela (ek baar) hai. Wo akela number lautao. O(log n) me.
  e.g. [1,1,2,3,3,4,4,8,8] -> 2
  kyun: 1,1 jodi · 2 ka jodidaar nahi · baaki 3,3 4,4 8,8 jodi -> 2

har element 2x, sirf EK akela. O(log n) (XOR = O(n), yahan log n chahiye)
single se PEHLE pairs (even,odd) idx pe -> single ke BAAD (odd,even) SHIFT. mid ko mid-1 se compare
```

**SAAR**
```
── DRY-RUN -- ISKO paper pe dekh, prose bhool (single = 2 @ idx 2) ──
   idx:  0  1  2  3  4  5  6  7  8
   arr:  1  1  2  3  3  4  4  8  8
   low0 high8 -> mid=4 (EVEN) : nums[4]=3 == nums[3]=3 -> high=4-1=3
   low0 high3 -> mid=1 (ODD)  : nums[1]=1 == nums[0]=1 -> low=1+1=2
   low2 high3 -> mid=2 (EVEN) : nums[2]=2 != nums[1]=1 -> low=2+1=3
   low3 high3 -> mid=3 (ODD)  : nums[3]=3 != nums[2]=2 -> high=3-1=2
   low3 > high2 -> STOP -> return nums[high]=nums[2]= 2  (sahi)
return nums[HIGH] (not low): dono branch mid REJECT karte -> high khud single pe aa ke rukta.
```

> [!IMPORTANT]
> **CHAABI:** Single se PEHLE pair (even, odd) index pe. mid ko `mid - 1` se milao: EVEN mid ka saathi peeche mila = shift ho chuki -> `high = mid - 1`; ODD mid ka saathi peeche mila = theek -> `low = mid + 1`. End `return nums[high]`.

> [!WARNING]
> `return nums[high]` (low nahi): dono branch mid REJECT karte, high khud single pe rukta. `mid == 0` pe seedha `nums[mid]` (`mid - 1` bahar). size 1 pe seedha `nums[0]`.

**CODE**
```cpp
int singleNonDuplicate(vector<int> &nums) {
    if (nums.size() == 1)
        return nums[0];                     // akela element hi single, BS ki zarurat nahi
    int n = nums.size();
    int low = 0;
    int high = n - 1;                       // poori array inclusive
    while (low <= high) {                   // <= : low>high pe hi ruko, tab high single pe
        int mid = low + (high - low) / 2;
        if (mid == 0)
            return nums[mid];               // edge: mid-1 = -1 bahar; yahan aaye to single idx 0 pe
        if (mid % 2 == 0) {                 // ★ even mid: single se pehle iska saathi AAGE hota
            if (nums[mid] == nums[mid - 1])
                high = mid - 1;             // saathi peeche = pairing shift ho chuki -> single PEECHE
            else
                low = mid + 1;              // pairing theek -> single aage
        }
        else {                              // odd mid: single se pehle iska saathi PEECHE hota
            if (nums[mid] != nums[mid - 1])
                high = mid - 1;             // saathi peeche nahi = shift ho chuki -> single PEECHE
            else
                low = mid + 1;              // saathi peeche mila = theek -> single aage
        }
    }
    return nums[high];                      // ★ HIGH (low nahi): dono branch mid reject, high single pe
}                                           // [1,1,2,3,3,4,4,8,8]: mid 4 -> 1 -> 2 -> 3 -> high=2 -> 2
```


#### ── FAMILY: 2D-index-map (2D matrix = 1D sorted array, index -> (row, col)) ──

### ▸ SEARCH 2D MATRIX  = BASIC BS, poori matrix ek 1D sorted array (index -> row, col)

**SAWAAL**
```
Ek matrix di hai jiski har row sorted hai, aur har row ka pehla number pichhli row ke aakhri number se bada hai (matlab poori matrix ek lambi sorted line hai).
Batao target matrix me hai ya nahi, true ya false.
  e.g. [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target=3 -> true
  kyun: 3 pehli row me index 1 pe mil gaya · target=13 hota to 11 aur 16 ke beech, nahi -> false
```

> [!IMPORTANT]
> **CHAABI:** Poori matrix ek 1D sorted line: `high = row * col - 1`, cell = `matrix[mid / col][mid % col]`. Baaki basic BS.

> [!WARNING]
> Dono jagah COL (`/ col`, `% col`), row nahi: 3x4, mid=5 -> `[1][1]`; `/3` likha to `[1][2]` galat. `col = matrix[0].size()`.

**CODE**
```cpp
bool searchMatrix(vector<vector<int>> &matrix, int target) {
    int row = matrix.size();
    int col = matrix[0].size();             // col = matrix[0].size(), row nahi
    int low = 0, high = row * col - 1;      // poore matrix ko 1D maano
    while (low <= high) {
        int mid = low + (high - low) / 2;
        // ★ dono jagah COL (row nahi): 3x4, mid=5 -> [1][1]; /3 likha to [1][2] galat
        if (matrix[mid / col][mid % col] == target)   // row = mid / col, col = mid % col
            return true;
        else if (matrix[mid / col][mid % col] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return false;
}
```


#### ── FAMILY: BS-on-PARTITION / CUT (value pe nahi, 2 sorted array me CUT-position pe BS) ──

### ▸ MEDIAN OF 2 SORTED ARRAYS (LC-4, Hard)  = BS-on-ANSWER ka cousin, par VALUE nahi CUT-position pe

**SAWAAL**
```
Do SORTED array A aur B diye hain. Dono ko milake jo poora sorted array banega, uska median lautao.
Kul number odd hon to beech wala, even hon to beech ke do ka average. O(log) me karna hai.
  e.g. A=[1,3,8,9,15], B=[7,11,18,19,21,25] -> 11.0
  kyun: mila ke 1,3,7,8,9,11,15,18,19,21,25 = 11 number -> beech wala (6th) = 11

MERGE mat karo -> CUT: LEFT aadha | RIGHT aadha, left ka HAR <= right ka HAR. 4 kinaare = median
A=[1,3,8,9,15], B=[7,11,18,19,21,25] -> Px=4 (1 3 8 9 | 15), Py=2 (7 11 | 18 ..) -> max(9,11) = 11
Px=2 try: x2=19 > x3=8 -> A ne KAM liya -> low = Px+1. cousin = BS-on-ANSWER (wahan VALUE pe)
```

**SAAR**
```
CUT idea (chhote array A pe BS -> Px; Py auto):
   A:  1  3  8  9 | 15            Px = A-left count (BS ISI pe, chhote array pe)
   B:  7 11       | 18 19 21 25   Py = (m+n+1)/2 - Px
   yahan:  Px = 4  (A-left = {1,3,8,9})  = binary-search ka MID jahan cut VALID nikla
           Py = (11+1)/2 - 4 = 6 - 4 = 2   (B-left = {7,11})
           [Px = BS variable "mid" hi hai · A[Px-1]=x1 left-kinaara · A[Px]=x3 right-kinaara]
          x1=9  x3=15
          x2=11 x4=18
   |___ LEFT = 6 (half) ___|___ RIGHT = 5 ___|

   4 KINAARE (baaki se matlab NAHI, sorted isliye apne-aap sahi):
     x1=A[Px-1] Aleft   x3=A[Px] Aright   (edge Px==0->-INF, Px==m->+INF)
     x2=B[Py-1] Bleft   x4=B[Py] Bright   (edge Py==0->-INF, Py==n->+INF)

   SAHI cut:  x1<=x4  &&  x2<=x3        (left-max <= right-min, dono-taraf cross-check)
     galat -> x1>x4 (A ne ZYADA liya) -> high=Px-1  |  warna -> low=Px+1
   MEDIAN: odd -> max(x1,x2)  |  even -> (max(x1,x2)+min(x3,x4))/2.0

   "= BS PAR array-value pe nahi -> CUT-POSITION (Px) pe. condition monotonic -> BS chalti."
   KYUN sirf 4 number: sorted -> Px se pehle sab <=x1, aage sab >=x3. har step O(1), Px pe BS = O(log(min(m,n))).

── DRY-RUN (A=[1,3,8,9,15] m=5, B=[7,11,18,19,21,25] n=6, half=6) ──

  TRY Px=2:   A:  1  3 | 8  9  15          x1=3   x3=8
              B:  7 11 18 19 | 21 25       x2=19  x4=21   (Py=4)
      x2(19) <= x3(8) ?  NAHI  -> A ne KAM liya -> low = Px+1 = 3

  TRY Px=4:   A:  1  3  8  9 | 15          x1=9   x3=15
              B:  7 11 | 18 19 21 25       x2=11  x4=18   (Py=2)
      x1(9)<=x4(18)  &&  x2(11)<=x3(15)  -> SAHI cut
      odd -> median = max(x1,x2) = max(9,11) = 11   ANS=11

  GOTCHA: even -> /2.0 + (double) cast (int-div se 3.5 bug) | edge ±INF warna out-of-bounds.
```

> [!IMPORTANT]
> **CHAABI:** CHHOTE array A pe CUT `Px` pe BS, `Py = (m + n + 1) / 2 - Px`. 4 kinaare: `x1 = A[Px-1]`, `x2 = B[Py-1]` (left) · `x3 = A[Px]`, `x4 = B[Py]` (right). `x1 <= x4 && x2 <= x3` = sahi cut: odd -> `max(x1, x2)`, even -> `(max(x1,x2) + min(x3,x4)) / 2.0`. `x1 > x4` -> `high = Px - 1`, warna `low = Px + 1`.

> [!WARNING]
> `m > n` to swap (chhote pe BS, Py kabhi -ve nahi). Edge: `Px == 0` / `Py == 0` -> `INT_MIN`, `Px == m` / `Py == n` -> `INT_MAX` (warna `A[-1]` / `A[m]` bahar). Even me `/ 2.0` (int division se .5 kat jaata). `(m + n + 1) / 2` me `+1`.

**CODE**
```cpp
double findMedianSortedArrays(vector<int> &A, vector<int> &B) {
    int m = A.size();
    int n = B.size();
    if (m > n)
        return findMedianSortedArrays(B, A);   // ★ chhote pe BS -> Py kabhi -ve nahi, O(log(min(m,n)))
    int low = 0, high = m;                  // high=m: A ke 0 se m tak kitne bhi left me
    while (low <= high) {                   // <= : Px=low==high wala cut bhi try
        int Px = low + (high - low) / 2;    // Px = A-left count (BS isi pe)
        int Py = ((m + n + 1) / 2) - Px;    // ★ +1: odd total pe left me ek zyada
        int x1 = (Px == 0) ? INT_MIN : A[Px - 1];   // left khaali -> -INF (A[-1] bahar)
        int x2 = (Py == 0) ? INT_MIN : B[Py - 1];
        int x3 = (Px == m) ? INT_MAX : A[Px];       // right khaali -> +INF (A[m] bahar)
        int x4 = (Py == n) ? INT_MAX : B[Py];
        if (x1 <= x4 && x2 <= x3) {         // ★ && : dono cross-check sahi tabhi cut valid
            if ((m + n) % 2 == 0)
                return (max(x1, x2) + min(x3, x4)) / 2.0;   // /2.0: int division se .5 kat jaata
            else
                return max(x1, x2);         // odd -> left ka max
        }
        if (x1 > x4)
            high = Px - 1;                  // A ne ZYADA liya -> cut left khiskao
        else
            low = Px + 1;                   // A ne kam liya
    }
    return 0.0;
}
```


