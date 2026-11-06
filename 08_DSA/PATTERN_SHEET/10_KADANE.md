## PATTERN 10 — KADANE (running value + max update)

```
 BROAD IDEA: ek value AAGE carry karo (min/sum), har step MAX update. single pass, O(1) space.
 ★ EDGE (circular): ans = max(normal kadane, total - minSubarray). SAB NEGATIVE -> total-min = 0 aata,
         tab normal kadane hi sahi (alag se dekho).
 ★ MILTE-JULTE (max sum vs count sum == K): upar NAAM-SE-CONFUSE C dekh.

 KAB: subarray ka MAX sum / product · stock ka ek baar kharido-becho
```

### ▸ BUY/SELL STOCK (LC-121)

```
prices[i] din i ka share ka daam hai. Ek din kharido aur uske baad ke kisi din becho. Sabse zyada kitna munafa ho sakta hai, wo lautao. Munafa na ho to 0.
  e.g. [7,1,5,3,6,4] -> 5
  kyun: din 1 pe 1 me kharida, din 4 pe 6 me becha = 5; pehle-chhota baad-bada ka isse bada farak nahi
```

```
TASVEER (purani sheet se, same to same):

TRICK: aaj becho = aaj ka daam MINUS ab-tak-ka-sasta. bas 2 cheez track: mini + maxProfit.
  mini = min(mini, prices[i]);   maxProfit = max(maxProfit, prices[i] - mini); // init mini=INT_MAX; mini PEHLE update -> diff kabhi minus nahi
(sell buy-ke-BAAD auto -- mini hamesha current se pehle ka.)
BRUTE->OPT (aise pahunche): brute = saare pair (i<j): prices[j]-prices[i], max. O(n^2).
  REDUNDANCY = har sell-din pe peeche ka "sabse sasta buy" baar-baar scan.
  us min ko ek VARIABLE me CACHE -> inner loop gayab -> O(n).  (redundancy->cache = running-min)
```

```cpp
int maxProfit(vector<int> &prices) {        // aaj becho = aaj ka daam MINUS ab tak ka sasta
    int ans = INT_MIN;
    int minSoFar = INT_MAX;                 // INT_MAX: pehla din hi sasta ban jaaye
    for (int i = 0; i < prices.size(); i++) {
        minSoFar = min(minSoFar, prices[i]);        // ★ min PEHLE: diff minus nahi, sell buy ke BAAD
        ans = max(ans, prices[i] - minSoFar);
    }
    return ans < 0 ? 0 : ans;
}
// brute = saare pair (i<j), O(n^2). REDUNDANCY = har sell-din pe peeche ka sasta baar-baar
//   scan -> us min ko VARIABLE me CACHE -> O(n) (redundancy -> cache = running min)
```

### ▸ MAX SUBARRAY SUM (LC-53)

```
Ek array diya hai. Lagaatar numbers ka aisa subarray (kam se kam ek number) chuno jiska jod sabse bada ho, aur wo jod lautao.
  e.g. [-2,1,-3,4,-1,2,1,-5,4] -> 6
  kyun: [4,-1,2,1] = 6; pehle ke -2,1,-3 ka jod negative, unhe saath lena sum ghatata
```

```
TASVEER (purani sheet se, same to same):

TRICK: pichhla running sum NEGATIVE = aage bojh -> PHENK do (sum=0), fresh. bas yahi.
  sum += nums[i];   max_sum = max(max_sum, sum);   if(sum < 0) sum = 0;   // max PEHLE, reset BAAD: warna all-negative [-1] pe 0 aata
TRAP: max_sum update RESET se PEHLE + max_sum=INT_MIN start (warna all-negative [-1] miss). // sum=0 se: abhi tak kuch joda nahi
```

```cpp
int maxSubArray(vector<int> &nums) {        // running sum NEGATIVE = bojh -> sum = 0
    int maxSum = INT_MIN;                   // ★ INT_MIN: all-negative [-1] bhi pakde
    int sum = 0;                            // 0 se: abhi tak kuch joda nahi
    for (auto &it : nums) {
        sum += it;
        maxSum = max(maxSum, sum);          // ★ max PEHLE, reset BAAD: warna all-negative [-1] pe 0 aata
        if (sum < 0)
            sum = 0;
    }
    return maxSum;
}
```

#### ── FAMILY: flip ho to MIN bhi + 3 candidate + FREEZE (temp) · 53 sum · 152 product · 1749 abs ──

### ▸ MAX PRODUCT SUBARRAY (LC-152)

```
Ek array diya hai. Lagaatar numbers ka aisa subarray chuno jiska guna sabse bada ho, aur wo guna lautao. Minus numbers bhi ho sakte hain.
  e.g. [-2,3,-4] -> 24
  kyun: -2 x 3 = -6 (sabse chhota), phir x -4 = 24; neg x neg = bada, isliye min bhi saath rakhna
```

```
TASVEER (purani sheet se, same to same):

TRICK: neg*neg = BADA -> sirf max se nahi banega, MIN bhi track karo (chhota kal sabse bada ban sakta).
  temp    = max({ num, num*max, num*min });     // 3 candidate ka max (temp me rakho: max pehle badla to min ko naya max milta)
  min     = min({ num, num*max, num*min });     // FREEZE: min purane max/min se
  max     = temp;  ans = max(ans, max);         // max min-ke-BAAD, ans me purana bhi
★ SHURUAAT (24-Sep theek kiya):  max = 1, min = 1, ans = INT_MIN,  loop i = 0 se.   // ans INT_MIN: 0 se [-2] pe 0 aata, sahi -2
  pehle max=min=nums[0], ans=0, loop 1 se tha -> nums[0] AKELA kabhi ans me nahi gaya
  -> [4,-3] pe 0 aata (sahi 4). niyam: loop 1 se ho to ans bhi nums[0] se; loop 0 se to 1 / INT_MIN se.
```

```cpp
int maxProduct(vector<int> &nums) {         // neg*neg = BADA -> MIN bhi track (chhota kal sabse bada)
    int mini = 1;                           // SHURUAAT: mini = maxi = 1, ans = INT_MIN, loop 0 se
    int maxi = 1;                           //   (loop 1 se ho to ans bhi nums[0] se; warna [4,-3] pe 0)
    int ans = INT_MIN;                      // INT_MIN: 0 se [-2] pe 0 aata, sahi -2
    for (int i = 0; i < nums.size(); i++) {
        int temp = max({nums[i], mini * nums[i], maxi * nums[i]});   // ★ 3 candidate, temp me:
        mini = min({nums[i], mini * nums[i], maxi * nums[i]});       // FREEZE: min purane max se
        maxi = temp;                        // ★ max min ke BAAD
        ans = max(ans, maxi);
    }
    return ans;
}
```

### ▸ MAX ABSOLUTE SUM (LC-1749)

```
Ek array diya hai. Kisi bhi lagaatar subarray ka jod lo aur uska absolute value (minus hata ke) dekho. Sabse bada absolute jod lautao.
  e.g. [2,-5,1,-4,3,-2] -> 8
  kyun: [-5,1,-4] = -8, |-8| = 8; sabse bada positive tukda sirf 3, to bada-negative jeeta
```

```
TASVEER (purani sheet se, same to same):

= MAX PRODUCT SUBARRAY (152) ka HUBAHU same skeleton (temp/min/max freeze, upar dekh), sirf 2 TWEAK:
   (1) GUNA (*) -> PLUS (+)                    (num*max -> num+max, num*min -> num+min)
   (2) ans me ABS dono -> ans = max({ ans, |max|, |min| })   (answer minSum se bhi aa sakta -> isliye abs)
★ SHURUAAT (24-Sep theek kiya): max = 0, min = 0, ans = 0, loop i = 0 se.   n==1 -> return abs(nums[0]).   // 0 se theek: + me 0 kuch nahi jodta, abs kabhi minus nahi
  pehle max=min=nums[0], ans=0, loop 1 se -> nums[0] akela chhoot jaata ([9,-6] pe 6, sahi 9);
  aur n==1 pe abs nahi tha ([-7] pe -7, sahi 7).
dry-run [2,-5,1,-4,3,-2] -> 8: subarray [-5,1,-4]=-8 minSum me -> |-8|=8.

FAMILY: 53=sum(1 value) · 152=product(flip -> max+min) · 1749=abs-sum(max+min).
MECHANIC yaad rakh: "flip ho to MIN bhi track + 3 candidate + old FREEZE (temp)".
```

```cpp
int maxAbsoluteSum(vector<int> &nums) {     // = MAX PRODUCT skeleton, 2 tweak: * -> +, ans me ABS
    int n = nums.size();
    if (n == 1)
        return abs(nums[0]);                // n==1 pe abs ([-7] -> 7)
    int mini = 0;                           // SHURUAAT: 0, 0, 0, loop 0 se: + me 0 kuch nahi jodta, abs
    int maxi = 0;                           //   kabhi minus nahi (loop 1 se tha to [9,-6] pe 6, sahi 9)
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int temp = max({nums[i], mini + nums[i], maxi + nums[i]});
        mini = min({nums[i], mini + nums[i], maxi + nums[i]});       // temp FREEZE, product jaisa
        maxi = temp;
        ans = max(ans, max(abs(mini), abs(maxi)));   // ★ answer minSum se bhi aa sakta -> abs dono
    }
    return ans;                             // [2,-5,1,-4,3,-2] -> [-5,1,-4] = -8 -> 8
}

── (STANDALONE — apni alag trick) ──
```

### ▸ MAX SUM CIRCULAR SUBARRAY (LC-918)  = TRICK: total − MIN-subarray

```
Ek gol (circular) array diya hai, matlab end ke baad shuru wala number aata hai. Lagaatar numbers ka sabse bada jod lautao (gol ghoom ke bhi le sakte ho, khaali nahi chalega).
  e.g. [5,-3,5] -> 10
  kyun: wrap: last 5 + first 5 = 10; total 7 - min-subarray (-3) = 10

circular (end -> start wrap), max-sum NON-EMPTY subarray. TRICK: PATA hona chahiye
A) no-wrap = normal Kadane  B) wrap = poora array MINUS beech ka chunk = TOTAL - min
```

```
TASVEER (purani sheet se, same to same):

★ ye TRICK-problem hai -- interview me on-the-spot derive lagbhag impossible; SEEKHNA padta.

IDEA:
   2 CASE (bada lo):
      A) NO-wrap  = normal Kadane (max)
      B) WRAP     = end+start juda, BEECH ka chunk chhoda.
   KEY INSIGHT: wrap = poora array MINUS beech-ka-chunk. max-wrap ke liye SABSE-BURA (min) chunk hatao ->
      wrap-max = TOTAL − (minimum-subarray-sum).   [min-subarray = Kadane ka MIN-version]

TEMPLATE:
    maxK = maxKadane(nums);   minK = minKadane(nums);   total = sum(nums); // maxK init: cur=maxK=nums[0], loop 1 se (ya cur=0, maxK=INT_MIN)
    circular = total − minK;   // wrap = poora array minus sabse bura (min) beech ka chunk

    ★ minK Kadane-MIN se:  cur = min(nums[i], cur+nums[i]);   minK = min(minK, cur).  // init cur=minK=nums[0], loop 1 se (ya cur=0, minK=INT_MAX)
       (prefix-sum se NAHI -- Arpan-10-Sep pakda.)

    ★ RETURN (edge yahin handle -- Arpan-way, saaf):
         if (maxK > 0)  return max(circular, maxK);   // normal: dono me se bada
         return maxK;                                 // maxK<=0 = SAB non-positive (all-neg) -> sirf maxK

DRY-RUN [5,-3,5]:  maxK=7 (5,-3,5 contiguous -- -3 skip nahi hota) · minK=-3 · total=7
   -> wrap = 7−(−3) = 10  ->  ans = max(7,10) = 10.   (10 sirf WRAP se, seedhe Kadane se nahi.)

EDGE KYUN: all-negative me min-subarray = poora array -> total−minK = 0 (EMPTY subarray) -> galat.
   isliye maxK>0 check: agar koi positive hi nahi -> wrap bekaar -> seedha maxK (empty allowed nahi).

CONNECT: [[house-robber-II]] jaisa "circular twist" par MECHANISM alag
         (HR-II = exclude-first/last · ye = total − minKadane).
```

```cpp
int maxKadaneK(vector<int> &nums) {
    int sum = nums[0];                      // init sum = maxSum = nums[0], loop 1 se (ya 0 + INT_MIN)
    int maxSum = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        sum = max(sum + nums[i], nums[i]);
        maxSum = max(maxSum, sum);
    }
    return maxSum;
}

int minKadaneK(vector<int> &nums) {         // Kadane ka MIN version (prefix-sum se NAHI)
    int sum = nums[0];                      // init nums[0], loop 1 se (ya 0 + INT_MAX)
    int minSum = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        sum = min(sum + nums[i], nums[i]);
        minSum = min(minSum, sum);
    }
    return minSum;
}

int maxSubarraySumCircular(vector<int> &nums) {
    int totalSum = 0;
    for (int i = 0; i < nums.size(); i++)
        totalSum += nums[i];
    int maxSum = maxKadaneK(nums);
    int minSum = minKadaneK(nums);
    int circularSum = totalSum - minSum;    // ★ wrap = poora - sabse bura (min) beech ka chunk
    if (maxSum > 0)
        return max(circularSum, maxSum);    // normal: dono me bada. [5,-3,5]: 7, 7-(-3)=10 -> 10
    return maxSum;                          // ★ sab non-positive: total-min = 0 = EMPTY, galat
}
// house robber II jaisa "circular twist", par mechanism alag (HR-II = pehla / aakhri chhodo)
```

### ▸ 2 MAX-SUM NON-OVERLAPPING SUBARRAYS (Google)  = bestLeft + bestRight + WALL

```
Ek array diya hai. Do subarray chuno jo lagaatar hon, khaali na hon, aur ek-doosre ko na chhuein (non-overlapping). Dono ke jod ko milake sabse bada total lautao.
  e.g. [1,2,-7,8,6,-4] -> 17
  kyun: [1,2] = 3 aur [8,6] = 14, beech ka -7 chhoda; 3 + 14 = 17

har WALL i pe: [0..i] ka best + [i+1..n-1] ka best -> sabka MAX
= KADANE (per index) + PREFIX / SUFFIX max
[2,1,-99,3,3] = 9: wall index 1 -> left [2,1] = 3 + right [3,3] = 6
```

```
TASVEER (purani sheet se, same to same):

ESSENCE: har wall pe -> [0..i] ka best + [i+1..n-1] ka best -> in sab ka MAX. bas yehi.

IDEA:
   CONNECT: ye KADANE + PREFIX/SUFFIX ka COMBO hai (do pattern jud rahe).
      05 (kadane per-index) -> maxEndingAt array -> uska PREFIX-max (aage) = bestLeft,
      SUFFIX-max (peeche) = bestRight -> phir WALL se jodo.  (var = code jaisa: ans/temp)
      KYUN kadane+prefix: har index tak ka running SUM = prefix-sum; uska MAX (neg aaye to reset) = kadane.
      eg [2,1,-99,3,3]=9 -> wall index-1 pe: LEFT best=3 ([2,1]) + RIGHT best=6 ([3,3]) = 9.

TEMPLATE:
    maxEndingAtForLeft   (base ans[0]=nums[0];        loop i=1..n-1  AAGE)   // index 0 pe khatam = sirf nums[0], isliye loop 1 se
        ans[i] = max( nums[i] , ans[i-1] + nums[i] )                   // akela naya shuru karo ya pichhle me judo, jo bada
           |__ ye jo array RETURN hua  -->  neeche bestLeftPrefix me 'temp' ban jaata

    bestLeftPrefix       (temp = UPAR wale ka RETURN;  base ans[0]=temp[0];   loop i=1..n-1  AAGE)   // 0 tak ka best = khud temp[0], isliye loop 1 se
        ans[i] = max( ans[i-1] , temp[i] )                            // 0..i ka best = pichhla best ya temp[i], jo bada

    maxEndingAtForRight  (base ans[n-1]=nums[n-1];    loop i=n-2..0  PEECHE)   // last index pe khatam = sirf wahi; right part peeche se banta
        ans[i] = max( nums[i] , ans[i+1] + nums[i] )                  // mirror: akela shuru ya agle (i+1) wale me judo, jo bada
           |__ ye jo array RETURN hua  -->  neeche bestRightPrefix me 'temp' ban jaata

    bestRightPrefix      (temp = UPAR wale ka RETURN;  base ans[n-1]=temp[n-1];  loop i=n-2..0  PEECHE)   // last se peeche bharo: ans[i+1] pehle se ready chahiye
        ans[i] = max( ans[i+1] , temp[i] )                           // i..n-1 ka best = agla best (i+1) ya temp[i], jo bada

    maxTwoNonOverlap     (bestLeft=bestLeftPrefix, bestRight=bestRightPrefix;  ans=INT_MIN)   // INT_MIN: sab minus ho to jod bhi minus, 0 se galat
        for i = 0..n-2 :  ans = max( ans , bestLeft[i] + bestRight[i+1] )    // WALL, i+1 = non-overlap (loop n-2 tak: i+1 array ke bahar na jaaye)

DRY-RUN:
   VISUAL [1,2,-7,8,6,-4]:
     val       : [ 1    2   -7    8    6   -4 ]
     maxEndAt  : [ 1    3   -4    8   14   10 ]   (05)
     bestLeft  : [ 1    3    3    8   14   14 ]   (06)
     bestRight : [ 14   14   14   14   6   -4 ]   (06 ka mirror)
     i=1: bestLeft[1]=3 + bestRight[2]=14 = 17

EDGE:
   i+1 KYUN (non-overlap ki JAAN): right ko i se lo (bestRight[i]) -> shared index DONO subarray me -> DO BAAR count -> OVERLAP.
     e.g. bestLeft[3]=8 + bestRight[3]=14 = 22, index 3 DONO me = jhootha. i+1 (ya i-1, loop i=1->n-1) se dono ALAG.

     val       : [  1   2  -7   8   6  -4 ]
     bestLeft  : [  1   3   3  (8) 14  14 ]   (0..i)          <- aage
     bestRight : [ 14  14  14 (14) 6  -4 ]   (i+1..n-1)      <- peeche
     overlap dekh: index 3 pe bestLeft ka 8 AUR bestRight ka 14 -- dono me index 3 COMMON = do baar count (jhootha).
     isliye: left = [0..i], right = [i+1..n-1] -> ek AAGE shift -> index 3 dono me nahi. (ya left [0..i-1], right [i..n-1])

   loop i<n-1: aakhri split (right = akela last element) MISS mat karo. [-5,10,10] -> 20.

PEHLI SHEET (Aug) se bhi, same to same:

  maxEndingAtForLeft   (base ans[0]=nums[0];        loop i=1..n-1  AAGE)
      ans[i] = max( nums[i] , ans[i-1] + nums[i] )                   // Kadane per-index (=05, prefix-sum+reset)
         |__ ye jo array RETURN hua  -->  neeche bestLeftPrefix me 'temp' ban jaata

  bestLeftPrefix       (temp = UPAR wale ka RETURN;  base ans[0]=temp[0];   loop i=1..n-1  AAGE)
      ans[i] = max( ans[i-1] , temp[i] )                            // PREFIX-max (=06)

  maxEndingAtForRight  (base ans[n-1]=nums[n-1];    loop i=n-2..0  PEECHE)
      ans[i] = max( nums[i] , ans[i+1] + nums[i] )                  // 05 ka MIRROR
         |__ ye jo array RETURN hua  -->  neeche bestRightPrefix me 'temp' ban jaata

  bestRightPrefix      (temp = UPAR wale ka RETURN;  base ans[n-1]=temp[n-1];  loop i=n-2..0  PEECHE)
      ans[i] = max( ans[i+1] , temp[i] )                           // SUFFIX-max (=06 ulta)
```

```cpp
vector<int> maxEndingAtForLeft(vector<int> &nums) {      // STEP 1: kadane per index (aage)
    int n = nums.size();
    vector<int> ans(n, 0);
    ans[0] = nums[0];                       // index 0 pe khatam = sirf nums[0], isliye loop 1 se
    for (int i = 1; i < n; i++)
        ans[i] = max(ans[i - 1] + nums[i], nums[i]);   // pichhle me judo ya akela naya shuru, jo bada
    return ans;
}

vector<int> bestLeftPrefix(vector<int> &nums) {          // STEP 2: STEP 1 ka running max (aage)
    int n = nums.size();
    vector<int> temp = maxEndingAtForLeft(nums);
    vector<int> ans(n, 0);
    ans[0] = temp[0];                       // 0 tak ka best = khud temp[0]
    for (int i = 1; i < n; i++)
        ans[i] = max(ans[i - 1], temp[i]);  // 0..i ka best = pichhla best ya temp[i]
    return ans;
}

vector<int> maxEndingAtForRight(vector<int> &nums) {     // STEP 3: STEP 1 ka mirror (peeche)
    int n = nums.size();
    vector<int> ans(n, 0);
    ans[n - 1] = nums[n - 1];               // last pe khatam = sirf wahi; right part peeche se banta
    for (int i = n - 2; i >= 0; i--)
        ans[i] = max(ans[i + 1] + nums[i], nums[i]);   // agle (i+1) me judo ya akela shuru
    return ans;
}

vector<int> bestRightPrefix(vector<int> &nums) {         // STEP 4: STEP 3 ka running max (peeche)
    int n = nums.size();
    vector<int> temp = maxEndingAtForRight(nums);
    vector<int> ans(n, 0);
    ans[n - 1] = temp[n - 1];
    for (int i = n - 2; i >= 0; i--)        // peeche bharo: ans[i+1] pehle se ready chahiye
        ans[i] = max(ans[i + 1], temp[i]);  // i..n-1 ka best = agla best ya temp[i]
    return ans;
}

int maxTwoNonOverlap(vector<int> &nums) {   // STEP 5: WALL
    int n = nums.size();
    vector<int> bestForleft = bestLeftPrefix(nums);
    vector<int> bestForRight = bestRightPrefix(nums);
    int ans = INT_MIN;                      // INT_MIN: sab minus ho to jod bhi minus, 0 se galat
    for (int i = 0; i < n - 1; i++)         // ★ n-2 tak: i+1 bahar na jaaye; aakhri split (right = akela
        ans = max(ans, bestForleft[i] + bestForRight[i + 1]);   // ★   last) bhi aaye: [-5,10,10] -> 20
    return ans;                             // i+1 = NON-OVERLAP: bestRight[i] liya to index i dono me =
}                                           //   do baar gina ([..,8,..] + [14..] = 22 jhootha)
// [1,2,-7,8,6,-4]: maxEndAt [1,3,-4,8,14,10] · bestLeft [1,3,3,8,14,14]
//   bestRight [14,14,14,14,6,-4] · i=1: 3 + 14 = 17
```

