# Transaction Internals (MySQL InnoDB)

## Scene: prod ticket, "order 101 cancel karna hai"

Workbench me:

```sql
START TRANSACTION;
UPDATE orders SET status = 'CANCELLED' WHERE id = 101;
-- COMMIT abhi nahi kiya, chai peene chala gaya
```

**1. Teammate ko kya dikhta hai?**
Usi waqt teammate apne laptop se `SELECT status FROM orders WHERE id = 101` chalata hai. Use **PLACED** dikhta hai, CANCELLED nahi. Tera
badlaav abhi pakka nahi hua, isliye sirf tujhe dikh raha hai.
Kaise: UPDATE karne se pehle MySQL ne purani value (PLACED) ek alag jagah rakh li thi. Isi ko **undo log** kehte hain. Baaki sab ko wahi
purani value dikhti hai.

**2. Teammate bhi wahi order badle to?**
Teammate `UPDATE orders SET status = 'SHIPPED' WHERE id = 101` chalata hai. Uski query atak jaati hai, aur 50 second (default) baad error
aata hai: `Lock wait timeout exceeded`.
Kyun: tune order 101 pe **taala (row lock)** laga rakha hai. Jab tak tu COMMIT ya ROLLBACK nahi karta, us row ko koi aur nahi badal sakta.
(9-Nov tere MySQL pe do session chala ke dekha, yahi error aaya.)

**3. Chai se aa ke dikha ki galat order tha.**
`ROLLBACK` chalaya. MySQL undo log se PLACED wapas rakh deta hai aur taala khol deta hai. Aisa lagta hai jaise kuch hua hi nahi tha.

**4. Sahi order pe COMMIT kiya, aur usi second DB server band ho gaya.**
Wapas chalu hone pe bhi order CANCELLED hi milta hai. Wajah: COMMIT pe MySQL nayi value pehle ek log file me disk pe likh deta hai. Isi ko
**redo log** (WAL, write-ahead log) kehte hain. Server wapas aaya to usne wo log padh ke badlaav dobara chala diya.

## Yaad rakhne ki cheez

**undo = purani value** (rollback ke liye, aur doosron ko purana dikhane ke liye). **redo = nayi value** (crash ke baad wapas laane ke liye).

InnoDB = MySQL ka engine jo data disk pe rakhta hai aur transaction, row lock aur crash recovery sambhalta hai (MySQL 5.5 se default, AWS RDS
MySQL me bhi yahi).

## Prod wala trap

Asli zindagi me yahi "COMMIT bhool ke chai" sabse bada khatra hai. Jab tak transaction khula hai, taala laga hai. App ki saari requests jo
order 101 chhoona chahti hain, line me rukti hain. Connection atakte hain aur Hikari pool khaali ho jaata hai. Upar se agar WHERE wale column
pe index na ho, to MySQL scan ki hui saari rows pe taala laga deta hai, lagbhag poori table pe. HikariCP wala din yahi tha. Isliye prod pe
transaction kholo to kaam karke turant COMMIT ya ROLLBACK karo.
