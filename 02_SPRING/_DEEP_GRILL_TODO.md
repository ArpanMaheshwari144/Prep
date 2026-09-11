# SPRING — DEEP-GRILL TODO (JP round-2 = Spring-internals GRILL)

> @Transactional + saara Hibernate + JWT + Kafka/Async/Cache/QueryDSL/GraphQL + container-internals
> (bean-lifecycle/proxy/auto-config/security-arch/entity-states/OSIV) = DONE, grill-ready.
> Ye file = ab tak jo BACHA hai (pending only). DONE items hata diye — notes-files me pakke (git history bhi).

## ★ METHOD (Arpan-LOCKED)
```
1. Claude PEHLE TEACH — concept + analogy (store/box/visual), samjhaye.
2. Arpan SAMJHE -> "samjha?" gate.
3. PHIR file me likhe (samajh pehle, likhna baad me). silent "update kar di" NAHI.
4. NAYA note ki jagah -> pehle EXISTING file me merge dekho.
```

---

## ★ BACHA HUA — PENDING (order me)

```
1. Spring MVC request lifecycle   [confirm 08_REVISION_VISUAL/02 ya naya]
   -> DispatcherServlet -> HandlerMapping -> HandlerAdapter -> HttpMessageConverters -> ViewResolver
   -> @ControllerAdvice/@ExceptionHandler global error, @Valid/Bean-Validation

2. Transaction-manager types + NESTED savepoint caveat + programmatic tx   [02_transactional EXTEND]
   -> Jpa vs DataSource vs Jta; NESTED JpaTransactionManager pe kaam nahi karta (savepoint-capable chahiye);
      TransactionTemplate

3. @GeneratedValue strategies   [05_HIBERNATE/07 EXTEND]  -> IDENTITY batch-insert disable karta (perf)

4. Spring AOP internals   [naya ya proxy-note ke saath]  -> pointcut/advice/advisor, @Aspect, auto-proxy weaving

5. ApplicationContext vs BeanFactory + context refresh() + ApplicationEvent/listeners
```
> (@Configuration CGLIB 11-Sep DONE — 04_proxy_jdk_cglib.md me, + SpringInternalsDemo live `/internals` se proxy/singleton/prototype/BPP dekhe.)

## ★ TIER-3 — nice-to-have
```
- L2 cache concurrency strategies + query cache · Spring Data pagination/Sort/Specifications ·
  HikariCP pool sizing · GraphQL DataLoader/N+1 · Actuator/observability (agar JP-scope me)
- Spring Data JPA: derived query methods, Pageable/Sort, @Query countQuery, Specifications
- Hibernate: MultipleBagFetchException (2 collection JOIN FETCH) + JOIN-FETCH+pagination in-memory trap
```

## ★ FACTUAL FIX (quick win — bache)
```
1. 02_transactional (isolation matrix) -> MySQL InnoDB REPEATABLE_READ phantom="allow" standard-sahi
   PAR InnoDB gap/next-key lock zyadatar phantom rok deta -> caveat add.
2. 02_transactional (NESTED) -> universal dikhaya; JpaTransactionManager pe kaam nahi karta -> caveat.
```

---
> SHURU: #1 (MVC-lifecycle) se TEACH-first. ek-ek topic.
