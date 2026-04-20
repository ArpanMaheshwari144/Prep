# Template: Binary Search

## Simple
```
Sorted hai? Aadha kaat. <= lagao.

int low = 0, high = n-1;
while(low <= high){
    int mid = low + (high-low)/2;
    if(nums[mid] == target) return mid;
    else if(nums[mid] < target) low = mid+1;
    else high = mid-1;
}
return -1;
```

## Boundary Finding
```
Target mila toh ruko mat — aur dhundho. 2 BS lagao.

Pehli position: if(nums[mid] == target){ ans[0] = mid; high = mid-1; }
Aakhri position: if(nums[mid] == target){ ans[1] = mid; low = mid+1; }
```

## BS on Answer
```
Answer ki range pe BS. Helper function se check.

int low = min_possible, high = max_possible;
while(low <= high){
    int mid = low + (high-low)/2;
    if(canDo(mid)){ ans = mid; high = mid-1; }
    else{ low = mid+1; }
}

CONTINUOUS (Ship): sum karte jao, cross → day++, sum=weights[i]. day=1.
INDEPENDENT (Koko): ceil(pile/speed) add karo. hours=0. (double) cast.
```

**IMPORTANT:** BS mein hamesha `<=`. `(low+high)/2` nahi, `low+(high-low)/2`.
