# Pattern 8: Recursion + Backtracking

## Ye kya hai?

Bhai dekh — tu ek building ki 5th floor pe hai. Neeche jaana hai. Har floor pe khud ko bolta hai "ek neeche ja" — jab tak ground floor na aa jaye (base case).

Recursion = **khud ko chhoti problem de, jab tak base case na aaye.**

## 2 cheezein zaroori:

1. **Base case** — kab rukna hai (warna infinite loop, stack overflow)
2. **Recursive call** — khud ko chhoti problem de

## Kab use karte hain?

- "Saare subsets/subsequences nikaal" → pick/not-pick
- "Saare permutations nikaal" → swap based
- "Tree/Graph traverse kar" → DFS recursion se
- "Problem chhoti problem mein tod sakta hai" → recursion
- Basically — **jab problem khud mein chhoti problem contain kare**

---

## Basic Recursion — Sum, Factorial, Fibonacci, Power

Sab mein same pattern:

```
Base case: sabse chhoti problem ka answer seedha de
Recursive: badi problem = chhota kaam + chhoti problem ka answer
```

### Sum of N:
```
sum(5) = 5 + sum(4) = 5 + 4 + sum(3) = ... = 15
Base: n==1 → return 1
```

### Factorial:
```
fact(5) = 5 * fact(4) = 5 * 4 * fact(3) = ... = 120
Base: n==0 ya n==1 → return 1
```

### Fibonacci:
```
fib(5) = fib(4) + fib(3) = ... = 5
Base: n==0 → return 0, n==1 → return 1
```

### Power:
```
pow(2,5) = 2 * pow(2,4) = 2 * 2 * pow(2,3) = ... = 32
Base: exp==0 → return 1
```

---

## Array pe Recursion

Loop nahi lagana — index badhao, recursion call karo.

```
arraySum(nums, index):
  Base: index == n → return 0
  return nums[index] + arraySum(nums, index+1)
```

**index+1 se aage jao, index-1 se nahi — warna peeche jayega crash!**

---

## String pe Recursion — Reverse

```
reverse("hello"):
  pehla character 'h' alag kar
  baaki "ello" reverse kar → "olle"
  "olle" + "h" = "olleh" ✅

Base: size 0 ya 1 → return s
Recursive: reverseStr(s.substr(1)) + s[0]
```

**Empty string ka base case bhoolna mat — s.substr(1) crash karega empty pe.**

---

## IMPORTANT: Pick / Not-Pick Pattern

Ye recursion ka sabse important pattern hai — subsets, subsequences, DP sab iska base hai.

**Real Life:** Dukaan mein khada hai. Har item ke liye — LO ya MAT LO.

```
Items: [a, b, c]

Har item pe 2 choices → 2 * 2 * 2 = 8 subsets:
[], [a], [b], [c], [a,b], [a,c], [b,c], [a,b,c]
```

### Template (YE YAAD RAKH):

```
solve(index):
  // BASE CASE — sab elements dekh liye
  if(index == n):
    result mein current subset daal
    return

  // PICK — current element lo
  current.push_back(nums[index])
  solve(index + 1)

  // BACKTRACK — pick wala hatao
  current.pop_back()

  // NOT-PICK — current element mat lo
  solve(index + 1)
```

### Dry run — [1, 2] ke subsets:

```
                    []
                /        \
           pick 1       not pick 1
            [1]            []
          /     \        /     \
      pick 2  not 2  pick 2  not 2
      [1,2]   [1]    [2]      []

Result: [[1,2], [1], [2], []]
```

### Subset Sum = Target (Same template + condition):

```
solve(index):
  if(index == n):
    if(currentSum == target):    ← extra condition
      result mein daal
    return

  // PICK
  currentSum += nums[index]
  current.push_back(nums[index])
  solve(index + 1)

  // BACKTRACK
  current.pop_back()
  currentSum -= nums[index]

  // NOT-PICK
  solve(index + 1)
```

**Pick mein jo kiya — backtrack mein undo karo. Push kiya toh pop. Add kiya toh subtract.**

---

## Questions Jo Kiye

| # | Question | Type | Key trick |
|---|----------|------|-----------|
| 31 | Sum of N | Basic | n + sumN(n-1). Base: n==1 |
| 32 | Factorial | Basic | n * fact(n-1). Base: n==0 or 1 |
| 33 | Fibonacci | Basic | fib(n-1) + fib(n-2). Base: n==0→0, n==1→1 |
| 34 | Power | Basic | base * power(base, exp-1). Base: exp==0→1 |
| 35 | Array Sum Recursive | Array | nums[index] + arraySum(index+1). Base: index==n→0 |
| 36 | Reverse String | String | reverseStr(s.substr(1)) + s[0]. Base: size<=1 |
| 37 | Palindrome Recursive | String | Pichla pattern reuse kiya — reverse karke compare |
| 39 | Subsets | Pick/Not-Pick | push, recurse, pop, recurse. Base: index==n |
| 40 | Subset Sum Target | Pick/Not-Pick + condition | Same + currentSum==target check |

---

## Silly Mistakes

1. **Base case bhoolna** — infinite recursion → stack overflow crash
2. **index-1 likhna index+1 ki jagah** — peeche jayega, crash
3. **Empty string/array ka base case missing** — substr(1) ya nums[-1] crash
4. **fact * factorial(n-1) likhna** — fact variable garbage hai, n * factorial(n-1) chahiye
5. **Backtrack bhoolna** — push kiya toh pop karo, add kiya toh subtract karo
6. **Pick/not-pick mein current mein element daalna bhoolna** — recursion call hogi but current khaali rahega
7. **0! = 1 handle na karna** — factorial(0) ka base case zaroori

---

## Ek Line Mein Yaad Rakh

> Basic Recursion = **"Base case + khud ko chhoti problem de."**
> Pick/Not-Pick = **"Base case. Pick → push, recurse. Backtrack → pop. Not-pick → recurse."**
> Backtrack = **"Jo kiya wo undo karo — push kiya toh pop, add kiya toh subtract."**
