# HTTP vs HTTPS

---

## 1-Line Analogy

```
HTTP  = Postcard      (anyone in postal chain can read)
HTTPS = Sealed letter (only recipient can open)
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  HTTP             │  HTTPS            │
├──────────────────┼──────────────────┼──────────────────┤
│ Encryption       │ Plain text     │ TLS encrypted  │
├──────────────────┼──────────────────┼──────────────────┤
│ Port             │ 80                 │ 443               │
├──────────────────┼──────────────────┼──────────────────┤
│ URL prefix       │ http://            │ https://          │
├──────────────────┼──────────────────┼──────────────────┤
│ Certificate      │ None            │ SSL cert (CA)  │
├──────────────────┼──────────────────┼──────────────────┤
│ Speed            │ Slightly faster   │ Slightly slower   │
│                  │ No handshake       │ TLS handshake     │
├──────────────────┼──────────────────┼──────────────────┤
│ MITM risk        │ Vulnerable      │ Protected      │
├──────────────────┼──────────────────┼──────────────────┤
│ SEO ranking      │ Lower           │ Higher (Google)│
├──────────────────┼──────────────────┼──────────────────┤
│ Browser warning  │ "Not Secure"   │ Lock icon      │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## WHY HTTP IS DANGEROUS

```
Scenario: Coffee shop WiFi pe HTTP banking

   YOU                          ATTACKER (same WiFi)
    │                                │
    │ Login: user/password           │
    │ ────────────────────►          │
    │           ◄──── READS PLAIN TEXT
    │                                │
    │ Bank: balance, transactions    │
    │ ◄──────────────────            │
    │           ◄──── READS ALL DATA

   Wifi sniffer reads:
      • Login credentials
      • Cookies / session tokens
      • All form data
      • All response data

   MITM attack:
      • Inject malicious JS
      • Modify response
      • Redirect to phishing
```

---

## HOW HTTPS PROTECTS

```
   YOU                              ATTACKER (same WiFi)
    │                                │
    │ [encrypted bytes]              │
    │ ────────────────────►          │
    │           ◄──── sees: gibberish
    │                                │
    │ [encrypted bytes]              │
    │ ◄──────────────────            │
    │           ◄──── sees: gibberish

   Encryption = symmetric (AES) after handshake
   Keys never travel plain
   Tampering detectable (HMAC)
```

---

## TLS HANDSHAKE (HTTPS Setup)

```
   CLIENT                          SERVER
      │                                │
      │  1. ClientHello                │
      │  ───────────────────────────►│  "I support TLS 1.3,
      │     (TLS version, ciphers)    │   here are my ciphers"
      │                                │
      │  2. ServerHello + Cert + key   │
      │  ◄───────────────────────────│  "Use TLS 1.3,
      │     (chosen cipher, cert)     │   here's my cert + public key"
      │                                │
      │  3. Verify cert with CA        │  
      │     Trusted (or block)   │
      │                                │
      │  4. Generate session key       │
      │  ───────────────────────────►│  "Encrypted with your public key,
      │     (encrypted with pub key)  │   only you can decrypt"
      │                                │
      │  ===== SECURE CHANNEL =====   │
      │                                │
      │  All data: AES-encrypted       │
      │  ◄──────────────────────────►│
```

---

## CERTIFICATE FLOW

```
   Website (google.com) wants HTTPS
        │
        ▼
   Apply to CA (DigiCert, Let's Encrypt)
        │
        ▼
   CA verifies "you actually own google.com"
        │
        ▼
   CA issues SIGNED certificate
        │
        ▼
   Server presents cert to clients
        │
        ▼
   Browser checks: "Is this signed by trusted CA?"
        │
        ▼
   Yes → trust + connect
   No → "Your connection is not private" warning
```

---

## WHEN ABSOLUTELY USE HTTPS

```
ALWAYS in 2026 — HTTP basically dead:

Banking / finance
Login pages
E-commerce (payment)
Personal info forms
Admin panels
APIs (anything sensitive)
Cookies / sessions
Public websites (SEO ranking)

HTTP ok only for:
   • Internal localhost dev
   • Old legacy intranet
   • Specific test environments
```

---

## BONUS — HTTPS Versions

```
TLS evolution (HTTPS = HTTP + TLS):

   SSL 2.0  →  DEAD (insecure)
   SSL 3.0  →  DEAD (POODLE attack)
   TLS 1.0  →  Deprecated 2020
   TLS 1.1  →  Deprecated 2020
   TLS 1.2  →  Still common
   TLS 1.3  →  Latest, faster, more secure
   
   Modern apps target TLS 1.2+
```

---

## INTERVIEW LINE

```
"HTTPS is HTTP over TLS — adds encryption, integrity,
 and server authentication.

 TLS handshake exchanges cipher suites and certificates.
 Server proves identity via CA-signed cert.
 Symmetric session key established for encrypted data.

 Without HTTPS, plain HTTP is readable on any shared
 network — login credentials, cookies, all visible.

 In 2026, HTTPS is default; HTTP only for localhost dev."
```

---

## Memory Hook

```
HTTP  = Postcard (everyone reads)
HTTPS = Sealed letter (only recipient)

HTTPS = HTTP + TLS (encryption layer)
Port: 80 (HTTP), 443 (HTTPS)

TLS handshake:
   ClientHello → ServerHello + Cert → Key exchange → Encrypted channel

CA = Certificate Authority (DigiCert, Let's Encrypt)
TLS 1.3 = current standard
```

[← HLD README](../README.md)
