# TCP vs UDP

---

## 1-Line Analogy

```
TCP = Phone call    (handshake, "hello?" "yes hi", ordered)
UDP = Postcard      (fire & forget, hope it arrives)
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  TCP              │  UDP              │
├──────────────────┼──────────────────┼──────────────────┤
│ Connection       │ Yes (handshake)│ No (fire & forget)│
│                  │ SYN→SYN-ACK→ACK   │ Just send         │
├──────────────────┼──────────────────┼──────────────────┤
│ Reliability      │ Guaranteed      │ Best-effort     │
│                  │ Lost? Retransmit  │ Lost? Tough luck  │
├──────────────────┼──────────────────┼──────────────────┤
│ Ordering         │ In-order        │ Out of order OK │
│                  │ Sequence numbers  │ Packets independent│
├──────────────────┼──────────────────┼──────────────────┤
│ Speed            │ Slower         │ Fast            │
│                  │ Overhead high     │ Minimal overhead  │
├──────────────────┼──────────────────┼──────────────────┤
│ Header size      │ 20 bytes          │ 8 bytes           │
├──────────────────┼──────────────────┼──────────────────┤
│ Flow control     │ Yes (windowing)│ No              │
│ Congestion ctrl  │ Yes             │ No              │
├──────────────────┼──────────────────┼──────────────────┤
│ Use case         │ Accuracy > speed  │ Speed > accuracy  │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## TCP 3-Way Handshake

```
   CLIENT                      SERVER
      │                          │
      │  ──── SYN ────────────►│  "Can we talk?"
      │                          │
      │  ◄─── SYN-ACK ──────── │  "Yes, can you?"
      │                          │
      │  ──── ACK ────────────►│  "Yes, let's go"
      │                          │
      │  ===== CONNECTION =====│
      │                          │
      │  Data flows reliably    │
```

```
UDP version:
   CLIENT ────► SERVER  (data)
   CLIENT ────► SERVER  (data)
   CLIENT ────► SERVER  (data)
   = No handshake, no ACKs, blast packets
```

---

## When to Use What

```
TCP USE WHEN:
   • Data MUST arrive correctly
   • Order matters
   • File transfer, web pages, email

   Examples:
   • HTTP / HTTPS — web browsing
   • SSH — remote login
   • SMTP — email sending
   • FTP — file transfer
   • Database connections (MySQL, etc.)

UDP USE WHEN:
   • Speed > perfection
   • Loss tolerable
   • Real-time matters more than accuracy

   Examples:
   • Live video streaming (Twitch, sports)
   • Online gaming (Counter-Strike, Fortnite)
   • VoIP (Zoom, WhatsApp call audio)
   • DNS queries (small, fast)
   • IoT sensors (frequent small updates)
```

---

## REAL-WORLD INSIGHT

```
Video streaming case:

LIVE STREAM (Twitch, sports):
   UDP — late frame useless, drop it
   Better stutter than 5-sec delay
   
ON-DEMAND (Netflix):
   TCP — buffer ahead, no rush
   Every frame matters for quality

= Same domain, different choice based on REAL-TIME need
```

---

## HTTP/3 PLOT TWIST

```
Historically: HTTP runs on TCP

But HTTP/3 = QUIC over UDP
   • UDP foundation
   • QUIC adds reliability on top
   • Faster connection setup
   • Better mobile network handoff

= UDP is making a COMEBACK
= Google, Facebook, YouTube use it
```

---

## INTERVIEW LINE

```
"TCP guarantees delivery, ordering, and flow control —
 use it when correctness matters.

 UDP just sends packets — use it when speed matters
 more than reliability.

 TCP for web, email, file transfer.
 UDP for video calls, gaming, DNS, live streaming.

 HTTP/3 uses QUIC over UDP — combining UDP's speed
 with custom reliability."
```

---

## Memory Hook

```
TCP = Phone call (handshake, reliable)
UDP = Postcard   (fire & forget)

TCP = "Confirm receive" → ACK-based
UDP = "Just shout it"   → no confirm

TCP = HTTP, SSH, email
UDP = Gaming, VoIP, DNS, live streaming
```

[← HLD README](../README.md)
