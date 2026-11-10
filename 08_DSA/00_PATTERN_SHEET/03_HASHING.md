## PATTERN 3 — HASHING


> [!IMPORTANT]
> **BROAD IDEA:** hashmap/hashset = O(1) lookup. "pehle dekha?" / "complement hai?" / count -> O(n).
>
> **KAB:** "pehle dekha?" · "jodi / complement hai?" · "kitni baar aaya?" · grouping (anagram)

**★ MILTE-JULTE** (longest consecutive vs LIS): 00_SHURU me NAAM-SE-CONFUSE B dekh.

### COMMON SKELETON (seen / complement / count family -- ek pass):

```
    map/set banao                      // kabhi PRELOAD: two-sum khaali · subarray-sum-k -> mp{0:1}
    for x in arr:
        CHECK pehle                    // map me complement/seen/count hai? -> ans use/count
        UPDATE baad                    // map me x daalo / count++ (warna x KHUD se match kar leta)
    // har hashing Q one-pass nahi: group-anagrams = pehle poora map bharo phir output ·
    // longest-consec = set-membership. ye skeleton sirf "seen/complement/count" family ke liye.
```

#### ── FAMILY: 2-MAP BIJECTION (mp1 + mp2 dono taraf; koi ek clash -> false, ek map kaafi nahi) ──

> [!WARNING]
> **★ DHYAAN:** 1-to-1 mapping (isomorphic / word pattern) -> DO map, ek kaafi nahi

---

### ▸ ISOMORPHIC STRINGS

**SAWAAL**
```
Do barabar lambai ki string s aur t di hain. Kya s ke har akshar ko kisi ek pakke akshar se badal ke t ban sakti hai?
Ek akshar hamesha ek hi akshar banega, aur do alag akshar ek hi akshar nahi ban sakte (one-to-one). true ya false lautao.
  e.g. "paper", "title" -> true
  kyun: p->t, a->i, e->l, r->e: har jagah wahi mapping, do char ek hi char pe nahi gaye
  TRUE wala chal ke: "egg", "add"
    i=0: e aur a dono naye -> e ke a, a ke e (mp1[e]=a, mp2[a]=e)
    i=1: g aur d dono naye -> g ke d, d ke g
    i=2: g pehle se hai, mp1[g] = d, aur t me bhi d aaya -> theek. d ka bhi mp2[d] = g -> theek
    -> loop poora, koi clash nahi -> true
  FALSE wala chal ke: "foo", "bar"
    i=0: f ke b, b ke f -> theek
    i=1: o ke a, a ke o -> theek
    i=2: o pehle se hai, mp1[o] = a, par t me aaya r -> a != r, clash -> false
```

> [!IMPORTANT]
> **CHAABI:** DO map, dono taraf (`mp1` s->t, `mp2` t->s). Kisi bhi EK taraf clash -> false. `count()` pehle, phir milao, phir dono me save.
>
> s->t har char ka 1-to-1 mapping? "egg"->"add" true; "foo"->"bar", "ab"->"aa" false

> [!WARNING]
> `mp2` (t->s) hatao mat: "ab"->"aa" ka clash sirf ULTI taraf pakda jaata. Aur `mp1.count()` pehle check karo, seedha `mp1[x]` khaali key bana ke galat clash deta.

**CODE**
```cpp
bool isIsomorphic(string s, string t) {
    unordered_map<char, char> mp1;          // s -> t
    unordered_map<char, char> mp2;          // ★ t -> s ("ab"->"aa" clash sirf ULTI taraf pakadta)
    for (int i = 0; i < s.size(); i++) {
        if ((mp1.count(s[i]) == 1 && mp1[s[i]] != t[i]) ||   // ★ count() pehle: seedha mp1[x] khaali key
            (mp2.count(t[i]) == 1 && mp2[t[i]] != s[i]))     //   bana ke galat clash deta
            return false;                   // || : kisi bhi EK taraf clash = 1-to-1 nahi
        else {
            mp1[s[i]] = t[i];               // check ke BAAD dono taraf mapping save
            mp2[t[i]] = s[i];
        }
    }
    return true;                            // bina clash poora chala = isomorphic
}
```


---

### ▸ WORD PATTERN  = ISOMORPHIC ka bhai (char <-> WORD)

**SAWAAL**
```
Ek pattern (akshar) aur ek string s (space se alag words) di hai. Kya pattern ka har akshar hamesha ek hi word se juda hai, aur har word hamesha ek hi akshar se?
Isomorphic jaisa hi, bas akshar ki jagah word. true ya false lautao.
  e.g. "abba", "dog cat cat dog" -> true
  kyun: a->dog, b->cat; a b b a = dog cat cat dog, mapping kahin nahi tooti
```

> [!IMPORTANT]
> **CHAABI:** ISOMORPHIC ka hi code, bas key `pattern[i] <-> word` (`stringstream ss >> word`). Clash line EXACT wahi.

> [!WARNING]
> Aakhir me words ki ginti milao: `pattern.size() == j`, warna "a", "dog cat" galat true. `j++` if ke BAHAR (extra word bhi gino), `i < pattern.size()` guard ke andar hi pattern padho.

**CODE**
```cpp
bool wordPattern(string pattern, string s) {   // = ISOMORPHIC, bas char <-> WORD
    unordered_map<char, string> mp1;
    unordered_map<string, char> mp2;
    stringstream ss(s);                     // ★ delta 1: input = word-stream (<sstream>)
    string word;
    int i = 0;                              // i = pattern ka index
    int j = 0;                              // j = words ki ginti
    while (ss >> word) {
        if (i < pattern.size()) {           // i<size: words zyada hon to pattern[i] bahar na padhe
            if ((mp1.count(pattern[i]) == 1 && mp1[pattern[i]] != word) ||   // delta 2: key =
                (mp2.count(word) == 1 && mp2[word] != pattern[i]))           //   pattern[i] <-> word
                return false;               // ISOMORPHIC ki EXACT clash line
            else {
                mp1[pattern[i]] = word;     // check ke BAAD save
                mp2[word] = pattern[i];
            }
            i++;                            // agla pattern char
        }
        j++;                                // if ke BAHAR: har word gino, extra word bhi
    }
    return (pattern.size() == j) ? true : false;   // ★ delta 3: words ki ginti == pattern length
}                                           //   warna "a","dog cat" galat true
```

---

── (STANDALONE — apni alag trick) ──


### ▸ TWO SUM

**SAWAAL**
```
Ek array aur ek target diya hai. Wo do index lautao jin pe rakhe numbers ka jod target ke barabar ho.
Jawab hamesha ek hi hai, aur ek hi number do baar nahi le sakte.
  e.g. [2,7,11,15], target=9 -> [0,1]
  kyun: nums[0] + nums[1] = 2 + 7 = 9
```

> [!IMPORTANT]
> **CHAABI:** map `{value -> index}`. Har number pe pehle dekho `target - nums[i]` map me hai? Mila to dono index. CHECK pehle, daalo BAAD.

> [!WARNING]
> `mp[nums[i]] = i` check ke BAAD hi. Pehle daala to number khud se match kar leta.

**CODE**
```cpp
vector<int> twoSum(vector<int> &nums, int target) {
    unordered_map<int, int> mp;             // {value -> index}
    for (int i = 0; i < nums.size(); i++) {
        if (mp.find(target - nums[i]) != mp.end())   // ★ CHECK pehle: (target - num) pehle dekha?
            return {mp[target - nums[i]], i};        // mila -> dono index
        mp[nums[i]] = i;                    // ★ daalo BAAD: taaki num khud se match na kare
    }
    return {-1, -1};
}
```


---

### ▸ VALID ANAGRAM

**SAWAAL**
```
Do string s aur t di hain. Kya t, s ka anagram hai, matlab dono me same akshar utni hi baar hain, bas kram alag ho sakta hai? true ya false lautao.
  e.g. "anagram", "nagaram" -> true
  kyun: dono me a=3, n=1, g=1, r=1, m=1
```

> [!IMPORTANT]
> **CHAABI:** ek map: s se `++`, t se `--`, end me har key 0 honi chahiye. Length alag -> turant false.

> [!WARNING]
> Check `!= 0` karo, sirf `> 0` nahi: t me zyada aaya char minus me jaata, wo bhi pakadna hai.

**CODE**
```cpp
bool isAnagram(string s, string t) {        // har char ka count barabar? (count[26] ya map)
    if (s.size() != t.size())
        return 0;                           // length alag -> turant false
    unordered_map<char, int> mp;
    for (auto &it : s)
        mp[it]++;                           // s se ++
    for (auto &it : t)
        mp[it]--;                           // t se --
    for (auto &it : mp) {
        if (it.second != 0)                 // ★ != 0 (sirf > 0 nahi, -ve bhi aata)
            return false;
    }
    return true;                            // sab ZERO -> anagram
}
```


---

### ▸ GROUP ANAGRAMS

**SAWAAL**
```
Strings ka ek array diya hai. Jo strings aapas me anagram hain (same akshar, kram alag), unhe ek group me daalo. Saare group lautao.
  e.g. ["eat","tea","tan","ate","nat","bat"] -> [eat,tea,ate] [tan,nat] [bat]
  kyun: sorted key: eat/tea/ate = "aet", tan/nat = "ant", bat = "abt" -> 3 key = 3 group
```

> [!IMPORTANT]
> **CHAABI:** KEY = sorted(word). `mp[key].push_back(word)`, map ki har list = ek group.
>
> "eat","tea","ate" -> teeno ka SORTED roop "aet" = same. Yahi KEY.
> brute = har jodi "anagram hai?" O(n² · k) -> har word ki KEY bana ke map me CACHE -> O(n · k log k)

> [!WARNING]
> `string key = it` copy pe sort karo. Asli word sort kiya to group me "aet" chala jaata, "tea" nahi.

**CODE**
```cpp
vector<vector<string>> groupAnagrams(vector<string> &strs) {
    unordered_map<string, vector<string>> mp;   // KEY = sorted(word) -> us group ke words
    for (auto &it : strs) {
        string key = it;                    // ★ copy lo: asli word sort hua to group me galat word jaata
        sort(begin(key), end(key));         // ★ "tea" -> "aet"
        mp[key].push_back(it);              // same key = same group
    }
    vector<vector<string>> ans;
    for (auto &it : mp)
        ans.push_back(it.second);           // map ki har list = ek group
    return ans;
}
// KEY ka doosra roop (sirf a-z): 26 ki ginti "1#0#0#...#" -> sort nahi, O(n · k)
//   (separator "#" zaroori: bina uske a=1,b=11 aur a=11,b=1 dono "111")
// "" (khaali string) bhi chalta: sorted("") = "" -> ek hi group
```


---

### ▸ SUBARRAY SUM = K  = prefix-sum + hashmap {prefixSum -> count}

**SAWAAL**
```
Ek array aur ek number k diya hai. Kitne lagaatar subarray hain jinka jod theek k ke barabar ho, wo ginti lautao.
Array me minus number bhi ho sakte hain.
  e.g. [1,2,3], k=3 -> 2
  kyun: [1,2] = 3 aur [3] = 3; baaki [1],[2],[2,3],[1,2,3] nahi
```

> [!IMPORTANT]
> **CHAABI:** map `{prefixSum -> kitni baar}`. Har i pe `count += mp[sum - k]`, phir `mp[sum]++`.

### SUM - K KYUN CHALTA HAI

```
Hum dhadadhad aage chalte hain aur sum jodte jaate hain. sum = yahan tak ka poora jod.
sum - k nikaalte hain: agar wo peeche kahin sum ban ke map me pada hai,
to wahan ke BAAD se yahan tak ka jod theek k hai.

[1,2,3], k=3      shuru: mp = {0:1}
  i=0: sum = 1       1 - 3 = -2   map me nahi
  i=1: sum = 1+2 = 3 3 - 3 =  0   map me hai -> yahan tak ka sum 3 hai, aur sum me se 3 ghataya
                                    tabhi 0 aaya -> [1,2] ka jod 3              count = 1
  i=2: sum = 6       6 - 3 =  3   map me hai (i=1 pe sum 3 tha) -> uske baad se yahan tak = [3]
                                    ka jod 3                                     count = 2

Ye trick sirf un sawaalon me jahan "LAGAATAAR tukde ka jod = k" maanga ho.
Tukda = abhi ka sum - peeche ka sum. To peeche ka sum = sum - k. Wahi map me dhoondte hain.
```

> [!WARNING]
> `mp[0] = 1` se shuru (sum khud == k wala case). Aur `sum - k` hi, two-sum ka `k - sum` ulta yahan mat lagao.

**CODE**
```cpp
int subarraySum(vector<int> &nums, int k) { // prefix-sum + hashmap {prefixSum -> count}
    unordered_map<int, int> mp;
    int sum = 0, count = 0;
    mp[0] = 1;                              // ★ START zaroori: jab sum khud == k, tab prefix-0 chahiye
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];                     // running sum
        count += mp[sum - k];               // ★ sum - k (k - sum NAHI): abhi 15 km - aakhri tukda 5 =
                                            //   10 pe tukda shuru -> "10 kitni baar dikha?" BADA pehle
                                            //   (two sum ka "target - x" ulta yahan mat lagao)
        mp[sum]++;                          // check ke BAAD daalo
    }
    return count;
}
```


---

### ▸ LONGEST SUBARRAY SUM = K  (03/15)  = SUBARRAY SUM = K ka bhai, GINTI ki jagah LAMBAI

**SAWAAL**
```
Ek array aur ek number k diya hai (minus number bhi ho sakte hain). Sabse lamba lagaatar subarray dhoondo jiska jod
theek k ho, aur uski lambai lautao. Koi na mile to 0.
  e.g. [3,-1,4,-2,2,1], k=4 -> 5
  kyun: [-1,4,-2,2,1] ka jod 4 hai aur lambai 5; [4] bhi 4 hai par lambai sirf 1
```

**SAAR**

> [!IMPORTANT]
> **FARAK (subarray sum = k se):**
>
> wahan map = {prefixSum -> KITNI BAAR}   (ginti chahiye thi)
>
> yahan map = {prefixSum -> PEHLA INDEX}  (lambai chahiye: i - jahan pehli baar ye sum dikha)
>
> start: mp[0] = 1 ki jagah mp[0] = -1   (index -1: shuru se i tak ka tukda, lambai = i + 1)

> [!IMPORTANT]
> **CHAABI:** map `{prefixSum -> PEHLA index}`, `mp[0] = -1`. `sum - k` mila to lambai `i - mp[sum - k]`.

### SUM - K KYUN CHALTA HAI (upar SUBARRAY SUM = K wali hi trick)

```
Wahi trick: dhadadhad chalo, sum jodo, sum - k map me dhoondo. Mila to wahan ke baad se yahan tak jod = k.
Bas yahan map me PEHLA index rakha, to lambai = i - mp[sum - k].

[3,-1,4,-2,2,1], k=4      shuru: mp = {0:-1}

i  num  sum  sum-k  map me?          lambai        ans   map me daala
0   3    3    -1    nahi             -             0     3:0
1  -1    2    -2    nahi             -             0     2:1
2   4    6     2    haan, index 1    2-1 = 1       1     6:2
3  -2    4     0    haan, index -1   3-(-1) = 4    4     4:3
4   2    6     2    haan, index 1    4-1 = 3       4     (6 pehle se, skip)
5   1    7     3    haan, index 0    5-0 = 5       5     7:5

i=3: sum 4, 4 - 4 = 0 -> shuru se yahan tak jod 4 -> [3,-1,4,-2], lambai 4
i=5: sum 7, 7 - 4 = 3 -> 3 index 0 pe pada -> index 0 ke baad se yahan tak jod 4
     -> [-1,4,-2,2,1], lambai 5 - 0 = 5
```

> [!WARNING]
> `if (!mp.count(sum)) mp[sum] = i`: PEHLA index hi rakho, baad wala likh diya to tukda chhota. Lambai me `+1` nahi, wo index tukde me nahi.

**CODE**
```cpp
int longestSubarraySumK(vector<int> &nums, int k) { // prefix-sum + hashmap {prefixSum -> pehla index}
    unordered_map<int, int> mp;
    mp[0] = -1;                             // ★ index -1: shuru se hi jod k ho to lambai i - (-1) = i + 1
    int sum = 0;
    int ans = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];                     // running sum
        if (mp.count(sum - k)) {            // sum - k pehle dikha? to wahan ke baad se i tak jod = k
            ans = max(ans, i - mp[sum - k]);// ★ lambai = i - us index (+1 nahi, wo index tukde me nahi)
        }
        if (!mp.count(sum))                 // ★ override se bachao: PEHLA index hi rakho,
            mp[sum] = i;                    //   baad wala likh diya to tukda chhota ho jaayega
    }
    return ans;
}
// [0,0,0], k=0 -> 3: mp[0] = -1 pe hi ruka rehta (guard), i=2 pe 2 - (-1) = 3
```


---

### ▸ COUNT DIVISIBLE PAIRS  = two-sum/hashmap-complement, par KEY = REMAINDER

**SAWAAL**
```
Ek array aur ek number k diya hai. Kitne jode (i < j) hain jinka jod k se poora kat jaaye, matlab (nums[i] + nums[j]) % k == 0. Wo ginti lautao.
  e.g. [1,2,3,4,5], k=3 -> 4
  kyun: 1+2=3, 1+5=6, 2+4=6, 4+5=9 -> chaaron 3 se divide; baaki pair nahi
```

> [!IMPORTANT]
> **CHAABI:** key = REMAINDER. partner ka rem = `(k - rem) % k`; `mp[partner]` jitna, utne pair. Lookup pehle, `mp[rem]++` baad.
>
> pairs (i<j): (A[i]+A[j])%k==0. brute O(n²) -> "partner pehle aaya?" ko hashmap me CACHE -> O(n)
> A[i] KYUN, prefix NAHI: pair = 2 ALAG element (contiguous range nahi) -> har element ka apna rem

> [!WARNING]
> `((it % k) + k) % k`: C++ me negative ka `%` negative aata, key galat. `rem = 0` pe partner `(k - 0) % k = 0`, isliye `% k`. count `long long` (jode n²/2 tak).

**SAAR**

> [!IMPORTANT]
> **★ k-rem ka INTUITION (ek element se, concrete):**
> maan it=1, k=3.   pehle remainder: 1%3 = 1.
> chahiye: (1 + partner) % 3 == 0.   to partner ka remainder = 3 - 1 = 2.
> ab MAP me dekho: koi element pehle aaya jiska remainder 2 tha?
> mp[2] jitna -> utne pair bane (har aisa element 1 ke saath jodi banata).
> yehi hai partner = (k - rem) = 3 - 1 = 2.
>
> (rem=0 aaya -> partner = 3-0 = 3, par 3%3 = 0 -> partner 0.  isiliye (k-rem)%k.)

### DRY-RUN

```
── DRY-RUN (nums=[1,2,3,4,5], k=3  ->  4 pairs) ──
 rem = ((it)%k+k)%k :   1->1   2->2   3->0   4->1   5->2

 it  rem  partner=(k-rem)%k   mp[partner]   count    mp-after
  1   1        2                  0           0       {1:1}
  2   2        1                  1          +1=1     {1:1, 2:1}      <- pair (1,2)
  3   0        0                  0           0       {1:1, 2:1, 0:1}
  4   1        2                  1          +1=2     {1:2, 2:1, 0:1} <- pair (2,4)
  5   2        1                  2          +2=4     {1:2, 2:2, 0:1} <- pairs (1,5),(4,5)
 ->  count = 4
```

> [!IMPORTANT]
> **KEY:** it=5 pe mp[1]=2 (element 1 aur 4 dono ka rem 1) -> ek saath 2 pair jude.

> [!WARNING]
> LOOKUP pehle, INSERT baad (khud-se-pair na gine).

**CODE**
```cpp
long long countDivisiblePairs(vector<int> &nums, int k) {
    unordered_map<int, int> mp;             // {remainder -> kitne aaye}
    // ★ jode n²/2 tak ja sakte (65537 zero, k=1 -> 2.14 arab > INT_MAX) -> long long
    long long count = 0;
    for (auto &it : nums) {
        int rem = (((it % k) + k) % k);     // ★ +k phir %k: C++ me negative ka % negative aata, key galat
        count += mp[(k - rem) % k];         // ★ partner rem: it=1,k=3 -> 1+? = 3 -> 2. rem=0 -> 3%3 = 0
                                            //   isliye (k-rem)%k. mp[2] jitne = utne pair
        mp[rem]++;                          // lookup PEHLE, insert BAAD: khud-se-pair na gine
    }
    return count;                           // [1,2,3,4,5],k=3 -> 4 (it=5 pe mp[1]=2 -> ek saath 2 pair)
}
```


---

### ▸ LONGEST CONSECUTIVE SEQ (LC-128)  = SET + har chain sirf START se gino

**SAWAAL**
```
Ek bina sort kiya hua array diya hai. Usme lagaatar numbers ki sabse lambi chain kitni lambi hai (jaise 1, 2, 3, 4), wo lautao.
Sort nahi karna, O(n) me karna hai.
  e.g. [100,4,200,1,3,2] -> 4
  kyun: 1,2,3,4 sab maujood = chain 4; 100 aur 200 akele
```

> [!IMPORTANT]
> **CHAABI:** sab SET me. START = jiska `num - 1` set me NAHI; wahi se `currNum + 1` tak chain gino, `max` lo.
>
> unsorted -> sabse LAMBI consecutive chain ki LENGTH. [100,4,200,1,3,2] -> 4 (1,2,3,4)
> sort (n log n) mat karo -> SET me O(1) lookup, har CHAIN sirf uske START se gino -> O(n)

> [!WARNING]
> SET pe ghoomo, `nums` pe nahi (duplicate START baar-baar na gine). `num - 1` HAI to skip, warna har num se ginti = O(n²).

**SAAR**

### DRY-RUN

```
DRY-RUN [100,4,200,1,3,2]  set={1,2,3,4,100,200}:
  100 -> 99 nahi -> START -> 101? nahi        -> count=1
  4   -> 3 HAI   -> beech ka -> skip
  200 -> 199 nahi-> START -> 201? nahi         -> count=1
  1   -> 0 nahi  -> START -> 2,3,4 hai         -> count=4   <= ans=4
  3,2 -> (num-1) HAI -> skip
  => ans = 4
```

> [!IMPORTANT]
> -> har element zindagi me 1 hi baar visit -> O(n)  (warna har num se ginti = O(n^2)).

**FAMILY:** hashing/SET (O(1) lookup se sort bachao). trick = "har group sirf START se process".

**CODE**
```cpp
int longestConsecutive(vector<int> &nums) {
    int ans = INT_MIN;
    unordered_set<int> st;                  // set: O(1) lookup + duplicate apne aap hat jaate
    for (auto &it : nums)
        st.insert(it);
    for (int num : st) {                    // ★ SET pe ghoomo (nums pe nahi): duplicate START baar-baar na gine
        if (!st.count(num - 1)) {           // ★ START tabhi jab num-1 set me NAHI, warna beech ka: skip
            int currNum = num;
            int count = 1;                  // count=1: START khud chain me
            while (st.count(currNum + 1)) { // agla set me hai tab tak chain lambi
                currNum++;
                count++;
            }
            ans = max(ans, count);          // while ke BAAD: poori chain gin ke record
        }
    }
    return ans == INT_MIN ? 0 : ans;        // khaali array -> 0
}                                           // 100 START(1) · 4 skip · 200 START(1) · 1 -> 1,2,3,4 = 4
```


---

### ▸ MAJORITY ELEMENT (LC-169, >n/2)  = BOYER-MOORE VOTING (do alag cancel)

**SAWAAL**
```
Ek array me ek number aadhe se zyada baar (n/2 se zyada) aata hai, aur aisa number hamesha hota hai. Wo number lautao.
  e.g. [2,2,1,1,1,2,2] -> 2
  kyun: n = 7, 2 aaya 4 baar > 7/2 = 3
```

> [!IMPORTANT]
> **CHAABI:** Boyer-Moore: same -> `count++`, alag -> `count--`. Do ALAG element cancel, majority bachta. O(1) space.
>
> map se: mp[x]++, count > n/2 wala return -> O(n) time, O(n) space (seedha)

> [!WARNING]
> `count == 0` pe RESET (`candidate = nums[i]`, `count = 1`). Chhoda to count -1 me jaata, candidate kabhi nahi badalta.

**CODE**
```cpp
int majorityElement(vector<int> &nums) {    // BOYER-MOORE VOTING, O(1) space
    int count = 1;                          // majority > n/2 -> baaki sab milke bhi kam ->
    int candidate = nums[0];                //   do ALAG element CANCEL karo, majority bachega
    for (int i = 1; i < nums.size(); i++) {
        if (candidate == nums[i])
            count++;                        // == candidate -> count++
        else if (candidate != nums[i]) {
            count--;                        // != candidate -> count--
            if (count == 0) {               // ★ 0 pe RESET (chhoda to -1 me jaata, reset kabhi nahi = BUG)
                candidate = nums[i];
                count = 1;
            }
        }
    }
    return candidate;                       // end: candidate = ans
}
```


---

### ▸ FIND DISAPPEARED NUMBERS (LC-448)  = ARRAY-KHUD-hashmap · index-negation, O(1) space

**SAWAAL**
```
n size ka array hai, har value 1 se n ke beech hai aur kuch number dohraye gaye hain. 1 se n tak ke jo number array me nahi hain, wo sab lautao.
Extra jagah O(1) honi chahiye.
  e.g. [4,3,2,7,8,2,3,1] -> [5,6]
  kyun: n = 8, maujood 1,2,3,4,7,8; 5 aur 6 kahin nahi
```

> [!IMPORTANT]
> **CHAABI:** value v ka ghar = index `v - 1`; dikha to us index ko NEGATIVE karo. End me jo `+ve` bacha, uska `i + 1` missing.
>
> size n, values 1..n (repeat ho sakte). jo MISSING wo sab. [4,3,2,7,8,2,3,1] -> [5,6]
> extra set/map nahi: value v ka ghar = index v-1 -> dikha to us index ko NEGATIVE mark

> [!WARNING]
> `abs(nums[i])` lo (value pehle hi -ve ho sakti) aur `nums[index] > 0` guard. Bina guard duplicate pe `-(-x)` wapas +ve: `[2,2]` pe `[1,2]`, sahi `[1]`.

**SAAR**
```
DRY-RUN [4,3,2,7,8,2,3,1]: mark index 3,2,1,6,7,1(already),2(already),0 -ve
  -> +ve bache index 4,5 -> missing = 5,6.

  index      :   0    1    2    3    4    5    6    7
  shuru      :   4    3    2    7    8    2    3    1
  mark baad  :  -4   -3   -2   -7   (8)  (2)  -3   -1
  +ve index 4, 5 -> missing value 5, 6
```

**CODE**
```cpp
vector<int> findDisappearedNumbers(vector<int> &nums) {
    vector<int> ans;
    for (int i = 0; i < nums.size(); i++) {
        int index = abs(nums[i]) - 1;       // ★ abs(): value pehle hi -ve ho chuki ho sakti
        // ★ >0 guard: duplicate dobara aaye to -(-x) = +ve wapas -> wo index 'missing' dikhega.
        //   [2,2] pe guard ke bina [1,2] (sahi [1])
        if (nums[index] > 0)
            nums[index] = -nums[index];
    }
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] > 0)                    // abhi bhi +ve = index+1 kabhi mark nahi hua = MISSING
            ans.push_back(i + 1);           // i+1: index i ka ghar value i+1 ka (1-based)
    }
    return ans;
}
// SAME trick: find-duplicate (287, sirf jab array badal sakte; asli 287 = Floyd) · first-missing-
//   positive (41) · single-in-pairs. "values 1..n + O(1) space" = index-marking ka signal
//   (array modify NA kar sakte -> Floyd cycle)
```

---

#### ── FAMILY: PER-KEY STREAK (log line todo -> har KEY ki apni chalti ginti; Wise OA 27-Oct) ──

### ▸ SERVER ERROR STREAK  (03/11)

**SAWAAL**
```
Logs ki lines di hain, har line "<server> <code> <msg>". Kisi server ke lagaatar 3 "error" aayein to ek alert gino aur us server ki ginti 0 kar do.
Beech me "success" aaya to us server ki ginti 0. Kul kitne alert hue, wo lautao.
  e.g. ["s1 500 error","s2 200 success","s1 500 error","s3 500 error","s1 500 error"] -> 1
  kyun: s1 ke 3 error (s2, s3 beech me aaye par s1 ki ginti nahi todte) -> 1 alert; s3 ka sirf 1 error
```

> [!IMPORTANT]
> **CHAABI:** key = server, map `{server -> lagaataar error}`. line todo `ss >> server >> code >> msg`; error -> `++`, 3 pura -> alert + 0; success -> 0.
>
> logs "s1 500 error", "s2 200 success" ... kisi server ke LAGAATAAR 3 error -> alert++, ginti 0.
> pench: beech me DOOSRE server ki lines aati -> ek window nahi, "KISKA lagaataar" = key = server   (00_SHURU me NAAM-SE-CONFUSE E)

> [!WARNING]
> success pe `mp[server] = 0`, `--` NAHI (lagaataar TOOTA, ghata nahi). Alert ke baad bhi 0 karo, warna 4th, 5th error bhi alert ginta.

**CODE**
```cpp
int countAlerts(vector<string> &logs) {
    unordered_map<string, int> mp;          // server -> abhi tak lagaataar error
    int count = 0;
    for (auto &s : logs) {
        stringstream ss(s);                 // ek LINE do (poora vector nahi -> compile error)
        string server, code, msg;
        ss >> server >> code >> msg;        // ★ space se 3 tukde
        if (msg == "error") {               // faisla msg se (code kuch bhi ho sakta)
            mp[server]++;
            if (mp[server] >= 3) {          // ★ 3 pura -> alert, aur ginti 0 (warna 4th, 5th bhi ginta)
                count++;
                mp[server] = 0;
            }
        }
        else if (msg == "success")
            mp[server] = 0;                 // ★ = 0, -- NAHI (lagaataar TOOTA, ghata nahi)
    }
    return count;
}
// SAME shakal: "user ke lagaataar 3 galat password -> lock", "card ke lagaataar fail" -> key = user / card
```

---

### ▸ CSV MARKS TOTAL  (03/13)

**SAWAAL**
```
Lines di hain, har line "naam,subject,marks". Har student ke kul marks nikaalo, aur naam ke alphabet kram me lautao.
  e.g. ["ravi,maths,40","priya,maths,55","ravi,sci,35","priya,sci,30"] -> priya 85, ravi 75
  kyun: ravi 40 + 35 = 75, priya 55 + 30 = 85
```

> [!IMPORTANT]
> **CHAABI:** `getline(ss, name, ',')` teen baar (comma tak), `stoi(marks)`, `mp[name] += t`. `map` (sorted) = alphabet kram apne aap.
>
> line todo -> naam = key -> map me marks jodte jao. pench: tukde COMMA se, space se nahi -> >> nahi chalega, getline

> [!WARNING]
> Tukde COMMA se hain, `>>` space pe todta, yahan nahi chalega; `getline` with `','`.

**CODE**
```cpp
map<string, int> totalMarks(vector<string> &lines) {
    map<string, int> mp;
    for (auto &it : lines) {
        stringstream ss(it);
        string name, sub, marks;
        getline(ss, name, ',');             // ★ comma tak -> "ravi"
        getline(ss, sub, ',');
        getline(ss, marks, ',');            // aakhri tukda, comma na ho tab bhi
        int t = stoi(marks);
        mp[name] += t;
    }
    return mp;
}
```

---

### ▸ SHIFT MINUTES  (03/14, HH:MM)

**SAWAAL**
```
Guard ki shift ka start aur end "HH:MM" (24 ghante ki ghadi) me diya hai. Shift raat 12 ke paar bhi ja sakti hai. Shift kitne minute ki hai, wo lautao.
  e.g. "22:30", "01:15" -> 165
  kyun: 22:30 se 24:00 = 90 minute, 00:00 se 01:15 = 75 minute, 90 + 75 = 165
```

**SAAR**
```
dono time ko MINUTE me badlo, phir ghatao. ghante aur minute alag ghatao to MM ka udhaar lena padta, ulajh jaata.
```

> [!WARNING]
> **pench:** raat 12 ke paar end chhota -> minus aata -> ek poora din (1440) jodo.

### KYUN 1440

```
KYUN 1440: din me 24 * 60 = 1440 minute.
  minus aaya:   75 - 1350 = -1275 -> +1440 = 165                  (raat 12 paar)
  minus nahi:   1050 - 540 = 510 -> +1440 = 1950 -> %1440 = 510   (% se extra din wapas hata)
```

> [!IMPORTANT]
> isliye "+1440 phir %1440" dono case ek line me. Caesar ka "+26 phir %26" bilkul yahi.

> [!IMPORTANT]
> **CHAABI:** dono ko MINUTE banao (`HH * 60 + MM`), phir `(mins2 - mins1 + 1440) % 1440`. Caesar ka `+26 %26` yahi.

> [!WARNING]
> Raat 12 paar end chhota -> minus aata, isliye `+1440`; par `% 1440` bhi zaroori warna seedhe case me ek extra din (1950) jud jaata.

**CODE**
```cpp
int shiftMinutes(string start, string end) {
    stringstream ss1(start);
    stringstream ss2(end);
    string HH1, MM1, HH2, MM2;
    getline(ss1, HH1, ':');
    getline(ss1, MM1, ':');
    getline(ss2, HH2, ':');
    getline(ss2, MM2, ':');
    int mins1 = stoi(HH1) * 60 + stoi(MM1);
    int mins2 = stoi(HH2) * 60 + stoi(MM2);
    return (mins2 - mins1 + 1440) % 1440;   // ★ +1440 phir %1440 (Caesar ka +26 %26). start == end -> 0
}
```

### ULTA: minute -> wapas "HH:MM" (grill 10-Nov, library me naya)

> [!IMPORTANT]
> **Twist:** time ko `m` minute aage / peeche karo (`m` minus bhi). Upar wali `+1440 %1440` hi, bas end me minute ko wapas ghadi banao: ghante = `total / 60`, minute = `total % 60`.

```cpp
total = (total + m + 1440) % 1440;     // minus aur 24 ghante ke paar, dono theek
int H = total / 60, M = total % 60;    // minute -> wapas ghadi
string ans = (H < 10 ? "0" : "") + to_string(H) + ":" + (M < 10 ? "0" : "") + to_string(M);   // 0 aage: "00:15"

// "00:10", m = -20 :  (10 - 20 + 1440) % 1440 = 1430 -> 1430/60 = 23, 1430%60 = 50 -> "23:50"
// "23:45", m = +30 :  (1425 + 30 + 1440) % 1440 = 15 -> 15/60 = 0, 15%60 = 15     -> "00:15"
```

