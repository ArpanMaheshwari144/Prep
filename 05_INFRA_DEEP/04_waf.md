# WAF — Web Application Firewall

---

## 1 Problem Without WAF

```
   ATTACKER                  YOUR APP

        │                          │
        │  SQL Injection:           │
        │  ?id=1' OR '1'='1         │
        │ ─────────────────────────►│ DB dumped
        │                          │
        │  XSS:                      │
        │  <script>steal()</script>  │
        │ ─────────────────────────►│ cookies stolen
        │                          │
        │  CSRF, brute force, bots   │
        │ ─────────────────────────►│ chaos

Problems:
   Direct attacks reach app
   Single bug = full compromise
   App-level checks not enough
```

---

## 2 Bouncer Analogy

```
Fancy nightclub:
   100 log andar jaana chahte

   Without bouncer:
      Sab andar — drunks, weapons, troublemakers
      → Chaos inside

   With BOUNCER :
      Har person scan kare entry pe
      Rules:
         No weapons
         No fake ID
         No drunks
         Dress code violators out
         VIP list = direct entry
      Suspicious behavior → block

   = Bouncer = WAF
   = Club = Your app
   = Visitors = HTTP requests
```

---

## 3 WAF Placement

```
   USER / ATTACKER
        │
        ▼
   ┌─────────────────┐
   │     WAF         │ ← Inspects EVERY request
   │  (rule engine)  │      Blocks malicious
   └────────┬────────┘
            │ clean traffic
            ▼
   ┌─────────────────┐
   │  LOAD BALANCER  │
   └────────┬────────┘
            │
            ▼
   ┌─────────────────┐
   │   APP SERVERS    │
   └─────────────────┘

   = WAF SITS IN FRONT
   = Attack stops here, never reaches app
```

```
Real deployment order:
   Internet → DNS → CDN → WAF → LB → Servers → DB
```

---

## 4 OWASP Top 10 (KEY KNOWLEDGE)

```
OWASP = Open Worldwide Application Security Project
Top 10 = Most common web attacks
WAF defends against most of these
```

```
┌────┬─────────────────────────────────────────────┐
│ #  │  Attack                                       │
├────┼─────────────────────────────────────────────┤
│ 1  │ Broken Access Control                         │
│    │ (User accesses admin pages)                   │
├────┼─────────────────────────────────────────────┤
│ 2  │ Cryptographic Failures                        │
│    │ (Weak encryption, plain text passwords)       │
├────┼─────────────────────────────────────────────┤
│ 3  │ INJECTION (SQL/NoSQL/Command)                 │
│    │ ?id=1' OR '1'='1                              │
│    │ WAF: blocks pattern                         │
├────┼─────────────────────────────────────────────┤
│ 4  │ Insecure Design                                │
├────┼─────────────────────────────────────────────┤
│ 5  │ Security Misconfiguration                      │
├────┼─────────────────────────────────────────────┤
│ 6  │ Vulnerable Components (old libraries)          │
├────┼─────────────────────────────────────────────┤
│ 7  │ Identification + Auth Failures (brute force)   │
│    │ WAF: rate limits                            │
├────┼─────────────────────────────────────────────┤
│ 8  │ Software + Data Integrity Failures             │
├────┼─────────────────────────────────────────────┤
│ 9  │ Logging + Monitoring Failures                  │
├────┼─────────────────────────────────────────────┤
│ 10 │ Server-Side Request Forgery (SSRF)             │
│    │ WAF: detects internal IP calls               │
└────┴─────────────────────────────────────────────┘
```

---

## 5 Common Attacks WAF Blocks

```
SQL INJECTION
   Attack: ?id=1' UNION SELECT password FROM users--
   WAF rule: pattern match SQL keywords in URL params

XSS (Cross-Site Scripting)
   Attack: <script>document.cookie</script>
   WAF rule: detect <script> tags in input

CSRF (Cross-Site Request Forgery)
   Attack: Tricks user into action via 3rd party site
   WAF rule: validate origin/referer headers

PATH TRAVERSAL
   Attack: /api/file?path=../../etc/passwd
   WAF rule: detect "../" sequences

COMMAND INJECTION
   Attack: ?cmd=ls;rm -rf /
   WAF rule: shell metacharacters block

BAD BOTS / SCRAPING
   Attack: 10,000 requests/min from bot
   WAF rule: rate limit + User-Agent check

DDoS (Layer 7)
   Attack: Flood app with valid-looking requests
   WAF rule: rate limit per IP, behavioral analysis

ZERO-DAY (with managed rules)
   New vulnerability discovered → WAF rules updated
   = Defense before app patched
```

---

## 6 How WAF Detects

```
3 DETECTION TECHNIQUES:
```

```
1. SIGNATURE-BASED (most common)
   Known attack patterns matched

   Example rule:
      "If URL contains 'UNION SELECT' → BLOCK"
      "If body contains '<script>' → BLOCK"

   Fast, catches known attacks
   Misses zero-days
```

```
2. ANOMALY-BASED (behavior)
   Normal traffic baseline
   Suspicious deviations flagged

   Example:
      Normal: 10 req/min per user
      Anomaly: 1000 req/min from one IP → BLOCK

   Catches new attacks
   False positives possible
```

```
3. CUSTOM RULES (your business logic)
   You define rules specific to your app

   Example:
      "Block all requests to /admin from non-corporate IPs"
      "Require API key header on /api/*"

   Tailored security
   Manual maintenance
```

---

## 7 Rule Types

```
┌──────────────┬──────────────────────────────────┐
│  Rule Type    │  How                              │
├──────────────┼──────────────────────────────────┤
│  ALLOW LIST   │ Only these allowed, rest blocked  │
│  (whitelist)  │ Strictest — corporate VPN apps    │
├──────────────┼──────────────────────────────────┤
│  DENY LIST    │ These blocked, rest allowed       │
│  (blacklist)  │ Most common — known bad IPs/agents│
├──────────────┼──────────────────────────────────┤
│  RATE LIMIT   │ Max X req per Y time per IP/user  │
│               │ Brute force / DDoS protection     │
├──────────────┼──────────────────────────────────┤
│  GEO BLOCK    │ Block specific countries          │
│               │ Compliance / fraud prevention      │
├──────────────┼──────────────────────────────────┤
│  IP REPUTATION│ Known bad IPs (threat intelligence)│
│               │ Auto-updated lists                 │
├──────────────┼──────────────────────────────────┤
│  CAPTCHA      │ Challenge suspected bots           │
│               │ Solved = allow, fail = block       │
└──────────────┴──────────────────────────────────┘
```

---

## 8 WAF + RATE LIMITER Integration

```
WAF mein Rate Limiter INTEGRATED hota hai.

┌─────────────────────────────────────┐
│   WAF — LAYERED DEFENSE              │
│                                       │
│   Layer 1: IP REPUTATION             │
│      Known bad IP → block instantly  │
│                                       │
│   Layer 2: RATE LIMITER               │
│      Too many requests → throttle    │
│      (token bucket / sliding window)  │
│                                       │
│   Layer 3: SIGNATURE MATCH            │
│      SQL/XSS patterns → block        │
│                                       │
│   Layer 4: ANOMALY DETECTION          │
│      Suspicious behavior → challenge │
│                                       │
│   Layer 5: GEO/CUSTOM RULES           │
│      Business-specific filters       │
│                                       │
│   ───────────────────────────────    │
│   Passed all 5? → forward to LB      │
└─────────────────────────────────────┘
```

```
= Rate Limiter concept (12_rate_limiter.md) USE hota WAF mein
= WAF aur Rate Limiter alag bhi ho sakte hain
  ya WAF mein bundled
```

---

## 9 Cloud WAFs

```
┌─────────────────┬──────────────────────────────┐
│  Provider       │  Strength                     │
├─────────────────┼──────────────────────────────┤
│ AWS WAF          │ Tight AWS integration         │
│                  │ ALB / API Gateway / CloudFront│
├─────────────────┼──────────────────────────────┤
│ Cloudflare WAF   │ Free tier, easy setup         │
│                  │ Global edge                    │
├─────────────────┼──────────────────────────────┤
│ Akamai Kona     │ Enterprise grade              │
├─────────────────┼──────────────────────────────┤
│ Azure WAF        │ For Azure apps                │
├─────────────────┼──────────────────────────────┤
│ Imperva          │ Specialized security vendor   │
└─────────────────┴──────────────────────────────┘
```

```
Managed RULESETS (pre-built):
   • OWASP Top 10 protection
   • SQL injection rules
   • XSS rules
   • Bot mitigation
   • DDoS protection

   Plug-and-play security
   Auto-updated as new threats emerge
```

---

## False Positives (THE CHALLENGE)

```
PROBLEM:
   WAF blocks legitimate requests by mistake

   Example:
      User comments: "I love your SELECT *FROM products page!"
      WAF detects "SELECT *FROM" → blocks comment
      = Legitimate user pissed
```

```
SOLUTION — Tuning Process:

   1. COUNT mode (audit, not block)
      WAF logs what it WOULD block
      No actual blocking

   2. Review false positives
      Are these real attacks or noise?

   3. Add exceptions
      "Allow SELECT in /comments endpoint"

   4. Switch to BLOCK mode
      Now blocks confidently

   5. Continuous monitoring
      New patterns → tune rules
```

---

## 11 WAF Deployment Modes

```
1. REVERSE PROXY MODE
   WAF dedicated server in front
   All traffic routes through it
   Full control
   Additional infrastructure

2. CLOUD WAF (most common today)
   Cloudflare / AWS WAF
   DNS points to WAF, WAF forwards to origin
   Zero infra
   Global edge
   Vendor lock-in

3. EMBEDDED (in app/web server)
   ModSecurity for Nginx/Apache
   Runs as plugin
   Tight integration
   Server resources used

4. HOST-BASED
   On each app server
   Distributed
   Maintenance overhead
```

---

## INTERVIEW LINE

```
"WAF is a Layer 7 firewall sitting in front of web apps.

 Inspects every HTTP request and blocks malicious ones
 — SQL injection, XSS, CSRF, path traversal, bad bots,
 OWASP Top 10 attacks.

 Detection: signature-based (known patterns),
 anomaly-based (behavioral), and custom rules.

 Integrates with rate limiter for layered defense
 — IP reputation → rate limit → signature → anomaly.

 Cloud WAFs (AWS WAF, Cloudflare) offer managed
 rulesets with auto-updates against new threats.

 Key challenge: false positives — tuned via count mode
 first, then block mode."
```

---

## Memory Hooks

```
WAF = Nightclub bouncer for HTTP requests

PLACEMENT:
   Internet → DNS → CDN → WAF → LB → App → DB

3 DETECTION:
   Signature (pattern match)
   Anomaly (behavior)
   Custom (your rules)

OWASP TOP 10 — WAF defends against most:
   #3 Injection (SQL, NoSQL)
   #7 Auth failures (brute force)
   #10 SSRF

LAYERED DEFENSE:
   IP rep → Rate limit → Signature → Anomaly → Custom

CLOUD WAFs: AWS WAF, Cloudflare, Akamai

CHALLENGE: False positives → COUNT then BLOCK
```

[← HLD README](../README.md)
