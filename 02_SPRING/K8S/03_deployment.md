# 🟢 Topic 3 — Deployment (Production-Grade Pod Management)

> **Critical:** Production K8s = NEVER raw pods, ALWAYS Deployment
> **Provides:** Self-healing, rolling updates, rollback, scaling

📚 [← Back to README](00_README.md) | [← Pod](02_pod.md)

---

## 🤔 Why Deployment?

```
Pod alone = problem:
   ❌ Pod crash → gone (no auto-replace)
   ❌ Need 3 copies? Manually start 3 pods
   ❌ Update image? Manually delete + recreate
   ❌ No rollback if bad deploy
   
Solution: DEPLOYMENT
   "Hey K8s, mujhe ALWAYS 3 pods running chahiye
    of usercrud-app:multi image. Tu manage kar."
```

---

## 🎬 STORY — Property Manager

```
Pod              = ek apartment (kabhi bhi destroy ho sakta)
Deployment       = Building's PROPERTY MANAGER
   "Hey, hamesha 3 apartments OCCUPIED rakho.
    Koi apartment khali ho jaye?
    Naya tenant fauran lao.
    
    Renovation karna? (image update)
    Ek apartment empty karo, naya banao,
    fir doosra, fir teesra — gradual.
    Customer (traffic) bina disturb."
```

---

## 🎯 Deployment ka Job

```
1. DESIRED STATE          → "3 replicas chahiye"
2. SELF-HEALING           → Pod crash? naya pod auto
3. ROLLING UPDATE         → Image v1 → v2 gradual replace, zero downtime
4. ROLLBACK               → Naya version buggy? wapas v1 instantly
5. SCALE                  → Traffic up? replicas badhao (3→10)
```

---

## 🎨 Architecture Hierarchy

```
   ┌──────────────────────────────────────────┐
   │  DEPLOYMENT (usercrud-deployment)         │
   │  Desired: 3 replicas of usercrud-app      │
   │                                            │
   │   ┌────────────────────────────────────┐  │
   │   │  REPLICASET (usercrud-rs-abc123)    │  │
   │   │  "Maintain 3 pods of this template" │  │
   │   │                                      │  │
   │   │   ┌────┐  ┌────┐  ┌────┐            │  │
   │   │   │Pod1│  │Pod2│  │Pod3│            │  │
   │   │   └────┘  └────┘  └────┘            │  │
   │   └────────────────────────────────────┘  │
   └──────────────────────────────────────────┘
   
   Layer hierarchy:
      Deployment → manages → ReplicaSet → manages → Pods
      (high-level)           (mid-level)             (execution)
```

---

## 📝 Deployment Manifest (YAML)

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: usercrud-deployment
  labels:
    app: usercrud
spec:
  replicas: 3                   # ← maintain 3 pods always
  selector:
    matchLabels:
      app: usercrud             # ← which pods to manage (label match)
  template:                     # ← pod template
    metadata:
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
        resources:
          requests:
            memory: "256Mi"
            cpu: "250m"
          limits:
            memory: "512Mi"
            cpu: "500m"
```

---

## 🛠️ Common kubectl Commands

```cmd
# Create / update
kubectl apply -f deployment.yaml

# View
kubectl get deployments
kubectl get pods                       # 3 pods auto-created
kubectl describe deployment usercrud-deployment

# Scale
kubectl scale deployment usercrud-deployment --replicas=5

# Update image
kubectl set image deployment/usercrud-deployment usercrud=usercrud-app:v2.0

# Rollout
kubectl rollout status deployment usercrud-deployment       # check status
kubectl rollout history deployment usercrud-deployment      # version history
kubectl rollout undo deployment usercrud-deployment         # rollback to previous
kubectl rollout undo deployment usercrud-deployment --to-revision=2

# Logs
kubectl logs -l app=usercrud --tail=100   # logs from all matching pods

# Delete
kubectl delete deployment usercrud-deployment
```

---

## 🎬 Self-Healing in Action

```
1. Deployment created with replicas: 3
   K8s: "OK 3 pods banata"
   ↓
   Pod1 ✓ Pod2 ✓ Pod3 ✓
   
2. Pod2 crashes (memory issue)
   ↓
   Pod1 ✓ Pod2 💀 Pod3 ✓
   
3. Deployment ke ReplicaSet ne dekha
   "Desired: 3, Actual: 2 → MISMATCH!"
   ↓
   Auto-create naya pod
   ↓
   Pod1 ✓ Pod4 ✓ Pod3 ✓        ← Pod4 = naya replacement
   
   = Self-healing automatic ✅
```

---

## 🚀 Rolling Update (zero downtime deploy)

```
Initial state: 3 pods running v1.0
   [v1] [v1] [v1]
   
kubectl set image deployment/usercrud-deployment usercrud=usercrud-app:v2.0
   ↓
K8s rolling update strategy:
   
Step 1: New pod with v2 starts
   [v1] [v1] [v1] [v2 starting...]
   
Step 2: v2 healthy, kill 1 v1
   [v1] [v1] [v2 ✓]
   
Step 3: New v2 starts
   [v1] [v1] [v2 ✓] [v2 starting...]
   
Step 4: Kill another v1
   [v1] [v2 ✓] [v2 ✓]
   
Step 5: Continue...
   [v2 ✓] [v2 ✓] [v2 ✓]
   
   = Zero downtime, gradual replacement ✅
```

---

## 🚀 Rollback (instant safety net)

```
Production deploy v2 → bug discovered (errors spike)
   ↓
kubectl rollout undo deployment usercrud-deployment
   ↓
K8s instantly reverts to ReplicaSet of v1
   ↓
v1 pods come back, v2 pods deleted
   ↓
Users see no disruption ✅

K8s maintains DEPLOYMENT HISTORY:
   Revision 1: v1.0 (original)
   Revision 2: v2.0 (current — buggy)
   
   Rollback to revision 1 in seconds.
```

---

## 🎯 Update Strategies

```yaml
spec:
  strategy:
    type: RollingUpdate          # default
    rollingUpdate:
      maxSurge: 1                 # max EXTRA pods during update (above replicas)
      maxUnavailable: 1           # max pods DOWN during update
```

```
Strategy types:
   RollingUpdate (default)  → gradual replace, zero downtime
   Recreate                  → kill all old, then create new (DOWNTIME)
                                Use only if app can't run dual versions
```

---

## 🆚 Pod vs Deployment

| | **Pod (alone)** | **Deployment** |
|---|---|---|
| Self-healing | ❌ Manual | ✅ Auto |
| Replicas | 1 | Configurable (1-N) |
| Update | Recreate | Rolling |
| Rollback | ❌ | ✅ |
| Scale | Manual | `kubectl scale` |
| Production use | ❌ Never alone | ✅ Standard |
| YAML kind | `Pod` | `Deployment` |

---

## 🎤 Quick Interview Sense

**Q: "Deployment kya?"**

> *"K8s resource jo Pods ko manage karta declaratively. Desired state spec — kitne replicas chahiye, kaunsi image. Self-healing (pod crash → auto-replace via ReplicaSet), rolling updates (zero downtime), rollback support. Production K8s mein direct pod NEVER deploy — always Deployment se."*

**Q: "Rolling update kaise?"**

> *"Deployment image change pe gradual replacement. maxSurge (extra pods during update) + maxUnavailable (max down) controls. Step 1: naya pod start, Step 2: old pod kill — repeat until all replaced. Zero downtime guaranteed."*

**Q: "Rollback?"**

> *"`kubectl rollout undo deployment X`. K8s deployment history maintain karta — previous versions ke ReplicaSets retain. Bad deploy detected? 1 command rollback. Production safety net."*

**Q: "Deployment vs ReplicaSet vs Pod?"**

> *"Hierarchy: Deployment > ReplicaSet > Pod. Deployment manages multiple ReplicaSets (versions). Each ReplicaSet manages N pods of one image version. Pods do actual work. Deployment = high-level (you write), ReplicaSet = mid-level (auto), Pod = execution (auto-created)."*

---

## ⚠️ Common Traps

```
🪤 Trap 1: Deploying raw pods in production
         ❌ kubectl apply -f pod.yaml directly
         ✅ ALWAYS use Deployment

🪤 Trap 2: Forgetting selector match
         ❌ Selector labels ≠ pod template labels
         → Deployment can't manage pods
         ✅ Match exactly:
            selector.matchLabels.app: usercrud
            template.metadata.labels.app: usercrud

🪤 Trap 3: Not setting resource limits
         ❌ No requests/limits → pod can starve cluster
         ✅ Always set resources.requests + limits

🪤 Trap 4: Recreate strategy in production
         ❌ strategy.type: Recreate (causes DOWNTIME)
         ✅ RollingUpdate (default, zero-downtime)

🪤 Trap 5: Replicas: 1 in production
         ❌ Single pod = single point of failure
         ✅ Min 2-3 replicas for HA
```

---

## 💎 Power Phrase

> *"Deployment = K8s resource managing Pods declaratively. Desired state spec (replicas + pod template). Self-healing — pod crash → ReplicaSet creates new. Rolling updates — image change pe gradual pod replacement, zero downtime. Rollback via `kubectl rollout undo`. Hierarchy: Deployment > ReplicaSet > Pod. Production: NEVER deploy raw pods — always via Deployment."*

---

## 🧠 Memory Hook

```
Deployment = "Property manager"
   • Hamesha N apartments occupied rakhe (replicas)
   • Tenant nikla? naya laao auto (self-healing)
   • Renovation? gradual building (rolling update)
   • Bad renovation? undo command (rollback)
   
Hierarchy:
   Deployment → ReplicaSet → Pods
   (high-level → mid-level → execution)

Production rule:
   Pod alone = NEVER (ephemeral)
   Always Deployment (resilient + scalable)
   
Update strategy:
   RollingUpdate (default, prod) — gradual, zero downtime
   Recreate (rare) — kill all then create (downtime)
```

---

## ✅ Concept Locked

```
✅ Deployment = production Pod management
✅ Desired state declarative (replicas + template)
✅ Self-healing via ReplicaSet
✅ Rolling updates (maxSurge + maxUnavailable)
✅ Rollback via rollout undo
✅ kubectl apply / scale / set image / rollout
✅ Hierarchy: Deployment > ReplicaSet > Pod
✅ Resource limits (requests + limits)
✅ Production: ALWAYS Deployment, NEVER raw Pod
```

📚 [← Back to README](00_README.md) | [← Pod](02_pod.md)
