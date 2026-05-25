# AWS — Complete Revision Reference


## WHAT + WHY


### What AWS is
Amazon Web Services — cloud computing platform.
~200 services across compute + storage + DB + network + ML.
Industry default (~33% cloud market share).


### Why exist
Pre-cloud era:
- Buy + rack physical servers
- Capacity guess (over-provision waste OR under-provision crash)
- Months to scale
- Hardware maintenance pain
- Data center management

AWS fix:
- Rent on-demand
- Per-hour/per-second billing
- Auto-scale
- Global regions (Mumbai, US, EU, etc.)
- Managed services (no infra babysit)


### Origin story
- 2003: Chris Pinkham + Benjamin Black wrote vision paper
- 2006: Andy Jassy led team — public launch
  - S3 first (March 2006)
  - EC2 next (August 2006)
- 2026: AWS revenue > Amazon retail profit
- = "AWS = the real Amazon money-maker"


### Key people
- Jeff Bezos    = sponsor + vision umbrella
- Andy Jassy    = real architect (now Amazon CEO)
- Chris Pinkham + Benjamin Black = engineering origin


## CORE CATEGORIES + SERVICES


### Compute (kahaan code chalega)
- EC2          — virtual servers (raw VM)
- Lambda       — serverless functions (no server manage)
- ECS          — container orchestration (Docker on AWS)
- EKS          — managed Kubernetes
- App Runner   — simplest auto-deploy


### Storage (data kahaan rahega)
- S3           — object storage (files, images, backups)
- EBS          — block storage (attached to EC2 disk)
- EFS          — file storage (shared NFS)


### Database (structured data)
- RDS          — managed SQL (MySQL, Postgres, etc.)
- DynamoDB     — managed NoSQL (key-value)
- Aurora       — cloud-native SQL (3-5x faster than RDS)
- ElastiCache  — managed Redis / Memcached


### Networking
- VPC          — private network in AWS
- Route 53     — DNS service
- CloudFront   — CDN (cache content near users)
- ALB / NLB    — load balancers


### Security
- IAM             — users + permissions + policies
- KMS             — encryption keys
- Secrets Manager — passwords + tokens


### Monitoring
- CloudWatch   — logs + metrics + alarms
- X-Ray        — distributed tracing


## ACCOUNT + SAFETY BASICS


### Account setup
- Free Plan (6 months, $200 credits, free tier 12 months)
- Primary purpose: Personal use
- Ownership: Individual
- Card verification ~₹2 hold (refundable)
- Spending limit ~₹15000 (authorization, NOT auto-charge)


### Region change
- Default region: Stockholm (eu-north-1) for some accounts
- Change to: Asia Pacific (Mumbai) — ap-south-1
- Top-right region dropdown
- Lower latency + data transfer cost for India


### Budget alert (safety net)
Path: Billing & Cost Management → Budgets → Create budget
- Template: Zero-spend budget ($0.01 alert)
- Email recipient: your personal email
- Result: Any spend > $0.01 = instant email alert


### Root user vs IAM user
- Root user = signup-time creation = unlimited access
  - High risk if compromised
  - Only billing + account-level work
  - Enable MFA always
- IAM user = daily ops (limited permissions)
  - MFA enabled
  - Least-privilege policies


### Cost discipline rules
1. Daily billing dashboard check
2. Delete unused resources immediately
3. Zero-spend budget always on
4. Region locked Mumbai
5. .pem in .gitignore (never leak SSH keys)
6. Cancel GPay autopay if AWS used briefly


## EC2 (key compute service)


### What EC2
Elastic Compute Cloud
- Virtual server rented per-hour
- Choose AMI (OS image), instance type (CPU/RAM)
- Public + private IP assigned
- SSH access via key pair


### Launch flow (UI)
1. EC2 → Launch instance
2. Name
3. AMI: Amazon Linux 2023 (free tier eligible)
4. Instance type: t3.micro (free tier, 1 GB RAM, 2 vCPU burstable)
5. Key pair: create new (.pem download)
6. Security group: inbound rules (SSH 22, app port)
7. Storage: 8 GB gp3 default
8. Launch


### Burstable CPU (t3 family trap)
- t3 = burstable (idle accumulate credits, busy burn)
- Credits = 0 → throttled to ~10% baseline CPU
- Heavy startup (JVM warmup, MySQL init) = burns credits fast
- Then thread starvation kicks in
- = Tera Day 20 root cause


### SSH to EC2
```
ssh -i todoapp-key.pem ec2-user@<public-ip>
```

Windows .pem permission fix (UNPROTECTED PRIVATE KEY error):
```
icacls todoapp-key.pem /inheritance:r
icacls todoapp-key.pem /grant:r "%username%:R"
```


### Inside EC2 (Amazon Linux 2023)
```
sudo yum update -y                 - system update
sudo yum install -y docker         - install Docker
sudo systemctl start docker        - start daemon
sudo systemctl enable docker       - auto-start on reboot
sudo usermod -aG docker ec2-user   - skip sudo for docker
exit + relogin                     - group change effective
```


### Docker Compose binary install
```
sudo curl -L "https://github.com/docker/compose/releases/latest/download/docker-compose-linux-x86_64" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
docker-compose version
```


## DOCKER HUB INTEGRATION


### Image distribution
- Docker Hub = public registry (free for public)
- Push from laptop, pull on EC2

```
docker login
docker tag todoapp-app:latest <username>/todoapp:latest
docker push <username>/todoapp:latest
```

Then EC2's docker-compose.yml uses `image: <username>/todoapp:latest`.


## DAY 20 REAL DEPLOY FLOW


### Steps recap
1. AWS account + region + budget alert
2. Push image to Docker Hub
3. EC2 launch (t3.micro, security group SSH+8080)
4. SSH connect (icacls Windows fix)
5. Docker install on EC2
6. Docker Compose binary install
7. Create docker-compose.yml (nano paste — use local file copy)
8. docker-compose up -d → all containers started
9. Postman test → FAILED (ECONNREFUSED)
10. Diagnose → CPU starvation, NOT OOM


### Diagnostic commands
```
docker ps                        - running containers
free -h                          - RAM check (proves OOM or not)
docker logs todoapp --tail 30    - app logs
docker stats                     - per-container live
top                              - process-level CPU
uptime                           - load average
```


### Key diagnostic combo
```
docker ps + free -h + docker logs todoapp
= 3-command CPU starvation detection
```

Smoking gun: `HikariPool — Thread starvation or clock leap detected`


## CLOUDWATCH (monitoring)


### Default metrics (free)
- CPUUtilization
- NetworkIn / NetworkOut
- DiskRead / DiskWrite
- StatusCheckFailed


### t3-specific (key for burstable trap)
- CPUCreditBalance       — credits remaining (0 = throttled)
- CPUCreditUsage         — burn rate
- CPUSurplusCreditBalance — overdraft


### Memory? NOT default
- AWS sees outside, can't peek guest OS
- Need: CloudWatch agent install inside EC2
- Then memory pushed as custom metric


## CLEANUP / TERMINATE


### Terminate EC2
- AWS Console → EC2 → Instances → select
- "Instance state" → Terminate instance
- Confirm
- Wait ~2 min for "Terminated" state


### Force terminate (if stuck Shutting-down)
- Same dropdown → "Force terminate (delete) instance"
- Bypasses graceful shutdown


### Verify $0 cost
- Billing dashboard → Cost summary
- Free plan banner: "Credits cover your free plan costs"
- Volumes (EBS) auto-deleted with instance (default)
- Cancel GPay autopay (defense layer)


## FREE TIER REALITY (interview gold)


### Marketing vs reality
- Designed for: static sites, single tiny apps, learning
- NOT designed for: Java backend + MySQL + Redis (memory hungry)
- 1 GB RAM + burstable CPU = "Hello World" only


### Production minimums
- t3.medium (4 GB, baseline 40% CPU) = ~$30/month
- RDS db.t3.micro = ~$15/month
- ElastiCache cache.t3.micro = ~$15/month
- ALB = ~$18/month
- NAT Gateway = ~$30/month (sneaky)
- Data transfer ~$5/month
- = Base ~$110/month for small startup stack


### Senior architect thinking
- Right-size instances
- Reserved Instances (40% off for 1-year commitment)
- Spot Instances (batch jobs, 70-90% cheaper)
- Offload stateful services (RDS, ElastiCache) — free EC2 compute
- FinOps as discipline


## INTERVIEW GOLD LINES


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


### 4. UI vs CLI vs IaC progression
"First deploy via UI for orientation, then scripted via
 AWS CLI, production-style with Terraform for repeatability."


## ERRORS ENCOUNTERED + FIXES (Day 20)

1. UNPROTECTED PRIVATE KEY (Windows .pem too open) → icacls fix
2. YAML parse error in nano paste → use local file copy
3. Heredoc EOF didn't terminate → abort + retry strategy
4. SSH Ctrl+C disconnect → reconnect via ssh
5. ECONNREFUSED external → CPU starvation (NOT OOM as assumed)


## KEY TAKEAWAYS

1. Free tier = marketing taste, not realistic production
2. t3 burstable family = unreliable for sustained load
3. Memory diagnosis ≠ CPU diagnosis (always check both)
4. Container "Started" ≠ App "Ready to serve"
5. AWS managed services (RDS, ElastiCache) free EC2 compute load
6. FinOps = senior engineering concern
7. UI = exploration, CLI = production, IaC = top-tier


## ANALOGIES INDEX

- AWS overall            = rent vs buy servers (cloud lease model)
- EC2                    = rented virtual server (per-hour rate)
- t3 burstable           = credit-card with spending cap (burn fast, throttle)
- Free tier              = first-class lounge access (limited, then pay)
- IAM root vs IAM user   = building owner vs employee with access card
