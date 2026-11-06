# Cache (Spring: @Cacheable)

## Kahani: HLD wala Ravi ka profile

Ravi ka profile har second hazaaron log kholte hain, aur har baar DB tak jaane me 50 ms lagte hain. HLD me humne DB se pehle ek dabba rakha tha
(04_HLD/SYSTEM_DESIGNS/10_distributed_cache). Spring me wahi dabba bas ek annotation se lag jaata hai.

## Tasveer

```
@Cacheable("users") getUser(id)

call aayi -> Spring PEHLE "users" dabbe me id dhoondhta
   mila (HIT)    -> method me ghusta hi nahi, seedha value
   nahi (MISS)   -> method chalao (DB), jo aaya dabbe me daal do

naam badla    -> @CacheEvict  (dabbe se hatao)      <- HLD wala "parchi kaato"
update pe     -> @CachePut    (method chalao + dabba taaza)
```

Call aate hi Spring pehle "users" dabbe me wo id dhoondhta hai. Mil gayi (HIT) to method ke andar ghusta hi nahi, seedha value lauta deta hai. Nahi mili (MISS) to method chalata hai, DB se data laata hai, aur jo aaya use dabbe me daal deta hai. Naam badla to `@CacheEvict` us entry ko dabbe se hata deta hai, ye HLD wala "parchi kaato" hai. Aur `@CachePut` method bhi chalata hai aur dabba bhi taaza kar deta hai.

## Dikkat aur ye kyun

Manual cache me har method me khud likhna padta hai: "map me hai? to de do, warna DB se laao aur map me daal do". Har jagah wahi if-else. `@Cacheable`
yahi kaam proxy se karta hai, method ke aage khada ho ke. Iske liye `@EnableCaching` aur `spring-boot-starter-cache` dono chahiye.

## Trade-off

Local cache default hai, aur ye app ki apni memory me rehta hai. Ye sabse tez hai kyunki network nahi lagta. Par ye sirf usi server ka hota hai, aur restart pe khatam ho jaata hai. Isme TTL nahi hota, par Caffeine lagao to TTL mil jaata hai.

Redis ek alag server hai. Isme lagbhag 1 ms lagta hai, kyunki ek network hop hai. Par saare server ek hi copy dekhte hain, aur restart pe bhi data bacha rehta hai. Isme TTL aur eviction dono milte hain.

Maan lo 3 server pe local cache hai. Ravi ne naam badla, to server A ka cache saaf ho gaya, par B aur C abhi bhi purana naam dikhate hain. Isliye multi-server setup me Redis lete hain, aur
bahut garam key (jaise IPL score) ke liye Redis ke aage 1-2 sec ka local cache bhi rakh dete hain. Yaad rakh, cache ek kaam hai aur Redis ek tool hai. Redis lock,
counter aur pub/sub bhi karta hai.

## Asli kaam wala trap

```java
@Service
public class UserService {
    public String profile(Long id) { return this.getUser(id); }   // cache KABHI nahi chalega

    @Cacheable("users")
    public String getUser(Long id) { ... }
}
```

`@Cacheable` proxy se chalta hai. Same class ke andar `this.getUser()` bulaya to call proxy ko bypass kar deti hai, aur har baar DB jaati hai. Ye wahi `@Transactional`
wala self-invocation jaal hai. Isliye usercrud me cache wala method alag bean (`CacheDemoService`) me rakha hai, aur controller usi bean ko bulata hai.

Do baatein aur. `@EnableCaching` lagana bhool gaye to annotation chup-chaap kuch nahi karta. Aur manual demo wala `HashMap` singleton controller me rakha hai, to
saare thread use share karte hain. Wo thread-safe nahi hai, aur kabhi saaf bhi nahi hota.

## Tera project (usercrud)

`/cache-demo/anno/{id}` pe doosri call me "METHOD CHAL RAHA" print hi nahi hua. Matlab cache ne method chalne se pehle hi jawab de diya.
