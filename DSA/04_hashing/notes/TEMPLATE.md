# Template: Hashing

```
"Mera complement map mein hai? Haan → mil gaya. Nahi → khud ko daal de."

unordered_map<int, int> mp;
for(int i = 0; i < n; i++){
    int complement = target - nums[i];
    if(mp.count(complement)){
        return {mp[complement], i};
    }
    mp[nums[i]] = i;
}
```

**Two Sum Unsorted:** complement = target - nums[i]. Map mein dhundh.
**Duplicate check:** HashSet — set.count(nums[i]) true → duplicate.
