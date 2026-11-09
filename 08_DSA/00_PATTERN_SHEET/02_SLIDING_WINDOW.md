## PATTERN 2 — SLIDING WINDOW


```

 KAB PEHCHANO: CONTIGUOUS subarray/substring + "longest / shortest / max / min / COUNT" maanga -> SW socho.

 2 TYPE:
   (a) FIXED window   -> size k DIYA hai. i/j pointer: sum += nums[j]; jab (j-i+1) >= k -> track + left hatao (sum -= nums[i], i++).
   (b) VARIABLE window-> size condition se. TEMPLATE:
          for j in arr:  window me arr[j] add (update tracker)
                         while (INVALID): left[i] hatao (tracker update), i++
                         ans update  (length = j-i+1, ya count += j-i+1)

 KYA TRACK: sum · freq-map (distinct=map.size / char-count) · zeros-count · product · maxFreq.

 KEY: shrink ke BAAD window HAMESHA valid -> ans UNCONDITIONAL (koi if(==k) nahi).

 LENGTH ya COUNT: length -> max(ans, j-i+1) · subarray COUNT -> count += (j-i+1) (window size).

 IF vs WHILE (ye confusion CLEAR kar) -> SAWAAL: "window ka SIZE fix hai, ya condition pe depend?"
     · SIZE fix (koi number diya: k-elements ya p.length()) -> IF (ek add, ek remove, size wahi rehta).   [Max-Sum-of-K · Anagram]
     · SIZE condition pe (sum/distinct/repeat/product) -> WHILE (jab tak invalid, ek se zyada nikal sakte).  [baaki sab]
     TRAP -- `k` ka matlab dekh: Max-Sum-size-K me k = SIZE (fix -> IF); Subarray-Product<K me k = product LIMIT (size NAHI -> WHILE). same letter `k`, alag matlab.
 [track] · [INVALID kab -> shrink] · [ans]

 ONE SKELETON (fixed + variable DONO isi pe -- ye poora khel):
    i = 0, j = 0
    while (j < n):                     // EXPAND (j HAMESHA aage). [for(j..) == while(j<n){..;j++} -- SAME]
        arr[j] window me ADD           (tracker update: sum / map / zeros / product)
        while / if  (WINDOW BAD):      // SHRINK (i aage)
            arr[i] REMOVE (tracker update);  i++
        ans UPDATE
        j++
    -> sirf 2 cheez badalti: (1) inner IF (fixed, ek remove) ya WHILE (variable, jab tak invalid)
                             (2) "WINDOW BAD" kya + kya track. baaki dhaancha har baar SAME.

 KAB NAHI: variable window jisme SUM pe shrink ho (sum > k pe chhota karo), aur numbers NEGATIVE hon -> sum ghat jaata,
     "bada ho gaya, chhota karo" wala faisla toot jaata -> wahan prefix + map. Fixed size window negative ke saath bhi chalti.
 ★ EDGE (variable window): shrink me count 0 ho jaye to map se ERASE karo, warna map.size() jhooth bolega.
```

---

## LAMBA wale (kharab hote hi chhota karo, while ke BAAD ans = max)


---

### ▸ LC-3 LONGEST SUBSTRING NO-REPEAT  = variable window + freq-map

**SAWAAL**  `"abcabcbb" -> 3`. Sabse lambi substring jisme koi char repeat na ho.
kyun: "abc" me teeno alag = 3; agla a aate hi repeat, isse lamba koi substring nahi

**SAAR**
```
longest substring jisme koi char REPEAT na ho. variable window + freq-map(mp).
INVALID = mp[s[j]]>1 (jo abhi add hua wahi repeat). empty-string edge -> INT_MIN -> 0.
```

> [!IMPORTANT]
> **CHAABI:** jo abhi add hua (`s[j]`) wahi repeat ho sakta hai, to INVALID = `mp[s[j]] > 1`. Shrink ke **baad** `ans = max(ans, j - i + 1)`.

> [!WARNING]
> **"abba":** `while` ki jagah `if` likha to 3 aata, sahi 2. Ek se zyada char nikaalne pad sakte hain (a aur purana b dono).

**CODE**
```cpp
int lengthOfLongestSubstring(string s) {
    if (s.empty())
        return 0;
    int ans = INT_MIN;
    unordered_map<char, int> mp;
    int i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;                         // window me add
        // ★ "abba": a aur purana b, dono nikaalo. if -> 3, sahi 2
        while (mp[s[j]] > 1) {              // INVALID: current char (s[j]) repeat
            mp[s[i]]--;                     // left shrink jab tak repeat khatam
            i++;
        }
        ans = max(ans, j - i + 1);          // valid window length (shrink ke BAAD record)
        j++;
    }
    return ans == INT_MIN ? 0 : ans;        // khaali string -> INT_MIN reh jaata -> 0
}
```


---

### ▸ LC-424 CHAR REPLACEMENT (LONGEST)  = variable window + freq-map (k badal sakte)
**SAWAAL**  `"AABABBA", k=1 -> 4`. Max k char badal ke sabse lambi window jisme sab same.

kyun: "AABA" (index 0..3) me ek B ko A banao -> "AAAA" = 4

**SAAR**
```
SAAR : longest window jisme <=k char badal ke sabko SAME bana sako. variable window + freq-map.
KEY  : windowLen - maxFreq = kitne char badalne padenge. <=k -> valid.
CORE TRICK (non-obvious): maxFreq kabhi GHATAO mat (shrink pe recompute nahi). window best-length
   se chhota hota hi nahi (sirf badhta/slide) -> stale/purana maxFreq bhi answer kharab nahi karta.
```

> [!IMPORTANT]
> **CHAABI:** `windowLen - maxFreq` = kitne char badalne padenge. Ye `> k` hua to shrink.

> [!WARNING]
> `maxFreq` ko shrink pe kabhi ghatao mat, dobara mat nikaalo. Purana `maxFreq` bhi jawab nahi bigaadta.

**CODE**
```cpp
int characterReplacement(string s, int k) {
    unordered_map<char, int> mp;
    int maxLen = INT_MIN, maxFreq = INT_MIN, i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;
        maxFreq = max(maxFreq, mp[s[j]]);   // maxFreq update PEHLE, phir windowLen - maxFreq check
        // ★ length - sabse zyada wala = kitne badalne padenge
        while (j - i + 1 - maxFreq > k) {   // INVALID: k se zyada badalne padenge -> left shrink
            mp[s[i]]--;
            if (mp[s[i]] == 0)
                mp.erase(s[i]);             // 0 hua to key hatao (DISTINCT gino to zaroori)
            i++;                            // ginti kam karne ke BAAD hi left aage
        }
        if (j - i + 1 - maxFreq <= k)       // VALID -> maxLen update
            maxLen = max(maxLen, j - i + 1);
        j++;
    }
    return maxLen;
}
```


---

### ▸ LC-1004 MAX CONSECUTIVE ONES III  = LC-3 ka bhai: variable window + zeros-count (k flips)
**SAWAAL**  `[1,1,1,0,0,0,1,1,1,1,0], k=2 -> 6`. Max k zero ko 1 bana ke sabse lambi 1 ki line.

kyun: index 4..9 = 0,0,1,1,1,1: do zero flip -> 6 lagaatar 1

**SAAR**
```
SAAR : at most k zeros ko FLIP -> longest all-1 window. sirf zeroCount track (freq-map nahi).
   LC-3 se farak: map ki jagah ek int (zeroCount), aur shrink "zero > k" pe.
CORE LEARNING: ans UNCONDITIONAL update -> if(zeroCount==k) MAT lagao.
   kyun: upar while(zeroCount>k) shrink ke BAAD window HAMESHA valid (<=k) -> yahan pahunchte hi valid -> condition faltu.
   (galti thi: if(zeroCount==k) -> sirf exactly-k ginta -> k=0 / k>zeros case toot jaate.)
shrink pe nums[i]==0 hi zeroCount-- (1 nikalne pe count na badle).
```

> [!IMPORTANT]
> **CHAABI:** map ki jagah ek `zeroCount`. `zeroCount > k` hua to shrink, phir ans bina shart.

**CODE**
```cpp
int longestOnes(vector<int> &nums, int k) {
    int ans = INT_MIN;                      // INT_MIN: end me check karke khaali array pe 0
    int zeroCount = 0;
    int i = 0, j = 0;
    while (j < nums.size()) {
        if (nums[j] == 0)
            zeroCount++;                    // window me zero add
        while (zeroCount > k) {             // INVALID: k se zyada zero (flip nahi kar sakte)
            if (nums[i] == 0)
                zeroCount--;                // left ka zero nikla to count kam
            i++;                            // zero check ke BAAD aage (pehle i++ to galat element dekhta)
        }
        // ★ bina shart lo. if (zeroCount == k) -> [1,1,0], k=2 pe 0, sahi 3
        ans = max(ans, j - i + 1);          // UNCONDITIONAL (koi if nahi)
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}
```


---

### ▸ FRUIT INTO BASKETS  = longest subarray with AT MOST 2 DISTINCT
**SAWAAL**  `[1,2,3,2,2] -> 4`. 2 basket, har basket me ek type. Lagaatar pedon ki sabse lambi line.

kyun: [2,3,2,2] me sirf 2 type (2,3) = 4; aage 1 jodo to 3 type ho jaate

**SAAR**
```
SAAR : "longest subarray with AT MOST 2 DISTINCT" (fruit/basket sirf kahani). SW skeleton + map<type,count>.
while (mp.size() > 2) KYUN while: ek type poora khatam hone tak kai element hatane padte.
   IF bhi chalta (sirf LONGEST): ek aaya ek gaya, size best se bada nahi -> len sahi. COUNT / SHORTEST me sirf while.
CRUX = erase-on-0: count 0 hone pe key map me PADI reh jaati -> mp.size() galat distinct dega -> condition toot. isliye ERASE.
```

> [!IMPORTANT]
> **CHAABI:** alag type kitne = `mp.size()`. `> 2` hua to shrink.

> [!WARNING]
> count 0 hua to ERASE. Nahi kiya to size 3 hi rehta, loop nahi rukta, i array ke bahar -> crash.

**CODE**
```cpp
int totalFruit(vector<int> &fruits) {
    int ans = INT_MIN;
    unordered_map<int, int> mp;
    int i = 0, j = 0;
    while (j < fruits.size()) {
        mp[fruits[j]]++;                    // window me add
        // ★ ginti nahi, ALAG type kitne = mp.size()
        while (mp.size() > 2) {             // INVALID: 2 se zyada distinct
            mp[fruits[i]]--;
            // ★ erase nahi kiya to size 3 hi rehta, loop rukta nahi, i array ke bahar -> crash
            if (mp[fruits[i]] == 0)
                mp.erase(fruits[i]);        // count 0 -> ERASE
            i++;                            // erase check ke BAAD hi left aage
        }
        ans = max(ans, j - i + 1);          // shrink ke BAAD record: window ab <=2 distinct
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}
```


---

### ▸ LONGEST AT-MOST-K DISTINCT  = FRUIT ka GENERAL version (2 ki jagah k)
**SAWAAL**  `"eceba", k=2 -> 3`. Sabse lambi substring jisme zyada se zyada k alag char.

kyun: "ece" me sirf e,c (2 alag) = 3; "eceb" me 3 alag ho jaate

> [!IMPORTANT]
> **CHAABI:** fruit ka hi code, bas `2` ki jagah `k` (`mp.size() > k`).
>
> wahi variable window + `map<char,count>`: distinct (`mp.size()`) `> k` ho to left se shrink (erase-on-0). ans = max length.

> [!WARNING]
> wahi erase-on-0: count 0 hua to key hatao, warna `mp.size()` jhooth bolega.

**CODE**
```cpp
int totalFruit(string &s, int k) {
    int ans = INT_MIN;
    unordered_map<int, int> mp;
    int i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;
        while (mp.size() > k) {             // ★ fruit me 2, yahan k
            mp[s[i]]--;
            if (mp[s[i]] == 0)
                mp.erase(s[i]);
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}

int longestKDistinct(string s, int k) {
    return totalFruit(s, k);
}
```


---

### ▸ LC-1208 EQUAL SUBSTRING WITHIN BUDGET  = longest-subarray-sum <= k (string ke bhes me)
**SAWAAL**  `s="abcd", t="bcdf", maxCost=3 -> 3`. s ke char ko t me badalne ki cost `|s[i]-t[i]|`. Kul cost budget ke andar, sabse lambi window.

kyun: cost = [1,1,1,2]; pehle 3 ka 1+1+1 = 3 <= 3; chaaron ka 5 > 3 · e.g. s="abcd", t="cdef", maxCost=3 -> 1

**SAAR**
```
SAAR : s ko t banane me har char ka cost = |s[i]-t[i]|. longest window jiska cost-SUM <= maxCost.
KEY  : cost-array bana ke "longest subarray with sum <= k" -- bas string ke bhes me chhupa.
   ONES III jaisa, ek int (cost) track, koi map nahi. cost hamesha 0 ya zyada, isliye negative wala dar nahi.
```

> [!IMPORTANT]
> **CHAABI:** har char ki cost `|s[i] - t[i]|`. Bas "longest subarray, sum <= maxCost", string ke bhes me.

**CODE**
```cpp
int equalSubstring(string s, string t, int maxCost) {
    int i = 0, j = 0, cost = 0, ans = INT_MIN;
    while (j < t.size()) {
        // ★ string ke bhes me "sum <= k" wala sawaal
        cost += abs(s[j] - t[j]);
        while (cost > maxCost) {            // INVALID -> WHILE shrink
            cost -= abs(s[i] - t[i]);
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans;
}
```

---


## CHHOTA wala (theek rahe tab tak chhota karo, ans while ke ANDAR = min)

### ▸ LC-209 MIN SUBARRAY LEN (>=TARGET)  = SHORTEST window (longest se ULTA)
**SAWAAL**  `target=7, [2,3,1,2,4,3] -> 2` ([4,3]). Positive numbers, sum >= target wala sabse chhota subarray.

kyun: [4,3] = 7 >= 7, length 2; akela koi number 7 tak nahi pahunchta

**SAAR**
```
SAAR : positive nums, sum >= target wala SABSE CHHOTA subarray ki length (nahi mile -> 0).
LONGEST vs SHORTEST (yahi confusion ki JAD -- window BADA chahiye ya CHHOTA):
   LONGEST  (jaise no-repeat): window BADA chahiye -> shrink SIRF jab INVALID ho (validity wapas laane ko) ->
            record BAAHAR (shrink ke BAAD, valid window ki length).
   SHORTEST (ye / min-window):  window CHHOTA chahiye -> VALID hote hi record + shrink (chhota karte jao jab tak valid) ->
            record ANDAR (shrink loop ke andar, har step).   <- YE wahi.
   1-line: LONGEST = shrink-jab-MAJBOORI(invalid), record-baad  ·  SHORTEST = shrink-jab-tak-VALID, record-andar.
char-matching wala SHORTEST = MIN WINDOW (LC-76) -> neeche "need-map + COUNT" family me (LC-1358 ke saath).
```

> [!IMPORTANT]
> **CHAABI:** SHORTEST: valid hote hi record + shrink, ans `while` ke ANDAR. (LONGEST me ans `while` ke BAAD.)

> [!WARNING]
> koi window na bane to `INT_MAX` hi bachta -> 0 lautao (`target=11, [1,1,1] -> 0`).

**CODE**
```cpp
int minSubArrayLen(int target, vector<int> &nums) {
    int sum = 0;
    int size = INT_MAX;                     // INT_MAX: min se ghatega; koi window na bani to end me 0
    int i = 0, j = 0;
    while (j < nums.size()) {
        sum += nums[j];                     // window add (expand)
        // ★ ULTA: theek hai tab tak chhota karo, ans ANDAR nikaalne se pehle
        while (sum >= target) {             // VALID hote hi -> record + SHRINK (aur chhota dhoondo)
            size = min(size, j - i + 1);    //   record ANDAR (shrink loop ke andar)
            sum -= nums[i];                 //   left shrink
            i++;
        }
        j++;                                // shrink loop ke BAHAR: har j pe ek baar aage
    }
    return size == INT_MAX ? 0 : size;      // koi valid window nahi -> 0 · target=11, [1,1,1] -> 0
}
```

---


## FIXED size k (while ki jagah IF)

### ▸ MAX SUM OF K (FIXED)  = fixed window size-k + running sum
**SAWAAL**  `[2,1,5,1,3,2], k=3 -> 9`. Har size-k window ka sum, sabse bada.

kyun: window sums 8, 7, 9, 6 -> max 9 (5+1+3)

**SAAR**
```
SAAR : k-size ke har window ka sum -> unme se MAX. fixed window (size hamesha k).
KEY  : variable nahi -- size fix k, isliye IF (ek hi remove).
WHY IF-not-WHILE : fixed size -> har step 1 add + 1 remove -> slide. variable hota to WHILE (kai remove).
```

> [!IMPORTANT]
> **CHAABI:** size fix `k` -> `if` (ek aaya, ek gaya), `while` nahi.

> [!WARNING]
> `maxSum` 0 se shuru kiya to `[-1,-2,-3], k=2` pe 0, sahi -3. `INT_MIN` lo.

**CODE**
```cpp
int maxSumK(vector<int> &nums, int k) {
    int sum = 0;
    // ★ 0 se shuru kiya to [-1,-2,-3], k=2 -> 0, sahi -3
    int maxSum = INT_MIN;                   // INT_MIN: sab negative ho to bhi sahi max
    int i = 0, j = 0;
    while (j < nums.size()) {
        sum += nums[j];                     // j pe add
        if (j - i + 1 >= k) {               // IF: size fix, ek juda to ek nikla
            maxSum = max(maxSum, sum);      // FULL window -> record
            sum -= nums[i];                 // phir 1 remove
            i++;
        }
        j++;
    }
    return maxSum;
}
```


---

### ▸ LC-1423 MAX POINTS FROM CARDS  = MAX-SUM-SIZE-K (01) + COMPLEMENT-twist
**SAWAAL**  `[1,2,3,4,5,6,1], k=3 -> 12` (peeche se 5+6+1). Dono kinaron se kul k card lo, sum sabse bada.

kyun: back se 3 = 1+6+5 = 12; front 3 = 6, mix (1+2+1 = 4, 1+1+6 = 8) kam

**SAAR**
```
Q: sirf DONO ENDS (front/back) se TOTAL k cards lo -> unka sum MAXIMIZE.

★ RECOGNITION (jab question AISE sound kare): "dono end se k cards lo, sum MAX" -> ye SIGNAL hai.
   k UTHANE ki tension MAT lo -> ULTA jo BACHTA hai (beech ka L-k) usko MIN kardo -> answer apne-aap.
   flip ke baad "ye problem THA HI NAHI" -> plain MAX-SUM-K ki MIN-version + (Total - min). bas.

DERIVE (kaise pahuncha -- yahin GADBAD ki thi, dobara padho to soch-ki-raah + galtiyan):
  (0) "sirf back se k" -> galat: [100,1,1,1,1] k=2 -> front-2=101 best.
  (1) "front se k + back se k" -> galat: wo 2k cards; problem = k TOTAL (dono end milake). [yahin atka: L-2k nikaal raha tha]
  (2) taken cards DONO end pe -> contiguous NAHI -> seedha SW fail.
  (3) FLIP (aha): untaken = beech ka CONTIGUOUS window, size L-k.
  (4) taken MAX = untaken MIN -> answer = Total - min.  ("hard" flip ke baad TRIVIAL.)

TWIST (yahi hard part): taken cards DONO end pe (contiguous NAHI) -> seedha SW nahi chalta.
     ULTA socho -> jo NAHI liye = beech ka CONTIGUOUS window, size L-k.
     taken MAX  <=>  untaken MIN.   answer = Total - min(window).
= MAX-SUM-SIZE-K (pattern-01) ka cousin -- SAME fixed-window mechanic, bas 3 DELTA:
     (a) window size = L-k  (b) MAX ki jagah MIN dhundo  (c) answer = Total - min.

DRY-RUN [1,2,3,4,5,6,1] k=3: Total=22, win=4 -> [1,2,3,4]=10 · [2,3,4,5]=14 · [3,4,5,6]=18 · [4,5,6,1]=16 -> min=10 -> 22-10=12.
FAMILY: fixed-size SW + COMPLEMENT trick ("kya NAHI liya" gino). trap = k cards TOTAL (dono end milake), na har end se k.
```

> [!IMPORTANT]
> **CHAABI:** ulta socho: jo NAHI liye wo beech ki lagaatar window (size n - k). Answer = Total - min.

> [!WARNING]
> k card dono kinaron se MILA ke, har kinare se k nahi. Yahin L - 2k pe atka tha.

**CODE**
```cpp
int minSumK(vector<int> &nums, int k) {
    int sum = 0;
    int minSum = INT_MAX;
    int i = 0, j = 0;
    while (j < nums.size()) {
        sum += nums[j];
        if (j - i + 1 >= k) {
            minSum = min(minSum, sum);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return minSum;
}

int maxScore(vector<int> &nums, int k) {
    int Total = 0;
    for (int i = 0; i < nums.size(); i++)
        Total += nums[i];
    if (nums.size() - k == 0)
        return Total;                       // saare card liye
    // ★ jo NAHI liye wo beech ki lagaatar window (size n-k)
    int minSum = minSumK(nums, nums.size() - k);
    return Total - minSum;
}
```



---

### ▸ LC-219 CONTAINS DUPLICATE II  = fixed-window (size k) + HashSet
**SAWAAL**  `[1,2,3,1], k=3 -> true`. Do barabar number jinka index ka farak <= k.

kyun: index 0 aur 3 dono pe 1, farak 3 <= 3
  k=0: |i-j|<=0 matlab same index, jodi ban hi nahi sakti

**SAAR**
```
★ RECOGNITION: "same value + index farak <=k ke ANDAR" -> window me sirf LAST k elements rakho (HashSet).
   KEY INSIGHT: agar set me sirf last-k elements hain, aur andar duplicate mila -> uska index-farak apne-aap <=k
   -> "|i-j|<=k" alag se check karne ki zaroorat NAHI. bas window ko k-size me rakho.
DRY-RUN [1,2,3,1] k=3: 1,2,3 add -> j=3 nums=1 set me hai -> true. | [1,2,3,1,2,3] k=2: har baar window<=2, kabhi dup nahi -> false.
★ k=0 kyun alag: check (st.count) window CHHOTI hone se PEHLE hota hai -> k=0 pe pichhla element
   abhi set me hota, [4,4] k=0 pe true aa jaata (sahi false).
FAMILY: fixed-window (size k) + HashSet (seen-check). value-equal -> set; within-k -> window size k.
```

> [!IMPORTANT]
> **CHAABI:** set me sirf pichhle k element rakho. Duplicate mila to index ka farak apne aap <= k.

> [!WARNING]
> check PEHLE, insert BAAD (warna khud se match). `k = 0` pe seedha false: `[4,4], k=0 -> false`.

**CODE**
```cpp
bool containsNearbyDuplicate(vector<int> &nums, int k) {
    if (k <= 0)
        return false;                       // [4,4], k=0 -> false
    int i = 0, j = 0;
    unordered_set<int> st;
    while (j < nums.size()) {
        if (st.count(nums[j]) > 0)
            return true;                    // check PEHLE
        if (j - i + 1 > k) {
            st.erase(nums[i]);
            i++;
        }
        // ★ check ke BAAD daalo, warna khud se hi match
        st.insert(nums[j]);
        j++;
    }
    return false;
}
```

---


## KITNE (ginti, length nahi)

### ▸ SUBARRAY PRODUCT < K  = variable window + COUNT (running product)
**SAWAAL**  `[10,5,2,6], k=100 -> 8`. Positive numbers, kitne subarray jinka guna k se kam.

kyun: 4 akele + [10,5]=50, [5,2]=10, [2,6]=12, [5,2,6]=60 = 8; [10,5,2]=100 kam nahi

**SAAR**
```
SAAR : kitne subarray jinka product < k. variable window, COUNT (length nahi).
   lamba wala dhaancha, bas max ki jagah count += j - i + 1.
COUNT TRICK: valid -> count += (j-i+1) = window size (j pe end hone wale saare valid subarray). (bahut count-Q me)
GINTI KYUN (j-i+1):  window [i..j] valid hai, to j pe KHATAM hone wale ye sab bhi valid:
                     [j], [j-1..j], ... [i..j]  =  j-i+1 subarray

DRY-RUN [10,5,2,6], k=100:
    j=0  prod 10               window [10]          +1   count 1
    j=1  prod 50               window [10,5]        +2   count 3
    j=2  prod 100 >= 100 -> 10 hatao, prod 10       +2   count 5   ([5,2],[2])
    j=3  prod 60               window [5,2,6]       +3   count 8

★ EDGE: shrink ki shart me `&& i <= j` zaroori.
    k = 1 pe window poori khaali hone ke baad bhi prod = 1 rehta (khaali ka product 1),
    aur 1 >= 1 sach -> bina i <= j ke loop array ke BAHAR chala jaata.  ([1], k=1 -> 0)
    k = 0 alag, upar hi return 0.
```

> [!IMPORTANT]
> **CHAABI:** `count += j - i + 1`: j pe khatam hone wale saare subarray.

> [!WARNING]
> shrink ki shart me `&& i <= j`, warna `[1], k=1` pe i array ke bahar. `k = 0` pe seedha 0.

**CODE**
```cpp
int numSubarrayProductLessThanK(vector<int> &nums, int k) {
    if (k == 0)
        return 0;                           // nums >=1: product kabhi < 0 nahi, koi subarray nahi
    int count = 0;
    int i = 0, j = 0, prod = 1;             // guna 1 se shuru, 0 se sab 0
    while (j < nums.size()) {
        prod *= nums[j];
        // ★ i <= j: [1,1,1], k=1 pe i bahar bhaagta
        while (prod >= k && i <= j) {       // INVALID (prod >= k) -> shrink
            prod /= nums[i];                // left wala guna se bahar: divide karo
            i++;                            // divide ke BAAD hi left aage
        }
        // ★ j pe khatam hone wale saare: [j], [j-1..j] .. [i..j]
        count += j - i + 1;
        j++;                                // count ke BAAD aage badho
    }
    return count;
}
```


---

### ▸ LC-992 SUBARRAYS WITH EXACTLY K DISTINCT  = FRUIT (atMost-k) x2, ghata do  [exactly-K trick]
**SAWAAL**  `[1,2,1,2,3], k=2 -> 7`. Kitne lagaatar subarray hain jinme theek k alag number hon.

**SAAR**
```
SAAR : kitne SUBARRAYS jinme EXACTLY k different numbers.
   atMostK = fruit ka code, bas max ki jagah COUNT (ans += j-i+1).
CHAAL: "exactly K" seedha SW se count ganda (boundary saaf nahi).
       => exactly(k) = atMost(k) - atMost(k-1).
          atMost(k) = "0..k distinct wale SAB" ; atMost(k-1) = "0..k-1 wale sab" ; ghata do -> exactly k.
          (jaise  <=5  -  <=1  =  2..5)

WHY (bucket): atMost(k) = CUMULATIVE dabba = (distinct=1) + (distinct=2) + ... + (distinct=k).
              atMost(k-1) = wahi bina aakhri layer.  ghata do -> sirf (distinct=k) bacha (baaki cancel).
              jaise bade dabbe me se chhota nikaalo -> extra layer = exactly k.

DRY  : [1,2,1,2,3], k=2   -- atMost(2) = konse subarray?
           distinct=1 (5):  [1] [2] [1] [2] [3]
           distinct=2 (7):  [1,2] [2,1] [1,2] [2,3] [1,2,1] [2,1,2] [1,2,1,2]   <- ANSWER
           atMost(2) = 5 + 7 = 12  (distinct 1 aur 2 dono gine)
           atMost(1) = 5           (sirf distinct=1)
           12 - 5 = 7 ✓            (distinct=1 dono me tha -> cancel, sirf exactly-2 bache)

★ k=1 pe special-case NAHI: atMost(0) naturally 0 -> subtraction rehne do.
★ TRAP: atMost me ans = 0 (max-template se copy karo to INT_MIN reh jaata -> k=1 pe garbage).

FAMILY: fruit-into-baskets · longest-k-distinct · (koi bhi "exactly K" -> atMost(k)-atMost(k-1))
```

> [!IMPORTANT]
> **CHAABI:** exactly(k) = atMost(k) - atMost(k - 1).

> [!WARNING]
> atMost me count 0 se shuru. Max wale template se `INT_MIN` copy kiya to k=1 pe garbage.

**CODE**
```cpp
int atMostK(vector<int> &fruits, int k) {
    int i = 0, j = 0, count = 0;
    unordered_map<int, int> mp;
    while (j < fruits.size()) {
        mp[fruits[j]]++;
        while (mp.size() > k) {             // K se zyada distinct -> shrink
            mp[fruits[i]]--;
            if (mp[fruits[i]] == 0)
                mp.erase(fruits[i]);        // erase: warna 0-count key bhi mp.size() me ginti
            i++;
        }
        count += j - i + 1;                 // is j pe khatam hone wale valid subarray
        j++;                                // count jodne ke BAAD aage badho
    }
    return count;
}

int subarraysWithKDistinct(vector<int> &nums, int k) {
    // ★ exactly k seedha nahi banta: 12 - 5 = 7. k=1 pe atMost(0) apne aap 0
    return atMostK(nums, k) - atMostK(nums, k - 1);   // atMost(k) = distinct 1 + 2 + .. + k ka dabba;
                                                       // atMost(k-1) bina aakhri layer -> exactly k
}
```

---


## NEED-MAP (t ke saare char chahiye)

### ▸ LC-76 MIN WINDOW SUBSTRING (Hard)  -- need-map + COUNT, answer = MIN-track, loop ke ANDAR
**SAWAAL**  `s="ADOBECODEBANC", t="ABC" -> "BANC"`. s ki sabse chhoti window jisme t ke saare akshar utni hi baar hon jitni baar t me hain (count ke saath; t="AA" ho to do A chahiye). Na mile to "".

kyun: "ADOBEC" (6) bhi A,B,C rakhta, par "BANC" (4) sabse chhota

**SAAR**
```
FAMILY need-map + COUNT (t ke SAARE char chahiye -- --/++ MIRROR):
   LC-76 + LC-1358 ka EXPAND+SHRINK bilkul SAME. FARAK = answer KAHAN (LC-76 loop-ANDAR · LC-1358 loop-BAAD).
count = t.size(). count==0 => window me t ke SAARE char (VALID).
DERIVE: chhote example HAATH-trace (ADOBECODEBANC -> BANC) se map+count nikla.
```

> [!IMPORTANT]
> **CHAABI:** `count = t.size()`. Aaya `--`, gaya `++` (mirror). `count == 0` = valid, record `while` ke ANDAR.

> [!WARNING]
> `mp[s[j]]--` IF ke BAHAR. Andar rakha to t me na hone wale char (D) pe shrink me count++ galat (D wala bug).

**CODE**
```cpp
string minWindow(string s, string t) {
    unordered_map<char, int> mp;
    for (auto &it : t)
        mp[it]++;                           // need-map: t ke har char pe mp[c]++
    int i = 0, j = 0, count = t.size(), minLen = INT_MAX, index = 0;
    while (j < s.size()) {                  // OUTER: j (right) aage
        if (mp[s[j]] > 0)
            count--;                        // t-char mila -> ek zaroorat puri
        // ★ IF ke BAHAR: t me na hone wale char bhi minus me, warna shrink pe count++ galat
        mp[s[j]]--;                         //   (andar rakha to D-ASYMMETRY BUG)
        while (count == 0) {                // INNER: VALID -> shrink + SAVE (chhota wala: theek hai tab tak)
            int len = j - i + 1;
            if (len < minLen) {             // record PEHLE: abhi [i..j] valid, i++ ke baad shayad nahi
                minLen = len;
                index = i;
            }
            mp[s[i]]++;                     // aaya --, gaya ++ (MIRROR)
            if (mp[s[i]] > 0)
                count++;                    // >0 hua = t ka zaroori char gaya, count++
            i++;
        }
        j++;                                // shrink ke BAAD agla char lo
    }
    return minLen == INT_MAX ? "" : s.substr(index, minLen);   // INT_MAX bacha = koi valid window nahi -> ""
                                                                // s="a", t="aa" -> ""
}
```


---

### ▸ LC-1358 SUBSTRINGS WITH ALL THREE (a, b, c)  = LC-76 ka SAME code, answer = COUNT, loop ke BAAD
**SAWAAL**  `"abcabc" -> 10`. Kitne substring jinme a, b, c teeno.

e.g. "aaacb" -> 3 · kyun: "aaacb", "aacb", "acb" me teeno; "cb" ya usse chhote me a nahi = 3

**SAAR**
```
= LC-76 ka SAME minWindow() -- literally REUSE: numberOfSubstrings(s) = minWindow(s, "abc").
  farak SIRF answer-line:
      LC-76   : while ke ANDAR -> if(len<minLen){minLen; index}   (MIN track)
      LC-1358 : while ke BAAD  -> ans += i                        (COUNT)
WHY ans += i: shrink window INVALID tak le jaata -> ab [0..i-1] SAARE valid left-starts -> ginti = i.
   "abcabc": har j pe i = 0,0,1,2,3,4 -> jod 10
★ Product < k wala j-i+1 yahan galat hai.
```

> [!IMPORTANT]
> **CHAABI:** LC-76 ka same code, bas answer `ans += i`, `while` ke BAAD.

> [!WARNING]
> Product < k wala `j - i + 1` yahan galat hai, yahan `i`.

**CODE**
```cpp
int minWindow(string s, string t) {
    unordered_map<char, int> mp;
    for (auto &it : t)
        mp[it]++;
    int i = 0, j = 0, count = t.size(), ans = 0;
    while (j < s.size()) {
        if (mp[s[j]] > 0)
            count--;
        mp[s[j]]--;                         // EXPAND (same)
        while (count == 0) {                // INNER: shrink to INVALID
            mp[s[i]]++;
            if (mp[s[i]] > 0)
                count++;
            i++;
        }
        // ★ j-i+1 NAHI. while ke baad start 0..i-1 sab theek the -> i
        ans += i;                           // outer ke ANDAR, inner-while ke BAAD -> har j pe
        j++;
    }
    return ans;
}

int numberOfSubstrings(string s) {
    return minWindow(s, "abc");             // t = "abc" (count = 3)
}
```

---


## ANAGRAM (fixed size = p ki length)

### ▸ LC-438 FIND ALL ANAGRAMS  = FIXED window (p ki length) + do map ki ginti (PARENT)  = LC-76 ka bhai
**SAWAAL**  `s="cbaebabacd", p="abc" -> [0,6]`. s me p ke saare anagram ki start index.

kyun: s[0..2]="cba" aur s[6..8]="bac" dono "abc" ke anagram

**SAAR**
```
SAAR : s me p ke SAARE anagram ki START-INDEX. fixed window (p ki length) + har window pe anagram-check.
LC-76 se farak: wahan EK map me t ki ginti, aana-jaana pe usi map ko ghatao-badhao;
   yahan DO map (mp1 = p, mp2 = window), aana +1 / jaana -1, 0 hua to erase, phir mp1 == mp2 milao. window fixed = p.size().
★ FIXED window = WHILE nahi, ek IF kaafi: window poori (p.size()) hote hi usi if me i++, neeche j++ ->
   window hamesha p.size() ki rehti, isse badi kabhi nahi. isliye map banaya: dono barabar size, seedha mp1 == mp2.
★ GALTI: pehle har window ka substr bana ke isAnagram -> LeetCode pe TLE (34/66). ginti har baar dobara mat banao.
```

> [!IMPORTANT]
> **CHAABI:** do map (p ki ginti, window ki ginti). Aana +1, jaana -1. Window fixed p.size() -> `if`, phir `mp1 == mp2`.

> [!WARNING]
> 0 hua to ERASE, warna 0 wali key padi rehti aur `mp1 == mp2` kabhi nahi. Har window ka substr banaya to TLE (34/66).

**CODE**
```cpp
vector<int> findAnagrams(string s, string p) {
    vector<int> ans;
    int i = 0, j = 0;
    unordered_map<char, int> mp1;           // p ki ginti, sirf EK baar
    unordered_map<char, int> mp2;           // window ki ginti, jodte-ghatate
    for (int i = 0; i < p.size(); i++)
        mp1[p[i] - 'a'] += 1;
    while (j < s.size()) {
        mp2[s[j] - 'a'] += 1;               // ★ j aaya -> +1
        if (j - i + 1 == p.size()) {        // == : window poora p-size hua tabhi check
            if (mp1 == mp2)                 // dono map barabar = anagram -> START-index push
                ans.push_back(i);
            mp2[s[i] - 'a']--;              // ★ i gaya -> -1
            if (mp2[s[i] - 'a'] == 0)       // ★ 0 hua to ERASE, warna 0 wali key padi rehti aur mp1 == mp2 kabhi nahi
                mp2.erase(s[i] - 'a');
            i++;
        }
        j++;
    }
    return ans;
}
```


---

### ▸ LC-567 PERMUTATION IN STRING  = FIND-ALL-ANAGRAMS ka EXACT same code
**SAWAAL**  `s1="ab", s2="eidbaooo" -> true`. Kya s2 me s1 ka koi anagram (permutation) lagaatar substring ban ke maujood hai? true ya false.

kyun: s2[3..4]="ba" = "ab" ka permutation

**SAAR**
```
SAAR : s2 me s1 ka koi permutation (=anagram) substring hai? (bool)
farak SIRF answer-line:
    LC-438 : mp1 == mp2 -> ans.push_back(i)  ... loop-baad: return ans    (SAARE indices)
    LC-567 : mp1 == mp2 -> return true        ... loop-baad: return false  (pehla match = bool)
★ ULTA kram: s1 ko s2 ke ANDAR dhoondhna (window s2 pe chalti).
```

> [!IMPORTANT]
> **CHAABI:** LC-438 ka same code, bas `push_back(i)` ki jagah `return true`.

> [!WARNING]
> s1 ko s2 ke ANDAR dhoondhna hai, window s2 pe chalti (ulta mat karo).

**CODE**
```cpp
bool checkInclusion(string s1, string s2) {
    unordered_map<char, int> mp1;           // s1 ki ginti, EK baar
    unordered_map<char, int> mp2;           // window ki ginti
    for (int i = 0; i < s1.size(); i++)
        mp1[s1[i] - 'a']++;
    int i = 0, j = 0;
    while (j < s2.size()) {
        mp2[s2[j] - 'a']++;                 // ★ j aaya -> +1
        if (j - i + 1 == s1.size()) {
            if (mp1 == mp2)
                return true;                // <- DELTA: push ki jagah return true
            mp2[s2[i] - 'a']--;             // ★ i gaya -> -1
            if (mp2[s2[i] - 'a'] == 0)      // ★ 0 hua to ERASE
                mp2.erase(s2[i] - 'a');
            i++;
        }
        j++;
    }
    return false;                           // loop-baad: koi na mila
}
```

---


## WINDOW KA MAX (deque)

### ▸ LC-239 SLIDING WINDOW MAXIMUM  = monotonic DEQUE of INDICES (monotonic stack + front-expiry)
**SAWAAL**  `[1,3,-1,-3,5,3,6,7], k=3 -> [3,3,5,5,6,7]`. Har k size ki window ka sabse bada number lautao.

**SAAR**
```
SAAR : har size-k window ka MAX, O(n). deque me INDICES rakho (values DECREASING) -> front = window ka max.
CONNECT : back-pop (nums[back]<=nums[i]) = MONOTONIC banana -- ye UNIVERSAL (stack ho ya deque, wahi cheez; isi se monotonic).
          DEQUE lene ki ASLI (aur AKELI) wajah = WINDOW-EXPIRY: front se purana index hatana (dq.front()==i-k -> pop_front).
          = monotonic STACK + bas wahi 1 window-front-expiry delta. (front-pop hamesha EXISTING element hota, naya nahi.) order dono me matter.
CORE : nums[i] aaya -> usse CHHOTE back-wale BEKAAR (jab tak nums[i] zinda, wo max ban hi nahi sakte) -> pop_back.
★ ORDER MATTERS (warna galat ans): (1) front-expiry -> (2) monotonic-clean + push -> (3) record. record hamesha LAST.
TRAP: deque me INDICES (values nahi -- expiry ke liye index chahiye) · front-expiry ==i-k kaafi (har step check) · record sirf i>=k-1.
FAMILY: MIN chahiye -> pop_back condition ULTA (nums[back] >= nums[i]).
```

> [!IMPORTANT]
> **CHAABI:** deque me INDEX, values ghatte kram me, front = max. Kram: (1) front se purana hatao (2) peeche ke chhote pop + push (3) record.

> [!WARNING]
> value nahi INDEX rakho (bahar kaun gaya, index se pata chalta). Record sirf `i >= k - 1`.

**CODE**
```cpp
vector<int> maxSlidingWindow(vector<int> &nums, int k) {
    vector<int> ans;
    // ★ INDEX rakho, value nahi: kaun window se bahar gaya ye index se pata chalta
    // INDICES, values DECREASING -> front = max
    deque<int> dq;
    for (int i = 0; i < nums.size(); i++) {
        if (!dq.empty() && dq.front() == i - k)   // 1) window se bahar (==i-k) -> hata
            dq.pop_front();                       //    (!dq.empty() pehle: khaali deque pe front() crash)
        // ★ 2) peeche ke chhote bekaar, kabhi max nahi banenge. MIN chahiye to >=
        while (!dq.empty() && nums[dq.back()] <= nums[i])
            dq.pop_back();                  // <= : barabar wala purana bhi bekaar, naya zyada der tikega
        dq.push_back(i);
        if (i >= k - 1)                     // 3) record. kram 1 -> 2 -> 3 hi (k-1 = pehle poore window ka aakhri index)
            ans.push_back(nums[dq.front()]);
    }
    return ans;
}
```


