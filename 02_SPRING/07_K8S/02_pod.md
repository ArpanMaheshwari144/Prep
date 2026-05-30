# Topic 2 — Pod (Smallest Deployable Unit)

> **Foundation:** Container vs Pod difference
> **Key:** K8s manages PODS, not containers directly

[← Back to README](00_README.md) | [← Why K8s](01_why_k8s_and_architecture.md) | [Deployment →](03_deployment.md)

---

## What's a Pod?

```
Container       → Docker se bana ek running app
Pod             → K8s mein DEPLOY ki ek smallest unit
                  Wraps 1 or more containers

Smallest unit:
   K8s containers ko DIRECTLY manage NAHI karta
   K8s manages PODS
   Pod ke andar containers
```

---

## STORY — Apartment Unit

```
Container = ek banda (process)
Pod       = apartment unit
   • Usually 1 banda (1 container per pod)
   • Sometimes 2-3 (sidecar containers — flatmates)
   • Same address (1 IP per pod)
   • Same kitchen (shared storage)
   • Same building entrance (shared network)

   Same pod ke containers = roommates
   Different pod ke containers = different apartments
```

---

## Pod Key Properties

```
1. SHARED NETWORK
   All containers in pod = SAME IP, SAME ports namespace
   Container A → localhost:8080 → reach Container B

2. SHARED STORAGE (volumes)
   Pod-level volumes mount to multiple containers
   Sidecar logging agent reads main app's logs

3. SHARED LIFECYCLE
   Pod start → all containers start
   Pod die → all containers die
   Tightly coupled

4. EPHEMERAL by default
   Pod crash → DELETED (no auto-restore alone)
   For resilience → DEPLOYMENT manages pods (next topic)
```

---

## Visual

```
   ┌─────────────────────────────────────┐
   │  POD (usercrud-pod)                  │
   │  IP: 10.244.0.5                      │
   │  ┌────────────────────────────────┐ │
   │  │ Container 1                     │ │
   │  │ • usercrud-app                  │ │
   │  │ • port 8080                     │ │
   │  └────────────────────────────────┘ │
   │  ┌────────────────────────────────┐ │
   │  │ Container 2 (optional sidecar) │ │
   │  │ • log-shipper                   │ │
   │  │ • reads main app logs           │ │
   │  └────────────────────────────────┘ │
   │                                      │
   │  Shared: network namespace,          │
   │          IPC namespace,              │
   │          volumes                     │
   └─────────────────────────────────────┘
```

---

## Pod Manifest (YAML — declarative)

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: usercrud-pod
  labels:
    app: usercrud
spec:
  containers:
  - name: usercrud
    image: usercrud-app:multi
    ports:
    - containerPort: 8080
    env:
    - name: SPRING_PROFILES_ACTIVE
      value: "compose"
```

```cmd
kubectl apply -f pod.yaml             # create pod
kubectl get pods                       # list pods
kubectl describe pod usercrud-pod      # details
kubectl logs usercrud-pod              # see logs
kubectl exec -it usercrud-pod -- bash  # shell into pod
kubectl delete pod usercrud-pod        # delete
```

---

## Pod vs Container

| | **Container** | **Pod** |
|---|---|---|
| Layer | Docker concept | K8s concept |
| Smallest unit | In Docker | In K8s |
| Network | Own IP | Shared IP per pod |
| Storage | Own filesystem | Shared volumes optional |
| Scheduling | Docker decides | K8s schedules |
| Manage | docker run | kubectl apply |

---

## Why Wrap Containers in Pods?

```
Single container = simple
Multiple containers in 1 pod = tightly coupled helpers

EXAMPLES:
   1. Main app + logging sidecar
      App writes logs to volume
      Sidecar (Fluentd) reads logs, ships to ELK

   2. Main app + service mesh proxy (Istio)
      Sidecar handles ingress/egress traffic
      App focuses on business logic

   3. Main app + OAuth proxy
      Sidecar terminates auth
      App stays simple

COMMON: 1 container per pod (90% cases)
ADVANCED: sidecar pattern (production patterns)
```

---

## Sidecar Pattern Visual

```
   ┌─────────────────────────────────┐
   │  POD                             │
   │                                  │
   │  ┌──────────────┐                │
   │  │ MAIN APP     │ writes logs    │
   │  │ (Spring Boot) │ ──┐            │
   │  └──────────────┘    │            │
   │                       ▼           │
   │                  shared volume    │
   │                       ▲           │
   │  ┌──────────────┐    │            │
   │  │ SIDECAR      │ ──┘            │
   │  │ (Fluentd)     │ reads logs     │
   │  │ ships to ELK  │                │
   │  └──────────────┘                │
   └─────────────────────────────────┘

   Main + helper = tightly coupled
   Both in same Pod
   Share network + storage
```

---

## Pod is EPHEMERAL — Important

```
Pod CRASH → simply gone (no auto-replace)
Pod DELETE → no recovery
Node down → all pods on that node gone
Memory limit hit → killed

Solution: NEVER deploy raw pods in production
Use: DEPLOYMENT (next topic) — manages pod lifecycle
```

---

## Quick Interview Sense

**Q: "Pod kya hai?"**

> *"K8s ki smallest deployable unit. Wraps 1+ containers sharing network, storage, lifecycle. Pod me containers same IP share, localhost se baat karte. Production: 1 container per pod usually, sidecar pattern advanced. Pods ephemeral, isliye Deployment se manage."*

**Q: "Pod vs Container?"**

> *"Container = Docker layer (running app). Pod = K8s wrapper around containers. K8s schedules pods, not containers directly. Multiple containers in pod = tightly coupled (sidecar)."*

**Q: "Sidecar pattern?"**

> *"Main app + helper container in SAME pod, sharing network/storage. Examples: logging agent, service mesh proxy (Istio), OAuth proxy. Helper enhances main app without coupling code. Production K8s pattern."*

---

## Power Phrase

> *"Pod = smallest deployable unit in K8s. Wraps 1+ containers sharing network (1 IP), storage (volumes), lifecycle. K8s schedules pods, not containers. Pods ephemeral by default — production use Deployment to manage pod lifecycle (replicas + auto-restart). Sidecar pattern = multiple containers in 1 pod for tightly coupled helpers (logging, proxy)."*

---

## Memory Hook

```
Pod = "Apartment unit"
   • 1 IP (1 address)
   • Shared kitchen (volumes)
   • Roommates = sidecar containers
   • Usually 1 person (1 container)
   • If unit demolished — everyone moves out (ephemeral)

Layers:
   Container (Docker level)
        ↑ wrapped in
   Pod (K8s level)
        ↑ managed by
   Deployment (production pattern, next topic)
```

---

## Concept Locked

```
Pod = smallest K8s unit
Wraps 1+ containers (usually 1)
Shared network (1 IP), storage, lifecycle
Ephemeral by default
Sidecar pattern for tightly coupled helpers
kubectl apply / get / describe / logs / exec / delete
Production: NEVER raw pods, always via Deployment
```

[← Back to README](00_README.md) | [← Why K8s](01_why_k8s_and_architecture.md) | [Deployment →](03_deployment.md)
