# Merge vs Rebase — do alag tareeke branch ko wapas main me laane ke

> Dono ka KAAM ek hi: mera branch-ka-kaam + main ka naya-kaam, ek jagah laana.
> FARAK sirf KAISE jodta. Merge = junction bana ke jodo. Rebase = apna kaam upar dobara likho.

---

## Setup (dikkat kaise banti)
- `main` = sabki common "final/saaf" copy.
- Naya feature seedha main pe karo to aadha-adhoora kaam sabki copy kharab karega.
- Isliye alag **branch** (`feature`) = apna alag kona. Wahan aaram se banao, main safe.
- End me us kaam ko **wapas main me laana** padta -> yahin merge/rebase aata.

**Asli dikkat = "do raaste alag ho gaye":**
```
main:      A --- B --- C --- D        <- jab tum busy the, kisi ne main aage badha diya (D)
                        \
feature:                 E --- F       <- tum abhi bhi PURANE C pe atke ho
```
Tumhara (E,F) aur main ka naya (D) — ab ek me laane hain.

---

## MERGE — junction bana ke jodo
Kuch tod-fod nahi. Dono raaste ke aakhri point (D aur F) ko pakad ke, ek NAYA
"jodne-wala" commit **M** banata jo dono ka beta hai:
```
main:      A --- B --- C --- D
                        \       \
feature:                 E --- F --- M      <- M = junction (yahan dono mile)
```
- E, F **jaise the waise** (chhue tak nahi). D bhi waisa. Bas ek naya M upar.
- History me `\ /` wala **kaanta/junction** ban jaata = merge ki nishani.

**Kyun:** SACH dikhata — "haan do log alag chal rahe the, yahan mile." Kuch chhupata/todta
nahi -> **SAFE**. Isliye shared/team branch pe yahi.

---

## REBASE — apna kaam D ke upar DOBARA likho
Junction nahi. Rebase dikhata jaise tumne C se nahi, **seedha D ke baad** kaam shuru kiya tha.
E, F ko utha ke D ke aage dobara likh deta:
```
main:      A --- B --- C --- D
                              \
feature:                       E --- F      <- ab seedhi line: A-B-C-D-E-F
```
- Koi junction/kaanta nahi. Ekdum **seedhi line**, jaise sab kramwar hua.
- **Catch:** purane E, F **phaad ke naye banaye** jaate (naya hash). Agar woh pehle se kisi
  aur ke paas the (pushed/shared) -> unki copy vs teri copy alag -> **gadbad**.
  Isliye shared branch pe **KABHI rebase nahi**.

---

## ★ Rebase ki zaroorat KYUN padi? (asli reason — SCALE)
Ek M junction koi dikkat nahi (nuclear bomb nahi). Problem bade project pe: 5 log, 2 saal,
hazaar commit — har koi merge kare to history SPAGHETTI:
```
main:  A─B─C─D────M1────M2──────M3────M4───...
            \    /  \   /  \    /  \  /
             E─F     G─H    I─J     K─L      <- 40 aisi bubble ek-doosre ko cross
```
Do asli dard:
1. **Bug dhoondhna:** kaunse commit ne toda? Seedhi line pe ek-ek peeche jaana easy;
   is jaal me bhool-bhulaiya -> debugging mushkil.
2. **History padhna:** aadhe commit sirf "Merge branch..." bekaar plumbing -> shor zyada, kaam kam.

-> Isliye rebase: haan EXTRA kaam (E,F dobara likhna) ABHI karta, taaki BAAD me history
seedhi-saaf rahe -> bug dhoondhna + padhna aasan. **Trade: aaj thoda extra, kal debugging easy.**

---

## ★ DECISION — kab kaunsa
| Situation | Use | Kyun |
|---|---|---|
| Branch SHARED (team/pushed) | **MERGE** | hash nahi badalta, kisi ki copy nahi tootti — safe |
| Chhota / solo kaam, ek junction | **MERGE** | junction se koi farak nahi, seedha-simple |
| Apni LOCAL akeli branch, PR se pehle safai | **REBASE** | seedhi line, clean history |
| Bade project me clean linear history chahiye | **REBASE** (policy) | debugging + padhna easy at scale |

**Golden rule:** SHARED/pushed branch pe **kabhi rebase mat karo.** Local safai = rebase theek.
Doubt ho to -> **MERGE** (hamesha safe).

---

## Commands (asli kaam bas 2 line)
```bash
git checkout feature      # apni branch pe aa jao
git merge main            # main ka naya kaam yahan jodo (junction M banega)
```
Rebase ka:
```bash
git checkout feature
git rebase main           # E,F ko D ke upar dobara likhega (seedhi line)
```
Merge me agar dono ne SAME line badli -> "CONFLICT":
```bash
# file kholo -> git ne >>>>  ====  <<<< markers daale honge
# haath se theek karo (kya rakhna), markers hata do
git add <file>            # theek kiya
git commit                # merge complete
```

---

## 1-line recall
Merge = junction bana ke jodo (E,F untouched, +1 M commit, SAFE, shared-branch pe yahi).
Rebase = kaam D ke upar dobara likho (seedhi line, purane phaadta, SOLO-local pe hi).
Doubt -> merge. Rebase ka faayda sirf SCALE pe (debugging/padhna easy).
