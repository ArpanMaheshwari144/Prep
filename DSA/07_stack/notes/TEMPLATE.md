# Template: Stack

## Valid Parentheses
```
Open → push. Close → top se match. End mein khaali → true.

for(char ch : s){
    if(open bracket) st.push(ch);
    else{
        if(!st.empty() && match) st.pop();
        else return false;
    }
}
return st.empty();
```

## Next Greater Element
```
Stack mein INDEX daalo. Bada aaye → while se pop, answer daalo.

for(int i = 0; i < n; i++){
    while(!st.empty() && nums[st.top()] < nums[i]){
        ans[st.top()] = nums[i];
        st.pop();
    }
    st.push(i);
}
```

**IMPORTANT:** Value nahi INDEX push. if nahi WHILE. Har element push — chhota bhi.

## Min Stack
```
2 stacks: st1 (normal), st2 (min track).

Push:
  st1.push(val)
  if(!st2.empty() && val > st2.top()) → st2.push(st2.top())  // min same
  else → st2.push(val)                                         // naya min ya pehla

Pop:    st1.pop(). st2.pop(). (dono saath)
Top:    st1.top() (st2 NAHI — wo min hai!)
getMin: st2.top()
```

**IMPORTANT:** st2 khaali pe st2.top() → CRASH. Pehle empty check.
