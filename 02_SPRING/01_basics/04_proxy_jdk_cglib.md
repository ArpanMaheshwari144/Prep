# Spring Proxy — JDK Dynamic vs CGLIB (AOP ka engine)

> **Spring 101: Topic 4** — @Transactional / @Async / @Cacheable SAB isi pe tikte. [[02_beans_ioc_container]] (BeanPostProcessor yahi proxy banata) · [[03_annotations_di_types]] (circular-dep 3rd-level-cache proxy ke liye)

---

## Proxy kya = ek SECRETARY jo asli bean ke aage baithi

Asli bean (bada aadmi) ke saamne ek **secretary (proxy)** baithi. Tu call karta -> pehle SECRETARY milti -> wo extra kaam karti -> andar asli method bhejti -> bahar cleanup:

```
Tu -> [SECRETARY / proxy] -> [ASLI bean ka method] -> wapas SECRETARY -> Tu

@Transactional pe secretary:
   1. DB transaction OPEN (BEGIN)
   2. asli method chalao
   3. exception nahi -> COMMIT   |   aaya -> ROLLBACK
```

**Isiliye @Transactional kaam karta** — tu asli object nahi, PROXY ko call karta. `new` se banaye object me koi secretary nahi -> `new` pe @Transactional bekaar.

---

## "Direct call kyun nahi, itna drama kyun?" (asli samajh)

Direct kar SAKTA hai — Spring se pehle log yahi karte the. Proxy annotation-khush-karne ko NAHI, **repetition hatane** ko hai.

**Direct karega to HAR method me ye likhna padega:**
```java
public void transfer() {
    conn.setAutoCommit(false);          // BEGIN
    try {
        // ... asli kaam (2-3 line) ...
        conn.commit();                  // COMMIT
    } catch (Exception e) {
        conn.rollback();                // ROLLBACK
        throw e;
    } finally { conn.close(); }
}
```
Asli kaam = 2 line · plumbing = 8 line. **100 method = 8 line × 100 copy.** + logging/security/retry bhi har jagah -> business logic doob jaata · policy badli -> 100 jagah edit. (wahi `new` wali REPETITION problem.)

**Proxy/AOP ke saath:**
```java
@Transactional
public void transfer() {
    // ... sirf asli kaam ...    <- plumbing GAYAB (proxy ek jagah likhi, sabpe lagti)
}
```

★ **Ulta hai: proxy hi SIMPLE tarika.** "Direct" asli me zyada kaam (har method me haath se plumbing). Proxy = taaki wahi 8 line 100 baar na likho.

---

## "Andar koi jaadu nahi" (Arpan-insight, sahi)

```
Andar: Spring wahi plumbing proxy me GENERATE karke chalata. Runtime pe jo hota = BILKUL wahi
       jo tu haath se likhta. Result IDENTICAL.
Farak: tu 2 line likhta (business + annotation), plumbing Spring likh/chala deta.
```
(proxy banate waqt Spring `new` nahi -> runtime BYTECODE generate karta; CGLIB teri class ka subclass. Par core sahi: koi jaadu nahi, wahi code tere liye.)

**"Kam code" = SYMPTOM. Asli faayda 3:**
```
1. Plumbing EK jagah + SAHI -> 100 method consistent -> rollback/close bhoolne wala BUG nahi
2. Policy badli -> EK jagah, 100 nahi
3. Business code SAAF (logic dikhta, plumbing nahi)
```
Interview 2 lafz: **DRY** (Don't Repeat Yourself) · **Separation of Concerns** (business alag, cross-cutting tx/log/security alag).

---

## Secretary 2 tarah ki — JDK Dynamic vs CGLIB

```
JDK DYNAMIC PROXY -> bean ek INTERFACE implement karta ho.
   Spring wahi interface implement karke proxy banata = secretary "wahi BADGE (interface)" pehen ke.
   (java.lang.reflect.Proxy)

CGLIB -> koi interface NAHI (sirf class).
   Spring runtime pe class ka SUBCLASS banata (extends + override) = secretary "nakli BETA".
```

**Spring kab kaunsa:**
```
interface hai    -> traditionally JDK dynamic
interface nahi   -> CGLIB
Spring Boot 2.x+ -> by DEFAULT CGLIB sabpe (proxyTargetClass=true), surprise se bachne ko
```

---

## 2 LIMITATION (interviewer yahi kholega)

**1. `final` pe proxy nahi banta (CGLIB).**
CGLIB subclass banake override karta -> `final` class extend nahi, `final`/`private`/`static` method override nahi -> un pe proxy/@Transactional lagega hi nahi.

**2. SELF-INVOCATION trap (famous — @Transactional/@Async/@Cacheable sab me):**
Secretary DARWAZE pe baithi. Bahar se call -> secretary se ho ke (proxy laga). Par andar baitha aadmi KHUD se baat kare (`this.method()`) -> darwaze wali secretary ko pata hi nahi -> koi extra kaam nahi.
```java
class OrderService {
    public void a() {
        b();                 // = this.b() -> proxy BYPASS -> b ka @Transactional NAHI chalega!
    }
    @Transactional
    public void b() { ... }
}
```
`a()` andar se `b()` seedha call kar raha -> secretary bypass -> `b()` ka transaction lagega hi nahi.
**Fix:** doosre bean se call, ya self-inject/`AopContext.currentProxy()`, ya method alag bean me nikaalo.

---

## ★ PROJECT CONNECT — usercrud @Transactional/@Cacheable/@Async (8-Sep)

> Proxy ki theory tere project me 3 jagah LIVE:
```
@Transactional -> UserService: create/update/delete, getById/getAll (readOnly=true), createWithSimulatedFailure
@Cacheable      -> CacheDemoService.getUser("users")
@Async          -> AsyncDemoController (background task)
```
```
1. UserService pe @Transactional -> Spring ne use PROXY me wrap kiya.
2. UserController -> userService.create() call = asli nahi, PROXY ko -> proxy BEGIN -> create() -> COMMIT.
3. demoRollback: createWithSimulatedFailure() ne RuntimeException -> PROXY ne pakda -> ROLLBACK
   -> DB me user nahi (ye proxy ka LIVE proof, khud test kiya).
4. @Cacheable getUser -> proxy pehle cache dekhta, hit -> method chalata hi nahi.
```
- Proxy hi @Transactional chalata = UserController -> proxy -> UserService (seedha nahi)
- readOnly optimization = getById/getAll pe proxy ne Hibernate ko "no dirty-check" bola

**Self-invocation honest note:** usercrud me create/update alag CONTROLLER se call (bahar se) -> proxy laga -> theek. Agar UserService.create() ANDAR se this.update() karta -> proxy bypass -> update ka tx na lagta. (project me aisa nahi, par trap yaad.)

---

## POWER PHRASE

> *"Spring implements @Transactional/@Async/@Cacheable through proxies — a wrapper around the bean that adds the cross-cutting logic (begin/commit/rollback) around the real method call. If the bean has an interface it uses a JDK dynamic proxy; otherwise CGLIB, which subclasses the class at runtime (Spring Boot defaults to CGLIB). Two gotchas: it can't proxy final classes/methods, and self-invocation (this.method()) bypasses the proxy so the annotation is ignored."*

> **Yaad rakh:**
> Proxy = secretary; extra kaam karke andar forward.
> JDK dynamic = interface (same badge) · CGLIB = class (nakli subclass) · Boot default = CGLIB.
> final = proxy nahi · this.method() = bypass (self-invocation trap).
> Faayda = DRY + Separation of Concerns (kam code = symptom; asli = ek-jagah + consistent + kam-bug).

---

## ★★ @Configuration ka CGLIB jaadu — inter-bean call SINGLETON deta (11-Sep, classic filter-Q)

> Proxy sirf @Transactional/@Async ke liye nahi — `@Configuration` class bhi CGLIB-proxied hoti. Interview me bahut poochte.

```java
@Configuration
class AppConfig {
    @Bean ServiceA serviceA() { return new ServiceA(dependency()); }
    @Bean ServiceB serviceB() { return new ServiceB(dependency()); }   // dependency() DOBARA call
    @Bean Dependency dependency() { return new Dependency(); }
}
```
**SAWAAL:** `dependency()` do method call kar rahe -> 2 alag Dependency object banenge (plain Java jaisa)?
**JAWAB: NAHI -> ek hi SINGLETON.** Yahi CGLIB ka jaadu.

**KAISE:**
```
@Configuration class ko Spring seedha nahi -> uska CGLIB PROXY (subclass) banata.
Proxy har @Bean method INTERCEPT karta:
   dependency() call -> proxy check "container me PEHLE se hai?"
      HAAN -> wahi cached SINGLETON return (dobara `new` nahi)
      NAHI -> asli method chala ke banao + container me daalo
-> serviceA aur serviceB dono ko SAME dependency.
```
Anchor: config-proxy = secretary jo har bean-request pe pehle almari (container) dekhti -> mila to wahi, warna banati. Plain Java me secretary nahi -> har call = naya `new`.

**LITE vs FULL config (yahi discriminator):**
```
FULL (@Configuration)              -> CGLIB proxy -> inter-bean call SINGLETON (upar wala)
LITE (@Component pe @Bean, YA @Configuration(proxyBeanMethods=false)) -> NO proxy
   -> inter-bean call = plain Java call -> HAR baar NAYA object (singleton guarantee GAYI -> bug-risk)
```
```
@Configuration(proxyBeanMethods = false) -> proxy skip -> thoda FAST startup,
   PAR inter-bean method call ho raha (dependency() 2 jagah) -> ab 2 alag object -> bug.
   -> proxyBeanMethods=false SIRF jab @Bean methods ek-doosre ko call NA karein.
```

**POWER-PHRASE:**
> "@Configuration classes are CGLIB-proxied so inter-bean method calls return the SAME singleton, not a new
>  instance. proxyBeanMethods=false (lite mode) skips the proxy — faster startup but inter-bean calls create
>  new objects; use it only when @Bean methods don't call each other."

---

## ★★ HANDS-ON DEMO — Spring internals LIVE (usercrud, 11-Sep)

> [`SpringInternalsDemo.java`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/demo/SpringInternalsDemo.java) — ek endpoint `/internals` jo Spring ke 4 andar-ke-raaz aankhon ke saamne dikha deta. Theory padhi se aage — actual print dekha.

### 4 RAAZ, 4 experiment, 4 print
```
RAAZ 1  PROXY  ->  userService.getClass().getName()
   print: UserService$$SpringCGLIB$$0
   matlab: bean asli nahi, uska CGLIB secretary (proxy). Yahi @Transactional chalati.

RAAZ 2  SINGLETON  ->  getBean(Dependency.class) 2 baar, obj1==obj2 ?
   print: true   (SomeService bhi true)
   matlab: container me EK hi copy, har maang pe wahi.
   + SomeService ko DemoConfig ne someService(){ new SomeService(dependency()) } se banaya ->
     dependency() do jagah call par ek hi Dependency = upar wala @Configuration CGLIB jaadu LIVE.

RAAZ 3  PROTOTYPE  ->  getBean(DummyBeam.class) 2 baar, p1==p2 ?
   print: false
   matlab: @Scope("prototype") -> har getBean pe NAYA object.

RAAZ 4  BeanPostProcessor  ->  LoggingBeanPostProcessor har bean ke init pe naam print karti
   print: "DummyBeam" 4 baar
   matlab: prototype 2 baar maanga -> 2 baar naya bana -> har banne pe BPP before+after = 4.
```

### ★ Chhupa gold (khud mil gaya, plan nahi tha)
```
Dependency/SomeService pe getBean kiya -> koi BPP print NAHI aaya.
   Kyunki SINGLETON startup pe ek baar bane -> BPP tabhi bol chuki ->
   ab getBean sirf CACHED laata -> naya nahi banta -> BPP CHUP.
DummyBeam PROTOTYPE -> ab-ab naya bana -> BPP ab boli -> 4 print.

=> output ne KHUD singleton-vs-prototype ka farak prove kar diya:
   singleton getBean = chup (cached) · prototype getBean = shor (naya banta).
```
(Aur `523` = Spring ne total kitne bean auto-banaye — tune 5-6 likhe, baaki auto-config ne.)

### Project connect
```
UserService        -> @Service -> proxy chadha (RAAZ 1 ka $$SpringCGLIB$$)
DemoConfig         -> @Configuration -> inter-bean singleton (RAAZ 2)
DummyBeam          -> @Scope("prototype") (RAAZ 3)
LoggingBeanPostProcessor -> BeanPostProcessor hook (RAAZ 4)
LifecycleBean      -> constructor -> @Autowired setter -> @PostConstruct -> afterPropertiesSet
                      (startup-log me 1->2->3->4 order dikhta = bean lifecycle LIVE)
```

> **Ek line (delivery):** "Spring har @Service ko proxy me wrap karta, singleton cache karta, prototype har baar naya deta, aur BeanPostProcessor se har bean ke init pe hook lagata — maine apne project me `/internals` endpoint se ye chaaron live print karke dekhe."
