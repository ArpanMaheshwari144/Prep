# DB 03 — Chalti table me naya column (bina dukaan band kiye)

---

## Koi poochhe to aise bolo

Badi table jispe har second laakhon log padh-likh rahe, usme naya column jodna hai bina roke. Purane DB me `ALTER TABLE ADD
COLUMN` poori table pe taala laga ke use dobara likhta tha, utni der sab ruk jaata. Aaj MySQL 8 / Postgres 11+ me **khaali
(nullable) column** jodna turant hota: sirf table ka naqsha badalta, data dobara nahi likha jaata. Column pakka (NOT NULL) aur
bhara chahiye to teen kadam: khaali column jodo -> purani rows chhote batch me bharo -> sab bhar jaaye tab NOT NULL.

---

## Kahani: chalti dukaan me naya shelf

```
Purana tareeka:  shutter gira ke poora maal naye rack me shift -> grahak bahar khade (downtime)
Khaali column:   diwaar pe khaali shelf tang di, kuch hila hi nahi (instant)
Bhara chahiye:   shelf tango -> raat ko thoda-thoda maal rakho (batch) -> sab bhar gaya to board "yahan hamesha maal" (NOT NULL)
Bahut badi table: bagal me naya godown (shadow table), maal dheere shift, nayi aamad dono jagah, ant me board badal do
```

---

## Do raaste

**Raasta 1 (zyaadatar): usi table me naya column**
```
1. usi table me khaali column (turant)
2. ab se jo naya aaya, app naye column me bhi likhti
3. purani rows peeche se query se bharte gaye, chhote batch me   (backfill)
4. sab bhar gaya to NOT NULL
```

**Raasta 2 (bahut badi table / purana DB): nayi table**
```
1. bagal me nayi table (purani + naya column)
2. purana data peeche se batch me copy
3. is beech jo naya aa raha wo dono jagah likha jaata (tool khud karta: gh-ost / pt-osc)
4. sab barabar to ek pal me naam badal do: nayi table hi ab asli
```

Dono me "peeche se dheere-dheere bharna" wahi baat; farak itna ki raasta 1 me sab usi table me, raasta 2 me bagal ki nayi
table ant me purani ki jagah leti.

---

## Dikkat se hal

Ek jhatke me NOT NULL + default wala column jodo to purane DB me poori table dobara likhi jaati, taala lagta. Hal: khaali
jodo, batch me bharo, phir shart (expand-contract).

---

## Trap

Purani rows bharne ke liye ek hi bada `UPDATE users SET city = ...` chala diya: laakhon rows pe ek saath taala (DB 02 wala),
replica peeche, app ruki. Bharna hamesha 1-10 hazaar rows ke batch me.
