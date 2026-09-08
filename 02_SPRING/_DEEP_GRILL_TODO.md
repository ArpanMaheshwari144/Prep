# SPRING — DEEP-GRILL TODO (Java folder ke baad — same treatment)

> 8-Sep audit (poora 02_SPRING padha): @Transactional + saara Hibernate + JWT-token half + Kafka/Async/Cache/QueryDSL/GraphQL = DEEP, grill-ready.
> Asli holes = CONTAINER-INTERNALS (bean lifecycle, proxy, auto-config, security architecture). JP round-2 = Spring-internals GRILL. Ye bharne hain.

## ★ METHOD (Java jaisa — Arpan-LOCKED)
```
1. Claude PEHLE TEACH kare — concept + analogy (store/box/visual), samjhaye.
2. Arpan SAMJHE -> "samjha?" gate.
3. PHIR file me likhe (samajh pehle, likhna baad me). silent "update kar di" NAHI.
4. NAYA note banane ki jagah -> pehle dekho EXISTING file me daala ja sakta -> usme merge. Naya sirf jab koi ghar na ho.
```

---

## ★ TIER-1 — near-certain JP round-2 probes, ABHI MISSING (order me)

```
1. [DONE 8-Sep] BEAN LIFECYCLE (poora) + CIRCULAR DEPENDENCY   [01_basics EXTEND]
   -> bean lifecycle onboarding-analogy 8-step + 3 crux + scope-lifespan -> 02_beans_ioc_container.md
   -> why-DI deep (shared-singleton + PROXY) + kaha/kaise-banta -> 01_spring_kya_hai.md
   -> circular-dep A/B chicken-egg + constructor-fail + setter-diary + 3-level cache + @Lazy/refactor -> 03_annotations_di_types.md
      (singletonObjects / earlySingletonObjects / singletonFactories),
      setter/field/@Lazy/ObjectProvider se cycle kaise tootta

2. [DONE 8-Sep] PROXY: JDK-dynamic vs CGLIB   [naya note 01_basics/04_proxy_jdk_cglib.md]
   -> secretary-model + direct-kyun-nahi (repetition) + koi-jaadu-nahi + DRY/SoC
   -> JDK(interface)/CGLIB(subclass) + Boot-default CGLIB + final-limitation + self-invocation trap

3. [DONE 8-Sep] SPRING BOOT AUTO-CONFIGURATION   [naya note 01_basics/05_autoconfiguration.md]
   -> naukar-almari analogy + TERE usercrud pom se mapping (har starter -> Spring ne kya banaya)
   -> @Conditional (OnClass/OnMissingBean/OnProperty) + spring.factories->AutoConfiguration.imports + starter=bundle

4. SPRING SECURITY asli ARCHITECTURE   [03_jwt_security EXTEND]
   -> DelegatingFilterProxy -> FilterChainProxy -> SecurityFilterChain (default filters)
   -> AuthenticationManager -> AuthenticationProvider -> UserDetailsService -> PasswordEncoder
   -> SecurityContextHolder (ThreadLocal + @Async propagation), ExceptionTranslationFilter/EntryPoint
   -> method security @PreAuthorize/@EnableMethodSecurity
   (JWT-token deep hai, filter-chain abhi "cartoon" level)

5. @Configuration CGLIB / proxyBeanMethods   [#2 ke saath ya 01_basics]
   -> inter-bean method call singleton kyun return karta (Lite vs Full config). classic filter-Q.
```

## ★ TIER-2 — likely follow-ups

```
6. Hibernate ENTITY STATES (formal)   [05_HIBERNATE/04 EXTEND]
   -> transient / persistent / detached / removed state-machine + persist/merge/remove/detach/flush
   -> save vs persist vs merge vs saveOrUpdate
7. OSIV (open-session-in-view)   [05_HIBERNATE/03 EXTEND]  -> Boot me default ON, lazy ka #1 follow-up, pros/cons
8. Spring MVC request lifecycle   [confirm 08_REVISION_VISUAL/02 ya naya]
   -> DispatcherServlet -> HandlerMapping -> HandlerAdapter -> HttpMessageConverters -> ViewResolver
   -> @ControllerAdvice/@ExceptionHandler global error, @Valid/Bean-Validation
9. Transaction-manager types + NESTED savepoint caveat + programmatic tx   [02_transactional EXTEND]
   -> Jpa vs DataSource vs Jta; NESTED JpaTransactionManager pe kaam nahi karta (savepoint-capable chahiye);
      TransactionTemplate
10. @GeneratedValue strategies   [05_HIBERNATE/07 EXTEND]  -> IDENTITY batch-insert disable karta (perf)
11. Spring AOP internals   [naya ya #2 ke saath]  -> pointcut/advice/advisor, @Aspect, auto-proxy weaving
12. ApplicationContext vs BeanFactory + context refresh() + ApplicationEvent/listeners
```

## ★ TIER-3 — nice-to-have
```
- L2 cache concurrency strategies + query cache · Spring Data pagination/Sort/Specifications ·
  HikariCP pool sizing · GraphQL DataLoader/N+1 · Actuator/observability (agar JP-scope me)
- Spring Data JPA: derived query methods, Pageable/Sort, @Query countQuery, Specifications
- Hibernate: MultipleBagFetchException (2 collection JOIN FETCH) + JOIN-FETCH+pagination in-memory trap
```

## ★ FACTUAL FIX (quick win)
```
1. 03_jwt_security (code) -> deprecated jjwt 0.9.x API (signWith(SignatureAlgorithm,secret),
   Jwts.parser().setSigningKey). Naya 0.11+/0.12: Key/SecretKey object, parserBuilder()/verifyWith(),
   HS256 ke liye >=256-bit key. -> update.
2. 02_transactional (isolation matrix) -> MySQL InnoDB REPEATABLE_READ phantom="allow" standard-sahi
   PAR InnoDB gap/next-key lock zyadatar phantom rok deta -> caveat add.
3. 02_transactional (NESTED) -> universal dikhaya; JpaTransactionManager pe kaam nahi karta -> caveat.
4. [DONE 8-Sep] 01_basics/03 -> "field injection discouraged kyunki reflection" IMPRECISE theek kar diya
   (asli reason = immutability/testability/null-safety/hidden-deps/circular-dep-masking).
```

## ★ PEHLE SE DEEP (inpe kaam nahi — chinta mat)
```
@Transactional (02) · saara 05_HIBERNATE (N+1/lazy-eager/dirty-check/cascade/relationships/opt-lock) ·
JWT-token half (03) · 14_KAFKA · 12_ASYNC · 13_CACHE · 09_QUERYDSL · 11_GRAPHQL · 10_SPRING_DATA · 04_profiles · docker/k8s
```

---
> SHURU: Claude #1 (bean lifecycle + circular-dep) se TEACH-first. ek-ek topic, samajh -> phir file. ~2-3/din.
