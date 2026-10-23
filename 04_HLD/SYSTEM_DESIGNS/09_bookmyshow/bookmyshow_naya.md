# BookMyShow: shuru se, ek kahani

Kram wahi jo interview me hota hai: **FR, phir NFR, phir numbers, phir design**. Design sabse seedhe dabbe
se shuru hota hai, aur har dikkat aane pe ek naya dabba judta hai. Diagram me peela dabba = abhi juda. Poori
file me ek hi kahani hai: Arpan ko Ravi ke saath PVR ka 9 baje wala show dekhna hai, seat A1.

**Teeno ka jod (banking, payment, BMS):** andar ka dhaancha ek hi hai. Jo kadam banking / payment me pehle
se hai, wahan bas ek line "ye wahi hai"; poora sirf wahi jo BMS ka apna hai.

```
dikkat                         banking            payment             BMS
do log ek saath, ek cheez      kadam 4: balance   (banking wala)      seat A1: UPDATE ... WHERE status = FREE
                               check + kaato
network gaya, dobara bheja     kadam 3: idem key  (banking wala)      pay dobara -> wahi idempotency key
bahar ka paisa (bank / PSP)    -                  kadam 2-4: PSP,     ticket ka paisa = payment file poori
                                                  PENDING, webhook
BMS ka APNA                    -                  -                   seat HOLD 5 min (umar ke saath),
                                                                      seat map ki bheed (cache),
                                                                      concert ki line (waiting room)
```

---

## 1. FR: kya karna hai

```
1. movie / show dekhna, seat map dekhna
2. seat chuno -> pay karo -> ticket pakka
scope ke bahar: refund, review, recommendation
```

## 2. NFR: kaisa chalna chahiye

```
- ek seat kabhi do logon ko na bike (yahi dil hai)
- seat map dekhna tez (bheed yahin hoti)
- badi movie / concert ke din ki bheed jhele
```

## 3. Numbers

```
- dekhne wale bahut, book karne wale kam: 100 log map dekhte to ~1-2 book karte
  -> dekhne ke liye cache, likhne ke liye ek pakka SQL DB
- active data chhota (aaj + agle hafte ke show) -> sharding nahi chahiye, purana archive
```

---

## 4. Design

### Kadam 1: sabse seedha (BMS Svc + SQL DB, pehle padho phir likho)

BMS Svc aur ek SQL DB. `seats` table me har seat ki row: `show 9pm | A1 | FREE`. Arpan "Book A1" dabata hai.
BMS Svc pehle padhta hai "A1 FREE hai?", haan. Phir likhta hai `status = BOOKED`.

```mermaid
flowchart LR
    U["Arpan"] -- "Book A1" --> S["BMS Svc"]
    S -- "1. padho: A1 FREE?<br/>2. likho: A1 = BOOKED" --> D[("SQL DB: seats")]
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style D fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Dikkat: Ravi ne bhi usi second me A1 dabaya. Dono ne padha "FREE", dono ne likha "BOOKED". Dono ko ticket,
ek seat pe do log. Ye banking ke "do kataai ek saath, balance -400" jaisi hi dikkat hai: pehle padho, phir
likho, aur beech ke pal me koi aur ghus jaata hai.

### Kadam 2: padhna aur likhna ek hi query me (banking kadam 4 wala hi)

"Pehle padho, phir likho" do alag kaam hain, aur dono ke beech Ravi ghus gaya. Ilaaj: dono ko ek hi query me.

```sql
UPDATE seats SET status = 'BOOKED', user = 'Arpan'
WHERE show = '9pm' AND seat = 'A1' AND status = 'FREE';
```

DB is row pe lock lagata hai, to Arpan aur Ravi ki query ek ke baad ek chalti hai, saath nahi:

```
Arpan ki query pehle: A1 FREE hai -> BOOKED, user = Arpan  -> "1 row badli" -> Arpan ko seat mili
Ravi ki query baad me: A1 ab FREE nahi, WHERE match nahi  -> "0 row badli" -> Ravi: "seat chali gayi"
```

Banking me yahi tha: `UPDATE ... SET balance = balance - 500 WHERE balance >= 500`. Wahan shart "balance
hai?", yahan shart "seat FREE hai?". Baaki sab same.

Par seat book karte hi paisa nahi aata, Arpan ko pay karne me 3 minute lagte hain. Do raaste, dono kharab:
- pehle seat BOOKED, phir pay: Arpan ne pay hi nahi kiya, app band kar di, to A1 hamesha ke liye BOOKED padi
  rahegi, kisi ko nahi milegi.
- pehle pay, phir seat: 3 minute me Ravi le gaya, to Arpan ka paisa kat gaya aur seat nahi mili.
