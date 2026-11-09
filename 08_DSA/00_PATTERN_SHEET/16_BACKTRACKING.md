## PATTERN 16 — BACKTRACKING (choose / explore / un-choose)


> [!IMPORTANT]
> **KAB:** "saare subsets / permutations / combinations", "N-queens", "har combination try karo".

> [!IMPORTANT]
> **BROAD IDEA:** saari possibilities explore -> solution EK-EK step build;
> galat lage -> PEECHE aao (backtrack) -> agla raasta.
>
> **MAZE:** raasta pakdo -> dead-end -> WAPAS last-junction -> doosra try. har element pe CHOICE(s).
>
> **DECISION TREE:** har node = choice-point; har leaf tak ka raasta = ek possible answer.

> [!IMPORTANT]
> **TEMPLATE (3 step):** CHOOSE (path me daalo) -> EXPLORE (aage recurse)
> -> UN-CHOOSE (pop / undo -> agli choice).

### 2 FORMS = SAME cheez, alag likhawat (dono me core: CHOOSE -> EXPLORE -> UN-CHOOSE):

```
   A) INCLUDE / EXCLUDE : 2 call (liya / nahi liya), record BASE (index == n) pe
   B) FOR-LOOP (start)  : loop se har element pe branch, record HAR node pe, alag base nahi
                          (index == n -> loop chalta hi nahi -> khud ruk jaata)
```

> [!WARNING]
> **GOTCHA 1 — EARLY RETURN 2ND BRANCH MAAR DETA:**
> ```
>     if (cond1) return solve(...);           // GALAT: function yahin exit -> neeche wali branch kabhi nahi
>     if (cond2) solve(...);
> ```
> // FIX: har branch bina return:  if (cond1) solve(...);  if (cond2) solve(...);
>
> // return HARMFUL sirf jab uske BAAD chalne wala code ho. LAST line pe return = safe.
>
> // BEECH me return = agla branch maar deta (void / non-void se lena-dena NAHI, FLOW ka rule).

> [!WARNING]
> **GOTCHA 2 — `else if` MAT LAGAO (dono branch ALAG if):**
> ```
>     if (cond1) solve(...);                  // sahi: dono chalti
>     if (cond2) solve(...);
> ```
> // else if = "ek mil gaya, bas" -> cond1 sach to cond2 KABHI nahi. backtracking = HAR combination banana.
>
> // dhoondhne wale code me else if theek (ek jawab). BANANE wale code me GALAT (saare jawab).

#### ── FAMILY: SUBSETS-family (start-aage = kaunse; used[] = kis order) ──

> [!IMPORTANT]
> **KAB:** "SAARE" subsets / permutations / combinations · grid me word dhoondo · N-queens

> [!WARNING]
> ★ DHYAAN: grid: visit mark -> explore -> UNMARK

---

### ▸ SUBSETS (LC-78)  = har element pe 2 choice (INCLUDE / EXCLUDE)

**SAWAAL**
```
Ek array diya hai (sab number alag). Iske saare subsets (khaali bhi) lautao, kram kuch bhi chalega.
  e.g. [1,2] -> [], [1], [2], [1,2]
  kyun: har element lo ya chhodo -> 2 x 2 = 4 subset
```

> [!IMPORTANT]
> **CHAABI:** har element pe 2 call: INCLUDE (`push`, `index + 1`) -> `pop` -> EXCLUDE (`index + 1`); base `index >= n` pe record. ALT: start-loop, HAR node record, alag base nahi.

> [!WARNING]
> `pop_back` EXCLUDE call se PEHLE, warna exclude path me bhi element chipka rehta. `temp` caller me EK baar banao, by-ref pass (helper me banaya to har call naya).

**CODE**
```cpp
void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp) {
    if (index >= nums.size()) {             // har element pe faisla ho gaya -> ek poora subset
        ans.push_back(temp);
        return;
    }
    temp.push_back(nums[index]);            // INCLUDE: le liya
    solve(nums, ans, index + 1, temp);      // index+1: agle element pe faisla
    temp.pop_back();  // ★ UN-CHOOSE, exclude se PEHLE: warna exclude me bhi chipka rehta
    solve(nums, ans, index + 1, temp);      // EXCLUDE: element chhod ke aage
}

vector<vector<int>> subsets(vector<int> &nums) {
    vector<vector<int>> ans;
    vector<int> temp;  // ★ BAHAR banao, by-ref pass: helper me banate to har call naya
    solve(nums, ans, 0, temp);              //   (ek SHARED path pe push / pop -> tabhi backtrack chalta)
    return ans;                             // 2^n subsets (har element in / out = power set)
}
```

```
── RECURSION TREE (nums=[1,2,3]) ── har edge 2 BAAR: ↓neeche=PUSH(choose) · ↑wapas=POP(backtrack)
                             [ ]                       <- solve(start=0)
            ┌─────────────────┼─────────────────┐
        ↓+1 ↑pop1        ↓+2 ↑pop2        ↓+3 ↑pop3
            │                 │                 │
           [1]               [2]               [3]      (har node = subset -> RECORD)
       ┌────┴────┐            │
   ↓+2 ↑pop2  ↓+3 ↑pop3   ↓+3 ↑pop3
       │         │            │
     [1,2]     [1,3]        [2,3]
       │
   ↓+3 ↑pop3
       │
    [1,2,3]  END (i=3, loop khatam)
```

```cpp
// ALT FORM (start-loop), same output:
void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp) {
    ans.push_back(temp);                    // HAR node = ek subset (alag base-case NAHI)
    for (int i = index; i < nums.size(); i++) {   // index se: peeche wale dobara nahi
        temp.push_back(nums[i]);            // CHOOSE
        solve(nums, ans, i + 1, temp);      // EXPLORE: i+1 -> agla aage se
        temp.pop_back();                    // UN-CHOOSE agle i se pehle: wahi jagah agle element ko mile
    }
}
// tree [1,2,3]: har edge pe neeche jaate PUSH, wapas aate POP. undo na karo -> galat subset
// ([1,2,3] -> pop3 -> [1,2] -> pop2 -> [1] -> loop aage +3 -> [1,3]). right side patli: node sirf aage
//   pe branch
```

---

### ▸ PERMUTATIONS (LC-46)  = used[] + FULL-LENGTH base (har position pe koi bhi bacha)

**SAWAAL**
```
Ek array diya hai (sab number alag). In numbers ko jitne alag-alag kram me laga sakte ho, wo saare kram (permutations) lautao.
  e.g. [1,2,3] -> [1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]
  kyun: pehli jagah 3 choice, doosri 2, teesri 1 -> 3x2x1 = 6
```

> [!IMPORTANT]
> **CHAABI:** loop HAMESHA `0` se + `used[]` (start nahi: order matter, peeche wala bhi kisi position pe); base `temp.size() == nums.size()` pe record.
>
> INDEX/start KYU NAHI PASS kiya (permutation me): order MATTER karta -> har position pe koi bhi UNUSED element (peeche wala bhi) chahiye -> loop HAMESHA 0-se -> "peeche mat jao" wali baat hi nahi -> isliye 'start' ki zaroorat nahi, uski jagah used[].
> (subsets/combinations me start = "peeche mat jao" (duplicate-rok, order-agnostic). permutation me wo nahi chahiye -> used[] = "same element dobara mat lo".)

> [!WARNING]
> un-choose me DONO undo: `temp.pop_back()` + `used[i] = false`. sirf pop kiya to aage ke arrangement me wo element band.

**CODE**
```cpp
void solve(vector<int> &nums, vector<int> &temp, vector<bool> &used, vector<vector<int>> &ans) {
    if (temp.size() == nums.size()) {       // base = full length (poora arrangement)
        ans.push_back(temp);
        return;
    }
    for (int i = 0; i < nums.size(); i++) {  // ★ 0 se, start NAHI: order matter, peeche wala bhi kisi
                                            //   position pe
        if (used[i])
            continue;                       // already liya -> skip
        temp.push_back(nums[i]);            // CHOOSE
        used[i] = true;
        solve(nums, temp, used, ans);       // EXPLORE (i+1 nahi -> phir 0 se, used skip karta)
        temp.pop_back();                    // UN-CHOOSE: DONO undo
        used[i] = false;                    // ★   used false bhi: warna aage ke arrangements me ye band
    }
}
// caller: temp + used(n, false) BAHAR bana ke by-ref -> solve(nums, temp, used, ans). n! permutations.
// SUBSETS = 'start' (sirf AAGE, [1,2]==[2,1] repeat nahi) · PERMUTATION = used[] + loop 0 se, start NAHI
```

---

### ▸ COMBINATIONS (LC-77)  = SUBSETS ka start-loop, bas base size==k

**SAWAAL**
```
Do number n aur k diye hain. 1 se n tak ke numbers me se k number chunne ke saare tareeke lautao. Kram matter nahi karta, [1,2] aur [2,1] ek hi hain.
  e.g. n=4, k=2 -> [1,2],[1,3],[1,4],[2,3],[2,4],[3,4]
  kyun: [2,1] = [1,2] same -> sirf aage wale chuno -> C(4,2) = 6
```

> [!IMPORTANT]
> **CHAABI:** subsets ka start-loop, bas base `temp.size() == k` pe record (har node pe nahi); recurse `i + 1` (aage hi, kram nahi).

> [!WARNING]
> loop `i <= n` (`n` khud bhi number) aur caller `index = 1` se (numbers 1..n, 0 nahi).

**CODE**
```cpp
void solve(int n, int k, vector<vector<int>> &ans, int index, vector<int> &temp) {
    if (temp.size() == k) {  // fixed-size combo poora (subsets har node pe, yahan sirf ==k)
        ans.push_back(temp);
        return;
    }
    for (int i = index; i <= n; i++) {     // 1..n numbers, index se aage (<= n: n khud bhi number)
        temp.push_back(i);                  // CHOOSE
        solve(n, k, ans, i + 1, temp);      // ★ EXPLORE (i+1 -> order nahi, aage hi)
        temp.pop_back();                    // UN-CHOOSE: agla i try karne se pehle path saaf
    }
}
// caller: solve(n, k, ans, 1, temp)  (1 se: numbers 1..n hain, 0 nahi)
// SUBSETS = start-aage, har node · COMBINATIONS = start-aage, size==k · PERM = used[] + loop 0, size==n
```

---

### ▸ COMBINATION SUM (LC-39)  = REUSE allowed + TARGET-base

**SAWAAL**
```
candidates[] (sab alag, sab positive) aur ek target diya hai. Wo saare alag combination lautao jinka jod target ke barabar ho. Ek number kitni bhi baar le sakte ho.
  e.g. [2,3,6,7], target=7 -> [[2,2,3],[7]]
  kyun: 2+2+3 = 7 (2 dobara liya) · 7 akela = 7
```

> [!IMPORTANT]
> **CHAABI:** combinations hi, 2 farak: recurse `i` (`i + 1` NAHI) = REUSE; base = target (`0` -> record, `< 0` -> prune), size nahi. include/exclude form me `index` SAME pass.
>
> FAMILY (5) 1-line: SUBSETS=start-aage,har-node · COMBINATIONS=start-aage,size==k · COMB-SUM=start-'i'(reuse),target-base · PERMUTATION=used[]+loop-0,size==n.

> [!WARNING]
> `i + 1` likh diya to 2 dobara nahi le sakte, `[2,2,3]` chhoot jaata. `target < 0` prune sabse upar, warna bekaar gehra jaata.

**CODE**
```cpp
void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp, int target) {
    if (target < 0)
        return;                             // overshoot: aage jaana bekaar (prune)
    if (index >= nums.size()) {             // end pe check
        if (target == 0)
            ans.push_back(temp);
        return;
    }
    temp.push_back(nums[index]);            // INCLUDE
    solve(nums, ans, index, temp, target - nums[index]);   // ★ index SAME: yahi element dobara le sakte
    temp.pop_back();                        // exclude se pehle undo: warna exclude path me bhi rehta
    solve(nums, ans, index + 1, temp, target);   // EXCLUDE: is candidate ko ab chhod diya
}

// ALT FORM (for-loop), same output:
void solve2(vector<int> &candidates, int target, vector<vector<int>> &ans, int index,
            vector<int> &temp) {
    if (target < 0)
        return;                             // prune
    if (target == 0) {                      // for-loop form: target==0 pe turant record
        ans.push_back(temp);
        return;
    }
    for (int i = index; i < candidates.size(); i++) {   // start se: peeche wale nahi, combo repeat nahi
        temp.push_back(candidates[i]);      // CHOOSE
        solve2(candidates, target - candidates[i], ans, i, temp);   // ★ 'i' (i+1 NAHI) -> REUSE allowed
        temp.pop_back();                    // UN-CHOOSE
    }
}
// combinations se FARAK: (1) recurse 'i' na i+1 (2) base = target (0 = record, <0 = prune), size==k nahi
// candidates positive -> target ghatta -> infinite nahi
// caller: solve(nums, ans, 0, temp, target)  ya  solve2(candidates, target, ans, 0, temp)
```


#### ── FAMILY: GRID PATH (element choose / skip nahi, grid pe raasta: mark -> recurse -> un-mark) ──

### ▸ WORD SEARCH (LC-79)  = grid-DFS + BACKTRACKING

**SAWAAL**
```
Aksharon ki ek grid aur ek word diya hai. Kya grid me paas-paas wale cells (upar, neeche, daayein, baayein) se chalte hue ye word ban sakta hai? Ek cell ek hi baar use hoga. true ya false lautao.
  e.g. board ABCE/SFCS/ADEE: "ABCCED" -> true, "ABCB" -> false
  kyun: A,B,C (row 0) -> C neeche -> E neeche -> D left · ABCB me wahi B dobara lagta
```

> [!WARNING]
> **★ BUG-TRAP:** loop me `if(solve(...)==false) return false;` -> PEHLA start fail hote hi poora false,
> baaki start dekhe hi nahi.  DRY-RUN "AAB":

### DRY-RUN "AAB"

```
         C A A        loop pehle (0,1) wala A -> paas me B nahi -> fail -> purana code yahin FALSE
         A A A        jabki (1,1)->(1,0)->(2,0) = A A B  -> start bacha hi nahi try karne ko
         B C D
```

> [!WARNING]
> + ulta bug: word[0] kahin na mila -> loop kuch nahi karta -> end me `return true` = jhootha "mila".

### purani atak (Aug sheet)

```
★ PURANI ATAK (Aug sheet): CRUX yahi atkaata tha: 4 DFS ko || me jodna (ek direction true -> baaki call hi nahi) + visited check bounds / mismatch ke saath EK hi if me.
```

> [!IMPORTANT]
> **ANY vs ALL (har jagah yahi):**
>
> "KOI EK bhi?" (any)  -> loop me: mila to return true   · loop ke BAAD: return false
>
> "SAB ke SAB?" (all)  -> loop me: fail to return false  · loop ke BAAD: return true
>
> (= stream ka anyMatch / allMatch, andar yahi loop)

> [!IMPORTANT]
> **CHAABI:** har cell pe ek `if`: bounds + `vis` + char mismatch -> false; mark -> 4 disha `||` (ek true = baaki call nahi) -> un-mark. pehla char mile wahin se start, KOI EK true = kaafi.

> [!WARNING]
> ANY wala loop: andar sirf true pe `return true`, loop ke BAAD `return false` (andar `== false` pe return = pehla start fail -> poora false). `vis[i][j] = false` un-mark zaroori, warna doosra raasta band.

**CODE**
```cpp
bool solve(vector<vector<char>> &board, const string &word, int index, int i, int j, int m, int n,
           vector<vector<bool>> &vis) {     // vis REFERENCE (&), copy nahi
    if (i < 0 || i >= m || j < 0 || j >= n || vis[i][j] || board[i][j] != word[index])
        return false;  // bounds PEHLE (warna board[i][j] bahar), visited bhi isi if me
    vis[i][j] = true;                       // mark PEHLE: is raaste me ye cell dobara na aaye
    index++;
    if (solve(board, word, index, i + 1, j, m, n, vis) ||   // || : ek true -> baaki call nahi
        solve(board, word, index, i, j + 1, m, n, vis) ||
        solve(board, word, index, i - 1, j, m, n, vis) ||
        solve(board, word, index, i, j - 1, m, n, vis))
        return true;
    vis[i][j] = false;                      // ★ BACKTRACK (un-mark): doosre raaste ke liye cell free
    return index == word.size() ? true : false;   // base: aakhri char match tabhi == size
}

bool exist(vector<vector<char>> &board, string word) {
    int m = board.size();
    int n = board[0].size();
    vector<vector<bool>> vis(m + 1, vector<bool>(n + 1, false));   // m x n kaafi (m+1 zaroori nahi)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == word[0]) {   // pehla char mile tabhi DFS
                if (solve(board, word, 0, i, j, m, n, vis) == true)
                    return true;            // KOI EK mila = kaafi
            }
        }
    }
    return false;                           // ★ SAARE start try, kahin nahi -> tab false
}
// BUG-TRAP: loop me `if (solve(..) == false) return false;` -> pehla start fail hote hi poora false.
//   "AAB" on [CAA / AAA / BCD]: (0,1) ka A fail, par (1,1)->(1,0)->(2,0) = AAB tha. + ulta bug: word[0]
//   kahin na mila aur end me `return true` = jhootha "mila"
// ANY vs ALL: KOI EK? -> loop me true pe return, BAAD me false · SAB? -> loop me false pe return, BAAD
//   me true
// islands-DFS se FARAK: islands = fill / count. word search = mark + recurse + UN-mark + return-bubble
```


#### ── FAMILY: CONSTRAINT / LEVEL choices (har step pe choice, rule ya level se) ──

### ▸ GENERATE PARENTHESES (LC-22)  = CONSTRAINT-guided (open / close count)

**SAWAAL**
```
Ek number n diya hai (n jode bracket). n jodon se banne wale saare sahi (balanced) bracket strings lautao.
  e.g. n=2 -> ["(())", "()()"]
  kyun: 2 '(' + 2 ')' ke sirf yahi 2 jahan ')' kabhi '(' se aage nahi nikalta ("())(" galat)

n jode ke saare VALID strings. brute (2^2n + filter) NAHI -> 2 RULE se hi banao
```

> [!WARNING]
> **★ BUG-TRAP #2 (Arpan-22-Sep, khud pakda):** dono ALAG `if` hain, `else if` NAHI --
> open<n aur close<open dono ek saath sach ho sakti hain, aur tab DONO chalni chahiye.
> `else if` = "ek mil gaya bas" = dhoondhne wali soch. Yahan BANANA hai, saare banane hain.
>
> **★ BUG-TRAP (Arpan-9-Sep, yaad rakh):** dono branch pe `return solve(...)` mat likho!
> first branch ke `return` se function KHATAM -> 2nd branch (')') kabhi chalti hi nahi -> sirf 1 string banti.
> backtracking = DONO choice ek-ke-baad-ek chalao (koi return nahi). = classic "early-return kills 2nd branch" trap.

> [!IMPORTANT]
> **CHAABI:** 2 rule hi branch: `open < n` -> `'('`, `close < open` -> `')'`; base `open == n && close == n`. path BY VALUE (`path + '('`) -> pop nahi chahiye.

> [!WARNING]
> dono ALAG `if` (`else if` nahi: dono sach ho sakti) aur `return solve(...)` NAHI (pehla return doosri branch maar deta -> sirf 1 string). n=3 pe 5 se kam = branch kat rahi.

**CODE**
```cpp
void solve(int open, int close, string path, vector<string> &ans, int n) {   // path BY VALUE: path+'('
    if (open == n && close == n) {          //   naya copy -> pop_back nahi chahiye
        ans.push_back(path);                // base: saare khule + band (= path.size() == 2*n)
        return;
    }
    if (open < n)                           // '(' : n se zyada '(' nahi
        solve(open + 1, close, path + '(', ans, n);
    if (close < open)                       // ★ ')' : band utne hi jitne khule. ALAG if, else if NAHI:
        solve(open, close + 1, path + ')', ans, n);   //   dono sach ho sakti, tab DONO chalni chahiye
}  // return solve(..) MAT: '(' ke return se ')' branch kabhi nahi
// caller: solve(0, 0, "", ans, n). n=3 -> 5, n=4 -> 14 (kam aaye = koi branch kat rahi)
```

---

### ▸ LETTER COMBINATIONS OF A PHONE NUMBER (LC-17)  = FOR-LOOP backtracking, har digit pe uske letters (index == n pe record)

**SAWAAL**
```
2 se 9 tak ke ankon ki ek string di hai. Phone keypad pe har ank ke kuch akshar hote hain (2 = abc, 3 = def ...). In ankon se banne wale saare shabd lautao. Khaali string pe khaali list.
  e.g. "23" -> [ad,ae,af,bd,be,bf,cd,ce,cf]
  kyun: 2 = abc, 3 = def -> har akshar ke saath har akshar = 3 x 3 = 9

"23" -> har ank ke akshar me se EK-EK. har LEVEL (index) = ek ank, us ke SAARE akshar try (for loop)
```

> [!WARNING]
> **★ BUG-TRAP (24-Sep):** `ans` ko GLOBAL mat rakho -- ek hi program me function do baar bula to
> pichhle jawab bhi jud jaate ("2" pe 3 ki jagah 12). LeetCode pe chal jaata (har test pe naya object),
> local test me pakda gaya. -> ans function ke andar, solve() me REFERENCE se.

> [!IMPORTANT]
> **CHAABI:** har level = ek ank, uske saare akshar for-loop me: `push` -> `solve(index + 1)` -> `pop`; `index >= digits.size()` pe record.

> [!WARNING]
> `digits == ""` pe `{}` (khaali LIST, `[""]` nahi). `ans` function ke andar banao, global pe do baar call = pichhle jawab jud jaate ("2" pe 12).

**CODE**
```cpp
void solve(int index, string &temp, unordered_map<char, string> &mp, vector<string> &ans,
           string &digits) {
    if (index >= digits.size()) {           // saare ank ho gaye
        ans.push_back(temp);
        return;
    }
    string str = mp[digits[index]];         // is ank ke akshar
    for (int i = 0; i < str.size(); i++) {  // har akshar ek alag branch
        temp.push_back(str[i]);             // choose
        solve(index + 1, temp, mp, ans, digits);   // explore (agla ank)
        temp.pop_back();                    // un-choose: agle akshar se pehle path saaf
    }
}

vector<string> letterCombinations(string digits) {
    if (digits.size() == 0)
        return {};                          // ★ EDGE: "" -> khaali LIST, [""] nahi
    unordered_map<char, string> mp;         // key = char ('2'): digits[index] char hai -> seedha chalta
    mp['2'] = "abc";  mp['3'] = "def";  mp['4'] = "ghi";  mp['5'] = "jkl";
    mp['6'] = "mno";  mp['7'] = "pqrs"; mp['8'] = "tuv";  mp['9'] = "wxyz";
    vector<string> ans;  // ★ ans GLOBAL mat rakho: function do baar bula to pichhle jawab
    string temp = "";                       //   bhi jud jaate ("2" pe 3 ki jagah 12)
    solve(0, temp, mp, ans, digits);
    return ans;
}
```


