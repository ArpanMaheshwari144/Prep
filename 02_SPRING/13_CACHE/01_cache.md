# Cache (Spring: @Cacheable)

## Kahani: HLD wala Ravi ka profile

Ravi ka profile har second hazaaron log kholte, har baar DB tak 50 ms. HLD me DB se pehle ek dabba rakha tha
(04_HLD/SYSTEM_DESIGNS/10_distributed_cache). Spring me wo dabba ek annotation se lagta.

## Tasveer

```
@Cacheable("users") getUser(id)

call aayi -> Spring PEHLE "users" dabbe me id dhoondhta
   mila (HIT)    -> method me ghusta hi nahi, seedha value
   nahi (MISS)   -> method chalao (DB), jo aaya dabbe me daal do

naam badla    -> @CacheEvict  (dabbe se hatao)      <- HLD wala "parchi kaato"
update pe     -> @CachePut    (method chalao + dabba taaza)
```

## Dikkat -> kyun

Manual me har method me khud: "map me hai? do, warna DB se laao, map me daalo". Har jagah wahi if-else. `@Cacheable`
wahi kaam proxy se karta, method ke aage khada ho ke. Chahiye: `@EnableCaching` + `spring-boot-starter-cache`.

## Trade-off

```
local (default, app ki memory)            Redis (alag server)
sabse tez, network nahi                   ~1 ms, ek hop
sirf usi server ka, restart pe gaya       saare server ek copy dekhte, bacha rehta
TTL nahi (Caffeine me haan)               TTL + eviction
```

3 server pe local: Ravi ne naam badla, A ka cache saaf, B aur C purana dikhate. Isliye multi-server me Redis, aur
bahut garam key (IPL score) pe Redis ke aage 1-2 sec ka local cache. Cache = kaam, Redis = ek tool (Redis lock,
counter, pub/sub bhi karta).

## Asli kaam wala trap

```java
@Service
public class UserService {
    public String profile(Long id) { return this.getUser(id); }   // cache KABHI nahi chalega

    @Cacheable("users")
    public String getUser(Long id) { ... }
}
```

`@Cacheable` proxy se chalta. Same class ke andar `this.getUser()` = proxy bypass, har baar DB. Wahi `@Transactional`
wala self-invocation jaal. Isliye usercrud me cache wala method alag bean (`CacheDemoService`) me, controller usse bulata.
Aur: `@EnableCaching` bhoole to annotation chupchaap kuch nahi karta. Manual demo ka `HashMap` singleton controller me
= saare thread share, thread-safe nahi, kabhi saaf nahi.

## Tera project (usercrud)

`/cache-demo/anno/{id}`: doosri call pe "METHOD CHAL RAHA" print hi nahi hua = cache ne method se pehle jawab de diya.

## Interview line

```
I use @Cacheable so Spring checks the cache before running the method and only hits the DB on a miss;
@CacheEvict on update keeps it fresh. It works through a proxy, so a call from inside the same class
skips the cache, which is why the cached method lives in a separate bean. The default in-memory cache is
per-server, so with multiple instances I use Redis as the shared cache.
```
