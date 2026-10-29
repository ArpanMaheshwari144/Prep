# DABBA SHEET

Har dabbe ke sirf 2-3 SACH. Sawaal aur jawab ratne nahi, sach se khud nikalne hain:
"ye sach hai -> to kya toot sakta? -> to kya karun?"

&nbsp;

## REDIS

**Sach:**
1. Data **memory** me rakhta hai.
2. Wo **ek machine** hai.
3. App se **network ke paar** baitha hai.

**Sach se nikle (khud check karne ke liye, ratne ke liye nahi):**
- Memory chhoti -> bhar jaayega -> purana hatao (LRU).
- Memory -> restart pe gayab -> asli data DB me, Redis sirf copy.
- Ek machine -> gir sakti -> copy rakho.
- Ek machine -> kitna jhelegi? (lagbhag 1-2 lakh operation/sec) -> kam pade to baanto (shard).
- Ek key ek machine pe -> sab usi ko maangein to wahi dooba (garam key) -> app ke paas hi rakh lo (local cache).
- Copy hai, asli DB me -> dono me farak -> DB badla to Redis ki key hatao.
- Key ki umar khatam -> sab DB pe (bheed) -> ek ko bhejo, baaki ruko (lock).
