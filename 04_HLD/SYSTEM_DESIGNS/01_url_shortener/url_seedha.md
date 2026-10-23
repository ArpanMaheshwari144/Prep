# URL shortener: shuru se, ek kahani

Kram video jaisa: **FR -> NFR -> numbers -> design**. Design sabse seedhe dabbe se shuru,
har dikkat pe ek dabba judta. Peela = abhi juda. Kahani ek hi: Arpan ka Amazon link.

---

## 1. FR: kya karna hai

Interviewer se pehle poochho, phir scope tay karo:

```
1. lamba link do     -> 7 akshar ka chhota code milta hai (short.ly/aB3x9)
2. chhota link kholo -> asli lambe link pe bhej do (redirect)
scope ke bahar (poochh ke hatao): apna naam wala code, kitni baar khula ki ginti
```

## 2. NFR: kaisa chalna chahiye

```
- kholna TEZ ho        : click pe turant Amazon khule
- hamesha chale        : link toota to Arpan ka bheja link bekaar
- ek code = ek link    : do logon ko kabhi same code na mile
```

## 3. Numbers: sirf wo jo design badlein

```
- roz 10 crore naye link        -> ~1,000 / sec banana
- har link ~100 baar khulta     -> ~1 lakh / sec kholna
- ek link ~500 byte, 5 saal     -> ~180 arab link -> ~90 TB
- 7 akshar, 62 tarah ke         -> ~3.5 lakh crore code, kaafi
```

Kholna banane se 100 guna, isliye aage cache aayega.
90 TB ek machine me nahi aata, isliye aage DB ke tukde (shard).
62 tarah = a-z (26) + A-Z (26) + 0-9 (10).

---

## 4. Design

### Kadam 1: sabse seedha (ek server, ek DB)

Arpan ke paas lamba link hai (`amazon.in/dp/B0C...?ref=xyz`). Wo hamari site pe daalta hai,
use `short.ly/aB3x9` milta hai. Koi bhi chhota link khole, hum use Amazon pe bhej dete hain.

```mermaid
flowchart LR
    U["Arpan"] --> S["App server"]
    S --> D[("DB<br/>aB3x9 -> amazon.in/dp/B0C...")]
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style D fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

```
BANAO:  lamba link aaya -> chhota code banao (aB3x9) -> DB me dono saath likho
KHOLO:  aB3x9 aaya      -> DB me dhoondho -> browser ko bolo "is lambe link pe jao"
                           (redirect, 302)
```

DB kyun, server ki memory (Java HashMap) kyun nahi? Memory me rakhte to server restart hote hi
saare link gayab. Link saalon chalna chahiye, isliye disk wala DB.

Par chhota code `aB3x9` banega kaise, aur do logon ko same code na mile ye kaise pakka?

### Kadam 2: code banana (counter + base62)

Teen tareeke ho sakte the:

```
(a) RANDOM:   7 akshar ulte-seedhe chuno (aB3x9Qz)
(b) HASH:     lambe link ka MD5 nikaalo, uske pehle 7 akshar lo
(c) COUNTER:  har naye link ko ek badhta number do (1, 2, 3 ...),
              us number ko 62 akshar me likh do (base62)
```

```
(a) random  -> do logon ko same code mil sakta. Har baar DB me dekho "pehle se hai?",
               mila to dobara.
               Link badhte gaye to takkar bhi badhti
(b) hash    -> do alag link ke pehle 7 akshar same aa sakte. Wahi DB check, wahi dobara
(c) counter -> har number naya, to code kabhi same nahi. DB check hi nahi chahiye. ISLIYE YE
```

Base62 = jaise 10 ko binary me `1010` likhte, waise number ko 62 akshar (a-z, A-Z, 0-9) me likho.
1 arab bhi sirf 6 akshar me. 7 akshar me ~3.5 lakh crore number.

```mermaid
flowchart LR
    U["Arpan"] --> S["App server<br/>counter: agla number<br/>-> base62 -> aB3x9"]
    S --> D[("DB<br/>aB3x9 -> amazon.in/...")]
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Abhi ek hi server, to counter bhi ek (DB ka apna auto-increment id bhi chal jaata).

Ab link ban rahe hain. Par har second 1 lakh log link khol rahe hain, aur har baar DB me
dhoondhna pad raha hai. DB itna bojh nahi jhel paayega.
