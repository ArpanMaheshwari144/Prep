# K8s Setup Journey — TODO App

## STEP 1: Verify kind installed
```
kind version
```
Output: kind v0.24.0


## STEP 2: Create local cluster
```
kind create cluster --name todoapp
```
- kind ka Docker container start
- Andar single-node K8s cluster
- API server + etcd + controller + scheduler + kubelet auto-launch
- kubectl context auto-set


## STEP 3: Verify cluster
```
kubectl get nodes
```
Output: todoapp-control-plane Ready (v1.31.0)
- Single node = control-plane + worker dono role


## STEP 4: Find existing app image
```
docker images | findstr todoapp
```
Output: todoapp-app:latest (595 MB) — built by docker compose


## STEP 5: Load image into kind cluster
```
kind load docker-image todoapp-app:latest --name todoapp
```
- Host Docker se image nikalo
- kind container ke andar push
- Without this = pod "ImagePullBackOff"


## STEP 6: Create YAML manifests in k8s/ folder
4 files banaye 07_PROJECTS/todoapp/k8s/ mein:

### app-deployment.yaml
- Kind: Deployment
- replicas: 1
- image: todoapp-app:latest
- imagePullPolicy: Never (kind se loaded)
- env vars: SPRING_DATASOURCE_URL, REDIS_HOST (use service names)

### mysql.yaml (3 resources in one file)
- PersistentVolumeClaim (1 Gi storage)
- Deployment (image mysql:8, MYSQL_DATABASE=tododb)
- Service (ClusterIP, port 3306)

### redis.yaml
- Deployment (image redis, --maxmemory 125mb, allkeys-lru)
- Service (ClusterIP, port 6379)

### app-service.yaml
- Service (NodePort, port 8080 → nodePort 30080)
- External access ke liye


## STEP 7: Apply all manifests
```
kubectl apply -f k8s/
```
Output:
- deployment.apps/todoapp created
- service/todoapp created
- persistentvolumeclaim/mysql-pvc created
- deployment.apps/mysql created
- service/mysql created
- deployment.apps/redis created
- service/redis created


## STEP 8: Verify pods running
```
kubectl get pods
```
Expected: 3 pods (todoapp, mysql, redis) all 1/1 Running
- todoapp restart 1-2 baar possible (race condition on MySQL ready)
- K8s auto-restart kar deta hai jab tak success


## NEXT STEPS (pending)
- App ka NodePort se Postman test
- kubectl logs / describe agar koi pod issue


## USEFUL COMMANDS DURING DEBUG
```
kubectl get pods                  - list all pods
kubectl get svc                   - list all services
kubectl describe pod <name>       - detailed status
kubectl logs <pod-name>           - container logs
kubectl logs -f <pod-name>        - follow logs live
kubectl exec -it <pod> -- bash    - shell inside pod
kubectl delete -f k8s/            - delete all from yaml
```


## CLEANUP COMMANDS (jab khatam)
```
kind delete cluster --name todoapp
```
- Pura cluster delete (Docker container gone)
- Volumes bhi delete (PVC data gone)
