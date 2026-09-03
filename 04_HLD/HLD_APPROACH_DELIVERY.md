# HLD — APPROACH / DELIVERY PLAYBOOK ("kaise bolna", content nahi)

> Ye note DESIGN ka content nahi — ye HLD interview me KAISE conduct karna hai, wahi.
> Banaya 3-Sep: Arpan ne pehli baar HLD-mock kiya + bola "padh liya par bolna kaise nahi aata".
> HLD = khula game (DSA binary + Java deterministic se ALAG). Par khula ≠ blank — hamesha RAIL hoti hai.

---

## 0 — 3 rounds ka farak (kyun HLD alag lagta)
```
   DSA     = BINARY      (library me pattern hai -> ho gaya; warna nahi)
   Java    = DETERMINISTIC (fixed sawaal -> fixed jawab)
   HLD     = OPEN GAME   (koi answer-key nahi; interviewer SAATH drive karta)
```
> HLD grade karta = SOCH + COMMUNICATION + TRADE-OFF navigate + ambiguity me aage badhna.
> "Perfect complete answer" grade karta hi NAHI. Open isliye = wo dekhna chahte tu ANJAAN me kaise chalega.

---

## 1 — THE RAIL (khula game me kabhi blank se shuru nahi; isi 7-step pe chal)
```
   Requirements  →  Estimate  →  API  →  Data model  →  HL boxes  →  Deep-dive  →  Bottleneck
```
- Ye rail HI "approach" hai. Kisi bhi design me isi kram pe bolo.
- Har box pe: KYA (component) + KYUN (1 line reason). Bas. "Redis cache — read-heavy, DB bachana."
- Tu YE LEAD karta — interviewer wait nahi karta.

> ★ ESTIMATE pe MAT ATKO (Hello-Interview + cracked-round comments): ek QUICK estimate
>   karo (scale justify — "read-heavy, billions -> cache+shard") phir aage. Exact number
>   (1500 vs 2000 RPS, storage 3TB vs 90TB) design nahi badalta -> us math pe waqt/energy mat do.
>   PAR bilkul SKIP bhi mat karo -> ek banda Zomato me reject hua kyunki BOTE skip karke
>   seedha distributed chala gaya. Balance = 30-sec estimate, phir move on.

---

## 2 — 4 META-MOVES (open-game ke asli sawaalon ke jawab)

### (a) "Kahan se start?"
```
   Tu drive karta: requirements clarify + 2 clarifying Q
   ("custom URL chahiye? links expire hote? scale kitna?")
   -> chup mat baitho, tu lead karta hai.
```

### (b) "Kya-kaise-kyun bolna?"
```
   Rail ke har box pe -> naam + EK why.
   Zyada nahi. Ek line why har component pe = seniority-signal.
```

### (c) "Trade-off kaise bolna?"  (ek fixed SAANCHA)
```
   "X vs Y — X deta [fayda] par costs [nuksan]; main X lunga kyunki [requirement]."

   e.g. "SQL vs Cassandra — SQL simple par 180B rows pe scale nahi;
         Cassandra lunga kyunki key-value + horizontal scale + HA."
   -> har trade-off isi saanche me. Rat lo ye pattern.
```

### (d) "Answer NAHI pata to kaise nikaalun?"  (SABSE IMPORTANT — noob-fear yahi)
```
   RULE: kabhi FREEZE / chup NAHI. 4 escape:

   1. REASON ALOUD   -> "Main isse aise sochunga..." (first-principles derive)
   2. ASSUME + MOVE  -> "Main maan leta X, aage badhta hoon."
   3. CLARIFYING Q   -> "Scale kitna maanu?" (waqt bhi milta)
   4. HONEST+APPROACH-> "Ye use nahi kiya, par aise approach karunga..."

   Interviewer YAHI test karta -> atakne pe kaise NAVIGATE karta.
   Wo "sab pata" nahi dekhta; wo "ambiguity handle" dekhta.
```

---

## 3 — META-SACH (darr todne ke liye)
```
   - Perfect answer koi nahi deta -> tu bhi kabhi nahi dega, na chahiye.
   - Nervousness + kuch point chhoot-na = NORMAL, fail nahi.
   - HLD = DIALOGUE (monologue nahi) -> interviewer nudge karta, tu respond.
   - Ye TRAINABLE performance-skill hai (English-delivery jaisa) -> mock-reps se aata,
     na ki "aata/nahi-aata" knowledge.
   - Pehla HLD kisi ka smooth nahi. Pehla interview = "pehla rep", "final" nahi.
```

---

## 4 — MOCK me main (Claude) kya coach karunga
```
   Answer "poora hai ki nahi" NAHI dekhunga.
   Dekhunga: rail pe chala? · har box why bola? · trade-off saanche me bola?
             · atakne pe 4-move se navigate kiya? · pichhli-baar-se behtar?
   -> laundry-list/quiz/gatekeeper nahi. Coaching, na test.
```

---

## 5 — ANJAAN DESIGN + "kuch nahi aata" ka darr (3-Sep, Arpan ka #1 fear)

### (A) Design jo padha NAHI (Google Docs / Search jaisa)
```
   Designs YAAD nahi karte -> wo same ~15 BLOCKS ke naye COMBINATION hain.
   Blocks: LB · cache · DB · queue · shard · replica · CDN · index · consistency-choice.
   15+ design padhe = ye blocks bahut baar milte dekhe -> naya = wahi blocks nayi jodni.

   Anjaan design -> RAIL pe chal ke DERIVE kar (recall nahi):
     e.g. Google Docs -> "real-time collab edit -> concurrent edits clash ->
          I'd order/sequence operations per doc." (naam OT/CRDT na aaye tab bhi REASON)
   Interviewer JAAN-BOOJH ke anjaan deta -> framework-apply dekhna, ratta nahi.
   JP-backend me Google-scale nahi -> rate-limiter/payment/notification/url type = tere paas.
```

### (B) "Kuch aata nahi -> nerves -> poora interview bekar" — us case me:
```
   1. Kuch na aana GUARANTEED + EXPECTED. Har candidate ko 1-3 cheez nahi aati.
      Grade = "na-aane pe kaise handle kiya", na ki "sab aata tha".
   2. Ek unknown se interview bekar NAHI. Poora round grade hota; JP = 2/3 round tera zone.
   3. Asli khatra = unknown nahi, SPIRAL hai ("fail ho raha -> panic -> baaki bikhra").
      Skill = unknown ko spiral se ALAG karna.
   4. FIX = pehle se ratti-hui line (rehearse -> automatic -> trigger hi na bane):
      > "I haven't worked with X directly, but I'd approach it by [jo aata usse reason].
         Let me note it and continue."
   5. "Nahi aata par aise nikaalunga" = POSITIVE signal -> bluff se zyada respect.
   6. ANCHOR: 4 saal + 700 prod-tickets -> "2am prod down, pata nahi kya toota" tune
      baar-baar dabaav me navigate kiya. Interview-unknown usse AASAAN. Tu mushkil kar chuka.
```
> NICHOD: sab nahi jaanega = normal, fail nahi. Interview banata/bigaadta = tera RESPONSE, na unknown.
> Pehla interview = warm-up rep, "final" nahi.

---

> 1-line recall: **RAIL pakdo (7-step) → har box naam+why → trade-off saanche me → atko to 4-move (kabhi chup nahi). Perfect nahi, NAVIGATE karna hai. Anjaan = blocks se DERIVE. "Nahi aata" = ratti-line + reason, spiral se alag.**
