# Topic 5 — Ingress (External Traffic Routing)

> **TL;DR:** Ingress = K8s ka NGINX. Same role, same logs, same kaam. Bus K8s context mein.

[← Back to README](00_README.md) | [← Service](04_service.md) | [ConfigMap+Secret →](06_configmap_secret.md)

---

## ONE-LINE ANSWER (yaad rakh)

```
Ingress = NGINX (in K8s context)
   • Same reverse proxy role
   • Same L7 routing
   • Same access logs (IP, URL, status, user-agent)
   • AWS EC2 mein nginx = K8s mein Ingress
   
Bus or kuch nahi.
```

---

## The Problem

```
Tu ne 5 microservices banaye:
   • auth-service       (LoadBalancer = 1 cloud LB = $$$)
   • user-service       (LoadBalancer = 1 cloud LB = $$$)
   • order-service      (LoadBalancer = 1 cloud LB = $$$)
   • payment-service    (LoadBalancer = 1 cloud LB = $$$)
   • notification       (LoadBalancer = 1 cloud LB = $$$)
   
   = 5 LBs = 5 public IPs = 5x cost
```

```
ALSO:
   TLS certs har service pe alag manage
   Path routing (/api/users vs /api/orders) impossible
   Single domain mein multiple services = headache
```

---

## Solution — Ingress (just like nginx in AWS)

```
Ingress = SINGLE entry point + ROUTING RULES

   ONE LoadBalancer
        ↓
   Ingress receives all traffic (LIKE NGINX)
        ↓
   Routes based on RULES:
      api.com/users    → user-service
      api.com/orders   → order-service
      api.com/payments → payment-service
      
   = 1 LB instead of 5 = cost saved 
   = Centralized routing + TLS 
```

---

## STORY — Office Reception

```
Without reception (each Service = LoadBalancer):
   • Visitor enter karta directly any office
   • 5 separate entrances
   • No single security
   • Hard to control who goes where

With reception (Ingress = nginx):
   • Single main entrance
   • Reception receives ALL visitors
   • Routes based on rules:
      "User dept? → 3rd floor"
      "Orders? → 5th floor"
      "Payments? → 7th floor"
   • Single security check (TLS)
   • One point of control
```

---

## Visual

```
   INTERNET
      │
      │ Single public IP
      ▼
   ┌─────────────────────────────────────┐
   │  INGRESS CONTROLLER (NGINX!!!)        │
   │  • Same role as AWS nginx            │
   │  • TLS termination                    │
   │  • Routing rules                      │
   │  • Access logs (IP, URL, status)      │
   └────────┬─────────────────────────────┘
            │
            │ Routes based on URL path/host
            │
            ├──── /api/users ────► user-service ────► user pods
            ├──── /api/orders ───► order-service ───► order pods
            ├──── /api/payments ─► payment-service ─► payment pods
            └──── /admin ────────► admin-service ───► admin pods
```

---

## Logs Captured (same as nginx)

```
Ingress level pe log hota:
   Source IP (kis user)
   Destination URL (kahan hit)
   HTTP status (200/500/404)
   Response time
   User-Agent (browser/app)
   Referrer
   Request size + response size
   Timestamp

= Same as AWS nginx access logs
= Same nginx log format option (combined log format)
```

---

## Ingress Provides

```
1. SINGLE ENTRY POINT      → 1 LB instead of N
2. PATH ROUTING            → /api/users → service A
3. HOST ROUTING            → api.com vs admin.com → different services
4. TLS TERMINATION         → HTTPS handled at Ingress level
5. CENTRALIZED RULES        → One YAML for routing logic
6. COST SAVINGS            → Less cloud LBs needed
7. ACCESS LOGS              → Same as nginx
```

---

## Two Components of Ingress

```
1. INGRESS RESOURCE (YAML rules — what tu likhta)
   • Routing rules: path → service
   • Host configurations
   • TLS settings
   
2. INGRESS CONTROLLER (actual implementation)
   • nginx-ingress (most common — LITERALLY nginx)
   • AWS Load Balancer Controller (uses ALB)
   • Traefik
   • Kong
   
   Ingress resource alone = USELESS without controller
   Controller = the engine that READS rules + ROUTES traffic
```

---

## Ingress Manifest Example

```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: usercrud-ingress
  annotations:
    nginx.ingress.kubernetes.io/rewrite-target: /
spec:
  ingressClassName: nginx
  
  tls:                                      # ← HTTPS support
  - hosts:
    - api.example.com
    secretName: api-tls-cert
  
  rules:
  - host: api.example.com                   # ← host-based routing
    http:
      paths:
      - path: /users                        # ← path-based routing
        pathType: Prefix
        backend:
          service:
            name: user-service
            port:
              number: 8080
      
      - path: /orders
        pathType: Prefix
        backend:
          service:
            name: order-service
            port:
              number: 8080
      
      - path: /payments
        pathType: Prefix
        backend:
          service:
            name: payment-service
            port:
              number: 8080
```

---

## Routing Patterns

### **Path-based routing** (most common):
```
api.example.com/users    → user-service
api.example.com/orders   → order-service
api.example.com/admin    → admin-service

Single domain, different paths
(Same as nginx location blocks)
```

### **Host-based routing**:
```
api.example.com          → api-service
admin.example.com        → admin-service
docs.example.com         → docs-service

Multiple subdomains, different services
(Same as nginx server blocks per domain)
```

### **Combination**:
```
api.example.com/v1/users   → user-service-v1
api.example.com/v2/users   → user-service-v2
admin.example.com          → admin-service

Path + host together
```

---

## Service vs Ingress

| | **Service (LoadBalancer)** | **Ingress** |
|---|---|---|
| Layer | L4 (TCP/UDP) | L7 (HTTP/HTTPS) |
| Routing | Pod selection (label) | URL path/host |
| TLS | Manual per service | Centralized at Ingress |
| Cost | 1 LB per service | 1 LB for all |
| Use case | Simple external | Multi-service routing |
| Analogous to | AWS ELB | AWS nginx EC2 / ALB |

---

## Production Pattern (AWS EKS example)

```
   INTERNET
      │
      ▼
   AWS ALB (provisioned by Ingress)
      │
   AWS Load Balancer Controller (Ingress Controller)
      │
   Reads Ingress YAML rules
      │
      ├─► /users    ─► user-service     ─► user pods
      ├─► /orders   ─► order-service    ─► order pods
      └─► /payments ─► payment-service  ─► payment pods

= Production-grade microservices routing
= Logs captured at ALB level (same as nginx access logs)
```

---

## kubectl Commands

```cmd
kubectl apply -f ingress.yaml         # create ingress
kubectl get ingress                   # list
kubectl describe ingress usercrud-ingress
kubectl delete ingress usercrud-ingress

# Check Ingress Controller (must be installed)
kubectl get pods -n ingress-nginx     # if using nginx controller

# Get Ingress Controller logs (= nginx access logs)
kubectl logs -n ingress-nginx <controller-pod-name>
```

---

## Quick Interview Sense

**Q: "Ingress kya hai?"**

> *"K8s ka nginx — same reverse proxy role. L7 (HTTP/HTTPS) routing layer above Services. Single entry point for cluster external traffic. Path-based + host-based routing. TLS termination centralized. Cost-effective: 1 LoadBalancer instead of N. Two parts: Ingress Resource (YAML rules) + Ingress Controller (nginx-ingress/AWS ALB — actual engine). Logs same as nginx (IP, URL, status, etc)."*

**Q: "LoadBalancer Service vs Ingress?"**

> *"LoadBalancer = L4 (TCP), 1 LB per service, expensive. Ingress = L7 (HTTP), 1 LB for multiple services with path/host routing, cost-effective. Production microservices: Ingress preferred — centralized routing + TLS at single point. Ingress = nginx in K8s context."*

**Q: "Ingress alone enough?"**

> *"NO! Ingress resource is JUST RULES (YAML). Need Ingress Controller (nginx-ingress, AWS ALB Controller) to actually implement routing. Cluster mein controller install ho — phir Ingress resources kaam karte."*

---

## Common Traps

```
Trap 1: Ingress without Controller
         Apply Ingress YAML, expect routing
         Install Ingress Controller FIRST (nginx-ingress)

Trap 2: Wrong ingressClassName
         Default class but specifying wrong name
         Match controller's class (nginx, alb, etc.)

Trap 3: TLS cert in wrong namespace
         Secret in default, Ingress in app namespace
         TLS Secret + Ingress in SAME namespace

Trap 4: Path conflict
         Multiple paths overlap (/users vs /users/profile)
         Order matters; specific paths first

Trap 5: HTTPS expected without TLS section
         rules.host: api.com but no tls: block
         Always include tls section for HTTPS
```

---

## Power Phrase

> *"Ingress = K8s ka nginx. L7 HTTP/HTTPS routing layer above Services. Single entry point with path-based + host-based rules. TLS termination centralized. Saves cost — 1 LoadBalancer instead of N. Ingress = Resource (YAML rules) + Controller (nginx-ingress / AWS ALB — actual implementation). Logs same as nginx (IP, URL, status, user-agent). Production microservices use Ingress for centralized routing + TLS."*

---

## Memory Hook

```
Ingress = "Same as nginx, just K8s context"
   • AWS EC2 nginx = K8s Ingress
   • Same role: reverse proxy + L7 routing
   • Same logs: IP, URL, status, user-agent
   • Bus K8s mein declarative YAML se manage

Office reception analogy:
   • Single main entrance (1 LB)
   • Receptionist routes visitors
   • "User dept? 3rd floor"
   • Single security check (TLS)

Two parts:
   • Resource = "Rules paper"
   • Controller = "Receptionist (executes rules)"
```

---

## Concept Locked

```
Ingress = K8s ka nginx (CORE INSIGHT)
L7 (HTTP/HTTPS) routing layer
Path-based + host-based routing
TLS termination centralized
Cost effective (1 LB vs N LBs)
Resource (YAML) + Controller (engine) = both needed
Logs same as nginx
AWS production = ALB controller usually
```

[← Back to README](00_README.md) | [← Service](04_service.md) | [ConfigMap+Secret →](06_configmap_secret.md)
