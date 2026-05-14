# 🟡 Topic 6 — ConfigMap + Secret (Config Injection)

> **TL;DR:** ConfigMap + Secret = Spring's `application-{profile}.properties` ka K8s equivalent. Decouple config from image (12-factor app).

📚 [← Back to README](00_README.md) | [← Ingress](05_ingress.md)

---

## 🤔 The Problem

```
Tera Spring Boot container:
   • DB URL hardcoded mein nahi rakh sakte
   • Different env (dev/staging/prod) = different values
   • Same image, different config NEEDED
   
   = "Config injection" chahiye
```

**Tera Compose pattern remember kar:**
```yaml
environment:
  SPRING_PROFILES_ACTIVE: compose
  SPRING_DATASOURCE_URL: jdbc:mysql://mysql:3306/userdb
```

**K8s mein same kaam = ConfigMap + Secret**

---

## 🎯 ConfigMap vs Secret — Quick

```
ConfigMap     = Non-sensitive config
                (DB URL, profile name, log level)

Secret        = Sensitive data
                (DB password, API key, JWT secret)

DONO inject hote pods mein as:
   • Environment variables, OR
   • Mounted files
```

---

## 🎬 STORY — Recipe vs Pantry

```
Container image  = "frozen meal" (sealed package)
ConfigMap        = "label on package" (cooking instructions, public)
Secret           = "secret family recipe" (locked, protected)

Both customize the meal at serve time
without changing the frozen package itself
```

---

## 📝 ConfigMap Manifest

```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: usercrud-config
data:
  SPRING_PROFILES_ACTIVE: compose
  SPRING_DATASOURCE_URL: jdbc:mysql://mysql:3306/userdb
  LOG_LEVEL: INFO
  TZ: Asia/Kolkata
```

---

## 📝 Secret Manifest

```yaml
apiVersion: v1
kind: Secret
metadata:
  name: usercrud-secret
type: Opaque
data:
  # values must be base64 encoded
  DB_PASSWORD: cm9vdHBhc3M=        # base64("rootpass")
  JWT_SECRET: bXlTdXBlclNlY3JldA== # base64("mySuperSecret")

# OR use stringData (auto-encodes):
stringData:
  DB_PASSWORD: rootpass
  JWT_SECRET: mySuperSecret
```

---

## 🎨 How Pod Uses Them

### **Method 1: As Environment Variables** (most common)

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: usercrud-deployment
spec:
  template:
    spec:
      containers:
      - name: usercrud
        image: usercrud-app:multi
        envFrom:
        - configMapRef:
            name: usercrud-config       # ← all ConfigMap entries as env vars
        - secretRef:
            name: usercrud-secret       # ← all Secret entries as env vars
```

```
Result inside pod:
   $SPRING_PROFILES_ACTIVE = "compose"
   $SPRING_DATASOURCE_URL = "jdbc:mysql://mysql:3306/userdb"
   $DB_PASSWORD = "rootpass"            (from Secret)
   $JWT_SECRET = "mySuperSecret"        (from Secret)
```

### **Method 2: Specific Keys**
```yaml
env:
- name: SPRING_PROFILES_ACTIVE
  valueFrom:
    configMapKeyRef:
      name: usercrud-config
      key: SPRING_PROFILES_ACTIVE
- name: DB_PASSWORD
  valueFrom:
    secretKeyRef:
      name: usercrud-secret
      key: DB_PASSWORD
```

### **Method 3: Mount as Files**
```yaml
volumes:
- name: config-volume
  configMap:
    name: usercrud-config
- name: secret-volume
  secret:
    secretName: usercrud-secret

containers:
- name: usercrud
  volumeMounts:
  - name: config-volume
    mountPath: /etc/config
  - name: secret-volume
    mountPath: /etc/secrets
```
**Use case:** Mount `application.properties` file directly.

---

## 🆚 ConfigMap vs Secret

| | **ConfigMap** | **Secret** |
|---|---|---|
| For | Non-sensitive | Sensitive |
| Encoding | Plain text | Base64 (NOT encryption!) |
| Examples | DB URL, profile | DB password, API key |
| etcd storage | Plain | Base64 (still readable) |
| Production secrets | OK | ⚠️ Use Vault/AWS Secrets Manager |

---

## ⚠️ CRITICAL — Secret Is NOT Encrypted!

```
Secret = base64 ENCODED, not ENCRYPTED
   • base64 = trivially decodable (echo $X | base64 -d)
   • Anyone with cluster access can read
   • etcd stores in base64 (not encrypted by default)

PRODUCTION REALITY:
   ❌ K8s Secret alone for sensitive prod data
   ✅ Use external secret managers:
      • AWS Secrets Manager
      • HashiCorp Vault
      • Sealed Secrets
      • External Secrets Operator
   
   These INJECT real secrets at runtime, K8s Secret = pointer
```

---

## 🔗 Real Spring + ConfigMap + Secret (Production-Pattern)

```yaml
# ConfigMap (non-sensitive)
apiVersion: v1
kind: ConfigMap
metadata:
  name: spring-config
data:
  SPRING_PROFILES_ACTIVE: production
  SPRING_DATASOURCE_URL: jdbc:mysql://mysql-prod:3306/userdb
  LOG_LEVEL: INFO

---
# Secret (sensitive)
apiVersion: v1
kind: Secret
metadata:
  name: spring-secret
stringData:
  SPRING_DATASOURCE_USERNAME: root
  SPRING_DATASOURCE_PASSWORD: actualProductionPassword
  JWT_SECRET: realProductionJwtSecret

---
# Deployment uses both
apiVersion: apps/v1
kind: Deployment
metadata:
  name: usercrud
spec:
  template:
    spec:
      containers:
      - name: usercrud
        image: usercrud-app:multi
        envFrom:
        - configMapRef:
            name: spring-config
        - secretRef:
            name: spring-secret
```

**Spring Boot picks up env vars automatically.** Same as `application-compose.properties` pattern.

---

## 🛠️ kubectl Commands

```cmd
# Create from YAML
kubectl apply -f configmap.yaml
kubectl apply -f secret.yaml

# Create directly (CLI shortcut)
kubectl create configmap usercrud-config --from-literal=LOG_LEVEL=INFO
kubectl create secret generic usercrud-secret --from-literal=DB_PASSWORD=rootpass

# View
kubectl get configmaps              # or 'kubectl get cm'
kubectl get secrets
kubectl describe configmap usercrud-config

# View Secret values (base64 decoded):
kubectl get secret usercrud-secret -o jsonpath='{.data.DB_PASSWORD}' | base64 -d

# Delete
kubectl delete configmap usercrud-config
kubectl delete secret usercrud-secret
```

---

## 💡 Connection to Spring Profiles (your existing knowledge)

```
DOCKER COMPOSE:
   docker-compose.yml:
     environment:
       SPRING_PROFILES_ACTIVE: compose
       DB_PASSWORD: rootpass
       
   ↓ (same pattern)

KUBERNETES:
   ConfigMap: SPRING_PROFILES_ACTIVE
   Secret:    DB_PASSWORD
   Deployment envFrom: both
   
= SAME concept, different syntax
= Spring Boot reads env vars same way
= Decouples config from image (12-factor app)
```

---

## 🎤 Quick Interview Sense

**Q: "ConfigMap vs Secret?"**

> *"ConfigMap = non-sensitive config (DB URL, profile, log level), plain text. Secret = sensitive data (passwords, keys), base64 encoded (NOT encrypted!). Both inject as env vars or mounted files. Production secrets: K8s Secret alone insufficient — use AWS Secrets Manager / Vault for real encryption."*

**Q: "Same image multiple env mein kaise?"**

> *"Build once, deploy many — 12-factor app principle. Image fixed, ConfigMap + Secret per environment differ. Dev cluster has ConfigMap with dev DB URL, Prod cluster with prod URL. Same image, different config injection."*

**Q: "Secret really secure?"**

> *"NO — K8s Secret = base64 encoded, NOT encrypted. Anyone with cluster access can decode. Production: use external managers (AWS Secrets Manager, Vault, Sealed Secrets) — they inject REAL secrets at runtime, K8s Secret just acts as pointer/reference."*

---

## ⚠️ Common Traps

```
🪤 Trap 1: Treating Secret as encrypted
         ❌ Sensitive prod data in K8s Secret only
         ✅ Use external secret manager (Vault/AWS)

🪤 Trap 2: ConfigMap update not reflecting
         ❌ Update ConfigMap, expect immediate pod refresh
         ✅ Restart pods OR use rollout restart deployment

🪤 Trap 3: Forgetting base64 in Secret YAML
         ❌ Plain text in data: section
         ✅ Use stringData: (auto-encodes)
            OR base64-encode manually

🪤 Trap 4: ConfigMap with too much data
         ❌ Mounting huge config files
         ✅ ConfigMap limit is 1MB; large files use volume

🪤 Trap 5: Cross-namespace reference
         ❌ ConfigMap in default, deployment in app namespace
         ✅ Same namespace required
```

---

## 💎 Power Phrase

> *"ConfigMap + Secret = K8s ka config injection. ConfigMap for non-sensitive (DB URL, profile, log level). Secret for sensitive (passwords, keys) — but base64 encoded NOT encrypted. Both inject as env vars or files. Same Docker image deployed multiple environments — different ConfigMap/Secret per env (12-factor app). Production: external secret managers (Vault, AWS Secrets Manager) for real security."*

---

## 🧠 Memory Hook

```
ConfigMap = "Spring's application-{env}.properties for K8s"
   • Public config (DB URL, profile name)
   • Inject as env vars

Secret = "Vault inside K8s" (but base64 encoded)
   • Sensitive (passwords, keys)
   • Production = external manager (Vault/AWS)

Pattern same as Compose:
   docker-compose environment: → K8s ConfigMap/Secret + envFrom
```

---

## ✅ Concept Locked

```
✅ ConfigMap = non-sensitive injection
✅ Secret = sensitive (base64, NOT encrypted)
✅ envFrom + valueFrom usage patterns
✅ Mount as env vars or files
✅ Production secrets = external manager
✅ Same image, different config per env (12-factor)
✅ Spring Boot reads env vars automatically
```

📚 [← Back to README](00_README.md) | [← Ingress](05_ingress.md)
