# Prep — Backend Engineering Interview Workspace

Java / Spring backend interview prep — DSA, system design, projects, hands-on notes.
**Neeche poora TREE hai — kisi bhi file pe click → seedha wahi note khul jaata.**

---

## Sections (ek nazar)

| # | Section | Kya hai |
|---|---------|---------|
| 01 | [**01_JAVA**](01_JAVA) | Core Java · Collections · Java-8 · Multithreading · SOLID · Design Patterns · Brain-ticklers |
| 02 | [**02_SPRING**](02_SPRING) | Boot · @Transactional · JWT · Hibernate · Docker · K8s · QueryDSL · GraphQL · Kafka · Cache · Async |
| 03 | [**03_LLD**](03_LLD) | Low-level design — 11 designs in code |
| 04 | [**04_HLD**](04_HLD) | System design — foundations + 11 designs + approach-playbook |
| 05 | [**05_INFRA_DEEP**](05_INFRA_DEEP) | CDN · LB · DNS · WAF · Prometheus/Grafana · load-test |
| 06 | [**06_COMPARES**](06_COMPARES) | 14 side-by-side compares |
| 07 | [**07_PROJECTS**](07_PROJECTS) | Hands-on Spring Boot projects |
| 08 | [**08_DSA**](08_DSA) | 19 patterns · ~200 problems |
| 09 | [**09_DATABASE**](09_DATABASE) | Indexing · transactions · isolation · deadlock |
| 10 | [**10_BEHAVIORAL**](10_BEHAVIORAL) | STAR stories (JP) |
| 11 | [**11_GIT**](11_GIT) | Git workflows |

> ★ Roz-revise: **[DSA Pattern Sheet](08_DSA/00_PATTERN_SHEET/PATTERN_SHEET.md)** · **[HLD Approach Playbook](04_HLD/HLD_APPROACH_DELIVERY.md)**

---

## 01_JAVA
- **[02_COLLECTIONS](01_JAVA/02_COLLECTIONS)** — [hashmap-internal](01_JAVA/02_COLLECTIONS/06_hashmap_internal.md) · [hashmap-vs-CHM](01_JAVA/02_COLLECTIONS/07_hashmap_vs_concurrenthashmap.md) · [arraylist](01_JAVA/02_COLLECTIONS/25_arraylist_internal.md) · [linkedlist](01_JAVA/02_COLLECTIONS/26_linkedlist_internal.md) · [iterator-vs-listiterator](01_JAVA/02_COLLECTIONS/27_iterator_vs_listiterator.md) · [hashmap/linkedhashmap/treemap](01_JAVA/02_COLLECTIONS/28_hashmap_linkedhashmap_treemap.md) · [hashset](01_JAVA/02_COLLECTIONS/29_hashset_internal.md) · [comparable-vs-comparator](01_JAVA/02_COLLECTIONS/30_comparable_vs_comparator.md) · [thenComparing](01_JAVA/02_COLLECTIONS/31_thencomparing_chained_sorting.md) · [fail-fast-vs-safe](01_JAVA/02_COLLECTIONS/32_fail_fast_vs_fail_safe.md) · [priorityqueue](01_JAVA/02_COLLECTIONS/33_priorityqueue_min_heap.md)
- **[03_JAVA_8](01_JAVA/03_JAVA_8)** — [lambda](01_JAVA/03_JAVA_8/34_lambda_expressions.md) · [functional-interface](01_JAVA/03_JAVA_8/35_functional_interface_pfcs.md) · [method-reference](01_JAVA/03_JAVA_8/36_method_reference.md) · [streams](01_JAVA/03_JAVA_8/37_streams_api.md) · [optional](01_JAVA/03_JAVA_8/38_optional.md) · [stream-cheatsheet](01_JAVA/03_JAVA_8/39_stream_methods_cheatsheet.md)
- **[04_MULTITHREADING](01_JAVA/04_MULTITHREADING)** — [basics](01_JAVA/04_MULTITHREADING/39_thread_basics.md) · [lifecycle](01_JAVA/04_MULTITHREADING/40_thread_lifecycle.md) · [join](01_JAVA/04_MULTITHREADING/40b_join.md) · [synchronized](01_JAVA/04_MULTITHREADING/41_synchronized.md) · [volatile](01_JAVA/04_MULTITHREADING/42_volatile.md) · [atomic/CAS](01_JAVA/04_MULTITHREADING/42b_atomic_integer_cas.md) · [deadlock](01_JAVA/04_MULTITHREADING/43_deadlock.md) · [wait-vs-sleep](01_JAVA/04_MULTITHREADING/44_wait_vs_sleep.md) · [notify](01_JAVA/04_MULTITHREADING/45_notify_deep_dive.md) · [executor/pool](01_JAVA/04_MULTITHREADING/46_executor_service_thread_pool.md) · [future-vs-completablefuture](01_JAVA/04_MULTITHREADING/47_future_vs_completablefuture.md) · [producer-consumer](01_JAVA/04_MULTITHREADING/48_producer_consumer.md)
- **[05_SOLID](01_JAVA/05_SOLID)** — [kya/kyun](01_JAVA/05_SOLID/61_solid_kya_hai_kyu.md) · [S](01_JAVA/05_SOLID/62_S_single_responsibility.md) · [O](01_JAVA/05_SOLID/63_O_open_closed.md) · [L](01_JAVA/05_SOLID/64_L_liskov_substitution.md) · [I](01_JAVA/05_SOLID/65_I_interface_segregation.md) · [D](01_JAVA/05_SOLID/66_D_dependency_inversion.md)
- **[06_DESIGN_PATTERNS](01_JAVA/06_DESIGN_PATTERNS)** — [builder](01_JAVA/06_DESIGN_PATTERNS/01_builder.md) · [singleton](01_JAVA/06_DESIGN_PATTERNS/02_singleton.md) · [factory](01_JAVA/06_DESIGN_PATTERNS/03_factory.md) · [observer](01_JAVA/06_DESIGN_PATTERNS/04_observer.md) · [adapter](01_JAVA/06_DESIGN_PATTERNS/05_adapter.md) · [strategy](01_JAVA/06_DESIGN_PATTERNS/06_strategy.md)
- **[07_BRAIN_TICKLERS](01_JAVA/07_BRAIN_TICKLERS)** — 13 gotchas (string-intern, equals-trap, immutable, diamond, final-vs-finally-vs-finalize, covariant-return, ...)

## 02_SPRING
- **[01_basics](02_SPRING/01_basics)** — [spring-kya-hai](02_SPRING/01_basics/01_spring_kya_hai.md) · [beans/IoC](02_SPRING/01_basics/02_beans_ioc_container.md) · [annotations/DI](02_SPRING/01_basics/03_annotations_di_types.md)
- **[02_transactional](02_SPRING/02_transactional/01_transactional_complete.md)** — @Transactional complete
- **[03_jwt_security](02_SPRING/03_jwt_security/01_jwt_complete.md)** — JWT complete
- **[04_profiles](02_SPRING/04_profiles)** — [basics](02_SPRING/04_profiles/01_basics.md) · [compose](02_SPRING/04_profiles/02_compose_integration.md) · [reference](02_SPRING/04_profiles/03_reference.md)
- **[05_HIBERNATE](02_SPRING/05_HIBERNATE)** — [jpa-vs-hibernate](02_SPRING/05_HIBERNATE/01_jpa_vs_hibernate.md) · [N+1](02_SPRING/05_HIBERNATE/02_n_plus_1_problem.md) · [lazy-vs-eager](02_SPRING/05_HIBERNATE/03_lazy_vs_eager.md) · [dirty-checking](02_SPRING/05_HIBERNATE/04_dirty_checking.md) · [1st-level-cache](02_SPRING/05_HIBERNATE/05_first_level_cache.md) · [cascade](02_SPRING/05_HIBERNATE/06_cascade_types.md) · [relationships](02_SPRING/05_HIBERNATE/07_relationship_mappings.md) · [optimistic-lock](02_SPRING/05_HIBERNATE/08_optimistic_locking.md)
- **[06_DOCKER](02_SPRING/06_DOCKER)** — foundation → compose → networks → volumes → multistage → arg-vs-env (11 files)
- **[07_K8S](02_SPRING/07_K8S)** — [why/arch](02_SPRING/07_K8S/01_why_k8s_and_architecture.md) · [pod](02_SPRING/07_K8S/02_pod.md) · [deployment](02_SPRING/07_K8S/03_deployment.md) · [service](02_SPRING/07_K8S/04_service.md) · [ingress](02_SPRING/07_K8S/05_ingress.md) · [configmap/secret](02_SPRING/07_K8S/06_configmap_secret.md)
- **[08_REVISION_VISUAL](02_SPRING/08_REVISION_VISUAL)** — basics · rest-flow · transactional · jwt · profiles (visual)
- **[09_QUERYDSL](02_SPRING/09_QUERYDSL/01_querydsl.md)** · **[10_SPRING_DATA_JPA](02_SPRING/10_SPRING_DATA_JPA/01_custom_queries_dto.md)** · **[11_GRAPHQL](02_SPRING/11_GRAPHQL/01_graphql.md)** · **[12_ASYNC_SCHEDULING](02_SPRING/12_ASYNC_SCHEDULING/01_async_scheduled.md)** · **[13_CACHE](02_SPRING/13_CACHE/01_cache.md)** · **[14_KAFKA](02_SPRING/14_KAFKA/01_kafka_basics.md)**

## 03_LLD (code — 11 designs)
- [parking-lot](03_LLD/01_parking_lot) · [bookmyshow](03_LLD/02_bookmyshow) · [tictactoe](03_LLD/03_tictactoe) · [snake-ladder](03_LLD/04_snake_ladder) · [elevator](03_LLD/06_elevator) · [atm](03_LLD/07_atm_machine) · [bank-account](03_LLD/08_bank_account) · [shopping-cart](03_LLD/09_shopping_cart) · [library](03_LLD/10_library) · [vending-machine](03_LLD/11_vending_machine) · [hotel-booking](03_LLD/12_hotel_booking)

## 04_HLD
- ★ **[HLD_APPROACH_DELIVERY](04_HLD/HLD_APPROACH_DELIVERY.md)** — "kaise bolna" playbook (rail · meta-moves · anjaan-design · darr) · **[INTERVIEW_FRAMEWORK](04_HLD/INTERVIEW_FRAMEWORK.md)** · **[SPEAKING_VOCAB](04_HLD/SPEAKING_VOCAB.md)**
- **[FOUNDATIONS](04_HLD/FOUNDATIONS)** — [hld-kya-hai](04_HLD/FOUNDATIONS/01_hld_kya_hai.md) · [capacity-estimation](04_HLD/FOUNDATIONS/02_capacity_estimation.md) · [load-balancing](04_HLD/FOUNDATIONS/03_load_balancing.md) · [caching](04_HLD/FOUNDATIONS/04_caching.md) · [replication](04_HLD/FOUNDATIONS/05_database_replication.md) · [sharding](04_HLD/FOUNDATIONS/06_database_sharding.md) · [message-queues](04_HLD/FOUNDATIONS/07_message_queues.md) · [CAP](04_HLD/FOUNDATIONS/08_cap_theorem.md) · [db-what-when](04_HLD/FOUNDATIONS/09_databases_what_when.md) · [ms-communication](04_HLD/FOUNDATIONS/10_ms_communication.md) · [reliability/SPOF](04_HLD/FOUNDATIONS/11_reliability_spof_cloud.md) · [elasticsearch](04_HLD/FOUNDATIONS/12_elasticsearch_search.md) · [snowflake-id](04_HLD/FOUNDATIONS/13_distributed_id_snowflake.md)
- **[SYSTEM_DESIGNS](04_HLD/SYSTEM_DESIGNS)** — [url-shortener](04_HLD/SYSTEM_DESIGNS/11_url_shortener/11_url_shortener.md) · [rate-limiter](04_HLD/SYSTEM_DESIGNS/12_rate_limiter/12_rate_limiter.md) · [twitter-feed](04_HLD/SYSTEM_DESIGNS/13_twitter_feed/13_twitter_feed.md) · [notification](04_HLD/SYSTEM_DESIGNS/14_notification_system/14_notification_system.md) · [url-browser-journey](04_HLD/SYSTEM_DESIGNS/15_url_browser_journey/15_url_browser_journey.md) · [stock-broker](04_HLD/SYSTEM_DESIGNS/16_stock_broker_trading/16_stock_broker_trading.md) · [payment](04_HLD/SYSTEM_DESIGNS/17_payment_system/17_payment_system.md) · [file-upload](04_HLD/SYSTEM_DESIGNS/18_file_upload_validate_system/18_file_upload_INTERVIEW.md) · [news-aggregator](04_HLD/SYSTEM_DESIGNS/19_news_aggregator/19_news_aggregator_INTERVIEW.md) · [bookmyshow](04_HLD/SYSTEM_DESIGNS/20_bookmyshow/20_bookmyshow_INTERVIEW.md) · [distributed-cache](04_HLD/SYSTEM_DESIGNS/21_distributed_cache/21_distributed_cache.md)
- **[RAW_DRILL](04_HLD/RAW_DRILL)** — [reflex-sheet](04_HLD/RAW_DRILL/00_REFLEX_SHEET.md) · [design-practice-log](04_HLD/RAW_DRILL/01_DESIGN_PRACTICE_LOG.md) · [why-how-QA](04_HLD/RAW_DRILL/02_WHY_HOW_QA.md) · [interview-anchor](04_HLD/RAW_DRILL/03_INTERVIEW_ANCHOR.md)

## 05_INFRA_DEEP
- [cdn](05_INFRA_DEEP/01_cdn.md) · [load-balancer](05_INFRA_DEEP/02_load_balancer.md) · [dns](05_INFRA_DEEP/03_dns.md) · [waf](05_INFRA_DEEP/04_waf.md) · [monitoring (Prometheus/Grafana)](05_INFRA_DEEP/05_monitoring_prometheus_grafana.md) · [load-test demo](05_INFRA_DEEP/LOADTEST_SCALE_DEMO/NOTES.md)

## 06_COMPARES
- [sql-vs-nosql](06_COMPARES/01_sql_vs_nosql.md) · [tcp-vs-udp](06_COMPARES/02_tcp_vs_udp.md) · [http-vs-https](06_COMPARES/03_http_vs_https.md) · [monolith-vs-ms](06_COMPARES/04_monolith_vs_microservices.md) · [stateful-vs-stateless](06_COMPARES/05_stateful_vs_stateless.md) · [sync-vs-async](06_COMPARES/06_sync_vs_async.md) · [authn-vs-authz](06_COMPARES/07_authentication_vs_authorization.md) · [process-vs-thread](06_COMPARES/08_process_vs_thread.md) · [concurrency-vs-parallelism](06_COMPARES/09_concurrency_vs_parallelism.md) · [rest-vs-graphql](06_COMPARES/10_rest_vs_graphql.md) · [http-versions](06_COMPARES/11_http_versions.md) · [ws-vs-sse-vs-polling](06_COMPARES/12_websockets_vs_sse_vs_long_polling.md) · [jwt-vs-oauth](06_COMPARES/13_jwt_vs_oauth.md) · [kafka-vs-rabbitmq](06_COMPARES/14_kafka_vs_rabbitmq.md)

## 07_PROJECTS (hands-on Spring Boot)
- [mini_payment_ms](07_PROJECTS/mini_payment_ms) ([learnings](07_PROJECTS/mini_payment_ms/LEARNINGS.md)) · [todoapp](07_PROJECTS/todoapp) · [usercrud](07_PROJECTS/usercrud) · [Microservices-CRUD](07_PROJECTS/Microservices-CRUD) · [InsuranceManagementSystem](07_PROJECTS/InsuranceManagementSystem)

## 08_DSA (19 patterns · ~200 problems)
- ★ **[00_PATTERN_SHEET](08_DSA/00_PATTERN_SHEET/PATTERN_SHEET.md)** — "gate ki chaabi" (saar + template + dry-run)
- [01_TWO_POINTER](08_DSA/01_TWO_POINTER) (22) · [02_SLIDING_WINDOW](08_DSA/02_SLIDING_WINDOW) (23) · [03_HASHING](08_DSA/03_HASHING) (18) · [04_PREFIX_SUM](08_DSA/04_PREFIX_SUM) (10) · [05_MATRIX](08_DSA/05_MATRIX) (5) · [06_STACK](08_DSA/06_STACK) (12) · [07_BINARY_SEARCH](08_DSA/07_BINARY_SEARCH) (18) · [08_LINKED_LIST](08_DSA/08_LINKED_LIST) (13) · [09_DESIGN](08_DSA/09_DESIGN) (5) · [10_KADANE](08_DSA/10_KADANE) (14) · [11_BIT_MANIPULATION](08_DSA/11_BIT_MANIPULATION) (1) · [12_TREES](08_DSA/12_TREES) (19) · [13_GRAPHS](08_DSA/13_GRAPHS) (13) · [14_HEAP](08_DSA/14_HEAP) (8) · [15_INTERVALS](08_DSA/15_INTERVALS) (6) · [16_BACKTRACKING](08_DSA/16_BACKTRACKING) (8) · [17_DP](08_DSA/17_DP) (25) · [18_GREEDY](08_DSA/18_GREEDY) (5)

## 09_DATABASE
- [indexing](09_DATABASE/01_indexing_practical.md) · [transaction-internals](09_DATABASE/02_transaction_internals.md) · [zero-downtime-add-column](09_DATABASE/03_zero_downtime_add_column.md) · [joins](09_DATABASE/04_joins.md) · [normalization](09_DATABASE/05_normalization.md) · [ACID](09_DATABASE/06_acid.md) · [isolation-levels](09_DATABASE/07_isolation_levels.md) · [deadlock](09_DATABASE/08_deadlock.md)

## 10_BEHAVIORAL (STAR)
- [JP-company](10_BEHAVIORAL/01_JP_COMPANY.md) · [why-JP](10_BEHAVIORAL/02_WHY_JP.md) · [STAR: HikariCP](10_BEHAVIORAL/03_STAR_HikariCP.md) · [STAR: WAF-IP](10_BEHAVIORAL/04_STAR_WAF_IP.md) · [STAR: payment](10_BEHAVIORAL/05_STAR_Doximity_Payment.md) · [STAR: mentoring](10_BEHAVIORAL/06_STAR_Mentoring.md) · [STAR: failure-JDK](10_BEHAVIORAL/07_STAR_Failure_JDK.md)

## 11_GIT
- [line-endings CRLF-vs-LF](11_GIT/01_line_endings_CRLF_vs_LF.md) · [stacked-PRs](11_GIT/02_stacked_PRs.md) · [merge-vs-rebase](11_GIT/03_merge_vs_rebase.md)

---

> Har link actual repo file/folder pe jaata — click karo, seedha khul jaayega.
