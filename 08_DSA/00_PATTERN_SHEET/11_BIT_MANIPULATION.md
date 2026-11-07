## PATTERN 11 — BIT MANIPULATION (XOR)

```
 BROAD IDEA: XOR: a^a=0 (pair cancel), a^0=a (single bacha). pairs khud gayab, extra memory nahi.
 ★ MILTE-JULTE (sorted / 1..n / dohra): 00_SHURU me NAAM-SE-CONFUSE D dekh, XOR lagane se PEHLE.

```

#### ── FAMILY: XOR-cancel (pair khud kat, akela / missing bacha = ans; Missing-Number bhi; Single-Number-II ALAG: wahan baaki 3 baar aate, XOR se nahi katte -> har bit ki ginti % 3) ──

```
 KAB: har number 2 baar, ek akela · extra memory nahi
```

### ▸ SINGLE NUMBER (LC-136)

```
Array me har number do baar aata hai, sirf ek number ek baar. Wo number lautao. O(n) time aur O(1) extra jagah me.
  e.g. [4,1,2,1,2] -> 4
  kyun: 4^1^2^1^2: 1^1 = 0, 2^2 = 0, bacha 4
```

```cpp
int singleNumber(vector<int> &nums) {       // sab 2 baar, ek 1 baar -> sab XOR = wahi ek
    int XORR = nums[0];                     // nums[0] se shuru, isliye loop 1 se
    for (int i = 1; i < nums.size(); i++)
        XORR ^= nums[i];                    // ★ a^a = 0 pair kat jaata, a^0 = a
    return XORR;                            // O(n), O(1), koi map nahi
}
```


