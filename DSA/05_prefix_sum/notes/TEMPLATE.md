# Template: Prefix Sum

```
Ek baar total bana. Phir koi bhi range sum O(1) mein.

prefix[0] = nums[0];
for(int i = 1; i < n; i++){
    prefix[i] = prefix[i-1] + nums[i];
}

sum(L, R) = L == 0 ? prefix[R] : prefix[R] - prefix[L-1]
```

**IMPORTANT:** L=0 toh seedha prefix[R]. L>0 toh prefix[R] - prefix[L-1].
