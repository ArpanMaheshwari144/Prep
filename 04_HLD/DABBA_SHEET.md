# DABBA SHEET

Har design me wahi ~15 dabbe lautte hain. Dabba bola, to uske sawaal yahin se turant.
Design ke notes me ye sawaal dobara nahi likhte, wahan sirf dabbon ko jodna hai.

&nbsp;

## REDIS

**Kyun aaya:** DB disk pe hai, isliye dheema. Redis memory me rakhta hai, isliye tez. Jo cheez baar-baar padhi jaati hai, wo yahan.

**Ek Redis kitna jhelta:** lagbhag 1-2 lakh operation/sec.

**Bhar gaya to?**
Jo sabse der se nahi padha gaya, wo hatao (LRU). Saath me har key pe umar (TTL).

**Bheed zyada, ek Redis kam pada to?**
Tukde karo: **shard**, consistent hashing se. Copy (replica) lagane se padhna batt-ta hai, likhna nahi.

**Ek key garam ho gayi (viral) to?**
Wo ek hi machine pe baithi hai, to wahi machine dooba. Har app server use 5-10 second apni memory me rakhe: **local cache**.

**Redis gira to?**
Uski copy main ban jaati hai. Copy bhi gayi to us design ka faisla lagta hai: cache me seedha DB se padho, rate limiter me fail-open ya fail-closed.

**Key expire hui aur sab ek saath DB pe toot pade to? (stampede)**
**Lock:** ek DB jaata hai, baaki ruk ke Redis dekhte hain. Lock wala mar gaya to lock ki umar khatam hote hi agla jaata hai. Lock wala dheema hai to **soft / hard umar**, matlab purana maal de do aur peeche se taaza laao.

**DB me badla par Redis me purana pada hai to?**
Pehle DB me likho, phir Redis ki key **delete** karo. Agli padhai DB se taaza laayegi.
