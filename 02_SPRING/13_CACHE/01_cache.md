# Caching — request tez karo (manual + @Cacheable) [Spring]

> "Baar-baar wahi mehnga kaam (DB call) mat karo — pehli baar ka result yaad rakho, agli baar seedha do."
> usercrud pe live kiya (25-Aug): ek endpoint 2 baar — pehli baar DB (200ms), doosri baar cache (turant).

---

## 1-LINE KYA
```
CACHE = beech me ek tez dabba (memory). request -> cache dekho:
          HIT  (mila)     -> seedha wapas, DB tak gaye hi nahi   (fast)
          MISS (nahi mila) -> DB jao -> result cache me daalo -> wapas  (slow, sirf pehli baar)
use: baar-baar padha jaane wala + kam badalne wala data (user profile, product, config).
```

---

## ═══ FLOW (asli picture) ═══
```
        ┌─────────┐  HIT (fast)   ┌─────────┐
 req -> │  CACHE  │ ------------> │ return  │
        └────┬────┘               └─────────┘
             │ MISS
             v
        ┌─────────┐  data   ┌──────────────┐
        │   DB    │ ------> │ cache me daal │ -> return
        └─────────┘         └──────────────┘
   agli baar same req -> CACHE me mil jaata -> DB skip.
```

---

## ═══ TAREEKA 1 — MANUAL (mechanism samajhne ke liye) ═══
Cache = ek `HashMap`. Khud check-store karo (guts khule):

```java
private Map<Long, String> cache = new HashMap<>();

@GetMapping("/cache-demo/{id}")
public String getUser(@PathVariable Long id) throws InterruptedException {
    if (cache.containsKey(id)) {                     // 1. cache check
        return cache.get(id);                        //    HIT -> seedha (DB skip)
    }
    Thread.sleep(200);                               // 2. MISS -> DB (slow)
    String nameFromDb = "User-" + id;
    cache.put(id, nameFromDb);                        // 3. cache me daalo
    return nameFromDb;
}
```
Live dikha: pehli hit MISS (200ms atka), doosri hit HIT (turant).

---

## ═══ TAREEKA 2 — @Cacheable (production ka asli tarika) ═══
Same kaam, par tu koi if-check nahi likhta — Spring karta:

```java
// main class: @EnableCaching   (warna @Cacheable silently ignore)
// pom.xml: spring-boot-starter-cache   (warna koi CacheManager nahi milta)

@Service
public class CacheDemoService {

    @Cacheable("users")                              // <-- yehi sab kuch
    public String getUser(Long id) throws InterruptedException {
        System.out.println(">> METHOD CHAL RAHA -- sirf MISS pe dikhega");
        Thread.sleep(200);                            // DB slow
        return "User-" + id;
    }
}
```

`@Cacheable("users")` ka matlab: **method chalane se PEHLE** Spring `users` dabbe me `id` dhoondhta —
- HIT -> method me ghusta hi nahi, cached value seedha deta.
- MISS -> method chalao, return-value dabbe me daal do.

Controller sirf service ko call karta (koi cache-logic nahi):
```java
@Autowired private CacheDemoService svc;
@GetMapping("/cache-demo/anno/{id}")
public String anno(@PathVariable Long id) throws InterruptedException { return svc.getUser(id); }
```

**LIVE PROOF (jo dekha):** 2nd call pe `METHOD CHAL RAHA` line PRINT hi nahi hui — matlab `@Cacheable`
ne method me ghusne se pehle hi cached value laut di. Manual `if(containsKey) return` ka kaam annotation ne kiya.

---

## ★ TRAP / GOTCHA (interview me poochte)
```
1. SELF-INVOCATION (sabse-asked, @Async jaisa): @Cacheable PROXY se chalta. SAME class ke andar se
   this.getUser() call -> proxy bypass -> cache NAHI chalta. isliye method ALAG bean (@Service) me,
   dusre bean se call. [maine isi liye service banaya]
2. @EnableCaching bhoole -> annotation silently ignore.
3. spring-boot-starter-cache dep na ho -> koi CacheManager nahi -> error / cache nahi banta.
4. STALE data: data badla par cache purana de raha -> @CacheEvict / @CachePut se hatao/update karo, ya TTL do.
5. default cache = in-memory (ConcurrentHashMap), single-node. multi-node -> REDIS (sab servers ek cache share karein).
```

## ★ RELATED annotations
```
@Cacheable  -> HIT? return cached : method chalao + store   (padhne wale GET pe)
@CacheEvict -> cache se hatao (jab data badle/delete ho)     (update/delete pe)
@CachePut   -> method HAMESHA chalao + result cache me daalo  (update pe fresh rakho)
```

## ★ CONNECT (HLD se jodo)
```
in-app cache (@Cacheable, ek server ki memory)  -> chhota, single-node.
distributed cache (REDIS/Memcached)             -> saare servers ek cache share -> HLD me yahi.
LB + cache: request -> LB -> koi server -> cache-check -> (miss) DB.  (aage full HLD me jodenge)
```

## INTERVIEW LINE
```
"Caching se baar-baar wahi mehnga read (DB call) bachta -- pehli baar ka result memory me rakh lete,
 agli baar HIT pe DB skip. Spring me @Cacheable + @EnableCaching; gotcha self-invocation (proxy bypass)
 isliye alag bean se call. Stale ke liye @CacheEvict/TTL, aur multi-node pe Redis distributed cache.
 Maine usercrud pe kiya -- 2nd call pe method chala hi nahi, cache ne short-circuit kiya."
```

### Dobara kaise
```
main: @EnableCaching + pom starter-cache -> service method pe @Cacheable("name") -> ALAG bean se call.
run -> 2nd same request pe method-body log nahi aata (HIT) + turant response.
```

[← Spring README](../README.md)
