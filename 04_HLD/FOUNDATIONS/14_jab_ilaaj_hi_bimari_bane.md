# Jab ILAAJ hi BIMARI ban jaaye — cascading / metastable failure

> HUB file: LB, cache, queue, retry, limiter ki ek hi shakal. Har ek ka poora detail apni file me (neeche table).

---

## Koi poochhe "X laga diya, phir bhi system gira, kyun?" to aise bolo

Jo cheez bachane ke liye lagayi (LB, cache, queue, retry), uski apni kharabi bhi hoti hai. Load hatta nahi, khisakta hai:
LB ne baanta, cache ne DB se Redis pe daala, queue ne abhi se baad pe khiskaya. Isliye har dabbe ke baad do sawaal:
"ye khud kharab hua to?" aur "ye wapas aaya to us waqt ki bheed kya karegi?"

---

## Arpan ki baat (25-Nov)

Dikkat hi ilaaj hai aur ilaaj hi dikkat. Kafka ilaaj ke liye lagaya, wo dikkat ban jaaye to bhi sahi; aur ye Kafka lagaye
bina bhi ho sakta hai, lagane ke baad bhi. Dikkat har jagah ho sakti hai. Redis gira to saari request DB pe, DB ki maa chhoot
gayi. Par Redis gira to jawab pehle se hai: copy (replica), aur DB pe ek saath itni request jaane mat do, system aise down
nahi hota. LB gira, Kafka gira = shayad koi nahi poochega (copy hai). Jo poocha ja sakta: worker gira, peeche dher (backlog)
jama ho gaya, wapas aaya to kya karoge.

---

## Kahani, apne hi design se

```
cache   garam kitaab ki parchi expire hui, 1000 log ek saath godown bhaage, DB gira (stampede)
          hal: ek hi aadmi godown jaaye, baaki ruke                              -> 04 caching hissa 10
retry   server dheema hua, sabne dobara bheja, load 3 guna, theek us waqt jab doob raha tha
          hal: backoff + jitter (thoda ruk ke, sab alag-alag waqt pe)           -> cache 7d / URL 11c
queue   worker 10 min band, wapas aaya to jama dher poori raftaar se DB pe de maara
          hal: wapas aane pe raftaar dheere-dheere badhao                       -> 07 MQ "backlog ka ulta spike"
limiter guard ka Redis gira, khula chhoda to rokne wala hi gayab
          hal: dono khule, har gateway apni memory se 5-5                       -> rate limiter kadam 6
```

---

## Do naam

```
CASCADING    ek gira, uska bojh doosre pe gaya, wo bhi gira (3 server, ek gira, baaki 2 pe bojh, wo bhi gaye)
METASTABLE   bheed hat gayi tab bhi system khud theek nahi hota, kyunki retry ka jama dher hi ab naya bojh
             -> dher phenko, zabardasti reset; "ruk jaate hain, theek ho jaayega" yahan nahi chalta
```

---

## Trap

Sirf "Redis gira to?" mat socho. DHEEMA hona girne se zyada khatarnak: gira to turant pata chalta, dheema thread pakad ke
baith jaata aur sab ruk jaata. Ilaaj: har bahar ki call pe timeout.

---

## Poora detail kahan

| kahan | section | file |
|---|---|---|
| Cache / Redis | 10. Counter pe kitaab nahi mili: teen tarah ki bheed (cache miss) | [04_caching.md](04_caching.md) |
| Queue / Kafka | Queue sahi jagah lagi, chal bhi rahi, aur usi ne system gira diya | [07_message_queues.md](07_message_queues.md) |
| Rate limiter | kadam 6: guard ka Redis gira | [RATE_LIMITER_HAND_NOTES.md](../SYSTEM_DESIGNS/02_rate_limiter/RATE_LIMITER_HAND_NOTES.md) |
| CAP / consistency | CHHE TARIKE JINSE CONSISTENCY KI CHOICE KHUD SYSTEM TODTI HAI | [08_cap_theorem.md](08_cap_theorem.md) |
| SPOF / copy | CHHE TARIKE JINSE REDUNDANCY KHUD FAIL HOTI HAI | [11_reliability_spof_cloud.md](11_reliability_spof_cloud.md) |
| Load balancer | health check, retry (poori file) | [03_load_balancing.md](03_load_balancing.md) |

---

## Asli kahaniyan kahan padho (postmortem)

Company me kuch toota, kyun toota, kaise theek kiya, aage kya badla = postmortem. Ek jagah sab:
```
github.com/danluu/post-mortems   saari companies ke postmortem, kism ke hisaab se (config galti, DB, time ...)
postmortems.app                  isi list ka saaf roop, category ke filter ke saath
```
Tareeka: ek line padho, jo design se jude wahi kholo, sirf teen cheez: kya toota, kyun, ab kya badla.

---

[← FOUNDATIONS](.) · [← Home README](../../README.md)
