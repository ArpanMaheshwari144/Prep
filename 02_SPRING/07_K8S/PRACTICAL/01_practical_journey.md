# K8s Practical Journey — Real Deployment Step By Step

> **Date:** 2026-05-07
> **What:** Real K8s deployment of UserCRUD (Spring Boot + MySQL)
> **Mode:** Hands-on, every step explained

[← Back to K8S README](../00_README.md)

---

## Overall Journey (6 Phases)

```
PHASE 1: Cluster Setup                     ← yahan hai
PHASE 2: MySQL Deploy (4 manifests)        ← yahan hai
PHASE 3: Image Visibility Problem + Fix    ← coming
PHASE 4: Spring Boot Deploy (3 manifests)  ← coming
PHASE 5: External Access (Port-forward)    ← coming
PHASE 6: Cleanup                           ← coming
```

---

# First — kubectl Kya Hai?

```
kubectl = "kube" (Kubernetes) + "ctl" (control)
        = "Kubernetes Control"

Pronunciation: "kube-control" (kube-cuddle bhi bolte log)
```

## What kubectl IS:

```
COMMAND-LINE CLIENT for Kubernetes
   • Tera laptop pe ek binary
   • Cluster ke API Server se baat karta
   • Saare K8s commands isi se hote
```

## Visual:

```
   YOUR LAPTOP                    K8S CLUSTER
   ┌──────────────┐              ┌──────────────────┐
   │              │   commands   │                   │
   │  kubectl     │ ──────────► │  API Server       │
   │  (CLI)       │              │  (cluster ka      │
   │              │              │   entry point)    │
   │              │   responses  │                   │
   │              │ ◄────────────│                   │
   └──────────────┘              └──────────────────┘
```

## Analogy:

```
TV remote = kubectl
TV       = K8s cluster (API Server = receiver)

Tu remote pe button dabaata → TV channel change
Tu kubectl command chalata → cluster state change
```

## Same Pattern in Other Tools:

```
mysql       client → MySQL server
docker      CLI    → Docker daemon
kubectl     CLI    → K8s API Server

Sab tools mein: CLI ↔ Server pattern
```

## How kubectl Knows WHICH Cluster:

```
kubectl looks at: ~/.kube/config
   (Windows: C:\Users\<You>\.kube\config)

Yeh file mein stored:
   • Cluster server URL
   • Credentials (cert/token)
   • Default namespace

Docker Desktop ne yeh auto-setup kiya jab cluster create kiya
```

## Memory Hook:

```
kubectl = "kube CONTROL"
        = remote control for cluster
        = laptop CLI ↔ cluster API Server bridge
```

---

# PHASE 1 — Cluster Setup

## What We Did:

```
1. Docker Desktop khol
2. Settings (gear icon) → Kubernetes
3. Click "Create Kubernetes Cluster"
4. Cluster Type chuna: kind (1 node, version 1.34.3)
5. Click Create
6. Wait 2-5 min (control-plane Ready)
7. Verify with kubectl
```

---

## Why Each Step?

### **Steps 1-3: Enable K8s in Docker Desktop**
```
Why?
   • K8s cluster chahiye deploy karne ke liye
   • Docker Desktop locally K8s provide karta (no AWS needed)
   • Free, light option for learning
```

### **Step 4: kind, 1 node, v1.34.3**
```
Why kind?
   • "Kubernetes IN Docker" — cluster runs as Docker container
   • Lightest local option
   • Production K8s ka close clone

Why 1 node?
   • Tera laptop heavy load nahi le sakta
   • Learning ke liye 1 node enough
   • Production = 3+ nodes (HA), but local me 1 OK

Why v1.34.3?
   • Latest stable
   • Auto-selected by Docker Desktop
```

### **Step 6: Wait for Ready**
```
Cluster startup mein:
   • Pull base images (kindest/node)
   • Start API Server
   • Start Scheduler, Controller, etcd
   • Initialize networking
   = 2-5 min total
```

### **Step 7: Verify**
```cmd
kubectl version            ← client + server connection?
kubectl get nodes          ← node Ready?
```

---

## Verification Output

```cmd
> kubectl version
Client Version: v1.34.1
Server Version: v1.34.3
   ↑ Both versions visible = cluster reachable

> kubectl get nodes
NAME                     STATUS   ROLES           AGE
desktop-control-plane    Ready    control-plane   2m
   ↑ Status: Ready = healthy cluster
```

---

## Behind The Scenes

```
Click "Create Cluster"
   ↓
Docker Desktop:
   1. Pulls "kindest/node:v1.34.3" image
   2. Creates Docker container "desktop-control-plane"
   3. Inside container, K8s control plane starts:
      • API Server (entry point)
      • Scheduler (decides pod placement)
      • Controller Manager (state reconciliation)
      • etcd (cluster's database)
   4. CNI networking initialized
   5. ~2-5 min later → ready
   ↓
kubectl on host CLI:
   • Talks to API Server inside container
   • You can deploy resources
```

---

## Key Concept (Phase 1)

```
Cluster = NOT just one thing
        = Control plane (brain) + Worker nodes (execution)

Local kind cluster = 1 container running BOTH
   (control plane + worker on same node — for learning)

Production = control plane SEPARATE from worker nodes (HA)
```

---

## Phase 1 Complete When:

```
Docker Desktop K8s enabled
kubectl version shows client + server
kubectl get nodes shows Ready
   = Cluster ready to deploy resources
```

---

# PHASE 2 — MySQL Deploy (4 Manifests)

## What We Did:

```
1. Folder banaya: usercrud/k8s/
2. 4 manifest files banaye:
      • mysql-secret.yaml      (root password)
      • mysql-pvc.yaml         (1GB storage)
      • mysql-deployment.yaml  (MySQL pod)
      • mysql-service.yaml     (network access)
3. Each apply: kubectl apply -f <file>
4. Verify: kubectl get pods/pvc/services
```

---

## Why 4 Files (not 1)?

```
Production pattern = SEPARATION OF CONCERNS:

   Secret      → sensitive data (password)
   PVC         → storage decision (separate from pod)
   Deployment  → pod template (replicas, image, env)
   Service     → network exposure (how to reach pod)

= Each file ONE responsibility
= Easy to update individually
= Real production every K8s app yeh follow karta
```

---

## File 1: mysql-secret.yaml

### Why?
```
Password = sensitive data
   • Hardcode YAML mein = security risk
   • Production: Vault/AWS Secrets Manager
   • Local learning: K8s Secret OK (base64 encoded)
```

### Manifest:
```yaml
apiVersion: v1
kind: Secret
metadata:
  name: mysql-secret           ← reference name
type: Opaque                    ← generic secret
stringData:                     ← plain text (auto base64)
  MYSQL_ROOT_PASSWORD: rootpass
```

### Apply:
```cmd
kubectl apply -f mysql-secret.yaml
   → "secret/mysql-secret created"
```

---

## File 2: mysql-pvc.yaml (Persistent Volume Claim)

### Why?
```
Container ephemeral hota — pod delete = data gone
PVC = "K8s, mujhe persistent storage chahiye"

Bird-eye view:
   PVC ≈ Docker Volume (same purpose — persistent data)

Under the hood:
   PVC (Claim — request)
        ↓
   PV (Persistent Volume — actual storage)
        ↓
   StorageClass (defines storage type)

Why K8s has 3 layers vs Docker's 1?
   Docker = 1 host, simple direct volumes
   K8s = many nodes, many clouds (AWS EBS / GCP disk / NFS)
       = need flexibility through layered abstraction
```

### Manifest:
```yaml
kind: PersistentVolumeClaim
metadata:
  name: mysql-pvc
spec:
  accessModes:
    - ReadWriteOnce             ← single pod read/write
  resources:
    requests:
      storage: 1Gi               ← 1 GB chahiye
```

### Apply:
```cmd
kubectl apply -f mysql-pvc.yaml
   → "persistentvolumeclaim/mysql-pvc created"

Verify:
kubectl get pvc
   → mysql-pvc Bound 1Gi
              ↑
              Bound = K8s ne actual storage allocate kiya
```

---

## File 3: mysql-deployment.yaml

### Why?
```
Deployment = Pod manage karta:
   • Replicas maintain (1 pod always)
   • Self-healing (crash → auto-replace)
   • Image, env, ports, volumes ka template
```

### Manifest:
```yaml
kind: Deployment
metadata:
  name: mysql
spec:
  replicas: 1                              ← 1 pod

  template:                                ← Pod ka template
    spec:
      containers:
      - name: mysql
        image: mysql:8                     ← Docker Hub se pull
        env:
        - name: MYSQL_ROOT_PASSWORD
          valueFrom:
            secretKeyRef:                  ← Secret se password
              name: mysql-secret
              key: MYSQL_ROOT_PASSWORD
        - name: MYSQL_DATABASE
          value: userdb                    ← auto-create userdb
        ports:
        - containerPort: 3306
        volumeMounts:
        - name: mysql-storage
          mountPath: /var/lib/mysql        ← MySQL data path

      volumes:
      - name: mysql-storage
        persistentVolumeClaim:
          claimName: mysql-pvc             ← PVC reference
```

### Apply:
```cmd
kubectl apply -f mysql-deployment.yaml
   → "deployment.apps/mysql created"
```

---

## File 4: mysql-service.yaml

### Why?
```
Pod IP ephemeral (change ho sakti)
Service = stable IP + DNS name
   • "mysql:3306" se reach kar sakte (DNS)
   • Same as docker-compose service name
```

### Manifest:
```yaml
kind: Service
metadata:
  name: mysql                       ← yeh DNS hostname banta!
spec:
  type: ClusterIP                   ← internal only (default)
  selector:
    app: mysql                      ← label match wale pods
  ports:
  - port: 3306                      ← service port
    targetPort: 3306                ← pod port
```

### Apply:
```cmd
kubectl apply -f mysql-service.yaml
   → "service/mysql created"
```

---

## All 4 Applied — Verify

```cmd
kubectl get pods
   → mysql-6d95cffffc8-vpkfm   1/1   Running
                                ↑
                              READY (1 of 1 containers up)

kubectl get pvc
   → mysql-pvc   Bound   1Gi

kubectl get services
   → mysql   ClusterIP   10.96.167.147   3306/TCP
                          ↑
                       service IP (stable)
```

---

## Connection Chain Inside Cluster

```
Other K8s pods (Spring Boot eventually):
   ↓ "mysql:3306" (service NAME — DNS resolves)
   ↓
Service (mysql) — selector finds pod
   ↓ load balances if multi-pod
   ↓
Pod (mysql-xxxx)
   ↓ container port 3306
   ↓
MySQL process inside pod
   ↓ /var/lib/mysql data path
   ↓
PVC (mysql-pvc) → PV → actual disk (data persistent)
```

---

## Concept Lock (Phase 2)

```
4 files = 4 layers:
   Secret      → credentials
   PVC         → storage
   Deployment  → compute (pod template)
   Service     → network

Each layer independent.
Update one without affecting others.
= Production architecture pattern.
```

---

## Docker vs K8s Storage Quick Compare

```
DOCKER:
   docker volume create mysql_data
   docker run -v mysql_data:/var/lib/mysql ...
   = ek hi step (volume + mount)

K8S:
   PVC (Persistent Volume Claim)  → "request for storage"
        ↓
   PV  (Persistent Volume)         → "actual storage"
        ↓
   Pod uses PVC → which binds to PV → which is actual disk
   = 3 layers (more separation)
```

```
For YOUR mental model:
   PVC ≈ Docker Volume (bird-eye view)
   PV under the hood (K8s plumbing)
```

---

---

# PHASE 3 — Image Visibility Problem + Docker Hub Fix

## The Problem

```
Built local image: usercrud-app:multi (multi-stage)
Tried K8s deploy with:
   image: usercrud-app:multi
   imagePullPolicy: Never (don't try registry)

Result: ErrImageNeverPull
```

```cmd
kubectl get pods
   → usercrud-779986b6d7-8mtb4   0/1   ErrImageNeverPull
```

---

## Why It Failed

```
Tera laptop pe 2 separate worlds:

   1. DOCKER DAEMON (Docker images)
      "docker images" yahan dikhata
      "usercrud-app:multi" YAHAN hai

   2. K8S CLUSTER's CONTAINERD (separate runtime)
      K8s containers run karta isi se
      "usercrud-app:multi" YAHAN NAHI

Docker Desktop's K8s = isolated container runtime
   ≠ same as Docker daemon's image store

= K8s nahi dekh paaya tera local image
```

---

## Tried 3 Approaches

### Attempt 1: Install kind CLI (didn't help)
```
Idea: "kind load docker-image" se Docker → kind cluster transfer

Steps:
   1. Downloaded kind v0.24.0 to C:\Users\ArpanMaheshwari\Tools\
   2. Added Tools folder to PATH
   3. Ran "kind get clusters"

Result:
   "No kind clusters found"
   ↓
   Docker Desktop's K8s = managed internally
   = Standalone kind CLI doesn't see it
```

### Attempt 2: Rebuild image (didn't help)
```
Idea: Rebuild lands in containerd?
Ran: docker build -t usercrud-app:multi -f Dockerfile.multistage .

Result:
   Still ErrImageNeverPull
   = Docker daemon and K8s containerd separate
```

### Attempt 3: Docker Hub Push WORKED

```
Production-realistic path:
   • Image push to public registry (Docker Hub)
   • K8s pulls from registry (regular flow)
   • Same as how PRODUCTION works (AWS ECR, etc.)
```

---

## The Working Solution

### Step 1: Login
```cmd
docker login
   Username: arpanmah
   Password: <Docker Hub password>
   → Login Succeeded
```

### Step 2: Tag image with namespace
```cmd
docker tag usercrud-app:multi arpanmah/usercrud-app:multi
                              ↑↑↑↑↑↑↑↑
                              Docker Hub username/repo:tag

Why?
   Docker Hub mein "namespace" = username
   Push ke liye "username/repo:tag" format mandatory
```

### Step 3: Push
```cmd
docker push arpanmah/usercrud-app:multi
   → Layers pushed
   → multi: digest: sha256:xxx size: 856
   = Image ab Docker Hub pe public available
```

---

## Update Deployment YAML

### Before (failed):
```yaml
containers:
- name: usercrud
  image: usercrud-app:multi      # ← local only
  imagePullPolicy: Never          # ← don't pull from registry
```

### After (working):
```yaml
containers:
- name: usercrud
  image: arpanmah/usercrud-app:multi   # ← Docker Hub image
  imagePullPolicy: IfNotPresent         # ← pull if not local
```

### Re-apply:
```cmd
kubectl apply -f usercrud-deployment.yaml
   → "deployment.apps/usercrud configured"

kubectl get pods
   → usercrud-7fb44b4c67-wgk87   1/1   Running
```

---

## Key Insight

```
Image Docker Hub pe push karo
     ↓
K8s ko bola "yeh image use karo" (deployment YAML mein)
     ↓
K8s Docker Hub se pull karega
     ↓
Pod chala deta
     ↓
Boom — kaam khatam
```

**EVERYWHERE same pattern:**

```
Local kind cluster   → pulls from Docker Hub
AWS EKS              → pulls from AWS ECR / Docker Hub
GCP GKE              → pulls from GCR / Docker Hub
Azure AKS            → pulls from ACR / Docker Hub

K8s NEVER sees laptop's Docker images directly.
ALWAYS via registry (Docker Hub / cloud registry).
```

---

## imagePullPolicy Options

```
Never           → Don't pull, use local only
                  Use case: development with local images
                  Failed for us — K8s couldn't see Docker daemon images

IfNotPresent    → Pull only if not cached locally (default for tagged)
                  Use case: production — pull once, cache
                  Worked for us

Always          → Pull every time (default for "latest" tag)
                  Use case: dev with same tag updates
```

---

## Production Registry Flow

```
DEVELOPER:
   docker build → tag → push to registry
                       (Docker Hub / AWS ECR / GCR / GitHub Container Registry)

K8S CLUSTER:
   ↓ kubectl apply (deployment YAML with registry image)
   ↓ K8s tells node "pull arpanmah/usercrud-app:multi"
   ↓ Node's container runtime pulls image
   ↓ Cache locally
   ↓ Run container

= Same pattern works EVERYWHERE
   Local kind cluster, AWS EKS, GCP GKE — all same
```

---

## Phase 3 Done When:

```
Image pushed to Docker Hub (arpanmah/usercrud-app:multi)
Deployment YAML updated (image source + imagePullPolicy)
Pod successfully Running
Production registry pattern understood
```

---

---

# PHASE 4 — Spring Boot Deploy (3 Manifests)

## What We Did

```
3 manifest files banaye for Spring Boot:

   • usercrud-configmap.yaml    (env vars — non-sensitive config)
   • usercrud-deployment.yaml   (Spring Boot pod, Docker Hub image)
   • usercrud-service.yaml      (network access, NodePort)

Apply + verify:
   kubectl apply -f <each>
   kubectl get pods
```

---

## File 1: usercrud-configmap.yaml

### Why?
```
Spring Boot ko env vars chahiye:
   • SPRING_PROFILES_ACTIVE=compose
     (mysql:3306 service-name use karega)
   • JAVA_OPTS=-Xms256m -Xmx512m
     (JVM memory tuning)

ConfigMap = K8s ka "non-sensitive config injection"
         = Same pattern as docker-compose environment block
```

### Manifest:
```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: usercrud-config
data:
  SPRING_PROFILES_ACTIVE: compose
  JAVA_OPTS: "-Xms256m -Xmx512m"
```

### Apply:
```cmd
kubectl apply -f usercrud-configmap.yaml
   → "configmap/usercrud-config created"
```

---

## File 2: usercrud-deployment.yaml

### Why?
```
Same as MySQL Deployment but for Spring Boot:
   • 1 replica
   • Image from Docker Hub (arpanmah/usercrud-app:multi)
   • Reads ConfigMap for env vars
   • Health checks (readiness + liveness probes)
```

### Key parts:
```yaml
kind: Deployment
metadata:
  name: usercrud
spec:
  replicas: 1

  template:
    spec:
      containers:
      - name: usercrud
        image: arpanmah/usercrud-app:multi   ← Docker Hub
        imagePullPolicy: IfNotPresent

        ports:
        - containerPort: 8080

        envFrom:
        - configMapRef:
            name: usercrud-config           ← all ConfigMap vars as env

        readinessProbe:                     ← "ready to serve traffic?"
          tcpSocket:
            port: 8080
          initialDelaySeconds: 30           ← Spring Boot ~30s start

        livenessProbe:                      ← "still alive?"
          tcpSocket:
            port: 8080
          initialDelaySeconds: 60
```

### Apply:
```cmd
kubectl apply -f usercrud-deployment.yaml
   → "deployment.apps/usercrud created"
```

---

## File 3: usercrud-service.yaml

### Why?
```
Pod ke andar Spring Boot 8080 pe listening
External access chahiye → Service banao

Type: NodePort
   • External access via fixed port (30000-32767 range)
   • 30080 chuna
```

### Manifest:
```yaml
kind: Service
metadata:
  name: usercrud-service
spec:
  type: NodePort                    ← external access
  selector:
    app: usercrud                   ← match pod label
  ports:
  - port: 8080                      ← service port
    targetPort: 8080                ← pod port
    nodePort: 30080                 ← external port
```

### Apply:
```cmd
kubectl apply -f usercrud-service.yaml
   → "service/usercrud-service created"
```

---

## Verify Pods Running

```cmd
kubectl get pods
   → mysql-6d95cffffc8-vpkfm        1/1   Running   35m
   → usercrud-7fb44b4c67-wgk87      1/1   Running   2m54s
                                     ↑     ↑
                                  READY  STATUS
```

---

## What Spring Boot Does Inside Pod (startup)

```
1. Container starts
2. Spring Boot ENTRYPOINT: java -jar app.jar
3. Reads ENV var SPRING_PROFILES_ACTIVE=compose
4. Loads application-compose.properties
5. spring.datasource.url = jdbc:mysql://mysql:3306/userdb
                                        ↑↑↑↑↑
                                        Service NAME (DNS)
6. K8s DNS resolves "mysql" → mysql-service IP → mysql pod
7. Connection established
8. Hibernate creates tables / verifies
9. Tomcat started on 8080
10. Pod becomes Ready (1/1)

= Same chain as docker-compose, in K8s
```

---

## Docker Compose vs K8s — Same Pattern

```
COMPOSE:
   docker-compose.yml = mysql service + app service + env vars
   → docker-compose up
   = 1 command for full stack

K8S:
   3-4 YAML files per service:
      Deployment + Service + ConfigMap + Secret
   → kubectl apply -f .
   = More verbose but more flexible

= Same concept (multi-container app), different syntax
```

---

## Phase 4 Done When:

```
ConfigMap applied
Deployment applied (image from Docker Hub)
Service applied (NodePort 30080)
usercrud pod 1/1 Running
MySQL pod 1/1 Running
   = Both services up, talking via service names
```

---

---

# PHASE 5 — External Access (Port-forward fix)

## The Problem

```
Service banayi NodePort type (30080)
Soch tha: localhost:30080 se reach kar lega

Postman se:
   POST http://localhost:30080/auth/register

Result: ECONNREFUSED 127.0.0.1:30080
```

---

## Why It Failed

```
Docker Desktop ka kind cluster = isolated container network

   Tera laptop                    Docker Desktop kind cluster
   ┌──────────────┐              ┌──────────────────────┐
   │              │              │                       │
   │  localhost   │              │  NodePort 30080       │
   │  :30080      │              │  (cluster ke andar    │
   │              │   REFUSED  │   exposed)             │
   │              │              │                       │
   └──────────────┘              └──────────────────────┘

Kind cluster ka network = Docker container ke andar
Tera laptop ka localhost ≠ kind cluster ka network
   = Direct reach NAHI
```

---

## The Solution — Port-Forward

```cmd
kubectl port-forward service/usercrud-service 8080:8080
                                              ↑↑↑↑ ↑↑↑↑
                                              local→service
```

### Output:
```
Forwarding from 127.0.0.1:8080 -> 8080
   = Bridge banaya:
     localhost:8080  ↔  K8s service:8080
```

### Important:
```
Yeh terminal BLOCK ho jaata
   = Port-forward chal raha as long as terminal open
   = Terminal close = forward gone
   = For dev/testing only
```

---

## Visual

```
   POSTMAN
       │
       │ POST http://localhost:8080/auth/register
       ▼
   ┌──────────────────┐
   │  kubectl         │
   │  port-forward    │  ← bridge process
   │  (terminal)      │
   └────────┬─────────┘
            │
            │ tunnel into cluster
            ▼
   ┌──────────────────────────────────┐
   │  K8S CLUSTER                      │
   │                                    │
   │  Service (usercrud-service)       │
   │      │ port 8080                   │
   │      ▼                             │
   │  Pod (usercrud-7fb44b4c67-wgk87)  │
   │      │ container 8080               │
   │      ▼                             │
   │  Spring Boot                       │
   │      │ JDBC                         │
   │      ▼                             │
   │  Service (mysql)                   │
   │      │                             │
   │      ▼                             │
   │  MySQL Pod                          │
   └──────────────────────────────────┘
```

---

## Postman Test Result

```
URL:  http://localhost:8080/auth/login
Body:
{
  "email": "arpan@test.com",
  "password": "test123"
}
```

```
Status: 200 OK (2.17s)
accessToken: "eyJhbGci..."
refreshToken: "92f8d44f-2476-4ab2-..."

= End-to-end K8s deployment WORKING
```

---

# IMPORTANT — K8s Mein 3 ALAG Ports Hote Hain

```
1. containerPort  → Pod ke andar (Spring Boot 8080 pe listen)
2. port           → Service ka internal port
3. nodePort       → External access port (30000-32767 ONLY)
```

## Visual

```
   EXTERNAL                    INTERNAL CLUSTER
   ┌─────────┐                 ┌─────────────────────┐
   │ Browser │                 │                      │
   │ Postman │ ─→ nodePort ─→  │ Service ─→ Pod      │
   │         │   30080         │ port 8080  container │
   │         │   (external)    │            Port 8080 │
   └─────────┘                 │                      │
                                │ (Spring Boot listens │
                                │  inside pod)          │
                                └─────────────────────┘
```

---

## NodePort 30080 KYU? Direct 8080 KYU NAHI?

```
K8s rule (security):
   nodePort MUST be in range 30000–32767

   YE FIXED RANGE HAI K8s ka.
   8080 = OUTSIDE this range
   = K8s rejects, deploy fail
```

---

## Why 30000-32767 Range?

```
Port number ranges (worldwide standard):
   0       - 1023     = SYSTEM ports (need root — SSH 22, HTTP 80)
   1024    - 49151    = REGISTERED ports (apps — DB 3306, web 8080)
   49152   - 65535    = EPHEMERAL ports (client connections, OS auto)

K8s NodePort range:
   30000   - 32767    = SAFE zone — doesn't conflict

Why?
   Far above system ports (no root needed)
   Out of way of common apps (8080, 3306, etc.)
   Won't clash with ephemeral ports
   = K8s ka cluster-friendly range
```

---

## Same Concept, Different Layer

```
Spring Boot inside pod:
   containerPort: 8080      ← Spring Boot ka choice
                             (koi bhi port chuna ja sakta)

Service (cluster ke andar):
   port: 8080               ← Service ka internal port
   targetPort: 8080         ← forward to pod port
                             (must match containerPort)

External access (NodePort):
   nodePort: 30080          ← RESTRICTED 30000-32767
```

---

## If You Try 8080 as nodePort:

```yaml
spec:
  type: NodePort
  ports:
  - nodePort: 8080    ← K8s rejects this
```

**K8s error:** `provided port 8080 is not in the valid range. The range of valid ports is 30000-32767`

---

## So Why Did Postman Hit 8080?

```
We used:
   POST http://localhost:8080/auth/register
                   ↑
                   port-forward ka port

Yeh nodePort NAHI tha — yeh:
   kubectl port-forward service/usercrud-service 8080:8080
                                                  ↑↑↑↑↑
                                                  local:cluster

   = port-forward ne BRIDGE banaya
   = Localhost:8080 → Service:8080 (internal)
   = NodePort 30080 ko bypass kiya

= Different mechanism (port-forward vs NodePort)
= Port-forward is RESTRICTED nahi (any port)
= Used local 8080 for convenience
```

---

## Three Access Methods Compared

```
Method 1: NodePort
   localhost:30080 → cluster node → service → pod
   Port limit: 30000-32767
   Docker Desktop kind = doesn't expose to localhost (issue we hit)

Method 2: Port-Forward (jo kiya)
   localhost:<any-port> → kubectl tunnel → service → pod
   Any port (no restriction)
   Works on any cluster including kind
   Terminal-bound (not production)

Method 3: LoadBalancer (production)
   Public-IP:80 → cloud LB → service → pod
   Production-grade
   Any port (cloud LB manages)
   Costs money, cloud-only

Method 4: Ingress (production — best)
   Single LB for many services
   Path/host-based routing
   TLS termination
   Cost-efficient
```

---

## Bonus — "Email already registered" Pe Logged In

```
We expected: fresh DB, register naya
What happened: old data already there

Reason:
   Docker Desktop's K8s shared storage backend
   = mysql_data Docker volume reused for K8s PV
   = Tera Konkmd se purana Arpan zinda mil gaya

= Storage abstraction layers in K8s (interesting bonus)
```

---

## Memory Hook

```
Pod port      = Spring Boot ka choice (8080)
Service port  = Service ke andar (usually same)
NodePort      = External access — RESTRICTED 30000-32767
   • 30080 = "30000-range mein 080" trick
   • Easy to remember pattern

Port-forward  = bypass nodePort
              = any local port works
              = dev/debug magic
```

---

## Phase 5 Done When:

```
kubectl port-forward bridge active
Postman test successful (200 OK)
End-to-end chain proven
Understood: NodePort restricted to 30000-32767
Understood: port-forward bypass = any port
Understood: Production = LoadBalancer + Ingress
```

---

---

# PHASE 6 — Cleanup

## Why Cleanup?

```
Cluster + pods running = laptop heat + RAM use
Practical done = resources free karo
```

---

## Cleanup Methods

### **Option 1: Delete K8s Resources Only (cluster runs)**
```cmd
kubectl delete deployment usercrud
kubectl delete service usercrud-service
kubectl delete configmap usercrud-config

kubectl delete deployment mysql
kubectl delete service mysql
kubectl delete pvc mysql-pvc
kubectl delete secret mysql-secret

= Deployments + pods gone, cluster still up
```

### **Option 2: Delete Cluster (Docker Desktop UI)** ← jo TUNE kiya
```
Docker Desktop → Kubernetes → "Reset cluster"
   ↓
   Cluster + ALL resources gone
   ↓
   Laptop cool down
```

### **Option 3: Stop Docker Desktop entirely**
```
System tray → Quit Docker Desktop
   = Docker + K8s + everything off
```

---

## Kind Cluster ki Nature (jo confuse ki thi)

```
Docker Desktop ke 2 K8s options:

   1. kind (jo tune chuna)         → cluster = container
                                       Stop/Reset = DELETE container
                                       Cluster gone

   2. Built-in K8s (older option)  → cluster persistent
                                       Stop = pause, data safe
                                       Restart se wapas same state
```

---

## Iska Matlab:

```
Tera laptop COOL ho gaya (resources free)
Tera K8s deployment GAYA:
   • mysql pod
   • usercrud pod
   • All manifests applied state
   • PVC data
```

```
Lekin BADA LOSS NAHI:
   Files (YAMLs) abhi bhi tere paas
   Docker Hub image preserved (arpanmah/usercrud-app:multi)
   Notes safe

Future revival:
   1. Docker Desktop → Create cluster (2 min)
   2. cd k8s && kubectl apply -f .
   3. kubectl port-forward
   = Sab wapas in 5 min
```

---

## Production Reality

```
Kind clusters = development/learning only
   • Ephemeral by design
   • Heavy on local resources

Production:
   • AWS EKS — persistent cluster
   • Cluster doesn't go down on laptop close
   • Managed control plane
   • Auto-restart, auto-scale
```

---

## Phase 6 Done When:

```
Cluster deleted / stopped
Laptop resources freed
Files preserved for future
Understood: kind = ephemeral, production = persistent
```

---

# FULL JOURNEY COMPLETE

```
Phase 1: Cluster Setup
Phase 2: MySQL Deploy (4 manifests)
Phase 3: Image Visibility → Docker Hub Fix
Phase 4: Spring Boot Deploy (3 manifests)
Phase 5: External Access (port-forward)
Phase 6: Cleanup
```

---

## What Was Achieved (1 day):

```
Real K8s cluster setup (Docker Desktop kind)
Multi-resource deployment:
      • Secret, PVC, ConfigMap, Deployment, Service × 2
Docker Hub image push (production-grade flow)
End-to-end production-style stack
Port-forward for external access
Postman test succeeded (200 OK)
Real production patterns understood

= Most devs don't have this hands-on
= Interview-ready for K8s questions
```

---

## Quick Recall (Single-Page Reference)

```
SETUP:
   Docker Desktop → Settings → Kubernetes → Create cluster

VERIFY:
   kubectl version       (client + server)
   kubectl get nodes     (Ready status)

MYSQL DEPLOY (4 files):
   mysql-secret.yaml      → password
   mysql-pvc.yaml         → 1GB storage
   mysql-deployment.yaml  → pod template
   mysql-service.yaml     → ClusterIP DNS

IMAGE FLOW:
   docker login
   docker tag <local> <username>/<repo>:<tag>
   docker push <username>/<repo>:<tag>
   = Image on Docker Hub

SPRING BOOT DEPLOY (3 files):
   usercrud-configmap.yaml    → env vars (profile)
   usercrud-deployment.yaml   → pod template (Docker Hub image)
   usercrud-service.yaml      → NodePort 30080

EXTERNAL ACCESS:
   kubectl port-forward service/<svc> <local>:<service-port>

POSTMAN:
   localhost:<local-port>/api/...

CLEANUP:
   Docker Desktop → Reset cluster (or quit)
```

---

## Top Concepts Locked

```
1. kubectl = K8s ka remote control (CLI ↔ API Server)
2. PVC ≈ Docker volume (persistent storage abstraction)
3. ConfigMap = env injection (Spring profiles K8s equivalent)
4. Service = stable network endpoint (DNS resolves by name)
5. Image registry mandatory (K8s ne local Docker images nahi dekh sakta)
6. NodePort restricted 30000-32767
7. port-forward = bypass mechanism (any port)
8. kind cluster = ephemeral (Reset = delete)
9. Production = AWS EKS + LoadBalancer + Ingress
10. Same Docker Compose pattern, different syntax in K8s
```

[← Back to K8S README](../00_README.md)
