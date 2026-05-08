# ⚓ Kubernetes (K8s) — Complete Notes

> **Topic:** Container orchestration at scale
> **Date started:** 2026-05-06 (Day 2 evening)
> **Mode:** Theory deep, hands-on later (Docker Desktop has K8s built-in for practice)

---

## 📚 READ ORDER (Strict — section by section)

```
1. 📖 00_README.md                    ← yeh file (start here)
                                        Overview + reading order

2. 🟢 01_why_k8s_and_architecture.md  ← K8s zaroori kyu, control plane vs workers
                                        [Foundation — must read first]

3. 🟢 02_pod.md                       ← Smallest deployable unit
                                        Container vs Pod difference
                                        Sidecar pattern

4. 🟢 03_deployment.md                ← Production-grade Pod management
                                        Self-healing, rolling updates, rollback
                                        ReplicaSet hierarchy

5. 🟢 04_service.md                   ← Network access for pods
                                        ClusterIP/NodePort/LoadBalancer/ExternalName
                                        [Day 2 morning — done]

6. 🟡 05_ingress.md                   ← External traffic routing
                                        = K8s ka NGINX (same role)
                                        L7 routing, TLS, path/host based
                                        [Day 2 morning — done]

7. 🟡 06_configmap_secret.md          ← Config injection
                                        = Spring profiles ka K8s equivalent
                                        ConfigMap (non-sensitive) + Secret (sensitive)
                                        [Day 2 morning — done]

8. 🔜 (lower priority — optional):
      07_namespaces.md                 ← Multi-tenancy
      08_hpa.md                        ← Auto-scaling
      09_pv_pvc.md                     ← Persistent volumes

---

## 🚀 Practical (Hands-On Folder)

📂 [PRACTICAL/](PRACTICAL/) — Real deployment journey
   ├── 01_practical_journey.md   ← Phase 1-6 step-by-step
                                   (cluster + MySQL + Spring Boot + Docker Hub + ports)
   = Theory yahan upar (00-09)
   = Practical alag folder mein (PRACTICAL/)
```

---

## 🎯 Why K8s in Notes? (Spring Project Context)

```
Phase 3 (July/August 2026 — locked memory):
   • Build full Spring microservices project
   • Deploy on AWS EKS (Kubernetes managed)
   • K8s = production deployment standard
   
Yeh notes us project ke liye foundation:
   • Pod, Deployment, Service understanding
   • Manifests likhne ka pattern
   • kubectl commands
   • Production patterns
```

---

## 🔑 K8s vs Docker (refresher)

```
Docker  = LOW level  (containers banao, run karo)
K8s     = HIGH level (containers ko manage karo at scale)

Same domain (containers)
Different abstraction levels

K8s sits ON TOP of Docker (or containerd)
Both work TOGETHER in production
```

---

## 🎯 What You'll Learn

```
✅ Why K8s (Docker alone vs K8s)
✅ K8s architecture (Control plane + Worker nodes)
✅ Pod — smallest deployable unit
✅ Deployment — self-healing + rolling updates
✅ Service — network access patterns (upcoming)
✅ Ingress — external routing (upcoming)
✅ ConfigMap + Secret — config (upcoming)
✅ HPA — auto-scaling (upcoming)
✅ PV/PVC — persistent volumes (upcoming)
```

---

## 💎 K8s 5 Superpowers

```
1. AUTO-SCALING       → Traffic up? more pods auto.
2. SELF-HEALING       → Pod crash? auto-replace.
3. ROLLING UPDATES    → Zero downtime deploys.
4. LOAD BALANCING     → Traffic split across healthy pods.
5. SERVICE DISCOVERY  → Pods find each other by name.
```

---

## 🚀 Production Stack Preview (Phase 3)

```
Spring Boot microservice
   ↓ Dockerfile.multistage
   ↓ docker build → image
   ↓ push to AWS ECR
   ↓
K8s Deployment YAML
   ↓ kubectl apply
   ↓
Pods running (3 replicas)
   ↓ Service (LoadBalancer)
   ↓ Ingress (TLS, routing)
   ↓
Public traffic ✅
```

---

**Ready? Start with [01_why_k8s_and_architecture.md](01_why_k8s_and_architecture.md)** →
