## PATTERN 1 — TWO POINTER

```
 BROAD IDEA: do pointer (dono taraf se, ya slow-fast) -> ek saath move karke O(n).
 KAB: array SORTED hai aur do number ki jodi chahiye · ya array me IN-PLACE kuch hatana / aage karna hai
 ★ EDGE (sorted 2-ptr): duplicate skip karna hai kya? 3-sum HAAN, 2-sum nahi (bina skip ek triplet kai baar).
 ★ MILTE-JULTE (container / trapping / histogram): 00_SHURU me NAAM-SE-CONFUSE A dekh.
```


#### ── FAMILY: slow/fast OVERWRITE (fast scan, rakhne wala slow pe; teeno same skeleton) ──

### ▸ MOVE ZEROES

**SAWAAL**
```
Ek array diya hai. Saare 0 ko array ke end me bhejna hai, aur baaki numbers jis kram me the usi kram me rehne chahiye.
Naya array nahi banana, isi array me badlaav karna hai (in-place).
  e.g. [0,1,0,3,12] -> [1,3,12,0,0]
  kyun: non-zero 1,3,12 usi kram me aage aa gaye, dono 0 peeche
```

> [!IMPORTANT]
> **CHAABI:** `slow` = agli jagah jahan non-zero rakhna, `fast` scan. Non-zero mila -> `swap(nums[slow], nums[fast])` PEHLE, `slow++` BAAD.

> [!WARNING]
> `fast++` har baar, `if` ke BAHAR. `slow++` sirf swap ke BAAD (non-zero apni jagah lag gaya tabhi aage).

**CODE**
```cpp
void moveZeroes(vector<int> &nums) {
    int slow = 0;                           // slow = agli jagah jahan non-zero rakhna hai
    int fast = 0;                           // fast = scan karta
    while (fast < nums.size()) {
        if (nums[fast] != 0) {
            swap(nums[slow], nums[fast]);   // ★ swap PEHLE
            slow++;                         // ★ slow++ BAAD: non-zero apni jagah lag gaya tabhi aage
        }
        fast++;                             // fast++ har baar (if ke BAHAR)
    }
}
```


---

### ▸ REMOVE DUP (SORTED)  = MOVE ZEROES ka bhai (yahan slow++ PEHLE, overwrite BAAD)

**SAWAAL**
```
Ek SORTED array diya hai jisme kuch number dohraye gaye hain. Har number sirf ek baar rakhna hai, isi array me (in-place).
Kitne unique number bache (k), wo lautao. Array ke pehle k khaane me wahi unique number hone chahiye.
  e.g. [0,0,1,1,1,2,2,3,3,4] -> k=5, [0,1,2,3,4,...]
  kyun: alag numbers sirf 0,1,2,3,4 = 5. sorted hai to dohra hamesha bagal me hota
```

> [!IMPORTANT]
> **CHAABI:** `slow` = last UNIQUE ka index. `nums[slow] != nums[fast]` -> naya unique: `slow++` PEHLE, phir `nums[slow] = nums[fast]`.

> [!WARNING]
> MOVE ZEROES se ULTA kram: yahan `slow++` PEHLE, overwrite BAAD. Return `slow + 1` (slow index hai, ginti nahi).

**CODE**
```cpp
int removeDuplicates(vector<int> &nums) {
    int slow = 0;                           // slow = last UNIQUE ka index; 0 se: pehla khud unique
    int fast = 0;
    while (fast < nums.size()) {
        if (nums[slow] != nums[fast]) {     // naya unique mila (compare nums[slow] se)
            slow++;                         // ★ slow++ PEHLE: duplicate slot pe (move zeroes se ULTA)
            nums[slow] = nums[fast];        // overwrite BAAD
        }
        fast++;                             // fast har baar
    }
    return slow + 1;                        // ★ unique count = slow + 1 (slow index hai)
}
```


---

### ▸ REMOVE ELEMENT  = MOVE ZEROES ka bhai (write PEHLE, slow++ BAAD)

**SAWAAL**
```
Ek array aur ek number val diya hai. Array me se val ke saare number hatane hain, isi array me (in-place).
Kitne number bache (k), wo lautao. Pehle k khaane me val nahi hona chahiye, unka kram kuch bhi chalega.
  e.g. [0,1,2,2,3,0,4,2], val=2 -> k=5, {0,1,3,0,4}
  kyun: teen 2 hate, baaki 5 bache
```

> [!IMPORTANT]
> **CHAABI:** `nums[fast] != val` -> `nums[slow] = nums[fast]` PEHLE, `slow++` BAAD (move zeroes jaisa). `val` mila to sirf `fast++`.

> [!WARNING]
> write PEHLE, `slow++` BAAD. Return `slow` hi k hai (REMOVE DUP me `slow + 1` tha, yahan nahi).

**CODE**
```cpp
int removeElement(vector<int> &nums, int val) {
    int slow = 0, fast = 0;                 // slow = agli keep-jagah, fast scan
    while (fast < nums.size()) {
        if (nums[fast] != val) {
            nums[slow] = nums[fast];        // ★ write PEHLE
            slow++;                         // slow++ BAAD (move zeroes jaisa)
        }                                   // nums[fast] == val -> skip, sirf fast++
        fast++;
    }
    return slow;                            // slow = k, bache hue shuru ke k me
}
```


#### ── FAMILY: left/right COMPARE-CONVERGE (dono chhor se andar, har step compare) ──

### ▸ VALID PALINDROME (SIMPLE)

**SAWAAL**
```
Ek string di hai jisme sirf chhote akshar (lowercase) hain. Batao ki ye aage se aur peeche se ek jaisi padhi jaati hai ya nahi (palindrome). true ya false lautao.
  e.g. "racecar" -> true   "abca" -> false
  kyun: abca: pehla 'a' = aakhri 'a', par 'b' != 'c'
```

> [!IMPORTANT]
> **CHAABI:** `left`/`right` dono chhor se andar, `s[left] != s[right]` pe turant false, `left <= right` tak.

> [!WARNING]
> `right = s.size() - 1` (aakhri index), `s.size()` nahi.

**CODE**
```cpp
bool isPalindrome(string s) {               // sirf lowercase letters wala aasan version
    int left = 0;
    int right = s.size() - 1;               // ★ size() - 1 : aakhri index, size nahi
    while (left <= right) {
        if (s[left] != s[right])
            return false;                   // mismatch pe turant false
        left++;
        right--;
    }
    return true;
}
```


---

### ▸ VALID PALINDROME (FULL, LC-125)  = SIMPLE wala + sirf alnum, case ignore

**SAWAAL**
```
Ek string di hai jisme kuch bhi ho sakta hai: space, comma, bade akshar, digit. Sirf letter aur digit gino, aur bade-chhote akshar ko ek jaisa maano.
Inhe gin ke string palindrome banti hai ya nahi, true ya false lautao.
  e.g. "A man, a plan, a canal: Panama" -> true     "race a car" -> false
  kyun: filter + lower = "amanaplanacanalpanama" (palindrome) · "raceacar" (nahi)
```

> [!IMPORTANT]
> **CHAABI:** 4 branch isi kram me: `tolower` match -> dono andar · left alnum nahi -> `left++` · right alnum nahi -> `right--` · dono alnum PAR alag -> false.

> [!WARNING]
> false sirf AAKHRI `else` me (dono alnum aur alag). Pehle match, phir non-alnum skip; compare `tolower` ke saath (`<cctype>`).

**CODE**
```cpp
bool isPalindrome(string s) {               // mixed char: sirf alnum ginna, case ignore (<cctype>)
    int left = 0;
    int right = s.size() - 1;               // aakhri index, size nahi
    while (left <= right) {
        if (tolower(s[left]) == tolower(s[right])) {   // ★ 1. match -> dono andar
            left++;
            right--;
        }
        else if (!isalnum(s[left]))         // ★ 2. left alnum nahi -> skip
            left++;
        else if (!isalnum(s[right]))        // 3. right alnum nahi -> skip
            right--;
        else
            return false;                   // 4. dono alnum PAR alag -> false
    }
    return true;                            // koi alnum na ho / khaali -> true
}
```


---

### ▸ VALID PALINDROME II  = SIMPLE wala + mismatch pe EK delete (left YA right)

**SAWAAL**
```
Ek string di hai. Batao ki ye palindrome hai, ya zyada se zyada EK akshar hata ke palindrome ban sakti hai. true ya false lautao.
  e.g. "abca" -> true    "abc" -> false
  kyun: abca se 'c' hatao -> "aba". abc se koi bhi ek hatao, palindrome nahi banta
```

> [!IMPORTANT]
> **CHAABI:** pehle mismatch pe do raaste: `isPalindrome(s, left + 1, right) || isPalindrome(s, left, right - 1)`. Koi mismatch nahi -> true.
>
> isPalindrome: helper: l..r palindrome hai?
> validPalindrome: palindrome YA ek char delete karke ban jaaye

**CODE**
```cpp
bool isPalindrome(string s, int left, int right) {   // helper: l..r palindrome hai?
    while (left <= right) {
        if (s[left] != s[right])   // ★
            return false;
        left++;
        right--;
    }
    return true;
}

bool validPalindrome(string s) {            // palindrome YA ek char delete karke ban jaaye
    int left = 0;
    int right = s.size() - 1;
    while (left <= right) {
        if (s[left] != s[right])            // mismatch: left delete YA right delete, koi ek bane -> true
            return isPalindrome(s, left + 1, right) || isPalindrome(s, left, right - 1);
        left++;
        right--;
    }
    return true;                            // koi mismatch nahi = pehle se palindrome
}
```


---

### ▸ REVERSE VOWELS

**SAWAAL**
```
Ek string di hai. Usme sirf vowels (a e i o u, bade bhi) ka kram ulta karna hai. Baaki saare akshar apni jagah pe hi rehne chahiye. Nayi string lautao.
  e.g. "hello" -> "holle"
  kyun: vowels e, o -> ulte o, e. h l l jahan the wahin
```

> [!IMPORTANT]
> **CHAABI:** dono vowel -> swap + dono andar; left vowel nahi -> `left++`; warna `right--`. `isVowel` me lower + upper dono.
>
> isVowel: a e i o u, lower + upper dono
> reverseVowels: sirf vowels aapas me ulte, baaki char apni jagah

> [!WARNING]
> `while (left < right)`: milte hi ruko, beech ka akela char swap nahi. Bade vowel (A E I O U) bhi `isVowel` me.

**CODE**
```cpp
bool isVowel(char ch) {                     // a e i o u, lower + upper dono
    if ((ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') ||
        (ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U'))
        return true;
    return false;
}

string reverseVowels(string s) {            // sirf vowels aapas me ulte, baaki char apni jagah
    int left = 0;
    int right = s.size() - 1;
    while (left < right) {                  // < : milte hi ruko, beech ka akela char swap nahi
        if (isVowel(s[left]) && isVowel(s[right])) {   // ★ dono vowel -> swap + dono andar
            swap(s[left], s[right]);
            left++;
            right--;
        }
        else if (!isVowel(s[left]))         // left vowel nahi -> left aage
            left++;
        else                                // warna right peeche
            right--;
    }
    return s;
}
```


#### ── FAMILY: left/right by-VALUE decision (sum / area ki value dekh ke kaunsa pointer chale) ──

### ▸ CONTAINER MOST WATER

**SAWAAL**
```
height[] me khadi lines ki unchai di hai. Koi bhi do line chuno. Unke beech jitna paani rukega wo = chhoti line ki unchai x dono ke beech ki doori.
Sabse zyada kitna paani rok sakte ho, wo lautao.
  e.g. [1,8,6,2,5,4,8,3,7] -> 49
  kyun: index 1 (8) aur index 8 (7): min 7 x doori 7 = 49
```

> [!IMPORTANT]
> **CHAABI:** area = `min(height[low], height[high]) * (high - low)`. CHHOTI height wala pointer andar (wahi bottleneck).

> [!WARNING]
> faisla height pe: chhoti wali andar, badi nahi. Barabar ho to `else` se `high--`. `low < high` (same index pe width 0).

**CODE**
```cpp
int maxArea(vector<int> &height) {
    int low = 0;
    int high = height.size() - 1;
    int max_area = INT_MIN;
    while (low < high) {                    // < : same index pe width 0
        int width = high - low;
        int area = width * min(height[low], height[high]);   // area = chhoti height x width
        max_area = max(max_area, area);
        if (height[low] < height[high])     // ★ CHHOTI height wala andar (wahi bottleneck)
            low++;
        else                                // barabar height -> high-- (else)
            high--;
    }
    return max_area;
}
```


---

### ▸ TWO SUM II (SORTED)  = left/right by-VALUE (unsorted two sum = hashmap)

**SAWAAL**
```
Ek SORTED array aur ek target diya hai. Wo do number dhoondho jinka jod target ke barabar ho, aur unke index lautao.
Index 1 se gine jaate hain (1-based), 0 se nahi.
  e.g. [2,7,11,15], target=9 -> [1,2]
  kyun: 2 + 7 = 9, wo 1st aur 2nd number hain
```

> [!IMPORTANT]
> **CHAABI:** SORTED -> `left`/`right`: jod chhota -> `left++`, bada -> `right--`. UNSORTED two sum = hashmap.

> [!WARNING]
> 1-based lautao: `{left + 1, right + 1}`. `left < right` (ek hi index do baar nahi).

**CODE**
```cpp
vector<int> twoSum(vector<int> &nums, int target) {   // UNSORTED two sum = hashmap; SORTED = 2 pointer
    int left = 0, right = nums.size() - 1;
    while (left < right) {                  // < : ek hi index do baar nahi
        if (nums[left] + nums[right] == target)
            return {left + 1, right + 1};   // ★ 1-based lautao
        else if (nums[left] + nums[right] < target)
            left++;                         // chhota -> bada chahiye
        else
            right--;                        // bada -> chhota chahiye
    }
    return {-1, -1};                        // na mile; O(1) space
}
```


---

### ▸ SUM OF SQUARE NUMBERS  = TWO SUM II jaisa (2 number ki jodi), low=0 high=sqrt(c)

**SAWAAL**
```
Ek number c diya hai. Batao ki kya aise do poore number a aur b hain jinke liye a*a + b*b = c ho. true ya false lautao.
  e.g. 5 -> true    3 -> false
  kyun: 5 = 1*1 + 2*2. 3 kisi bhi do square ka jod nahi
```

> [!IMPORTANT]
> **CHAABI:** 2 number ki jodi = two pointer: `low = 0, high = sqrt(c)`; `low*low + high*high` chhota -> `low++`, bada -> `high--`.

> [!WARNING]
> `low <= high` (`<=`: c=0 me 0+0 jaisa ek hi point bhi check ho). Guna se PEHLE `(long long)` cast, warna overflow.

**CODE**
```cpp
bool judgeSquareSum(long long c) {          // a² + b² == c ? (LC 633) 2 number ki jodi -> two pointer
    int low = 0, high = sqrt(c);
    while (low <= high) {                   // ★ <= : c=0 me 0+0 jaisa ek hi point bhi check ho
        long long ans = (long long)low * low + (long long)high * high;   // ★ guna se PEHLE cast: overflow
        if (ans == c)
            return true;
        else if (ans < c)
            low++;
        else
            high--;
    }
    return false;
}
```


#### ── FAMILY: fill-from-END (result peeche se bharo, bada element end me) ──

### ▸ SQUARES OF SORTED ARR

**SAWAAL**
```
Ek SORTED array diya hai jisme minus number bhi ho sakte hain. Har number ka square nikaalo, aur jawab wala array bhi sorted hona chahiye.
Dobara sort() nahi lagana, ek hi baar me (O(n)) karna hai.
  e.g. [-4,-1,0,3,10] -> [0,1,9,16,100]
  kyun: -4 ka square 16 hai, isliye sabse bade square dono SIRE pe hote, beech me nahi
```

> [!IMPORTANT]
> **CHAABI:** sabse BADA square dono chhor pe (negative) -> peeche se bharo: `k = n - 1`, jo square bada wo `ans[k]`, `k--`.

> [!WARNING]
> `left <= right` (`<` nahi): left==right wala aakhri element bhi `ans[0]` me jaana, `<` se `ans[0]` khaali (`[-1,2] -> [0,4]`).

**CODE**
```cpp
vector<int> sortedSquares(vector<int> &nums) {   // sabse BADA square dono chhor pe (negative)
    vector<int> ans(nums.size(), -1);       // size n pehle se: k-- se index pe likho; O(n), sort nahi
    int left = 0, right = nums.size() - 1, k = nums.size() - 1;   // ★ k = n-1 : sabse bada aakhri slot me
    // ★ <= (< nahi): left==right wala aakhri element bhi ans[0] me jaana. < se ans[0] khaali ([-1,2] -> [0,4])
    while (left <= right) {
        if (nums[left] * nums[left] <= nums[right] * nums[right]) {
            ans[k] = nums[right] * nums[right];   // bada square aakhri khaali slot pe
            right--;
            k--;                            // likho, phir peeche
        }
        else {
            ans[k] = nums[left] * nums[left];     // left bada (negative tha), uska square peeche
            left++;
            k--;
        }
    }
    return ans;
}
```


---

### ▸ MERGE SORTED ARRAY  = SQUARES jaisa fill-from-END

**SAWAAL**
```
Do sorted array hain. nums1 me pehle m asli number hain aur peeche n khaali jagah (0). nums2 me n number hain.
nums2 ko nums1 me hi milana hai (in-place), taaki poora nums1 sorted ho jaaye.
  e.g. nums1=[1,2,3,0,0,0] m=3, nums2=[2,5,6] -> nums1=[1,2,2,3,5,6]
  kyun: peeche se bharo (khaali jagah peeche hai), sabse bada pehle rakh do
```

> [!IMPORTANT]
> **CHAABI:** peeche se bharo: `i = m - 1`, `j = n - 1`, `k = m + n - 1`; jo bada wahi `nums1[k]`. Ant me bache `nums2[j]` copy.

> [!WARNING]
> `k = m + n - 1` (`m*n-1` NAHI). `while (i >= 0 && j >= 0)`: `&&` (`||` se -1 index crash). Aakhri loop `nums2[j]` (`nums1[j]` NAHI), ye abhi copy nahi hue.

**CODE**
```cpp
void merge(vector<int> &nums1, int m, vector<int> &nums2, int n) {   // nums1 ke end me n khaali
    int i = m - 1;
    int j = n - 1;
    int k = m + n - 1;                      // ★ k = m+n-1 (m*n-1 NAHI) : nums1 ka aakhri index
    while (i >= 0 && j >= 0) {              // && (|| nahi -> -1 index crash)
        if (nums1[i] < nums2[j]) {          // jo bada wahi peeche (aage se bharo to zinda element mitta)
            nums1[k] = nums2[j];
            j--;
            k--;
        }
        else {                              // nums1[i] bada ya barabar -> wahi aakhri khaali slot pe
            nums1[k] = nums1[i];
            i--;
            k--;
        }
    }
    while (i >= 0) {                        // optional: nums1 ke bache pehle se apni jagah pe hain
        nums1[k] = nums1[i];
        i--;
        k--;
    }
    while (j >= 0) {                        // ★ zaroori: nums2[j] (nums1[j] NAHI), ye abhi copy nahi hue
        nums1[k] = nums2[j];
        j--;
        k--;
    }
}                                           // in-place: naya array nahi -> O(1) extra, O(m+n)
```


#### ── FAMILY: PARTITION / swap-in-place (category me baanto, galat jagah pe swap) ──

### ▸ SORT COLORS (0,1,2)

**SAWAAL**
```
Ek array me sirf 0, 1 aur 2 hain. Inhe isi array me sort karna hai (pehle saare 0, phir 1, phir 2).
sort() nahi lagana, ek hi pass me karna hai.
  e.g. [2,0,2,1,1,0] -> [0,0,1,1,2,2]
```

> [!IMPORTANT]
> **CHAABI:** 3 pointer: 0 -> `swap(low, mid)`, `low++ mid++` · 1 -> `mid++` · 2 -> `swap(mid, high)`, `high--` (mid wahin).

> [!WARNING]
> `mid <= high` (`<` se `[2,0,1]` pe `[1,0,2]` reh jaata). 2 pe `mid++` NAHI: high se aaya element unknown, dobara check.

**CODE**
```cpp
void sortColors(vector<int> &nums) {        // Dutch flag, 3 pointer
    int low = 0;
    int mid = 0;
    int high = nums.size() - 1;
    // ★ <= (< nahi): high wala cell abhi UNKNOWN, use bhi check. [2,0,1] pe < se [1,0,2] reh jaata
    while (mid <= high) {
        if (nums[mid] == 0) {
            swap(nums[low], nums[mid]);
            low++;
            mid++;                          // mid++ bhi: low se aaya element pehle hi check ho chuka
        }
        else if (nums[mid] == 1)
            mid++;                          // sirf mid++
        else if (nums[mid] == 2) {
            swap(nums[mid], nums[high]);
            high--;                         // ★ mid++ NAHI: high se aaya element unknown, dobara check
        }
    }
}
```


---

### ▸ SORT ARRAY BY PARITY  = SORT COLORS ka 2-category wala (left/right swap)

**SAWAAL**
```
Ek array diya hai. Saare even number pehle aur saare odd number baad me rakhne hain. Even aapas me ya odd aapas me kis kram me hain, isse farak nahi padta.
  e.g. [3,1,2,4] -> [2,4,3,1] (ya [4,2,1,3], koi bhi jisme even pehle)
```

> [!IMPORTANT]
> **CHAABI:** left odd + right even (dono galat jagah) -> swap + dono andar; left even -> `left++`; warna `right--`.

> [!WARNING]
> left even + right odd = dono sahi jagah, swap MAT (TRAP).

**CODE**
```cpp
vector<int> sortArrayByParity(vector<int> nums) {   // even pehle, odd baad (aapas ka order koi bhi)
    int left = 0;
    int right = nums.size() - 1;
    while (left < right) {
        if (nums[left] % 2 != 0 && nums[right] % 2 == 0) {   // dono galat jagah -> swap + dono andar
            swap(nums[left], nums[right]);
            left++;
            right--;
        }
        else if (nums[left] % 2 == 0)       // left even = sahi -> left aage
            left++;
        else                                // warna right peeche
            right--;
    }                                       // ★ TRAP: left even + right odd = dono sahi, swap MAT
    return nums;
}
```


── (STANDALONE — apni alag trick) ──

### ▸ 3SUM (LC-15)  = SORT + ek fix + baaki 2-pointer (+ duplicate skip)

**SAWAAL**
```
Ek array diya hai. Aise saare teen numbers ke group [a, b, c] lautao jinka jod 0 ho.
Ek hi group do baar nahi aana chahiye, chahe array me number dohraye gaye hon.
  e.g. [-1,0,1,2,-1,-4] -> [-1,-1,2], [-1,0,1]
  kyun: -1 do baar hai, par [-1,0,1] sirf ek baar likhna hai
```

**SAAR**
```
CRUX = DUPLICATE skip 3 JAGAH: (1) fixed i (2) match ke baad LOW (3) match ke baad HIGH.
sum = -nums[i] (fixed ko doosri taraf) -> sorted pe 2-pointer (= 2SUM-II).
```

> [!IMPORTANT]
> **CHAABI:** sort + har `i` FIX, `sum = -nums[i]`, baaki `low/high` = 2SUM-II. Duplicate skip 3 jagah: fixed `i`, match ke baad `low`, match ke baad `high`.

> [!WARNING]
> skip loop AAKHRI copy pe rukta -> uske baad `low++`, `high--` zaroori, warna wahi triplet dobara. Skip me `low < high &&` PEHLE (`low+1` bahar). `i > 0` pehle (`nums[-1]`).

**CODE**
```cpp
vector<vector<int>> threeSum(vector<int> &nums) {   // saare UNIQUE triplet jinka sum 0
    vector<vector<int>> ans;
    sort(begin(nums), end(nums));           // sort PEHLE: 2 pointer + duplicate skip sorted pe hi
    for (int i = 0; i < nums.size(); i++) { // har number ek baar FIX
        if (i > 0 && nums[i] == nums[i - 1])
            continue;                       // ★ skip 1/3: duplicate FIXED (i>0 pehle, warna nums[-1])
        int low = i + 1, high = nums.size() - 1, sum = -nums[i];   // low=i+1 : fixed ke baad wale hi
        while (low < high) {                // < : low==high ek element; sum=-fixed = 2SUM-II
            if (nums[low] + nums[high] == sum) {
                ans.push_back({nums[low], nums[high], nums[i]});
                // ★ && (|| nahi): pehle low<high dekho, jhooth ho to nums[low+1] padho hi mat
                while (low < high && nums[low] == nums[low + 1])
                    low++;                  // ★ skip 2/3: duplicate LOW (low<high pehle: low+1 bahar)
                while (low < high && nums[high] == nums[high - 1])
                    high--;                 // skip 3/3: duplicate HIGH (low<high pehle)
                // ★ skip loop AAKHRI copy pe rukta, naye number pe nahi -> bina ++ ke wahi triplet dobara
                //   ([-2,0,0,2,2] me L idx2 = 0, H idx3 = 2 pe rukte)
                low++;                      // skip ke baad bhi 1 kadam: abhi purane number pe khade
                high--;
            }
            else if (nums[low] + nums[high] < sum)
                low++;                      // chhota -> low aage
            else
                high--;                     // bada -> high peeche
        }
    }
    return ans;
}
```


---

### ▸ IS SUBSEQUENCE

**SAWAAL**
```
Do string s aur t di hain. Batao ki kya t me se kuch akshar hata ke (baaki ka kram badle bina) s ban sakti hai. true ya false lautao.
  e.g. s="abc", t="ahbgdc" -> true     s="axc" -> false
  kyun: a..b..c t me usi kram me mil gaye. 'x' t me hai hi nahi
```

> [!IMPORTANT]
> **CHAABI:** `i` -> s, `j` -> t. Char mila -> dono aage; nahi mila -> sirf `j++`. Ant me `i >= s.size()` -> true.

> [!WARNING]
> mismatch pe sirf `j++` (t aage), `i` wahin. Loop `&&` pe rukta: koi ek khatam -> ruko.

**CODE**
```cpp
bool isSubsequence(string s, string t) {    // s, t me order se hai? beech ke char skip chalega
    int i = 0, j = 0;                       // i -> s, j -> t
    while (i < s.size() && j < t.size()) {  // koi ek khatam -> ruko
        if (s[i] == t[j]) {                 // char mila -> dono aage
            i++;
            j++;
        }
        else                                // ★ nahi mila -> sirf t aage
            j++;
    }
    if (i >= s.size())                      // s ke saare char order me mil gaye
        return true;
    return false;
}
```


---

### ▸ TRAPPING RAIN WATER (LC-42)

**SAWAAL**
```
height[] me buildings ki unchai di hai, har building ki chaudai 1 hai. Baarish ke baad buildings ke beech ke gaddhon me kul kitna paani rukega, wo lautao.
  e.g. [4,2,0,3,2,5] -> 9
  kyun: beech wale bars ke upar paani = min(left ka max, right ka max) - apni height. 2+4+1+2 = 9
```

**SAAR**
```
kyun SAFE: jis taraf ki DEEWAR chhoti, wahi taraf niptao -- doosri taraf ek badi deewar pakka khadi,
           to had isi chhoti taraf ke max se tay hoti.
★ MILNE KI JAGAH: dono pointer jahan milte, wo cell kabhi process nahi hota. Deewar dekh ke chalne se
  badi deewar apni jagah rukti -> dono SABSE UNCHI deewar pe milte, jahan paani hota hi nahi.
★ TRAP: faisla height[left] <= height[right] (DEEWAR) pe lo, leftMax <= rightMax pe NAHI.
  max pe liya to ek khamba chhoot jaata: [4,1,4,1] -> 0 aata (sahi 3).

★ 24-Sep THEEK KIYA (Arpan ka apna code):
    ★ PURANA (GALAT) jo yahan likha tha: faisla `leftMax <= rightMax` pe, leftMax=height[0] se shuru
      (REFRESH pehle, ADD baad).
      max abhi current deewar ko shaamil kiye bina -> milne ki jagah galat -> ek khamba chhoot jaata.
      [4,1,4,1] pe 0 (sahi 3) · [4,5,3,1,4,4] pe 3 (sahi 4). 12 test pass the, random input pe ~1/5 galat.
```

> [!IMPORTANT]
> **CHAABI:** faisla DEEWAR pe: `height[left] <= height[right]` -> chhoti deewar wali taraf niptao: `leftMax > height[left]` -> paani `leftMax - height[left]`, warna nayi deewar `leftMax`. Doosri taraf mirror.
>
> bar i ke upar paani = min(leftMax[i], rightMax[i]) - height[i]   (min: chhoti side se bah jaata)
> O(n) space wala: leftMax[] aage se, rightMax[] peeche se, ans += min(..) - height[i]

> [!WARNING]
> faisla `height[left] <= height[right]` (DEEWAR) pe lo, `leftMax <= rightMax` pe NAHI: max pe liya to ek khamba chhoot jaata, `[4,1,4,1] -> 0` (sahi 3).

**CODE**
```cpp
int trap(vector<int> &height) {             // O(1) space, two pointer
    int n = height.size();
    int left = 0, right = n - 1;
    int leftMax = 0, rightMax = 0, ans = 0; // max 0 se: abhi koi deewar dekhi nahi; empty -> 0
    while (left < right) {                  // < : milne wali cell sabse unchi deewar, uspe paani nahi
        if (height[left] <= height[right]) {    // ★ DEEWAR pe faisla, max pe NAHI ([4,1,4,1] -> 3)
            if (leftMax > height[left])     // ★ chhoti deewar wali taraf niptao: doosri taraf badi pakki
                ans += leftMax - height[left];  // chhota -> paani
            else
                leftMax = height[left];     // bada -> nayi deewar
            left++;
        }
        else {
            if (rightMax > height[right])
                ans += rightMax - height[right];   // chhota -> paani (rightMax tak)
            else
                rightMax = height[right];   // bada -> nayi deewar
            right--;
        }
    }
    return ans;
}
```


---

### ▸ PALINDROMIC SUBSTRINGS (LC-647)  = EXPAND AROUND CENTER

**SAWAAL**
```
Ek string di hai. Usme kitni substrings (lagaatar akshar) palindrome hain, wo gino.
Akela akshar bhi palindrome gina jaata hai, aur alag jagah wali same substring alag gini jaati hai.
  e.g. "aaa" -> 6
  kyun: a, a, a (3) + aa, aa (2) + aaa (1) = 6
```

**SAAR**
```
★★ EXAMPLE (yahi yaad -> poora recall):   "abcba"
    a  b  c  b  a
          ^            "c"        <- center se shuru
       ^  ^  ^         "bcb"      <- ek kadam phaila (b==b)
    ^  ^  ^  ^  ^      "abcba"    <- aur phaila (a==a)
    => ek hi center se 3 palindrome mile -> har phailav pe count++

★ ARPAN-LINE (jisse ye derive hua): "beech wala khada ho ke dono taraf poochta --
  'tum dono same ho? aa jao mere paas'. same hue -> palindrome bana -> phir agli jodi se poochta."

(substring = CONTIGUOUS -> skip ka option nahi -> choice nahi -> recursion nahi.
 recursion ki khushboo SUBSEQUENCE me aati, substring me nahi).
```

> [!IMPORTANT]
> **CHAABI:** har CENTER se phailo: odd `solve(i, i)`, even `solve(i, i + 1)`; `while` me range check PEHLE phir `s[i] == s[j]`, HAR match pe `count++`.
>
> kitne substring palindrome (single char bhi; alag position = alag count). har CENTER se phailo
> "abcba": c -> bcb -> abcba = ek center se 3. ODD center = ek char, EVEN center = do char jodi
> solve: &count : warna bahar count badhta hi nahi

> [!WARNING]
> `int &count` reference, warna bahar count badhta hi nahi. `count++` HAR match pe (ant me ek baar nahi).

**CODE**
```cpp
void solve(int i, int j, const string &s, int &count) {   // ★ &count : warna bahar count badhta hi nahi
    while (i >= 0 && j < s.size() && s[i] == s[j]) {   // range check PEHLE, phir s[i]==s[j]
        i--;                                // dono taraf phailo
        j++;
        count++;                            // ★ HAR match = ek naya palindrome (ant me ek baar nahi)
    }
}

int countSubstrings(string s) {
    int count = 0;
    for (int i = 0; i < s.size(); i++) {    // n odd center + (n-1) even center
        solve(i, i, s, count);              // odd: center ek char (khud bhi palindrome)
        solve(i, i + 1, s, count);          // even: i+1 == n bhi theek, while me j < n pehle check
    }
    return count;                           // O(n²), O(1); DP nahi: substring contiguous, choice nahi
}
```

