# 🌍 Spring Profiles — Visual Revision

---

## 1️⃣ Problem (Wardrobe Analogy)

```
   Tu morning office jaata → formals
   Tu evening gym jaata → tracksuit
   Tu shaadi attend → sherwani
   
   SAME TU — different OUTFIT per occasion.
```

```
   App ka same scenario:
   
   Tu DEV machine pe code likh raha → H2 in-memory DB
   Tu STAGING server pe deploy → MySQL on staging
   Tu PRODUCTION pe deploy → MySQL on AWS RDS
   
   SAME APP — different CONFIG per environment.
```

---

## 2️⃣ Without Profiles (Pain)

```
Manual config swap:
   1. Code mein DB URL hardcode
   2. Test ke time MySQL URL change
   3. Prod deploy ke time wapas change
   4. Bhul gaye? = Production breaks
   = Disaster
```

---

## 3️⃣ With Profiles (Clean)

```
   ┌────────────────────────────────────────┐
   │  application.properties (default)       │
   │  spring.profiles.active=dev             │
   └────────────────────────────────────────┘
   
   ┌────────────────────────────────────────┐
   │  application-dev.properties             │
   │  spring.datasource.url=jdbc:h2:mem:...  │
   └────────────────────────────────────────┘
   
   ┌────────────────────────────────────────┐
   │  application-prod.properties            │
   │  spring.datasource.url=jdbc:mysql://prod│
   │  spring.datasource.username=...         │
   │  spring.datasource.password=...         │
   └────────────────────────────────────────┘
   
   ┌────────────────────────────────────────┐
   │  application-compose.properties         │
   │  spring.datasource.url=jdbc:mysql://    │
   │     mysql:3306/usercrud  ← container DNS│
   └────────────────────────────────────────┘
```

```
Spring AUTO loads matching profile based on:
   spring.profiles.active = <profile>
```

---

## 4️⃣ How Spring Picks the File

```
   App start
      │
      ▼
   Read spring.profiles.active
      │
      ▼
   Active = "prod"
      │
      ▼
   Load application.properties (default base)
      +
   Load application-prod.properties (override)
      │
      ▼
   prod-specific values WIN where conflict
```

---

## 5️⃣ Activate Profile — 3 Ways

```
1. application.properties:
   spring.profiles.active=prod

2. JVM argument:
   java -jar app.jar -Dspring.profiles.active=prod

3. Environment variable:
   export SPRING_PROFILES_ACTIVE=prod
   java -jar app.jar

4. Docker / K8s ConfigMap:
   SPRING_PROFILES_ACTIVE=compose
```

---

## 6️⃣ @Profile Annotation (Beans)

```java
@Service
@Profile("prod")
public class RealEmailService implements EmailService {
    // Real SMTP integration
}

@Service
@Profile("dev")
public class FakeEmailService implements EmailService {
    public void send(...) {
        log.info("Email skipped in dev");
    }
}
```

```
   Active = dev    → FakeEmailService injected
   Active = prod   → RealEmailService injected
   
   = Same interface, different bean per profile
```

---

## 7️⃣ Real Project Pattern (UserCRUD)

```
   ┌───────────────────────────────────────────┐
   │ application.properties                     │
   │ spring.application.name=usercrud          │
   │ spring.profiles.active=dev   ← default     │
   └───────────────────────────────────────────┘

   ┌───────────────────────────────────────────┐
   │ application-dev.properties                 │
   │ spring.datasource.url=jdbc:h2:mem:devdb   │
   │ spring.jpa.hibernate.ddl-auto=create-drop │
   └───────────────────────────────────────────┘

   ┌───────────────────────────────────────────┐
   │ application-mysql.properties               │
   │ spring.datasource.url=                     │
   │     jdbc:mysql://localhost:3307/usercrud  │
   │ spring.jpa.hibernate.ddl-auto=update      │
   └───────────────────────────────────────────┘

   ┌───────────────────────────────────────────┐
   │ application-compose.properties             │
   │ spring.datasource.url=                     │
   │     jdbc:mysql://mysql:3306/usercrud      │
   │     ↑                                      │
   │     container name (Docker DNS)            │
   └───────────────────────────────────────────┘
```

---

## 8️⃣ Visual — Profile in Action

```
Local development:
   ┌─────────────────────────┐
   │  IntelliJ run config:    │
   │  active=dev              │
   └────────────┬────────────┘
                │
                ▼
        H2 in-memory DB
        = Fast, no setup

Local Docker:
   ┌─────────────────────────┐
   │  docker run -e           │
   │   SPRING_PROFILES_ACTIVE=│
   │   compose                │
   └────────────┬────────────┘
                │
                ▼
        MySQL container

Production K8s:
   ┌─────────────────────────┐
   │  ConfigMap:              │
   │  SPRING_PROFILES_ACTIVE  │
   │  = prod                  │
   └────────────┬────────────┘
                │
                ▼
        MySQL on RDS
```

---

## 9️⃣ Common Mistakes

```
❌ Hardcoding DB URL in main application.properties
   ✅ Use profile-specific files

❌ Sensitive data (passwords) in git-committed properties
   ✅ Use environment variables / secret manager

❌ Forgetting to set profile in production
   ✅ Always explicit (env var or K8s ConfigMap)

❌ Multiple profiles overlapping properties chaos
   ✅ Common stuff in default application.properties
   ✅ Profile-specific only overrides
```

---

## 🔟 Profile Hierarchy (Override Chain)

```
   1. Command-line args     ← HIGHEST priority
   2. JVM -D flags
   3. OS environment vars
   4. application-{profile}.properties
   5. application.properties  ← LOWEST priority
   
   Higher wins on conflict
```

---

## 🎯 Memory Hook

```
Wardrobe = Profile system
Outfits  = different profile property files
Occasion = environment (dev, prod, compose)

@Profile = bean appears only on right occasion
spring.profiles.active = "kya pehnu aaj"
```

---

## 💎 One-Line Summary

```
"Profiles = wardrobe for app config.
 Different config per environment (dev/prod/compose).
 application-{profile}.properties auto-loaded.
 spring.profiles.active = current outfit.
 @Profile annotation = beans appear only on matching profile."
```

📚 [← Back to SPRING](../)
