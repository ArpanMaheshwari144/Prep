## PATTERN 1 — TWO POINTER

```
 BROAD IDEA: do pointer (dono taraf se, ya slow-fast) -> ek saath move karke O(n).
 ★ EDGE (sorted 2-ptr): duplicate skip karna hai kya? 3-sum HAAN, 2-sum nahi (bina skip ek triplet kai baar).
 ★ MILTE-JULTE (container / trapping / histogram): upar NAAM-SE-CONFUSE A dekh.

```

#### ── FAMILY: slow/fast OVERWRITE (fast scan, rakhne wala slow pe; teeno same skeleton) ──

```
 KAB: array SORTED hai aur do number ki jodi chahiye · ya array me IN-PLACE kuch hatana / aage karna hai
```

### ▸ MOVE ZEROES

```
Ek array diya hai. Saare 0 ko array ke end me bhejna hai, aur baaki numbers jis kram me the usi kram me rehne chahiye.
Naya array nahi banana, isi array me badlaav karna hai (in-place).
  e.g. [0,1,0,3,12] -> [1,3,12,0,0]
  kyun: non-zero 1,3,12 usi kram me aage aa gaye, dono 0 peeche
```

```
TASVEER (purani sheet se, same to same):

slow = agli jagah jahan non-zero rakhna hai; fast SCAN karta.
fast pe non-zero mila -> pehle swap(nums[slow], nums[fast]) -> PHIR slow++.   // fast++ har baar (if ke BAHAR) · slow=0, fast=0 se shuru
ORDER: swap PEHLE, slow++ BAAD (slow tabhi aage jab ek non-zero apni jagah lag gaya).
```

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

### ▸ REMOVE DUP (SORTED)

```
Ek SORTED array diya hai jisme kuch number dohraye gaye hain. Har number sirf ek baar rakhna hai, isi array me (in-place).
Kitne unique number bache (k), wo lautao. Array ke pehle k khaane me wahi unique number hone chahiye.
  e.g. [0,0,1,1,1,2,2,3,3,4] -> k=5, [0,1,2,3,4,...]
  kyun: alag numbers sirf 0,1,2,3,4 = 5. sorted hai to dohra hamesha bagal me hota
```

```
TASVEER (purani sheet se, same to same):

move-zeroes cousin. slow = last-UNIQUE ka index, fast SCAN.   // slow=0, fast=0: pehla element khud unique, compare nums[slow]
naya unique (nums[slow] != nums[fast]) -> pehle slow++ (slow ab duplicate/khali slot pe) -> PHIR nums[slow] = nums[fast] (overwrite).
ORDER: yahan slow++ PEHLE, overwrite BAAD (move-zeroes se ULTA). fast har baar++. unique count = slow+1.
```

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

### ▸ REMOVE ELEMENT

```
Ek array aur ek number val diya hai. Array me se val ke saare number hatane hain, isi array me (in-place).
Kitne number bache (k), wo lautao. Pehle k khaane me val nahi hona chahiye, unka kram kuch bhi chalega.
  e.g. [0,1,2,2,3,0,4,2], val=2 -> k=5, {0,1,3,0,4}
  kyun: teen 2 hate, baaki 5 bache
```

```
TASVEER (purani sheet se, same to same):

saare 'val' hata, non-val ki count k lauta (move-zeroes cousin). slow = agli keep-jagah, fast scan.
nums[fast] != val -> pehle nums[slow] = nums[fast] -> PHIR slow++.   nums[fast] == val -> skip (sirf fast++).
ORDER: write PEHLE, slow++ BAAD (move-zeroes jaisa). end me slow = k (bache shuru ke k me).
```

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

```
Ek string di hai jisme sirf chhote akshar (lowercase) hain. Batao ki ye aage se aur peeche se ek jaisi padhi jaati hai ya nahi (palindrome). true ya false lautao.
  e.g. "racecar" -> true   "abca" -> false
  kyun: abca: pehla 'a' = aakhri 'a', par 'b' != 'c'
```

```
TASVEER (purani sheet se, same to same):

left/right dono taraf se, compare karte andar aao. (sirf lowercase-letters wala aasan version.)   // while(left<=right), mismatch pe turant return false
```

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

### ▸ VALID PALINDROME (FULL, LC-125)

```
Ek string di hai jisme kuch bhi ho sakta hai: space, comma, bade akshar, digit. Sirf letter aur digit gino, aur bade-chhote akshar ko ek jaisa maano.
Inhe gin ke string palindrome banti hai ya nahi, true ya false lautao.
  e.g. "A man, a plan, a canal: Panama" -> true     "race a car" -> false
  kyun: filter + lower = "amanaplanacanalpanama" (palindrome) · "raceacar" (nahi)
```

```
TASVEER (purani sheet se, same to same):

left/right dono taraf se, compare karte andar aao. (sirf lowercase-letters wala aasan version.)   // while(left<=right), mismatch pe turant return false
```

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

### ▸ VALID PALINDROME II

```
Ek string di hai. Batao ki ye palindrome hai, ya zyada se zyada EK akshar hata ke palindrome ban sakti hai. true ya false lautao.
  e.g. "abca" -> true    "abc" -> false
  kyun: abca se 'c' hatao -> "aba". abc se koi bhi ek hatao, palindrome nahi banta

isPalindrome: helper: l..r palindrome hai?
validPalindrome: palindrome YA ek char delete karke ban jaaye
```

```
TASVEER (purani sheet se, same to same):

= palindrome YA ek char DELETE karke palindrome ban jaaye.
IDEA: two-pointer; MISMATCH pe -> ya LEFT char delete ya RIGHT char delete. koi ek bane -> true, dono na bane -> false.
TEMPLATE (CLEAN): helper isPalindrome(s,l,r). main loop; mismatch pe seedha ->   // helper bhi while(l<=r), mismatch -> false · match pe l++ r--
    return isPalindrome(s,l+1,r) || isPalindrome(s,l,r-1);   (left-delete YA right-delete; || = ek se bhi bane -> true)
    koi mismatch na aaye -> already palindrome -> true.
```

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

### ▸ REVERSE VOWELS

```
Ek string di hai. Usme sirf vowels (a e i o u, bade bhi) ka kram ulta karna hai. Baaki saare akshar apni jagah pe hi rehne chahiye. Nayi string lautao.
  e.g. "hello" -> "holle"
  kyun: vowels e, o -> ulte o, e. h l l jahan the wahin

isVowel: a e i o u, lower + upper dono
reverseVowels: sirf vowels aapas me ulte, baaki char apni jagah
```

```
TASVEER (purani sheet se, same to same):

sirf vowels (a/e/i/o/u, lower+upper) aapas me reverse, baaki char apni jagah.
chhota helper isVowel(ch). left/right pointer:
dono vowel -> swap + left++ right--.  left vowel nahi -> left++.  warna right--.   // left<right: milte hi ruko, beech ka akela char swap nahi · return s
```

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

```
height[] me khadi lines ki unchai di hai. Koi bhi do line chuno. Unke beech jitna paani rukega wo = chhoti line ki unchai x dono ke beech ki doori.
Sabse zyada kitna paani rok sakte ho, wo lautao.
  e.g. [1,8,6,2,5,4,8,3,7] -> 49
  kyun: index 1 (8) aur index 8 (7): min 7 x doori 7 = 49
```

```
TASVEER (purani sheet se, same to same):

left/right; area = min(h)*width; CHHOTI height wala pointer andar (bottleneck).   // left<right: same index pe width 0 · barabar height -> right-- (else)
```

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

### ▸ TWO SUM II (SORTED)

```
Ek SORTED array aur ek target diya hai. Wo do number dhoondho jinka jod target ke barabar ho, aur unke index lautao.
Index 1 se gine jaate hain (1-based), 0 se nahi.
  e.g. [2,7,11,15], target=9 -> [1,2]
  kyun: 2 + 7 = 9, wo 1st aur 2nd number hain
```

```
TASVEER (purani sheet se, same to same):

sorted -> 2 pointer (left,right); sum==target -> mila (return 1-based).
sum<target -> left++ (bada chahiye); sum>target -> right-- (chhota). O(1) space.   // while(left<right): ek hi index do baar nahi · na mile -> {-1,-1}
(regular two-sum UNSORTED = hashmap; SORTED = 2-pointer.)
```

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

### ▸ SUM OF SQUARE NUMBERS

```
Ek number c diya hai. Batao ki kya aise do poore number a aur b hain jinke liye a*a + b*b = c ho. true ya false lautao.
  e.g. 5 -> true    3 -> false
  kyun: 5 = 1*1 + 2*2. 3 kisi bhi do square ka jod nahi
```

```
TASVEER (purani sheet se, same to same):

a²+b²==c possible? (LC 633). RECOGNITION: 2 NUMBER (pair) chahiye -> TWO-POINTER.
low=0, high=sqrt(c). ans=low²+high²: ==c->true · <c->low++ · >c->high--. low<=high tak, warna false.
★ low<=high (NOT low<high) -- c=0 ka 0+0 single-point bhi check ho jaaye. OVERFLOW: ans = long long.   // (long long)low*low: guna se PEHLE cast
```

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

```
Ek SORTED array diya hai jisme minus number bhi ho sakte hain. Har number ka square nikaalo, aur jawab wala array bhi sorted hona chahiye.
Dobara sort() nahi lagana, ek hi baar me (O(n)) karna hai.
  e.g. [-4,-1,0,3,10] -> [0,1,9,16,100]
  kyun: -4 ka square 16 hai, isliye sabse bade square dono SIRE pe hote, beech me nahi
```

```
TASVEER (purani sheet se, same to same):

SAAR: sorted arr me negatives ka square order ULTA -> sabse BADA square dono CHHOR pe (beech me nahi). 2-ptr, ans PEECHE se bharo. O(n), no sort.
TEMPLATE: left=0, right=n-1, k=n-1.   // k=n-1: sabse bada square ans ke aakhri slot me · ans(n) pehle se size n (k-- se index pe likho)
   while(left<=right):              // <= : aakhri bacha 1 element bhi daalna hai
      if(left² <= right²) -> ans[k--]=right², right--   // bada square aakhri khaali slot pe; k-- = likho phir peeche
      else                -> ans[k--]=left²,  left++   // left wala bada (negative tha), uska square peeche daalo
FAMILY: fill-from-END (result peeche se bharo).
```

```cpp
vector<int> sortedSquares(vector<int> &nums) {   // sabse BADA square dono chhor pe (negative)
    vector<int> ans(nums.size(), -1);       // size n pehle se: k-- se index pe likho; O(n), sort nahi
    int left = 0, right = nums.size() - 1, k = nums.size() - 1;   // ★ k = n-1 : sabse bada aakhri slot me
    while (left <= right) {                 // <= : aakhri bacha 1 element bhi daalna hai
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

### ▸ MERGE SORTED ARRAY

```
Do sorted array hain. nums1 me pehle m asli number hain aur peeche n khaali jagah (0). nums2 me n number hain.
nums2 ko nums1 me hi milana hai (in-place), taaki poora nums1 sorted ho jaaye.
  e.g. nums1=[1,2,3,0,0,0] m=3, nums2=[2,5,6] -> nums1=[1,2,2,3,5,6]
  kyun: peeche se bharo (khaali jagah peeche hai), sabse bada pehle rakh do
```

```
TASVEER (purani sheet se, same to same):

SAAR: 2 sorted arr; nums1 ke END me n khaali. in-place -> PEECHE se bharo (aage se karo to zinda element overwrite). O(m+n).  ★ k=m+n-1 NA m*n-1
in-place = naya array NAHI banaya; usi nums1 me answer bhara -> O(1) EXTRA space.
TEMPLATE: i=m-1, j=n-1, k=m+n-1.   // k=m+n-1: nums1 ka aakhri index (total m+n jagah)
   while(i>=0 && j>=0):                       // ★ && (|| nahi -> -1 index crash)
      if(nums1[i] < nums2[j]) -> nums1[k--]=nums2[j], j--   // jo bada wahi peeche; barabar pe else (nums1 wala) chalega
      else                    -> nums1[k--]=nums1[i], i--   // nums1[i] bada ya barabar -> wahi aakhri khaali slot pe
   while(i>=0): nums1[k--]=nums1[i], i--   // optional: nums1 ke bache pehle se apni jagah pe hi hain
   while(j>=0): nums1[k--]=nums2[j], j--      // nums2[j] NA nums1[j] (nums2 ke bache abhi copy nahi hue, ye loop zaroori)
```

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

```
Ek array me sirf 0, 1 aur 2 hain. Inhe isi array me sort karna hai (pehle saare 0, phir 1, phir 2).
sort() nahi lagana, ek hi pass me karna hai.
  e.g. [2,0,2,1,1,0] -> [0,0,1,1,2,2]
```

```
TASVEER (purani sheet se, same to same):

Dutch flag, 3 pointer (low/mid/high).  loop: while(mid <= high)   // <= : high wala abhi dekha nahi, use bhi check · low=mid=0, high=n-1
nums[mid]==0 -> swap(low,mid), low++ AUR mid++ (dono).   // mid++ bhi: low se aaya element pehle hi check ho chuka
nums[mid]==1 -> sirf mid++.
nums[mid]==2 -> swap(mid,high), high-- (mid++ NAHI -> high se aaya element unknown, dobara check).

PEHLI SHEET (Aug) se bhi, same to same:

Dutch flag, 3 pointer (low/mid/high):
nums[mid]==0 -> swap(low,mid), low++ AUR mid++ (dono).
nums[mid]==1 -> sirf mid++.
nums[mid]==2 -> swap(mid,high), high-- (mid++ NAHI -> high se aaya element unknown, dobara check).
```

```cpp
void sortColors(vector<int> &nums) {        // Dutch flag, 3 pointer
    int low = 0;
    int mid = 0;
    int high = nums.size() - 1;
    while (mid <= high) {                   // <= : high wala abhi dekha nahi, use bhi check
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

### ▸ SORT ARRAY BY PARITY

```
Ek array diya hai. Saare even number pehle aur saare odd number baad me rakhne hain. Even aapas me ya odd aapas me kis kram me hain, isse farak nahi padta.
  e.g. [3,1,2,4] -> [2,4,3,1] (ya [4,2,1,3], koi bhi jisme even pehle)
```

```
TASVEER (purani sheet se, same to same):

evens pehle, odds baad (aapas order koi bhi). left/right pointer.   // while(left<right) · odd check = nums[x]%2 != 0
left ODD && right EVEN (dono galat jagah) -> swap + left++ right--.
left EVEN (sahi) -> left++.  warna -> right--.
TRAP: left even && right odd pe swap MAT karo -> dono already sahi jagah.
```

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

── (STANDALONE — apni alag trick) ──
```

### ▸ 3SUM (LC-15)  = SORT + ek fix + baaki 2-pointer (+ duplicate skip)

```
Ek array diya hai. Aise saare teen numbers ke group [a, b, c] lautao jinka jod 0 ho.
Ek hi group do baar nahi aana chahiye, chahe array me number dohraye gaye hon.
  e.g. [-1,0,1,2,-1,-4] -> [-1,-1,2], [-1,0,1]
  kyun: -1 do baar hai, par [-1,0,1] sirf ek baar likhna hai
```

```
TASVEER (purani sheet se, same to same):

SAAR : saare UNIQUE triplets jinka sum==0. SORT -> ek number FIX -> baaki 2 ke liye 2-pointer (sum = -fixed).
TEMPLATE:
    sort(begin(nums), end(nums));   // sort PEHLE: 2-pointer aur duplicate skip dono sorted pe hi chalte
    for(int i=0; i<nums.size(); i++){   // har number ko ek baar FIX karo
        if(i>0 && nums[i]==nums[i-1]) continue;              // duplicate FIXED skip (i>0 pehle: i=0 pe nums[i-1] bahar padhta)
        int low=i+1, high=nums.size()-1, sum=-nums[i];   // low=i+1: fixed ke baad wale hi lo, warna wahi element dobara
        while(low<high){   // < : low==high = ek hi element, jodi nahi banti
            if(nums[low]+nums[high]==sum){   // jodi mili = fixed ke saath triplet ka sum 0
                ans.push_back({nums[i],nums[low],nums[high]});
                while(low<high && nums[low]==nums[low+1]) low++;   // duplicate LOW skip (pehle low<high, warna low+1 bahar)
                while(low<high && nums[high]==nums[high-1]) high--; // duplicate HIGH skip (pehle low<high, warna high low se neeche khisak jaata)
                low++; high--;                                       // skip ke baad bhi 1 kadam: abhi purane number pe khade
            else if(nums[low]+nums[high] < sum) low++;       // chhota -> low aage (bada chahiye)
CRUX = DUPLICATE skip 3 JAGAH: (1) fixed i (i>0 && nums[i]==nums[i-1]) (2) match ke baad LOW (3) match ke baad HIGH.
sum = -nums[i] (fixed ko doosri taraf) -> sorted pe 2-pointer (= 2SUM-II).
```

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
                while (low < high && nums[low] == nums[low + 1])
                    low++;                  // ★ skip 2/3: duplicate LOW (low<high pehle: low+1 bahar)
                while (low < high && nums[high] == nums[high - 1])
                    high--;                 // skip 3/3: duplicate HIGH (low<high pehle)
                low++;                      // ★ skip ke baad bhi 1 kadam: abhi purane number pe khade
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

### ▸ IS SUBSEQUENCE

```
Do string s aur t di hain. Batao ki kya t me se kuch akshar hata ke (baaki ka kram badle bina) s ban sakti hai. true ya false lautao.
  e.g. s="abc", t="ahbgdc" -> true     s="axc" -> false
  kyun: a..b..c t me usi kram me mil gaye. 'x' t me hai hi nahi
```

```
TASVEER (purani sheet se, same to same):

s, t ki subsequence hai kya (order same, beech ke char skip allowed). 2 pointer: i for s, j for t.
s[i]==t[j] -> i++ AUR j++ (char mila).  nahi mila -> sirf j++ (t aage scan).   // while(i<s.size() && j<t.size()): koi ek khatam -> ruko
ANT: i >= s.size() -> saare s-char order me mil gaye -> TRUE. warna FALSE.
```

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

### ▸ TRAPPING RAIN WATER (LC-42)

```
height[] me buildings ki unchai di hai, har building ki chaudai 1 hai. Baarish ke baad buildings ke beech ke gaddhon me kul kitna paani rukega, wo lautao.
  e.g. [4,2,0,3,2,5] -> 9
  kyun: beech wale bars ke upar paani = min(left ka max, right ka max) - apni height. 2+4+1+2 = 9

bar i ke upar paani = min(leftMax[i], rightMax[i]) - height[i]   (min: chhoti side se bah jaata)
O(n) space wala: leftMax[] aage se, rightMax[] peeche se, ans += min(..) - height[i]
```

```
TASVEER (purani sheet se, same to same):

CORE: bar `i` ke upar paani = min(leftMax[i], rightMax[i]) - height[i].  (min kyun: CHHOTI side se paani bah jaata.)

[A] O(n) space: leftMax[] ek pass AAGE se · rightMax[] ek pass PEECHE se · ans += min(leftMax[i], rightMax[i]) - height[i].

[B] O(1) two-pointer (arrays hata, bas 2 var):
      left=0, right=n-1;   leftMax=0, rightMax=0;   // max 0 se: abhi koi deewar dekhi hi nahi
      while (left < right):   // < : milne wali cell sabse unchi deewar, uspe paani nahi
          if (height[left] <= height[right]):          // chhoti ASLI deewar wali taraf niptao: doosri taraf badi pakki
              if (leftMax > height[left])  ans += leftMax - height[left];   // chhota -> paani
              left++;   // left ka kaam ho gaya, aage chalo
          else:
              if (rightMax > height[right]) ans += rightMax - height[right];   // chhota -> paani (rightMax tak bharega)
              right--;   // right ka kaam ho gaya, andar aao
    kyun SAFE: jis taraf ki DEEWAR chhoti, wahi taraf niptao -- doosri taraf ek badi deewar pakka khadi,
               to had isi chhoti taraf ke max se tay hoti.
    ★ MILNE KI JAGAH: dono pointer jahan milte, wo cell kabhi process nahi hota. Deewar dekh ke chalne se
      badi deewar apni jagah rukti -> dono SABSE UNCHI deewar pe milte, jahan paani hota hi nahi.
    ★ TRAP: faisla height[left] <= height[right] (DEEWAR) pe lo, leftMax <= rightMax pe NAHI.
      max pe liya to ek khamba chhoot jaata: [4,1,4,1] -> 0 aata (sahi 3).
edge: empty -> 0.

PEHLI SHEET (Aug) se bhi, same to same:

[B] O(1) two-pointer (arrays hata, bas 2 var):
      left=0, right=n-1;   leftMax=height[0], rightMax=height[n-1];
      while (left < right):
          if (leftMax <= rightMax):  leftMax  = max(leftMax,  height[left]);   ans += leftMax  - height[left];   left++;
          else:                      rightMax = max(rightMax, height[right]);  ans += rightMax - height[right];  right--;
    REFRESH pehle, ADD baad (warna naya-bada-bar pe ans NEGATIVE de deta).
    kyun SAFE: chhoti-max side ka paani sirf usi side se limit (doosri side oonchi wall already khadi).
edge: empty -> 0.
```

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

### ▸ PALINDROMIC SUBSTRINGS (LC-647)  = EXPAND AROUND CENTER

```
Ek string di hai. Usme kitni substrings (lagaatar akshar) palindrome hain, wo gino.
Akela akshar bhi palindrome gina jaata hai, aur alag jagah wali same substring alag gini jaati hai.
  e.g. "aaa" -> 6
  kyun: a, a, a (3) + aa, aa (2) + aaa (1) = 6

kitne substring palindrome (single char bhi; alag position = alag count). har CENTER se phailo
"abcba": c -> bcb -> abcba = ek center se 3. ODD center = ek char, EVEN center = do char jodi
solve: &count : warna bahar count badhta hi nahi
```

```
TASVEER (purani sheet se, same to same):

String me kitne SUBSTRING (contiguous) palindrome hain? COUNT karo.
single char bhi palindrome · same-dikhne wale par ALAG POSITION wale ALAG count.
SIGNAL: "count palindromes / palindromic substrings" -> har CENTER pe khade ho ke PHAILO.

★★ EXAMPLE (yahi yaad -> poora recall):   "abcba"
    a  b  c  b  a
          ^            "c"        <- center se shuru
       ^  ^  ^         "bcb"      <- ek kadam phaila (b==b)
    ^  ^  ^  ^  ^      "abcba"    <- aur phaila (a==a)
    => ek hi center se 3 palindrome mile -> har phailav pe count++

★ ARPAN-LINE (jisse ye derive hua): "beech wala khada ho ke dono taraf poochta --
  'tum dono same ho? aa jao mere paas'. same hue -> palindrome bana -> phir agli jodi se poochta."

DO tarah ke CENTER (dono chahiye):
   ODD  length -> center ek CHAR       ("abcba" -> c)     -> expand(i, i)
   EVEN length -> center do CHAR jodi  ("abba"  -> bb)    -> expand(i, i+1)
   n-length string me:  n odd-centers + (n-1) even-centers

TEMPLATE:
   count = 0
   for i in 0..n-1:   // har index ko center bana ke dono taraf phailao
       expand(i, i)        // odd: center ek char (khud bhi palindrome, count hoga)
       expand(i, i+1)      // even · i+1==n bhi theek: while me j<n pehle check
   return count

   expand(i, j):   // count by REFERENCE (&count), warna bahar count badhta hi nahi
       while (i >= 0 && j < n && s[i] == s[j]):   // pehle range check, phir s[i]==s[j] (warna bahar padhega)
           i--;  j++        // dono taraf phailo

★ count HAR match pe badhta (ant me ek baar nahi) -> kyunki har phailav khud ek palindrome hai.
★ Complexity: O(n^2) time, O(1) space. Koi DP/recursion NAHI chahiye
  (substring = CONTIGUOUS -> skip ka option nahi -> choice nahi -> recursion nahi.
   recursion ki khushboo SUBSEQUENCE me aati, substring me nahi).
```

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

