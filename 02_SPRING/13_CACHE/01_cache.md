# Cache (Spring: @Cacheable)

## Scene: HLD wala Ravi ka profile

Ravi ka profile har second hazaaron log kholte hain, aur har baar DB tak jaane me 50 ms lagte hain. HLD me humne DB se pehle ek dabba rakha
tha ([10_distributed_cache](../../04_HLD/SYSTEM_DESIGNS/10_distributed_cache/10_distributed_cache.md)). Spring me wahi dabba bas ek line
`@Cacheable("users")` se lag jaata hai. Iske liye `@EnableCaching` aur `spring-boot-starter-cache` dono chahiye.

```
@Cacheable("users") getUser(id)

call aayi -> Spring PEHLE "users" dabbe me id dhoondhta
   mila (HIT)    -> method me ghusta hi nahi, seedha value
   nahi (MISS)   -> method chalao (DB), jo aaya dabbe me daal do

naam badla    -> @CacheEvict  (dabbe se hatao)      <- HLD wala "parchi kaato"
update pe     -> @CachePut    (method chalao + dabba taaza)
```

## Chala ke dekha (plain Java proxy, wahi kaam jo Spring andar karta)

```
controller -> proxy.getUser(7), pehli baar:
   MISS
   METHOD CHAL RAHA (DB 50 ms) id=7
controller -> proxy.getUser(7), doosri baar:
   HIT, method me ghusa hi nahi
```

Spring method ke aage proxy bana ke khada hota hai. HIT pe method chalta hi nahi, value seedha dabbe se aati hai. MISS pe method chalta hai
aur jo DB se aaya wo dabbe me chala jaata hai. Manual cache me yahi "map me hai? de do, warna DB se laao aur map me daalo" wala if-else har
method me khud likhna padta.

## Chhoti dikkat: 3 server, teen alag dabbe

Local cache default hai, aur ye app ki apni memory me rehta hai. Sabse tez hai kyunki network nahi lagta, par sirf usi server ka hota hai aur
restart pe khatam. Maan lo 3 server pe local cache hai. Ravi ne naam badla, to server A ka dabba saaf ho gaya, par B aur C abhi bhi purana naam
dikha rahe hain.

Hal: multi-server me Redis lo. Wo alag server hai, lagbhag 1 ms lagta hai kyunki ek network hop hai, par saare server ek hi copy dekhte hain,
aur TTL aur eviction dono milte hain. Bahut garam key (IPL score) ke liye Redis ke aage 1-2 sec ka local cache bhi rakh dete hain. Local me
TTL chahiye to Caffeine lagao.

## Tera project (usercrud)

`/cache-demo/anno/{id}` pe doosri call me "METHOD CHAL RAHA" print hi nahi hua, matlab cache ne method chalne se pehle hi jawab de diya.
Cache wala method alag bean `CacheDemoService` me hai, aur controller usi bean ko bulata hai (wajah neeche trap me).

## Prod wala trap: this se bulaya to cache kabhi nahi

```java
@Service
public class UserService {
    public String profile(Long id) { return this.getUser(id); }   // cache KABHI nahi chalega

    @Cacheable("users")
    public String getUser(Long id) { ... }
}
```

```
controller -> proxy.profile(9) jo andar this.getUser(9) bulata, do baar:
   METHOD CHAL RAHA (DB 50 ms) id=9
   METHOD CHAL RAHA (DB 50 ms) id=9      <- cache kabhi nahi laga
```

Same class ke andar `this.getUser()` bulaya to call proxy ke paas se guzarti hi nahi, isliye har baar DB jaati hai. Ye wahi `@Transactional`
wala self-invocation jaal hai. Aur `@EnableCaching` lagana bhool gaye to annotation chup-chaap kuch nahi karta.
