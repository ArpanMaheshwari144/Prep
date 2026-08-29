# AWS Setup Journey — TODO App Deploy

## STEP 1: AWS Account Creation
- URL: https://aws.amazon.com/
- Plan: Free (6 months, $200 credits, free tier services)
- Primary purpose: Personal use
- Ownership type: Individual
- Card verification: ~₹2 hold (refundable)
- Auto-set spending limit: ₹15000 (authorization limit, NOT auto-charge)


## STEP 2: Region Change
- Default region was Stockholm (eu-north-1) — far for India
- Top-right region dropdown → Asia Pacific (Mumbai) = ap-south-1
- Lower latency, lower data transfer cost


## STEP 3: Budget Alert (Safety Net)
- Path: Billing and Cost Management → Budgets → Create budget
- Template: Zero-spend budget (notifies at $0.01 spend)
- Name: My Zero-Spend Budget
- Email recipient: armanmaliksmartcool@gmail.com
- Result: Any AWS spend > $0.01 = instant email alert


## STEP 4: Push Docker Image to Docker Hub
Pre-req: Docker Hub account (username: arpanmah)

```
docker login                                            - authenticate
docker tag todoapp-app:latest arpanmah/todoapp:latest   - re-tag local image
docker push arpanmah/todoapp:latest                     - upload (595 MB)
```

After push: image at https://hub.docker.com/r/arpanmah/todoapp


## STEP 5: EC2 Launch (UI walkthrough)
- Name: todoapp
- AMI: Amazon Linux 2023 (free tier eligible, 64-bit x86)
- Instance type: t3.micro (free tier, 2 vCPU BURSTABLE, 1 GiB RAM)
- Key pair: todoapp-key (RSA, .pem format)
  - Downloaded to C:\DSA_PRACTICE\todoapp-key.pem
  - gitignore updated: *.pem, *.ppk, *.key
- Security group inbound rules:
  - SSH (TCP 22) from Anywhere
  - Custom TCP 8080 from Anywhere
- Storage: 8 GiB gp3 (default, free tier)
- Result:
  - Instance ID: i-032c273fe7c7e31dd
  - Public IP: 13.127.249.108
  - State: Running


## STEP 6: SSH into instance

### Error 1: UNPROTECTED PRIVATE KEY
```
ssh -i todoapp-key.pem ec2-user@13.127.249.108
```
- "Bad permissions" — Windows default ACL too open
- SSH refuses to use the key

### Fix:
```
icacls todoapp-key.pem /inheritance:r              - remove inherited perms
icacls todoapp-key.pem /grant:r "%username%:R"     - grant READ to current user
```

### Successful SSH:
```
ssh -i todoapp-key.pem ec2-user@13.127.249.108
# yes to "authenticity" prompt
```
Output: Amazon Linux 2023 banner + prompt `[ec2-user@ip-172-31-39-240 ~]$`


## STEP 7: System update
```
sudo yum update -y
```
Output: "Nothing to do. Complete!" (fresh AMI up to date)


## STEP 8: Install Docker
```
sudo yum install -y docker          - install Docker (25.0.14)
sudo systemctl start docker         - start Docker daemon
sudo systemctl enable docker        - auto-start on reboot
sudo usermod -aG docker ec2-user    - add user to docker group
exit                                 - logout
ssh -i todoapp-key.pem ec2-user@13.127.249.108   - relogin
docker --version                     - verify
docker ps                            - verify empty list (working)
```


## STEP 9: Install Docker Compose (standalone binary)
```
docker compose version             - "compose is not a docker command"
                                   - plugin NOT installed by default

sudo curl -L "https://github.com/docker/compose/releases/latest/download/docker-compose-linux-x86_64" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
docker-compose version             - "Docker Compose version v5.1.4"
```


## STEP 10: Create docker-compose.yml on EC2

### Attempts that FAILED:
1. nano + paste — indentation corruption (auto-indent conflict)
2. heredoc (`cat > file << 'EOF'`) — EOF terminator had leading whitespace

### Success: local file → copy from VS Code → paste in nano
- Created C:\DSA_PRACTICE\07_PROJECTS\todoapp\docker-compose.deploy.yml locally
- VS Code Ctrl+A → Ctrl+C (clean text from FILE, not chat/terminal)
- EC2: rm docker-compose.yml + nano docker-compose.yml
- Right-click paste in nano = clean indentation
- Ctrl+O → Enter → Ctrl+X = saved

### docker-compose.yml content (3 services):
- app: arpanmah/todoapp:latest (Docker Hub) | port 8080
- mysql: mysql:8 | tododb auto-create | PVC volume | healthcheck
- redis: redis | 125mb maxmemory | LRU eviction

### Validate:
```
docker-compose config              - YAML valid, no errors
```


## STEP 11: Deploy stack
```
docker-compose up -d
```
Output:
- Image redis Pulled (6.6s)
- Image arpanmah/todoapp:latest Pulled (17.8s)
- Image mysql:8 Pulled (23.9s)
- Network ec2-user_default Created
- Volume ec2-user_mysql-data Created
- Container redis Started
- Container todoapp-mysql Healthy (21.8s)
- Container todoapp Started

= ALL 3 CONTAINERS RUNNING ON EC2 (proof of deploy)


## STEP 12: External test FAILED — diagnostics

### Postman:
- POST http://13.127.249.108:8080/auth/register
- Error: connect ECONNREFUSED 13.127.249.108:8080

### Initial wrong diagnosis: OOM (out of memory)
- Reboot tried via AWS Console — same result

### Correct diagnosis (after `docker logs` + `free -h`):
- free -h: 916 MB total, 173 MB USED, 606 MB FREE
  = RAM was PLENTY, not OOM
- Logs showed: "Thread starvation or clock leap detected"
- = CPU starvation, NOT memory

### Root cause: t3.micro burstable CPU credit exhaustion
- t3 family = BURSTABLE CPU
- Idle = credits accumulate
- Busy = credits burn
- Credits 0 = throttled to ~10% baseline CPU
- Spring Boot startup + MySQL init + Redis = burned all credits
- App started but couldn't serve traffic (thread starvation)


## STEP 13: Useful diagnostic commands

```
docker ps                        - running containers
docker logs <name> --tail 50     - container logs
free -h                          - real memory state
top                              - live process CPU/mem
docker stats                     - per-container resources live
dmesg | grep -i kill             - OOM killer events
df -h                            - disk usage
uptime                           - load average (1/5/15 min)
                                   > vCPU count = overloaded
```

### Exact 3-command combo that uncovered CPU starvation:
```
docker ps                        - empty (containers died/gone)
free -h                          - 916M total, 173M used, 606M free
                                   = RAM was FINE, not OOM
docker logs todoapp --tail 30    - revealed Hikari warning:
                                   "Thread starvation or clock leap detected
                                    (housekeeper delta=47s)"
                                   = JVM threads not getting CPU
                                   = CPU credits exhausted
```

### CloudWatch t3-specific (in AWS Console):
- Path: EC2 → Instance → Monitoring tab
- CPUCreditBalance graph = credits remaining (0 = throttled)
- CPUCreditUsage graph = how fast burning
- CPUUtilization graph = actual %


## STEP 14 (pending): DELETE everything (cost zero)
- AWS Console → EC2 → Instances → select todoapp
- Instance state → TERMINATE instance
- Confirm
- Wait ~2 min for instance terminated state
- Verify Billing dashboard = $0


## REAL INSIGHTS (interview gold)

### 1. t3.micro burstable trap
"Hit t3.micro burstable CPU credit exhaustion deploying
 Spring stack — initial JVM warmup + Hibernate schema
 creation burned credits, fell to throttled baseline,
 triggered thread starvation in HikariCP. App started
 but couldn't serve traffic."

### 2. Production architecture answer
"Production: use t3.medium+ (more baseline CPU) OR
 m5/c5 family (fixed CPU, not burstable). Better yet,
 offload stateful services to managed (RDS, ElastiCache)
 to reduce EC2 compute load."

### 3. Cost discipline (FinOps signal)
"AWS free tier is acquisition marketing, not production
 reality. t3.micro 1 GB RAM + burstable CPU sized for
 'Hello World', not full Java backend. Senior architect
 right-sizes and considers managed services trade-off."


## ERRORS ENCOUNTERED + FIXES
1. UNPROTECTED PRIVATE KEY (Windows .pem permissions) — icacls fix
2. YAML parse error in nano paste — used local file copy approach
3. Heredoc EOF didn't terminate (leading whitespace) — abort + retry strategy
4. SSH Ctrl+C disconnect — reconnected via ssh command
5. ECONNREFUSED on external test — root cause CPU starvation (not OOM as initially assumed)


## NANO PASTE GOTCHA
- Auto-indent ON by default + paste = double-indent = YAML break
- Workaround: keep file local in VS Code, copy from FILE (not terminal/chat)
- Right-click paste in nano then preserves source indentation


## CHANGES LOCKED POST-LAUNCH
- AMI (OS image)
- Key pair
- Storage size decrease
- VPC/subnet
- Region


## CHANGES CAN BE DONE POST-LAUNCH
- Security group rules (add/remove ports)
- Tags / name
- IAM role attach/detach
- Storage size INCREASE
- Stop → change instance type → start (downtime)


## USEFUL COMMANDS
```
docker ps                                       - list running containers
docker logs <name>                              - container logs
docker logs -f <name>                           - follow logs
docker-compose ps                               - compose-managed containers
docker-compose down                             - stop + remove all
docker-compose down -v                          - also delete volumes
aws ec2 describe-instances                      - list EC2 instances
aws ec2 terminate-instances --instance-ids <id> - delete instance
free -h                                         - memory state
top                                             - live processes
docker stats                                    - per-container resources
```


## SAFETY RULES
- Daily billing dashboard check
- Delete unused resources immediately
- Zero-spend budget = always on
- Region locked to Mumbai (ap-south-1)
- *.pem in .gitignore (never commit private keys)


## KEY TAKEAWAYS
1. Free tier = MARKETING TASTE, not realistic production
2. t3 burstable family = unreliable for sustained load
3. Memory diagnosis ≠ CPU diagnosis (always check both)
4. Container "Started" ≠ App "Ready to serve"
5. AWS managed services (RDS, ElastiCache) free EC2 compute load
6. FinOps = senior engineering concern
