# Template: Recursion + Backtracking

## Basic Recursion
```
Base case + khud ko chhoti problem de.

solve(n):
    if(base case) return answer
    return kuch_kaam + solve(n-1)
```

## Pick / Not-Pick (Subsets, Subsequences)
```
Base case. Pick → push, recurse. Backtrack → pop. Not-pick → recurse.

solve(index):
    if(index == n):
        result mein current daal
        return

    // PICK
    current.push_back(nums[index])
    solve(index + 1)

    // BACKTRACK
    current.pop_back()

    // NOT-PICK
    solve(index + 1)
```

**Backtrack = jo kiya wo undo karo. Push kiya toh pop. Add kiya toh subtract.**
**Subset Sum Target = same + base case mein currentSum == target check.**
