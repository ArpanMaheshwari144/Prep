# ThreadLocal: Har Thread Ka Apna Locker

## Scene: ek whiteboard pe sab cashier ke customer

Ek hi whiteboard pe sab cashier apne customer ka naam likhte hain. Cashier 1 ne "Arpan" likha, cashier 2 ne usi pe "Rahul"
likh diya, to cashier 1 ne Arpan ka bill Rahul ke naam bana diya. Bank me iska matlab hai kisi ko kisi aur ka account dikh
gaya. `ThreadLocal` = har cashier ka apna locker: naam ek, par har ek ko sirf apna dikhta hai.

Iska fayda ye hai ki userId ya traceId ko har method me parameter bana ke 10 layer tak bhejna nahi padta, jahan chahiye wahan
`get()` kar lo. Spring andar se yahi karta hai: `SecurityContextHolder` (JWT wala logged-in user), `@Transactional`, aur logs
ka MDC traceId. Chala ke dekha:

```
remove() bhoola, agle customer ko dikha:  Arpan    <- LEAK
finally me remove(), agle ko dikha:       null     <- saaf
request thread:  Arpan
async thread:    null                              <- doosre thread pe locker khaali
```

## Trap 1: pool

Pool me cashier naya nahi aata, wahi purana dobara aata hai. Shift ke baad locker saaf nahi kiya, to agle customer ko Arpan ka
naam dikh gaya. Isliye:

```java
try { customer.set(userId); /* kaam */ }
finally { customer.remove(); }   // pool me wapas jaane se pehle locker saaf
```

JWT filter request khatam hone pe SecurityContext saaf karta hai, wo saaf karna yahi `remove()` hai.
([ThreadLocalDemo](practical/ThreadLocalDemo.java) me bhi DEMO 2a / 2b yahi dikhata hai.)

## Trap 2: async

Kaam `@Async` ya `CompletableFuture` pe bheja (file 10, 11), to wo doosre cashier ke paas gaya, aur uska locker khaali hai.
Isiliye async me `SecurityContextHolder` khaali aata hai aur log se traceId gaayab ho jaata hai. Ilaaj: value pehle nikaal ke
pass karo, ya Spring ka `TaskDecorator` lagao jo context naye thread me copy kar de.

## Interview line

```
ThreadLocal gives each thread its own copy behind one name, so request context like
userId, traceId or the SecurityContext doesn't have to be passed through every method. With
thread pools I always remove() in finally, otherwise the reused thread leaks the previous
request's value. And it doesn't cross threads: in @Async or CompletableFuture the value is
null, so I pass it explicitly or use a TaskDecorator.
```
