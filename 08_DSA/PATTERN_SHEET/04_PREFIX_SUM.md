## PATTERN 4 — PREFIX SUM

```
 BROAD IDEA: running sum pehle se rakho -> range/left-right sum O(1) (baar-baar loop nahi).
 ★ EDGE (prefix + hashmap, sum == K): map[0] = 1 se SHURU karo. [3], K=3: bina iske poora-array wala case chhoot jaata.
 ★ MILTE-JULTE (max sum vs count sum == K): upar NAAM-SE-CONFUSE C dekh.
 ★ EDGE (C++ %): minus % minus deta (-2 % 5 = -2) -> ((x % m) + m) % m  (divisible-by-k remainder).

```

#### ── FAMILY: "assign PEHLE, update BAAD" (pehle current index ka result rakho, PHIR running value badlo) ──

```
 KAB: range sum baar-baar · "kitne subarray ka sum = K" (negative bhi) · left sum = right sum
 ★ DHYAAN: mp[0] = 1 bhoolna nahi ([3], K=3)
```

### ▸ PIVOT INDEX

```
Ek array diya hai. Wo pehla index lautao jahan uske baayi taraf ke sabhi numbers ka jod, daayi taraf ke sabhi numbers ke jod ke barabar ho.
Us index ka apna number kisi bhi taraf nahi gina jaata. Aisa index na mile to -1.
  e.g. [1,7,3,6,5,6] -> 3
  kyun: index 3 (6): left 1+7+3 = 11, right 5+6 = 11
```

```cpp
int pivotIndex(vector<int> &nums) {         // index jahan left sum == right sum (khud kisi taraf nahi)
    int totalSum = 0;
    int leftSum = 0;
    for (int &it : nums)
        totalSum += it;                     // pehle poora total
    for (int i = 0; i < nums.size(); i++) {
        int rightSum = totalSum - leftSum - nums[i];   // ★ right = total - left - khud
        if (leftSum == rightSum)
            return i;                       // pehla aisa index
        leftSum += nums[i];                 // ★ leftSum BAAD me: check ke baad hi khud ko left me jodo
    }
    return -1;
}
```

### ▸ PRODUCT EXCEPT SELF (LC-238)

```
Ek array diya hai. Har index i ke liye ans[i] = baaki sabhi numbers ka guna (nums[i] ko chhod ke).
Bhaag (division) nahi lagana, O(n) me karna hai.
  e.g. [1,2,3,4] -> [24,12,8,6]
  kyun: ans[1] = left 1 x right 3x4=12 -> 12; ans[0] = 2x3x4 = 24

ans[i] = (i se PEHLE sabka product) x (i ke BAAD sabka product). division nahi, O(n)
O(1) extra (follow-up): left ko seedha ans me likho, right-pass me ans[i] *= prod (alag right[] nahi)
```

```cpp
vector<int> productExceptSelf(vector<int> &nums) {
    int n = nums.size();
    vector<int> left(n, 1);
    vector<int> right(n, 1);
    vector<int> ans(n, 1);
    int prod = 1;                           // prod=1 se: index 0 ke left me koi nahi
    for (int i = 0; i < n; i++) {
        left[i] = prod;                     // ★ ASSIGN PEHLE
        prod *= nums[i];                    // ★ UPDATE BAAD: warna khud ka element bhi product me
    }
    prod = 1;                               // reset: left-pass ka bacha product saath mat lao
    for (int i = n - 1; i >= 0; i--) {      // peeche se: n-1 se 0 tak
        right[i] = prod;                    // yahan bhi assign PEHLE
        prod *= nums[i];
    }
    for (int i = 0; i < n; i++)
        ans[i] = left[i] * right[i];        // combine
    return ans;
}
```

── (STANDALONE — apni alag trick) ──

### ▸ HIGHEST ALTITUDE

```
Ek cyclist 0 altitude se shuru karta hai. gain[i] batata hai ki agle point tak altitude kitni badhi ya ghati. Raste me sabse unchi altitude kitni thi, wo lautao.
  e.g. [-5,1,5,0,-7] -> 1
  kyun: altitude 0, -5, -4, 1, 1, -6 -> max 1
```

```cpp
int largestAltitude(vector<int> &gain) {    // running sum chalao, max track (simple prefix)
    int sum = 0;
    int maxi = 0;                           // ★ sum=0, maxi=0: shuru ki altitude 0 bhi gino (sab -ve -> 0)
    for (int i = 0; i < gain.size(); i++) {
        sum += gain[i];
        maxi = max(maxi, sum);
    }
    return maxi;
}
```

### ▸ SUBARRAY SUMS DIVISIBLE BY K (LC-974)  = SUBARRAY-SUM-K + REMAINDER-key

```
Ek array (plus aur minus dono) aur ek number k diya hai. Kitne lagaatar subarray hain jinka jod k se poora kat jaaye (sum % k == 0), wo ginti lautao.
  e.g. [1,2,3], k=3 -> 3
  kyun: [3]=3, [1,2]=3, [1,2,3]=6 -> teeno 3 se divide; [1],[2],[2,3]=5 nahi

brute: har subarray sum baar-baar -> PREFIX -> do prefix ka remainder same = beech wala %k==0
PREFIX kyun, single A[i] nahi: subarray CONTIGUOUS range -> do prefix ka farak = wahi subarray
```

```cpp
int subarraysDivByK(vector<int> &nums, int k) {   // count subarrays jinka sum % k == 0
    unordered_map<int, int> mp;
    int sum = 0, count = 0;
    mp[0] = 1;                              // ★ SEED (khaali prefix): shuru se hi divisible wala bhi gino
    for (auto &it : nums) {
        sum += it;                          // pehle sum += , PHIR rem nikaalo
        int rem = ((sum) % k + k) % k;      // ★ +k phir %k: negative sum ka % negative aata
        count += mp[rem];                   // = subarray-sum-k, bas KEY = remainder (sum ki jagah)
        mp[rem]++;
    }
    return count;
}
```

### ▸ COUNT SUBARRAYS WITH XOR = K  = SUBARRAY-SUM-K, bas + ki jagah ^

```
Ek array aur ek number k diya hai. Kitne lagaatar subarray hain jinke saare numbers ka XOR theek k ke barabar ho, wo ginti lautao.
  e.g. [4,2,2,6,4], k=6 -> 4
  kyun: [4,2]=6, [2,2,6]=6, [6]=6, [4,2,2,6,4]=6 (4^4 aur 2^2 kat gaye) = 4

[4,2,2,6,4], k=6 -> 4.   CHAABI: x ^ x = 0 (same number khud se XOR = gayab)
(shuru se j tak XOR) ^ (shuru se i tak XOR) = beech ka XOR (common hissa kat gaya)
beech wala = k chahiye -> pichhla XOR = abhi ka XOR ^ k -> map me dekho kitni baar aaya
```

```
↳ SAME: SUBARRAY SUM = K ka hubahu copy, sirf sign badla:
   sum wale me:  complement = curSum - k      (a+b=c -> c-b=a)
   XOR wale me:  complement = curXor ^ k      (a^b=c -> c^b=a)   [4^2=6 to 4^6=2]
```

```cpp
int subarrayXorK(vector<int> &nums, int k) {
    unordered_map<int, int> mp;
    mp[0] = 1;                              // shuru se hi XOR == k wala subarray bhi gino
    int xorr = 0, count = 0;
    for (auto &it : nums) {
        xorr ^= it;                         // abhi tak ka XOR
        count += mp[xorr ^ k];              // ★ pichhla XOR jo chahiye, kitni baar dikha
        mp[xorr]++;                         // abhi wala map me, lookup ke BAAD
    }
    return count;
}
```

### ▸ COUNT SUBARRAYS WITH MAX == K  = BOUNDARY-index (2 pointer track, NO stack/SW)

```
Ek array aur ek number K diya hai. Kitne lagaatar subarray hain jinka sabse bada number theek K ho (na zyada, na kam), wo ginti lautao.
  e.g. [8,2,1,3,4,5,1,10], K=3 -> 3
  kyun: [2,1,3], [1,3], [3]; 8 aur 4 K se bade = deewar, unhe chhoo nahi sakte

kitne subarray jinka MAX bilkul K. [8,2,1,3,4,5,1,10],K=3 -> 3 ([2,1,3],[1,3],[3])
valid = (har element <= K) AND (>=1 element == K)
K se BADE element = WALL (array todte). segment ke andar K wale subarray gino
═════════════════════════ ★ EDGE ═════════════════════════════════════
"subarray"->SW, "max"->STACK, "count"->PQ sab galat: max FIXED K se compare -> simple scan
```

```
TASVEER:

[8, 2, 1, 3, 4, 5, 1, 10],  K = 3      (K se bada = DEEWAR)

 10 |                                  #
  8 | #                                #
  5 | #                     #          #
  4 | #                 #   #          #
  3 | #           [3]   #   #          #   <- K wali line
  2 | #     #     [3]   #   #          #
  1 | #     #  #  [3]   #   #   #      #
    +----------------------------------------
idx   0     1  2   3    4   5   6      7
     WALL   .  .   K  WALL WALL .    WALL
     |<-- kamra -->|

har i pe:   lastWall = aakhri DEEWAR ka INDEX,   lastK = aakhri K ka INDEX
            agar lastK > lastWall  ->  count += lastK - lastWall

 i  arr   lastWall  lastK   jodo
 0   8       0       -1      -       (deewar)
 1   2       0       -1      -       (K abhi aaya hi nahi)
 2   1       0       -1      -
 3   3       0        3     +3       [2,1,3] [1,3] [3]  = shuru 1, 2, 3
 4   4       4        3      -       deewar ne K ko peeche chhod diya
 5   5       5        3      -
 6   1       5        3      -       3 - 5 = -2  -> GUARD na ho to ginti ghatti
 7  10       7        3      -
                            = 3

★ 7-Nov REDO_2 ki galti: lastWall = arr[i] likha (8, 4...)  ->  INDEX chahiye (0, 4...), ginti index se banti
★ teesri if (lastK > lastWall) chhooti: i = 6 pe 3 - 5 = -2 jud jaata

PURANI SHEET KI LINE (git b086ea01):
★ count ~n^2/2 (5e9) -> LONG LONG (int overflow).
WHY simple: "hard-dikhne" wala tha (scary constraints + 3 candidate-pattern), par EK insight (walls) ne
   collapse kar diya -> linear. mushkil = insight-dhundhna, code chhota. (search->test->eliminate = process.)
```

```cpp
long long countSubarraysMaxK(vector<int> &arr, int K) {
    int lastWall = -1;                      // aakhri index jahan arr[i] > K  (-1: abhi koi wall nahi)
    int lastK = -1;                         // aakhri index jahan arr[i] == K (-1: abhi koi K nahi)
    long long count = 0;                    // ★ LONG LONG: count ~n^2/2 (5e9) -> int overflow
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] > K)
            lastWall = i;                   // pehle lastWall / lastK update, PHIR count
        if (arr[i] == K)
            lastK = i;
        if (lastK > lastWall)               // ★ guard: warna lastK - lastWall NEGATIVE ([5,3,5] pe dikhta)
            count += lastK - lastWall;      // start choices = lastWall+1 .. lastK
    }
    return count;
}
```

### ▸ DIFFERENCE ARRAY / RANGE UPDATE (LC-1109 Corporate Flight Bookings)

```
n flights hain (1 se n). Har booking [first, last, seats] ka matlab: first se last tak har flight me itni seats book hui.
Saari bookings ke baad har flight ki kul booked seats ka array lautao.
  e.g. n=5, [[1,2,10],[2,3,20],[2,5,25]] -> [10,55,45,25,25]
  kyun: flight 2 = 10+20+25 = 55; flight 3 = 20+25 = 45; flight 4, 5 = sirf 25

= prefix-sum ka ULTA · SWEEP-LINE (MEETING ROOMS II, PATTERN 15) ka sagaa bhai
KAB: har RANGE pe kuch ADD karo, 20,000 range. seedha = O(range x length), chahiye O(n + ranges)
har element mat chhuo: sirf DO jagah nishaan (shuru + khatam ke baad), phir ek baar chalti-sum = value
```

```
TASVEER:

DRY-RUN  n=5, bookings=[[1,2,10],[2,3,20],[2,5,25]]:
    diff:   idx  1    2    3    4    5    6
                +10  +20  -10  -20   .   -25
                     +25
            ---------------------------------
            net  +10  +45  -10  -20   0   -25
    run :        10   55   45   25   25
    ans = [10, 55, 45, 25, 25]

PURANI SHEET KI KYUN LINE (code ke saath wali):
★★ SWEEP-LINE se RISHTA (Arpan ne khud jod diya — "dono same hain, bas do farak"):
```

```cpp
vector<int> corpFlightBookings(vector<vector<int>> &bookings, int n) {
    vector<int> diff(n + 2);                // ★ n+2: end+1 sabse bada n+1 ho sakta (n+1 rakha to crash)
    for (auto &it : bookings) {
        int start = it[0];
        int end = it[1];
        int count = it[2];
        diff[start] += count;               // yahan se shuru
        diff[end + 1] -= count;             // ★ +1: last din bhi inclusive, uske AGLE din se hatao
    }
    vector<int> ans;
    int cummSum = 0;                        // chalti-sum abhi khaali
    for (int i = 1; i <= n; i++) {          // 1..n: flights 1-based, diff[0] / diff[n+1] answer me nahi
        cummSum += diff[i];
        ans.push_back(cummSum);
    }
    return ans;
}
// SWEEP-LINE se farak (machine EK, teen cheez badalti):
//   rakho : meeting rooms = map (time bada / bikhra, 10^9) · yahan = plain array (index chhota, 1..n)
//   ghatao: meeting rooms [start,end) exclusive -> -1 at end
//           yahan [first,last] inclusive -> -count at end+1
//           (inclusive me end pe ghataya = aakhri element chhoot gaya)
//   nikalo: meeting rooms = chalti-sum ka MAX ("kitne kamre")
//           yahan = poora array ("har flight ka total")
// FAMILY: range-add · range-flip · car-pooling · "kitne ek-saath"
//   (JP OA bank me 3-4 baar: "Interval Flip Operations", "Minimum Cores", "Maximum Concurrent Tasks")
```

