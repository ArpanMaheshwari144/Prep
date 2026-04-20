# Template: DP (Dynamic Programming)

## Top-Down (Memoization) — Recursion + Yaad rakh
```
int solve(int n, vector<int>& dp) {
    if(base case) return value;
    if(dp[n] != -1) return dp[n];        // yaad hai → return
    
    int ans = solve(n-1, dp) + ...;       // recursion
    return dp[n] = ans;                    // yaad rakh
}

int main_fn(int n) {
    vector<int> dp(n+1, -1);              // n+1 size!
    return solve(n, dp);
}
```

## Bottom-Up (Tabulation) — Loop
```
dp[0] = base. dp[1] = base.
for(i = 2 to n):
    dp[i] = dp[i-1] + dp[i-2]
return dp[n]
```

## Steps:
```
1. Recursion likho
2. dp array bana (n+1, -1)
3. Check: dp[n] != -1 → return
4. Store: dp[n] = ans
= DP ban gaya!
```

**IMPORTANT:** dp size n+1. Base case sahi rakho (n==0 → 0 ya 1 soch ke decide).

## Coin Change
```
solve(amount):
    amount == 0 → return 0
    amount < 0  → return INT_MAX
    dp[amount] != -1 → return dp[amount]
    
    minAns = INT_MAX
    for each coin:
        res = solve(amount - coin)
        if(res != INT_MAX) minAns = min(minAns, 1 + res)   ← overflow check!
    return dp[amount] = minAns
```

## Min Cost Climbing Stairs
```
solve(i):
    i >= n → return 0 (top pahunch gaya)
    dp[i] != -1 → return dp[i]
    
    ans = cost[i] + min(solve(i+1), solve(i+2))
    return dp[i] = ans

main: min(solve(0), solve(1))   ← dono start points try
```

## 0/1 Knapsack
```
solve(i, W):
    i >= n || W == 0 → return 0
    dp check
    pick = INT_MIN
    if(weights[i] <= W): pick = values[i] + solve(i+1, W-weights[i])
    not_pick = solve(i+1, W)
    return dp[i][W] = max(pick, not_pick)
```

## LCS
```
solve(i, j):
    i < 0 || j < 0 → return 0
    dp check
    if(s1[i] == s2[j]): return dp[i][j] = 1 + solve(i-1, j-1)
    else: return dp[i][j] = max(solve(i-1, j), solve(i, j-1))
```

## DP Pattern:
```
Total ways → ADD:    solve(a) + solve(b)
Min cost   → MIN:    cost + min(solve(a), solve(b))
Min coins  → MIN:    1 + min(solve for each choice)
Max value  → MAX:    value + max(pick, not_pick)
Match/No   → if-else: match → 1+solve(both-1). No → max(solve one-1)
```
