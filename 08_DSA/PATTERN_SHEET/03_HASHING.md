## PATTERN 3 — HASHING

```
 BROAD IDEA: hashmap/hashset = O(1) lookup. "pehle dekha?" / "complement hai?" / count -> O(n).
 ★ MILTE-JULTE (longest consecutive vs LIS): upar NAAM-SE-CONFUSE B dekh.

 COMMON SKELETON (seen / complement / count family -- ek pass):
    map/set banao                      // kabhi PRELOAD: two-sum khaali · subarray-sum-k -> mp{0:1}
    for x in arr:
        CHECK pehle                    // map me complement/seen/count hai? -> ans use/count
        UPDATE baad                    // map me x daalo / count++ (warna x KHUD se match kar leta)
    // har hashing Q one-pass nahi: group-anagrams = pehle poora map bharo phir output ·
    // longest-consec = set-membership. ye skeleton sirf "seen/complement/count" family ke liye.

```

#### ── FAMILY: 2-MAP BIJECTION (mp1 + mp2 dono taraf; koi ek clash -> false, ek map kaafi nahi) ──

```
 KAB: "pehle dekha?" · "jodi / complement hai?" · "kitni baar aaya?" · grouping (anagram)
 ★ DHYAAN: 1-to-1 mapping (isomorphic / word pattern) -> DO map, ek kaafi nahi
```

### ▸ ISOMORPHIC STRINGS

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

s->t har char ka 1-to-1 mapping? "egg"->"add" true; "foo"->"bar", "ab"->"aa" false
```

```
PURANI SHEET SE (same to same):

SAAR : s->t har char ka 1-to-1 mapping (bijection)? "egg"->"add" true; "foo"->"bar", "ab"->"aa" false.
KEY  : 2 MAP dono taraf -- mp1=s->t, mp2=t->s. ek map kaafi nahi ("ab"->"aa" clash sirf ULTI-taraf pakadta).
CLASH: mp.count(key)==1 && mp[key]!=val. dono taraf check, || (kisi ek taraf bhi clash -> false).
TEMPLATE:
    for(i=0..n-1){
        if( (mp1.count(s[i]) && mp1[s[i]]!=t[i]) || (mp2.count(t[i]) && mp2[t[i]]!=s[i]) )   // count() pehle: seedha mp1[x] khaali key bana ke galat clash deta
            return false;   // kisi bhi taraf clash = mapping 1-to-1 nahi
        mp1[s[i]]=t[i];  mp2[t[i]]=s[i];   // check ke BAAD dono taraf mapping save
    return true;   // poora chal gaya bina clash = isomorphic
```

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

### ▸ WORD PATTERN

```
Ek pattern (akshar) aur ek string s (space se alag words) di hai. Kya pattern ka har akshar hamesha ek hi word se juda hai, aur har word hamesha ek hi akshar se?
Isomorphic jaisa hi, bas akshar ki jagah word. true ya false lautao.
  e.g. "abba", "dog cat cat dog" -> true
  kyun: a->dog, b->cat; a b b a = dog cat cat dog, mapping kahin nahi tooti
```

```
PURANI SHEET SE (same to same):

= ISOMORPHIC ka SAME 2-map clash-check. REUSE: wahi mp1/mp2 || check, bas char <-> WORD.
  farak SIRF 3 delta:
      (1) INPUT: char[] ki jagah word-stream -> stringstream ss(s); while(ss>>word).
      (2) KEY  : pattern[i] <-> word  (char<->char nahi).
      (3) GUARD: end me pattern.size()==j (word-count) -> warna false ("a","dog cat" galat-true se bache).
TEMPLATE:
    stringstream ss(s); string word; int i=0, j=0;   // i = pattern ka index, j = words ki ginti
            if( (mp1.count(pattern[i]) && mp1[pattern[i]]!=word) || (mp2.count(word) && mp2[word]!=pattern[i]) )
                return false;                       // <- ISOMORPHIC ki EXACT clash-line, bas char->word
            mp1[pattern[i]]=word; mp2[word]=pattern[i]; i++;   // check ke BAAD save, phir agla pattern char
        j++;   // if ke BAHAR: har word gino, extra word bhi (end check ke liye)
    return pattern.size()==j;   // words ki ginti == pattern ki length, warna false
EDGE: words ki ginti (j) == pattern.size() honi chahiye (warna "a","dog cat" galat true). end me pattern.size()==j -> true.
```

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

── (STANDALONE — apni alag trick) ──
```

### ▸ TWO SUM

```
Ek array aur ek target diya hai. Wo do index lautao jin pe rakhe numbers ka jod target ke barabar ho.
Jawab hamesha ek hi hai, aur ek hi number do baar nahi le sakte.
  e.g. [2,7,11,15], target=9 -> [0,1]
  kyun: nums[0] + nums[1] = 2 + 7 = 9
```

```
PURANI SHEET SE (same to same):

map me {value -> index}. har num pe dekho: (target - num) map me HAI? -> haan to jodi mil gayi (dono index).
ek pass: pehle CHECK phir daalo (taaki num khud se na match kare).   // mp.find(target-num) se check · mila -> return {mp[target-num], i}
```

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

### ▸ VALID ANAGRAM

```
Do string s aur t di hain. Kya t, s ka anagram hai, matlab dono me same akshar utni hi baar hain, bas kram alag ho sakta hai? true ya false lautao.
  e.g. "anagram", "nagaram" -> true
  kyun: dono me a=3, n=1, g=1, r=1, m=1
```

```
PURANI SHEET SE (same to same):

dono me har char ka count barabar? count-array[26] (ya map): string-1 se ++, string-2 se -- -> sab ZERO -> anagram.
(length alag -> turant false.)   // end me koi count != 0 -> false (sirf >0 nahi, -ve bhi aata)
```

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

### ▸ GROUP ANAGRAMS

```
Strings ka ek array diya hai. Jo strings aapas me anagram hain (same akshar, kram alag), unhe ek group me daalo. Saare group lautao.
  e.g. ["eat","tea","tan","ate","nat","bat"] -> [eat,tea,ate] [tan,nat] [bat]
  kyun: sorted key: eat/tea/ate = "aet", tan/nat = "ant", bat = "abt" -> 3 key = 3 group

"eat","tea","ate" -> teeno ka SORTED roop "aet" = same. Yahi KEY.
brute = har jodi "anagram hai?" O(n² · k) -> har word ki KEY bana ke map me CACHE -> O(n · k log k)
```

```
PURANI SHEET SE (same to same):

DERIVE: brute = har jodi check "anagram hai?" O(n² · k) -> baar-baar wahi letters gin rahe
        -> har word ki ek pehchaan (KEY) bana ke map me CACHE -> O(n · k log k).
DS = unordered_map<string, vector<string>>   ·   KEY = sorted(word)

  for (auto &w : strs) {
      string key = w;   // copy lo: asli word sort hua to group me galat word jaata
      sort(key.begin(), key.end());      // "tea" -> "aet"
      mp[key].push_back(w);              // same key = same group
  for (auto &p : mp) ans.push_back(p.second);   // map ki har list = ek group

★ KEY ka doosra roop (sirf a-z ho to): 26 ki ginti  "1#0#0#...#"  -> sort nahi, O(n · k).
  (separator zaroori: bina "#" ke a=1,b=11 aur a=11,b=1 dono "111" ban jaate)
★ "" (khaali string) bhi chalta: sorted("") = "" -> ek hi group.

PEHLI SHEET (Aug) se bhi, same to same:

anagrams ka SORTED-roop same hota -> har word SORT karo = KEY -> map<sortedKey, list<word>> me daalo -> lists hi groups.
```

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

### ▸ SUBARRAY SUM = K

```
Ek array aur ek number k diya hai. Kitne lagaatar subarray hain jinka jod theek k ke barabar ho, wo ginti lautao.
Array me minus number bhi ho sakte hain.
  e.g. [1,2,3], k=3 -> 2
  kyun: [1,2] = 3 aur [3] = 3; baaki [1],[2],[2,3],[1,2,3] nahi
```

```
PURANI SHEET SE (same to same):

prefix-sum + hashmap {prefixSum -> count}. running sum chalao; count += mp[sum - k], phir mp[sum]++.
mp[0]=1 se START zaroori (jab sum khud == k ho, tab prefix-0 chahiye). idea: pichla prefix jahan se ab tak k bana.
★ mp[sum - k], k - sum NAHI. ODOMETER: abhi 15 km (sum) - aakhri tukda 5 (k) = 10 km pe tukda shuru -> "10 kitni baar dikha?"
  BADA (jahan hoon) pehle. Two Sum ka "target - x" ulta yahan mat lagao: x + y = target  vs  sum - pichhla = k.
```

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

### ▸ COUNT DIVISIBLE PAIRS  = two-sum/hashmap-complement, par KEY = REMAINDER

```
Ek array aur ek number k diya hai. Kitne jode (i < j) hain jinka jod k se poora kat jaaye, matlab (nums[i] + nums[j]) % k == 0. Wo ginti lautao.
  e.g. [1,2,3,4,5], k=3 -> 4
  kyun: 1+2=3, 1+5=6, 2+4=6, 4+5=9 -> chaaron 3 se divide; baaki pair nahi

pairs (i<j): (A[i]+A[j])%k==0. brute O(n²) -> "partner pehle aaya?" ko hashmap me CACHE -> O(n)
A[i] KYUN, prefix NAHI: pair = 2 ALAG element (contiguous range nahi) -> har element ka apna rem
```

```
PURANI SHEET SE (same to same):

count pairs (i<j): (A[i]+A[j])%k==0.
DERIVE: brute O(n²) har pair -> "partner pehle aaya?" baar-baar -> partner ko hashmap me CACHE -> O(n).
★ A[i] KYUN, prefix NAHI: pair = 2 ALAG element (contiguous range nahi) -> har element ka apna remainder.
rem = ((A[i]%k)+k)%k;   count += mp[(k-rem)%k];   mp[rem]++;   // +k phir %k: C++ me negative ka % negative aata, key galat
   partner = (k-rem)%k · lookup PEHLE phir insert (warna khud-se-pair).

 ★ k-rem ka INTUITION (ek element se, concrete):
   maan it=1, k=3.   pehle remainder: 1%3 = 1.
   chahiye: (1 + partner) % 3 == 0.   to partner ka remainder = 3 - 1 = 2.
   ab MAP me dekho: koi element pehle aaya jiska remainder 2 tha?
        mp[2] jitna -> utne pair bane (har aisa element 1 ke saath jodi banata).
   yehi hai partner = (k - rem) = 3 - 1 = 2.
   (rem=0 aaya -> partner = 3-0 = 3, par 3%3 = 0 -> partner 0.  isiliye (k-rem)%k.)

── DRY-RUN (nums=[1,2,3,4,5], k=3  ->  4 pairs) ──
 rem = ((it)%k+k)%k :   1->1   2->2   3->0   4->1   5->2

 it  rem  partner=(k-rem)%k   mp[partner]   count    mp-after
  1   1        2                  0           0       {1:1}
  2   2        1                  1          +1=1     {1:1, 2:1}      <- pair (1,2)
  3   0        0                  0           0       {1:1, 2:1, 0:1}
  4   1        2                  1          +1=2     {1:2, 2:1, 0:1} <- pair (2,4)
  5   2        1                  2          +2=4     {1:2, 2:2, 0:1} <- pairs (1,5),(4,5)
 ->  count = 4

 KEY: it=5 pe mp[1]=2 (element 1 aur 4 dono ka rem 1) -> ek saath 2 pair jude.
 LOOKUP pehle, INSERT baad (khud-se-pair na gine).

PEHLI SHEET (Aug) se bhi, same to same:

count pairs (i<j): (A[i]+A[j])%k==0.
DERIVE: brute O(n²) har pair -> "partner pehle aaya?" baar-baar -> partner ko hashmap me CACHE -> O(n).
★ A[i] KYUN, prefix NAHI: pair = 2 ALAG element (contiguous range nahi) -> har element ka apna remainder.
sum = ((A[i]%k)+k)%k;   count += mp[(k-sum)%k];   mp[sum]++;
   partner = (k-sum)%k · lookup PEHLE phir insert (warna khud-se-pair).
```

```cpp
int countDivisiblePairs(vector<int> &nums, int k) {
    unordered_map<int, int> mp;             // {remainder -> kitne aaye}
    int count = 0;
    for (auto &it : nums) {
        int rem = (((it % k) + k) % k);     // ★ +k phir %k: C++ me negative ka % negative aata, key galat
        count += mp[(k - rem) % k];         // ★ partner rem: it=1,k=3 -> 1+? = 3 -> 2. rem=0 -> 3%3 = 0
                                            //   isliye (k-rem)%k. mp[2] jitne = utne pair
        mp[rem]++;                          // lookup PEHLE, insert BAAD: khud-se-pair na gine
    }
    return count;                           // [1,2,3,4,5],k=3 -> 4 (it=5 pe mp[1]=2 -> ek saath 2 pair)
}
```

### ▸ LONGEST CONSECUTIVE SEQ (LC-128)

```
Ek bina sort kiya hua array diya hai. Usme lagaatar numbers ki sabse lambi chain kitni lambi hai (jaise 1, 2, 3, 4), wo lautao.
Sort nahi karna, O(n) me karna hai.
  e.g. [100,4,200,1,3,2] -> 4
  kyun: 1,2,3,4 sab maujood = chain 4; 100 aur 200 akele

unsorted -> sabse LAMBI consecutive chain ki LENGTH. [100,4,200,1,3,2] -> 4 (1,2,3,4)
sort (n log n) mat karo -> SET me O(1) lookup, har CHAIN sirf uske START se gino -> O(n)
```

```
PURANI SHEET SE (same to same):

INPUT: [100,4,200,1,3,2] -> 4   (1,2,3,4 = chain; 100/200 akele)

SAAR: sort(O(nlogn)) mat karo -> SET me daal ke O(1) lookup. har CHAIN sirf uske START se gino.
KEY: num START hai TAB HI jab (num-1) set me NAHI -> warna wo kisi chain ke BEECH ka -> skip.
     -> har element zindagi me 1 hi baar visit -> O(n)  (warna har num se ginti = O(n^2)).

TEMPLATE:
  unordered_set<int> st(nums.begin(), nums.end());   // set: O(1) lookup + duplicate apne aap hat jaate
  int ans = 0;   // 0 se: khaali array pe koi chain nahi -> 0
  for (auto &num : st) {     // set pe ghoomo (nums pe = duplicate start baar-baar -> O(n^2))
     if (!st.count(num-1)) {                 // chain ka START
        int currNum = num, count = 1;   // count=1: START wala number khud chain me
        while (st.count(currNum+1)) { currNum++; count++; }   // agla number set me hai tab tak chain lambi karo
  return ans;                                // khali array -> 0

DRY-RUN [100,4,200,1,3,2]  set={1,2,3,4,100,200}:
  100 -> 99 nahi -> START -> 101? nahi        -> count=1
  4   -> 3 HAI   -> beech ka -> skip
  200 -> 199 nahi-> START -> 201? nahi         -> count=1
  1   -> 0 nahi  -> START -> 2,3,4 hai         -> count=4   <= ans=4
  3,2 -> (num-1) HAI -> skip
  => ans = 4

FAMILY: hashing/SET (O(1) lookup se sort bachao). trick = "har group sirf START se process".

PEHLI SHEET (Aug) se bhi, same to same:

TEMPLATE:
unordered_set<int> st(nums.begin(), nums.end());
int ans = 0;
for (auto &num : nums) {
   if (!st.count(num-1)) {                 // chain ka START
      int currNum = num, count = 1;
      while (st.count(currNum+1)) { currNum++; count++; }
      ans = max(ans, count);
   }
}
return ans;                                // khali array -> 0
```

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

### ▸ MAJORITY ELEMENT (LC-169, >n/2)

```
Ek array me ek number aadhe se zyada baar (n/2 se zyada) aata hai, aur aisa number hamesha hota hai. Wo number lautao.
  e.g. [2,2,1,1,1,2,2] -> 2
  kyun: n = 7, 2 aaya 4 baar > 7/2 = 3

map se: mp[x]++, count > n/2 wala return -> O(n) time, O(n) space (seedha)
```

```
PURANI SHEET SE (same to same):

map se: mp[x]++; count > n/2 wala return. O(n) time, O(n) space. (seedha.)
BOYER-MOORE VOTING (O(1) space): majority n/2 se zyada -> baaki sab milkar bhi kam -> do ALAG element CANCEL karo, majority bachega.
  candidate + count. ==candidate -> count++ · !=candidate -> count-- · count==0 -> candidate=nums[i], count=1 (0 chhoda to -1 me chala jaayega, reset kabhi nahi -> BUG). end: candidate = ans.
```

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

### ▸ FIND DISAPPEARED NUMBERS (LC-448)  = ARRAY-KHUD-hashmap · index-negation, O(1) space

```
n size ka array hai, har value 1 se n ke beech hai aur kuch number dohraye gaye hain. 1 se n tak ke jo number array me nahi hain, wo sab lautao.
Extra jagah O(1) honi chahiye.
  e.g. [4,3,2,7,8,2,3,1] -> [5,6]
  kyun: n = 8, maujood 1,2,3,4,7,8; 5 aur 6 kahin nahi

size n, values 1..n (repeat ho sakte). jo MISSING wo sab. [4,3,2,7,8,2,3,1] -> [5,6]
extra set/map nahi: value v ka ghar = index v-1 -> dikha to us index ko NEGATIVE mark
```

```
PURANI SHEET SE (same to same):

INPUT: [4,3,2,7,8,2,3,1] -> [5,6]

SAAR: extra set/map nahi -> ARRAY KHUD ko hashmap banao. "value v dikha" ka nishaan = index (v-1) ko NEGATIVE.
KEY: value v -> uska ghar = index v-1. jo value dikhi us index ko -ve mark. end me jo index abhi bhi +ve
     -> us index+1 kabhi mark nahi hua -> MISSING. (abs() lo kyunki value pehle -ve ho chuki ho sakti.)

TEMPLATE:
  for (int i=0;i<n;i++){ int idx = abs(nums[i])-1; if (nums[idx]>0) nums[idx] = -nums[idx]; }   // >0 check: duplicate pe dobara minus = wapas plus
  for (int i=0;i<n;i++) if (nums[i]>0) ans.push_back(i+1);   // i+1: index i ka ghar value i+1 ka (1-based)

DRY-RUN [4,3,2,7,8,2,3,1]: mark index 3,2,1,6,7,1(already),2(already),0 -ve
  -> +ve bache index 4,5 -> missing = 5,6.
FAMILY: array-as-hashmap (index-marking). SAME trick: find-duplicate(287, sirf jab array badal sakte; asli 287 = Floyd)/first-missing-positive(41)/single-in-pairs.
★ CONNECT: "values 1..n + O(1) space" = index-marking ka signal. (agar array modify NA kar sakta -> Floyd cycle.)
```

```cpp
vector<int> findDisappearedNumbers(vector<int> &nums) {
    vector<int> ans;
    for (int i = 0; i < nums.size(); i++) {
        int index = abs(nums[i]) - 1;       // ★ abs(): value pehle hi -ve ho chuki ho sakti
        if (nums[index] > 0)                // ★ >0 check: duplicate pe dobara minus = wapas plus
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

#### ── FAMILY: PER-KEY STREAK (log line todo -> har KEY ki apni chalti ginti; Wise OA 27-Oct) ──

### ▸ SERVER ERROR STREAK  (03/11)

```
Logs ki lines di hain, har line "<server> <code> <msg>". Kisi server ke lagaatar 3 "error" aayein to ek alert gino aur us server ki ginti 0 kar do.
Beech me "success" aaya to us server ki ginti 0. Kul kitne alert hue, wo lautao.
  e.g. ["s1 500 error","s2 200 success","s1 500 error","s3 500 error","s1 500 error"] -> 1
  kyun: s1 ke 3 error (s2, s3 beech me aaye par s1 ki ginti nahi todte) -> 1 alert; s3 ka sirf 1 error

logs "s1 500 error", "s2 200 success" ... kisi server ke LAGAATAAR 3 error -> alert++, ginti 0.
pench: beech me DOOSRE server ki lines aati -> ek window nahi, "KISKA lagaataar" = key = server   (upar NAAM-SE-CONFUSE E)
```

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

### ▸ CSV MARKS TOTAL  (03/13)

```
Lines di hain, har line "naam,subject,marks". Har student ke kul marks nikaalo, aur naam ke alphabet kram me lautao.
  e.g. ["ravi,maths,40","priya,maths,55","ravi,sci,35","priya,sci,30"] -> priya 85, ravi 75
  kyun: ravi 40 + 35 = 75, priya 55 + 30 = 85

line todo -> naam = key -> map me marks jodte jao. pench: tukde COMMA se, space se nahi -> >> nahi chalega, getline
```

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

### ▸ SHIFT MINUTES  (03/14, HH:MM)

```
Guard ki shift ka start aur end "HH:MM" (24 ghante ki ghadi) me diya hai. Shift raat 12 ke paar bhi ja sakti hai. Shift kitne minute ki hai, wo lautao.
  e.g. "22:30", "01:15" -> 165
  kyun: 22:30 se 24:00 = 90 minute, 00:00 se 01:15 = 75 minute, 90 + 75 = 165

dono time ko MINUTE me badlo, phir ghatao. ghante aur minute alag ghatao to MM ka udhaar lena padta, ulajh jaata.
pench: raat 12 ke paar end chhota -> minus aata -> ek poora din (1440) jodo.

KYUN 1440: din me 24 * 60 = 1440 minute.
  minus aaya:   75 - 1350 = -1275 -> +1440 = 165                  (raat 12 paar)
  minus nahi:   1050 - 540 = 510 -> +1440 = 1950 -> %1440 = 510   (% se extra din wapas hata)
isliye "+1440 phir %1440" dono case ek line me. Caesar ka "+26 phir %26" bilkul yahi.
```

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

