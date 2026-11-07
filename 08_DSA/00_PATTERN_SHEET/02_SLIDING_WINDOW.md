## PATTERN 2 — SLIDING WINDOW


**Kab SW:** lagaatar subarray / substring + sabse lamba / sabse chhota / kitne.
**Kab nahi:** variable window jisme SUM pe shrink ho (`sum > k` pe chhota karo), aur numbers negative hon. Negative jodne pe sum ghat jaata hai, to "bada ho gaya, chhota karo" wala faisla toot jaata hai. Wahan prefix + map lagta hai. Fixed size window (max sum of k) negative ke saath bhi chalti hai.

```
 dhaancha (lamba wale):  j se jodo -> kharab ho to while me i se nikaalo -> ans lo -> j++
 sabse chhota wale:      j se jodo -> THEEK rahe tab tak while me ans lo + i se nikaalo -> j++
 fixed size k:           while ki jagah IF (ek juda, ek nikla)
 ★ EDGE (variable window): shrink me count 0 ho jaye to map se ERASE karo, warna map.size() jhooth bolega.
 ★ TRAP: max-sum-K me k = SIZE (IF); product<K me k = LIMIT (WHILE)
```

---

## LAMBA wale (kharab hote hi chhota karo, while ke BAAD ans = max)


### ▸ LC-3 LONGEST SUBSTRING NO-REPEAT  = variable window + freq-map
`"abcabcbb" -> 3`. Sabse lambi substring jisme koi char repeat na ho.

kyun: "abc" me teeno alag = 3; agla a aate hi repeat, isse lamba koi substring nahi

```
SAAR : longest substring jisme koi char REPEAT na ho. variable window + freq-map(mp).
INVALID = mp[s[j]]>1 (jo abhi add hua wahi repeat). empty-string edge -> INT_MIN -> 0.
```

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


### ▸ LC-424 CHAR REPLACEMENT (LONGEST)  = variable window + freq-map (k badal sakte)
`"AABABBA", k=1 -> 4`. Max k char badal ke sabse lambi window jisme sab same.

kyun: "AABA" (index 0..3) me ek B ko A banao -> "AAAA" = 4

```
SAAR : longest window jisme <=k char badal ke sabko SAME bana sako. variable window + freq-map.
KEY  : windowLen - maxFreq = kitne char badalne padenge. <=k -> valid.
CORE TRICK (non-obvious): maxFreq kabhi GHATAO mat (shrink pe recompute nahi). window best-length
   se chhota hota hi nahi (sirf badhta/slide) -> stale/purana maxFreq bhi answer kharab nahi karta.
```

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


### ▸ LC-1004 MAX CONSECUTIVE ONES III  = LC-3 ka bhai: variable window + zeros-count (k flips)
`[1,1,1,0,0,0,1,1,1,1,0], k=2 -> 6`. Max k zero ko 1 bana ke sabse lambi 1 ki line.

kyun: index 4..9 = 0,0,1,1,1,1: do zero flip -> 6 lagaatar 1

```
SAAR : at most k zeros ko FLIP -> longest all-1 window. sirf zeroCount track (freq-map nahi).
   LC-3 se farak: map ki jagah ek int (zeroCount), aur shrink "zero > k" pe.
CORE LEARNING: ans UNCONDITIONAL update -> if(zeroCount==k) MAT lagao.
   kyun: upar while(zeroCount>k) shrink ke BAAD window HAMESHA valid (<=k) -> yahan pahunchte hi valid -> condition faltu.
   (galti thi: if(zeroCount==k) -> sirf exactly-k ginta -> k=0 / k>zeros case toot jaate.)
shrink pe nums[i]==0 hi zeroCount-- (1 nikalne pe count na badle).
```

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


### ▸ FRUIT INTO BASKETS  = longest subarray with AT MOST 2 DISTINCT
`[1,2,3,2,2] -> 4`. 2 basket, har basket me ek type. Lagaatar pedon ki sabse lambi line.

kyun: [2,3,2,2] me sirf 2 type (2,3) = 4; aage 1 jodo to 3 type ho jaate

```
SAAR : "longest subarray with AT MOST 2 DISTINCT" (fruit/basket sirf kahani). SW skeleton + map<type,count>.
while (mp.size() > 2) KYUN while: ek type poora khatam hone tak kai element hatane padte.
   IF bhi chalta (sirf LONGEST): ek aaya ek gaya, size best se bada nahi -> len sahi. COUNT / SHORTEST me sirf while.
CRUX = erase-on-0: count 0 hone pe key map me PADI reh jaati -> mp.size() galat distinct dega -> condition toot. isliye ERASE.
```

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


### ▸ LONGEST AT-MOST-K DISTINCT  = FRUIT ka GENERAL version (2 ki jagah k)
`"eceba", k=2 -> 3`. Sabse lambi substring jisme zyada se zyada k alag char.

kyun: "ece" me sirf e,c (2 alag) = 3; "eceb" me 3 alag ho jaate

```
wahi variable window + map<char,count>, distinct (mp.size()) > k ho -> left se shrink (erase-on-0). ans = max length.
farak: fruit me "2" hardcoded tha, yahan wahi jagah "k" (mp.size() > k).
```

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


### ▸ LC-1208 EQUAL SUBSTRING WITHIN BUDGET  = longest-subarray-sum <= k (string ke bhes me)
`s="abcd", t="bcdf", maxCost=3 -> 3`. s ke char ko t me badalne ki cost `|s[i]-t[i]|`. Kul cost budget ke andar, sabse lambi window.

kyun: cost = [1,1,1,2]; pehle 3 ka 1+1+1 = 3 <= 3; chaaron ka 5 > 3 · e.g. s="abcd", t="cdef", maxCost=3 -> 1

```
SAAR : s ko t banane me har char ka cost = |s[i]-t[i]|. longest window jiska cost-SUM <= maxCost.
KEY  : cost-array bana ke "longest subarray with sum <= k" -- bas string ke bhes me chhupa.
   ONES III jaisa, ek int (cost) track, koi map nahi. cost hamesha 0 ya zyada, isliye negative wala dar nahi.
```

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
`target=7, [2,3,1,2,4,3] -> 2` ([4,3]). Positive numbers, sum >= target wala sabse chhota subarray.

kyun: [4,3] = 7 >= 7, length 2; akela koi number 7 tak nahi pahunchta

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
`[2,1,5,1,3,2], k=3 -> 9`. Har size-k window ka sum, sabse bada.

kyun: window sums 8, 7, 9, 6 -> max 9 (5+1+3)

```
SAAR : k-size ke har window ka sum -> unme se MAX. fixed window (size hamesha k).
KEY  : variable nahi -- size fix k, isliye IF (ek hi remove).
WHY IF-not-WHILE : fixed size -> har step 1 add + 1 remove -> slide. variable hota to WHILE (kai remove).
```

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


### ▸ LC-1423 MAX POINTS FROM CARDS  = MAX-SUM-SIZE-K (01) + COMPLEMENT-twist
`[1,2,3,4,5,6,1], k=3 -> 12` (peeche se 5+6+1). Dono kinaron se kul k card lo, sum sabse bada.

kyun: back se 3 = 1+6+5 = 12; front 3 = 6, mix (1+2+1 = 4, 1+1+6 = 8) kam

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



### ▸ LC-219 CONTAINS DUPLICATE II  
`[1,2,3,1], k=3 -> true`. Do barabar number jinka index ka farak <= k.

kyun: index 0 aur 3 dono pe 1, farak 3 <= 3
  k=0: |i-j|<=0 matlab same index, jodi ban hi nahi sakti

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


## KITNE (ginti, length nahi)

### ▸ SUBARRAY PRODUCT < K  
`[10,5,2,6], k=100 -> 8`. Positive numbers, kitne subarray jinka guna k se kam.

kyun: 4 akele + [10,5]=50, [5,2]=10, [2,6]=12, [5,2,6]=60 = 8; [10,5,2]=100 kam nahi

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


## NEED-MAP (t ke saare char chahiye)

### ▸ LC-76 MIN WINDOW SUBSTRING  
`s="ADOBECODEBANC", t="ABC" -> "BANC"`. s ki sabse chhoti window jisme t ke saare akshar utni hi baar hon jitni baar t me hain (count ke saath; t="AA" ho to do A chahiye). Na mile to "".

kyun: "ADOBEC" (6) bhi A,B,C rakhta, par "BANC" (4) sabse chhota

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


### ▸ LC-1358 SUBSTRINGS WITH ALL THREE (a, b, c)  
`"abcabc" -> 10`. Kitne substring jinme a, b, c teeno.

e.g. "aaacb" -> 3 · kyun: "aaacb", "aacb", "acb" me teeno; "cb" ya usse chhote me a nahi = 3

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


## ANAGRAM (fixed size = p ki length)

### ▸ LC-438 FIND ALL ANAGRAMS

```
SAWAAL:
s aur p (sirf a-z). s me jahan-jahan p ka anagram lagaatar baitha hai
(wahi akshar, kram koi bhi), un sab ki START index lautao.
  e.g. s="cbaebabacd", p="abc" -> [0,6]
  kyun: s[0..2]="cba", s[6..8]="bac" -> dono me a, b, c ek-ek

SOCH:
kya bol raha: window ka size FIX (= p.size()), har window me akshar ki ginti p jaisi?
kahan dekha: LC-76 min window. wahan bhi t ki ginti rakhi, j aaya to ginti badli, i gaya to wapas.
  farak: wahan window chhoti-badi, yahan size fixed. wahan ek map, yahan need (p) + have (window).
to: need me p ki ginti EK baar -> j aaya have +1 -> size p jitna -> have == need ? index
    -> i gaya have -1, i++
★ PEHLI GALTI: har window ka substr bana ke isAnagram -> LeetCode pe TLE (34/66).
  ginti har window pe dobara mat banao, aate-jaate jodo-ghatao.
```

```cpp
vector<int> findAnagrams(string s, string p) {
    vector<int> ans;
    int i = 0, j = 0;
    vector<int> need(26, 0);                // p ki ginti, sirf EK baar
    vector<int> have(26, 0);                // window ki ginti, jodte-ghatate
    for (int i = 0; i < p.size(); i++)
        need[p[i] - 'a']++;
    while (j < s.size()) {
        have[s[j] - 'a']++;                 // ★ j aaya -> +1
        if (j - i + 1 == p.size()) {
            if (have == need)               // 26 khaane barabar = anagram
                ans.push_back(i);
            have[s[i] - 'a']--;             // ★ i gaya -> -1, PHIR i++
            i++;
        }
        j++;
    }
    return ans;
}
```


### ▸ LC-567 PERMUTATION IN STRING

```
SAWAAL:
s1, s2. kya s2 me kahin lagaatar s1 ka anagram (permutation) baitha hai? true / false.
  e.g. s1="ab", s2="eidbaooo" -> true
  kyun: s2[3..4]="ba" -> a, b ek-ek

SOCH:
upar wala 438 hi hai: s2 pe window s1.size() ki, need = s1 ki ginti.
farak: pehla match mila -> turant true, poora ghoom liya -> false.
★ dhyaan: s1 ko s2 ke ANDAR dhoondhna hai, ulta nahi (window s2 pe chalti).
```

```cpp
bool checkInclusion(string s1, string s2) {
    int i = 0, j = 0;
    vector<int> need(26, 0);                // s1 ki ginti, EK baar
    vector<int> have(26, 0);
    for (int i = 0; i < s1.size(); i++)
        need[s1[i] - 'a']++;
    while (j < s2.size()) {
        have[s2[j] - 'a']++;                // ★ j aaya -> +1
        if (j - i + 1 == s1.size()) {
            if (have == need)
                return true;                // pehla match = true
            have[s2[i] - 'a']--;            // ★ i gaya -> -1, PHIR i++
            i++;
        }
        j++;
    }
    return false;
}
```

---


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


