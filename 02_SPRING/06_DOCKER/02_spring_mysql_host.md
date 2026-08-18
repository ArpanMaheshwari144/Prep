# Section A2 — Spring Boot ↔ MySQL Container (Day 1)

> **Setup:** Spring Boot runs on HOST, MySQL in container
> **Connection:** localhost:3307 (port mapping)
> **Profile:** docker

[← Back to README](00_README.md) | [← Foundation](01_foundation.md) | [Volumes →](03_volumes.md)

---

# PART 9 — `pom.xml` Update (MySQL Connector)

## Added dependency:

```xml
<dependency>
    <groupId>com.mysql</groupId>
    <artifactId>mysql-connector-j</artifactId>
    <scope>runtime</scope>
</dependency>
```

## Why mysql-connector-j (not -java)?
```
"mysql-connector-java" → DEPRECATED (since 8.0.31)
"mysql-connector-j"    → NEW official artifact name

Dono same library — sirf naam refresh
Spring Boot manages version (no <version> tag)
scope = runtime → compile time nahi chahiye, runtime pe load
```

## H2 status:
```
H2 dependency RAKHA hai (scope: runtime)
   → Tests run karne ke liye useful
   → Future fallback ke liye
   → Use nahi karenge actively (MySQL primary now)
```

---

# PART 10 — `application.properties` Switch

## Concept first — DB switch karne ke liye 4 cheezein:

```
1. URL          → Database kahaan hai (host:port/dbname)
2. Driver       → Java se DB tak baat karne ki library
3. Credentials  → Username + password
4. Dialect      → Hibernate ko DB ka SQL flavor
```

**Bus yeh 4 change. App code mein ek line nahi badli — yahi JPA ka power.**

## Before (H2 — in-memory fake):

```properties
spring.datasource.url=jdbc:h2:mem:userdb
spring.datasource.driverClassName=org.h2.Driver
spring.datasource.username=sa
spring.datasource.password=
spring.jpa.database-platform=org.hibernate.dialect.H2Dialect
spring.h2.console.enabled=true
```

## After (MySQL — real, Docker container):

```properties
spring.datasource.url=jdbc:mysql://localhost:3307/userdb
spring.datasource.driverClassName=com.mysql.cj.jdbc.Driver
spring.datasource.username=root
spring.datasource.password=rootpass
spring.jpa.database-platform=org.hibernate.dialect.MySQLDialect
# Note: MySQL8Dialect REMOVED in Hibernate 7 (Spring Boot 4 uses Hibernate 7.2)
# Use MySQLDialect — auto-detects version from JDBC connection
# (Detail in PART 12 below)
# h2.console removed — not used anymore
```

## Each line decoded:

```properties
spring.datasource.url=jdbc:mysql://localhost:3307/userdb
                       ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
                       jdbc:mysql:    → JDBC protocol + MySQL
                       //localhost:3307 → tera laptop ka 3307
                       /userdb        → database name (auto-created)

spring.datasource.driverClassName=com.mysql.cj.jdbc.Driver
                                  ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
                                  "cj" = Connector/J (MySQL Java driver)
                                  Yeh class pom.xml dependency mein hai

spring.datasource.username=root        ← docker run mein set
spring.datasource.password=rootpass    ← MYSQL_ROOT_PASSWORD value

spring.jpa.database-platform=org.hibernate.dialect.MySQLDialect
                                                    ↑↑↑↑↑↑↑↑↑↑↑↑↑↑
                                                    Hibernate ko bola
                                                    "MySQL ka SQL flavor use kar"
                                                    (universal — auto-detects 5.7/8.x)
```

---

# PART 11 — Maven Install (Detour)

## Problem:
```powershell
mvn clean package -DskipTests
```
**Error:** `mvn is not recognized as the name of a cmdlet...`

**Root cause:** Maven not in PATH. Java was there but Maven separate.

## 3 Approaches Tried

### Approach 1: winget (Windows Package Manager)
```powershell
winget install Apache.Maven
```
**Result:** "No package found" — Apache Maven not in winget repo.

### Approach 2: Chocolatey
```powershell
choco install maven -y
```
**Result:** "Access denied" — Chocolatey needs admin (current session not elevated).

### Approach 3: Manual Download (worked, NO ADMIN)

```powershell
# Step 1 — Download Maven binary zip from Apache archive
$url = "https://archive.apache.org/dist/maven/maven-3/3.9.9/binaries/apache-maven-3.9.9-bin.zip"
Invoke-WebRequest -Uri $url -OutFile "$env:TEMP\maven.zip"
# Output: 9.5 MB downloaded

# Step 2 — Extract to USER folder (no admin needed for Users\...)
$installDir = "C:\Users\ArpanMaheshwari\Tools"
New-Item -ItemType Directory -Path $installDir -Force | Out-Null
Expand-Archive -Path "$env:TEMP\maven.zip" -DestinationPath $installDir -Force
# Result: C:\Users\ArpanMaheshwari\Tools\apache-maven-3.9.9\

# Step 3 — Set USER environment variables (no admin)
$mavenHome = "$installDir\apache-maven-3.9.9"
[Environment]::SetEnvironmentVariable("MAVEN_HOME", $mavenHome, "User")

$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
[Environment]::SetEnvironmentVariable("Path", "$userPath;$mavenHome\bin", "User")

# Step 4 — Verify (NEW PowerShell window for PATH refresh)
mvn --version
```

**Output (success):**
```
Apache Maven 3.9.9 (8e8579a9e76f7d015ee5ec7bfcdc97d260186937)
Maven home: C:\Users\ArpanMaheshwari\Tools\apache-maven-3.9.9
Java version: 26
OS name: "windows 11"
```

## Concept — User vs System env vars

```
SYSTEM env vars  → All users on machine, needs ADMIN
USER env vars    → Only current user, NO ADMIN needed
                   Stored in: HKCU:\Environment registry

We used USER level → Admin nahi padi
```

## Maven Wrapper alternative

Spring Initializr har project mein bundle karta:
```powershell
.\mvnw.cmd clean package -DskipTests
```
- No global Maven install needed
- Project-local
- Project mein commit hota — har dev same version
- Pehli baar slow (correct Maven download)

---

# PART 12 — Hibernate 7 Dialect Trap (BIG GOTCHA!)

## The Error:

`mvn clean package` → `BUILD SUCCESS`
`java -jar target/usercrud-0.0.1-SNAPSHOT.jar` → ERROR:

```
WARN HHH100046: Could not obtain connection to query JDBC database metadata

org.hibernate.boot.registry.selector.spi.StrategySelectionException:
   Unable to resolve name [org.hibernate.dialect.MySQL8Dialect]
   as strategy [org.hibernate.dialect.Dialect]
```

## Root Cause:

```
Spring Boot 4.0.6 → uses Hibernate 7.2.x

Hibernate dialect timeline:
   Hibernate 5.x   →  org.hibernate.dialect.MySQL8Dialect exists
   Hibernate 6.x   →  MySQL8Dialect DEPRECATED
   Hibernate 7.x   →  MySQL8Dialect REMOVED

Old guides + StackOverflow → MySQL8Dialect bata rahe (5.x era)
Tu Spring Boot 4 use kar raha → Hibernate 7 → class hi nahi
```

## Fix (in `application.properties`):

```properties
# Old (Hibernate 5/6 era):
# spring.jpa.database-platform=org.hibernate.dialect.MySQL8Dialect

# New (Hibernate 7+):
spring.jpa.database-platform=org.hibernate.dialect.MySQLDialect
```

**`MySQLDialect`** = universal MySQL dialect (auto-detects version from JDBC connection).

## Interview Gold

**Q: "Spring Boot 3 → 4 migration mein kya breaks?"**

> *"Hibernate version jump 6→7. Sabse common: dialect classes removed (MySQL8Dialect, PostgreSQL10Dialect etc.). Solution: universal dialect (MySQLDialect, PostgreSQLDialect) — auto-detects version. Also Jakarta EE 11 (javax → jakarta), some deprecated APIs gone."*

---

# PART 13 — Build + Run

## Build:
```powershell
cd C:\DSA_PRACTICE\02_SPRING\PROJECT\usercrud
mvn clean package -DskipTests
```

### Phases (Maven lifecycle):
```
clean      → target/ folder delete (purana build hatao)
package    → compile + test (skipped) + create jar
-DskipTests → tests skip (faster build, dev mode)
```

### Pehli baar slow (~3-5 min):
- Maven plugins download
- Spring Boot dependencies (~50-100 MB)
- Hibernate, JJWT, MySQL Connector, Lombok, etc.
- All cached in `~/.m2/repository/` (next time fast)

### Last expected output:
```
[INFO] BUILD SUCCESS
[INFO] Total time:  XX s
```

**Generated artifact:** `target/usercrud-0.0.1-SNAPSHOT.jar` (~50 MB fat jar)

## Run:
```powershell
java -jar target\usercrud-0.0.1-SNAPSHOT.jar
```

Spring Boot startup begins, console output flows.

---

# PART 14 — Spring Boot Connection SUCCESS

## Real Output Captured:

```
Database info:
   Database JDBC URL [jdbc:mysql://localhost:3307/userdb]
   Database driver: MySQL Connector/J
   Database dialect: MySQLDialect
   Database version: 8.4.9
   Default catalog/schema: userdb/undefined
   Isolation level: REPEATABLE_READ [default REPEATABLE_READ]
   Pool: DataSourceConnectionProvider

Hibernate: create table refresh_tokens (
    id bigint not null auto_increment,
    expires_at datetime(6) not null,
    token varchar(255) not null,
    user_id bigint not null,
    primary key (id)
) engine=InnoDB

Hibernate: create table users (
    id bigint not null auto_increment,
    age integer not null check ((age>=18) and (age<=120)),
    email varchar(255) not null,
    name varchar(50) not null,
    password varchar(255) not null,
    role varchar(255) not null,
    primary key (id)
) engine=InnoDB

Hibernate: alter table refresh_tokens add constraint UK... unique (token)
Hibernate: alter table users add constraint UK... unique (email)

Initialized JPA EntityManagerFactory for persistence unit 'default'
Tomcat started on port 8080 (http) with context path '/'
Started UsercrudApplication in 8.254 seconds
```

## Yeh DEKHA:

```
MySQL container 8.4.9 connected (localhost:3307)
Schema auto-created (ddl-auto=update)
engine=InnoDB (MySQL transactional storage)
Constraints applied:
      • check (age>=18 AND age<=120) — Bean Validation se aaya
      • unique email
      • unique token
HikariPool active (Spring Boot's connection pooling)
Tomcat listening on port 8080
App started in 8.254 seconds
```

## Concept — REPEATABLE_READ Isolation

```
MySQL InnoDB default isolation level:
   • Same transaction mein same query → same result (consistent reads)
   • Phantom reads possible

Other levels (lowest → highest isolation):
   READ UNCOMMITTED < READ COMMITTED < REPEATABLE READ < SERIALIZABLE

Trade-off: Higher isolation → more locks → less concurrency
```

---

# PART 15 — Postman Test 1: Register

## Request:
```http
POST http://localhost:8080/auth/register
Content-Type: application/json

{
  "name": "Arpan",
  "email": "arpan@test.com",
  "age": 27,
  "password": "test123"
}
```

## Response (200 OK):
```json
{
  "id": 1,
  "name": "Arpan",
  "email": "arpan@test.com",
  "password": "$2a$10$qG1NHoJ1GOELldSA6xSv2uV7bBuS01fFhafROy4/tFStgVE5ngalC",
  "age": 27,
  "role": "USER"
}
```

## Important Observations:

```
1. id: 1 → MySQL auto-increment (DB ne diya, app ne nahi)

2. password → BCrypt HASHED (NOT plain "test123"):
      $2a$10$qG1NHoJ1...
       │  │  └─ actual hash
       │  └──── cost factor (10 = 2^10 rounds)
       └─────── BCrypt version

   Production-grade security:
   • Plain "test123" NEVER stored
   • Hash one-way (cannot reverse)
   • Same password = different hash each time (salt)

3. role: "USER" → default role assigned by Spring Security

4. End-to-end flow:
      Postman → HTTP → Spring Boot → BCrypt hash
            → JPA save → JDBC INSERT → MySQL container → row stored
```

---

# PART 16 — Postman Test 2: Login (JWT + Refresh)

## Request:
```http
POST http://localhost:8080/auth/login
Content-Type: application/json

{
  "email": "arpan@test.com",
  "password": "test123"
}
```

## Response (200 OK, 374ms):
```json
{
  "accessToken": "eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJhcnBhbkB0ZXN0LmNvbSIsImlhdCI6...",
  "refreshToken": "9ff5a083-5af3-4b94-aec3-c675876b9b23"
}
```

## Two Token Types — Why?

```
ACCESS TOKEN (JWT format)
   • header.payload.signature (3 parts, base64-encoded)
   • eyJhbGc... → decodes to {"alg":"HS256","typ":"JWT"}
   • Self-contained — backend verifies offline (no DB hit)
   • Short-lived: 15 min (jwt.expiration=900000 ms)
   • Stateless — server doesn't store
   • Trade-off: cannot be revoked instantly until expires

REFRESH TOKEN (UUID format)
   • Random UUID (9ff5a083-5af3-4b94-aec3-c675876b9b23)
   • Server-side STORED in MySQL refresh_tokens table
   • Long-lived: 7 days (jwt.refresh.expiration=604800000 ms)
   • REVOKABLE — DELETE from DB = token dead
   • Trade-off: DB hit needed for refresh
```

## Token Flow (interview):

```
Login           → access (15 min) + refresh (7 days)
Use access      → Bearer header in API requests
Access expires  → 401 Unauthorized
Send refresh    → /auth/refresh endpoint
Validate        → DB lookup (token exists + not expired)
                  Issue new access token
Logout          → DELETE refresh from DB → can't refresh again
```

## Backend Behind The Scenes:

```
1. POST /auth/login received
2. AuthController.login(LoginRequest)
3. CustomUserDetailsService.loadUserByUsername(email)
   → MySQL query: SELECT * FROM users WHERE email = ?
4. BCryptPasswordEncoder.matches(plain, storedHash)
   → algorithmic match (NOT raw string compare)
5. JwtService.generateAccessToken(email)
   → eyJ... 15-min JWT
6. RefreshTokenService.create(user)
   → INSERT INTO refresh_tokens (UUID, user_id, expires_at = NOW + 7 days)
7. Return TokenResponse(access, refresh)
```

---

# PART 17 — MySQL Verify (Real Data Inside Container)

## Command:
```powershell
docker exec -it mysql-userdb mysql -u root -prootpass
```

### Flag breakdown:
```
docker exec    → Container ke andar command run
-i             → Interactive (stdin open)
-t             → TTY allocate (proper terminal)
-it            → -i + -t combined (interactive shell)
mysql-userdb   → Container ka naam
mysql          → Container ke ANDAR mysql client run
-u root        → User: root
-prootpass     → Password (NO space after -p — MySQL convention)
```

## Inside MySQL prompt:

### Show all tables
```sql
USE userdb;
SHOW TABLES;
```
**Output:**
```
+------------------+
| Tables_in_userdb |
+------------------+
| refresh_tokens   |
| users            |
+------------------+
```

### Users data
```sql
SELECT id, name, email, age, role,
       SUBSTRING(password, 1, 30) AS pass_preview
FROM users;
```
**Output:**
```
+----+-------+-----------------+-----+------+--------------------------------+
| id | name  | email           | age | role | pass_preview                   |
+----+-------+-----------------+-----+------+--------------------------------+
|  1 | Arpan | arpan@test.com  |  27 | USER | $2a$10$qG1NHoJ1GOELldSA6xSv2u  |
+----+-------+-----------------+-----+------+--------------------------------+
```

### Refresh tokens
```sql
SELECT id, token, user_id, expires_at FROM refresh_tokens;
```
**Output:**
```
+----+--------------------------------------+---------+---------------------+
| id | token                                | user_id | expires_at          |
+----+--------------------------------------+---------+---------------------+
|  1 | 9ff5a083-5af3-4b94-aec3-c675876b9b23 |       1 | 2026-05-12 ...      |
+----+--------------------------------------+---------+---------------------+
```

### Exit:
```sql
\q
```
or `exit`

## FULL CIRCLE PROOF:

```
Postman ne dikhaya  → "refreshToken": "9ff5a083-5af3-4b94-..."
MySQL container mein → token: 9ff5a083-5af3-4b94-...
                       SAME UUID

Spring Boot → MySQL container → SAME data
End-to-end persistence VERIFIED
```

---

# PART 18 — Local MySQL vs Docker MySQL (ISOLATION DEMO)

## Discovery Command:
```powershell
Get-Service | Where-Object { $_.Name -match 'MySQL|mysqld' }
```

**Output:**
```
Name      Status   DisplayName
----      ------   -----------
MySQL57   Running  MySQL57
```

**Surprise:** Tera laptop pe local MySQL57 service already running thi (kabhi install kiya hoga, password forgot).

## Concept Lock — Two MySQLs running

```
TERA LAPTOP (host)

  ┌─────────────────────────┐
  │ LOCAL MySQL57           │  ← Windows service
  │ Port: 3306              │  ← purana install
  │ Apna data store         │
  │ Spring Boot ne          │
  │ CONNECT NAHI KIYA    │
  └─────────────────────────┘

  ┌─────────────────────────┐
  │ DOCKER CONTAINER        │
  │ MySQL 8 inside          │
  │ Port mapping: 3307→3306 │
  │ Spring Boot CONNECTED │
  │ userdb + tables yahan   │
  └─────────────────────────┘

  Yeh DO ALAG MySQL servers hain.
  Inn dono ka kuch lena-dena nahi.
  Yahi Docker ka CORE concept = ISOLATION
```

## Key Insights:

```
1. Container = isolated (filesystem, network, process tree)
2. Local MySQL stop karu → Docker MySQL pe ZERO impact
3. Docker container delete → local MySQL UNTOUCHED
4. Yahi reason Docker = "works on my machine" solver
5. Production environment = container behavior IDENTICAL everywhere
```

## Local MySQL handle (deferred):

```
Abhi Docker MySQL primary → local MySQL57 ko abhi chedna nahi
Future cleanup options:
   1. Stop service:    Stop-Service MySQL57       (admin needed)
   2. Disable boot:    Set-Service -StartupType Disabled
   3. Uninstall:       Control Panel → MySQL → Uninstall
```

---

# FULL COMMAND REFERENCE (Aaj Use Hua Sab)

## PowerShell / System Commands

```powershell
# Verify Docker installation
docker --version                          # Show Docker version
docker info                               # Detailed engine info

# Check what's running on which port (debugging)
Get-Service | Where-Object { $_.Name -match 'MySQL' }
                                          # Find local MySQL services
netstat -an | Select-String ":3306"      # Who's using port 3306?

# Maven install (manual, no admin)
Invoke-WebRequest -Uri $url -OutFile $tempZip
                                          # Download file
Expand-Archive -Path $zip -DestinationPath $dir -Force
                                          # Extract zip
[Environment]::SetEnvironmentVariable("MAVEN_HOME", $value, "User")
                                          # Set env var (user-level)
mvn --version                             # Verify Maven
```

## Docker Image Commands

```powershell
docker pull mysql:8                       # Download image from Docker Hub
docker images                             # List local images
docker rmi mysql:8                        # Delete image (containers must be removed first)
docker image prune                        # Remove unused images
```

## Docker Container Lifecycle

```powershell
# Create + start (background)
docker run -d --name mysql-userdb \
   -e MYSQL_ROOT_PASSWORD=rootpass \
   -e MYSQL_DATABASE=userdb \
   -p 3307:3306 \
   mysql:8

# Each flag means:
# -d                     → detached (background)
# --name X               → friendly name
# -e KEY=VALUE           → environment variable
# -p host:container      → port mapping (forward host port → container port)
# image:tag              → which image to run

# Container management
docker ps                                 # Running containers
docker ps -a                              # All (incl. stopped)
docker ps -q                              # Only IDs (scripting)
docker stop  mysql-userdb                 # Stop (preserves data, can start again)
docker start mysql-userdb                 # Restart stopped container
docker restart mysql-userdb               # Stop + start
docker rm    mysql-userdb                 # Delete container (data gone if no volume)
docker rm -f mysql-userdb                 # Force delete (running bhi)
```

## Docker Debugging

```powershell
docker logs    mysql-userdb               # Show container's stdout/stderr
docker logs -f mysql-userdb               # Follow real-time (like tail -f)
docker logs --tail 50 mysql-userdb        # Last 50 lines
docker inspect mysql-userdb               # Detailed JSON (network, mounts, env)
docker stats                              # Live CPU/memory usage
docker top mysql-userdb                   # Processes inside container
```

## Docker Exec (Container ke Andar)

```powershell
# General shell
docker exec -it <container> bash          # Linux bash shell
docker exec -it <container> sh            # Minimal shell (alpine)

# Direct MySQL client
docker exec -it mysql-userdb mysql -u root -prootpass

# Inside MySQL prompt:
USE userdb;                               # Switch database
SHOW DATABASES;                           # List all DBs
SHOW TABLES;                              # List tables
DESCRIBE users;                           # Table schema
SELECT * FROM users;                      # Query data
SELECT * FROM refresh_tokens;
\q   or  exit                             # Exit MySQL
```

## Maven + Java Run

```powershell
# Inside project folder
cd C:\DSA_PRACTICE\02_SPRING\PROJECT\usercrud

# Build (clean + compile + package)
mvn clean package -DskipTests

# Phases ki order:
# clean        → target/ delete
# validate     → project structure OK?
# compile      → .java → .class
# test         → unit tests run (skip with -DskipTests)
# package      → jar/war banao
# verify, install, deploy... (later phases)

# Other useful:
mvn dependency:tree                       # Dependency graph
mvn dependency:resolve                    # Just download deps
mvn -X clean package                      # Verbose debug

# Run jar
java -jar target\usercrud-0.0.1-SNAPSHOT.jar
java -jar -Dserver.port=8090 target\...   # Override property
```

## Spring Boot Property Overrides (runtime)

```powershell
# Command-line override (highest precedence)
java -jar usercrud.jar --spring.datasource.url=jdbc:mysql://localhost:3307/userdb

# Environment variable override
$env:SPRING_DATASOURCE_URL="jdbc:mysql://..."
java -jar usercrud.jar
```

---

