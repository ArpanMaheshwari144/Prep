# Banking: hand notes (bank branch)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Shabd (poore notes me yahi):** branch = Banking Service · cashier = service machine · register = DB · diary = ledger · photocopy = read replica · ladka = relay · godown = S3

&nbsp;

**1. Bijli gayi, Arpan ke 500 kate, Ravi ko nahi mile**
Mere 500 wapas de do: ya dono kaam honge, ya ek bhi nahi. Aur cashier ko yaad nahi rehta, to kaam shuru karne se pehle diary me likh lo. Wapas aaye to diary dekh ke adhoora ulta kar de.
**transaction, WAL**
Interview: "Dono UPDATE ek transaction me. Crash ke baad WAL se adhoora kaam rollback ho jaata hai."

**2. Line kati, Arpan ne wahi parchi dobara di**
Parchi pe ek number jo dobara bhejne pe badle nahi. Number pehle aa chuka, to mana. Nayi parchi ka number naya hota hai, to wo chal jaati hai.
**idempotency key**
Interview: "Send screen khulte hi Idempotency-Key banti hai. DB me UNIQUE, dobara aayi to purana jawab, paisa dobara nahi katta."

**3. 600 the, do counter pe ek saath 500-500**
Dekhna aur kaatna ek hi kaam me. Pehle wale ne kaata, to doosre ko 600 nahi, 100 dikhega.
**ek query + row lock**
Interview: "`UPDATE ... SET balance = balance - 500 WHERE balance >= 500`. Row lock se doosri query naye balance pe jaanchti hai."

**4. Ravi: "500 mile hi nahi"**
Ek alag diary, usme kaato mat, neeche likhte jao: Arpan -500, Ravi +500. Balance alag rakho, wo badalta rehta hai.
**ledger (double-entry, append-only)**
Interview: "Usi transaction me ledger ki do line. Ledger sach hai, balance bas tez padhne ki copy."

**5. Ravi ko khabar deni thi, ladka nahi aaya**
Jo yaad nahi rehta, use likh lo. Usi register me "Ravi ko khabar do", aur koi bhi ladka aaye, dekh ke pahuncha de.
**outbox + relay**
Interview: "Khabar usi transaction me outbox table me jaati hai. Relay use bhejta hai."

**6. Salary ka din, line sadak tak**
Cashier badha do, 10 counter. Jaise bank aur movie hall me hota hai.
**LB, stateless**
Interview: "Kai service machine, aage LB. Service kuch yaad nahi rakhti, sab DB me hai."

**7. Register jal gaya**
Laptop me copy rakho. Aur copy me turant likho, jab tak laptop me na aaye tab tak pakka mat maano.
**sync replica (Multi-AZ)**
Interview: "DB ki sync copy doosri AZ me. Async me aakhri transfer kho sakte, paise me nahi chalega."

**8. Sab balance dekhne aaye, likhne wale atke**
Register sirf likhne walon ke liye. Padhne walon ke liye photocopy ka alag desk.
**read replica**
Interview: "Balance aur history read replica se. Paisa kaatne ka faisla hamesha primary pe."

**9. Abhi bheja, photocopy purani dikha rahi**
Jisne abhi likha, uska thodi der main register se padho. Photocopy taaza ho jaaye, phir wahin se.
**read-your-writes**
Interview: "Transfer ke baad kuch second us user ka balance primary se, ya response me hi naya balance."

**10. Kisi ne haath se balance badal diya**
Asli sach diary hai. Balance ko diary se milao.
**raat ka milaan (reconciliation)**
Interview: "Raat ko har account ka balance ledger ke jod se milata hoon. Farak pe alert, khud theek nahi."

**11. Har raat 5 saal ki diary jodna bhaari**
Purana mahina badalta nahi. Mahine-mahine tod do, har mahine ka total likh lo.
**snapshot, partition**
Interview: "Ledger mahine ke partition me. Mahine ka snapshot + uske baad ki lines se jod."

**12. Diary itni moti ki almari me jagah nahi**
Purani diary head office ke godown me bhej do.
**S3**
Interview: "Saal purana ledger S3 me. Padhna ho to Athena se."

**13. Godown me koi purani line badal de**
Kohinoor heera: ijazat wala bhi na badal sake, taala laga do. Aur kai copy alag jagah, koi ek badle to mila ke pakad lo.
**Object Lock, versioning**
Interview: "S3 Object Lock, yaani koi badal ya mita nahi sakta. RBI ke liye 10 saal tak."

&nbsp;

Poora design = [13_banking_system.md](13_banking_system.md)
