# Template: Two Pointer

## Opposite Direction
```
Dono sides se aao. Swap ya compare.

int low = 0, high = n-1;
while(low < high){
    // swap ya compare
    low++;
    high--;
}
```

**Reverse:** swap(nums[low], nums[high])
**Palindrome:** s[low] != s[high] → false
**Two Sum Sorted:** sum == target → found, < → low++, > → high--
**Container Water:** area = min(h[low],h[high]) * (high-low), chhoti wall wala move

## Same Direction
```
Slow builder, fast scanner.

int slow = 0, fast = 0;
while(fast < n){
    if(kaam ka hai){
        assign ya swap
        slow++;
    }
    fast++;
}
```

**Remove duplicates:** nums[fast] != nums[slow] → slow++, assign
**Move zeroes:** nums[fast] != 0 → swap, slow++
**Remove element:** nums[fast] != val → assign, slow++
**IMPORTANT:** Pehle assign/swap, PHIR slow++.

## Two Arrays
```
End se start. Bada element peeche.

int i = m-1, j = n-1, k = m+n-1;
while(i >= 0 && j >= 0){
    bada wala nums1[k] pe daalo, k--
}
while(j >= 0){ nums1[k] = nums2[j]; j--; k--; }
```

**IMPORTANT:** Equal case handle kar — `<=` ya `>=` use kar.
