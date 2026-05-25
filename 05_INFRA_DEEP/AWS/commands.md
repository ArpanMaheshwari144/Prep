# AWS — Revision Reference (in progress)


## WHAT IS AWS

Amazon Web Services = cloud platform.
Compute + Storage + Database + Network + Security + Monitoring
sab ek jagah, pay-as-you-go.

= "Server kiraye pe lo" ka internet version.
= Apna datacenter banane ki zaroorat nahi.


## WHY AWS EXISTS

Pre-cloud era pain:
- Server kharidna = lakhs upfront
- Datacenter setup = months
- Capacity planning = guess game
- Peak ke liye over-provision, idle mein paisa waste
- DR/backup infrastructure = extra cost

AWS fix:
- Server chahiye = 2 min mein launch
- Use kiya = pay
- Band kiya = stop
- Global reach = ek click



HOTEL ROOM ANALOGY:
   Pre-cloud      = ghar khareedna (lakhs, permanent, maintenance tera)
   AWS            = hotel room (jab chahiye, jitna chahiye, leave karo)



## ORIGIN STORY

- 2003: Chris Pinkham + Benjamin Black paper at Amazon
  ("infrastructure as a service" vision)
- Amazon ne internally retail platform scale karne ke liye
  reusable infra banayi thi
- 2006: Public launch — S3 first, then EC2
- Andy Jassy = launch lead (later AWS CEO, then Amazon CEO)
- Jeff Bezos = sponsor + visionary backing
- Today: ~32% global cloud market share (largest)


## 6 CORE CATEGORIES

```
1. COMPUTE     - servers, run karo
2. STORAGE     - files, backup
3. DATABASE    - structured data
4. NETWORK     - connectivity
5. SECURITY    - access, encryption
6. MONITORING  - visibility, alerts
```

MALL ANALOGY:
- AWS = mall (200+ shops)
- Each shop = ek service
- Tu jitne chahiye utne use kar, rent pay


## CATEGORY 1: COMPUTE

```
EC2         - virtual server (full control, you manage)
Lambda      - serverless (function chalao, server bhul jao)
ECS         - container service (Docker on AWS)
EKS         - managed K8s
App Runner  - "Heroku of AWS" (push code, auto-deploy)
```

WHEN TO USE WHAT:
- Full control + long-running = EC2
- Event-driven small task = Lambda
- Docker containers = ECS / EKS
- Quick web app deploy = App Runner

TERA DAY 20: EC2 t3.micro use kiya (Spring Boot + MySQL + Redis)


## CATEGORY 2: STORAGE

```
S3   - object storage (files, images, backups, videos)
EBS  - block storage (attached to EC2 = disk)
EFS  - file storage (shared NFS, multi-EC2 access)
```

ANALOGY:
- S3   = Google Drive (object storage, infinite scale)
- EBS  = laptop HDD/SSD (attached to ONE EC2)
- EFS  = shared NFS folder (multiple EC2 same data)

TERA DAY 20: EBS 8 GiB gp3 attached to EC2 (default storage)


## CATEGORY 3: DATABASE

```
RDS         - managed SQL (MySQL, Postgres, MariaDB, Oracle, SQL Server)
DynamoDB    - managed NoSQL (key-value, like Redis but durable)
Aurora      - cloud-native SQL (3-5x faster than RDS)
ElastiCache - managed Redis / Memcached
```

"MANAGED" KA MATLAB:
- AWS = OS, backups, patching, replication, monitoring sab handle
- Tu = just connect + use
- vs self-host = cheaper but YOUR responsibility

WHEN MANAGED vs SELF-HOST:
- Self-host = cheaper, more control, more work
- Managed   = costlier, less work, production reliable

ROOM ANALOGY (Day 20 lesson):
- Self-host (Day 20): 1 room (EC2) = Spring + MySQL + Redis sab
  = Bheed, RAM fine but CPU credits crashed
- Production:
  - EC2          = sirf Spring
  - RDS          = sirf MySQL (separate)
  - ElastiCache  = sirf Redis (separate)
  = Each comfortable

PRICE:
- Self-host (Day 20):   t3.micro free tier — failed under load
- Managed (production): ~$60/month — but reliable


## CATEGORY 4: NETWORK

```
VPC          - private network in AWS
Route 53     - DNS service (name → IP resolver)
CloudFront   - CDN (cache content near users globally)
ALB / NLB    - load balancers (traffic distribute)
```

SIMPLE EACH:
- VPC          = private fence (koi external direct enter nahi)
- Route 53     = phone directory (name lookup → IP)
- CloudFront   = global cache (Mumbai user → Mumbai server)
- ALB          = receptionist (multiple servers traffic distribute)

TERA DAY 20:
- Default VPC use kiya (vpc-019c7092b58bb22a3)
  = AWS har account ko default VPC deta har region mein
- Security group: launch-wizard-1
  - SSH (TCP 22) from Anywhere   — login
  - Custom TCP 8080 from Anywhere — app access
- Auto-assign public IP: Enable → 13.127.249.108

ANALOGY:
- VPC             = private compound/society
- Security group  = compound ka guard with rule book
- Public IP       = compound ka official outside address


## CATEGORY 5: SECURITY

```
IAM            - users, roles, permissions ("kaun kya kar sakta")
Security Group - firewall at EC2 level (ports rules)
KMS            - encryption key management
Secrets Mgr    - passwords/API keys secure storage
WAF            - web application firewall (block bad requests)
```

SIMPLE EACH:
- IAM            = HR + access cards
- Security Group = building guard (which port khula)
- KMS            = vault keys manager
- Secrets Mgr    = locker for DB passwords (no hardcode)
- WAF            = bouncer with rules (SQL injection, bad IPs block)

TERA DAY 20 EVIDENCE:
- IAM           = root account use kiya (production = NAHI)
- Security Group = launch-wizard-1
- Secrets       = passwords hardcoded compose mein (production = Secrets Mgr)

PRODUCTION FIX:
- Day 20: root user + hardcoded password = development OK
- Production = IAM user + Secrets Manager mandatory
- Interview red flag if mention root account without context


## CATEGORY 6: MONITORING

```
CloudWatch    - metrics, logs, alarms (AWS ka monitoring stack)
CloudTrail    - audit log (kaun ne kya kiya, kab)
X-Ray         - distributed tracing (request journey)
SNS           - notification service (email/SMS alerts)
```

SIMPLE EACH:
- CloudWatch  = car dashboard (speed, fuel, temp)
- CloudTrail  = CCTV footage (every action timestamped)
- X-Ray       = X-ray of microservices (slow point dikhata)
- SNS         = bell/intercom (alarm → message bhejta)

TERA DAY 20 EVIDENCE:
- CloudWatch:
  - EC2 → Monitoring tab
  - CPUCreditBalance graph (credits trending to 0)
  - CPUUtilization graph (100% spike)
  = CPU starvation confirm hua

- Budget alert:
  - Zero-spend budget = SNS underneath email bhejta


## 6 CATEGORIES SUMMARY

```
1. Compute     - EC2, Lambda, ECS, EKS
2. Storage     - S3, EBS, EFS
3. Database    - RDS, DynamoDB, Aurora, ElastiCache
4. Network     - VPC, Route 53, CloudFront, ALB
5. Security    - IAM, Security Group, KMS, Secrets, WAF
6. Monitoring  - CloudWatch, CloudTrail, X-Ray, SNS
```


## FREE TIER REALITY

AWS bolta: "12 months free!"
Truth: marketing trap, production-grade NAHI.


WHAT'S ACTUALLY FREE (12 months):
```
EC2         - 750 hrs/month t2.micro OR t3.micro (1 instance 24x7)
S3          - 5 GB storage, 20K GET, 2K PUT
RDS         - 750 hrs db.t3.micro, 20 GB storage
Lambda      - 1M requests/month (always free)
CloudFront  - 1 TB out, 10M requests
```

WHAT'S ALWAYS FREE (no expiry):
```
Lambda      - 1M req + 400K GB-sec/month
DynamoDB    - 25 GB storage
CloudWatch  - 10 metrics, 10 alarms
```

HIDDEN TRAPS (BILL EXPLODE):
```
Data transfer OUT       - 1 GB/month free, beyond = paid
EBS snapshots           - charged separately
Elastic IP (unused)     - charged if not attached
NAT Gateway             - $32/month FROM DAY 1 (no free tier)
Public IPv4 (Feb 2024+) - $3.6/month per IP
```

TERA DAY 20 LEARNING:
- t3.micro = free tier, BUT burstable CPU
- Idle "free" easy
- Real Java app load = credits khatam = useless
- Free tier = TASTE TEST, not production


## ACCOUNT SAFETY BASICS

AWS = real money. Mistake = real bill.
Pehle safety nets, phir use.


SAFETY LAYER 1: BUDGET ALERT
   Path: Billing → Budgets → Create budget
   Type: Zero-spend budget
   Trigger: $0.01 spend = email turant
   = Tera Day 20 setup mein already ON


SAFETY LAYER 2: BILLING DASHBOARD (daily check)
   Path: Billing and Cost Management → Home
   Dekho:
   - Month-to-date charges
   - Forecast (kitna hoga month end tak)
   - Service-wise breakdown
   = "Roz subah 30 sec check" habit banao


SAFETY LAYER 3: REGION LOCK
   Hamesha 1 region use (Mumbai = ap-south-1)
   Different region mein chhupi resource = bill silent
   = Region switch karke dekho kabhi-kabhi (oversight catch)


SAFETY LAYER 4: ROOT vs IAM
   Root account = "owner" (full control, billing access)
   Production rule: Root use ONLY for:
   - Initial setup
   - Billing access
   - IAM user create
   Daily work = IAM user (limited permissions)
   = Root = "all-powerful key" — locker mein rakho


SAFETY LAYER 5: MFA (mandatory)
   Root account pe MFA enable
   Password leak ho bhi to MFA = second wall
   = Google Authenticator app use


SAFETY LAYER 6: DELETE WHEN DONE
   Tera Day 20 = EC2 + EBS + Elastic IP (if any)
   Terminate karne ke baad: Billing dashboard verify $0
   "Free tier mein hai" = false sense
   = Kuch services free tier ke baahar charge (data transfer, etc.)


## EC2 LAUNCH FLOW (UI walkthrough — Day 20)

AWS Console → Services → EC2 → Launch instance


STEP 1: NAME
   Name: todoapp
   = Just a label (rename anytime)


STEP 2: AMI (Amazon Machine Image)
   AMI = pre-built OS image
   Choices:
   - Amazon Linux 2023 (free tier)  ← tune liya
   - Ubuntu Server 22.04 (free tier)
   - RHEL, SUSE, Windows (paid)
   = AMI = "OS pre-installed disk image"


STEP 3: INSTANCE TYPE (server size)
   t3.micro = free tier (2 vCPU burstable, 1 GiB RAM)  ← tune liya
   t3.small = 2 GiB RAM (paid)
   t3.medium = 4 GiB RAM (paid)
   m5.large = production-grade (fixed CPU, 8 GiB)
   = Tera Day 20 trap: t3.micro burstable = Java app fail


STEP 4: KEY PAIR (SSH login)
   todoapp-key (RSA, .pem)
   Downloaded: C:\DSA_PRACTICE\todoapp-key.pem
   - Ek baar download = phir kabhi nahi
   - Lost = EC2 access GONE
   - .pem NEVER commit (gitignore mein hai)


STEP 5: NETWORK SETTINGS
   VPC: Default (vpc-019c7092b58bb22a3)
   Subnet: Default (auto-assigned)
   Auto-assign public IP: Enable  ← public access ke liye
   Security group: Create new (launch-wizard-1)
   Inbound rules:
   - SSH (TCP 22) from Anywhere     — login
   - Custom TCP 8080 from Anywhere   — app access


STEP 6: STORAGE
   8 GiB gp3 (free tier) ← default
   gp3 = general purpose SSD (newer/cheaper than gp2)
   Free tier limit: 30 GiB total EBS


STEP 7: LAUNCH
   Click "Launch instance"
   Wait 30-60 sec → State: Running
   Output:
   - Instance ID: i-032c273fe7c7e31dd
   - Public IP: 13.127.249.108


POST-LAUNCH LOCKED (cannot change):
   - AMI
   - Key pair
   - Region
   - VPC/subnet
   - Storage DECREASE

POST-LAUNCH CHANGEABLE:
   - Security group rules (add/remove ports)
   - Tags
   - IAM role
   - Storage INCREASE
   - Stop → change instance type → start (downtime)


## DAY 20 DEPLOYMENT RECAP (full pipeline)


STEP A: PUSH IMAGE TO DOCKER HUB (local laptop)
   docker login                                                    — Docker Hub auth
   docker tag todoapp-app:latest arpanmah/todoapp:latest
   docker push arpanmah/todoapp:latest                             — 595 MB upload
   = Image public: hub.docker.com/r/arpanmah/todoapp


STEP B: SSH INTO EC2 (Windows .pem fix needed)
   Error pehle: "UNPROTECTED PRIVATE KEY"
   Fix:
      icacls todoapp-key.pem /inheritance:r
      icacls todoapp-key.pem /grant:r "%username%:R"
   Phir:
      ssh -i todoapp-key.pem ec2-user@13.127.249.108


STEP C: INSTALL DOCKER ON EC2
   sudo yum update -y
   sudo yum install -y docker
   sudo systemctl start docker
   sudo systemctl enable docker           — auto-start on reboot
   sudo usermod -aG docker ec2-user       — sudo bina chalane ke liye
   exit                                   — logout (group apply ho)
   ssh -i todoapp-key.pem ec2-user@13.127.249.108


STEP D: INSTALL DOCKER COMPOSE
   sudo curl -L "https://github.com/docker/compose/releases/latest/download/docker-compose-linux-x86_64" -o /usr/local/bin/docker-compose
   sudo chmod +x /usr/local/bin/docker-compose
   docker-compose version                 — verify


STEP E: CREATE docker-compose.yml ON EC2
   Failed:
   - nano paste = auto-indent corrupted YAML
   - heredoc = EOF terminator alignment issue
   Success:
   - Local file (docker-compose.deploy.yml) banaya
   - VS Code Ctrl+A → Ctrl+C
   - EC2 nano → right-click paste → Ctrl+O → Ctrl+X
   Validate: docker-compose config


STEP F: DEPLOY
   docker-compose up -d
   3 services started:
   - redis container
   - todoapp-mysql container (healthy)
   - todoapp container


STEP G: EXTERNAL TEST FAIL (ECONNREFUSED)
   Postman: POST http://13.127.249.108:8080/auth/register
   Error: connect ECONNREFUSED
   Initial misdiagnosis: OOM (RAM full)
   Real diagnosis:
   - free -h  → 916M total, 173M used, 606M FREE = RAM OK
   - docker logs todoapp → "Thread starvation detected"
   = CPU credits exhausted (t3.micro burstable trap)


LESSON STACK:
   1. Free tier = TASTE, not production
   2. t3.micro burstable = Java workload undersized
   3. RAM vs CPU diagnosis = always BOTH check
   4. "Container Started" NOT EQUAL "App Ready to serve"
   5. Senior fix = managed services (RDS, ElastiCache) free EC2 load


HIKARICP CLARIFICATION:
   HikariCP = JDBC connection pool manager (MySQL connections handle karta)
   Day 20 message: "Thread starvation or clock leap detected (housekeeper delta=47s)"
   = HikariCP internal threads ko CPU mil nahi raha
   = Connection pool maintenance thread (housekeeper) 47 sec lag se chala
   = Underlying problem CPU, but HikariCP ne report kiya
   = JDBC layer ki har dikkat HikariCP logs mein dikhti (messenger + manager)


INTERVIEW NARRATIVE:
   "Deployed Spring + MySQL + Redis stack to AWS EC2 t3.micro
    free tier. Hit burstable CPU credit exhaustion during JVM
    warmup + Hibernate schema init. Diagnosed via docker logs
    (HikariCP thread starvation) and free -h (ruled out OOM).
    Real fix = t3.medium+ for fixed CPU OR offload MySQL/Redis
    to RDS/ElastiCache."


## USEFUL AWS CLI COMMANDS

CLI = console UI ka command-line equivalent
Same actions, scripts mein use ho sakte


SETUP (one-time):
   aws configure
   = Asks: Access Key ID, Secret Key, Region, Output format
   = Creates ~/.aws/credentials + ~/.aws/config


EC2 COMMANDS:
   aws ec2 describe-instances                          — list all EC2
   aws ec2 describe-instances --instance-ids i-xxx     — specific one
   aws ec2 start-instances --instance-ids i-xxx        — start stopped
   aws ec2 stop-instances --instance-ids i-xxx         — stop running
   aws ec2 terminate-instances --instance-ids i-xxx    — DELETE (irreversible)
   aws ec2 describe-security-groups                    — firewall list


S3 COMMANDS:
   aws s3 ls                                           — list buckets
   aws s3 ls s3://my-bucket                            — list contents
   aws s3 cp file.txt s3://my-bucket/                  — upload
   aws s3 cp s3://my-bucket/file.txt ./                — download
   aws s3 sync ./local s3://my-bucket                  — folder sync
   aws s3 rb s3://my-bucket --force                    — delete bucket


IAM COMMANDS:
   aws iam list-users                                  — all users
   aws iam create-user --user-name dev                 — create user
   aws iam attach-user-policy --user-name dev \
       --policy-arn arn:aws:iam::aws:policy/ReadOnlyAccess
                                                       — give read access


BILLING:
   aws ce get-cost-and-usage \
       --time-period Start=2026-05-01,End=2026-05-25 \
       --granularity DAILY --metrics UnblendedCost
   = Daily spend dekho via CLI


CLI vs CONSOLE — WHEN WHICH:
   Console (UI)  = exploration, learning, one-off
   CLI           = scripts, automation, CI/CD pipelines
   Terraform/CDK = infrastructure-as-code (production)
   = Senior engineer rarely uses console daily
   = Console = "tourist mode", CLI/IaC = "resident mode"


## COST DISCIPLINE RULES (FinOps signal)

FinOps = Finance + DevOps
= Cloud spend optimization = senior engineering responsibility
= JP/GS/BlackRock = cost-conscious culture


RULE 1: DELETE WHEN DONE
   Day 20 stack chala → done → TERMINATE
   "Kal phir start karunga" = NO
   = Habit: experiment done = same minute delete


RULE 2: STOP vs TERMINATE
   STOP      = container "paused" (EBS storage CHARGE jaari)
   TERMINATE = instance + EBS gone (zero charge)
   = Practice mein: TERMINATE
   = Production mein: STOP (instance preserve, restart fast)


RULE 3: TAGGING (cost attribution)
   Every resource ko tag karo:
   - Project: todoapp
   - Environment: dev / staging / prod
   - Owner: arpan
   = Billing dashboard "Project-wise cost" filter
   = Production: tags = mandatory (audit + cost allocation)


RULE 4: RIGHT-SIZING
   Don't over-provision
   = CloudWatch metrics → real usage → resize
   = AWS Compute Optimizer = recommendation tool free


RULE 5: RESERVED INSTANCES (production)
   Production server 24x7 = Reserved Instance buy
   1-year commit = 30-40% discount
   3-year commit = 50-60% discount
   = "Predictable workload = commit = save"


RULE 6: SPOT INSTANCES (batch/non-critical)
   Spot = AWS ka spare capacity (60-90% cheaper)
   Catch: AWS 2-min notice mein kabhi bhi le sakta
   Use cases: batch jobs, ML training, CI runners
   = NOT for production DB / always-on services


RULE 7: COST ALERTS (multi-level)
   Zero-spend budget already (Day 20)
   Add:
   - $5 warning
   - $10 critical
   - $20 stop-everything panic


RULE 8: REGION CHOICE = COST
   us-east-1 (N. Virginia) = cheapest (oldest, biggest)
   ap-south-1 (Mumbai)     = mid-range
   sa-east-1 (Brazil)      = costly
   = Production = latency vs cost trade-off


INTERVIEW LINE:
   "Cloud cost is a senior engineering responsibility,
    not just finance team's. Right-sizing, lifecycle
    management, tagging discipline = engineering culture."


## KEY TAKEAWAYS

1. AWS = 6 categories (Compute, Storage, DB, Network, Security, Monitoring)
2. Free tier = TASTE TEST, not production reality
3. t3 burstable = unreliable for sustained load
4. Memory diagnosis NOT EQUAL CPU diagnosis (always both)
5. HikariCP = JDBC pool, messenger of any DB/CPU issue
6. Managed services (RDS, ElastiCache) = free EC2 compute
7. Root account = locker (use IAM daily)
8. Delete when done (cost discipline)
9. CLI/IaC = senior engineer's daily mode (not console)
10. FinOps = engineering responsibility, not just finance


## ANALOGIES INDEX

- AWS overall     = hotel room (vs ghar = owning datacenter)
- 6 categories    = mall (200+ shops, pick what you need)
- Self vs managed = ek room sab bhare vs alag rooms har service
- VPC             = private compound/society
- Security Group  = compound guard with rule book
- Public IP       = compound's outside address
- IAM             = HR + access cards
- KMS             = vault keys manager
- Secrets Manager = locker for DB passwords
- WAF             = bouncer with rules
- CloudWatch      = car dashboard
- CloudTrail      = CCTV footage
- X-Ray           = X-ray of microservices
- SNS             = bell/intercom (alarm trigger)
- Root account    = all-powerful master key (locker mein)
- HikariCP        = messenger + manager (JDBC pool)

