# Prep — Backend Engineering Interview Workspace

Java / Spring backend interview prep — DSA, system design, projects, hands-on notes.
**Har heading/link click → seedha wahi note ya folder khul jaata.** Har section ke saath **Kya hai** + **Kyu matter karta** likha hai — kholne se pehle hi pata chal jaaye.

> ★ **Abhi (DELIVERY phase — build ~done):** roz HLD-mock **BOLKE** + STAR **BOLKE** + DSA sheet-glance/redo + Java/Spring revise + shaam apply. Ab "jaanna" se "bolke fluently nikaalna" pe focus.
> Live state + plan → **[PROGRESS.md](PROGRESS.md)**

---

## Ek nazar — poora map

| # | Section | Kya hai | Interview me kyu |
|---|---------|---------|------------------|
| 01 | [**01_JAVA**](01_JAVA) | Core · Collections · Java-8 · Multithreading · SOLID · Patterns · Testing | Java grill round ka **core** — collections-internal + multithreading sabse zyada pucha jaata |
| 02 | [**02_SPRING**](02_SPRING) | Boot internals · @Transactional · Hibernate · JWT · Docker/K8s · Kafka/Async/Cache · QueryDSL/GraphQL | Backend role ka **dil** — proxy / N+1 / tx / Kafka = deep-grill favourites |
| 03 | [**03_LLD**](03_LLD) | 11 OOP designs, code me | LLD round — class-design + patterns **bolke** |
| 04 | [**04_HLD**](04_HLD) | Foundations + 12 designs + delivery-playbook | Tera **strong zone** — JP ka pura ek round |
| 05 | [**05_INFRA_DEEP**](05_INFRA_DEEP) | CDN · LB · DNS · WAF · monitoring + devops + hands-on demos | HLD me "deploy/scale kaise" ki depth + STAR-fodder |
| 06 | [**06_COMPARES**](06_COMPARES) | 14 side-by-side "X vs Y" | Rapid-fire — turant clear answer chahiye |
| 07 | [**07_PROJECTS**](07_PROJECTS) | 5 Spring Boot projects | Resume-backing + "kya banaya" ka **live proof** |
| 08 | [**08_DSA**](08_DSA) | 18 pattern · ~200 problem + sheet | **Gate** — pass karna hai (ab revise-only) |
| 09 | [**09_DATABASE**](09_DATABASE) | Indexing · txn · isolation · deadlock (hands-on) | Backend + HLD ka data-layer depth |
| 10 | [**10_BEHAVIORAL**](10_BEHAVIORAL) | STAR stories | JP ka pura **behavioral round** |
| 11 | [**11_GIT**](11_GIT) | line-endings · PR · merge-vs-rebase | PR-review + daily workflow Qs |
| 12 | [**12_CODE_REVIEW**](12_CODE_REVIEW) | 5-bucket checklist + drills | JP **round-2 code-review** |

> ★ **Roz-revise pinned:** **[DSA Pattern Sheet](08_DSA/00_PATTERN_SHEET/PATTERN_SHEET.md)** (gate ki chaabi) · **[HLD Approach Playbook](04_HLD/HLD_APPROACH_DELIVERY.md)** (kaise bolna)

---

## [01_JAVA](01_JAVA)
**Kya:** poori Java language + concurrency + design. &nbsp; **Kyu:** Java grill round ka core; multithreading + collections-internal = sabse zyada poochte.

- **[01_CORE_JAVA](01_JAVA/01_CORE_JAVA)** — language ki neev — [OOP](01_JAVA/01_CORE_JAVA/01_OOP) (kya-hai · 4-pillars · abstract-vs-interface · overloading-vs-overriding · immutable · constructor-chaining · type-casting · instanceof) · [STRING](01_JAVA/01_CORE_JAVA/02_STRING) (immutable · builder/buffer) · [KEYWORDS](01_JAVA/01_CORE_JAVA/03_KEYWORDS) (static · final · this-vs-super) · [CORE_EXTRAS](01_JAVA/01_CORE_JAVA/04_CORE_EXTRAS) (jdk/jre/jvm · access-modifiers · wrapper/autobox · GC · enum · pass-by-value) · [EXCEPTIONS](01_JAVA/01_CORE_JAVA/05_EXCEPTIONS) (checked-vs-unchecked · try/catch/finally)

- **[02_COLLECTIONS](01_JAVA/02_COLLECTIONS)** — *internal working* (interview-favourite: "HashMap andar kaise?") — [01 hashmap-internal](01_JAVA/02_COLLECTIONS/01_hashmap_internal.md) · [02 hashmap-vs-CHM](01_JAVA/02_COLLECTIONS/02_hashmap_vs_concurrenthashmap.md) · [03 arraylist](01_JAVA/02_COLLECTIONS/03_arraylist_internal.md) · [04 linkedlist](01_JAVA/02_COLLECTIONS/04_linkedlist_internal.md) · [05 iterator-vs-listiterator](01_JAVA/02_COLLECTIONS/05_iterator_vs_listiterator.md) · [06 hashmap/linkedhashmap/treemap](01_JAVA/02_COLLECTIONS/06_hashmap_linkedhashmap_treemap.md) · [07 hashset](01_JAVA/02_COLLECTIONS/07_hashset_internal.md) · [08 comparable-vs-comparator](01_JAVA/02_COLLECTIONS/08_comparable_vs_comparator.md) · [09 thenComparing](01_JAVA/02_COLLECTIONS/09_thencomparing_chained_sorting.md) · [10 fail-fast-vs-safe](01_JAVA/02_COLLECTIONS/10_fail_fast_vs_fail_safe.md) · [11 priorityqueue](01_JAVA/02_COLLECTIONS/11_priorityqueue_min_heap.md)

- **[03_JAVA_8](01_JAVA/03_JAVA_8)** — functional style (har modern codebase me) — [01 lambda](01_JAVA/03_JAVA_8/01_lambda_expressions.md) · [02 functional-interface](01_JAVA/03_JAVA_8/02_functional_interface_pfcs.md) · [03 method-reference](01_JAVA/03_JAVA_8/03_method_reference.md) · [04 streams](01_JAVA/03_JAVA_8/04_streams_api.md) · [05 optional](01_JAVA/03_JAVA_8/05_optional.md) · [06 stream-cheatsheet](01_JAVA/03_JAVA_8/06_stream_methods_cheatsheet.md)

- **[04_MULTITHREADING](01_JAVA/04_MULTITHREADING)** — *biggest grill-risk* (theory + runnable demos) — [01 basics](01_JAVA/04_MULTITHREADING/01_thread_basics.md) · [02 lifecycle](01_JAVA/04_MULTITHREADING/02_thread_lifecycle.md) · [03 join](01_JAVA/04_MULTITHREADING/03_join.md) · [04 synchronized](01_JAVA/04_MULTITHREADING/04_synchronized.md) · [05 volatile](01_JAVA/04_MULTITHREADING/05_volatile.md) · [06 atomic/CAS](01_JAVA/04_MULTITHREADING/06_atomic_integer_cas.md) · [07 deadlock](01_JAVA/04_MULTITHREADING/07_deadlock.md) · [08 wait-vs-sleep](01_JAVA/04_MULTITHREADING/08_wait_vs_sleep.md) · [09 notify](01_JAVA/04_MULTITHREADING/09_notify_deep_dive.md) · [10 executor/pool](01_JAVA/04_MULTITHREADING/10_executor_service_thread_pool.md) · [11 future-vs-completablefuture](01_JAVA/04_MULTITHREADING/11_future_vs_completablefuture.md) · [12 producer-consumer](01_JAVA/04_MULTITHREADING/12_producer_consumer.md) · [13 threadlocal](01_JAVA/04_MULTITHREADING/13_threadlocal.md)
  - **[practical/](01_JAVA/04_MULTITHREADING/practical)** — chala-ke-dekhe wale demos — [ThreadLocalDemo](01_JAVA/04_MULTITHREADING/practical/ThreadLocalDemo.java) (isolation + pool-leak + fix) · [SafePubDemo](01_JAVA/04_MULTITHREADING/practical/SafePubDemo.java) · [DeadlockDemo](01_JAVA/04_MULTITHREADING/practical/DeadlockDemo.java) · [RaceDemo](01_JAVA/04_MULTITHREADING/practical/RaceDemo.java) · [ProducerConsumer](01_JAVA/04_MULTITHREADING/practical/ProducerConsumer.java)

- **[05_SOLID](01_JAVA/05_SOLID)** — design principles (LLD + code-review me) — [01 kya/kyun](01_JAVA/05_SOLID/01_solid_kya_hai_kyu.md) · [02 S](01_JAVA/05_SOLID/02_S_single_responsibility.md) · [03 O](01_JAVA/05_SOLID/03_O_open_closed.md) · [04 L](01_JAVA/05_SOLID/04_L_liskov_substitution.md) · [05 I](01_JAVA/05_SOLID/05_I_interface_segregation.md) · [06 D](01_JAVA/05_SOLID/06_D_dependency_inversion.md)

- **[06_DESIGN_PATTERNS](01_JAVA/06_DESIGN_PATTERNS)** — GoF patterns (LLD ki bhasha) — [01 builder](01_JAVA/06_DESIGN_PATTERNS/01_builder.md) · [02 singleton](01_JAVA/06_DESIGN_PATTERNS/02_singleton.md) · [03 factory](01_JAVA/06_DESIGN_PATTERNS/03_factory.md) · [04 observer](01_JAVA/06_DESIGN_PATTERNS/04_observer.md) · [05 adapter](01_JAVA/06_DESIGN_PATTERNS/05_adapter.md) · [06 strategy](01_JAVA/06_DESIGN_PATTERNS/06_strategy.md)

- **[07_BRAIN_TICKLERS](01_JAVA/07_BRAIN_TICKLERS)** — 13 gotcha traps (string-intern · equals-trap · immutable · diamond · final-vs-finally-vs-finalize · covariant-return ...) — quick-fire trick Qs.

- **[08_REVISION_VISUAL](01_JAVA/08_REVISION_VISUAL)** — sab kuch ek-nazar visual revise-set — [CORE](01_JAVA/08_REVISION_VISUAL/01_CORE) · [COLLECTIONS](01_JAVA/08_REVISION_VISUAL/02_COLLECTIONS) · [JAVA_8](01_JAVA/08_REVISION_VISUAL/03_JAVA_8) · [MULTITHREADING](01_JAVA/08_REVISION_VISUAL/04_MULTITHREADING) · [OOP_DESIGN (SOLID+patterns)](01_JAVA/08_REVISION_VISUAL/05_OOP_DESIGN) · [QUICK_QA](01_JAVA/08_REVISION_VISUAL/06_QUICK_QA)

- **[09_WRITE_PRACTICE](01_JAVA/09_WRITE_PRACTICE)** — streams write-to-learn (filter/map/groupingBy/reduce + REDO files) — likhke pakka karna.

- **[10_TESTING](01_JAVA/10_TESTING)** — [unit testing (JUnit + Mockito)](01_JAVA/10_TESTING/01_unit_testing_mockito.md) — mock/when/assert/verify + tautology-vs-logic + usercrud anchor.

- **[PROJECT — SimpleBankSystem](01_JAVA/PROJECT/SimpleBankSystem)** — plain-Java bank (factory + observer, layered model/repository/service) — patterns LIVE.

---

## [02_SPRING](02_SPRING)
**Kya:** Spring Boot internals + persistence + messaging + deployment. &nbsp; **Kyu:** backend role ka dil; proxy / @Transactional / Hibernate-N+1 / Kafka = deep-grill ke pasandeeda sawaal.

- **[01_basics](02_SPRING/01_basics)** — Spring ka andar-ka-jaadu (IoC/proxy) — [spring-kya-hai](02_SPRING/01_basics/01_spring_kya_hai.md) · [beans/IoC](02_SPRING/01_basics/02_beans_ioc_container.md) · [annotations/DI](02_SPRING/01_basics/03_annotations_di_types.md) · [proxy JDK-vs-CGLIB](02_SPRING/01_basics/04_proxy_jdk_cglib.md) · [auto-configuration](02_SPRING/01_basics/05_autoconfiguration.md)
  <br>&nbsp;&nbsp;↳ *live chal-ke-dekha:* proxy/singleton/prototype/BPP — demo `usercrud` project me (neeche [usercrud sandbox map](#usercrud--live-demo-sandbox))

- **[02_transactional](02_SPRING/02_transactional/01_transactional_complete.md)** — @Transactional complete (proxy pe tika, rollback/propagation/isolation).

- **[03_jwt_security](02_SPRING/03_jwt_security/01_jwt_complete.md)** — JWT + Spring Security filter-chain complete.

- **[04_profiles](02_SPRING/04_profiles)** — env-wise config — [basics](02_SPRING/04_profiles/01_basics.md) · [compose](02_SPRING/04_profiles/02_compose_integration.md) · [reference](02_SPRING/04_profiles/03_reference.md)

- **[05_HIBERNATE](02_SPRING/05_HIBERNATE)** — ORM ki asli samajh (N+1/lazy = interview gold) — [jpa-vs-hibernate](02_SPRING/05_HIBERNATE/01_jpa_vs_hibernate.md) · [N+1](02_SPRING/05_HIBERNATE/02_n_plus_1_problem.md) · [lazy-vs-eager](02_SPRING/05_HIBERNATE/03_lazy_vs_eager.md) · [dirty-checking](02_SPRING/05_HIBERNATE/04_dirty_checking.md) · [1st-level-cache](02_SPRING/05_HIBERNATE/05_first_level_cache.md) · [cascade](02_SPRING/05_HIBERNATE/06_cascade_types.md) · [relationships](02_SPRING/05_HIBERNATE/07_relationship_mappings.md) · [optimistic-lock](02_SPRING/05_HIBERNATE/08_optimistic_locking.md)

- **[06_DOCKER](02_SPRING/06_DOCKER)** — containerize + compose — [foundation](02_SPRING/06_DOCKER/01_foundation.md) · [spring+mysql](02_SPRING/06_DOCKER/02_spring_mysql_host.md) · [volumes](02_SPRING/06_DOCKER/03_volumes.md) · [transition](02_SPRING/06_DOCKER/04_transition.md) · [networks](02_SPRING/06_DOCKER/05_networks.md) · [compose](02_SPRING/06_DOCKER/06_docker_compose.md) · [reference](02_SPRING/06_DOCKER/07_reference.md) · [revision (visual)](02_SPRING/06_DOCKER/08_revision_visual.md) · [multistage](02_SPRING/06_DOCKER/09_multistage_builds.md) · [arg-vs-env](02_SPRING/06_DOCKER/10_arg_vs_env.md)

- **[07_K8S](02_SPRING/07_K8S)** — orchestration basics — [why/arch](02_SPRING/07_K8S/01_why_k8s_and_architecture.md) · [pod](02_SPRING/07_K8S/02_pod.md) · [deployment](02_SPRING/07_K8S/03_deployment.md) · [service](02_SPRING/07_K8S/04_service.md) · [ingress](02_SPRING/07_K8S/05_ingress.md) · [configmap/secret](02_SPRING/07_K8S/06_configmap_secret.md)

- **[08_REVISION_VISUAL](02_SPRING/08_REVISION_VISUAL)** — basics · rest-flow · transactional · jwt · profiles — ek-nazar revise.

- **[09_QUERYDSL](02_SPRING/09_QUERYDSL/01_querydsl.md)** — type-safe dynamic queries (Q-classes).
- **[10_SPRING_DATA_JPA](02_SPRING/10_SPRING_DATA_JPA/01_custom_queries_dto.md)** — custom @Query · DTO projection.
- **[11_GRAPHQL](02_SPRING/11_GRAPHQL/01_graphql.md)** — schema · @QueryMapping · client-picks-fields.
- **[12_ASYNC_SCHEDULING](02_SPRING/12_ASYNC_SCHEDULING/01_async_scheduled.md)** — @Async · @Scheduled (proxy pe tika).
- **[13_CACHE](02_SPRING/13_CACHE/01_cache.md)** — @Cacheable · @CacheEvict.
- **[14_KAFKA](02_SPRING/14_KAFKA/01_kafka_basics.md)** — producer/consumer · DLQ · idempotency (event-driven ka core).

---

## [03_LLD](03_LLD) — code me 11 design
**Kya:** classic OOP designs, poore working code me. &nbsp; **Kyu:** LLD round — class-nikaalna + patterns lagana **bolke** dikhana.

- [01 parking-lot](03_LLD/01_parking_lot) · [02 bookmyshow](03_LLD/02_bookmyshow) · [03 tictactoe](03_LLD/03_tictactoe) · [04 snake-ladder](03_LLD/04_snake_ladder) · [05 elevator](03_LLD/05_elevator) · [06 atm](03_LLD/06_atm_machine) · [07 bank-account](03_LLD/07_bank_account) · [08 shopping-cart](03_LLD/08_shopping_cart) · [09 library](03_LLD/09_library) · [10 vending-machine](03_LLD/10_vending_machine) · [11 hotel-booking](03_LLD/11_hotel_booking)

---

## [04_HLD](04_HLD) — tera strong zone
**Kya:** neev (foundations) + 12 poore design + "kaise bolna" playbook. &nbsp; **Kyu:** JP ka pura ek round; tera favourite — yahan sabse confident.

> ★★ **HLD RAIL** — har design isi kram pe bolo (roz dekh, dimaag me ghoome):
```
┌──────────────┐   ┌──────────┐   ┌─────┐   ┌────────────┐   ┌───────────┐   ┌────────────┐   ┌─────────────┐
│ Requirements │ → │ Estimate │ → │ API │ → │ Data model │ → │ HL boxes  │ → │ Deep-dive  │ → │ Bottleneck  │
└──────────────┘   └──────────┘   └─────┘   └────────────┘   └───────────┘   └────────────┘   └─────────────┘
```

- ★ **Delivery-tools (pehle ye):** [HLD_APPROACH_DELIVERY](04_HLD/HLD_APPROACH_DELIVERY.md) — "kaise bolna" playbook (rail · meta-moves · anjaan-design · darr-jawab) · [INTERVIEW_FRAMEWORK](04_HLD/INTERVIEW_FRAMEWORK.md) · [SPEAKING_VOCAB](04_HLD/SPEAKING_VOCAB.md) (word-freeze se bachne ko)

- **[FOUNDATIONS](04_HLD/FOUNDATIONS)** — building-blocks (har design inhi se bante) — [01 hld-kya-hai](04_HLD/FOUNDATIONS/01_hld_kya_hai.md) · [02 capacity-estimation](04_HLD/FOUNDATIONS/02_capacity_estimation.md) · [03 load-balancing](04_HLD/FOUNDATIONS/03_load_balancing.md) · [04 caching](04_HLD/FOUNDATIONS/04_caching.md) · [05 replication](04_HLD/FOUNDATIONS/05_database_replication.md) · [06 sharding](04_HLD/FOUNDATIONS/06_database_sharding.md) · [07 message-queues](04_HLD/FOUNDATIONS/07_message_queues.md) · [08 CAP](04_HLD/FOUNDATIONS/08_cap_theorem.md) · [09 db-what-when](04_HLD/FOUNDATIONS/09_databases_what_when.md) · [10 ms-communication](04_HLD/FOUNDATIONS/10_ms_communication.md) · [11 reliability/SPOF](04_HLD/FOUNDATIONS/11_reliability_spof_cloud.md) · [12 elasticsearch](04_HLD/FOUNDATIONS/12_elasticsearch_search.md) · [13 snowflake-id](04_HLD/FOUNDATIONS/13_distributed_id_snowflake.md)

- **[RAW_DRILL](04_HLD/RAW_DRILL)** — bolke-practice + reflex — [00 reflex-sheet](04_HLD/RAW_DRILL/00_REFLEX_SHEET.md) · [01 design-practice-log](04_HLD/RAW_DRILL/01_DESIGN_PRACTICE_LOG.md) · [02 why-how-QA](04_HLD/RAW_DRILL/02_WHY_HOW_QA.md) · [03 interview-anchor](04_HLD/RAW_DRILL/03_INTERVIEW_ANCHOR.md)

- **[SYSTEM_DESIGNS](04_HLD/SYSTEM_DESIGNS)** — 12 poore design (roz ek rotate) — [01 url-shortener](04_HLD/SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md) · [02 rate-limiter](04_HLD/SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md) · [03 twitter-feed](04_HLD/SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md) · [04 notification](04_HLD/SYSTEM_DESIGNS/04_notification_system/04_notification_system.md) · [05 url-browser-journey](04_HLD/SYSTEM_DESIGNS/05_url_browser_journey/05_url_browser_journey.md) · [06 stock-broker](04_HLD/SYSTEM_DESIGNS/06_stock_broker_trading/06_stock_broker_trading.md) · [07 payment](04_HLD/SYSTEM_DESIGNS/07_payment_system/07_payment_system.md) · [08 file-upload](04_HLD/SYSTEM_DESIGNS/08_file_upload_validate_system/08_file_upload_INTERVIEW.md) · [09 news-aggregator](04_HLD/SYSTEM_DESIGNS/09_news_aggregator/09_news_aggregator_INTERVIEW.md) · [10 bookmyshow](04_HLD/SYSTEM_DESIGNS/10_bookmyshow/10_bookmyshow_INTERVIEW.md) · [11 distributed-cache](04_HLD/SYSTEM_DESIGNS/11_distributed_cache/11_distributed_cache.md) · [12 google-docs-collab](04_HLD/SYSTEM_DESIGNS/12_google_docs_collab/12_google_docs_collab.md)

---

## [05_INFRA_DEEP](05_INFRA_DEEP)
**Kya:** network/infra deep-dives + devops commands + chal-ke-dekhe demos. &nbsp; **Kyu:** HLD me "deploy/scale/monitor kaise" ki depth aur STAR ke liye asli material.

- [01 cdn](05_INFRA_DEEP/01_cdn.md) · [02 load-balancer](05_INFRA_DEEP/02_load_balancer.md) · [03 dns](05_INFRA_DEEP/03_dns.md) · [04 waf](05_INFRA_DEEP/04_waf.md) · [05 monitoring (Prometheus/Grafana)](05_INFRA_DEEP/05_monitoring_prometheus_grafana.md) · [load-test demo](05_INFRA_DEEP/LOADTEST_SCALE_DEMO/NOTES.md)
- **[DEVOPS](05_INFRA_DEEP/DEVOPS)** — cheat-commands — [AWS](05_INFRA_DEEP/DEVOPS/AWS) · [DOCKER](05_INFRA_DEEP/DEVOPS/DOCKER) · [K8S](05_INFRA_DEEP/DEVOPS/K8S) (helm + setup-journey)
- **Hands-on demos:** [LB_DEMO](05_INFRA_DEEP/LB_DEMO) (nginx-lb.conf) · [MONITORING_DEMO](05_INFRA_DEEP/MONITORING_DEMO) (prometheus.yml) · [LOADTEST_SCALE_DEMO](05_INFRA_DEEP/LOADTEST_SCALE_DEMO)

---

## [06_COMPARES](06_COMPARES) — 14 "X vs Y"
**Kya:** side-by-side tulna, ek-ek file. &nbsp; **Kyu:** rapid-fire — interviewer "farak batao" pooche to turant chahiye.

[01 sql-vs-nosql](06_COMPARES/01_sql_vs_nosql.md) · [02 tcp-vs-udp](06_COMPARES/02_tcp_vs_udp.md) · [03 http-vs-https](06_COMPARES/03_http_vs_https.md) · [04 monolith-vs-microservices](06_COMPARES/04_monolith_vs_microservices.md) · [05 stateful-vs-stateless](06_COMPARES/05_stateful_vs_stateless.md) · [06 sync-vs-async](06_COMPARES/06_sync_vs_async.md) · [07 authn-vs-authz](06_COMPARES/07_authentication_vs_authorization.md) · [08 process-vs-thread](06_COMPARES/08_process_vs_thread.md) · [09 concurrency-vs-parallelism](06_COMPARES/09_concurrency_vs_parallelism.md) · [10 rest-vs-graphql](06_COMPARES/10_rest_vs_graphql.md) · [11 http-versions](06_COMPARES/11_http_versions.md) · [12 ws-vs-sse-vs-polling](06_COMPARES/12_websockets_vs_sse_vs_long_polling.md) · [13 jwt-vs-oauth](06_COMPARES/13_jwt_vs_oauth.md) · [14 kafka-vs-rabbitmq](06_COMPARES/14_kafka_vs_rabbitmq.md)

---

## [07_PROJECTS](07_PROJECTS) — hands-on Spring Boot
**Kya:** 5 chalne wale projects. &nbsp; **Kyu:** resume-backing + "tumne kya banaya" ka live proof; **usercrud = saara Spring concept ek jagah hands-on**.

- [usercrud](07_PROJECTS/usercrud) — **main sandbox** (sab concept live) · [mini_payment_ms](07_PROJECTS/mini_payment_ms) ([learnings](07_PROJECTS/mini_payment_ms/LEARNINGS.md)) · [todoapp](07_PROJECTS/todoapp) · [Microservices-CRUD](07_PROJECTS/Microservices-CRUD) · [InsuranceManagementSystem](07_PROJECTS/InsuranceManagementSystem)

#### usercrud — live demo sandbox
> Yahi wo "connection" hai. Notes (Spring/Java folder me) = **theory (kya + kyu)**. Neeche ki demo-files (`07_PROJECTS/usercrud` me = inka asli ghar) = **wahi cheez CHAL-KE**. Note isi demo ko link karta.

| Concept | Live demo (usercrud me) | Theory note (kya+kyu) |
|---------|-------------------------|-----------------------|
| Proxy · singleton · prototype · BeanPostProcessor | [SpringInternalsDemo](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/demo/SpringInternalsDemo.java) `/internals` | [proxy JDK-vs-CGLIB](02_SPRING/01_basics/04_proxy_jdk_cglib.md) |
| Lazy vs Eager · LazyInitException + 2 fix | [LazyDemoController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/LazyDemoController.java) | [lazy-vs-eager](02_SPRING/05_HIBERNATE/03_lazy_vs_eager.md) |
| N+1 problem (live) | [N1Controller](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/N1Controller.java) · [N1Seeder](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/N1Seeder.java) | [N+1](02_SPRING/05_HIBERNATE/02_n_plus_1_problem.md) |
| Optimistic locking (@Version conflict) | [OptLockController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/OptLockController.java) | [optimistic-lock](02_SPRING/05_HIBERNATE/08_optimistic_locking.md) |
| QueryDSL dynamic query | [UserSearchController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/UserSearchController.java) · [QueryDslConfig](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/config/QueryDslConfig.java) | [querydsl](02_SPRING/09_QUERYDSL/01_querydsl.md) |
| GraphQL (client picks fields) | [UserGraphQLController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/UserGraphQLController.java) | [graphql](02_SPRING/11_GRAPHQL/01_graphql.md) |
| @Async · @Scheduled | [AsyncDemoController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/AsyncDemoController.java) · [DemoJobs](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/DemoJobs.java) | [async-scheduled](02_SPRING/12_ASYNC_SCHEDULING/01_async_scheduled.md) |
| @Cacheable · @CacheEvict | [CacheDemoController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/CacheDemoController.java) · [CacheDemoService](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/service/CacheDemoService.java) | [cache](02_SPRING/13_CACHE/01_cache.md) |
| Kafka produce/consume · DLQ · idempotency | [KafkaProducerController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/KafkaProducerController.java) · [KafkaConsumer](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/KafkaConsumer.java) · [KafkaConfig](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/config/KafkaConfig.java) · [IdempotencyController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/IdempotencyController.java) | [kafka-basics](02_SPRING/14_KAFKA/01_kafka_basics.md) |
| JWT · Spring Security filter-chain | [AuthController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/AuthController.java) · [SecurityConfig](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/security/SecurityConfig.java) | [jwt-complete](02_SPRING/03_jwt_security/01_jwt_complete.md) |
| volatile visibility (live) | [VolatileDemoController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/VolatileDemoController.java) | [volatile](01_JAVA/04_MULTITHREADING/05_volatile.md) |
| Rate limiter | [RateLimitController](07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/RateLimitController.java) | [rate-limiter (HLD)](04_HLD/SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md) |

---

## [08_DSA](08_DSA) — 18 pattern · ~200 problem
**Kya:** pattern-wise solved problems + master sheet. &nbsp; **Kyu:** interview ka **gate** — pass karna hai. Ab naya nahi, sirf REVISE + spaced-redo.

- ★ **[00_PATTERN_SHEET](08_DSA/00_PATTERN_SHEET/PATTERN_SHEET.md)** — "gate ki chaabi" (har pattern ka saar + template + dry-run) — **roz-glance**.

| # | Pattern | Problems | | # | Pattern | Problems |
|---|---------|----------|---|---|---------|----------|
| 01 | [TWO_POINTER](08_DSA/01_TWO_POINTER) | 22 | | 10 | [KADANE](08_DSA/10_KADANE) | 14 |
| 02 | [SLIDING_WINDOW](08_DSA/02_SLIDING_WINDOW) | 23 | | 11 | [BIT_MANIPULATION](08_DSA/11_BIT_MANIPULATION) | 1 |
| 03 | [HASHING](08_DSA/03_HASHING) | 18 | | 12 | [TREES](08_DSA/12_TREES) | 19 |
| 04 | [PREFIX_SUM](08_DSA/04_PREFIX_SUM) | 10 | | 13 | [GRAPHS](08_DSA/13_GRAPHS) | 13 |
| 05 | [MATRIX](08_DSA/05_MATRIX) | 5 | | 14 | [HEAP](08_DSA/14_HEAP) | 8 |
| 06 | [STACK](08_DSA/06_STACK) | 12 | | 15 | [INTERVALS](08_DSA/15_INTERVALS) | 6 |
| 07 | [BINARY_SEARCH](08_DSA/07_BINARY_SEARCH) | 18 | | 16 | [BACKTRACKING](08_DSA/16_BACKTRACKING) | 8 |
| 08 | [LINKED_LIST](08_DSA/08_LINKED_LIST) | 13 | | 17 | [DP](08_DSA/17_DP) | 25 |
| 09 | [DESIGN](08_DSA/09_DESIGN) | 5 | | 18 | [GREEDY](08_DSA/18_GREEDY) | 5 |

---

## [09_DATABASE](09_DATABASE)
**Kya:** DB internals hands-on (index/txn/lock). &nbsp; **Kyu:** backend + HLD ka data-layer — "query slow kyun / deadlock kaise" real Qs.

[01 indexing](09_DATABASE/01_indexing_practical.md) · [02 transaction-internals](09_DATABASE/02_transaction_internals.md) · [03 zero-downtime-add-column](09_DATABASE/03_zero_downtime_add_column.md) · [04 joins](09_DATABASE/04_joins.md) · [05 normalization](09_DATABASE/05_normalization.md) · [06 ACID](09_DATABASE/06_acid.md) · [07 isolation-levels](09_DATABASE/07_isolation_levels.md) · [08 deadlock](09_DATABASE/08_deadlock.md)

---

## [10_BEHAVIORAL](10_BEHAVIORAL) — STAR
**Kya:** JP-fit STAR kahaniyaan. &nbsp; **Kyu:** pura behavioral round; "tell me about a time" — bolke ready rakhni.

[01 JP-company](10_BEHAVIORAL/01_JP_COMPANY.md) · [02 why-JP](10_BEHAVIORAL/02_WHY_JP.md) · [03 STAR: HikariCP](10_BEHAVIORAL/03_STAR_HikariCP.md) · [04 STAR: WAF-IP](10_BEHAVIORAL/04_STAR_WAF_IP.md) · [05 STAR: payment](10_BEHAVIORAL/05_STAR_Doximity_Payment.md) · [06 STAR: mentoring](10_BEHAVIORAL/06_STAR_Mentoring.md) · [07 STAR: failure-JDK](10_BEHAVIORAL/07_STAR_Failure_JDK.md)

---

## [11_GIT](11_GIT)
**Kya:** git ke woh corner jo interview/PR me aate. &nbsp; **Kyu:** code-review + daily workflow Qs.

[01 line-endings CRLF-vs-LF](11_GIT/01_line_endings_CRLF_vs_LF.md) · [02 stacked-PRs](11_GIT/02_stacked_PRs.md) · [03 merge-vs-rebase](11_GIT/03_merge_vs_rebase.md)

---

## [12_CODE_REVIEW](12_CODE_REVIEW)
**Kya:** PR-review round ka framework. &nbsp; **Kyu:** JP round-2 me "is PR me kya galat" — 5-bucket se chalna.

[PR-review checklist (5-bucket) + worked drills](12_CODE_REVIEW/PR_REVIEW_CHECKLIST.md) — security · java-traps · resource · db · design.

---

> Har link actual repo file/folder pe jaata — click karo, seedha khul jaayega.
