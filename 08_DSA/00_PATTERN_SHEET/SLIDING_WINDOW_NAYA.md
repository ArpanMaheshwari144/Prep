# SLIDING WINDOW (naya saancha, sample)

Ye `PATTERN_SHEET.md` ka Pattern 2 hai, naye tareeke se. Purani sheet waisi hi hai, isme kuch nahi chheda.
Har sawaal ka code wahi hai jo tune likha tha. "Galti pakdo" wale saare input tere code pe chala ke dekhe hain.

---

## Pehle pehchaan

**Kab SW:** lagaatar (contiguous) subarray ya substring chahiye, aur "sabse lamba / sabse chhota / kitne" poocha gaya hai.

**Soch:** har subarray ko shuru se jaanchne ki jagah, pichhli window me ek element jodo (j) ya ek nikaalo (i).

**Lagane se pehle ek sawaal:** element jodne ya nikaalne pe kya pakka pata chalta hai ki window theek hui ya kharab? Agar haan, to SW chalegi.
- Isi wajah se **sum wali SW negative numbers pe nahi chalti.** Negative jodne pe sum ghat jaata hai, to "sum bada ho gaya, ab chhota karo" wala faisla bharosemand nahi rehta. Wahan prefix sum + map lagta hai.
- Char ginti, distinct ginti ya zero ginti wali SW pe negative ka koi lena-dena nahi hai.

**Milte-julte se pehle:** "lagaatar 3 error" jaisa sawaal, jahan har server / user ka alag hisaab hai, wo SW nahi, map hai (NAAM-SE-CONFUSE E).

---

## Ek dhaancha (sab sawaal isi pe)

```cpp
int i = 0, j = 0;
while (j < n) {
    arr[j] window me JODO;              // kya ginna hai, wo har sawaal me badalta
    while (WINDOW KHARAB) {             // size fix ho to IF, condition pe ho to WHILE
        arr[i] NIKAALO; i++;            // nikaalo PEHLE, phir i++
    }
    ans UPDATE;                         // length -> max(ans, j-i+1) · ginti -> count += j-i+1
    j++;                                // ans ke BAAD j++, warna j-i+1 galat
}
```

Har sawaal me sirf teen cheez badalti hain: **kya ginna hai**, **kab nikaalna hai**, aur **ans kab lena hai**. Neeche har family me sirf yahi farak dikhaya hai.

---

## Family 1: SABSE LAMBA (kharab hote hi chhota karo, ans = max)

Yahan window ko majboori me chhota karte hain, jab wo kharab ho jaaye. Chhota karne ke **baad** window theek hai, isliye ans wahin lete hain.

| Sawaal | Kya ginna | Kab nikaalo | e.g. |
|---|---|---|---|
| LC-3 no repeat | har char ki ginti (map) | jo abhi aaya wo 2 baar ho gaya | "abcabcbb" -> 3 |
| LC-424 char replacement | ginti + sabse zyada wale ki ginti (maxFreq) | length - maxFreq > k | "AABABBA", k=1 -> 4 |
| LC-1004 ones III | sirf zero kitne | zero > k | [1,1,1,0,0,0,1,1,1,1,0], k=2 -> 6 |
| Fruit baskets | alag type kitne (map.size) | type > 2 | [1,2,3,2,2] -> 4 |
| At most k distinct | alag char kitne (map.size) | alag > k | "eceba", k=2 -> 3 |
| LC-1208 budget | kul cost | cost > maxCost | "abcd", "bcdf", 3 -> 3 |

**Teen galti jo is family me hoti hain (sab chala ke dekhi):**
- **`if` ki jagah `while`:** `"abba"` me doosra `b` aata hai to pehla `b` index 1 pe hai, isliye `a` aur `b` dono nikaalne padte hain. `if` sirf ek (`a`) nikaalta hai, aur window "bb" me repeat reh jaata hai. `while` ke saath jawab 2 aata hai, `if` ke saath 3, jo galat hai.
- **ans bina shart ke lo:** ones III me `if (zero == k)` laga ke ans liya to `[1,1,0], k=2` pe 0 aata hai, jabki sahi jawab 3 hai. Chhota karne ke baad window pehle se theek hai, isliye ans seedha lo.
- **map me 0 hote hi erase:** fruit me ginti 0 hone pe key nahi hatayi to `map.size()` jhooth bolta hai. `[0,1,2,2]` pe sahi jawab 3 hai, bina erase ke 2 aata hai.

**LC-424 ki ek khaas baat:** shrink karte waqt maxFreq ko kabhi ghatao mat. Window apni sabse achhi length se chhoti nahi hoti, isliye purana maxFreq bhi theek kaam karta hai.

<details><summary>poora code (6 sawaal)</summary>

```cpp
int lengthOfLongestSubstring(string s) {   // LC-3
    if (s.empty())
        return 0;
    int ans = INT_MIN;
    unordered_map<char, int> mp;
    int i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;
        while (mp[s[j]] > 1) {              // jo abhi aaya wahi repeat
            mp[s[i]]--;
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}

int characterReplacement(string s, int k) {   // LC-424
    unordered_map<char, int> mp;
    int maxLen = INT_MIN, maxFreq = INT_MIN, i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;
        maxFreq = max(maxFreq, mp[s[j]]);   // update PEHLE, phir check
        while (j - i + 1 - maxFreq > k) {   // kitne badalne padenge > k
            mp[s[i]]--;
            if (mp[s[i]] == 0)
                mp.erase(s[i]);
            i++;
        }                                   // maxFreq kabhi GHATAO mat
        if (j - i + 1 - maxFreq <= k)
            maxLen = max(maxLen, j - i + 1);
        j++;
    }
    return maxLen;
}

int longestOnes(vector<int> &nums, int k) {   // LC-1004
    int ans = INT_MIN;
    int zeroCount = 0;
    int i = 0, j = 0;
    while (j < nums.size()) {
        if (nums[j] == 0)
            zeroCount++;
        while (zeroCount > k) {
            if (nums[i] == 0)
                zeroCount--;                // sirf zero nikla tabhi kam
            i++;
        }
        ans = max(ans, j - i + 1);          // bina shart, if (==k) MAT
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}

int totalFruit(vector<int> &fruits) {       // fruit = at most 2 distinct
    int ans = INT_MIN;
    unordered_map<int, int> mp;
    int i = 0, j = 0;
    while (j < fruits.size()) {
        mp[fruits[j]]++;
        while (mp.size() > 2) {
            mp[fruits[i]]--;
            if (mp[fruits[i]] == 0)
                mp.erase(fruits[i]);        // erase-on-0, warna size jhooth
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}

int totalFruit(string &s, int k) {          // at most k distinct: fruit me "2" ki jagah "k"
    int ans = INT_MIN;
    unordered_map<int, int> mp;
    int i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;
        while (mp.size() > k) {
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

int equalSubstring(string s, string t, int maxCost) {   // LC-1208, map nahi, sirf cost
    int i = 0, j = 0, cost = 0, ans = INT_MIN;
    while (j < t.size()) {
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
</details>

---

## Family 2: SABSE CHHOTA (theek rahe tab tak chhota karo, ans = min)

Family 1 ka ulta hai. Yahan window jaise hi theek hoti hai, ans **usi waqt, loop ke andar** lete hain, aur phir use aur chhota karke dekhte hain ki shayad aur chhoti bhi theek ho.

| | Sabse lamba (Family 1) | Sabse chhota (yahan) |
|---|---|---|
| Andar wala while kab chalta | window kharab hai | window theek hai |
| ans kab lete | while ke baad | while ke andar, nikaalne se pehle |
| ans shuru | INT_MIN, max se badhta | INT_MAX, min se ghatta |

**LC-209 min subarray len:** target=7, [2,3,1,2,4,3] -> 2 ([4,3]).
**Galti pakdo:** target=11, [1,1,1] -> 0. Koi window theek nahi bani, size INT_MAX hi raha, isliye end me `== INT_MAX ? 0`.

Min window (LC-76) bhi isi family ka hai, par wo need-map wala hai, isliye Family 5 me hai.

<details><summary>poora code</summary>

```cpp
int minSubArrayLen(int target, vector<int> &nums) {
    int sum = 0;
    int size = INT_MAX;
    int i = 0, j = 0;
    while (j < nums.size()) {
        sum += nums[j];
        while (sum >= target) {             // THEEK hote hi record + aur chhota karo
            size = min(size, j - i + 1);    // record ANDAR
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return size == INT_MAX ? 0 : size;
}
```
</details>

---

## Family 3: FIXED size (k pehle se diya)

Size pehle se tay hai, isliye har kadam pe ek element judta hai aur ek nikalta hai. Andar `while` ki zaroorat nahi, `if` kaafi hai.

| Sawaal | Kya ginna | Kya badla |
|---|---|---|
| Max sum of k | sum | size k hote hi record, phir ek nikaalo |
| LC-1423 cards | sum | ulta socho: jo NAHI liye wo beech ki lagaatar window (size n-k) hai. Total - us window ka MIN |
| LC-219 duplicate within k | set | check PEHLE, phir size > k pe purana nikaalo, phir daalo |

**Galti pakdo:**
- Max sum: `[-1,-2,-3], k=2` -> -3. `maxSum` ko 0 se shuru kiya to jawab 0 aata hai, jo galat hai. Isliye INT_MIN se shuru karte hain.
- Cards: `k == n` ho to window size 0 hai, seedha Total lautao.
- Duplicate: `[4,4], k=0` -> false. Isliye `k <= 0` pe pehle hi false. Aur set me daalna check ke **baad** karo, warna number khud se hi match ho jaata hai.

<details><summary>poora code (3 sawaal)</summary>

```cpp
int maxSumK(vector<int> &nums, int k) {
    int sum = 0;
    int maxSum = INT_MIN;                   // sab negative ho to bhi sahi
    int i = 0, j = 0;
    while (j < nums.size()) {
        sum += nums[j];
        if (j - i + 1 >= k) {               // IF: size fix
            maxSum = max(maxSum, sum);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return maxSum;
}

int minSumK(vector<int> &nums, int k) {     // max_sum_k ka MIN version
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

int maxScore(vector<int> &nums, int k) {    // LC-1423
    int Total = 0;
    for (int i = 0; i < nums.size(); i++)
        Total += nums[i];
    if (nums.size() - k == 0)
        return Total;
    int minSum = minSumK(nums, nums.size() - k);
    return Total - minSum;
}

bool containsNearbyDuplicate(vector<int> &nums, int k) {   // LC-219
    if (k <= 0)
        return false;
    int i = 0, j = 0;
    unordered_set<int> st;
    while (j < nums.size()) {
        if (st.count(nums[j]) > 0)
            return true;                    // check PEHLE
        if (j - i + 1 > k) {
            st.erase(nums[i]);
            i++;
        }
        st.insert(nums[j]);                 // check ke BAAD daalo
        j++;
    }
    return false;
}
```
</details>

---

## Family 4: KITNE (ginti, length nahi)

Dhaancha Family 1 wala hi hai. Farak bas itna hai ki ans me `max` ki jagah `count += j - i + 1` aata hai, kyunki j pe khatam hone wale saare theek subarray ginte hain: [j], [j-1..j], ..., [i..j].

| Sawaal | Kya ginna | Kya badla |
|---|---|---|
| Product < k | guna (prod = 1 se shuru) | `while (prod >= k && i <= j)` |
| LC-992 exactly k distinct | map.size | seedha nahi banta. exactly(k) = atMost(k) - atMost(k-1), aur atMost = fruit ka code + count |

**Galti pakdo:**
- Product: `[1,1,1], k=1` -> 0. `i <= j` na ho to khaali window ka prod 1 rehta hai aur i bahar bhaagta hai. Aur prod ko 0 se shuru kiya to sab 0 ho jaata.
- Exactly k: `k = 1` pe koi alag case nahi chahiye, kyunki `atMost(0)` apne aap 0 aata hai. `[1,2,1,2,3], k=2` -> 12 - 5 = 7.

<details><summary>poora code (2 sawaal)</summary>

```cpp
int numSubarrayProductLessThanK(vector<int> &nums, int k) {
    if (k == 0)
        return 0;
    int count = 0;
    int i = 0, j = 0, prod = 1;             // guna 1 se shuru
    while (j < nums.size()) {
        prod *= nums[j];
        while (prod >= k && i <= j) {       // i <= j: k=1 pe bachav
            prod /= nums[i];
            i++;
        }
        count += j - i + 1;
        j++;
    }
    return count;
}

int atMostK(vector<int> &fruits, int k) {   // fruit ka code, ans = COUNT
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
    return atMostK(nums, k) - atMostK(nums, k - 1);
}
```
</details>

---

## Family 5: NEED-MAP (t ke saare char chahiye)

Pehle t ke har char ki zaroorat map me likh lo. `count` = abhi kitne char aur chahiye. Aaya to `--`, gaya to `++`. Dono taraf bilkul ek jaisa (mirror) hona chahiye.

| Sawaal | ans kab | e.g. |
|---|---|---|
| LC-76 min window | `while (count == 0)` ke ANDAR, nikaalne se pehle (sabse chhota = Family 2) | "ADOBECODEBANC", "ABC" -> "BANC" |
| LC-1358 teeno a b c | while ke BAAD `ans += i` | "abcabc" -> 10 |

**Galti pakdo:**
- `mp[s[j]]--` ko `if` ke bahar rakho. Andar rakha to t me na hone wale char ka hisaab bigad jaata hai, aur shrink pe `count++` galat ho jaata hai.
- Min window: `s="a", t="aa"` -> "". Koi theek window nahi bani, minLen INT_MAX hi raha.
- 1358: `"abc"` -> 1. Jab j=2 hota hai to while chal ke i=1 ho jaata hai, to ans += 1. Matlab start 0 se 0..2 wala ek substring.

<details><summary>poora code (2 sawaal)</summary>

```cpp
string minWindow(string s, string t) {      // LC-76
    unordered_map<char, int> mp;
    for (auto &it : t)
        mp[it]++;
    int i = 0, j = 0, count = t.size(), minLen = INT_MAX, index = 0;
    while (j < s.size()) {
        if (mp[s[j]] > 0)
            count--;
        mp[s[j]]--;                         // IF ke BAHAR
        while (count == 0) {
            int len = j - i + 1;
            if (len < minLen) {             // record PEHLE
                minLen = len;
                index = i;
            }
            mp[s[i]]++;                     // MIRROR
            if (mp[s[i]] > 0)
                count++;
            i++;
        }
        j++;
    }
    return minLen == INT_MAX ? "" : s.substr(index, minLen);
}

int minWindow(string s, string t) {         // LC-1358: wahi code, sirf ans ki line badli
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
        ans += i;                           // start 0..i-1 sab theek the
        j++;
    }
    return ans;
}

int numberOfSubstrings(string s) {
    return minWindow(s, "abc");
}
```
</details>

---

## Family 6: ANAGRAM (fixed size = p ki length)

Window ko p ki length pe rakho, aur har poori window pe `isAnagram` check karo.

| Sawaal | Kya badla |
|---|---|
| LC-438 find all anagrams | mila to `ans.push_back(i)` |
| LC-567 permutation in string | wahi code, push ki jagah `return true`. Dhyaan: `findAnagrams(s2, s1)`, ulta kram |

**Galti pakdo:** `j - i + 1 == p.size()` pe hi check karo. Shuru me window chhoti hoti hai, us pe check bekaar hai.

<details><summary>poora code (2 sawaal)</summary>

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

vector<int> findAnagrams(string s, string p) {   // LC-438
    vector<int> ans;
    int i = 0, j = 0;
    while (j < s.size()) {
        while (j - i + 1 > p.size())
            i++;
        if (j - i + 1 == p.size())
            if (isAnagram(s.substr(i, j - i + 1), p))
                ans.push_back(i);
        j++;
    }
    return ans;
}

bool findAnagrams(string s, string p) {     // LC-567 ke liye: push ki jagah return true
    int i = 0, j = 0;
    while (j < s.size()) {
        while (j - i + 1 > p.size())
            i++;
        if (j - i + 1 == p.size())
            if (isAnagram(s.substr(i, j - i + 1), p))
                return true;
        j++;
    }
    return false;
}

bool checkInclusion(string s1, string s2) {
    return findAnagrams(s2, s1);            // ulta kram
}
```
</details>

---

## Family 7: WINDOW KA MAX (deque)

**LC-239:** [1,3,-1,-3,5,3,6,7], k=3 -> [3,3,5,5,6,7].
Deque me **index** rakho, value nahi, taaki pata chale kaun window se bahar gaya. Front pe hamesha max hota hai.

Kram teen kadam ka hai, aur ulta kiya to galat aata hai:
1. Front ka index `i - k` ho gaya to wo window se bahar hai, nikaalo.
2. Peeche se apne se chhote (ya barabar) nikaalo, kyunki naya bada aa gaya to wo kabhi max nahi banenge. Min chahiye to shart ulti.
3. `i >= k - 1` ho to front ka value ans me.

**Galti pakdo:** `dq.front()` se pehle `!dq.empty()` check karo, khaali deque pe front crash karta hai.

<details><summary>poora code</summary>

```cpp
vector<int> maxSlidingWindow(vector<int> &nums, int k) {
    vector<int> ans;
    deque<int> dq;                          // INDEX, value ghatte kram me
    for (int i = 0; i < nums.size(); i++) {
        if (!dq.empty() && dq.front() == i - k)
            dq.pop_front();                 // 1) window se bahar
        while (!dq.empty() && nums[dq.back()] <= nums[i])
            dq.pop_back();                  // 2) chhote bekaar
        dq.push_back(i);
        if (i >= k - 1)
            ans.push_back(nums[dq.front()]);   // 3) record
    }
    return ans;
}
```
</details>

---

## Ek line me poora SW

Jodo, kharab ho to nikaalo, ans lo. Sabse lamba ho to ans while ke baad, sabse chhota ho to while ke andar, aur ginti ho to `count += j - i + 1`. Size pehle se fix ho to `if`, warna `while`.
