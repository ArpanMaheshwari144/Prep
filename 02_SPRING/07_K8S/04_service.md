# Topic 4 — Service (Network Access for Pods)

> **Solves:** Pods are ephemeral (IPs change). Service = stable IP + DNS.
> **Critical:** Production = ALWAYS reach pods via Service, never pod IP

[← Back to README](00_README.md) | [← Deployment](03_deployment.md) | [Ingress →](05_ingress.md)

---

## The Problem K8s Service Solves

```
Tu Deployment chalaya — 3 pods running:
   Pod1: IP 10.244.0.5
   Pod2: IP 10.244.0.7
   Pod3: IP 10.244.0.9

Pod1 crashed → K8s ne naya banaya:
   Pod1 (NEW): IP 10.244.0.12     ← IP BADAL GAYA!

Issue:
   Other apps Pod1 ke OLD IP pe call kar rahe → BROKEN
   Pods are EPHEMERAL — IPs change on every restart
```

---

## Solution — Service

```
Service = STABLE IP + DNS NAME for a SET of pods

Pods rotate karo / IPs change → koi farak nahi
Service ka IP + name = HAMESHA SAME
Traffic auto-routed to healthy pods
```

---

## STORY — Restaurant Phone Number

```
PODS              = waiters (shifts change, new waiters daily)
SERVICE           = Restaurant's PUBLIC PHONE NUMBER
   • Customer calls phone → reception routes to ANY available waiter
   • Waiter changes? Phone number SAME hi
   • Customer ko specific waiter ka naam yaad nahi rakhna

Service = stable contact point for changing pods
```

---

## Visual

```
   ┌─────────────────────────────────────────┐
   │  CLIENT (other pod / external)            │
   │     │                                      │
   │     │ http://usercrud-service:8080         │
   │     │ (stable DNS)                         │
   │     ▼                                      │
   │  ┌────────────────────────────────────┐   │
   │  │  SERVICE (usercrud-service)         │   │
   │  │  IP: 10.96.45.10 (STABLE)            │   │
   │  │  Selector: app=usercrud             │   │
   │  └─────────┬──────────────────────────┘   │
   │            │ load balance                   │
   │            ▼                                │
   │   ┌────┐  ┌────┐  ┌────┐                   │
   │   │Pod1│  │Pod2│  │Pod3│  ← pods can change│
   │   │10.5│  │10.7│  │10.9│    IPs over time  │
   │   └────┘  └────┘  └────┘                   │
   └─────────────────────────────────────────┘
```

---

## Service Provides 5 Things

```
1. STABLE IP / DNS         → Service name = DNS, never changes
2. LOAD BALANCING          → Traffic distributed across pods
3. SERVICE DISCOVERY       → Other apps reach by service name
4. POD SELECTION           → Label selector picks matching pods
5. AUTO-FAILOVER           → Unhealthy pod removed from rotation
```

---

## 4 Service Types (interview classic)

```
1. ClusterIP    (default — INTERNAL only)
2. NodePort     (basic external — dev)
3. LoadBalancer (cloud external — production )
4. ExternalName (DNS alias)
```

---

### **1) ClusterIP — INTERNAL ONLY** (default)
```
Visible only inside cluster
   • Other pods can reach
   • External traffic = NO

Use case:
   • Microservice-to-microservice talk
   • Spring Boot ↔ Redis (both inside cluster)
   • Backend-for-backend
```

### **2) NodePort — Basic External**
```
Service exposed on each NODE's IP at fixed port (30000-32767)
   • External user → Node IP : 30080 → Service → Pod

Use case:
   • Dev / testing only
   • NOT production (use LoadBalancer instead)
```

### **3) LoadBalancer — Production External **
```
Cloud provider provisions external LB (AWS ELB, GCP LB)
   • Public IP assigned
   • Traffic: Internet → LB → Service → Pods

Use case:
   • Production public APIs
   • Most common for prod external services
```

### **4) ExternalName — DNS Alias**
```
Service maps to external DNS (CNAME)
   • No proxy, no load balancing
   • Just DNS pointer

Use case:
   • Cluster pods need to reach external service
   • my-db.cluster → external.db.aws.com
```

---

## Service Manifest — ClusterIP (most common)

```yaml
apiVersion: v1
kind: Service
metadata:
  name: usercrud-service
spec:
  type: ClusterIP                # default
  selector:
    app: usercrud                 # match pods with this label
  ports:
  - protocol: TCP
    port: 8080                    # service port
    targetPort: 8080              # pod port
```

---

## Service Manifest — LoadBalancer (production)

```yaml
apiVersion: v1
kind: Service
metadata:
  name: usercrud-public
spec:
  type: LoadBalancer
  selector:
    app: usercrud
  ports:
  - port: 80                      # external port (public)
    targetPort: 8080              # pod port
```

---

## Service DNS (Cluster Internal)

```
K8s automatically creates DNS:

Pods inside cluster reach service via:
   http://usercrud-service:8080         (same namespace)
   http://usercrud-service.default:8080  (different namespace)
   http://usercrud-service.default.svc.cluster.local:8080  (full)

= Just like docker-compose service names!
```

---

## Real Spring App + MySQL Service Example

```yaml
# MySQL Deployment
apiVersion: apps/v1
kind: Deployment
metadata:
  name: mysql
spec:
  replicas: 1
  selector:
    matchLabels:
      app: mysql
  template:
    metadata:
      labels:
        app: mysql
    spec:
      containers:
      - name: mysql
        image: mysql:8
        env:
        - name: MYSQL_ROOT_PASSWORD
          value: rootpass

---
# MySQL Service
apiVersion: v1
kind: Service
metadata:
  name: mysql                    # ← service name = DNS hostname
spec:
  type: ClusterIP
  selector:
    app: mysql
  ports:
  - port: 3306
    targetPort: 3306
```

```yaml
# Spring app connects via:
spring.datasource.url=jdbc:mysql://mysql:3306/userdb
                                    ↑↑↑↑↑
                                    Service name (DNS)
```

**Same pattern as docker-compose** — service name as hostname.

---

## Quick Comparison Table

| Type | Scope | Use Case | Production? |
|---|---|---|---|
| ClusterIP | Internal | Microservice talk (default) | Yes (internal) |
| NodePort | Basic external | Dev / testing | No |
| LoadBalancer | Cloud external | Public APIs | Yes (external) |
| ExternalName | DNS alias | External mapping | Yes (specific) |

---

## kubectl Commands

```cmd
kubectl apply -f service.yaml         # create
kubectl get services                  # list (or 'kubectl get svc')
kubectl describe service usercrud-service
kubectl delete service usercrud-service

# Find service IP:
kubectl get svc usercrud-service
   → NAME              TYPE       CLUSTER-IP    PORT(S)
   → usercrud-service  ClusterIP  10.96.45.10   8080/TCP
```

---

## Quick Interview Sense

**Q: "Service kya hai K8s mein?"**

> *"Service = stable network endpoint for set of pods. Pods ephemeral (IPs change), service IP + DNS stable. Load balances across matching pods (label selector). 4 types: ClusterIP (internal), NodePort (basic external), LoadBalancer (production), ExternalName (DNS alias). Other pods reach by service name DNS."*

**Q: "Pod IP vs Service IP?"**

> *"Pod IP ephemeral, changes on restart. Service IP stable. Production = always reach pods via Service, not pod IP directly. Service uses label selector to find matching pods."*

**Q: "ClusterIP vs LoadBalancer?"**

> *"ClusterIP = default, internal only (microservice talk). LoadBalancer = cloud-managed external LB, public IP, production external services. NodePort = simple external exposure (dev), LoadBalancer preferred for prod."*

---

## Common Traps

```
Trap 1: Pod IP direct use
         Hardcode pod IP in app config
         Use Service name (stable DNS)

Trap 2: Selector mismatch
         Service selector ≠ Pod labels
         → Service routes to ZERO pods
         Match exactly:
            Service.spec.selector.app: usercrud
            Pod.metadata.labels.app: usercrud

Trap 3: NodePort in production
         NodePort exposes node IPs (security risk)
         LoadBalancer for production

Trap 4: Port vs targetPort confusion
         Mix up service port and pod port
         port = service entry, targetPort = pod listen
            (often same, but can differ)

Trap 5: Cross-namespace DNS
         Same namespace mein 'mysql' use,
            different namespace fail
         Use FQDN: mysql.namespace.svc.cluster.local
```

---

## Power Phrase

> *"Service = stable network frontend for ephemeral pods. Pods rotate (IPs change), Service IP + DNS stable. Label selector picks matching pods, load balances traffic. 4 types: ClusterIP (internal default), NodePort (basic external), LoadBalancer (cloud production), ExternalName (DNS alias). Service name = DNS hostname inside cluster — same pattern as docker-compose."*

---

## Memory Hook

```
Service = "Restaurant phone number"
   • Waiters change (pods)
   • Phone number stable (Service)
   • Customer calls → reception routes to free waiter

Types (memorize):
   ClusterIP     → "Inside the building only" (internal)
   NodePort      → "Side door entry" (dev)
   LoadBalancer  → "Main reception with security" (prod )
   ExternalName  → "Forward to another building" (DNS alias)

DNS pattern:
   service-name = hostname (auto-resolves)
   Just like docker-compose service names
```

---

## Concept Locked

```
Pods ephemeral, Service stable (IP + DNS)
Label selector picks matching pods
Load balancing automatic
4 types (ClusterIP / NodePort / LoadBalancer / ExternalName)
DNS resolution by service name
ClusterIP for internal (default)
LoadBalancer for production external
Same pattern as docker-compose service names
```

[← Back to README](00_README.md) | [← Deployment](03_deployment.md) | [Ingress →](05_ingress.md)
