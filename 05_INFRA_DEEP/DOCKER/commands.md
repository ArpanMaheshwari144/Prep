# Docker — Complete Revision Reference


## WHAT + WHY


### What Docker is
Tool to package application + dependencies + OS bits = ek portable container.
Same package laptop, server, cloud — sab jagah identical chalega.


### Why exist
Pre-Docker era problems:
- "Works on my machine" — dev pe chalta, prod pe nahi
- Dependency conflicts (Python 3.8 vs 3.10)
- Setup hours per new server
- Environment drift (dev / staging / prod)

Docker fix:
- Container = same env everywhere
- Image build once → run anywhere
- Industry standard since 2013


### Shipping analogy
Pre-1950s: cargo loose, different sizes, breakage common
Container revolution: standard metal box, fits any ship/truck/train
= Docker = shipping container of software


## 3 CORE CONCEPTS

```
Image       = blueprint (recipe + ingredients)
Container   = running instance (cooked dish from recipe)
Dockerfile  = recipe text (instructions to build image)
```

Image is dormant. Container is alive (running).
One image can spawn many containers.


## CATEGORY 1: IMAGES


### docker --version
```
docker --version
```
- Docker installed hai ya nahi, version kya
- "Tool exists?" check
- Output: Docker version 24.x.x or 25.x.x


### docker images
```
docker images
```
- Local pe jitne images downloaded, list dikhata
- "Pantry mein kaun se ingredients hai?" check
- Shows: repository, tag, image ID, size


### docker pull
```
docker pull nginx
docker pull mysql:8
docker pull arpanmah/todoapp:latest
```
- Docker Hub se image download
- Local cache mein store
- Phir docker images mein appear


### Image naming: name:tag
```
todoapp:latest    (default if no tag)
todoapp:v1.0
todoapp:demo
nginx:1.25-alpine
```
- name = image name
- :tag = version/label (TU define kar sakta kuch bhi)
- Default tag = "latest"
- Use case: multiple versions track kar sakta


### docker build
```
docker build -t <name> .
docker build -t todoapp:demo .
```
- `.` = current directory (Dockerfile yahaan)
- `-t name` = tag image with this name
- Dockerfile padhta + image build karta


### docker rmi
```
docker rmi <image>
docker rmi nginx:latest
```
- Local pe image delete
- Use cases: disk free, purana version hatana
- WARNING: container use kar raha = error first


## CATEGORY 2: CONTAINER LIFECYCLE


### docker run (the big one)
```
docker run -d -p 6379:6379 --name redis redis
```
Breakdown:
```
docker run -d -p 6379:6379 --name redis redis
   │   │  │  │              │     │     │
   │   │  │  │              │     │     └─ image name (last arg)
   │   │  │  │              │     └─ container name
   │   │  │  │              └─ name flag
   │   │  │  └─ port: HOST:CONTAINER
   │   │  └─ publish port
   │   └─ detached (background)
   └─ Docker + sub-command
```

Use: Image se naya container banao + chalao
= "Pantry ingredient se actual dish cook"


### Port mapping HOST:CONTAINER
Container = isolated network. Without port map = unreachable.

```
-p 6379:6379   HOST 6379 → CONTAINER 6379
-p 8080:80     HOST 8080 → CONTAINER port 80 (e.g., nginx)
```

Building analogy:
- Container = apartment with internal phone (extension 6379)
- Host port = building's outside phone
- Port map = "Dial 6379 outside → ring apartment 6379"

Different ports possible — useful for parallel containers:
```
-p 3306:3306   primary MySQL
-p 3307:3306   secondary MySQL (different host port, same container)
```


### docker ps
```
docker ps         - running containers only
docker ps -a      - all (including stopped)
```
Shows: container ID, image, status, ports, names


### docker stop
```
docker stop <name>
```
Graceful shutdown (SIGTERM → SIGKILL after timeout).
Data on volumes safe.


### docker start
```
docker start <name>
```
Stopped container wapas chalao. Same state, same data.
NOT same as docker run (which creates new container).


### docker rm
```
docker rm <name>
```
Container delete (filesystem + metadata gone).
Running container = error → docker stop first.


### docker exec
```
docker exec -it <name> bash
docker exec -it redis redis-cli
docker exec -it todoapp-mysql mysql -uroot -proot
```

Breakdown:
- `-i` interactive (input se commands lo)
- `-t` terminal (shell view)
- combined = interactive terminal session
- `bash` = which command to run inside

Analogy:
- Container = closed box with app running inside
- docker exec -it = dwar khol ke ANDAR jaake terminal lo
- Andar se ls, cat, mysql, redis-cli — kuch bhi chala sakta

Use: debug, inspect, manual operation inside container


### docker logs
```
docker logs <name>            - all logs (since container start)
docker logs -f <name>         - follow live (real-time stream)
docker logs --tail 50 <name>  - last 50 lines
```

Critical debug tool.
Day 17 Redis fail + Day 20 AWS CPU starvation = both diagnosed via logs.


### Lifecycle flow
```
docker run    → naya container (image se)
docker stop   → pause (data safe)
docker start  → wapas chalao (same data)
docker rm     → permanent delete (only if stopped)
```


## CATEGORY 3: DOCKER COMPOSE


### Why compose
- `docker run` = 1 container manual
- 3+ services (app + mysql + redis) = painful
- docker-compose.yml = declarative file
- `docker compose <cmd>` = manage stack as ONE unit


### docker compose up
```
docker compose up               - foreground (logs visible)
docker compose up -d            - detached (background)
docker compose up --build       - rebuild images first
```

`--build` = useful when Dockerfile or source code changed.


### docker compose down
```
docker compose down             - stop + remove containers + network
docker compose down -v          - + volumes (DATA GONE)
```

`-v` flag = volumes also deleted. WITHOUT it, data persists.


### docker compose ps
```
docker compose ps               - stack containers status
```


### docker compose logs
```
docker compose logs             - all services combined
docker compose logs app         - specific service
docker compose logs -f          - follow live
```


### Key compose concept
```
1 container = 1 service
Multi-service app = multi-container
```

Example tera stack on EC2:
- todoapp container       (Spring app)
- todoapp-mysql container (MySQL)
- redis container         (Redis)
= 3 SEPARATE containers same EC2 pe
= compose ne network banaya = ek doosre se "service name" se baat

Building analogy:
- EC2          = building (entire server)
- Each container = isolated room
- Docker network = phone line between rooms


## CATEGORY 4: VOLUMES


### Why volumes
- Container delete = filesystem inside GONE
- Database data lose = disaster
- Volume = external storage (USB stick)
- Container delete pe data safe


### docker volume ls
```
docker volume ls
```
All volumes list (named + anonymous).


### docker volume inspect
```
docker volume inspect mysql-data
```
Details: mount path, driver, labels, attached containers.


### docker volume rm
```
docker volume rm <name>
```
CAREFUL — data permanently gone.


### docker volume prune
```
docker volume prune
```
All UNUSED volumes delete. Cleanup command.


### USB stick analogy
- Container = phone (data inside)
- Volume = SD card plugged in
- Phone factory reset = SD card untouched
- SD card delete = data gone


## CATEGORY 5: NETWORK


### Why networks
- Container = isolated network by default
- Multiple containers ek doosre se baat = same network
- Compose auto-creates network for stack


### docker network ls
```
docker network ls
```
List all networks. Default: bridge, host, none + user-created.


### docker network inspect
```
docker network inspect <name>
```
Subnet, attached containers, gateway info.


### docker network create
```
docker network create my-net
```
Custom network banao (manual link multiple containers).


### docker network rm
```
docker network rm <name>
```
Delete network.


### Service name DNS
Compose ka magic:
- App container code mein "mysql:3306" likh
- Compose auto-resolves "mysql" → MySQL container's internal IP
- = Container ke andar se service name se reach


## CATEGORY 6: CLEANUP


### docker system prune
```
docker system prune        - stopped containers + unused networks + dangling images
docker system prune -a     - aggressive (all unused images)
```


### Disk reclaim
```
docker system df           - disk usage breakdown
```


## DEBUGGING WORKFLOW (real-world)

```
1. Container not running?       docker ps -a
2. Container died fast?          docker logs <name>
3. Inside container debug?       docker exec -it <name> bash
4. Memory/CPU check?             docker stats
5. Network issue?                docker network inspect
6. Config check?                 docker inspect <container>
```


## REAL-WORLD DAYS RECAP

### Day 18 (Local Docker)
- docker compose up --build
- 3 containers: app + mysql + redis
- Race condition: app starts before MySQL ready
- Fix: healthcheck + depends_on:service_healthy

### Day 20 (AWS EC2 Docker)
- Same compose pattern on EC2
- arpanmah/todoapp image from Docker Hub (not build)
- Hit t3.micro CPU limit
- Diagnosed via docker logs (thread starvation)


## KEY TAKEAWAYS

1. Image vs Container = blueprint vs running instance
2. Port mapping HOST:CONTAINER = isolation bridge
3. 1 container = 1 service (microservices pattern)
4. Compose = orchestrate multi-container stack
5. Volume = data survives container delete
6. Network = service name DNS within stack
7. docker logs = first debugging weapon


## ANALOGIES INDEX

- Docker overall          = shipping container of software
- Image vs Container      = recipe vs cooked dish
- Port mapping            = building outside phone → apartment extension
- Container architecture  = EC2 building, containers = isolated rooms
- Volumes                 = USB stick (data survives device reset)
- Networks                = phone line between rooms
