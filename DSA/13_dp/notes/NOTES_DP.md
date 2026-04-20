# Pattern 13: Dynamic Programming (DP)

## Ye kya hai?

Bhai dekh — **DP = Recursion + Yaad rakh.** Koi naya concept nahi. Recursion mein same cheez baar baar calculate hoti hai. DP mein **ek baar calculate, yaad rakh, dobaara puche → seedha answer.**

## Kyun chahiye jab Recursion hai?

```
Fibonacci Recursion:
fib(5) → fib(4) + fib(3)
         fib(3)+fib(2)   fib(2)+fib(1)
         ...same cheez baar baar!

Recursion: O(2^n) — fib(50) mein crores calculations. SLOW.
DP:        O(n)   — fib(50) mein 50 calculations. FAST.
```

## 2 Tarike:

### 1. Top-Down (Memoization) — Recursion + array store
```
Pehle recursion likho (ye tu kar chuka)
Phir dp array add karo:
  - dp[n] already hai → return (yaad hai)
  - Nahi hai → calculate, dp[n] mein store, return
```

### 2. Bottom-Up (Tabulation) — Loop se chhote se bade
```
dp[0] = base, dp[1] = base
for i = 2 to n:
    dp[i] = dp[i-1] + dp[i-2]
return dp[n]
```

## DP kaise likhein — Steps:

```
Step 1: Recursion likho (base case + recursive call)
Step 2: dp array bana (n+1 size, -1 se fill)
Step 3: Check add karo (dp[n] != -1 → return)
Step 4: Store karo (dp[n] = ans)
Bas. DP ban gaya!
```

## Template — Top-Down (Memoization):
```
int solve(int n, vector<int>& dp) {
    if(base case) return value;
    if(dp[n] != -1) return dp[n];       // yaad hai → return
    
    int ans = solve(n-1, dp) + ...;      // recursion
    return dp[n] = ans;                   // yaad rakh
}

int main_function(int n) {
    vector<int> dp(n+1, -1);             // n+1 size!
    return solve(n, dp);
}
```

**IMPORTANT:** dp size `n+1` rakho — `dp[n]` access karna hai toh n+1 chahiye.

---

## Q52: Climbing Stairs — Pehla DP Question

**Problem:** n steps ki ladder. 1 ya 2 step chadh sakta. Kitne tarike?

**Soch:** n pe pahunchne ke liye — ya (n-1) se 1 step aaya, ya (n-2) se 2 step.
```
ways(n) = ways(n-1) + ways(n-2)    ← Fibonacci!
```

**Base cases:**
```
n == 0 → return 1 (already ground pe — ek tarika: kuch mat kar)
n == 1 → return 1 (sirf 1 step)
```

**GALTI THI:** n==0 pe return 0 kiya tha → n=2 ka answer 1 aaya (galat). Sahi: return 1.

**Dry Run:**
```
climbStairs(5):
  dp = [-1,-1,-1,-1,-1,-1]

  solve(5) = solve(4) + solve(3)
  solve(4) = solve(3) + solve(2)
  solve(3) = solve(2) + solve(1)
  solve(2) = solve(1) + solve(0) = 1 + 1 = 2. dp[2]=2.
  solve(3) = 2 + 1 = 3. dp[3]=3.
  solve(4) = 3 + 2 = 5. dp[4]=5.      ← dp[2] yaad tha, dobaara nahi kiya!
  solve(5) = 5 + 3 = 8. dp[5]=8.      ← dp[3] yaad tha!

  Answer: 8 ✅
```

---

## Questions Jo Kiye

| # | Question | Type | Key Trick |
|---|----------|------|-----------|
| 52 | Climbing Stairs | DP (Memo) | ways(n) = ways(n-1) + ways(n-2). Base: n==0→1, n==1→1 |

## Q53: Coin Change — Minimum coins se amount banao

**Problem:** Coins diye hain, minimum kitne coins se amount banayega?

**Soch:** Har coin try kar — `1 + solve(amount - coin)`. Sab mein se min lo.

```
Coins: [1, 2, 5], Amount: 11

solve(11) = 1 + min(solve(10), solve(9), solve(6))

Base: amount == 0 → 0 (ban gaya!)
      amount < 0 → INT_MAX (impossible!)
```

**INT_MAX OVERFLOW PROBLEM:**
```
solve(-1) returns INT_MAX.
1 + INT_MAX = OVERFLOW → negative ban jaata!

Fix: solve ka result pehle check — INT_MAX hai toh 1+ mat karo, skip.
```

**Dry Run:**
```
Coins: [1, 2], Amount: 3

solve(3): min(1+solve(2), 1+solve(1))
solve(2): min(1+solve(1), 1+solve(0)) = min(1+1, 1+0) = 1
solve(1): min(1+solve(0), 1+solve(-1)) = min(1, INT_MAX skip) = 1
solve(3) = min(1+1, 1+1) = 2 ✅ (coins: 1+2=3)
```

**Galtiyan:**
1. **amount==0 pe return 1** — 0 chahiye (koi coin nahi laga)
2. **amount<0 missing** — infinite recursion
3. **amount<0 pe return -1** — 1+(-1)=0, impossible 0 coins dikhega
4. **1+INT_MAX overflow** — pehle check karo result INT_MAX toh skip
5. **ans=0 default** — impossible case mein 0 min mein aayega. INT_MAX rakho

---

## Q54: Min Cost Climbing Stairs — Minimum cost mein top pahuncho

**Problem:** Har step ka cost hai. 1 ya 2 step chadh sakta. Minimum cost se top pahuncho.

**Climbing Stairs vs Min Cost:**
```
Climbing: kitne TARIKE → solve(n-1) + solve(n-2) (ADD)
Min Cost: kam COST     → cost[i] + min(solve(i+1), solve(i+2)) (MIN)
```

**Dry Run:**
```
cost = [10, 15, 20]

solve(0) = 10 + min(solve(1), solve(2))
solve(1) = 15 + min(solve(2), solve(3))
solve(2) = 20 + min(solve(3), solve(4))
solve(3) = i>=n → return 0 (top pahunch gaya, koi cost nahi)

solve(2) = 20 + 0 = 20
solve(1) = 15 + min(20, 0) = 15
solve(0) = 10 + min(15, 20) = 25

min(solve(0), solve(1)) = min(25, 15) = 15 ✅
```

**Base case:** `i >= n → return 0` (top pe koi step nahi, koi cost nahi)

**Galtiyan:**
1. **solve(i+1) dono jagah likh diya** — haha copy paste. Ek i+1, ek i+2.
2. **Loop lagane ki koshish** — loop nahi, sirf 2 choices (1 step ya 2 step)

---

## DP Pattern Summary

| Question | Choices | Operation | Base |
|---|---|---|---|
| Climbing Stairs | solve(n-1), solve(n-2) | ADD (total ways) | n==0→1, n==1→1 |
| Coin Change | solve(amt-coin) for each coin | MIN (min coins) | amt==0→0, amt<0→INT_MAX |
| Min Cost Stairs | solve(i+1), solve(i+2) | MIN (min cost) | i>=n→0 |

**Sab mein same template:**
```
Base case check → dp check → choices try (loop ya 2 calls) → min/max/add → dp store → return
```

---

## Q55: LIS — Longest Increasing Subsequence

**Problem:** Array mein sabse lambi increasing subsequence (continuous nahi, order same).

**Soch:** Pick/not-pick! Har element ke liye — lo ya mat lo. Sirf tab lo jab prev se bada ho.

```
[10, 9, 2, 5, 3, 7, 101, 18]
LIS = [2, 3, 7, 101] ya [2, 5, 7, 101] → length 4
```

**Template:**
```
solve(i, prevIdx):
  i >= n → return 0
  
  pick = 0
  if(prevIdx == -1 || nums[i] > nums[prevIdx]):
    pick = 1 + solve(i+1, i)
  not_pick = solve(i+1, prevIdx)
  
  return max(pick, not_pick)
```

**Galtiyan:**
1. **prevIdx=-1 pe nums[-1] access** — pehle -1 check karo
2. **not-pick mein i same rakha i+1 ki jagah** — aage nahi badha, infinite recursion
3. **pick mein 1+ bhoolna** — element liya toh count badha
4. **pick = INT_MAX default** — max mein hamesha jeetega. INT_MIN rakho
5. **return maxAns likha ans ki jagah** — variable confusion
6. **dp[i][prevIdx] check, dp[i][prevIdx+1] store** — check aur store SAME jagah karo!
7. **prevIdx = -1 pe dp[-1] access** — crash. Pehle prevIdx != -1 check karo

---

## Q56: 0/1 Knapsack — Maximum value bharde bag mein

**Problem:** Bag capacity W. Items mein weight aur value. Maximum value lo. Har item 0 ya 1 baar (aadha nahi).

**Soch:** Pick/not-pick! Weight fit ho toh pick, warna not-pick.

```
solve(i, W):
  base: i >= n → return 0
  pick (agar weights[i] <= W): values[i] + solve(i+1, W-weights[i])
  not-pick: solve(i+1, W)
  return max(pick, not-pick)
```

**Dry Run:**
```
weights=[1,2,3], values=[6,10,12], W=5

solve(0,5): max(6+solve(1,4), solve(1,5))
solve(1,4): max(10+solve(2,2), solve(2,4))
solve(2,2): 3>2 → only not-pick → 0
solve(2,4): 12+solve(3,1)=12
solve(1,4) = max(10+0, 12) = 12
solve(1,5) = max(10+12, 12) = 22
solve(0,5) = max(6+12, 22) = 22 ✅
```

**First attempt mein solve hua! Pick/not-pick pattern same — LIS, Subsets sab jaisa.**

---

## Q57: LCS — Longest Common Subsequence

**Problem:** Do strings. Sabse lambi common subsequence (order same, continuous nahi).

```
s1 = "abcde", s2 = "ace" → "ace" → length 3
```

**Soch:** Dono strings ke end se. 2 cases:
```
Match:    s1[i] == s2[j] → 1 + solve(i-1, j-1)  (dono mein se hata)
No match: max(solve(i-1, j), solve(i, j-1))       (ek mein se hata)
```

**IMPORTANT:** Match pe sirf pick. No match pe sirf not-pick. Dono saath mat karo.

**Dry Run:**
```
s1="ace", s2="abce"  i=2, j=3

e==e → 1 + solve(1,2)
c==c → 1 + solve(0,1)
a!=b → max(solve(-1,1), solve(0,0))
a==a → 1 + solve(-1,-1) = 1
max(0, 1) = 1

1 + 1 + 1 = 3 ✅
```

**Galtiyan:**
1. **Match pe pick + not-pick dono** — kaam karta lekin extra calls. Match pe sirf pick.
2. **Not-pick puri tarah hataya** — no match pe kuch return nahi, INT_MIN. Dono chahiye if-else mein.

---

## DP Medium ✅ DONE

## DP Pattern Summary (Final):

| Question | Choices | Operation | Base |
|---|---|---|---|
| Climbing Stairs | solve(n-1), solve(n-2) | ADD (total ways) | n==0→1, n==1→1 |
| Coin Change | solve(amt-coin) for each | MIN (min coins) | amt==0→0, amt<0→INT_MAX |
| Min Cost Stairs | solve(i+1), solve(i+2) | MIN (min cost) | i>=n→0 |
| LIS | pick(i+1,i), not-pick(i+1,prev) | MAX (longest) | i>=n→0 |
| Knapsack | pick(i+1,W-w[i]), not-pick(i+1,W) | MAX (max value) | i>=n→0, W==0→0 |
| LCS | match: solve(i-1,j-1), no match: solve(i-1,j)/solve(i,j-1) | MAX (longest) | i<0 or j<0→0 |

---

## Galtiyan

1. **n==0 return 0** — ground pe khade ho, 1 tarika hai (kuch mat karo). return 1.
2. **dp size n** — dp[n] access karna hai toh n+1 chahiye. Out of bounds.
3. **Har call mein naya dp array** — ek hi dp sab calls share karein. Alag function mein bana.
4. **Memo check bhoolna** — dp[n] != -1 check nahi kiya → same kaam dobaara.

---

## Ek Line Mein Yaad Rakh

> DP = **"Recursion likho. dp array bana. Check add karo (yaad hai?). Store karo (yaad rakh). Bas."**
