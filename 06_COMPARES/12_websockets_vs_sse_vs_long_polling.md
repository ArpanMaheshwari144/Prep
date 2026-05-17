# ⚡ WebSockets vs SSE vs Long Polling

---

## 🎯 1-Line Analogy

```
Long Polling = 🚪 Repeated knocks on door
SSE          = 📻 Radio broadcast (one-way)
WebSocket    = 📞 Phone call (two-way always-on)
```

---

## 📊 Side by Side

```
┌────────────────┬───────────┬───────────┬──────────────┐
│  Feature       │ Long Poll │ SSE        │ WebSocket    │
├────────────────┼───────────┼───────────┼──────────────┤
│ Direction      │ Client→S   │ Server→C  │ Bidirectional│
├────────────────┼───────────┼───────────┼──────────────┤
│ Protocol       │ HTTP       │ HTTP       │ WS / WSS     │
├────────────────┼───────────┼───────────┼──────────────┤
│ Connection     │ Closes+    │ Persistent │ Persistent   │
│                │ reopens    │ HTTP       │ full duplex  │
├────────────────┼───────────┼───────────┼──────────────┤
│ Real-time      │ Near       │ ✅ Yes     │ ✅ Yes        │
├────────────────┼───────────┼───────────┼──────────────┤
│ Auto-reconnect │ Manual     │ ✅ Built-in│ ❌ Manual    │
├────────────────┼───────────┼───────────┼──────────────┤
│ Proxy friendly │ ✅          │ ✅         │ ⚠️ Sometimes  │
├────────────────┼───────────┼───────────┼──────────────┤
│ Complexity     │ Medium     │ Low        │ Higher        │
├────────────────┼───────────┼───────────┼──────────────┤
│ Overhead       │ High       │ Low        │ Lowest        │
└────────────────┴───────────┴───────────┴──────────────┘
```

---

## 🚪 LONG POLLING

```
CLIENT                    SERVER
   │                         │
   │ GET /messages           │
   │ ──────────────────────►│
   │                         │
   │   WAITING...             │ "No msg yet,
   │   WAITING...             │  hold connection"
   │   WAITING...             │
   │                         │
   │  ◄────── New msg!      │ Data arrived!
   │                         │
   │ GET /messages (again)   │
   │ ──────────────────────►│ Repeat...
```

```
Logic:
   1. Client requests
   2. Server holds until data OR timeout (30s)
   3. Server responds
   4. Client RE-REQUESTS immediately
   5. Repeat

= HTTP overhead each cycle
= Many connection re-opens
```

---

## 📻 SSE (Server-Sent Events)

```
CLIENT                    SERVER
   │                         │
   │ GET /events             │
   │ Accept: text/event-stream
   │ ──────────────────────►│
   │                         │
   │  ◄──── data: msg1      │
   │  ◄──── data: msg2      │
   │  ◄──── data: msg3      │ Server pushes
   │  ◄──── data: msg4      │ continuously
   │  ◄──── data: msg5      │
   │      ...                │
   │                         │
   │  (auto-reconnects)      │
```

```
Browser code:
   const events = new EventSource('/events');
   events.onmessage = (e) => console.log(e.data);

= ONE-WAY only (server → client)
= Built on HTTP (firewall friendly)
= AUTO-RECONNECT included ✨
= Lightweight
```

---

## 📞 WEBSOCKET

```
CLIENT                    SERVER
   │                         │
   │ HTTP Upgrade request    │
   │ Connection: Upgrade      │
   │ Upgrade: websocket       │
   │ ──────────────────────►│
   │                         │
   │  ◄── 101 Switching      │
   │                         │
   │  ═══ WS connected ═══   │
   │                         │
   │  ──► "Hello"             │
   │  ◄── "Hi"                │
   │  ──► "How are you"       │ Both ways
   │  ◄── "Great"             │ anytime
   │  ──► "Send file"         │
   │  ◄── "Got it"            │
```

```
Browser code:
   const ws = new WebSocket('wss://server.com');
   ws.onopen = () => ws.send('Hello');
   ws.onmessage = (e) => console.log(e.data);

= BIDIRECTIONAL real-time
= Low latency
= Custom protocol after HTTP upgrade
```

---

## 🎯 WHEN TO USE WHAT

```
✅ LONG POLLING — Use when:
   • Legacy systems / very old browsers
   • Simple "occasional update" UX
   • Fallback when WS not available
   
   Examples:
   • Pre-2010 chat apps
   • Socket.io fallback mode

✅ SSE — Use when:
   • ONE-WAY server → client push
   • Notifications, alerts
   • Stock tickers, news feeds
   • Live dashboards, log streaming
   
   Examples:
   • Twitter notifications
   • Stock price updates
   • Server logs to UI
   • Real-time dashboards

✅ WEBSOCKET — Use when:
   • TRUE bidirectional needed
   • Chat applications
   • Online multiplayer games
   • Collaborative editing (Google Docs)
   • Live trading platforms
   
   Examples:
   • WhatsApp Web
   • Slack messaging
   • Online games
   • Trading apps
```

---

## 💎 OVERHEAD COMPARE

```
LONG POLLING per message:
   HTTP request: ~500 bytes (headers, cookies, auth)
   HTTP response: ~200 bytes
   Actual data: "Hi" = 10 bytes
   TOTAL: 710 bytes per "Hi"
   Useful: 1.4%

WEBSOCKET per message:
   WS frame header: 2-14 bytes
   Actual data: "Hi" = 10 bytes
   TOTAL: ~24 bytes per "Hi"
   Useful: 40%

= WebSocket WAY more efficient for chatty apps
```

---

## 🎬 REAL-WORLD CHOICES

```
WhatsApp Web        → WebSocket (bidirectional chat)
Twitter feed        → SSE (one-way push of tweets)
GitHub Codespaces   → WebSocket (live collab editing)
Stock dashboard     → SSE for prices + WebSocket for orders
Older Gmail         → Long polling (HTTP/1.1 era)
```

---

## 🚨 WEBSOCKET GOTCHAS

```
1. SCALING
   Stateful — each WS holds memory
   Sticky sessions or shared state needed

2. PROXIES / FIREWALLS
   Some corporate proxies block WS
   Fallback to long polling needed

3. AUTH
   No headers per message
   Token in initial handshake

4. RECONNECT
   Manual — drops require reconnect logic
   Libraries handle (Socket.io, SockJS)
```

---

## 🎤 INTERVIEW LINE

```
"Long polling holds HTTP request open until data,
 then client re-requests. Near real-time but high
 HTTP overhead per cycle.

 SSE maintains open HTTP for server-to-client stream.
 One-way, auto-reconnect built-in, lightweight —
 ideal for notifications and live updates.

 WebSocket creates persistent bidirectional connection
 via HTTP upgrade. True real-time both ways, lowest
 overhead, ideal for chat, gaming, collaboration.

 Choose SSE for one-way server push, WebSocket for
 bidirectional, long polling as legacy fallback."
```

---

## 🎯 Memory Hook

```
Long Polling = 🚪 Knock-and-wait (HTTP repeats)
SSE          = 📻 Radio broadcast (server→client)
WebSocket    = 📞 Phone call (both ways)

Long Poll: legacy, simple, high overhead
SSE:       one-way push, auto-reconnect
WebSocket: bidirectional, low overhead

Real apps:
   Chat → WebSocket
   Notifications → SSE
   Old/fallback → Long Polling
```

📚 [← HLD README](../README.md)
