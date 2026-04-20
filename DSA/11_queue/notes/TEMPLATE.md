# Template: Queue

## BFS Template (Trees / Graphs)
```
queue mein start node daal
while queue khaali nahi:
    int size = q.size()         ← PEHLE size save
    for(i = 0 to size):
        node nikaal (front + pop)
        process karo
        neighbors/children push karo
```

## Queue Operations
```
q.push(x)    — peeche se daal
q.front()    — aage wala dekh
q.pop()      — aage se nikaal
q.empty()    — khaali hai?
q.size()     — kitne elements
```

**IMPORTANT:** front() aur pop() alag hain — front() sirf dikhata hai, pop() nikalta hai. Dono karo.
