# Template: Linear Scan

```
Ek loop. Kuch variables. Har element pe decision.

int variable = starting_value;
for(int i = 0; i < n; i++){
    if(condition){
        variable update
    }
}
return variable;
```

**Max dhundh:** variable = INT_MIN, nums[i] > variable → update
**Sum nikaal:** variable = 0, variable += nums[i]
**Count kar:** variable = 0, nums[i] == target → variable++
**Sorted check:** nums[i-1] > nums[i] → return false
**Kadane's:** sum += nums[i], maxSum update, sum < 0 → sum = 0. Pehle update PHIR reset.
