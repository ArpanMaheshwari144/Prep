## PATTERN 2 — SLIDING WINDOW



**Kab SW:** lagaatar subarray / substring + sabse lamba / sabse chhota / kitne.
**Kab nahi:** variable window jisme SUM pe shrink ho (`sum > k` pe chhota karo), aur numbers negative hon. Negative jodne pe sum ghat jaata hai, to "bada ho gaya, chhota karo" wala faisla toot jaata hai. Wahan prefix + map lagta hai. Fixed size window (max sum of k) negative ke saath bhi chalti hai.

```
 dhaancha (lamba wale):  j se jodo -> kharab ho to while me i se nikaalo -> ans lo -> j++
 sabse chhota wale:      j se jodo -> THEEK rahe tab tak while me ans lo + i se nikaalo -> j++
 fixed size k:           while ki jagah IF (ek juda, ek nikla)
```

---

## LAMBA wale (kharab hote hi chhota karo, while ke BAAD ans = max)

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
 ★ EDGE (variable window): shrink me count 0 ho jaye to map se ERASE karo, warna map.size() jhooth bolega.
         arr[j] window me ADD;               // track: sum · freq-map (mp.size()) · zeros · prod · maxFreq
         while (WINDOW BAD) {                // SIZE fix (k / p.length()) -> IF (ek add, ek remove)
                                             // SIZE condition pe (sum / distinct / repeat / product) -> WHILE
                                             // TRAP: max-sum-K me k = SIZE (IF); product<K me k = LIMIT
             arr[i] REMOVE; i++;             // remove PEHLE, phir i++ (warna agla element hat jaata)
     // sirf 2 cheez badalti: (1) inner IF ya WHILE  (2) "WINDOW BAD" kya + kya track. baaki SAME.
     // LONGEST  -> shrink jab INVALID (majboori), record shrink ke BAAD, ans = MAX
```

### ▸ LC-3 NO REPEAT  
`"abcabcbb" -> 3`. Sabse lambi substring jisme koi char repeat na ho.

```
YAAD:

INVALID = mp[s[j]]>1 (jo abhi add hua wahi repeat). empty-string edge -> INT_MIN -> 0.

PURANI SHEET KI KYUN LINE (code ke saath wali):
unordered_map<char,int> mp;  int i=0, j=0, maxLen=INT_MIN;   // INT_MIN: end me check karke khaali string pe 0
    while(mp[s[j]] > 1){                 // INVALID: current char (s[j]) repeat (while: repeat wala char nikalne tak kai char hatane padte)
        mp[s[i]]--; i++;                 // left shrink jab tak repeat khatam
    maxLen = max(maxLen, j - i + 1);     // valid window length (shrink ke BAAD record, ab window valid)
    j++;   // record ke BAAD aage badho
return maxLen==INT_MIN ? 0 : maxLen;     // khaali string -> INT_MIN reh jaata -> 0
```

```cpp
int lengthOfLongestSubstring(string s) {
    if (s.empty())
        return 0;
    int ans = INT_MIN;
    unordered_map<char, int> mp;
    int i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;
        // ★ "abba": a aur purana b, dono nikaalo. if -> 3, sahi 2
        while (mp[s[j]] > 1) {
            mp[s[i]]--;
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : longest substring jisme koi char REPEAT na ho. variable window + freq-map(mp).
PEHLI SHEET (Aug) se bhi, same to same:
SAAR : longest substring jisme koi char REPEAT na ho. variable window + freq-map(mp).
    unordered_map<char,int> mp;  int i=0, j=0, minLen=INT_MIN;
        minLen = max(minLen, j - i + 1);     // valid window length
    return minLen==INT_MIN ? 0 : minLen;     // khaali string -> INT_MIN reh jaata -> 0
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     string s. sabse LAMBI substring ki LENGTH lautao jisme koi char REPEAT na ho.
       kyun: "abc" me teeno alag = 3; agla a aate hi repeat, isse lamba koi substring nahi
```

### ▸ LC-424 CHAR REPLACEMENT  
`"AABABBA", k=1 -> 4`. Max k char badal ke sabse lambi window jisme sab same.

```
YAAD:

KEY  : windowLen - maxFreq = kitne char badalne padenge. <=k -> valid.
CORE TRICK (non-obvious): maxFreq kabhi GHATAO mat (shrink pe recompute nahi). window best-length
   se chhota hota hi nahi (sirf badhta/slide) -> stale/purana maxFreq bhi answer kharab nahi karta.

PURANI SHEET KI KYUN LINE (code ke saath wali):
j pe : mp[s[j]]++, maxFreq = max(maxFreq, mp[s[j]]).   // maxFreq update PEHLE, phir windowLen-maxFreq check
            if (mp[s[i]] == 0) mp.erase(s[i]);   // 0 hua to key hatao (DISTINCT gino to zaroori)
            i++;   // ginti kam karne ke BAAD hi left aage
```

```cpp
int characterReplacement(string s, int k) {
    unordered_map<char, int> mp;
    int maxLen = INT_MIN, maxFreq = INT_MIN, i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;
        maxFreq = max(maxFreq, mp[s[j]]);
        // ★ length - sabse zyada wala = kitne badalne padenge
        while (j - i + 1 - maxFreq > k) {
            mp[s[i]]--;
            if (mp[s[i]] == 0)
                mp.erase(s[i]);
            i++;
        }
        if (j - i + 1 - maxFreq <= k)
            maxLen = max(maxLen, j - i + 1);
        j++;
    }
    return maxLen;
}
```
`★ maxFreq ko shrink pe ghatao mat, recompute ki zaroorat nahi.` Wo ab tak ki sabse badi ginti hai. Thodi der purana reh jaaye to bhi best galat nahi aata (tera code har test pe sahi).

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : longest window jisme <=k char badal ke sabko SAME bana sako. variable window + freq-map.
INVALID (windowLen - maxFreq > k)   -> left shrink:
            mp[s[i]]--;   // left wala char window se bahar, uski ginti kam
PEHLI SHEET (Aug) se bhi, same to same:
SAAR : longest window jisme <=k char badal ke sabko SAME bana sako. variable window + freq-map.
INVALID (windowLen - maxFreq > k)   -> left shrink (mp[s[i]]--, i++).
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     string s (UPPERCASE) + k. max k char badal sakte. sabse lambi substring jisme sab char SAME -> LENGTH.
       kyun: "AABA" (index 0..3) me ek B ko A banao -> "AAAA" = 4
                                                 // window best-length se chhota nahi -> purana maxFreq theek
```

### ▸ LC-1004 MAX CONSECUTIVE ONES III  
`[1,1,1,0,0,0,1,1,1,1,0], k=2 -> 6`. Max k zero ko 1 bana ke sabse lambi 1 ki line.

```
YAAD:

CORE LEARNING: ans UNCONDITIONAL update -> if(zeroCount==k) MAT lagao.
   kyun: upar while(zeroCount>k) shrink ke BAAD window HAMESHA valid (<=k) -> yahan pahunchte hi valid -> condition faltu.
   (galti thi: if(zeroCount==k) -> sirf exactly-k ginta -> k=0 / k>zeros case toot jaate.)
shrink pe nums[i]==0 hi zeroCount-- (1 nikalne pe count na badle).

PURANI SHEET KI KYUN LINE (code ke saath wali):
int ans=INT_MIN, zeroCount=0, i=0, j=0;   // INT_MIN: end me check karke khaali array pe 0
    while(zeroCount > k){                // INVALID: k se zyada zero (flip nahi kar sakte)
        i++;   // zero check ke BAAD aage (pehle i++ to galat element dekhta)
```

```cpp
int longestOnes(vector<int> &nums, int k) {
    int ans = INT_MIN;
    int zeroCount = 0;
    int i = 0, j = 0;
    while (j < nums.size()) {
        if (nums[j] == 0)
            zeroCount++;
        while (zeroCount > k) {
            if (nums[i] == 0)
                zeroCount--;
            i++;
        }
        // ★ bina shart lo. if (zeroCount == k) -> [1,1,0], k=2 pe 0, sahi 3
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}
```
`↳ SAME: LC-3 wala dhaancha. Farak: map ki jagah ek int (zeroCount), aur shrink "zero > k" pe.`

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : at most k zeros ko FLIP -> longest all-1 window. sirf zeroCount track (freq-map nahi).
        if(nums[j]==0) zeroCount++;          // window me zero add
            if(nums[i]==0) zeroCount--;      // left ka zero nikla to count kam
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     binary nums[] (0/1) + k. max k zero ko 1 me FLIP kar sakte. sabse lambi lagaatar 1s ki LENGTH lautao.
       kyun: index 4..9 = 0,0,1,1,1,1: do zero flip -> 6 lagaatar 1
                     zeroCount--;                // sirf zero nikla tabhi kam (1 nikalne pe nahi)
                                                 // window valid; ==k = sirf exactly-k, k=0 / k>zeros toot-te
```

### ▸ FRUIT INTO BASKETS  
`[1,2,3,2,2] -> 4`. 2 basket, har basket me ek type. Lagaatar pedon ki sabse lambi line.

```
YAAD:

SAAR : "longest subarray with AT MOST 2 DISTINCT" (fruit/basket sirf kahani). SW skeleton + map<type,count>.
while (mp.size() > 2) KYUN while: ek type poora khatam hone tak kai element hatane padte.
   IF bhi chalta (sirf LONGEST): ek aaya ek gaya, size best se bada nahi -> len sahi. COUNT / SHORTEST me sirf while.
CRUX = erase-on-0: count 0 hone pe key map me PADI reh jaati -> mp.size() galat distinct dega -> condition toot. isliye ERASE.

PURANI SHEET KI KYUN LINE (code ke saath wali):
    if(mp[nums[i]]==0) mp.erase(nums[i]);   // count 0 -> ERASE
    i++;   // erase check ke BAAD hi left aage
ans = max(ans, j - i + 1);   // shrink ke BAAD record: window ab <=2 distinct · ans=INT_MIN se, return ans==INT_MIN ? 0 : ans
```

```cpp
int totalFruit(vector<int> &fruits) {
    int ans = INT_MIN;
    unordered_map<int, int> mp;
    int i = 0, j = 0;
    while (j < fruits.size()) {
        mp[fruits[j]]++;
        // ★ ginti nahi, ALAG type kitne = mp.size()
        while (mp.size() > 2) {
            mp[fruits[i]]--;
            // ★ erase nahi kiya to size 3 hi rehta, loop rukta nahi, i array ke bahar -> crash
            if (mp[fruits[i]] == 0)
                mp.erase(fruits[i]);
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
DELTA (SW skeleton pe -- sirf ye badalta):
    mp[nums[j]]++;                              // window me add
    while(mp.size() > 2){                       // INVALID: 2 se zyada distinct (while: ek type poora khatam hone tak kai element hatane padte)
        mp[nums[i]]--;   // left wala fruit window se bahar, uski ginti kam
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     fruits[] = pedon ki line, har number ek type. 2 basket (ek me ek type). lagaatar pedon ki sabse lambi window ki LENGTH.
       kyun: [2,3,2,2] me sirf 2 type (2,3) = 4; aage 1 jodo to 3 type ho jaate
                     mp.erase(fruits[i]);        // CRUX erase-on-0: warna 0 wali key bhi mp.size() me
```

### ▸ LONGEST AT-MOST-K DISTINCT  
`"eceba", k=2 -> 3`. Sabse lambi substring jisme zyada se zyada k alag char.

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
`↳ SAME: fruit hi hai. Farak sirf "> 2" ki jagah "> k".`

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
FRUIT-INTO-BASKETS ka GENERAL version. wahi variable window + map<char,count>,
distinct (mp.size()) > k ho -> left se shrink (erase-on-0). ans = max length.
farak: fruit me "2" hardcoded tha, yahan wahi jagah "k" (mp.size() > k).
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
       kyun: "ece" me sirf e,c (2 alag) = 3; "eceb" me 3 alag ho jaate
                     mp.erase(s[i]);             // erase-on-0 (fruit jaisa)
```

### ▸ LC-1208 EQUAL SUBSTRING WITHIN BUDGET  
`s="abcd", t="bcdf", maxCost=3 -> 3`. s ke char ko t me badalne ki cost `|s[i]-t[i]|`. Kul cost budget ke andar, sabse lambi window.

```cpp
int equalSubstring(string s, string t, int maxCost) {
    int i = 0, j = 0, cost = 0, ans = INT_MIN;
    while (j < t.size()) {
        // ★ string ke bhes me "sum <= k" wala sawaal
        cost += abs(s[j] - t[j]);
        while (cost > maxCost) {
            cost -= abs(s[i] - t[i]);
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans;
}
```
`↳ SAME: ones III jaisa, ek int. Farak: zero ki jagah cost jodo. Cost hamesha 0 ya zyada, isliye negative wala dar nahi.`

---

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : s ko t banane me har char ka cost = |s[i]-t[i]|. longest window jiska cost-SUM <= maxCost.
KEY  : cost-array bana ke "longest subarray with sum <= k" -- bas string ke bhes me chhupa.
j pe : cost += abs(s[j]-t[j]).
INVALID (cost > maxCost) -> WHILE shrink: cost -= abs(s[i]-t[i]), i++.
ans  : max(ans, j-i+1).  track sirf cost (koi map nahi).
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     s, t same length + maxCost. s[i]->t[i] cost = |s[i]-t[i]|. sabse lamba window jiska total cost <= maxCost -> LENGTH.
       kyun: cost = [1,1,1,2]; pehle 3 ka 1+1+1 = 3 <= 3; chaaron ka 5 > 3
         while (j < t.size()) {                  // s="abcd", t="cdef", maxCost=3 -> 1
── FAMILY: variable-SHORTEST (ULTA: VALID ho tab tak shrink, ans = MIN length) ──
```

## CHHOTA wala (theek rahe tab tak chhota karo, ans while ke ANDAR = min)

### ▸ LC-209 MIN SUBARRAY LEN  
`target=7, [2,3,1,2,4,3] -> 2` ([4,3]). Positive numbers, sum >= target wala sabse chhota subarray.

```
YAAD:

LONGEST vs SHORTEST (yahi confusion ki JAD -- window BADA chahiye ya CHHOTA):
   LONGEST  (jaise no-repeat): window BADA chahiye -> shrink SIRF jab INVALID ho (validity wapas laane ko) ->
            record BAAHAR (shrink ke BAAD, valid window ki length).
   SHORTEST (ye / min-window):  window CHHOTA chahiye -> VALID hote hi record + shrink (chhota karte jao jab tak valid) ->
            record ANDAR (shrink loop ke andar, har step).   <- YE wahi.
   1-line: LONGEST = shrink-jab-MAJBOORI(invalid), record-baad  ·  SHORTEST = shrink-jab-tak-VALID, record-andar.
char-matching wala SHORTEST = MIN WINDOW (LC-76) -> neeche "need-map + COUNT" family me (LC-1358 ke saath).

PURANI SHEET KI KYUN LINE (code ke saath wali):
int sum=0, minLen=INT_MAX, i=0, j=0;   // INT_MAX: min se ghatega; koi window na bani to end me 0
    j++;   // shrink loop ke BAHAR: har j pe ek baar aage
return minLen==INT_MAX ? 0 : minLen;      // koi valid window nahi -> 0
```

```cpp
int minSubArrayLen(int target, vector<int> &nums) {
    int sum = 0;
    int size = INT_MAX;
    int i = 0, j = 0;
    while (j < nums.size()) {
        sum += nums[j];
        // ★ ULTA: theek hai tab tak chhota karo, ans ANDAR nikaalne se pehle
        while (sum >= target) {
            size = min(size, j - i + 1);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return size == INT_MAX ? 0 : size;      // target=11, [1,1,1] -> 0
}
```
`↳ SAME: lamba wala dhaancha ulta. Lamba: kharab pe while, ans baad me, max. Chhota: theek pe while, ans andar, min.`

---

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : positive nums, sum >= target wala SABSE CHHOTA subarray ki length (nahi mile -> 0).
        sum += nums[j];                       // window add (expand)
        while(sum >= target){                 // VALID hote hi -> record + SHRINK (aur chhota dhoondo)
            sum -= nums[i]; i++;               //   left shrink
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     positive nums[] + target. sabse CHHOTI length ka subarray jiska sum >= target; koi na ho to 0.
       kyun: [4,3] = 7 >= 7, length 2; akela koi number 7 tak nahi pahunchta
── FAMILY: FIXED window (size k pehle se diya, slide) ──
```

## FIXED size k (while ki jagah IF)

### ▸ MAX SUM OF K  
`[2,1,5,1,3,2], k=3 -> 9`. Har size-k window ka sum, sabse bada.

```
YAAD:

PURANI SHEET KI KYUN LINE (code ke saath wali):
j pe : sum += nums[j].   // sum=0, maxSum=INT_MIN (sab negative ho to bhi sahi max)
```

```cpp
int maxSumK(vector<int> &nums, int k) {
    int sum = 0;
    // ★ 0 se shuru kiya to [-1,-2,-3], k=2 -> 0, sahi -3
    int maxSum = INT_MIN;
    int i = 0, j = 0;
    while (j < nums.size()) {
        sum += nums[j];
        if (j - i + 1 >= k) {               // IF: size fix, ek juda to ek nikla
            maxSum = max(maxSum, sum);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return maxSum;
}
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : k-size ke har window ka sum -> unme se MAX. fixed window (size hamesha k).
KEY  : variable nahi -- size fix k, isliye IF (ek hi remove).
FULL (j-i+1 >= k) -> maxSum = max(maxSum, sum); phir 1 remove (sum -= nums[i], i++).
WHY IF-not-WHILE : fixed size -> har step 1 add + 1 remove -> slide. variable hota to WHILE (kai remove).
ans  : maxSum. window k-size pe slide.
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     array + k. har size-k lagaatar subarray ka sum dekho, sabse BADA sum lautao.
       kyun: window sums 8, 7, 9, 6 -> max 9 (5+1+3)
                 maxSum = max(maxSum, sum);      // FULL window -> record
── FAMILY: COUNT (length nahi; valid window pe count += j-i+1) ──
```

### ▸ LC-1423 MAX POINTS FROM CARDS  
`[1,2,3,4,5,6,1], k=3 -> 12` (peeche se 5+6+1). Dono kinaron se kul k card lo, sum sabse bada.

```
TASVEER:

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

PURANI SHEET KI KYUN LINE (code ke saath wali):
  int Total = accumulate(nums, 0);   // saare cards ka sum: isme se beech ka min ghatayenge
minSumK (fixed window, MIN): sum += nums[j]; jab (j-i+1 >= size) -> minSum=min(minSum,sum); sum-=nums[i]; i++.   // minSum=INT_MAX se
```

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
`↳ SAME: max sum of k ka MIN version. Farak: seedha nahi, ulta socho, Total - beech ka min.`

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
INPUT: [1,2,3,4,5,6,1], k=3 -> 12   (back se 5,6,1)
★ COLD nahi hota (combo-twist) -- library me daalke revise -> next time instant.
PEHLI SHEET (Aug) se bhi, same to same:
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     cardPoints[] + k. EXACTLY k card lo, sirf dono ENDS se (front/back mix chalega). max SUM lautao.
       kyun: back se 3 = 1+6+5 = 12; front 3 = 6, mix (1+2+1 = 4, 1+1+6 = 8) kam
         int minSum = INT_MAX;                   // INT_MAX se: min ghatega
```

### ▸ LC-219 CONTAINS DUPLICATE II  
`[1,2,3,1], k=3 -> true`. Do barabar number jinka index ka farak <= k.

```
TASVEER:

★ RECOGNITION: "same value + index farak <=k ke ANDAR" -> window me sirf LAST k elements rakho (HashSet).
   KEY INSIGHT: agar set me sirf last-k elements hain, aur andar duplicate mila -> uska index-farak apne-aap <=k
   -> "|i-j|<=k" alag se check karne ki zaroorat NAHI. bas window ko k-size me rakho.
DRY-RUN [1,2,3,1] k=3: 1,2,3 add -> j=3 nums=1 set me hai -> true. | [1,2,3,1,2,3] k=2: har baar window<=2, kabhi dup nahi -> false.
★ k=0 kyun alag: check (st.count) window CHHOTI hone se PEHLE hota hai -> k=0 pe pichhla element
   abhi set me hota, [4,4] k=0 pe true aa jaata (sahi false).
FAMILY: fixed-window (size k) + HashSet (seen-check). value-equal -> set; within-k -> window size k.
```

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

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
    if (k <= 0) return false;   // k=0: |i-j|<=0 matlab same index, jodi ban hi nahi sakti
        if(st.count(nums[j])) return true;   // window me pehle se hai -> mil gaya (within k)
        if(j - i + 1 > k){ st.erase(nums[i]); i++; }   // window k se badi -> oldest hatao
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     nums[] + k. kya do index i, j hain jahan nums[i]==nums[j] aur |i-j| <= k? true/false.
       kyun: index 0 aur 3 dono pe 1, farak 3 <= 3
     nums[i]==nums[j] AND |i-j| <= k ?
     bool containsNearbyDuplicate(vector<int> &nums, int k) {   // nums[i]==nums[j] AND |i-j| <= k ?
             return false;                       // k=0: check shrink se PEHLE -> [4,4] k=0 galat true
```

## KITNE (ginti, length nahi)

### ▸ SUBARRAY PRODUCT < K  
`[10,5,2,6], k=100 -> 8`. Positive numbers, kitne subarray jinka guna k se kam.

```
TASVEER:

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

PURANI SHEET KI KYUN LINE (code ke saath wali):
if (k == 0) return 0;   // nums >=1: product kabhi < 0 nahi, koi subarray nahi
        prod /= nums[i];   // left wala guna se bahar: divide karo
        i++;   // divide ke BAAD hi left aage
    j++;   // count ke BAAD aage badho
```

```cpp
int numSubarrayProductLessThanK(vector<int> &nums, int k) {
    if (k == 0)
        return 0;
    int count = 0;
    int i = 0, j = 0, prod = 1;             // guna 1 se shuru, 0 se sab 0
    while (j < nums.size()) {
        prod *= nums[j];
        // ★ i <= j: [1,1,1], k=1 pe i bahar bhaagta
        while (prod >= k && i <= j) {
            prod /= nums[i];
            i++;
        }
        // ★ j pe khatam hone wale saare: [j], [j-1..j] .. [i..j]
        count += j - i + 1;
        j++;
    }
    return count;
}
```
`↳ SAME: lamba wala dhaancha. Farak: max ki jagah count += j - i + 1.`

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : kitne subarray jinka product < k.   (lambai nahi, GINTI chahiye)
    int count = 0, prod = 1;   // prod=1: guna ka shuru 1 (0 rakha to sab 0)
        prod *= nums[j];                      // naya element window me
        while (prod >= k && i <= j) {         // zyada ho gaya -> baayi taraf se chhoto (i <= j: k=1 pe khaali window ka prod 1, i bahar bhaagta)
PEHLI SHEET (Aug) se bhi, same to same:
SAAR : kitne subarray jinka product < k. variable window, COUNT (length nahi).
INVALID (prod >= k) -> shrink: prod /= nums[i], i++.
COUNT TRICK: valid -> count += (j-i+1) = window size (j pe end hone wale saare valid subarray). (bahut count-Q me)
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     positive nums[] + k. kitne lagaatar subarray jinka PRODUCT strictly k se kam -> GINTI.
       kyun: 4 akele + [10,5]=50, [5,2]=10, [2,6]=12, [5,2,6]=60 = 8; [10,5,2]=100 kam nahi
── FAMILY: need-map + COUNT (t ke SAARE char chahiye; aana = --, jaana = ++ MIRROR) ──
```

### ▸ LC-992 SUBARRAYS WITH EXACTLY K DISTINCT  
`[1,2,1,2,3], k=2 -> 7`. Kitne lagaatar subarray hain jinme theek k alag number hon.

```
TASVEER:

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

PURANI SHEET KI KYUN LINE (code ke saath wali):
    if(mp[nums[i]] == 0) mp.erase(nums[i]);   // erase: warna 0-count key bhi mp.size() me ginti
ans += j - i + 1;                         // is j pe khatam hone wale valid subarray
j++;   // count jodne ke BAAD aage badho
```

```cpp
int atMostK(vector<int> &fruits, int k) {
    int i = 0, j = 0, count = 0;
    unordered_map<int, int> mp;
    while (j < fruits.size()) {
        mp[fruits[j]]++;
        while (mp.size() > k) {
            mp[fruits[i]]--;
            if (mp[fruits[i]] == 0)
                mp.erase(fruits[i]);
            i++;
        }
        count += j - i + 1;
        j++;
    }
    return count;
}

int subarraysWithKDistinct(vector<int> &nums, int k) {
    // ★ exactly k seedha nahi banta: 12 - 5 = 7. k=1 pe atMost(0) apne aap 0
    return atMostK(nums, k) - atMostK(nums, k - 1);   // atMost(k) = distinct 1 + 2 + .. + k ka dabba;
                                                       // atMost(k-1) bina aakhri layer -> exactly k
}
```
`↳ SAME: atMostK = fruit ka code. Farak: max ki jagah count, aur upar exactly = atMost(k) - atMost(k-1).`

---

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : kitne SUBARRAYS jinme EXACTLY k different numbers.  [1,2,1,2,3],k=2 -> 7
        int i = 0, j = 0, ans = 0;  map<int,int> mp;   // ans=0: yahan count jod rahe, max nahi (INT_MIN = garbage)
            mp[nums[j]]++;   // naya number window me
            while(mp.size() > K){                     // K se zyada distinct -> shrink
                mp[nums[i]]--;   // left wala number bahar, uski ginti kam
    exactly = atMost(k) - atMost(k-1);   // <=k me se <=k-1 hatao, sirf exactly k bache
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
             count += j - i + 1;                 // is j pe khatam hone wale saare valid subarray
     // koi bhi "exactly K" -> atMost(k) - atMost(k-1)   (fruit · longest-k-distinct isi family)
```

## NEED-MAP (t ke saare char chahiye)

### ▸ LC-76 MIN WINDOW SUBSTRING  
`s="ADOBECODEBANC", t="ABC" -> "BANC"`. s ki sabse chhoti window jisme t ke saare akshar utni hi baar hon jitni baar t me hain (count ke saath; t="AA" ho to do A chahiye). Na mile to "".

```
TASVEER:

┌── FAMILY: need-map + COUNT (t ke SAARE char chahiye -- --/++ MIRROR) ─────
│ LC-76 + LC-1358 ka EXPAND+SHRINK bilkul SAME. FARAK = answer KAHAN (LC-76 loop-ANDAR · LC-1358 loop-BAAD). t="abc" -> dono ek.
└───────────────────────────────────────────────────────────────

SHARED SKELETON (expand + shrink -- dono me same):
     need-map(t);  count = t.size();                 // count==0 => window me t ke SAARE char (VALID) · need-map: t ke har char pe mp[c]++
     while (j < n) {                                  // OUTER: j (right) aage
         if (mp[s[j]] > 0) count--;                  // t-char mila -> ek zaroorat puri
         mp[s[j]]--;                                 // IF ke BAHAR: non-t char -ve -> count me "invisible"
                                                     //   (andar rakha to D-ASYMMETRY BUG: shrink me count++ galat)
         while (count == 0) {                         // INNER: VALID -> shrink (i++)
             mp[s[i]]++;  if (mp[s[i]] > 0) count++;  i++;   // MIRROR: aana = -- , jaana = ++
         }
         j++;   // shrink ke BAAD agla char lo
     }

DERIVE: chhote example HAATH-trace (ADOBECODEBANC -> BANC) se map+count nikla.

PURANI SHEET KI KYUN LINE (code ke saath wali):
        len=j-i+1; if(len<minLen){minLen=len; index=i;}   // record PEHLE: abhi [i..j] valid, i++ ke baad shayad nahi
        mp[s[i]]++; if>0 count++; i++;   // wapas ++; >0 hua = t ka zaroori char gaya, count++
return minLen==INT_MAX ? "" : s.substr(index, minLen).   // INT_MAX bacha = koi valid window nahi mila -> khaali string
```

```cpp
string minWindow(string s, string t) {
    unordered_map<char, int> mp;
    for (auto &it : t)
        mp[it]++;
    int i = 0, j = 0, count = t.size(), minLen = INT_MAX, index = 0;
    while (j < s.size()) {
        if (mp[s[j]] > 0)
            count--;
        // ★ IF ke BAHAR: t me na hone wale char bhi minus me, warna shrink pe count++ galat
        mp[s[j]]--;
        while (count == 0) {                // chhota wala: theek hai tab tak
            int len = j - i + 1;
            if (len < minLen) {             // record pehle, nikaalne se pehle
                minLen = len;
                index = i;
            }
            mp[s[i]]++;                     // aaya --, gaya ++ (mirror)
            if (mp[s[i]] > 0)
                count++;
            i++;
        }
        j++;
    }
    return minLen == INT_MAX ? "" : s.substr(index, minLen);   // s="a", t="aa" -> ""
}
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
    while (count == 0) {                 // INNER: VALID -> shrink + SAVE (answer andar)
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     s, t. s ka sabse CHHOTA substring jisme t ke saare char (count ke saath) aa jaayein; na ho to "".
       kyun: "ADOBEC" (6) bhi A,B,C rakhta, par "BANC" (4) sabse chhota
         int i = 0, j = 0, count = t.size(), minLen = INT_MAX, index = 0;   // count==0 VALID; index=start
```

### ▸ LC-1358 SUBSTRINGS WITH ALL THREE (a, b, c)  
`"abcabc" -> 10`. Kitne substring jinme a, b, c teeno.

```
TASVEER:

IDEA:
   = LC-76 ka SAME code reuse:  numberOfSubstrings(s) = minWindow(s, "abc")
     farak sirf answer ki line:
         LC-76   : while ke ANDAR -> if(len<minLen){minLen; index}   (sabse chhoti)
         LC-1358 : while ke BAAD  -> ans += i                        (ginti)
   ans += i KYUN: shrink window ko invalid hone tak le jaata -> 0 se i-1 tak har start valid tha -> i substring.
      "abcabc": har j pe i = 0,0,1,2,3,4 -> jod 10

PURANI SHEET KI KYUN LINE (code ke saath wali):
ans += i;                                   // outer ke ANDAR, inner-while ke BAAD -> har j pe
```

```cpp
// LC-76 se alag file / naam me rakhna: same naam, return type alag -> saath compile nahi hoga
int minWindow(string s, string t) {
    unordered_map<char, int> mp;
    for (auto &it : t)
        mp[it]++;
    int i = 0, j = 0, count = t.size(), ans = 0;
    while (j < s.size()) {
        if (mp[s[j]] > 0)
            count--;
        mp[s[j]]--;
        while (count == 0) {
            mp[s[i]]++;
            if (mp[s[i]] > 0)
                count++;
            i++;
        }
        // ★ j-i+1 NAHI. while ke baad start 0..i-1 sab theek the -> i
        ans += i;
        j++;
    }
    return ans;
}

int numberOfSubstrings(string s) {
    return minWindow(s, "abc");
}
```
`↳ SAME: LC-76 ka code. Farak: record nahi, aur ans += i (while ke baad). Product < k wala j-i+1 yahan galat hai.`

---

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
   CODE: numberOfSubstrings(s) = minWindow(s, "abc")  -- wahi SW function reuse, bas t="abc" pass.
PEHLI SHEET (Aug) se bhi, same to same:
= LC-76 ka SAME minWindow() -- literally REUSE: numberOfSubstrings(s) = minWindow(s, "abc").
      LC-76   : while ke ANDAR -> if(len<minLen){minLen; index}   (MIN track)
  WHY ans += i: shrink INVALID tak -> i = kitne valid left-starts. hand-trace "abcabc": i = 0,0,1,2,3,4 -> sum 10.
WHY ans += i: shrink window INVALID tak le jaata -> ab [0..i-1] SAARE valid left-starts -> ginti = i.
CODE: numberOfSubstrings(s) = minWindow(s, "abc")  -- wahi SW function reuse, bas t="abc" pass.
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     s me sirf a, b, c. kitne substrings jinme a, b, c TEENO kam se kam ek baar -> GINTI.
       e.g. "aaacb" -> 3
       kyun: "aaacb", "aacb", "acb" me teeno; "cb" ya usse chhote me a nahi = 3
── FAMILY: ANAGRAM-window (fixed p-length window + isAnagram helper reuse) ──
```

## ANAGRAM (fixed size = p ki length)

### ▸ LC-438 FIND ALL ANAGRAMS  
`s="cbaebabacd", p="abc" -> [0,6]`. s me p ke saare anagram ki start index.

```
YAAD:

PURANI SHEET KI KYUN LINE (code ke saath wali):
if(j-i+1 == p.size())   // == : window poora p-size hua tabhi check (shuru me chhota)
j++;   // check ke BAAD aage badho
```

```cpp
bool isAnagram(string s, string t) {
    if (s.size() != t.size())
        return 0;
    unordered_map<char, int> mp;
    for (auto &it : s)
        mp[it]++;
    for (auto &it : t)
        mp[it]--;
    for (auto &it : mp)
        if (it.second != 0)
            return false;
    return true;
}

vector<int> findAnagrams(string s, string p) {
    vector<int> ans;
    int i = 0, j = 0;
    while (j < s.size()) {
        while (j - i + 1 > p.size())
            i++;
        if (j - i + 1 == p.size())          // poori p-size ki window pe hi check
            if (isAnagram(s.substr(i, j - i + 1), p))
                ans.push_back(i);
        j++;
    }
    return ans;
}
```
`★ dhyaan: har window pe substr + isAnagram = har baar p jitna kaam (O(n * m)). Tez chahiye to window ki ginti map me jodte-ghatate raho, har baar dobara mat gino.`

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : s me p ke SAARE anagram ki START-INDEX. fixed window (p ki length) + har window pe anagram-check.
    bool isAnagram(a,b): size same + har char count barabar (map: a se ++, b se --, sab 0 -> true).
        while(j-i+1 > p.size()) i++;                          // window ko p-size pe rakho (FIXED)
valid-anagram helper reuse. fixed-window slide + har full-window pe check.
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     s, p. s me p ke saare ANAGRAM (same char, order koi bhi) ki START index lautao.
       kyun: s[0..2]="cba" aur s[6..8]="bac" dono "abc" ke anagram
```

### ▸ LC-567 PERMUTATION IN STRING  
`s1="ab", s2="eidbaooo" -> true`. Kya s2 me s1 ka koi anagram (permutation) lagaatar substring ban ke maujood hai? true ya false.

```
YAAD:

= FIND-ALL-ANAGRAMS ka SAME code -- REUSE: checkInclusion(s1,s2) = findAnagrams(s2, s1).
  farak SIRF answer-line:
      LC-438 : isAnagram mila -> ans.push_back(i)  ... loop-baad: return ans    (SAARE indices)
      LC-567 : isAnagram mila -> return true        ... loop-baad: return false  (pehla match = bool)
```

```cpp
// LC-438 se alag rakhna: same naam, return type alag
bool findAnagrams(string s, string p) {
    int i = 0, j = 0;
    while (j < s.size()) {
        while (j - i + 1 > p.size())
            i++;
        if (j - i + 1 == p.size())
            if (isAnagram(s.substr(i, j - i + 1), p))
                return true;                // push ki jagah return true
        j++;
    }
    return false;
}

bool checkInclusion(string s1, string s2) {
    // ★ ULTA kram: s2 me s1 dhoondho
    return findAnagrams(s2, s1);
}
```
`↳ SAME: LC-438. Farak: pehla mila to true, aur call me (s2, s1).`

---

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : s2 me s1 ka koi permutation (=anagram) substring hai? (bool)
CODE (findAnagrams; skeleton LC-438 wala, sirf answer-handle badla):
        while(j-i+1 > p.size()) i++;                  // window ko p-size pe rakho (FIXED)
                return true;                          // <- DELTA: push ki jagah return true
LONGEST vs SHORTEST (yaad rakh): LONGEST -> shrink jab INVALID, ans=MAX. SHORTEST (min-len) -> shrink jab VALID, ans=MIN.
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
       kyun: s2[3..4]="ba" = "ab" ka permutation
         return findAnagrams(s2, s1);            // dhyaan: ulta order (s2, s1)
── FAMILY: MONOTONIC DEQUE (window max/min; deque me INDICES, front = answer) ──
```

## WINDOW KA MAX (deque)

### ▸ LC-239 SLIDING WINDOW MAXIMUM  
`[1,3,-1,-3,5,3,6,7], k=3 -> [3,3,5,5,6,7]`. Har k size ki window ka sabse bada number lautao.

```
YAAD:

SAAR : har size-k window ka MAX, O(n). deque me INDICES rakho (values DECREASING) -> front = window ka max.
CONNECT : back-pop (nums[back]<=nums[i]) = MONOTONIC banana -- ye UNIVERSAL (stack ho ya deque, wahi cheez; isi se monotonic).
          DEQUE lene ki ASLI (aur AKELI) wajah = WINDOW-EXPIRY: front se purana index hatana (dq.front()==i-k -> pop_front).
          = monotonic STACK + bas wahi 1 window-front-expiry delta. (front-pop hamesha EXISTING element hota, naya nahi.) order dono me matter.
CORE : nums[i] aaya -> usse CHHOTE back-wale BEKAAR (jab tak nums[i] zinda, wo max ban hi nahi sakte) -> pop_back.
★ ORDER MATTERS (warna galat ans): (1) front-expiry -> (2) monotonic-clean + push -> (3) record. record hamesha LAST.
TRAP: deque me INDICES (values nahi -- expiry ke liye index chahiye) · front-expiry ==i-k kaafi (har step check) · record sirf i>=k-1.
FAMILY: MIN chahiye -> pop_back condition ULTA (nums[back] >= nums[i]).

PURANI SHEET KI KYUN LINE (code ke saath wali):
if(!dq.empty() && dq.front()==i-k) dq.pop_front();        // 1) front-index window se bahar (==i-k) -> hata (!dq.empty() pehle: khaali deque pe front() crash)
while(!dq.empty() && nums[dq.back()]<=nums[i]) dq.pop_back(); // 2) chhote bekaar -> pop (<= : barabar wala purana bhi bekaar, naya zyada der tikega)
if(i>=k-1) ans.push_back(nums[dq.front()]);               // 3) window poora -> front=max (k-1 = pehle poore window ka aakhri index)
```

```cpp
vector<int> maxSlidingWindow(vector<int> &nums, int k) {
    vector<int> ans;
    // ★ INDEX rakho, value nahi: kaun window se bahar gaya ye index se pata chalta
    // INDICES, values DECREASING -> front = max
    deque<int> dq;
    for (int i = 0; i < nums.size(); i++) {
        if (!dq.empty() && dq.front() == i - k)   // 1) window se bahar
            dq.pop_front();
        // ★ 2) peeche ke chhote bekaar, kabhi max nahi banenge. MIN chahiye to >=
        while (!dq.empty() && nums[dq.back()] <= nums[i])
            dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1)                     // 3) record. kram 1 -> 2 -> 3 hi
            ans.push_back(nums[dq.front()]);
    }
    return ans;
}
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     nums[] + k. har size-k window ka MAX nikaalo (left se right slide), vector lautao.
                                                 //    deque ki AKELI wajah yahi; baaki monotonic STACK
```


