# Section C — Quick Reference (Commands + Traps + Power Phrase)

> **Use:** Quick lookup during work or interview prep
> **Visit:** Any time after reading sections 1-6

[← Back to README](00_README.md) | [← Compose](06_docker_compose.md)

---

# COMPLETE COMMAND REFERENCE

## System / PowerShell

```powershell
# Verify Docker installation
docker --version                     # version check
docker info                          # detailed engine info

# Find services / port usage (debugging)
Get-Service | Where-Object { $_.Name -match 'MySQL' }
netstat -an | Select-String ":3306"  # who's using port 3306?

# Maven install (manual, no admin)
Invoke-WebRequest -Uri $url -OutFile $tempZip
Expand-Archive -Path $zip -DestinationPath $dir -Force
[Environment]::SetEnvironmentVariable("MAVEN_HOME", $value, "User")
mvn --version                        # verify Maven
```

---

## Docker Image Commands

```cmd
docker pull mysql:8                  # download image from Docker Hub
docker pull eclipse-temurin:17-jdk-jammy
docker images                        # list local images
docker rmi mysql:8                   # delete image (containers must be removed first)
docker image prune                   # remove unused images
```

---

## Docker Container Lifecycle

```cmd
# Create + start (background)
docker run -d --name mysql-userdb ^
   --network app-net ^
   -e MYSQL_ROOT_PASSWORD=rootpass ^
   -e MYSQL_DATABASE=userdb ^
   -p 3307:3306 ^
   -v mysql_data:/var/lib/mysql ^
   mysql:8

# Each flag means:
#   -d                     → detached (background)
#   --name X               → friendly name
#   --network NAME         → join custom network
#   -e KEY=VALUE           → environment variable
#   -p host:container      → port mapping
#   -v volume:/path        → volume mount
#   image:tag              → which image

# Container management
docker ps                            # running containers
docker ps -a                         # all (incl. stopped)
docker ps -q                         # only IDs (scripting)
docker stop  mysql-userdb            # stop (preserves data)
docker start mysql-userdb            # restart stopped
docker restart mysql-userdb          # stop + start
docker rm    mysql-userdb            # delete container
docker rm -f mysql-userdb            # force delete (running bhi)
```

---

## Docker Debugging

```cmd
docker logs    mysql-userdb          # show stdout/stderr
docker logs -f mysql-userdb          # follow real-time (like tail -f)
docker logs --tail 50 mysql-userdb   # last 50 lines
docker logs mysql-userdb | findstr "ready"   # filter (CMD)
docker logs mysql-userdb | Select-String "ready"  # filter (PowerShell)

docker inspect mysql-userdb          # detailed JSON (network, mounts, env)
docker stats                         # live CPU/memory usage
docker top mysql-userdb              # processes inside container
```

---

## Docker Exec (Container ke Andar)

```cmd
# General shell
docker exec -it <container> bash     # Linux bash shell
docker exec -it <container> sh       # Minimal shell (alpine)

# Direct MySQL client
docker exec -it mysql-userdb mysql -u root -prootpass

# Inside MySQL prompt:
USE userdb;                          # switch database
SHOW DATABASES;                      # list all DBs
SHOW TABLES;                         # list tables
DESCRIBE users;                      # table schema
SELECT * FROM users;                 # query data
\q   or  exit                        # exit MySQL
```

---

## Network Commands

```cmd
docker network create app-net        # create custom bridge
docker network ls                    # list all networks
docker network inspect app-net       # detailed info (containers, IPs, subnet)
docker network rm app-net            # delete network (no containers using it)
docker network prune                 # remove unused networks

# Connect existing container to network
docker network connect app-net mysql-userdb
docker network disconnect app-net mysql-userdb
```

---

## Volume Commands

```cmd
docker volume create mysql_data      # create named volume
docker volume ls                     # list all volumes
docker volume inspect mysql_data     # details (location, mount points)
docker volume rm mysql_data          # delete (no containers using)
docker volume prune -f               # cleanup all unused (orphans)
```

---

## Docker Compose Commands

```cmd
# Build + Start
docker-compose up -d                 # start (background), use existing images
docker-compose up -d --build         # rebuild app image first
docker-compose up                    # foreground (logs flood)

# Status
docker-compose ps                    # services status
docker-compose ps -a                 # all (stopped bhi)

# Logs
docker-compose logs                  # all services
docker-compose logs app              # specific service
docker-compose logs -f app           # follow (live tail)
docker-compose logs --tail 20 app    # last 20 lines

# Stop / Remove
docker-compose stop                  # stop (keep images, volumes)
docker-compose start                 # restart stopped
docker-compose restart               # stop + start
docker-compose down                  # stop + remove containers + network
docker-compose down -v               # also remove volumes (DATA GONE!)

# Inside container
docker-compose exec mysql bash       # shell into mysql service
docker-compose exec app bash         # shell into app service

# Build only (no start)
docker-compose build                 # build all
docker-compose build app             # build specific
```

---

## Maven + Java Run

```cmd
cd C:\DSA_PRACTICE\02_SPRING\PROJECT\usercrud

# Build (clean + compile + package)
mvn clean package -DskipTests

# Phases:
#   clean        → target/ delete
#   validate     → project structure OK?
#   compile      → .java → .class
#   test         → unit tests run (skip with -DskipTests)
#   package      → jar/war banao

# Run jar (Spring on host)
java -jar target\usercrud-0.0.1-SNAPSHOT.jar
java -jar target\usercrud-0.0.1-SNAPSHOT.jar --spring.profiles.active=local
java -jar target\usercrud-0.0.1-SNAPSHOT.jar --spring.profiles.active=docker
```

---

## Spring Profile Activation (4 ways)

```cmd
REM 1. application.properties (default)
spring.profiles.active=local

REM 2. CLI argument
java -jar app.jar --spring.profiles.active=docker

REM 3. OS env var
set SPRING_PROFILES_ACTIVE=docker

REM 4. Docker run -e flag
docker run -e SPRING_PROFILES_ACTIVE=docker my-app

REM 5. docker-compose.yml
services:
  app:
    environment:
      SPRING_PROFILES_ACTIVE: compose

REM Priority (high → low): CLI > Env var > Properties file
```

---

# CONCEPTS LOCKED (Full Master List)

```
DAY 1 (Foundation + Integration):
Image vs Container             (Recipe vs Dish)
Docker Hub registry            (cloud images)
Layered file system            (caching saves bandwidth)
Container ID + Name            (--name = friendly)
Detached mode                  (-d = background)
Environment variables          (-e = runtime config injection)
Port mapping                   (-p host:container)
Port conflict resolution       (different host port)
Container lifecycle            (run/stop/start/rm)
docker ps columns              (status decode)
docker logs                    (verify readiness)
docker exec                    (container ke andar enter)
Container isolation            (local vs Docker MySQL)
Spring Boot DB switch          (URL+Driver+Creds+Dialect = 4 changes)
JDBC URL format                (jdbc:mysql://host:port/dbname)
MySQL Connector/J              (com.mysql.cj.jdbc.Driver)
Hibernate dialect              (DB-specific SQL flavor)
Hibernate 7 migration trap     (MySQL8Dialect REMOVED → MySQLDialect)
HikariPool                     (Spring Boot connection pooling)
ddl-auto=update                (Hibernate auto-schema)
Maven manual install           (user-level env vars, no admin)
Maven phases                   (clean/compile/test/package)
BCrypt password hashing        ($2a$10$... format)
JWT vs Refresh token           (stateless 15min vs DB-stored 7 days)

VOLUMES:
Container ephemeral by default
Volume = persistent, container-independent
Named / Bind / tmpfs (3 types)
Volume lifecycle (create/mount/unmount/rm)
Anonymous volumes (Dockerfile VOLUME directive)
Volume orphan cleanup (docker volume prune)

DAY 2 (Networks + Compose):
Custom bridge networks (vs default)
Container DNS (service name = hostname)
docker network create / inspect / connect
Internal IP assignment (subnet auto-managed)
Multi-container talk by name
docker-compose YAML structure
services / image / build / environment
depends_on + healthcheck (proper ordering)
external volumes (reuse pre-created)
Auto-generated network (Compose default)
Eclipse Temurin migration (openjdk deprecated)
Profile activation via env var
Spring auto-loads application-{profile}.properties
Compose env var = bridge between Compose and Spring
Production-grade pattern (multi-container stack)
```

---

# TRAP BOX (22 Production Gotchas)

```
Trap 1: Container "Up" = ready
         Up status milta jaldi, but app inside takes time
         docker logs X → "ready for connections" wait

Trap 2: Same host port conflict
         Host port 3307 ek hi container use kar sakta
         Different host ports for different containers

Trap 3: Container delete = data gone (no volumes)
         docker rm = container record + writable layer GONE
         Volumes use karo data persist

Trap 4: Image delete with running containers
         docker rmi running container ki image nahi delete karega
         Stop + remove containers FIRST, phir image delete

Trap 5: localhost in container ≠ localhost on host
         Inside container "localhost" = container itself
         Host se: localhost:3307 (mapped port)
            Container-to-container: container/service name

Trap 6: Wrong Hibernate dialect = startup fail
         MySQL8Dialect with Hibernate 7 → CLASS NOT FOUND
         MySQLDialect (universal, auto-detects version)

Trap 7: -p without -d = terminal stuck
         Foreground + bound port = can't Ctrl+C cleanly
         Always -d (detached) for services

Trap 8: Port conflict from local services
         Local MySQL57 service silently uses 3306
         Get-Service check + use different port (3307)

Trap 9: Old PowerShell window after env var change
         Same window mein PATH update reflect nahi
         NAYA terminal khol — env var refresh

Trap 10: -p on no space (MySQL convention)
         -p (port) → -p HOST:CONTAINER
            -p (mysql password) → -prootpass (NO space)
         Different commands, different conventions

Trap 11: Spring Boot password verification raw compare
         Plain string compare passwords ke liye
         BCryptPasswordEncoder.matches() — algorithmic

Trap 12: Mount path container ke andar mismatch
         -v mysql_data:/var/mysql      (wrong path)
         -v mysql_data:/var/lib/mysql  (MySQL ka actual data path)

Trap 13: Volume + container delete cleanup
         docker rm container → volume orphan ho jata
         docker rm -v container        (volume bhi delete)

Trap 14: Linux containers permissions issue
         Bind mount Windows folder → Linux container
            permission errors (UID mismatch)
         Named volume preferred (Docker handles permissions)

Trap 15: Volume stale data trap
         Volume mein purana DB schema, naya app version
            → migration error
         Volume + DB version sync rakho

Trap 16: openjdk image deprecated
         FROM openjdk:17-jdk-slim  → "not found" 2024+
         FROM eclipse-temurin:17-jdk-jammy

Trap 17: external volume forgot = naya empty
         volumes: mysql_data: (no external: true)
            → compose creates "<project>_mysql_data" empty
         external: true to reuse pre-created volume

Trap 18: depends_on without healthcheck
         depends_on: - mysql
            → app starts before MySQL ready
         condition: service_healthy + healthcheck on mysql

Trap 19: Service name ≠ container name
         Confusion: "mysql" service, "mysql-userdb" container
         Service name = YAML key (DNS), container_name = actual

Trap 20: Build without rebuild
         docker-compose up after Dockerfile change → uses old image
         docker-compose up --build  (force rebuild)

Trap 21: Profile env var typo
         SPRING_PROFILES_ACTIVE: compose-profile
         SPRING_PROFILES_ACTIVE: compose
            (must match application-{X}.properties suffix)

Trap 22: Compose YAML overrides Dockerfile?
         Both run = wrong assumption
         docker-compose runs ONLY when you use docker-compose command
            "docker run" IGNORES docker-compose.yml entirely
```

---

# ULTIMATE POWER PHRASE (Day 1 + Day 2)

> *"Docker = Image (read-only template) + Container (running instance) + Volume (persistent) + Network (talk) + Compose (orchestrate). Container ephemeral by default — delete = data gone (volumes solve this). Custom bridge network gives container-name DNS — no IP needed. Compose YAML declarative entire stack — depends_on + healthcheck for ordering, external volumes for data reuse. Spring on host = localhost:host-port (port mapping); Spring in container = service-name:internal-port (DNS). Profile via SPRING_PROFILES_ACTIVE env var = Compose injects, Spring auto-loads matching properties file. openjdk deprecated → Eclipse Temurin. Production multi-container apps = Compose pattern, K8s migration easy from here."*

---

# COMPLETE JOURNEY (Day 1 + Day 2 single view)

```
DAY 1 — Foundation + Integration + Volumes
═══════════════════════════════════════════════
1.  Docker Desktop install (WSL2)
2.  docker pull mysql:8
3.  docker run with -p 3306 → port conflict (local MySQL57)
4.  docker rm + retry with -p 3307:3306 → container UP
5.  docker logs → "ready for connections" 
6.  pom.xml: add mysql-connector-j dependency
7.  application.properties: H2 → MySQL (4 lines)
8.  mvn → "mvn not recognized"
9.  Manual Maven install (download + env vars)
10. mvn clean package → BUILD SUCCESS
11. java -jar → MySQL8Dialect not found (Hibernate 7)
12. application.properties: MySQL8Dialect → MySQLDialect
13. mvn rebuild + java -jar → Connection LIVE!
14. Postman /auth/register → 200 OK, user in DB
15. Postman /auth/login → 200 OK, JWT + refresh
16. docker exec → SELECT * FROM users → REAL DATA seen
17. Get-Service → local MySQL57 discovered
   = ISOLATION concept fully understood
18. VOLUMES disaster:
       docker rm -f → naya container empty (no volume) → SAB DATA GONE
19. VOLUMES fix:
       docker volume create mysql_data
       docker run -v mysql_data:/var/lib/mysql ... → Arpan re-add
       docker rm -f → naya container WITH SAME volume → DATA SURVIVES 
   = VOLUMES LOCKED


DAY 2 — Networks + Compose
═══════════════════════════════════════════════
20. docker network ls → 3 default networks
21. docker rm -f mysql-userdb (cleanup, volume safe)
22. docker network create app-net
23. docker run --network app-net + -v mysql_data
24. docker network inspect (verify mysql container on network)
25. docker run -it --rm --network app-net mysql:8 mysql -h mysql-userdb
       → Connect by NAME (no IP, no localhost)
       → SELECT * FROM users → Arpan visible
   = NETWORKS LOCKED 

26. CREATE: application-compose.properties
       url=jdbc:mysql://mysql:3306/userdb (service name + internal port)
27. CREATE: docker-compose.yml
       mysql + app services + healthcheck + external volume
28. docker rm -f mysql-userdb (cleanup before compose)
29. mvn clean package -DskipTests (build JAR)
30. docker-compose up -d --build
       → openjdk:17-jdk-slim NOT FOUND (deprecated)
31. FIX: Dockerfile FROM eclipse-temurin:17-jdk-jammy
32. docker-compose up -d --build → SUCCESS
       • mysql container healthy (waited via healthcheck)
       • app container running (started after mysql healthy)
       • Both on auto-created network
33. docker-compose ps → both Up
34. docker-compose logs app → Tomcat started
35. Postman /auth/login → 200 OK + new refresh token
       → Arpan's data from Day 1 volume + Compose stack working
   = COMPOSE LOCKED 

36. ENTRYPOINT pattern (Dockerfile updated):
       CMD ["java", "-jar"] → ENTRYPOINT ["java", "-jar"]
       Production-grade, signals work properly
```

---

## What's Pending (Docker phase)

```
ARG vs ENV                 (build vs runtime config)
Multi-stage builds         (production image small)
HEALTHCHECK deepen         (already used in compose)
Image push to Docker Hub   (registry workflow)

= ~2 hr more for full Docker mastery
```

[← Back to README](00_README.md) | [← Compose](06_docker_compose.md)
