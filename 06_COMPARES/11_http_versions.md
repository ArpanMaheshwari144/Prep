# ⚡ HTTP/1 vs HTTP/2 vs HTTP/3

---

## 🎯 1-Line Analogy

```
HTTP/1.0 = 📞 Call, hang up, call again (per request)
HTTP/1.1 = 📞 Stay on line, but one-by-one queries
HTTP/2   = 📱 Multiple WhatsApp chats (multiplexed, parallel)
HTTP/3   = ⚡ WhatsApp call (UDP, super fast reconnect)
```

---

## 📊 Side by Side

```
┌──────────────┬──────────┬──────────┬──────────┬──────────┐
│  Feature     │ HTTP/1.0 │ HTTP/1.1 │ HTTP/2   │ HTTP/3   │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ Year         │ 1996     │ 1997     │ 2015     │ 2022     │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ Transport    │ TCP      │ TCP      │ TCP      │ QUIC/UDP │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ Format       │ Text     │ Text     │ Binary   │ Binary   │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ Connection   │ Per req  │ Persistent│Persistent│ 0-RTT     │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ Multiplexing │ ❌       │ ❌ (pipe)│ ✅       │ ✅ (better)│
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ Header comp  │ ❌       │ ❌       │ ✅ HPACK │ ✅ QPACK  │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ Server push  │ ❌       │ ❌       │ ✅        │ ❌ (drop) │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ HoL blocking │ Bad      │ Bad      │ TCP-level│ ✅ NONE   │
├──────────────┼──────────┼──────────┼──────────┼──────────┤
│ Encryption   │ Optional │ Optional │ Practical│ Mandatory │
└──────────────┴──────────┴──────────┴──────────┴──────────┘
```

---

## 🐌 HTTP/1.0 — New connection per request

```
Browser wants 3 files (HTML + CSS + JS):

   Connection 1 → GET /index.html → response → CLOSE
   Connection 2 → GET /style.css   → response → CLOSE
   Connection 3 → GET /app.js      → response → CLOSE

= TCP handshake EACH time (slow!)
= Lots of overhead
= Modern web = unusable
```

---

## 🚶 HTTP/1.1 — Persistent connections (1997 dominant)

```
ONE connection, sequential requests:

   Connection established
      ↓
   GET /index.html → response
   GET /style.css   → response
   GET /app.js      → response
      ↓
   Connection stays open

✅ Better than 1.0
❌ One-at-a-time still (queued)
❌ Head-of-line blocking:
   If style.css slow, app.js waits

PIPELINING (rare in practice):
   Send all 3 requests at once
   Receive responses in order
   ❌ Still blocked if response 1 slow
   ❌ Broken in many proxies
```

---

## 🚀 HTTP/2 — Multiplexing (2015 revolution)

```
ONE connection, MULTIPLE streams in parallel:

   Connection
      ├─ Stream 1: GET /index.html  ↻
      ├─ Stream 2: GET /style.css   ↻ ← in parallel
      ├─ Stream 3: GET /app.js       ↻
      └─ Stream 4: GET /logo.png    ↻

   All STREAMS multiplex over ONE TCP connection
   Each frame tagged with stream ID

✅ TRUE parallel requests (within one connection)
✅ Binary framing (efficient parsing)
✅ HPACK header compression
✅ Server push (preempt resources)
   "Browser asked for /index.html — I'll also push /style.css"
   
❌ TCP-level head-of-line blocking
   (one lost packet = all streams stall)
```

---

## ⚡ HTTP/3 — QUIC over UDP (2022 future)

```
QUIC protocol (built on UDP):

   - 0-RTT connection setup (reuse known servers)
   - TLS 1.3 built-in
   - Multiplexed streams (no TCP HoL blocking)
   - Connection migration (WiFi → cellular seamless)
   - Better mobile performance

UDP foundation:
   - No TCP handshake delay
   - QUIC adds reliability ON TOP
   - Per-stream loss handling (one lost packet
     doesn't block other streams)
```

```
HTTP/2 problem (TCP):
   Stream 1: ████████░████ (packet lost)
   Stream 2: ████ WAITING for stream 1 packet
   Stream 3: ████ WAITING ...
   = Even though only stream 1 lost, others stuck

HTTP/3 (QUIC):
   Stream 1: ████████░████ (packet lost) — wait
   Stream 2: ████████████ ← continues independently
   Stream 3: ████████████ ← continues independently
   = Per-stream loss isolation
```

---

## 📊 EVOLUTION VISUAL

```
HTTP/1.0:
   [Connect] [Request] [Response] [Close]
   [Connect] [Request] [Response] [Close]
   = Slow

HTTP/1.1:
   [Connect] [Req1] [Resp1] [Req2] [Resp2] ...
   = Persistent but sequential

HTTP/2:
   [Connect]
   ├─ [Req1] [Resp1]
   ├─ [Req2] [Resp2]   ← all parallel
   └─ [Req3] [Resp3]
   = Multiplexed but TCP HoL

HTTP/3 (QUIC):
   [0-RTT Connect]
   ├─ [Req1] [Resp1] (independent)
   ├─ [Req2] [Resp2] (independent)
   └─ [Req3] [Resp3] (independent)
   = True parallel, no HoL
```

---

## 🎯 ADOPTION 2026

```
HTTP/1.1 → Still common (legacy support)
HTTP/2   → DEFAULT for most modern sites (80%+)
HTTP/3   → Growing fast (~30%+)
           • Google, Facebook, YouTube use it
           • CloudFlare, Fastly support
           • Browsers (Chrome, Firefox, Safari) ready
```

---

## 💎 KEY INSIGHT — HOL Blocking

```
Head-of-Line Blocking:
   = One slow item blocks everything behind it

HTTP/1.1 HoL: Application-level
   Slow response → next response waits

HTTP/2 HoL: TCP-level
   Lost packet → all streams wait
   (multiplexed BUT still on same TCP)

HTTP/3: NO HoL
   QUIC over UDP handles streams independently
   Per-stream reliability
   = Real fix for HoL
```

---

## 🎤 INTERVIEW LINE

```
"HTTP/1.0 used a new connection per request — very slow.
 HTTP/1.1 added persistent connections, but requests
 sequential, head-of-line blocking at app level.

 HTTP/2 introduced multiplexing — multiple streams over
 one TCP connection, binary framing, header compression
 (HPACK), and server push. But still vulnerable to
 TCP-level head-of-line blocking.

 HTTP/3 uses QUIC over UDP — eliminates TCP HoL blocking
 with per-stream reliability, 0-RTT connection setup,
 connection migration for mobile. Mandatory TLS 1.3.

 Modern web uses HTTP/2 by default; HTTP/3 adopted by
 Google, Facebook, YouTube, growing fast."
```

---

## 🎯 Memory Hook

```
HTTP/1.0 = 📞 Call/hangup per request
HTTP/1.1 = 📞 Stay on line, sequential
HTTP/2   = 📱 Multiplexed streams (TCP)
HTTP/3   = ⚡ QUIC over UDP, no HoL

Key innovations per version:
   1.1 → Persistent connections
   2   → Multiplexing + binary + HPACK
   3   → QUIC + UDP + no HoL blocking

HoL = head-of-line blocking
TCP = HTTP/1, /2  | QUIC/UDP = HTTP/3

2026 reality:
   HTTP/2 = default
   HTTP/3 = rising fast
```

📚 [← HLD README](../README.md)
