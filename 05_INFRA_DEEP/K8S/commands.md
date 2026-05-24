# K8s — Complete Revision Reference


## WHAT + WHY


### What K8s is
Container orchestrator at scale.
Docker compose ka "bada bhai".
Hundreds of containers, multiple machines manage karta.


### Why exist
Docker compose = single laptop manage karta.
Production reality:
- 100s of containers
- 10s of machines
- Auto-restart on crash (self-healing)
- Auto-scale on load
- Rolling updates (zero downtime)
= Manual management impossible
= K8s automates all this


### Mall analogy
- Docker compose = single shop manager (3-5 employees)
- K8s = MALL MANAGER:
  - 100+ shops
  - Employee leave = naya hire turant (self-healing)
  - Crowd badhe = aur counters open (auto-scale)
  - Renovation = ek shop band, dusra chalu (rolling deploy)


### Industry standard
AWS EKS, Google GKE, Azure AKS — all run K8s underneath.
JP/GS/BlackRock expect K8s familiarity.


## 5 CORE CONCEPTS


### Cluster
Entire K8s setup (1+ nodes working together).


### Node
Ek physical/virtual machine in cluster.

Two types:
- **control-plane node** = "brain" (API server, etcd, scheduler, controller-manager)
- **worker node** = pods actually run yahaan (kubelet + kube-proxy)


### Pod
Smallest deployable unit.
1+ containers wrapped together.
Usually 1 container per pod (multi-container = rare).


### Deployment
Pod ka blueprint + replica manager.
- "5 pods chahiye, koi mara to naya banao"
- Maintains declared state


### Service
Network access layer.
Pods come and go (IPs change), Service = stable endpoint.

3 types:
- **ClusterIP** (default) = internal only
- **NodePort** = external accessible (port 30000-32767)
- **LoadBalancer** = cloud provider managed (AWS ELB)


### Hierarchy
```
Cluster
  └─ Node (control-plane)
  └─ Node (worker 1)
      └─ Pod 1
          └─ Container
      └─ Pod 2
  └─ Node (worker 2)
      └─ Pod 3
```


## 2 TOOLS


### kubectl
Universal K8s CLI.
Works on ANY cluster: kind, EKS, GKE, AKS, on-prem.
Same commands across providers.


### kind
Local cluster creator.
Cluster runs INSIDE a Docker container.
Lightweight, faster than minikube.


## KIND COMMANDS


### kind version
```
kind version
```
Output: kind v0.x.x
Check installation.


### kind create cluster
```
kind create cluster --name todoapp
   │    │       │       │     │
   │    │       │       │     └─ cluster ka naam
   │    │       │       └─ name flag
   │    │       └─ resource type
   │    └─ action
   └─ tool
```
Single-node K8s cluster spawned inside ek Docker container.
30-60 sec first time (image download).


### kind delete cluster
```
kind delete cluster --name todoapp
```
Entire cluster delete. Pods, PVCs, services — sab gone. RAM free.


## KUBECTL COMMANDS


### kubectl get nodes
```
kubectl get nodes
```
Cluster ke nodes list.
Output: NAME STATUS ROLES AGE VERSION


### kubectl cluster-info
```
kubectl cluster-info
```
Connection info — API server URL, services.


### kubectl get
```
kubectl get pods           - list pods
kubectl get svc            - list services
kubectl get deployments    - list deployments
kubectl get pvc            - persistent volume claims
kubectl get all            - everything in current namespace
```


### Shortcuts (senior signal)
```
pod        / po
service    / svc
deployment / deploy
namespace  / ns
node       / no
```


### kubectl apply
```
kubectl apply -f <file>            - apply single YAML
kubectl apply -f k8s/              - apply all YAMLs in folder
```
- YAML padhta + cluster mein resources create/update
- Declarative (file describes state, K8s achieves it)
- Re-run safe (idempotent)


### kubectl logs
```
kubectl logs <pod-name>            - all logs
kubectl logs -f <pod-name>         - follow live
kubectl logs --tail 50 <pod-name>  - last 50 lines
kubectl logs <pod> -c <container>  - specific container in multi-container pod
```
First debug tool. Same role as docker logs.


### kubectl describe
```
kubectl describe pod <name>
kubectl describe svc <name>
kubectl describe deployment <name>
```
Full details dump:
- Status, events, conditions
- Resource limits, env vars
- Recent events (Pulled image, Started, Failed)

`kubectl logs` vs `describe`:
- logs = container ke andar ki output
- describe = K8s POV se kya ho raha (scheduling, events)


### kubectl port-forward
```
kubectl port-forward svc/todoapp 8080:8080
```
Local laptop se cluster service tak tunnel.
localhost:8080 → service inside cluster.
Terminal block rehta (keep running). Ctrl+C = close.

Use cases:
- kind cluster (no public IP)
- EKS private cluster
- Debug specific pod


### kubectl exec
```
kubectl exec -it <pod> -- bash
kubectl exec -it mysql-xxx -- mysql -uroot -proot
```
- `-it` = interactive terminal
- `--` = separator (pod ke andar ka command alag)
- `bash` = which shell to launch

docker exec ka K8s version. Pod target instead of container directly.


### kubectl delete
```
kubectl delete -f <file>            - reverse apply
kubectl delete -f k8s/              - delete entire stack
kubectl delete pod <name>           - specific pod (deployment will recreate)
kubectl delete deployment <name>    - deployment + its pods
```


## YAML MANIFEST STRUCTURE


### Every K8s YAML has 4 top-level keys
```yaml
apiVersion: <version>   # API spec version
kind: <type>            # resource type
metadata:               # name, labels
  name: my-app
spec:                   # actual config
  ...
```


### Deployment YAML
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: todoapp
spec:
  replicas: 1                    # kitne pods chahiye
  selector:                      # "ye pods kaun se"
    matchLabels:
      app: todoapp
  template:                      # pod blueprint
    metadata:
      labels:
        app: todoapp             # same as selector
    spec:
      containers:
      - name: todoapp
        image: todoapp-app:latest
        ports:
        - containerPort: 8080
```

Key parts:
- `replicas` = scale (1, 3, 10)
- `selector` = which pods belong
- `template` = pod recipe
- `image` = Docker image
- `containerPort` = container's listening port


### Service YAML
```yaml
apiVersion: v1
kind: Service
metadata:
  name: todoapp                  # DNS name in cluster
spec:
  selector:
    app: todoapp                 # target these pods
  ports:
  - port: 8080                   # service-level port
    targetPort: 8080             # container's port
    nodePort: 30080              # external port (NodePort only)
  type: ClusterIP                # or NodePort, LoadBalancer
```

Port fields:
- `port` = service-level (consumers hit this)
- `targetPort` = container actual listening
- `nodePort` = external port (NodePort type, 30000-32767)


### PVC YAML
```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: mysql-pvc
spec:
  accessModes:
    - ReadWriteOnce              # ek node se mount
  resources:
    requests:
      storage: 1Gi               # 1 GB allocate
```

Access modes:
- `ReadWriteOnce` (RWO) — 1 node R/W (most common for DBs)
- `ReadOnlyMany` (ROX) — multiple nodes read
- `ReadWriteMany` (RWX) — multiple nodes R/W (rare)

Use: Pod restart = data preserved on PVC.


## SERVICE-NAME DNS


### Magic of cluster networking
Within cluster, services resolvable by name:
- App code mein "mysql:3306" likh
- K8s auto-resolves "mysql" → MySQL service IP
- = Inter-pod communication via service name


### Tera Day 19 example
- todoapp pod env: `SPRING_DATASOURCE_URL=jdbc:mysql://mysql:3306/...`
- App resolves "mysql" via cluster DNS
- Reaches mysql service → routes to MySQL pod


## DEBUGGING WORKFLOW

```
1. Pods running?               kubectl get pods
2. Pod Pending?                 kubectl describe pod <name>
3. Container crashed?            kubectl logs <pod>
4. Inside pod?                  kubectl exec -it <pod> -- bash
5. Service reachable?            kubectl port-forward + test
6. Network issue?                kubectl get endpoints
```


## DAY 19 RECAP

- kind cluster created (single-node)
- 4 YAMLs applied:
  - app-deployment.yaml (1 replica, imagePullPolicy Never)
  - app-service.yaml (NodePort 30080)
  - mysql.yaml (PVC + Deployment + Service)
  - redis.yaml (Deployment + Service)
- Port-forward used (kind doesn't expose NodePort externally)
- App restart 1-2 times before MySQL ready (race condition)
- Service-name DNS worked (mysql/redis as hostnames)


## ADVANCED (NOT COVERED — POST-BANGALORE)

- StatefulSet (better than Deployment for DBs)
- DaemonSet (one pod per node)
- Job / CronJob (batch + scheduled tasks)
- ConfigMap / Secret (config externalization)
- Ingress controller (domain-based routing)
- HPA (horizontal pod autoscaler)
- RBAC (access control)
- Helm (package manager — npm of K8s)
- Operators (custom controllers)


## KEY TAKEAWAYS

1. K8s = container orchestrator for production scale
2. Pod = smallest deployable unit (usually 1 container)
3. Deployment = pod blueprint + replica manage
4. Service = stable network endpoint
5. kubectl = universal K8s CLI
6. kind = lightweight local cluster
7. YAML = declarative state (apply = make it so)
8. Service DNS = pods talk via service names
9. kubectl logs + describe = debugging duo


## ANALOGIES INDEX

- K8s vs Docker compose       = mall manager vs single shop manager
- Pod                         = container wrapper (smallest unit)
- Deployment                  = replica manager (factory floor supervisor)
- Service                     = stable phone number (pods come/go)
- kubectl                     = universal remote control for any cluster
- kind                        = local K8s in a Docker container
