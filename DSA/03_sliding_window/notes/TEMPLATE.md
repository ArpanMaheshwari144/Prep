# Template: Sliding Window

## Fixed Size
```
ADD karo. SHRINK karo. UPDATE karo. j++. ORDER MAT BADLO.

int i = 0, j = 0, sum = 0;
while(j < n){
    sum += nums[j];              // 1. ADD
    if(j-i+1 > k){              // 2. SHRINK
        sum -= nums[i]; i++;
    }
    if(j-i+1 == k){             // 3. UPDATE
        maxSum = max(maxSum, sum);
    }
    j++;                         // 4. j++
}
```

**Window size:** j - i + 1

## Variable Size
```
ADD karo. SHRINK (while). UPDATE (if valid). j++.

while(j < n){
    mp[s[j]]++;                  // 1. ADD
    while(condition toote){      // 2. SHRINK (while!)
        mp[s[i]]--;
        if(mp[s[i]] == 0) mp.erase(s[i]);
        i++;
    }
    if(valid){                   // 3. UPDATE
        ans = max(ans, j-i+1);
    }
    j++;                         // 4. j++
}
```

**No repeat:** while mp[s[j]] > 1 → shrink. Update har valid window.
**K unique:** while mp.size() > k → shrink. Update jab mp.size() == k.
**IMPORTANT:** if nahi WHILE — variable mein ek baar shrink kaafi nahi.
