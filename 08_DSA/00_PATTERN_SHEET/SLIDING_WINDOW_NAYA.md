# SLIDING WINDOW (naya saancha, sample)

Purani `PATTERN_SHEET.md` waisi hi hai. Yahan code aur variable naam tere hi hain, sirf comment chhote kiye hain.
`★` sirf un lines pe hai jo kick hain, ya jinhe chhodne pe code toot-ta hai. Har `★` wala galat input tere code pe chala ke dekha hai.

**Kab SW:** lagaatar subarray / substring + sabse lamba / sabse chhota / kitne.
**Kab nahi:** variable window jisme SUM pe shrink ho (`sum > k` pe chhota karo), aur numbers negative hon. Negative jodne pe sum ghat jaata hai, to "bada ho gaya, chhota karo" wala faisla toot jaata hai. Wahan prefix + map lagta hai. Fixed size window (max sum of k) negative ke saath bhi chalti hai.

```
 dhaancha (lamba wale):  j se jodo -> kharab ho to while me i se nikaalo -> ans lo -> j++
 sabse chhota wale:      j se jodo -> THEEK rahe tab tak while me ans lo + i se nikaalo -> j++
 fixed size k:           while ki jagah IF (ek juda, ek nikla)
```

---

## LAMBA wale (kharab hote hi chhota karo, while ke BAAD ans = max)

### ▸ LC-3 NO REPEAT  
`"abcabcbb" -> 3`. Sabse lambi substring jisme koi char repeat na ho.

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

### ▸ LC-424 CHAR REPLACEMENT  
`"AABABBA", k=1 -> 4`. Max k char badal ke sabse lambi window jisme sab same.

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
`★ maxFreq ko shrink pe kabhi ghatao mat:` maxFreq ab tak ki sabse badi ginti hai, zaroori nahi ki abhi wali window ki ho. Purane maxFreq se ek kharab window thodi der tik sakti hai, par usse best galat bada nahi banta, isliye recompute ki zaroorat nahi. (Best bina maxFreq badhe bhi badhta hai: "AB", k=1 me maxFreq dono baar 1, best 1 -> 2.)

### ▸ LC-1004 MAX CONSECUTIVE ONES III  
`[1,1,1,0,0,0,1,1,1,1,0], k=2 -> 6`. Max k zero ko 1 bana ke sabse lambi 1 ki line.

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

### ▸ FRUIT INTO BASKETS  
`[1,2,3,2,2] -> 4`. 2 basket, har basket me ek type. Lagaatar pedon ki sabse lambi line.

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

### ▸ LONGEST AT-MOST-K DISTINCT  
`"eceba", k=2 -> 3`. Sabse lambi substring jisme zyada se zyada k alag char.

```cpp
int totalFruit(string &s, int k) {
    int ans = INT_MIN;
    unordered_map<int, int> mp;
    int i = 0, j = 0;
    while (j < s.size()) {
        mp[s[j]]++;
        while (mp.size() > k) {             // fruit me 2, yahan k
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

## CHHOTA wala (theek rahe tab tak chhota karo, ans while ke ANDAR = min)

### ▸ LC-209 MIN SUBARRAY LEN  
`target=7, [2,3,1,2,4,3] -> 2` ([4,3]). Positive numbers, sum >= target wala sabse chhota subarray.

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

## FIXED size k (while ki jagah IF)

### ▸ MAX SUM OF K  
`[2,1,5,1,3,2], k=3 -> 9`. Har size-k window ka sum, sabse bada.

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

### ▸ LC-1423 MAX POINTS FROM CARDS  
`[1,2,3,4,5,6,1], k=3 -> 12` (peeche se 5+6+1). Dono kinaron se kul k card lo, sum sabse bada.

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

### ▸ LC-219 CONTAINS DUPLICATE II  
`[1,2,3,1], k=3 -> true`. Do barabar number jinka index ka farak <= k.

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
`[1,2,1,2,3], k=2 -> 7`.

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
    return atMostK(nums, k) - atMostK(nums, k - 1);
}
```
`↳ SAME: atMostK = fruit ka code. Farak: max ki jagah count, aur upar exactly = atMost(k) - atMost(k-1).`

---

## NEED-MAP (t ke saare char chahiye)

### ▸ LC-76 MIN WINDOW SUBSTRING  
`s="ADOBECODEBANC", t="ABC" -> "BANC"`. s ki sabse chhoti window jisme t ke saare char.

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
`s="cbaebabacd", p="abc" -> [0,6]`. s me p ke saare anagram ki start index.

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

### ▸ LC-567 PERMUTATION IN STRING  
`s1="ab", s2="eidbaooo" -> true`.

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

## WINDOW KA MAX (deque)

### ▸ LC-239 SLIDING WINDOW MAXIMUM  
`[1,3,-1,-3,5,3,6,7], k=3 -> [3,3,5,5,6,7]`.

```cpp
vector<int> maxSlidingWindow(vector<int> &nums, int k) {
    vector<int> ans;
    // ★ INDEX rakho, value nahi: kaun window se bahar gaya ye index se pata chalta
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
